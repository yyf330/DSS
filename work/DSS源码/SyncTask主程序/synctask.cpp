#include <string>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <dirent.h>
#include "task.h"
#include "taskqueue.h"
#include "threadstate.h"
#include "threadstatecollection.h"
#include "commandparser.h"
#include "socketex.h"
#include "smsmanager.h"
#include "alarmmanager.h"

/************************************* 常量定义 *********************************/
#define DEFAULT_SCAN_AGENT_STATUS_INTERVAL          30          // 秒
#define DEFAULT_CHECK_DAEMON_STATUS_INTERVAL		60			// 秒
#define APP_VERSION_INFORMATION                     "1.0.1"
#define APP_RELEASE_TIME                            "2016-12-12"
#define APP_OS_SUPPORT                              "Linux/AIX"

/************************************* 函数声明 *********************************/
void * RefreshTaskThread(void *arg);		//刷新任务线程
void * CleanThread(void *arg);
void * ListenMonitorThread(void *arg);		//侦听监控信息线程
void * ListenOperationThread(void *arg);	//侦听操作线程
void * ListenInternalUdpThread(void *arg);	//侦听内部线程

void * QueryAgentStatusThread(void *arg);
void * QueryDBMirrorStatusThread(void *arg);

void * SendAlarmThread(void *arg);
void * MonitorDaemonThread(void *arg);	//监视Daemon是否运行
void * HBHandleThread(void *arg);		//根据心跳状态处理报警
void * SelfCheckThread(void *arg);

bool CreateThreads();
void StopThreads();

void InitSourceDBMap();
void InitStrategyMap();

void CatchInterrupt(int sig);
bool SelfCheck();
bool CheckLicense();
void BaseInit();
void ShowLicense();					//显示授权信息
void PrintVersion();				//打印版本信息
void ShowBasicInfo();				//显示基本信息
bool ParseOption(int argc, char** argv);

/*
 *  功能：
 *      处理客户端操作
 *  参数
 *      sock                :   套接字对象
 */
void HandleClientOperation(SocketEx *sock);

bool HandleMonitorData(string monitordata);

/************************************* 私有变量 *********************************/
ThreadStateCollection MyThreadStateCol; // 线程状态集合
bool MyNeedToExitFlag = false; // 需要退出标志

DbDataManager *MyDbDataManager = NULL;
BeCommand *MyCommandManager = NULL;

int ZwgDebugLevel = 2;
int MyCurrentListenPort = 0;
string ExecutePath = "/home/dss/";
string LicenseType = "LICENSE";
int ExtractorAutostartInterval = 60;
int LoaderAutostartInterval = 60;
int AutoClean = 0;

map<string, time_t> SourceDBMap;		// 源库集合(源库ID,挖取进程最近一次发送状态的时间)
map<string, time_t> StrategyMap;		// 策略集合(策略ID,装载进程最近一次发送状态的时间)

/*------------------------------------ 入口函数 --------------------------------*/
int main(int argc, char** argv)
{
    BaseInit();

    // 解析选项
    if (ParseOption(argc, argv) == true)
    {
        if (MyDbDataManager != NULL) delete MyDbDataManager;
        return (EXIT_SUCCESS);
    }

    // 自检
    if (SelfCheck() == false)
    {
        return (EXIT_FAILURE);
    }

	if (CheckLicense() == false) return (EXIT_FAILURE);

    signal(SIGINT, CatchInterrupt);

	//对所有需要自启的挖取，创建001任务
	sql::ResultSet * SourceDBResultSet = MyDbDataManager->GetAutostartSourceDBResultSet();
	if (SourceDBResultSet != NULL && SourceDBResultSet->rowsCount() > 0)
	{
		while (SourceDBResultSet->next())
		{
			string SourceID = SourceDBResultSet->getString("SID");
			MyDbDataManager->AddTask(DEFAULT_COMMAND_START_EXTRACTOR, SourceID+string(";"));
			BesLog::DebugPrint("add autostart capture task", BesLog::LOG_MESSAGE_TYPE_INFO);
		}
	}
	MyDbDataManager->DestroyResultSet(SourceDBResultSet);

	//对所有需要自启的装载，创建003任务
	sql::ResultSet * StrategyResultSet = MyDbDataManager->GetAutostartStrategyResultSet();
	if (StrategyResultSet != NULL && StrategyResultSet->rowsCount() > 0)
	{
		while (StrategyResultSet->next())
		{
			string StrategyID = StrategyResultSet->getString("STRATEGY_ID");
			MyDbDataManager->AddTask(DEFAULT_COMMAND_START_LOADER, StrategyID+string(";"));
			BesLog::DebugPrint("add autostart loader task", BesLog::LOG_MESSAGE_TYPE_INFO);
		}
	}
	MyDbDataManager->DestroyResultSet(StrategyResultSet);

	InitSourceDBMap();
	InitStrategyMap();

    // 添加到线程集合
    MyThreadStateCol.Add(ThreadState("RefreshTaskThread", RefreshTaskThread));		//刷新任务    启动抓取、停止抓取、启动装载、停止装载
	if(AutoClean == 1) MyThreadStateCol.Add(ThreadState("CleanThread", CleanThread));	//清理
	MyThreadStateCol.Add(ThreadState("ListenMonitorThread", ListenMonitorThread));	//接收状态    在8585端口接收进程发来的状态信息(抓取:源库id,状态  装载:策略id,状态)。根据状态信息修改数据库的状态字段。
	MyThreadStateCol.Add(ThreadState("HBHandleThread", HBHandleThread));
	MyThreadStateCol.Add(ThreadState("SelfCheckThread", SelfCheckThread));			//检查数据库状态、web服务器状态、授权状态

    // 创建所有线程
    if (CreateThreads() == false) return (EXIT_FAILURE);

    StopThreads();
    delete MyDbDataManager;
    return (EXIT_SUCCESS);
}

bool CheckLicense()
{
	if(LicenseType == "DOG"){
		if(BesEncryptD::CheckDogExist() == false){
			BesLog::DebugPrint("[License]Dog Not Exist!", BesLog::LOG_MESSAGE_TYPE_ERROR);
			return false;
		}
	}
	else{
	    // License文件是否存在
	    if (BesEncryptF::IsLicenseExist() == false){
	        BesLog::DebugPrint("[License]License File Not Exist!", BesLog::LOG_MESSAGE_TYPE_ERROR);
	        return false;
	    }
	}

    // 检查策略数授权
	BesLog::DebugPrint("Check strategy number...");
    int TotalStrategy = MyDbDataManager->GetStrategyCount();
    int StrategyLicense;
	//加密狗方式
	if(LicenseType == "DOG"){
		StrategyLicense = BeGeneralLib::StringToInt(BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_HOST));
		if (TotalStrategy <= StrategyLicense && StrategyLicense > 0 && BesEncryptD::ReadDogHostCount() <= StrategyLicense){
//			BesLog::DebugPrint("Strategy number:     [OK]");
		}
		else{
			if (StrategyLicense == 0)
			   BesLog::DebugPrint("Strategy number:     [0]", BesLog::LOG_MESSAGE_TYPE_INFO);
	        else
	            BesLog::DebugPrint("Strategy number:     [FAILED]", BesLog::LOG_MESSAGE_TYPE_ERROR);
			return false;
		}

		//检查CREATETIME
		sql::ResultSet *HostRs = MyDbDataManager->GetStrategyResultSet();
	    if (HostRs != NULL && HostRs->rowsCount() > 0){
	        while (HostRs->next()){
				if(BesEncryptD::GetDogIndex(HostRs->getString("CREATETIME"))==-1){
					BesLog::DebugPrint("invalid strategy detected! will quit!", BesLog::LOG_MESSAGE_TYPE_ERROR);
					MyDbDataManager->DestroyResultSet(HostRs);
					return false;
				}
	        }
	    }
	    MyDbDataManager->DestroyResultSet(HostRs);
	}
	//文件方式
	else{
		StrategyLicense = BeGeneralLib::StringToInt(BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_HOST));
		if (TotalStrategy <= StrategyLicense && StrategyLicense > 0){
//			BesLog::DebugPrint("Strategy number:     [OK]");
	    }
	    else{
	        if (StrategyLicense == 0)
	            BesLog::DebugPrint("Strategy number:     [0]", BesLog::LOG_MESSAGE_TYPE_INFO);
	        else
	            BesLog::DebugPrint("Strategy number:     [FAILED]", BesLog::LOG_MESSAGE_TYPE_ERROR);
			return false;
	    }
	}

	// 检查源库数授权
//  BesLog::DebugPrint("Check source db number...");
    int TotalSourceDB = MyDbDataManager->GetSourceDBCount();
    int SourceDBLicense;
	if(LicenseType == "DOG"){
		SourceDBLicense = BeGeneralLib::StringToInt(BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_SNAPSHOT));
	}
	else{
		SourceDBLicense = BeGeneralLib::StringToInt(BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_SNAPSHOT));
	}
    if (TotalSourceDB <= SourceDBLicense && SourceDBLicense > 0){
//      BesLog::DebugPrint("Source db number:     [OK]");
    }
    else{
        if (SourceDBLicense == 0)
            BesLog::DebugPrint("Source db number:     [0]", BesLog::LOG_MESSAGE_TYPE_INFO);
        else
            BesLog::DebugPrint("Source db number:     [FAILED]", BesLog::LOG_MESSAGE_TYPE_ERROR);
		return false;
    }

    // 检查时间授权 start
	string ExpiredDate;
    string LicenseCreateTime;
	if(LicenseType == "DOG"){
		ExpiredDate = BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_EXPIRED_DATE);
	    LicenseCreateTime = BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_CREATE_TIME);
	}
	else{
		ExpiredDate = BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_EXPIRED_DATE);
	    LicenseCreateTime = BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_CREATE_TIME);
	}
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(ExpiredDate) == true)
    {
        // 无效的授权到期日
		BesLog::DebugPrint("[License]Expired Date Invalid!", BesLog::LOG_MESSAGE_TYPE_ERROR);
        return false;
    }

    // 如果为临时时间授权
    if (ExpiredDate != "00000000")
    {
        // 检查当前系统时间
        time_t LicenseCreateDate = BeGeneralLib::ConvertStringToTime(BeGeneralLib::GetTimeString(BeGeneralLib::ConvertStringToTime(LicenseCreateTime, BeGeneralLib::TIME_FORMAT_FULL_DATETIME), BeGeneralLib::TIME_FORMAT_FULL_DATE), BeGeneralLib::TIME_FORMAT_FULL_DATE);
        if (BeGeneralLib::ConvertStringToTime(BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_FULL_DATE), BeGeneralLib::TIME_FORMAT_FULL_DATE) < LicenseCreateDate)
        {
            // 当前系统时间早于授权文件创建时间
			BesLog::DebugPrint("[License]System Time Earlier Than License Date!", BesLog::LOG_MESSAGE_TYPE_ERROR);
            return false;
        }

        // 检查时间戳文件
        string TimestampFile = BeGeneralLib::GetExecutePath() + DEFAULT_LOG_DIRECTORY + LINUX_PATH_SEPCHAR + string("zts");
        if (BeGeneralLib::IsFileExist(TimestampFile) == false)
        {
        	FILE *fp = fopen(TimestampFile.c_str(), "wb");
        	if (fp == NULL)
        	{
        		BesLog::DebugPrint("Handle timestamp error");
        		return false;
        	}
        	long ts = atol(LicenseCreateTime.c_str());
        	if (fwrite(&ts, sizeof(ts), 1, fp) != 1)
        	{
        		BesLog::DebugPrint("Handle timestamp error");
        		return false;
        	}
        	fclose(fp);
        }

        // 获取文件上次状态修改时间
        if (BeGeneralLib::GetFileStatusChangeTime(TimestampFile) > BeGeneralLib::GetSystemTime())
        {
            // 系统时间已修改
			BesLog::DebugPrint("[License]System Time Changed!", BesLog::LOG_MESSAGE_TYPE_ERROR);
            return false;
        }

        // 读取文件内容
        long ts = 0;
        FILE *fp = fopen(TimestampFile.c_str(), "rb");
        if (fp == NULL)
        {
        	BesLog::DebugPrint("Handle timestamp error");
        	return false;
        }
        if(fread(&ts, sizeof(ts), 1, fp)!=1)
        {
        	BesLog::DebugPrint("Handle timestamp error");
        	return false;
        }
        fclose(fp);
        if (BeGeneralLib::ConvertStringToTime(BeGeneralLib::Number2String(ts), BeGeneralLib::TIME_FORMAT_FULL_DATETIME) > BeGeneralLib::ConvertStringToTime(LicenseCreateTime, BeGeneralLib::TIME_FORMAT_FULL_DATETIME))
        {
            // 该授权文件早于上个授权文件的授权时间
			BesLog::DebugPrint("[License]Current Earlier Than Last License Date!", BesLog::LOG_MESSAGE_TYPE_ERROR);
            return false;
        }

        // 检查授权是否到期
        if (BeGeneralLib::ConvertStringToTime(BeGeneralLib::GetTimeString(BeGeneralLib::GetSystemTime(), BeGeneralLib::TIME_FORMAT_FULL_DATE), BeGeneralLib::TIME_FORMAT_FULL_DATE) > BeGeneralLib::ConvertStringToTime(ExpiredDate, BeGeneralLib::TIME_FORMAT_FULL_DATE))
        {
            // 授权到期
			BesLog::DebugPrint("[License]License Expired!", BesLog::LOG_MESSAGE_TYPE_ERROR);
            return false;
        }

        BeGeneralLib::SetFileLocked(TimestampFile, false);	// 解锁文件
        BeGeneralLib::ChangeFileTimestamp(TimestampFile);	// 修改文件时间戳
        BeGeneralLib::SetFileLocked(TimestampFile, true);	// 锁定文件
    }
	// 检查时间授权 end

    return true;
}

void * CleanThread(void *arg)
{
    ThreadState Thread = MyThreadStateCol.GetThreadState("CleanThread");

    while (Thread.Running())
    {
//		BesLog::DebugPrint("CleanThread is running...");
        time_t NowTime;
        time(&NowTime);

        if (MyNeedToExitFlag == true) break;

		// 删除最近一个月内的任务
		MyDbDataManager->DeleteExpiredTask();

		//清理TBL_TRUNKDATA_xx表
		sql::ResultSet * SourceDBResultSet = MyDbDataManager->GetSourceDBResultSet();
        if (SourceDBResultSet != NULL && SourceDBResultSet->rowsCount() > 0)
        {
			while (SourceDBResultSet->next())
			{
				string SourceID = SourceDBResultSet->getString("SID");
				string TrunkTable = SourceDBResultSet->getString("TRUNK_TABLE");
				if(BeGeneralLib::IsStringEmpty(TrunkTable) == false){
					MyDbDataManager->DeleteExpiredTrunkData(SourceID);
				}
			}
		}
		MyDbDataManager->DestroyResultSet(SourceDBResultSet);

		if (MyNeedToExitFlag == true)
			break;
		else
			sleep(30);
    }
    BesLog::DebugPrint("CleanThread exit...");
    pthread_exit(NULL);
}

//刷新任务线程
void * RefreshTaskThread(void *arg)
{
    ThreadState Thread = MyThreadStateCol.GetThreadState("RefreshTaskThread");

    // 将原有的准备执行、正在执行任务设置为完成，并置结果为失败
    MyDbDataManager->UpdateUncompletedTask();

    while (Thread.Running() == true)
    {
//		BesLog::DebugPrint("RefreshTaskThread is running...");
		if (MyNeedToExitFlag == true) break;

		// 获取空闲任务
		sql::ResultSet * TaskResultSet = MyDbDataManager->GetTaskResultSet();

        if (TaskResultSet == NULL) MyDbDataManager->Repair();

        if (TaskResultSet != NULL && TaskResultSet->rowsCount() > 0)
        {
			while (TaskResultSet->next())
			{
				BesLog::DebugPrint("new task...");
				string TaskId = TaskResultSet->getString("TASKID");
//				string CommandHeader = TaskResultSet->getString("COMMANDHEADER");
				string CommandCode = TaskResultSet->getString("COMMANDCODE");
				string Parameter = TaskResultSet->getString("PARAMETER");
//				int Status = TaskResultSet->getInt("STATUS");
//				int MaxCounter = TaskResultSet->getInt("MAXCOUNTER");
				vector<string> ParaArray = BeGeneralLib::StringSplit(Parameter, COMMAND_PARAMETER_SEPCHAR, BeGeneralLib::WITH_EMPTY_ITEM);

				if (ParaArray.size() != MyCommandManager->GetParameterCount(CommandCode)){
					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "命令参数个数不符");
					BesLog::DebugPrint("命令参数个数不符", BesLog::LOG_MESSAGE_TYPE_ERROR);
					continue;
				}

				bool flag;
				string PID;

				//启动抓取
				if(CommandCode == DEFAULT_COMMAND_START_EXTRACTOR){
					//检查授权 start
					if (CheckLicense() == false){
//						MyNeedToExitFlag = true;
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "超出授权");
						BesLog::DebugPrint("超出授权", BesLog::LOG_MESSAGE_TYPE_ERROR);
						break;
					}
					//检查授权 end

					string SourceID = ParaArray[0];

					if(! MyDbDataManager->IsSourceDBExist(SourceID)){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "源库不存在");
						BesLog::DebugPrint("源库不存在", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					//创建TBL_TRUNKDATA_xx表
					flag = MyDbDataManager->CreateSourceTable(SourceID);
					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "创建相关表失败");
						BesLog::DebugPrint("创建相关表失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
//					BesLog::DebugPrint("table created");

					string ProcessName = string("cd /home/dss/capture/;source /home/dss/capture/env.sh;java zw.com.cn.Capture ") + SourceID;

					string ProcessName1 = string("zw.com.cn.Capture ") + SourceID;

					//如果进程已经启动则报错
					string ShellCommand = string("ps -eo pid,cmd | gawk '{if($0~\"") + ProcessName1 + string("\") print $0}' | grep -v gawk | gawk '{print $1}'");
//					BesLog::DebugPrint(ShellCommand);
					vector<string> PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
					if(PidList.size() > 0){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "进程已启动");
						BesLog::DebugPrint("进程已启动", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					//启动抓取进程
					ShellCommand = ProcessName + string(" &");
					flag = BeGeneralLib::ExecuteSystem(ShellCommand, true, false);
					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "启动进程失败");
						BesLog::DebugPrint("启动进程失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
					BesLog::DebugPrint("process created");

					//获取进程号
					sleep(1);
					ShellCommand = string("ps -eo pid,cmd | gawk '{if($0~\"") + ProcessName1 + string("\") print $0}' | grep -v gawk | gawk '{print $1}'");
					BesLog::DebugPrint(ShellCommand);
					PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
					if(PidList.size() != 1){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "获取进程号失败");
						BesLog::DebugPrint("获取进程号失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					PID = PidList[0];
					BesLog::DebugPrint(string("extractor pid : ") + PID);

					//保存进程号
					MyDbDataManager->UpdateExtractorProcess(SourceID, PID);

					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS, "");

					InitSourceDBMap();
					time(&SourceDBMap[SourceID]);
				}

				//停止抓取
				else if(CommandCode == DEFAULT_COMMAND_STOP_EXTRACTOR){
					string SourceID = ParaArray[0];

					//获取保存的进程号
					string PID = MyDbDataManager->GetExtractorProcess(SourceID);

					//获取实际的进程号
					string ProcessName1 = string("zw.com.cn.Capture ") + SourceID;
					string ShellCommand = string("ps -eo pid,cmd | gawk '{if($0~\"") + ProcessName1 + string("\") print $0}' | grep -v gawk | gawk '{print $1}'");
					BesLog::DebugPrint(ShellCommand);
					vector<string> PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
					if(PidList.size() == 0){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "进程已停止");
						BesLog::DebugPrint("进程已停止", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					//实际的和保存的不匹配
					if(PidList[0] != PID){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "需要人工停止进程");
						BesLog::DebugPrint("需要人工停止进程", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					//杀死进程
				    ShellCommand = string("kill -s KILL ") + PID;
					BesLog::DebugPrint(ShellCommand);
				    flag = BeGeneralLib::ExecuteSystem(ShellCommand, false, false);
					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "停止进程失败");
						BesLog::DebugPrint("停止进程失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
					BesLog::DebugPrint("process stopped");
					MyDbDataManager->UpdateExtractorProcess(SourceID, "0");

					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS, "");

					SourceDBMap[SourceID] = -1;
				}

				//启动装载
				else if(CommandCode == DEFAULT_COMMAND_START_LOADER){
					//检查授权 start
					if (CheckLicense() == false){
//						MyNeedToExitFlag = true;
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "超出授权");
						BesLog::DebugPrint("超出授权", BesLog::LOG_MESSAGE_TYPE_ERROR);
						break;
					}
					//检查授权 end

					string StrategyID = ParaArray[0];

					if(! MyDbDataManager->IsStrategyExist(StrategyID)){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "策略不存在");
						BesLog::DebugPrint("策略不存在", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					//创建TBL_DONETRANS_xx表
					flag = MyDbDataManager->CreateStrategyTable(StrategyID);
					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "创建相关表失败");
						BesLog::DebugPrint("创建相关表失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
//					BesLog::DebugPrint("table created");

					string ProcessName = string("cd /home/dss/load;source /home/dss/load/env.sh;java cn.com.zhiwang.load.SyncTask ") + StrategyID;

					string ProcessName1 = string("cn.com.zhiwang.load.SyncTask ") + StrategyID;

					//如果进程已经启动则报错
					string ShellCommand = string("ps -eo pid,cmd | gawk '{if($0~\"") + ProcessName1 + string("\") print $0}' | grep -v gawk | gawk '{print $1}'");
					vector<string> PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
					if(PidList.size() > 0){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "进程已启动");
						BesLog::DebugPrint("进程已启动", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					//启动装载进程
					ShellCommand = ProcessName + string(" &");
					flag = BeGeneralLib::ExecuteSystem(ShellCommand, false, false);
					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "启动进程失败");
						BesLog::DebugPrint("启动进程失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
					BesLog::DebugPrint("process created");

					//获取进程号
					sleep(1);
					ShellCommand = string("ps -eo pid,cmd | gawk '{if($0~\"") + ProcessName1 + string("\") print $0}' | grep -v gawk | gawk '{print $1}'");
					PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
					if(PidList.size() != 1){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "获取进程号失败");
						BesLog::DebugPrint("获取进程号失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					PID = PidList[0];
					BesLog::DebugPrint(string("loader pid : ") + PID);

					//保存进程号
					MyDbDataManager->UpdateLoaderProcess(StrategyID, PID);

					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS, "");

					InitStrategyMap();
					time(&StrategyMap[StrategyID]);
				}

				//停止装载
				else if(CommandCode == DEFAULT_COMMAND_STOP_LOADER){
					string StrategyID = ParaArray[0];

					//获取保存的进程号
					string PID = MyDbDataManager->GetLoaderProcess(StrategyID);

					//获取实际的进程号
					string ProcessName1 = string("cn.com.zhiwang.load.SyncTask ") + StrategyID;
					string ShellCommand = string("ps -eo pid,cmd | gawk '{if($0~\"") + ProcessName1 + string("\") print $0}' | grep -v gawk | gawk '{print $1}'");
					BesLog::DebugPrint(ShellCommand);
					vector<string> PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
					if(PidList.size() == 0){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "进程已停止");
						BesLog::DebugPrint("进程已停止", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					//实际的和保存的不匹配
					if(PidList[0] != PID){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "需要人工停止进程");
						BesLog::DebugPrint("需要人工停止进程", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					//杀死进程
				    ShellCommand = string("kill -s KILL ") + PID;
					BesLog::DebugPrint(ShellCommand);
				    flag = BeGeneralLib::ExecuteSystem(ShellCommand, false, false);
					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "停止进程失败");
						BesLog::DebugPrint("停止进程失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
					BesLog::DebugPrint("process stopped");
					MyDbDataManager->UpdateLoaderProcess(StrategyID, "0");

					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS, "");

					StrategyMap[StrategyID] = -1;
				}

				//删除策略
				else if(CommandCode == DEFAULT_COMMAND_DEL_STRATEGY){
					string StrategyID = ParaArray[0];

					if(! MyDbDataManager->IsStrategyExist(StrategyID)){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "策略不存在");
						BesLog::DebugPrint("策略不存在", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					flag = false;

					//加密狗授权
					if(LicenseType == "DOG"){
						string CreateTime = MyDbDataManager->GetStrategyCreateTime(StrategyID);
		                if(MyDbDataManager->DeleteStrategy(StrategyID)){
							if(BesEncryptD::DelDogTime(BesEncryptD::GetDogIndex(CreateTime))){
								BesEncryptD::WriteDogHostCount(BesEncryptD::ReadDogHostCount() - 1);
								flag = true;
							}
						}
					}
					//文件授权
					else{
						flag = MyDbDataManager->DeleteStrategy(StrategyID);
					}

					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "删除策略失败");
						BesLog::DebugPrint("删除策略失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
					BesLog::DebugPrint("strategy deleted");
					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS, "");

					InitStrategyMap();
				}

				//删除源库
				else if(CommandCode == DEFAULT_COMMAND_DEL_SOURCEDB){
					string SourceID = ParaArray[0];

					if(! MyDbDataManager->IsSourceDBExist(SourceID)){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "源库不存在");
						BesLog::DebugPrint("源库不存在", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}

					flag = MyDbDataManager->DeleteSourceDB(SourceID);
					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "删除源库失败");
						BesLog::DebugPrint("删除源库失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
					BesLog::DebugPrint("sourcedb deleted");
					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS, "");

					InitSourceDBMap();
				}

				//增加策略
				else if(CommandCode == DEFAULT_COMMAND_ADD_STRATEGY){

					//检查授权 start
					int TotalStrategy = MyDbDataManager->GetStrategyCount();
					//加密狗授权
					if(LicenseType == "DOG"){
						int StrategyLicense = BeGeneralLib::StringToInt(BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_HOST));
						if (TotalStrategy >= StrategyLicense || StrategyLicense <= 0 || BesEncryptD::ReadDogHostCount() >= StrategyLicense){
							MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "超出授权个数");
							BesLog::DebugPrint("超出授权个数", BesLog::LOG_MESSAGE_TYPE_ERROR);
							continue;
						}
					}
					//文件授权
					else{
						int StrategyLicense = BeGeneralLib::StringToInt(BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_HOST));
						if (TotalStrategy >= StrategyLicense || StrategyLicense <= 0){
				            MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "超出授权个数");
							BesLog::DebugPrint("超出授权个数", BesLog::LOG_MESSAGE_TYPE_ERROR);
							continue;
				        }
					}
					//检查授权 end

					string StrategyName = ParaArray[0];
					string SID = ParaArray[1];
					string DID = ParaArray[2];
					string SCN = ParaArray[3];
					string SyncUsers = ParaArray[4];
					string Interval = ParaArray[5];
					string ErrorSkip = ParaArray[6];
					string AutoStart = ParaArray[7];

					flag = false;

					//加密狗授权
					if(LicenseType == "DOG"){
						if(MyDbDataManager->AddStrategy(StrategyName,SID,DID,SCN,SyncUsers,Interval,ErrorSkip,AutoStart)){
							if(BesEncryptD::AddDogTime(MyDbDataManager->GetStrategyCreateTime(""))){	//取最新的时间
								BesEncryptD::WriteDogHostCount(BesEncryptD::ReadDogHostCount() + 1);
								flag = true;
							}
						}
					}
					//文件授权
					else{
						flag = MyDbDataManager->AddStrategy(StrategyName,SID,DID,SCN,SyncUsers,Interval,ErrorSkip,AutoStart);
					}

					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "增加策略失败");
						BesLog::DebugPrint("增加策略失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
					BesLog::DebugPrint("strategy added");
					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS, "");

					InitStrategyMap();
				}

				//增加源库
				else if(CommandCode == DEFAULT_COMMAND_ADD_SOURCEDB){
					//检查授权start
					int TotalSourceDB = MyDbDataManager->GetSourceDBCount();
					if(LicenseType == "DOG"){
						int SourceDBLicense = BeGeneralLib::StringToInt(BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_SNAPSHOT));
						if (TotalSourceDB >= SourceDBLicense || SourceDBLicense <= 0){
							MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "超出授权个数");
							BesLog::DebugPrint("超出授权个数", BesLog::LOG_MESSAGE_TYPE_ERROR);
							continue;
					    }
					}
					else{
					    int SourceDBLicense = BeGeneralLib::StringToInt(BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_SNAPSHOT));
					    if (TotalSourceDB >= SourceDBLicense || SourceDBLicense <= 0){
							MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "超出授权个数");
							BesLog::DebugPrint("超出授权个数", BesLog::LOG_MESSAGE_TYPE_ERROR);
							continue;
					    }
					}
					//检查授权end

					string DbType = ParaArray[0];
					string SIP = ParaArray[1];
					string InstanceName = ParaArray[2];
					string Port = ParaArray[3];
					string Username = ParaArray[4];
					string Password = ParaArray[5];
					string TSName = ParaArray[6];
					string TrunkScn = ParaArray[7];
					string BranchScn = ParaArray[8];
					string Interval = ParaArray[9];

					flag = MyDbDataManager->AddSourceDB(DbType,SIP,InstanceName,Port,Username,Password,TSName,TrunkScn,BranchScn,Interval);
					if(flag == false){
						MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, "增加源库失败");
						BesLog::DebugPrint("增加源库失败", BesLog::LOG_MESSAGE_TYPE_ERROR);
						continue;
					}
					BesLog::DebugPrint("sourcedb added");
					MyDbDataManager->UpdateTaskStatus(TaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS, "");

					InitSourceDBMap();
				}
            }
		}

        MyDbDataManager->DestroyResultSet(TaskResultSet);
		if (MyNeedToExitFlag == true) break;
		sleep(3);
    }
    BesLog::DebugPrint("RefreshTaskThread exit...");
    pthread_exit(NULL);
}

//侦听监控信息线程
void * ListenMonitorThread(void *arg)
{
    ThreadState Thread = MyThreadStateCol.GetThreadState("ListenMonitorThread");

    struct sockaddr_in ClientSock;													// 客户端套接字
    SocketEx * ServerSock = new SocketEx(SocketEx::SOCKETEX_TYPE_UDP_SERVER, 8585);	// 服务端套接字
    ServerSock->Open();

    while (Thread.Running())
    {
		if (MyNeedToExitFlag == true) break;
//		BesLog::DebugPrint("ListenMonitorThread is running...");
        bzero(&ClientSock, sizeof (ClientSock));
        string RecvString = ServerSock->ReceiveFrom(&ClientSock);
        string ClientIp = inet_ntoa(ClientSock.sin_addr);
        BesLog::DebugPrint(string("[") + ClientIp + string("]:MonitorMsg=") + RecvString, BesLog::LOG_MESSAGE_TYPE_NORMAL, false);

        if (HandleMonitorData(RecvString) == false) sleep(1);

        if (MyNeedToExitFlag == true) break;
    }
    ServerSock->Close();
    delete ServerSock;
    BesLog::DebugPrint("ListenMonitorThread exit...");
    pthread_exit(NULL);
}

void InitSourceDBMap()
{
    if (MyDbDataManager == NULL) return;

    sql::ResultSet *SourceRs = MyDbDataManager->GetSourceDBResultSet();
    if (SourceRs != NULL && SourceRs->rowsCount() > 0)
    {
        while (SourceRs->next())
        {
            string SourceID = SourceRs->getString("SID");
			if (SourceDBMap.find(SourceID) == SourceDBMap.end()){
				SourceDBMap.insert(pair<string,time_t> (SourceID, -1));
			}
        }
    }
    MyDbDataManager->DestroyResultSet(SourceRs);

    // 检查是否有不存在的SourceDB
	map<string, time_t> ::iterator iter;
	for(iter = SourceDBMap.begin(); iter != SourceDBMap.end(); iter++){
		string TempID = iter -> first;
		if (MyDbDataManager->IsSourceDBExist(TempID) == false) SourceDBMap.erase(iter);
	}
}

void InitStrategyMap()
{
    if (MyDbDataManager == NULL) return;

    sql::ResultSet *StrategyRs = MyDbDataManager->GetStrategyResultSet();
    if (StrategyRs != NULL && StrategyRs->rowsCount() > 0)
    {
        while (StrategyRs->next()) {
            string StrategyID = StrategyRs->getString("STRATEGY_ID");
			if (StrategyMap.find(StrategyID) == StrategyMap.end()){
				StrategyMap.insert(pair<string,time_t> (StrategyID, -1));
			}
        }
    }
    MyDbDataManager->DestroyResultSet(StrategyRs);

    // 检查是否有不存在的Strategy
	map<string, time_t> ::iterator iter;
    for(iter = StrategyMap.begin(); iter != StrategyMap.end(); iter++){
		string TempID = iter -> first;
		if (MyDbDataManager->IsStrategyExist(TempID) == false) StrategyMap.erase(iter);
	}
}

bool CreateThreads()
{
    for (int i = 0; i < MyThreadStateCol.Size(); i++)
    {
        if (MyThreadStateCol[i].Create() == false)
        {
            return false;
        }
    }

    for (int i = 0; i < MyThreadStateCol.Size(); i++)
    {
        void * r;
        int res = pthread_join(MyThreadStateCol[i].ThreadId(), &r);
        if (res != 0)
        {
            cout << "Join Failed" << endl;
        }
    }
    return true;
}

void StopThreads()
{
    for (int i = 0; i < MyThreadStateCol.Size(); i++)
    {
        MyThreadStateCol[i].Stop();
    }
}

//获取中断信号
void CatchInterrupt(int sig)
{
    if (sig == SIGINT)
    {
        if (MyNeedToExitFlag == false)
        {
            MyNeedToExitFlag = true;
            StopThreads();
        }
    }
}

//自检
bool SelfCheck()
{
    BesLog::DebugPrint("[SyncTask Self Checking]...");

    // 检查程序是否已经运行
    BesLog::DebugPrint("Check instance ...");
    if (BeGeneralLib::IsInstanceExist()){
        BesLog::DebugPrint("Instance:   [already running]!");
        return false;
    }
    BesLog::DebugPrint("Instance:   [OK]");

    // 检测端口号是否在使用中
    BesLog::DebugPrint("Check port ...");
    BeGeneralLib::CheckPort(8585);

	//centos7
	BeGeneralLib::ExecuteSystem("systemctl disable udisks2");
	BeGeneralLib::ExecuteSystem("systemctl stop udisks2");
	BeGeneralLib::ExecuteSystem("systemctl disable NetworkManager");
	BeGeneralLib::ExecuteSystem("systemctl stop NetworkManager");

    return true;
}

void BaseInit()
{
    // 设置终端颜色
    BeGeneralLib::ResetConsoleColor();

    // 配置动态链接
    BeGeneralLib::ConfigureDynamicLinker();

    // 检查数据库是否OK
    string Host = DEFAULT_EMPTY_STRING;
    string UserName = DEFAULT_EMPTY_STRING;
    string Password = DEFAULT_EMPTY_STRING;
    string DbName = DEFAULT_EMPTY_STRING;
    string ConfigFileName = BeGeneralLib::GetExecutePath() + DEFAULT_CONFIG_FILE_NAME;
    if (BeGeneralLib::IsFileExist(ConfigFileName))
    {
        Host = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_HOST);
        UserName = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_USER);
        Password = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_PASSWORD);
        DbName = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_DBNAME);
		ExtractorAutostartInterval = BeGeneralLib::StringToInt(BeGeneralLib::GetConfigItemValue("EXTRACTORAUTOSTARINTERVAL"));
		LoaderAutostartInterval = BeGeneralLib::StringToInt(BeGeneralLib::GetConfigItemValue("LOADERAUTOSTARINTERVAL"));
		AutoClean = BeGeneralLib::StringToInt(BeGeneralLib::GetConfigItemValue("AUTOCLEAN"));

		LicenseType = BeGeneralLib::GetConfigItemValue("LICENSETYPE");
		if(LicenseType == "DOG") {
		}
		else{
			LicenseType = "FILE";
		}
		BesLog::DebugPrint(string("License Type : ") + LicenseType);
    }
    else
    {
        Host = BES_DATABASE_HOST;
        UserName = BES_USER_NAME;
        Password = BES_PASSWORD;
        DbName = DEFAULT_BES_DB_NAME;
    }

    MyDbDataManager = new DbDataManager(Host, UserName, Password, DbName);
    MyCommandManager = new BeCommand();

	ExecutePath = BeGeneralLib::AddPathSeparator(BeGeneralLib::GetExecutePath());	//    /home/dss/
}

//显示授权信息
void ShowLicense()
{
	int StrategyLicense;
	int SourceDBLicense;
	string ExpiredDate;
	string LicenseCreateTime;

	if(LicenseType == "DOG"){
		StrategyLicense = BeGeneralLib::StringToInt(BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_HOST));
	    SourceDBLicense = BeGeneralLib::StringToInt(BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_SNAPSHOT));
	    ExpiredDate = BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_EXPIRED_DATE);
	    LicenseCreateTime = BesEncryptD::GetLicense(BesEncryptD::LICENSE_TYPE_CREATE_TIME);
	}
	else{
	    StrategyLicense = BeGeneralLib::StringToInt(BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_HOST));
	    SourceDBLicense = BeGeneralLib::StringToInt(BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_SNAPSHOT));
	    ExpiredDate = BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_EXPIRED_DATE);
	    LicenseCreateTime = BesEncryptF::GetLicense(BesEncryptF::LICENSE_TYPE_CREATE_TIME);
	}

    // 写入license信息文件
    string LicenseStr = "";
    string FileName = BeGeneralLib::GetExecutePath() + "dsslicinfo.txt";
    FILE *fp = fopen(FileName.c_str(), "w+");
    if (fp != NULL)
    {
		string DataStr;

        // 策略数
        DataStr = string("Strategy:\t") + BeGeneralLib::Number2String(StrategyLicense) + string("\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);

        // 源库数
        DataStr = string("SourceDB:\t") + BeGeneralLib::Number2String(SourceDBLicense) + string("\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);

        // 许可到期日
        if (BeGeneralLib::IsStringEmptyOrInvalidValue(ExpiredDate) == false)
        {
            if (ExpiredDate == "00000000")
                DataStr = string("Expired date:\t Never\n");
            else
                DataStr = string("Expired date:\t") + BeGeneralLib::GetTimeString(BeGeneralLib::ConvertStringToTime(ExpiredDate, BeGeneralLib::TIME_FORMAT_FULL_DATE), BeGeneralLib::TIME_FORMAT_DATE) + string("\n");

            BeGeneralLib::WriteDataToFile(fp, DataStr);
        }

        // 授权创建时间
        DataStr = string("Create time:\t") + BeGeneralLib::GetTimeString(BeGeneralLib::ConvertStringToTime(LicenseCreateTime, BeGeneralLib::TIME_FORMAT_FULL_DATETIME), BeGeneralLib::TIME_FORMAT_LONG) + string("\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);

        fclose(fp);
    }

    // 显示license信息
    string ShellCommand = string("cat ") + FileName;
    BeGeneralLib::ExecuteSystem(ShellCommand, false, false);
}

//处理监控数据
bool HandleMonitorData(string monitordata)
{
	vector<string> ParaArray1 = BeGeneralLib::StringSplit(monitordata, '#', BeGeneralLib::REMOVE_EMPTY_ITEM);
	if (ParaArray1.size() < 3){
        BesLog::DebugPrint("Monitor parameter is invalid!");
		BesLog::DebugPrint(string("size:")+BeGeneralLib::Number2String(ParaArray1.size()));
        return false;
    }
	BesLog::DebugPrint(string("time:") + ParaArray1[0] + string(" code:") + ParaArray1[1] + string(" status_str:") + ParaArray1[2],BesLog::LOG_MESSAGE_TYPE_NORMAL,false);

	string CmdCode = ParaArray1[1];
	string Parameter = ParaArray1[2];
//  Parameter = BeGeneralLib::StringRTrim(Parameter, COMMAND_PARAMETER_SEPCHAR); // 清除参数右端分割符

    vector<string> ParaArray2 = BeGeneralLib::StringSplit(Parameter, ';', BeGeneralLib::REMOVE_EMPTY_ITEM);
    if (ParaArray2.size() < 2){
        BesLog::DebugPrint("Monitor parameter is invalid!");
		BesLog::DebugPrint(string("size:")+BeGeneralLib::Number2String(ParaArray2.size()));
        return false;
    }

    string ID = ParaArray2[0];
	string status = ParaArray2[1];

	BesLog::DebugPrint(string("ID:") + ID + string(" status:") + status,BesLog::LOG_MESSAGE_TYPE_NORMAL,false);

	if(CmdCode == DEFAULT_COMMAND_QUERY_EXTRACTOR_STATE){
		if (SourceDBMap[ID] == 0){
			BesLog::DebugPrint(string("source db ") + ID +string(" not exist"));
			return false;
		}

		if(status == "0"){
			MyDbDataManager->UpdateExtractorStatus(ID, "0");
		}
		else if(status == "1"){
			MyDbDataManager->UpdateExtractorStatus(ID, "1");
			time(&SourceDBMap[ID]);
		}
	}
	else if(CmdCode == DEFAULT_COMMAND_QUERY_LOADER_STATE){
		if (StrategyMap[ID] == 0){
			BesLog::DebugPrint(string("strategy ") + ID +string(" not exist"));
			return false;
		}

		if(status == "0"){
			MyDbDataManager->UpdateLoaderStatus(ID, "0");
		}
		else if(status == "1"){
			MyDbDataManager->UpdateLoaderStatus(ID, "1");
			time(&StrategyMap[ID]);
		}
	}
	return true;
}

//更新抓取进程、装载进程的状态
void * HBHandleThread(void *arg)
{
    ThreadState Thread = MyThreadStateCol.GetThreadState("HBHandleThread");
    while (Thread.Running()){
//    	BesLog::DebugPrint("HBHandleThread is Running...");
    	if (MyNeedToExitFlag == true) break;

		map<string, time_t> ::iterator iter;

		//更新抓取进程的状态
		for(iter = SourceDBMap.begin(); iter != SourceDBMap.end(); iter++){
			string TempID = iter -> first;
//			time_t TempTime = SourceDBMap[TempID];
			time_t NowTime; time(&NowTime);

			if (SourceDBMap[TempID] > 0){
				if(difftime(NowTime, SourceDBMap[TempID]) >= 30){
					MyDbDataManager->UpdateExtractorStatus(TempID, "0");
				}
				//自动重启抓取进程
				if (ExtractorAutostartInterval > 0 && difftime(NowTime, SourceDBMap[TempID]) >= ExtractorAutostartInterval){
					MyDbDataManager->AddTask(DEFAULT_COMMAND_START_EXTRACTOR, TempID+string(";"));
					BesLog::DebugPrint("capture failed, add autostart task", BesLog::LOG_MESSAGE_TYPE_INFO);
					time(&SourceDBMap[TempID]);
				}
			}
		}

		//更新装载进程的状态
		for(iter = StrategyMap.begin(); iter != StrategyMap.end(); iter++){
			string TempID = iter -> first;
//			time_t TempTime = StrategyMap[TempID];
			time_t NowTime; time(&NowTime);

			if(StrategyMap[TempID] > 0){
				if (difftime(NowTime, StrategyMap[TempID]) >= 30){
					MyDbDataManager->UpdateLoaderStatus(TempID, "0");
				}
				//自动重启装载进程
				if (LoaderAutostartInterval >0 && difftime(NowTime, StrategyMap[TempID]) >= LoaderAutostartInterval){
					MyDbDataManager->AddTask(DEFAULT_COMMAND_START_LOADER, TempID+string(";"));
					BesLog::DebugPrint("loader failed, add autostart task", BesLog::LOG_MESSAGE_TYPE_INFO);
					time(&StrategyMap[TempID]);
				}
			}
		}

		sleep(1);
    }
    BesLog::DebugPrint("HBHandleThread exit...");
    pthread_exit(NULL);
}

void * SelfCheckThread(void *arg)
{
    ThreadState Thread = MyThreadStateCol.GetThreadState("SelfCheckThread");

    // 获取执行路径
    string ExecPath = BeGeneralLib::GetExecutePath();

    // 检查是否需要检查License
    time_t LastCheckTime; time(&LastCheckTime);
//  LastCheckTime -= 2* DEFAULT_CHECK_LICENSE_TIME;		//确保立即执行

    while (Thread.Running())
    {
//		BesLog::DebugPrint("SelfCheckThread is running...");
        // 检查License
        time_t NowTime; time(&NowTime);

        // 是否需要退出
        if (MyNeedToExitFlag == true) break;

        if (difftime(NowTime, LastCheckTime) > 300)
        {
            // 记录上次检查时间
            LastCheckTime = NowTime;
            // 检查授权
			if (CheckLicense() == false) MyNeedToExitFlag = true;
        }

        // 是否需要退出
        if (MyNeedToExitFlag == true)
        {
            break;
        }
		else{
	        sleep(30);
		}
    }
    BesLog::DebugPrint("SelfCheckThread exit...");
    pthread_exit(NULL);
}

void PrintVersion()
{
    cout << "\33[2J";
    cout << "\33[3;5H \33[36m" << endl;
    cout << "\33[4;5H************************************************************" << endl;
    cout << "\33[5;5H*                                                          *" << endl;
    cout << "\33[6;5H*                    APPNAME :  SyncTask                   *" << endl;
    cout << "\33[8;5H*                    VERSION :  " << APP_VERSION_INFORMATION << "                     *" << endl;
    cout << "\33[9;5H*                    RETIME  :  " << APP_RELEASE_TIME << "                 *" << endl;
    cout << "\33[10;5H*                    AGENT   :  " << APP_OS_SUPPORT << "   *" << endl;
    cout << "\33[11;5H*                                                          *" << endl;
    cout << "\33[12;5H************************************************************" << endl;
    cout << "\33[0m";
}

bool ParseOption(int argc, char** argv)
{
    bool HasOption = false;
    opterr = 0;
    int oc = -1;
    while ((oc = getopt(argc, argv, "bdefghiklprsvwx")) != -1)
    {
        HasOption = true;
        switch (oc)
        {
            case 0:
            {
                // 处理长选项
                break;
            }
            case 'b':
            {
                MyDbDataManager->BackupDatabase();
                break;
            }
            case 'k': // 杀死程序
            {
                for (int i = 0; i < 3; i++)
                {
                    string ShellCommand = string("killall -s 2 SyncTask");
                    BeGeneralLib::ExecuteSystem(ShellCommand, true, false);
                    sleep(1);
                }
                break;
            }
            case 'l':
            {
				ShowLicense();
                break;
            }
            case 'r':
            {
                MyDbDataManager->Repair();
                break;
            }
            case 'v':
            {
                PrintVersion();
                break;
            }
            default:
            {
                break;
            }
        }
    }
    return HasOption;
}

void ShowBasicInfo(){
    cout << "-------------------------INFORMATION-------------------------" << endl;
    cout << "|" << endl;
    cout << "|" << endl;
    cout << string("|\tHostName:\t") << BeGeneralLib::GetSystemHostName() << endl;
    cout << "|" << endl;
    cout << "|" << endl;
    cout << "-------------------------------------------------------------" << endl;
}
