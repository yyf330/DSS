#include "task.h"

//---------------------------------- 公有方法 ----------------------------------//

Task::Task()
{
	MyLastError=NULL;
    MySubTask = NULL;
    MyTaskExecStep = Task::TASK_EXEC_STEP_NONE;
}

/*
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *      fcmanager       :   FC存储管理器
 *      id              :   任务ID
 *      cmdcode         :   指令编码
 *      hostip          :   主机IP
 *      hostport        :   主机端口
 *      priority        :   优先级
 *      maxcounter      :   最大执行次数
 */
Task::Task(DbDataManager *dbdatamanager, BeCommand *becommand, string id, string cmdheader, string cmdcode, string parameter, string hostcode, string hostip, string hostport, int priority, int maxcounter)
{
	MyLastError=NULL;
    MySubTask = NULL;
    MyTaskExecStep = Task::TASK_EXEC_STEP_NONE;

    Add(dbdatamanager, becommand, NULL, id, cmdheader, cmdcode, parameter, hostcode, hostip, hostport, "", "", priority, maxcounter);
}

/*
 *  功能：
 *      构造函数
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *      fcmanager       :   FC存储管理器
 *      cmdcode         :   指令编码
 *      parameter       :   参数
 *      server          :   服务器IP
 *      serverport      :   服务器端口
 *  返回：
 *      无
 */
Task::Task(DbDataManager *dbdatamanager, BeCommand *becommand, string cmdheader, string cmdcode, string parameter, string serverip, string serverport)
{
	MyLastError=NULL;
    MySubTask = NULL;
    MyTaskExecStep = Task::TASK_EXEC_STEP_NONE;

    Add(dbdatamanager, becommand, NULL, "", cmdheader, cmdcode, parameter, "", "", "", serverip, serverport);
}

Task::~Task()
{
}

/*
 *  功能：
 *      增加任务
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *      fcmanager       :   FC存储管理器
 *      id              :   任务ID
 *      cmdcode         :   指令编码
 *      hostip          :   主机IP
 *      hostport        :   主机端口
 *      priority        :   优先级
 *      maxcounter      :   最大执行次数
 *  返回：
 *      增加成功返回true，否则返回false
 */
bool Task::Add(DbDataManager* dbdatamanager, BeCommand *becommand, Task* mastertask, string id, string cmdheader, string cmdcode, string parameter, string hostcode, string hostip, string hostport, string serverip, string serverport, int priority, int maxcounter)
{
    MyDbDataManager = dbdatamanager;
    MyLastError = new BesError();		// 错误对象
    MyCommandManager = becommand;

    // 父任务对象
    MyMasterTask = mastertask;
    MyTaskId = id;
    MyCmdHeader = cmdheader;
    MyCmdCode = cmdcode;
    MyParameter = parameter;
    MyHostCode = hostcode;

    // 设置ip
    MyHostIp = hostip;
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(MyHostIp) == true && (MyDbDataManager->IsHostExist(MyHostCode) == true))
    {
        MyHostIp = MyDbDataManager->GetHostIp(MyHostCode);
    }

    // 设置端口
    MyHostPort = hostport;
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(MyHostPort) == true && (MyDbDataManager->IsHostExist(MyHostCode) == true))
    {
        MyHostPort = MyDbDataManager->GetHostPort(MyHostCode);
    }

    MyMaxCounter = maxcounter;
    MyCmdHeader = cmdheader;

    // 设置命令头
    if (cmdheader.size() != COMMAND_HEADER_SIZE)
    {
        MyCmdHeader = BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_COMMAND_HEADER);
    }
    MyCommandStr = BeCommand::CreateStandardCommand(MyCmdHeader, COMMAND_CODE_ID, MyCmdCode, MyParameter);

    MyServerIp = serverip;
    MyServerPort = serverport;

    MyExecWithErrorFlag = false;
    MyMaxCounter = 1;
    MyExecCounter = 0;
    MyTaskExecStep = Task::TASK_EXEC_STEP_VALIDATION_CHECK;
    MyLastTaskExecStep = Task::TASK_EXEC_STEP_NONE;
    MyTaskExecResult = Task::TASK_EXEC_RESULT_NONE;
    MyTaskControl = Task::TASK_CONTROL_START;

    MyWaitResultOK = false;
    MyNotifyStopFlag = false;

    // 初始化任务检查超时时间和标志
    time(&MyStartCheckTime);
    MyStartCheckTimeoutFlag = false;

    // 如果为一键虚拟机应急任务
    if (MyMasterTask != NULL && MyMasterTask->CommandCode() != DEFAULT_COMMAND_START_PLAN_EMERGENCY && MyCmdCode == DEFAULT_COMMAND_START_VM_ONE_KEY && (BeGeneralLib::IsStringEmptyOrZero(MyTaskId) == true || BeGeneralLib::IsStringEmptyOrInvalidValue(MyTaskId) == true))
    {
        // 增加任务
        MyDbDataManager->AddTask(MyCmdHeader, MyCmdCode, MyParameter, DbDataManager::TASK_STATE_EXECUTING, DbDataManager::TASK_RESULT_FAILED, "");

        // 获取任务id
        MyTaskId = MyDbDataManager->GetTaskId(MyCmdHeader);
    }
    return true;
}

/*
 *  功能：
 *      获取任务结果
 *  返回：
 *      任务结果
 */
Task::TaskExecResult Task::Result()
{
    return MyTaskExecResult;
}

void Task::FreeMem(){
	if(MyLastError!=NULL) {delete MyLastError;MyLastError=NULL;}
	if(MySubTask!=NULL) {delete MySubTask;MySubTask=NULL;}
}

//启动任务
void Task::Start()
{
    // 如果为空，则返回
    if (Empty() == true)
    {
    	FreeMem();
        return;
    }

    MyExecCounter = 0;

    // 记录任务开始时间
    time(&MyStartTime);
    time(&StopEALastPingTime);

    bool HasDoneStopControl = false;
    while (true)
    {
        if (MyTaskControl == Task::TASK_CONTROL_START || MyTaskControl == Task::TASK_CONTROL_RESUME)
        {
            MyTaskControl = Task::TASK_CONTROL_START;
        }

        if (MyTaskControl == Task::TASK_CONTROL_PAUSE)
        {
            sleep(3);
            continue;
        }

        if (MyTaskControl == Task::TASK_CONTROL_STOP && HasDoneStopControl == false)
        {
            if (MyNotifyStopFlag == false)
            {
                break;
            }

            // 通知Agent中断任务
            string Parameter = BeCommand::CreateCommandParameter(1, MyHostCode.c_str());
            Task::TaskExecResult TaskResult = StartSubTask(MyCmdHeader, DEFAULT_COMMAND_NOTIFY_HOST_BREAK_TASK, Parameter, MyHostCode, MyHostIp, MyHostPort);
            //if (TaskResult == Task::TASK_EXEC_RESULT_SUCCESS)
            {
                MyExecWithErrorFlag = true;
                MyTaskExecStep = Task::TASK_EXEC_STEP_POST_PROCESSING;
            }
            HasDoneStopControl = true;
            //            break;
        }

        // 显示任务阶段信息
        ShowTaskStageInformation();

        switch (MyTaskExecStep)
        {
            case Task::TASK_EXEC_STEP_VALIDATION_CHECK: // 任务有效性检查
            {
                MyLastTaskExecStep = Task::TASK_EXEC_STEP_VALIDATION_CHECK;
                if (CheckTaskValidation() == true)
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_PRE_PROCESSING;
                }
                else
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_GOTO_FINISH;
                }
                break;
            }
            case Task::TASK_EXEC_STEP_PRE_PROCESSING: // 预处理
            {
                MyLastTaskExecStep = Task::TASK_EXEC_STEP_PRE_PROCESSING;
                if (PreProcessing() == true)
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_WAIT_PREPARE_RESULT;
                }
                else
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_ERROR_PROCESSING;
                }
                break;
            }
            case Task::TASK_EXEC_STEP_WAIT_PREPARE_RESULT: // 等待预处理结果
            {
                MyLastTaskExecStep = Task::TASK_EXEC_STEP_WAIT_PREPARE_RESULT;
                if (WaitPrepareResult() == true)
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_EXCUTE;
                }
                else
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_ERROR_PROCESSING;
                }
                break;
            }
            case Task::TASK_EXEC_STEP_EXCUTE: // 执行任务
            {
                MyLastTaskExecStep = Task::TASK_EXEC_STEP_EXCUTE;
                if (Execute() == true)
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_WAIT_EXCUTING_RESULT;
                }
                else
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_ERROR_PROCESSING;
                }
                break;
            }
            case Task::TASK_EXEC_STEP_WAIT_EXCUTING_RESULT: // 等待执行结果
            {
                MyLastTaskExecStep = Task::TASK_EXEC_STEP_WAIT_EXCUTING_RESULT;
                switch (WaitExecutingResult())
                {
                    case Task::ACTION_PROC_RESULT_SUCCESS:
                    {
                        MyTaskExecStep = Task::TASK_EXEC_STEP_POST_PROCESSING;
                        break;
                    }
                    case Task::ACTION_PROC_RESULT_FAIL:
                    {
                        MyTaskExecStep = Task::TASK_EXEC_STEP_ERROR_PROCESSING;
                        break;
                    }
                    case Task::ACTION_PROC_RESULT_ERROR:
                    {
                        MyTaskExecStep = Task::TASK_EXEC_STEP_POST_PROCESSING;
                        break;
                    }
                    default:
                    {
                        sleep(1);
                        break;
                    }
                }
                break;
            }
            case Task::TASK_EXEC_STEP_POST_PROCESSING: // 后处理
            {
                MyLastTaskExecStep = Task::TASK_EXEC_STEP_POST_PROCESSING;
                if (PostProcessing() == true)
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_GOTO_FINISH;
                }
                else
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_ERROR_PROCESSING;
                }
                break;
            }
            case Task::TASK_EXEC_STEP_ERROR_PROCESSING: // 错误处理
            {
                ErrorProcessing();
                MyTaskExecStep = Task::TASK_EXEC_STEP_GOTO_FINISH;
                break;
            }
            case Task::TASK_EXEC_STEP_GOTO_FINISH: // 准备结束
            {
                if (GotoFinish() == true)
                {
                    MyTaskExecStep = Task::TASK_EXEC_STEP_FINISH;
                }
                else
                {
                    MyLastError->Clear();
                    MyTaskExecStep = Task::TASK_EXEC_STEP_VALIDATION_CHECK;
                }
                break;
            }
            case Task::TASK_EXEC_STEP_FINISH: // 完成
            {
                // 更新任务结果
                if (MyExecWithErrorFlag)
                {
                    MyTaskExecResult = Task::TASK_EXEC_RESULT_FAIL;
                    string ErrDesc = MyLastError->GetErrorDescription(DESCRIPTION_LANG_CN);
                    MyDbDataManager->UpdateTaskStatus(MyTaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_FAILED, ErrDesc);

                    // 对于连接Agent失败，不计入日志
                    bool IsToLog = (MyLastError->GetErrorType() != BesError::BES_ERR_AGENT_CONNECT_FAILED);
                    BesLog::DebugPrint(string("Task [") + MyCmdCode + string("] fail:") + ErrDesc, BesLog::LOG_MESSAGE_TYPE_ERROR, IsToLog);
                }
                else
                {
                    MyTaskExecResult = Task::TASK_EXEC_RESULT_SUCCESS;
                    if (MyLastError->HasError())
                    {
                        string ErrDesc = MyLastError->GetErrorDescription(DESCRIPTION_LANG_CN);
                        MyDbDataManager->UpdateTaskStatus(MyTaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_END, ErrDesc);
                    }
                    else
                    {
                        MyDbDataManager->UpdateTaskStatus(MyTaskId, DbDataManager::TASK_STATE_FINISH, DbDataManager::TASK_RESULT_SUCCESS);
                    }
                }

                // 如果有服务器，则发送响应信息
                if (BeGeneralLib::IsStringEmptyOrInvalidValue(MyServerIp) == false
                        && BeGeneralLib::IsStringEmptyOrInvalidValue(MyServerPort) == false)
                {
                    // 回应信息
                    string Response = "";
                    if (MyExecWithErrorFlag)
                    {
                        string ErrDesc = MyLastError->GetErrorDescription(DESCRIPTION_LANG_CN);
                        Response = BeCommand::CreateStandardCommand(MyCmdHeader, COMMAND_RESPONSE_ID, DEFAULT_COMMAND_RESULT_FAIL, ErrDesc);
                    }
                    else
                    {
                        Response = BeCommand::CreateStandardCommand(MyCmdHeader, COMMAND_RESPONSE_ID, DEFAULT_COMMAND_RESULT_FINISH, "");
                    }

                    BeGeneralLib::SendTcpData(MyServerIp, MyServerPort, Response);
                }

                // 记录结束时间
                time(&MyEndTime);

                // 增加报警
                if (MyExecWithErrorFlag == true && BeGeneralLib::IsStringEmpty(MyTaskId) == false)
                {
                	if (MyCmdCode == DEFAULT_COMMAND_WINDOWS_SNAPSHOT_COPY // windows立即快照
                            || MyCmdCode == DEFAULT_COMMAND_WINDOWS_DATABASE_COPY // windows数据库快照
                            || MyCmdCode == DEFAULT_COMMAND_LINUX_SNAPSHOT_COPY // linux立即快照
                            || MyCmdCode == DEFAULT_COMMAND_LINUX_DATABASE_COPY // linux数据库快照
                            || MyCmdCode == DEFAULT_COMMAND_SCHEDULE_SNAPSHOT_COPY) // 调度快照
                    {
                        map<string, string> FieldList = MyDbDataManager->GetTaskRecord(MyTaskId);
                        MyDbDataManager->AddAlarm(MyHostCode, FieldList["DESCRIPTION"], FieldList["FAILDESC"], DbDataManager::ALARM_LEVEL_WARNING,	//DbDataManager::ALARM_LEVEL_ATTENTION,
                        							BeGeneralLib::GetTimeString(MyStartTime, BeGeneralLib::TIME_FORMAT_LONG),
                        							BeGeneralLib::GetTimeString(MyEndTime, BeGeneralLib::TIME_FORMAT_LONG));
                    }
                }

                // 打印任务时间
                PrintTaskTime(MyStartTime, MyEndTime);

                // 设置步骤为空
                MyTaskExecStep = Task::TASK_EXEC_STEP_NONE;
                FreeMem();
                return;
            }
            default:
            {
                break;
            }
        }
        usleep(500000);
    }
}

/*
 *  功能：
 *      暂停任务
 *  参数：
 *      无
 *  返回：
 *      无
 */
void Task::Pause()
{
    MyTaskControl = Task::TASK_CONTROL_PAUSE;
}

/*
 *  功能：
 *      重新执行任务
 *  参数：
 *      无
 *  返回：
 *      无
 */
void Task::Resume()
{
    if (MyTaskControl != Task::TASK_CONTROL_STOP)
    {
        MyTaskControl = Task::TASK_CONTROL_RESUME;
    }
}

/*
 *  功能：
 *      停止任务
 *  参数：
 *      notifyflag      :   是否通知标志
 *  返回：
 *      无
 */
void Task::Stop(bool notifyflag)
{
    MyTaskControl = Task::TASK_CONTROL_STOP;
    MyWaitResultOK = true;
    MyNotifyStopFlag = notifyflag;
}

/*
 *  功能：
 *      设置Agent响应信息
 *  参数：
 *      rpstr           :   响应信息
 *  返回：
 *      无
 */
bool Task::SetResponse(string rpstr)
{
    // 如果任务为空，则返回
    if (Empty() == true)
    {
        return false;
    }

    vector<string> RpSubArray = BeGeneralLib::StringSplit(rpstr, COMMAND_SEGMENT_SEPCHAR, BeGeneralLib::WITH_EMPTY_ITEM);

    // 如果有子任务，检查是否为子任务的命令头
    if (MySubTask != NULL)
    {
        if (MySubTask->SetResponse(rpstr) == true)
        {
            return true;
        }
    }

    if (CommandHeader() == RpSubArray[0])
    {
        // 设置开始检查时间
        time(&MyStartCheckTime);

        MyAgentResponse = rpstr;
        return true;
    }
    return false;

}

/*
 *  功能：
 *      发送处理状态
 *  参数：
 *      无
 *  返回：
 *      无
 */
void Task::SendProcessingState()
{

}

// 功能：检查Agent是否超时
void Task::CheckTimeout()
{
    return;
}

/*
 *  功能：
 *      解锁信号量
 *  参数
 *      recvstring      :   执行结果
 *  返回：
 *      无
 */
void Task::Release(string recvstring)
{
    // 如果任务为空，则返回
    if (Empty() == true || BeGeneralLib::IsStringEmpty(recvstring) == true)
    {
        return;
    }

    if (MySubTask != NULL)
    {
        MySubTask->Release(recvstring);
    }

    // 如果为退出，则停止任务
    if (recvstring == INTERNAL_EXIT_COMMAND)
    {
        MyTaskControl = Task::TASK_CONTROL_STOP;
    }

    BesLog::DebugPrint("Internal Result:" + recvstring);

    MyInternalResult = recvstring;

    usleep(200000);
    MyWaitResultOK = true;
    //    sem_post(&MySemaphore);
}

/*
 *  功能：
 *      是否为空
 *  参数
 *      无
 *  返回：
 *      如果是返回true，否则返回false
 */
bool Task::Empty()
{
    return (MyTaskExecStep == Task::TASK_EXEC_STEP_NONE);
}

/*
 *  功能：
 *      返回任务ID
 *  参数：
 *      无
 *  返回：
 *      任务ID
 */
string Task::Id()
{
    return MyTaskId;
}

/*
 *  功能：
 *      返回命令头
 *  参数：
 *      无
 *  返回：
 *      命令头
 */
string Task::CommandHeader()
{
    return MyCmdHeader;
}

/*
 *  功能：
 *      返回命令字
 *  参数：
 *      无
 *  返回：
 *      命令字
 */
string Task::CommandCode()
{
    return MyCmdCode;
}

/*
 *  功能：
 *      返回命令参数
 *  参数：
 *      无
 *  返回：
 *      命令参数
 */
string Task::CommandParameter()
{
    return MyParameter;
}

/*
 *  功能：
 *      返回命令对应的主机编码
 *  参数：
 *      无
 *  返回：
 *      主机编码
 */
string Task::HCode()
{
    return MyHostCode;
}

/*
 *  功能：
 *      重载小于符
 *  参数：
 *      task        ：   任务对象
 *  返回：
 *      如果当前任务的优先级小于比较任务的优先级返回true,否则返回false
 */
bool Task::operator<(const Task& task) const
{
/*    if (MyPriority < task.MyPriority)
    {
        return false;
    }
    else if (MyPriority == task.MyPriority)
    {
        if (atol(MyTaskId.c_str()) < atol(task.MyTaskId.c_str()))
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        return true;
    }*/
	return true;
}

/*
 *  功能：
 *      最近一次错误
 *  参数：
 *      无
 *  返回：
 *      返回最近一次错误
 */
BesError* Task::LastError()
{
    return MyLastError;
}

//---------------------------------- 私有方法 ----------------------------------//

//检查任务有效性
bool Task::CheckTaskValidation()
{
   return true;
}

bool Task::PreProcessing()
{
    return true;
}

/*
 *  功能：
 *      等待准备结果
 *  参数：
 *      无
 *  返回：
 *      等待成功返回true，否则返回false
 */
bool Task::WaitPrepareResult()
{
    return true;
}

bool Task::Execute()
{
	return true;
}

/*
 *  功能：
 *      等待执行结果
 *  参数：
 *      无
 *  返回：
 *      处理成功返回true，否则返回false
 */
Task::ActionProcResult Task::WaitExecutingResult()
{
    // 获取参数列表
    vector<string> ParaArray = BeGeneralLib::StringSplit(MyParameter, COMMAND_PARAMETER_SEPCHAR, BeGeneralLib::WITH_EMPTY_ITEM);

    // 获取命令字首字符
    string FirstCode = MyCmdCode.substr(0, 1);

    if (FirstCode == DEFAULT_QUERY_COMMAND_PREFIX // 查询类
            || FirstCode == DEFAULT_COPY_COMMAND_PREFIX // 复制类
            || FirstCode == DEFAULT_RECOVER_COMMAND_PREFIX // 恢复类
            || FirstCode == DEFAULT_DISK_OPERATION_COMMAND_PREFIX // 盘符操作类命令
            || FirstCode == DEFAULT_HOST_CONTROL_COMMAND_PREFIX // 主机控制类
            || FirstCode == DEFAULT_NOTIFY_COMMAND_PREFIX) // 通知类
    {
        // 如果为AIX复制，则退出Agent处理操作
        if (MyCmdCode == DEFAULT_COMMAND_AIX_INITIALIZE_COPY
                || MyCmdCode == DEFAULT_COMMAND_AIX_SNAPSHOT_COPY
                || MyCmdCode == DEFAULT_COMMAND_AIX_SCHEDULE_SNAPSHOT_COPY
                || MyCmdCode == DEFAULT_COMMAND_AIX_DATABASE_COPY
                || MyCmdCode == DEFAULT_COMMAND_AIX_DATA_RECOVER)
        {
            return Task::ACTION_PROC_RESULT_SUCCESS;
        }

        // 如果为应急模式，则退出Agent处理操作
        DbDataManager::HostProtectedStatus HostStatus = MyDbDataManager->GetHostProtectedStatus(MyHostCode);
        DbDataManager::HostOSType OsType = MyDbDataManager->GetHostOSType(MyHostCode);
        if (BeCommand::IsWindowsOrLinuxSnapshotCopyCommand(MyCmdCode) == true)
        {
            switch (HostStatus)
            {
                case DbDataManager::HOST_PROTECTED_STATUS_NET:
                case DbDataManager::HOST_PROTECTED_STATUS_FCBOOT:
                {
                    // 当前卷类型
                    DbDataManager::VolumeType CurrentVolumeType = MyDbDataManager->GetCurrentVolumeType(MyHostCode, ParaArray[1]);

                    // 是否用于cdp
                    bool IsUsedForCDP = MyDbDataManager->IsStrategyUsedForCDP(MyHostCode, ParaArray[1]);

                    // 检查是否直接返回
                    if ((OsType == DbDataManager::HOST_OS_TYPE_LINUX) || (OsType == DbDataManager::HOST_OS_TYPE_WINDOWS && (CurrentVolumeType == DbDataManager::VOLUME_TYPE_ISCSI || CurrentVolumeType == DbDataManager::VOLUME_TYPE_FC || (CurrentVolumeType == DbDataManager::VOLUME_TYPE_LOCAL && IsUsedForCDP == false))))
                    {
                        return Task::ACTION_PROC_RESULT_SUCCESS;
                    }
                    break;
                }
                case DbDataManager::HOST_PROTECTED_STATUS_VM:
                {
                    return Task::ACTION_PROC_RESULT_SUCCESS;
                }
                case DbDataManager::HOST_PROTECTED_STATUS_LOCAL:
                {
                    if (MyDbDataManager->IsStrategyUsedForCDP(MyHostCode, ParaArray[1]) == true)
                    {
                        return Task::ACTION_PROC_RESULT_SUCCESS;
                    }
                }
            }
        }

        // 如果为初始复制，并且主机复制方式不是iscsi和fc，则退出Agent处理操作
        if ((MyCmdCode == DEFAULT_COMMAND_WINDOWS_INITIALIZE_COPY))
        {
            DbDataManager::HostCopyMode CopyMode = MyDbDataManager->GetHostCopyMode(ParaArray[0]);
            if (CopyMode != DbDataManager::HOST_COPY_MODE_ISCSI && CopyMode != DbDataManager::HOST_COPY_MODE_FC)
            {
                return Task::ACTION_PROC_RESULT_SUCCESS;
            }
        }

        if (MyCmdCode == DEFAULT_COMMAND_NOTIFY_HOST_STOPEA){		//针对510命令的特殊处理(agent处理完510后无法发送回应信息)
        	string HCode = ParaArray[0];
        	if(difftime(time(NULL), StopEALastPingTime) > 30){
        		time(&StopEALastPingTime);
        		if(!BeGeneralLib::IsPingOK(MyDbDataManager->GetHostVirtualIp(HCode), 3, 1)) return Task::ACTION_PROC_RESULT_SUCCESS;
        	}
        }

        if (BeGeneralLib::IsStringEmptyOrInvalidValue(MyAgentResponse) == true)
        {
            return Task::ACTION_PROC_RESULT_WAIT;
        }

        ActionProcResult Result;
        string RpStr = MyAgentResponse;
        MyAgentResponse.clear();

        // 响应信息处理结果
        switch (ProcResponse(RpStr))
        {
            case BeCommand::COMMAND_RESULT_FINISH:
            {
                Result = Task::ACTION_PROC_RESULT_SUCCESS;
                break;
            }
            case BeCommand::COMMAND_RESULT_PROCESSING:
            case BeCommand::COMMAND_RESULT_UNKNOWN:
            {
                Result = Task::ACTION_PROC_RESULT_WAIT;
                break;
            }
            case BeCommand::COMMAND_RESULT_BUSY:
            case BeCommand::COMMAND_RESULT_FAIL:
            {
                Result = Task::ACTION_PROC_RESULT_FAIL;
                break;
            }
            case BeCommand::COMMAND_RESULT_ERROR:
            {
                Result = Task::ACTION_PROC_RESULT_ERROR;
                break;
            }
            default:
            {
                Result = Task::ACTION_PROC_RESULT_FAIL;
                break;
            }
        }

        // 如果为windows和linux的初始复制，则将失败更改为成功，进入后处理过程
        if (BeCommand::IsWindowsOrLinuxInitCopyCommand(MyCmdCode) == true)
        {
            if (Result == Task::ACTION_PROC_RESULT_FAIL)
            {
                // 参数:主机编码;盘符
                string HCode = ParaArray[0];

                // 获取最新副本id
                string CopyId = MyDbDataManager->GetFinalCopyId(HCode, ParaArray[1]);

                // 更新副本状态
                MyDbDataManager->UpdateCopyStatus(HCode, CopyId, DbDataManager::COPY_STATE_UNUSED);

                Result = Task::ACTION_PROC_RESULT_SUCCESS;
            }
        }

        return Result;
    }
    return Task::ACTION_PROC_RESULT_SUCCESS;
}

/*
 *  功能：
 *      任务后处理
 *  返回：
 *      处理成功返回true，否则返回false
 */
bool Task::PostProcessing()
{/*
    // 设置超时检查标志为false
    MyStartCheckTimeoutFlag = false;

    // 更新任务状态
    UpdateTaskStateDesc(TaskProcDesc::TASK_PROC_POST_PROCESSING);

    // 对于重启BES和关闭BES，则更新任务状态描述为空
    if (MyCmdCode == DEFAULT_COMMAND_REBOOT_BES
            || MyCmdCode == DEFAULT_COMMAND_SHUTDOWN_BES)
    {
        MyDbDataManager->UpdateTaskStateDesc(MyTaskId, DEFAULT_EMPTY_STRING);
    }

    //    if (IsDebugCommand() == true)
    //    {
    //        BesLog::DebugPrint(string("[PostProcessing]****") + MyCmdCode);
    //    }

    vector<string> ParaArray = BeGeneralLib::StringSplit(MyParameter, COMMAND_PARAMETER_SEPCHAR, BeGeneralLib::WITH_EMPTY_ITEM);
    string FirstCode = MyCmdCode.substr(0, 1);
    if (FirstCode == DEFAULT_QUERY_COMMAND_PREFIX)
    {
        if (MyCmdCode == DEFAULT_COMMAND_QUERY_HOST_INFORMATION) // 查询主机信息
        {
            // 参数：主机编码;主机IP;数据库用户名;数据库密码;服务器IP;
            string HCode = ParaArray[0];

            // 更新主机为在线状态
            if (MyDbDataManager->GetHostProtectedStatus(HCode) == DbDataManager::HOST_PROTECTED_STATUS_OFFLINE)
            {
                MyDbDataManager->UpdateHostProtectedStatus(HCode, DbDataManager::HOST_PROTECTED_STATUS_LOCAL);
            }

            // 更新主机网卡的虚拟mac
            UpdateHostNicVirtualMac(HCode, true);

            //

            // 更新DHCP配置
            NetworkManager::OperateDHCP(NetworkManager::DHCP_ACTION_INIT, MyDbDataManager);
        }
    }
    else if (FirstCode == DEFAULT_COPY_COMMAND_PREFIX)
    {
        if (BeCommand::IsWindowsOrLinuxInitCopyCommand(MyCmdCode) == true // windows或linux初始复制
                || MyCmdCode == DEFAULT_COMMAND_FULL_COPY) // 全量复制
        {

            // 参数:主机编码;盘符
            string HCode = ParaArray[0];
            string StrategyId = ParaArray[1];

            bool IsCreateNew = false;
            if (MyCmdCode == DEFAULT_COMMAND_FULL_COPY) // 全量复制
            {
                IsCreateNew = BeGeneralLib::StringToBool(ParaArray[2]);
            }

            // 初始复制后处理
            DoInitializeCopyPost(HCode, StrategyId, IsCreateNew);
        }
        else if (BeCommand::IsWindowsOrLinuxSnapshotCopyCommand(MyCmdCode) == true) // windows或linux快照复制
        {
            // 参数:主机编码;盘符;
            string HCode = ParaArray[0];
            string StrategyId = ParaArray[1];

            // 获取主机信息
            DbDataManager::HostProtectedStatus HostStatus = MyDbDataManager->GetHostProtectedStatus(HCode);
            DbDataManager::HostOSType OsType = MyDbDataManager->GetHostOSType(HCode);
            DbDataManager::VolumeType CurrentVolumeType = MyDbDataManager->GetCurrentVolumeType(HCode, StrategyId);
            bool IsUsedForCDP = MyDbDataManager->IsStrategyUsedForCDP(HCode, StrategyId);

            // 如果为虚拟机启动，并且为系统盘，则通知主机进行注册表备份
            if ((HostStatus == DbDataManager::HOST_PROTECTED_STATUS_VM)
                    && (MyDbDataManager->IsBootPartitionType(HCode, StrategyId) == true))
            {
                string Parameter = BeCommand::CreateCommandParameter(1, HCode.c_str());
                StartSubTask(DEFAULT_COMMAND_NOTIFY_HOST_BACKUP_REGISTER, Parameter, HCode, MyHostIp, MyHostPort);
            }

            // 获取应急副本id
            string CopyId = MyDbDataManager->GetEmergencyCopyId(HCode, StrategyId);

            // 删除主机应急配置
            DeleteHostEmergencyConfiguration(HCode, CopyId);

            // 进行快照后处理
            DoSnapshotPost(HCode, StrategyId, CopyId, false);

            // 获取主机复制方式，如果为ssh，则将磁盘卸载到本地
            if (MyDbDataManager->GetHostCopyMode(HCode) == DbDataManager::HOST_COPY_MODE_RDS)
            {
                // 注销target
                MyScsiManager->LogoffTarget(MyDbDataManager->GetCopyIqn(HCode, CopyId));
            }

            // 如果为应急模式
            if ((HostStatus == DbDataManager::HOST_PROTECTED_STATUS_VM)
                    || ((HostStatus == DbDataManager::HOST_PROTECTED_STATUS_NET || HostStatus == DbDataManager::HOST_PROTECTED_STATUS_FCBOOT) && (CurrentVolumeType != DbDataManager::VOLUME_TYPE_LOCAL || OsType == DbDataManager::HOST_OS_TYPE_LINUX))
                    || (IsUsedForCDP == true))
            {
                // 如果为数据库复制或调度复制，则需要通知主机数据库退出备份模式
                if (MyCmdCode == DEFAULT_COMMAND_WINDOWS_DATABASE_COPY || MyCmdCode == DEFAULT_COMMAND_LINUX_DATABASE_COPY || MyCmdCode == DEFAULT_COMMAND_SCHEDULE_SNAPSHOT_COPY)
                {
                    bool ExitBackupMode = true;
                    if (MyCmdCode == DEFAULT_COMMAND_SCHEDULE_SNAPSHOT_COPY)
                    {
                        string ScheduleId = ParaArray[2];
                        if (!MyDbDataManager->IsDbProtected(HCode) || !MyDbDataManager->IsStrategyHasDb(StrategyId, ScheduleId))
                        {
                            ExitBackupMode = false;
                        }
                    }

                    // 通知主机数据库退出备份模式
                    if (ExitBackupMode)
                    {
                        string Parameter = BeCommand::CreateCommandParameter(1, HCode.c_str());
                        StartSubTask(DEFAULT_COMMAND_NOTIFY_HOST_EXIT_BACKUP_MODE, Parameter, HCode, MyHostIp, MyHostPort);
                    }
                }
            }
            else
            {
                // 复制后的target映射
                DoCopyPostTargetMapping(HCode, StrategyId, CopyId);
            }

            if (HostStatus == DbDataManager::HOST_PROTECTED_STATUS_LOCAL
                    || ((HostStatus == DbDataManager::HOST_PROTECTED_STATUS_NET || HostStatus == DbDataManager::HOST_PROTECTED_STATUS_FCBOOT) && (CurrentVolumeType != DbDataManager::VOLUME_TYPE_LOCAL || OsType == DbDataManager::HOST_OS_TYPE_LINUX))
                    || (IsUsedForCDP == false))
            {
                // 设置副本为锁定状态
                SetCopyLocked(HCode, CopyId, true);
            }
        }
        else if (MyCmdCode == DEFAULT_COMMAND_AIX_INITIALIZE_COPY) // AIX初始复制
        {
            // 参数:主机编码;策略id;
            string HCode = ParaArray[0];
            string StrategyId = ParaArray[1];

            // 镜像卷id
            string MirrorLunId = MyDbDataManager->GetMirrorLunId(HCode, StrategyId);

            // 镜像卷的镜像卷id
            string MMLunId = MyDbDataManager->GetMMLunId(HCode, StrategyId);

            // 创建卷对
            string VolumePairId = MyDbDataManager->GetVolumePairId(HCode, StrategyId);
            if (BeGeneralLib::IsStringEmptyOrInvalidValue(VolumePairId) == true)
            {
                VolumePairId = MyFCManager->AddVolumePair(FCManager::VOLUME_PAIR_MODE_MIRROR, FCManager::LUN_MAP_MODE_PARTITION, MirrorLunId, MMLunId);
                if (BeGeneralLib::IsStringEmptyOrInvalidValue(VolumePairId) == true) // 卷对创建失败
                {
                    MyLastError->SetError(BesError::BES_ERR_FC_CREATE_VOLUME_PAIR_FAILED, HCode + SEPARATOR_CHAR_COLON + StrategyId);
                    return false;
                }

                // 更新卷对id
                MyDbDataManager->UpdateStrategyVolumePairId(StrategyId, VolumePairId);
            }

            // 创建快照零
            MyDbDataManager->AddSnapshotRecord(HCode, StrategyId, DbDataManager::SNAPSHOT_TYPE_ZERO);

            // 创建快照
            string SnapshotLunId = MyFCManager->AddSnapshotImage(MMLunId);

            // 更新快照时间
            MyDbDataManager->UpdateFinalSnapshotEndTime(HCode, StrategyId, SnapshotLunId);

            // 设置主机状态为可网启
            MyDbDataManager->UpdateHostCanEmergencyStatus(HCode, DbDataManager::HOST_CAN_EMERGENCY_STATUS_YES);
        }
        else if (MyCmdCode == DEFAULT_COMMAND_AIX_SNAPSHOT_COPY // AIX立即快照复制
                || MyCmdCode == DEFAULT_COMMAND_AIX_SCHEDULE_SNAPSHOT_COPY) // AIX调度快照复制
        {
            // 参数:主机编码;策略id;
            string HCode = ParaArray[0];
            string StrategyId = ParaArray[1];

            // 镜像卷的镜像卷id
            string MMLunId = MyDbDataManager->GetMMLunId(HCode, StrategyId);

            // 创建快照
            MyDbDataManager->AddSnapshotRecord(HCode, StrategyId, DbDataManager::SNAPSHOT_TYPE_VOLUME);

            // 创建快照
            string SnapshotLunId = MyFCManager->AddSnapshotImage(MMLunId);

            // 更新快照时间
            MyDbDataManager->UpdateFinalSnapshotEndTime(HCode, StrategyId, SnapshotLunId);
        }
    }
    else if (FirstCode == DEFAULT_RECOVER_COMMAND_PREFIX)
    {
        if (MyCmdCode == DEFAULT_COMMAND_LINUX_DATA_RECOVER) // linux数据盘手工恢复
        {
            // 参数:主机编码;策略id;
            string HCode = ParaArray[0];
            string StrategyId = ParaArray[1];

            // 获取主机复制方式，如果为ssh，则将磁盘卸载到本地
            if (MyDbDataManager->GetHostCopyMode(HCode) == DbDataManager::HOST_COPY_MODE_RDS)
            {
                // 获取应急副本id
                string CopyId = MyDbDataManager->GetEmergencyCopyId(HCode, StrategyId);

                // 获取副本iqn
                string Iqn = MyDbDataManager->GetCopyIqn(HCode, CopyId);

                // 注销target
                MyScsiManager->LogoffTarget(Iqn, DEFAULT_LOCALHOST_IP);
            }
        }
        else if (MyCmdCode == DEFAULT_COMMAND_AIX_DATA_RECOVER) // AIX数据恢复
        {
            // 参数:主机编码;策略id;
            string HCode = ParaArray[0];
            string StrategyId = ParaArray[1];

            // 镜像卷的镜像卷id
            string MMLunId = MyDbDataManager->GetMMLunId(HCode, StrategyId);

            // 创建快照
            MyDbDataManager->AddSnapshotRecord(HCode, StrategyId, DbDataManager::SNAPSHOT_TYPE_VOLUME);

            // 创建快照
            string SnapshotLunId = MyFCManager->AddSnapshotImage(MMLunId);

            // 更新快照时间
            MyDbDataManager->UpdateFinalSnapshotEndTime(HCode, StrategyId, SnapshotLunId);

            // 解除镜像卷对
            string VolumePairId = MyDbDataManager->GetVolumePairId(HCode, StrategyId);
            MyFCManager->DeleteVolumePair(VolumePairId);

            // 获取镜像卷id
            string MirrorLunId = MyDbDataManager->GetMirrorLunId(HCode, StrategyId);

            // 关闭主机镜像卷映射
            MyFCManager->DeleteLunMap(MirrorLunId, FCManager::LUN_MAP_MODE_PARTITION);

            // 获取副本零id
            string ZeroCopyId = MyDbDataManager->GetZeroCopyId(HCode, StrategyId);

            // 获取副本卷id
            string CopyLunId = MyDbDataManager->GetCopyLunId(HCode, ZeroCopyId);
            //
            string Channel = MyDbDataManager->GetHostFCChannel(HCode);
            if (BeGeneralLib::IsStringEmptyOrInvalidValue(Channel) == true)
            {
                Channel = DEFAULT_FC_CHANNEL_NUMBER;
            }

            switch (MyDbDataManager->GetHostOSType(HCode))
            {
                case DbDataManager::HOST_OS_TYPE_AIX:
                {
                    // 获取最新快照id
                    string SnapshotId = MyDbDataManager->GetFinalSnapshotId(HCode, StrategyId);

                    // 更新副本快照id
                    MyDbDataManager->UpdateCopySnapshotId(HCode, ZeroCopyId, SnapshotId);

                    // 交换副本卷和mirror卷的id
                    MyDbDataManager->UpdateCopyLunId(HCode, ZeroCopyId, MirrorLunId);

                    MyDbDataManager->UpdateStrategyMirrorVolumeId(StrategyId, CopyLunId);

                    // 重新创建镜像卷对
                    VolumePairId = MyFCManager->AddVolumePair(FCManager::VOLUME_PAIR_MODE_MIRROR, FCManager::LUN_MAP_MODE_PARTITION, CopyLunId, MMLunId);
                    MyDbDataManager->UpdateStrategyVolumePairId(StrategyId, VolumePairId);

                    // 重新设置镜像卷映射
                    MyFCManager->AddLunMap(MyDbDataManager->GetHostWWN(HCode), CopyLunId, FCManager::LUN_MAP_MODE_PARTITION, Channel);
                    break;
                }
                case DbDataManager::HOST_OS_TYPE_HPUX:
                {
                    // 创建卷复制对
                    string TempVolumePairId = MyFCManager->AddVolumePair(FCManager::VOLUME_PAIR_MODE_COPY, FCManager::LUN_MAP_MODE_PARTITION, CopyLunId, MirrorLunId);
                    MyFCManager->DeleteVolumePair(TempVolumePairId);

                    // 重新创建镜像卷对
                    VolumePairId = MyFCManager->AddVolumePair(FCManager::VOLUME_PAIR_MODE_MIRROR, FCManager::LUN_MAP_MODE_PARTITION, MirrorLunId, MMLunId);
                    MyDbDataManager->UpdateStrategyVolumePairId(StrategyId, VolumePairId);

                    // 重新设置镜像卷映射
                    MyFCManager->AddLunMap(MirrorLunId, FCManager::LUN_MAP_MODE_PARTITION, Channel, "0");
                    break;
                }
            }
        }
    }
    else if (FirstCode == DEFAULT_DISK_OPERATION_COMMAND_PREFIX)
    {
        if (MyCmdCode == DEFAULT_COMMAND_UMOUNT_NET_DISK) // 卸载网络盘
        {
            // 参数:主机编码;iqn;
            string HCode = ParaArray[0];
            string Iqn = ParaArray[1];

            // 获取主机操作系统类型
            DbDataManager::HostOSType OSType = MyDbDataManager->GetHostOSType(HCode);

            // 获取副本id
            string CopyId = MyDbDataManager->GetCopyId(HCode, Iqn, DbDataManager::COPY_ID_RETRIEVE_TYPE_IQN);
            string LMountPoint = MyDbDataManager->GetCopyLMountPoint(HCode, CopyId);

            // 检查该网络盘是否在使用中
            if (CheckCopyValidation(HCode, CopyId, (Task::CopyCheckItem)(Task::COPY_CHECK_ITEM_NOT_USED)) == false)
            {
                return false;
            }

            // 设置Target模式
            TargetMode TgtMode = GetTargetModeByEmergencyProtocol(HCode);
            if (MyDbDataManager->IsStrategyUsedForCDP(HCode, LMountPoint) == true && (MyDbDataManager->IsCopyAsEmergencyCopy(HCode, CopyId) == true))
            {
                // 如果为网络、光纤应急、本地
                if (CheckHostStateValidation(HCode, (Task::HostStateCheckItem)(Task::HOST_STATE_CHECK_ITEM_IS_NET_EMERGENCY | Task::HOST_STATE_CHECK_ITEM_IS_FC_EMERGENCY | Task::HOST_STATE_CHECK_ITEM_IS_LOCAL), false) == true)
                {
                    TgtMode = GetTargetModeByCopyMode(HCode);
                }
            }

            // 设置磁盘为注销
            if ((MyDbDataManager->IsCopyAsEmergencyCopy(HCode, CopyId) == false)
                    && DoCopyTargetMapping(HCode, CopyId, TgtMode, false) == false)
            {
                return false;
            }

            // 设置副本为锁定状态
            SetCopyLocked(HCode, CopyId, true);
        }
    }
    else if (FirstCode == DEFAULT_NOTIFY_COMMAND_PREFIX){ // 通知类
    	if (MyCmdCode == DEFAULT_COMMAND_ADD_DB_IMAGE_PROTECT){	// 531 添加数据库镜像保护
    		string HCode = ParaArray[0];
    		string MIRRORSERVERIP = ParaArray[1];
    		string MIRRORDBIP = ParaArray[2];
    		string MIRRORMODE = ParaArray[3];
    		MyDbDataManager->UpdateDbImageProtect(HCode, MIRRORSERVERIP, MIRRORDBIP, MIRRORMODE, DbDataManager::DB_MIRROR_STATE_USED);	//531 镜像状态改为启用
    	}
    	if (MyCmdCode == DEFAULT_COMMAND_START_DB_IMAGE_PROTECT){	// 532 通知数据库镜像应急
    		string HCode = ParaArray[0];
    		MyDbDataManager->UpdateDbMirrorState(HCode, DbDataManager::DB_MIRROR_STATE_TAKEOVER);		// 应急成功，把镜像状态改为应急
    	}
    	if (MyCmdCode == DEFAULT_COMMAND_STOP_DB_IMAGE_PROTECT){	// 533 通知取消数据库镜像应急
    		string HCode = ParaArray[0];
    		MyDbDataManager->UpdateDbMirrorState(HCode, DbDataManager::DB_MIRROR_STATE_UNUSED);		// 取消应急成功，把镜像状态改为未启用
    	}
    	if (MyCmdCode == DEFAULT_COMMAND_QUERY_DB_IMAGE_STATUS){	// 008 查询数据库镜像状态
    		string HCode = ParaArray[0];
    		MyDbDataManager->UpdateDbMirrorState(HCode, DbDataManager::DB_MIRROR_STATE_TAKEOVER);		// 成功，把镜像状态改为应急
    	}
    }*/
    return true;
}

/*
 *  功能：
 *      错误处理
 *  参数：
 *      无
 *  返回：
 *      无
 */
void Task::ErrorProcessing()
{/*
    UpdateTaskStateDesc(TaskProcDesc::TASK_PROC_ERROR_PROCESSING);

    //    if (IsDebugCommand() == true)
    //    {
    //        BesLog::DebugPrint(string("[ErrorProcessing]****") + MyCmdCode);
    //    }
    vector<string> ParaArray = BeGeneralLib::StringSplit(MyParameter, COMMAND_PARAMETER_SEPCHAR, BeGeneralLib::WITH_EMPTY_ITEM);
    string FirstCode = MyCmdCode.substr(0, 1);

    MyExecWithErrorFlag = true;

    BesError::ErrorType LastErrorType = MyLastError->GetErrorType();

    if (FirstCode == DEFAULT_QUERY_COMMAND_PREFIX)	//0**
    {
    	if (MyCmdCode == DEFAULT_COMMAND_QUERY_DB_IMAGE_STATUS){	// 008 查询数据库镜像状态
    		string HCode = ParaArray[0];
    		MyDbDataManager->UpdateDbMirrorState(HCode, DbDataManager::DB_MIRROR_STATE_UNUSED);		// 把镜像状态改为未启用
    	}
    }
    else if (FirstCode == DEFAULT_COPY_COMMAND_PREFIX)	//1**
    {
        if (BeCommand::IsWindowsOrLinuxInitCopyCommand(MyCmdCode) == true) // windows或linux初始复制  参数:主机编码;盘符
        {
            string HCode = ParaArray[0];
            string VolumeSymbol = ParaArray[1];

            if (MyLastTaskExecStep == Task::TASK_EXEC_STEP_PRE_PROCESSING && (LastErrorType == BesError::BES_ERR_TARGET_CREATE_FAILED || LastErrorType == BesError::BES_ERR_COPY_CREATE_FAILED)) // 创建Target失败
            {
                RollbackInitializeCopy(HCode, VolumeSymbol, true);
            }

            if (MyLastTaskExecStep == Task::TASK_EXEC_STEP_POST_PROCESSING
                    && (LastErrorType == BesError::BES_ERR_TARGET_LOGOFF_FAILED
                    || LastErrorType == BesError::BES_ERR_TARGET_LOGON_FAILED
                    || LastErrorType == BesError::BES_ERR_TARGET_MOUNT_FAILED))
            {
                RollbackInitializeCopy(HCode, VolumeSymbol, true);
            }

        }
        else if (BeCommand::IsWindowsOrLinuxSnapshotCopyCommand(MyCmdCode) == true)
        {
            string HCode = ParaArray[0];
            string VolumeSymbol = ParaArray[1];

            switch (MyLastTaskExecStep)
            {
                case Task::TASK_EXEC_STEP_EXCUTE:
                {
                    switch (MyLastError->GetErrorType())
                    {
                        case BesError::BES_ERR_SOCKET_OPEN_FAILED: // 打开套接字错误
                        case BesError::BES_ERR_AGENT_NO_RESPONSE: // 无响应
                        case BesError::BES_ERR_AGENT_CONNECT_FAILED: // 连接失败
                        case BesError::BES_ERR_AGENT_IS_BUSY:
                        case BesError::BES_ERR_AGENT_TASK_FAILED:
                        case BesError::BES_ERR_AGENT_RECEIVE_TIMEOUT:
                        {
                            RollbackIncreasementCopy(HCode, VolumeSymbol);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case Task::TASK_EXEC_STEP_WAIT_EXCUTING_RESULT:
                {
                    switch (MyLastError->GetErrorType())
                    {
                        case BesError::BES_ERR_COMMAND_INVALID_RESPONSE: // 无效响应
                        case BesError::BES_ERR_COMMAND_INVALID_TAILOR: // 无效结束符
                        case BesError::BES_ERR_AGENT_TASK_FAILED: // AGENT任务错误
                        case BesError::BES_ERR_AGENT_IS_BUSY: // AGENT繁忙
                        case BesError::BES_ERR_AGENT_RECEIVE_TIMEOUT:
                        {
                            RollbackIncreasementCopy(HCode, VolumeSymbol);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                case Task::TASK_EXEC_STEP_POST_PROCESSING:
                {
                    switch (MyLastError->GetErrorType())
                    {
                        case BesError::BES_ERR_SHARE_FOLDER_IS_EMPTY:
                        case BesError::BES_ERR_TARGET_MOUNT_FAILED:
                        case BesError::BES_ERR_SHARE_CLOSE_FAILED:
                        case BesError::BES_ERR_COPY_NO_SPACE:
                        case BesError::BES_ERR_COPY_FOLDER_IS_EMPTY:
                        {
                            RollbackIncreasementCopy(HCode, VolumeSymbol);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else if (MyCmdCode == DEFAULT_COMMAND_FULL_COPY)
        {
            // 参数:主机编码;盘符
            string HCode = ParaArray[0];
            string StrategyId = ParaArray[1];
            bool IsCreateNew = BeGeneralLib::StringToBool(ParaArray[2]);

            switch (MyLastTaskExecStep)
            {
                case Task::TASK_EXEC_STEP_PRE_PROCESSING:
                {
                    if (LastErrorType == BesError::BES_ERR_TARGET_CREATE_FAILED) // 创建Target失败
                    {
                        RollbackInitializeCopy(HCode, StrategyId, IsCreateNew);
                    }
                    break;
                }
                case Task::TASK_EXEC_STEP_EXCUTE:
                {
                    switch (MyLastError->GetErrorType())
                    {
                        case BesError::BES_ERR_SOCKET_OPEN_FAILED: // 打开套接字错误
                        case BesError::BES_ERR_AGENT_NO_RESPONSE: // 无响应
                        case BesError::BES_ERR_AGENT_CONNECT_FAILED: // 连接失败
                        case BesError::BES_ERR_AGENT_TASK_FAILED:
                        case BesError::BES_ERR_AGENT_IS_BUSY:
                        case BesError::BES_ERR_AGENT_RECEIVE_TIMEOUT:
                        {
                            RollbackInitializeCopy(HCode, StrategyId, IsCreateNew);
                            break;
                        }
                        default:
                        {
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }
    else if (FirstCode == DEFAULT_RECOVER_COMMAND_PREFIX)	//2**
    {
    }
    else if (FirstCode == DEFAULT_DISK_OPERATION_COMMAND_PREFIX)	//3**
    {
    }
    else if (FirstCode == DEFAULT_HOST_CONTROL_COMMAND_PREFIX)	//4**
    {
    }
    else if (FirstCode == DEFAULT_NOTIFY_COMMAND_PREFIX)	//5**
    {
    }

    else if (FirstCode == DEFAULT_INTERACT_COMMAND_PREFIX)	//6**
    {
    	if (MyCmdCode == DEFAULT_COMMAND_CREATE_DOCKER_IMAGE){	// 661 创建docker镜像
    		string HCode = ParaArray[0];
    		string CopyId = ParaArray[1];
    		string TargetFile = MyDbDataManager->GetCopyTargetFile(HCode, CopyId);
    		BeGeneralLib::FreeFileLoopDevice(TargetFile, true);	// 卸载副本、删除挂载目录、释放系统副本的loop设备
    		MyDbDataManager->UpdateCopyStatus(HCode, CopyId, DbDataManager::COPY_STATE_UNUSED);	//创建镜像失败，把副本状态改为未使用
    	}
    	if (MyCmdCode == DEFAULT_COMMAND_START_DOCKER_EMERGENCY){	// 663 docker应急
    		string HCode = ParaArray[0];
    		//docker应急失败，卸载所有数据盘副本，状态改为未使用
    		sql::ResultSet * TempRs1 = MyDbDataManager->GetNonSystemEmergencyCopyResultSet(HCode);
    		if (TempRs1 != NULL && TempRs1->rowsCount() <= 8){
    			while (TempRs1->next()){
    				string CopyId = TempRs1->getString("SSCOPYID");
    				string TargetFile = TempRs1->getString("TARGETFILE");
    				BeGeneralLib::FreeFileLoopDevice(TargetFile, true);
 					MyDbDataManager->UpdateCopyStatus(HCode, CopyId, DbDataManager::COPY_STATE_UNUSED);
    			}
    		}
    		MyDbDataManager->DestroyResultSet(TempRs1);
    		if(MyTaskId == "")		//server自身产生的任务
    			MyDbDataManager->AddAlarm(HCode, "自动接管", "DOCKER应急失败", DbDataManager::ALARM_LEVEL_WARNING,
    											BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_LONG),
    											BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_LONG));
    	}
    }

    else if (FirstCode == DEFAULT_MANAGE_COMMAND_PREFIX)	//9**
    {
        if (MyCmdCode == DEFAULT_COMMAND_CREATE_NORMAL_COPY || // 生成快照副本
                MyCmdCode == DEFAULT_COMMAND_SET_EMERGENCY_COPY) // 指定应急副本
        {
            string HCode = ParaArray[0];
            string SnapshotID = ParaArray[1];

            if (MyLastTaskExecStep == Task::TASK_EXEC_STEP_EXCUTE)
            {
                if ((LastErrorType == BesError::BES_ERR_TARGET_CREATE_FAILED) //Target创建失败
                        || LastErrorType == BesError::BES_ERR_COPY_NO_SPACE)// 副本无空间
                {
                    // 获取副本ID

                    string CopyId = MyDbDataManager->GetCopyId(HCode, SnapshotID, DbDataManager::COPY_ID_RETRIEVE_TYPE_SNAPSHOTID);

                    // 删除副本
                    RemoveCopy(HCode, CopyId);
                }
            }
        }
    }*/
    return;
}

/*
 *  功能：
 *      转入结束
 *  参数：
 *      无
 *  返回：
 *      转入结束成功返回true，否则返回false
 */
bool Task::GotoFinish()
{
    //    if (IsDebugCommand() == true)
    //    {
    //        BesLog::DebugPrint(string("[GotoFinish]****") + MyCmdCode);
    //    }

    // 如果为任务检查失败，则可以直接结束
    if (MyLastTaskExecStep == Task::TASK_EXEC_STEP_VALIDATION_CHECK)
    {
        MyExecWithErrorFlag = true;
        return true;
    }

	if(MyExecWithErrorFlag && MyCmdCode == DEFAULT_COMMAND_START_VM_ONE_KEY){
		vector<string>ParaArray = BeGeneralLib::StringSplit(MyParameter, COMMAND_PARAMETER_SEPCHAR, BeGeneralLib::WITH_EMPTY_ITEM);
		string HCode = ParaArray[0];
		DbDataManager::HostProtectedStatus HostStatus = MyDbDataManager->GetHostProtectedStatus(HCode);
		if(HostStatus == DbDataManager::HOST_PROTECTED_STATUS_VMING) MyDbDataManager->UpdateHostProtectedStatus(HCode, DbDataManager::HOST_PROTECTED_STATUS_OFFLINE);
	}

    // 检查执行次数，如果超过则直接进入结束操作
    MyExecCounter++;
    if (MyExecCounter >= MyMaxCounter) return true;

    if (MyCmdCode == DEFAULT_COMMAND_REBOOT_BES // 重启BES主机
            || MyCmdCode == DEFAULT_COMMAND_SHUTDOWN_BES // 关闭BES主机
            || MyCmdCode == DEFAULT_COMMAND_DELETE_HOST // 删除主机
            || MyCmdCode == DEFAULT_COMMAND_RESET_HOST // 重置主机
            || MyCmdCode == DEFAULT_COMMAND_FORCE_DELETE_HOST) // 强制删除主机
    {
        return true;
    }
    return false;
}

/*
 *  功能：
 *      更新任务状态描述
 *  参数：
 *      type            :   状态描述类型
 *  返回：
 *      无
 */
void Task::UpdateTaskStateDesc(TaskProcDesc::TaskProcEnum type, string msg)
{
    string StateDesc = TaskProcDesc::GetTaskStageDescription(DESCRIPTION_LANG_CN, type);
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(StateDesc) == true)
    {
        return;
    }

    // 检查是否为空
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(msg) == false)
    {

        StateDesc = StateDesc + string("[") + msg + string("]");
    }
    MyDbDataManager->UpdateTaskStateDesc(MyTaskId, StateDesc);
}

/*
 *  功能：
 *      将命令发送到Agent
 *  参数
 *      hostip          :   主机IP
 *      command         :   命令字符串
 *      recvstring      :   接收的字符串
 *  返回：
 *      操作结果
 */
bool Task::PushToAgent(string hostip, string command, string & recvstring)
{
    bool Flag = false;
    char MsgBuf[DEFAULT_BUFFER_SIZE + 1];
    int Counter = 0;
    string RpString = "";
    recvstring = "";

    // 如果任务已经停止，则退出
    if (MyTaskControl == Task::TASK_CONTROL_STOP)
    {
        return false;
    }

    // 创建套接字
    int ClientSock = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientSock < 0)
    {
        BesLog::DebugPrint("Push agent sock failed", BesLog::LOG_MESSAGE_TYPE_ERROR);
        MyLastError->SetError(BesError::BES_ERR_SOCKET_OPEN_FAILED);
        return false;
    }

    // 初始化套接字地址
    struct sockaddr_in ClientAddr;
    bzero(&ClientAddr, sizeof (ClientAddr));
    ClientAddr.sin_family = AF_INET;
    ClientAddr.sin_port = htons((unsigned short) BeGeneralLib::StringToInt(MyHostPort));

    // 设置主机地址
    ClientAddr.sin_addr.s_addr = inet_addr(hostip.c_str());

    // 将结构剩下的部分清零
    bzero(&(ClientAddr.sin_zero), 8);

    // 设置开始检查超时时间和标志
    time(&MyStartCheckTime);
    MyStartCheckTimeoutFlag = true;
    while (Counter++ < MAX_CONNECT_AGENT_COUNT)
    {
        BesLog::DebugPrint("ConnectAgent:" + string(inet_ntoa(ClientAddr.sin_addr)) + string("\tPort:") + MyHostPort + string("\tcounter:") + BeGeneralLib::Number2String(Counter), BesLog::LOG_MESSAGE_TYPE_VERBOSE, false);
        // 设置连接超时时间
        struct timeval Timeout = {DEFAULT_SOCKET_CONNECT_TIMEOUT, 0};
        setsockopt(ClientSock, SOL_SOCKET, SO_SNDTIMEO, (char *) & Timeout, sizeof (struct timeval));

        int Res = connect(ClientSock, (struct sockaddr *) & ClientAddr, sizeof (struct sockaddr));

        if (Res == 0) // 连接成功
        {
            // 发送指令
            string CommandStr = command;
            send(ClientSock, CommandStr.c_str(), CommandStr.size(), 0);

            time(&MyStartCheckTime);
            MyStartCheckTimeoutFlag = true;

            // 设置接收超时
            Timeout.tv_sec = 3;
            Timeout.tv_usec = 0;
            setsockopt(ClientSock, SOL_SOCKET, SO_RCVTIMEO, (char *) & Timeout, sizeof (struct timeval));

            // 接收数据
            bzero(MsgBuf, sizeof (MsgBuf));
            int RecvLen = recv(ClientSock, MsgBuf, sizeof (MsgBuf) - 1, 0);
            if (RecvLen > 0)
            {
                MsgBuf[RecvLen] = DEFAULT_C_STRING_END_FLAG;
                RpString = MsgBuf;

                BesLog::DebugPrint(string("RecvAgentMsg [") + string(inet_ntoa(ClientAddr.sin_addr)) + SEPARATOR_CHAR_COLON + MyCmdCode + string("]:") + RpString, BesLog::LOG_MESSAGE_TYPE_VERBOSE, false);
                time(&MyStartCheckTime);
                Flag = true;
                break;
            }
            else
            {
                BesLog::DebugPrint(string("No message received from agent [") + string(inet_ntoa(ClientAddr.sin_addr)) + SEPARATOR_CHAR_COLON + MyCmdCode + string("]!"), BesLog::LOG_MESSAGE_TYPE_ERROR);
                MyLastError->SetError(BesError::BES_ERR_AGENT_NO_RESPONSE, MyHostIp);
            }
        }
        else
        {
            BesLog::DebugPrint(string("Connect[") + string(inet_ntoa(ClientAddr.sin_addr)) + string("]failed!"), BesLog::LOG_MESSAGE_TYPE_VERBOSE, false);
            MyLastError->SetError(BesError::BES_ERR_AGENT_CONNECT_FAILED, MyHostIp);
        }

        // 如果任务已经停止，则退出
        if (MyTaskControl == Task::TASK_CONTROL_STOP)
        {
            break;
        }

        // 睡眠一段随机时间
        int SleepTime = 1 + (int) (2.0 * (rand() / (RAND_MAX + 1.0)));
        sleep(SleepTime);
    }

    // 关闭socket
    BeGeneralLib::CloseSock(ClientSock);
    recvstring = RpString;

    BesLog::DebugPrint(string("Push command [") + MyCmdCode + string("] to [") + string(inet_ntoa(ClientAddr.sin_addr)) + string("] finished!"), BesLog::LOG_MESSAGE_TYPE_VERBOSE, false);

    return Flag;
}

/*
 *  功能：
 *      处理回应信息
 *  参数：
 *      rpstr       :   回应信息串
 *  返回：
 *      返回处理Agent响应结果
 */
BeCommand::CommandResult Task::ProcResponse(string rpstr)
{
    BeCommand::CommandResult CmdResult = BeCommand::COMMAND_RESULT_UNKNOWN;

    // 检查命令格式是否合法
    BeCommand::CommandStruct* CmdStructPtr = new BeCommand::CommandStruct;
    BeCommand::InitCommandStruct(CmdStructPtr);
    if (BeCommand::Parse(rpstr, CmdStructPtr) == false
            || CmdStructPtr->Type != BeCommand::COMMAND_TYPE_RESPONSE)
    {
        MyLastError->SetError(BesError::BES_ERR_COMMAND_INVALID_RESPONSE, rpstr);
        delete CmdStructPtr;
        return BeCommand::COMMAND_RESULT_UNKNOWN;
    }

    // 处理指令头
    if (CmdStructPtr->Header != MyCmdHeader)
    {
        delete CmdStructPtr;
        return BeCommand::COMMAND_RESULT_UNKNOWN;
    }

    // 重置任务开始时间
    time(&MyStartCheckTime);

    // 应答内容
    string RpContentStr = CmdStructPtr->ParaOrContent;
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(RpContentStr) == true)
    {
        RpContentStr = DEFAULT_EMPTY_STRING;
    }

    CmdResult = CmdStructPtr->Result;
    switch (CmdResult)
    {
        case BeCommand::COMMAND_RESULT_ACK:
        case BeCommand::COMMAND_RESULT_FINISH:
        {
            break;
        }
        case BeCommand::COMMAND_RESULT_ERROR:
        {
            MyLastError->SetError(BesError::BES_ERR_AGENT_TASK_ERROR, RpContentStr);
            break;
        }
        case BeCommand::COMMAND_RESULT_BUSY:
        {
            MyLastError->SetError(BesError::BES_ERR_AGENT_IS_BUSY);
            break;
        }
        case BeCommand::COMMAND_RESULT_FAIL:
        {
            MyLastError->SetError(BesError::BES_ERR_AGENT_TASK_FAILED, RpContentStr);
            break;
        }
        case BeCommand::COMMAND_RESULT_PROCESSING:
        {
            if (BeGeneralLib::IsStringEmptyOrInvalidValue(RpContentStr) == false)
            {
                MyDbDataManager->UpdateTaskStateDesc(MyTaskId, RpContentStr);
            }
            else
            {
                UpdateTaskStateDesc(TaskProcDesc::TASK_PROC_PROCESS_RESPONSE);
            }
            break;
        }
        default:
        {
            MyLastError->SetError(BesError::BES_ERR_COMMAND_INVALID_RESPONSE, rpstr);
            break;
        }
    }
    delete CmdStructPtr;

    return CmdResult;
}

/*
 *  功能：
 *      发送命令并解析回应
 *  参数
 *      recvstring      :   接收的字符串
 *  返回：
 *      操作结果
 */
bool Task::PushAndProcResponse()
{

    return PushAndProcResponse(MyCommandStr);
}

/*
 *  功能：
 *      发送命令并解析回应
 *  参数
 *      command         :   命令字符串
 *      recvstring      :   接收的字符串
 *  返回：
 *      操作结果
 */
bool Task::PushAndProcResponse(string command)
{

    return PushAndProcResponse(MyHostIp, command);
}

/*
 *  功能：
 *      发送命令并解析回应
 *  参数
 *      command         :   命令字符串
 *      recvstring      :   接收的字符串
 *  返回：
 *      操作结果
 */
bool Task::PushAndProcResponse(string hostip, string command)
{
    string RecvString = "";
    if (PushToAgent(hostip, command, RecvString))
    {
        // Agent响应信息处理结果
        switch (ProcResponse(RecvString))
        {
            case BeCommand::COMMAND_RESULT_BUSY: return false;
            case BeCommand::COMMAND_RESULT_FAIL: return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

/*
 *  功能：
 *      打印任务执行时间
 *  参数：
 *      start           :   开始时间
 *      end             :   结束时间
 *  返回：
 *      无
 */
void Task::PrintTaskTime(time_t start, time_t end)
{
    char Buf[DEFAULT_BUFFER_SIZE + 1];

    // 开始时间
    string StartTimeStr = BeGeneralLib::GetTimeString(start, BeGeneralLib::TIME_FORMAT_TIME);

    // 结束时间
    string EndTimeStr = BeGeneralLib::GetTimeString(end, BeGeneralLib::TIME_FORMAT_TIME);

    // 计算差值
    long int Interval = abs(end - start);
    bzero(Buf, sizeof (Buf));
    sprintf(Buf, "%02d:%02d:%02d\0", Interval / 3600, (Interval % 3600) / 60, Interval % 60);
    string CostTimeStr = string(Buf);

    string FirstCode = MyCmdCode.substr(0, 1);
    if (FirstCode != DEFAULT_QUERY_COMMAND_PREFIX && MyCmdCode != DEFAULT_COMMAND_DO_TIMING_SNAPSHOT)
    {
        cout << "\33[36m";
        BesLog::DebugPrint("================================================");
        BesLog::DebugPrint("            Command Code:  " + MyCmdCode);
        BesLog::DebugPrint("            Start Time  :  " + StartTimeStr);
        BesLog::DebugPrint("            End Time    :  " + EndTimeStr);
        BesLog::DebugPrint("            Total Time  :  " + CostTimeStr);
        BesLog::DebugPrint("================================================");
        cout << "\33[0m";
    }

    UpdateTaskStateDesc(TaskProcDesc::TASK_PROC_COMPLETED, CostTimeStr);
}

/*
 *  功能：
 *      执行子任务
 *  参数：
 *      cmdcode         :   命令字
 *      parameter       :   命令参数
 *      hcode           :   主机编码
 *      hostip          :   主机ip
 *      hostport        :   主机端口
 *  返回：
 *      子任务执行结果
 */
Task::TaskExecResult Task::StartSubTask(string cmdcode, string parameter, string hcode, string hostip, string hostport)
{

    return StartSubTask("", cmdcode, parameter, hcode, hostip, hostport);
}

/*
 *  功能：
 *      执行子任务
 *  参数：
 *      cmdheader       :   命令头
 *      cmdcode         :   命令字
 *      parameter       :   命令参数
 *      hcode           :   主机编码
 *      hostip          :   主机ip
 *      hostport        :   主机端口
 *  返回：
 *      子任务执行结果
 */
Task::TaskExecResult Task::StartSubTask(string cmdheader, string cmdcode, string parameter, string hcode, string hostip, string hostport)
{
    // 更新父任务的运行状态
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(MyTaskId) == false)
    {
        string TaskStateString = MyDbDataManager->GetCommandDescription(cmdcode) + TaskProcDesc::GetTaskStageDescription(DESCRIPTION_LANG_CN, TaskProcDesc::TASK_PROC_IN_PROCESSING);
        MyDbDataManager->UpdateTaskStateDesc(MyTaskId, TaskStateString);
    }

    MySubTask = new Task();
    MySubTask->Add(MyDbDataManager, MyCommandManager, this, "", cmdheader, cmdcode, parameter, hcode, hostip, hostport);
    MySubTask->Start();
    Task::TaskExecResult TaskResult = MySubTask->Result();

    delete MySubTask;
    MySubTask = NULL;

    return TaskResult;
}

/*
 *  功能：
 *      通知agent中断任务
 *  参数：
 *      无
 *  返回：
 *      无
 */
void Task::NotifyAgentToBreakTask()
{
    string CommandString = BeCommand::CreateStandardCommand(COMMAND_CODE_ID, DEFAULT_COMMAND_NOTIFY_HOST_BREAK_TASK, "");
    sql::ResultSet * TempRs = MyDbDataManager->GetHostResultSet();
    if (TempRs != NULL && TempRs->rowsCount() > 0)
    {
        while (TempRs->next())
        {
            string RecvStr = "";
            PushToAgent(TempRs->getString("IPADDR"), CommandString, RecvStr);
        }
    }

    // 销毁结果集
    MyDbDataManager->DestroyResultSet(TempRs);

    return;
}

/*
 *  功能：
 *      获取系统状态
 *  参数：
 *      无
 *  返回：
 *      状态信息
 */
string Task::GetSystemStatus()
{
    string StatusString = "";

    // 获取CPU使用
    string ShellCommand = string("cpus=$(cat /proc/cpuinfo | grep processor | wc -l) && totals=$(ps aux | gawk '{total+=$3}; END{print total}') && echo \"scale=2;$totals/$cpus\" | bc");
    StatusString = string("CPU=") + BeGeneralLib::ReadShellReturnValue(ShellCommand) + SEPARATOR_CHAR_SEMICOLON;

    // 内存使用率
    ShellCommand = string("free | grep '^-/+' | gawk '{printf(\"%.1f\\n\"), $3 / ($3 + $4)*100}'");
    StatusString = StatusString + string("MEM=") + BeGeneralLib::ReadShellReturnValue(ShellCommand) + SEPARATOR_CHAR_SEMICOLON;

    // 磁盘使用率
    ShellCommand = string("df -Pl | grep '^/dev/' | gawk '{total += $2} {used += $3}; END {printf(\"%.1f\\n\"),  used/total * 100}'");
    StatusString = StatusString + string("DISK=") + BeGeneralLib::ReadShellReturnValue(ShellCommand) + SEPARATOR_CHAR_SEMICOLON;

    // 获取网络流量
    ShellCommand = string("cat /proc/net/dev | gawk '{if($1 ~ /eth/) print $0}' |  sed 's/^.*://' | gawk 'BEGIN{total=0;total1=0}{total+=$1;total1+=$9}END{print total,total1}'");
    vector<string> OldSubArray = BeGeneralLib::StringSplit(BeGeneralLib::ReadShellReturnValue(ShellCommand), SEPARATOR_CHAR_SPACE, BeGeneralLib::REMOVE_EMPTY_ITEM);
    sleep(1);
    vector<string> NewSubArray = BeGeneralLib::StringSplit(BeGeneralLib::ReadShellReturnValue(ShellCommand), SEPARATOR_CHAR_SPACE, BeGeneralLib::REMOVE_EMPTY_ITEM);

    string NetIn = BeGeneralLib::Number2String((long) (BeGeneralLib::StringToFloat(NewSubArray[0]) - BeGeneralLib::StringToFloat(OldSubArray[0])));
    string NetOut = BeGeneralLib::Number2String((long) (BeGeneralLib::StringToFloat(NewSubArray[1]) - BeGeneralLib::StringToFloat(OldSubArray[1])));
    StatusString = StatusString + string("NETRECV=") + NetIn + SEPARATOR_CHAR_SEMICOLON + string("NETSEND=") + NetOut + SEPARATOR_CHAR_SEMICOLON;

    return StatusString;
}

/*
 *  功能：
 *      验证监控用户名和密码
 *  参数：
 *      user            :   用户名
 *      pwd             :   密码
 *  返回：
 *      验证成功返回true，否则返回false
 */
bool Task::ValidateMonitorUserPassword(string user, string pwd)
{
    // 读取配置文件中的用户名和密码
    string MonitorUser = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_MONITORUSER);
    string MonitorPassword = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_MONITORPWD);

    // 对用户名和密码加密
    MonitorUser = BeGeneralLib::Cipher(BeGeneralLib::StringReverse(MonitorUser), MonitorUser.length() + BeGeneralLib::SumStringChars(MyCmdCode), true);
    MonitorPassword = BeGeneralLib::Cipher(BeGeneralLib::StringReverse(MonitorPassword), MonitorPassword.length() + BeGeneralLib::SumStringChars(MyCmdCode), true);

    if (MonitorUser != user || MonitorPassword != pwd)
    {

        return false;
    }
    return true;
}

/*
 *  功能：
 *      检查存储空间
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   盘符或策略编号
 *      oldvolumesize   :   原有卷空间大小
 *      newvolumesize   :   新的卷空间大小
 *  返回：
 *      检查成功返回true，否则返回false
 */
bool Task::CheckStorageSpace(string hcode, string strategyid, float oldvolumesize, float newvolumesize)
{
    string HCode = hcode;
    string StrategyId = strategyid;

    // 更新存储空间
    MyDbDataManager->UpdateStorageSpace();

    // 获取策略的存储空间id
    string StorageId = MyDbDataManager->GetStrategyStorageId(HCode, StrategyId);

    // 获取当前策略的空间总和
    float TotalUsedSpace = BeGeneralLib::StringToFloat(MyDbDataManager->GetUsedStorageSpace(StorageId)) - oldvolumesize + newvolumesize * 1.1;

    // 获取存储空间记录字段值
    map<string, string> FieldValueList = MyDbDataManager->GetStorageFieldValue(StorageId);
    float ValidStorageSpace = BeGeneralLib::StringToInt(FieldValueList["CAPACITY"]) * BeGeneralLib::StringToInt(FieldValueList["USEDLIMIT"]) / 100;

//	cout<<"oldsize:"<<oldvolumesize<<" newsize:"<<newvolumesize<<" totalused:"<<TotalUsedSpace<<" validspace:"<<ValidStorageSpace<<endl;

    if (TotalUsedSpace < ValidStorageSpace) return true;
    MyLastError->SetError(BesError::BES_ERR_COPY_NO_SPACE, HCode + SEPARATOR_CHAR_COLON + StrategyId);

    return false;
}

/*
 *  功能：
 *      检查主机协议是否相等
 *  参数：
 *      oldprotocol     :   旧的协议
 *      newprotocol     :   新的协议
 *  返回：
 *      如果相等则返回true，否则返回false
 */
bool Task::CheckHostProtocolEqual(string oldprotocol, string newprotocol)
{
    // 检查新旧协议是否为网启协议
    if ((oldprotocol == DEFAULT_BOOT_PROTOCOL_PXE || oldprotocol == DEFAULT_BOOT_PROTOCOL_SNSBOOT || oldprotocol == DEFAULT_BOOT_PROTOCOL_HBA)
            && (newprotocol == DEFAULT_BOOT_PROTOCOL_PXE || newprotocol == DEFAULT_BOOT_PROTOCOL_SNSBOOT || newprotocol == DEFAULT_BOOT_PROTOCOL_HBA))
    {
        return true;
    }

    // 检查新旧协议是否为虚拟机协议
    if ((oldprotocol == DEFAULT_BOOT_PROTOCOL_VMBOOT) && (newprotocol == DEFAULT_BOOT_PROTOCOL_VMBOOT))
    {
        return true;
    }

    // 检查新旧协议是否为光纤卡协议
    if ((oldprotocol == DEFAULT_BOOT_PROTOCOL_FCBOOT) && (newprotocol == DEFAULT_BOOT_PROTOCOL_FCBOOT))
    {

        return true;
    }

    return false;
}

/*
 *  功能：
 *      检查主机的有效性
 *  参数
 *      hcode           :   主机编码
 *      items           :   检查项
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckHostValidation(string hcode, Task::HostCheckItem items, bool isseterror)
{/*
    string HCode = hcode;

    // 检查主机是否存在
    if ((items & Task::HOST_CHECK_ITEM_HOST_EXIST) == Task::HOST_CHECK_ITEM_HOST_EXIST)
    {
        // 检查主机是否存在
        if (MyDbDataManager->IsHostExist(HCode) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_NOT_EXIST, HCode);
            }
            return false;
        }
    }

    // 检查本地应急是否打开
    if ((items & Task::HOST_CHECK_ITEM_LOCAL_EMERGENCY_IS_OPENED) == Task::HOST_CHECK_ITEM_LOCAL_EMERGENCY_IS_OPENED)
    {
        // 检查主机本地应急状态功能是否打开
        if (MyDbDataManager->IsHostLocalEmergencyOpen(HCode) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_LOCAL_EMERGENCY_FUNCTION_IS_CLOSED, HCode);
            }
            return false;
        }
    }

    // 检查主机编码是否不存在
    if ((items & Task::HOST_CHECK_ITEM_HCODE_NOT_EXIST) == Task::HOST_CHECK_ITEM_HCODE_NOT_EXIST)
    {
        // 检查主机编码是否不存在
        if (MyDbDataManager->IsHostExist(HCode) == true)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_CODE_EXIST, HCode);
            }
            return false;
        }
    }

    // 检查主机编码是否有效
    if ((items & Task::HOST_CHECK_ITEM_HCODE_IS_VALID) == Task::HOST_CHECK_ITEM_HCODE_IS_VALID)
    {
        // 检查主机编码是否符合长度
        if (HCode.size() != DEFAULT_HOST_CODE_SIZE)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_INVALID_HOST_CODE, HCode);
            }
            return false;
        }

        // 检查主机编码是否为合法的数字组成
        for (int i = 0; i < HCode.size(); i++)
        {
            if (isdigit(HCode[i]) == false || (i == 0 && HCode[i] == '0'))
            {
                if (isseterror == true)
                {
                    MyLastError->SetError(BesError::BES_ERR_INVALID_HOST_CODE, HCode);
                }
                return false;
            }
        }
    }

    // 检查主机是否是windows系统
    if ((items & Task::HOST_CHECK_ITEM_HOST_IS_WINDOWS) == Task::HOST_CHECK_ITEM_HOST_IS_WINDOWS)
    {
        // 检查主机是否是windows系统
        if (MyDbDataManager->GetHostOSType(HCode) != DbDataManager::HOST_OS_TYPE_WINDOWS)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_OSTYPE_NOT_MATCH_COMMAND_CODE, HCode + SEPARATOR_CHAR_COLON + MyCmdCode);
            }
            return false;
        }
    }

    // 检查主机是否是linux系统
    if ((items & Task::HOST_CHECK_ITEM_HOST_IS_LINUX) == Task::HOST_CHECK_ITEM_HOST_IS_LINUX)
    {
        // 检查主机是否是linux系统
        if (MyDbDataManager->GetHostOSType(HCode) != DbDataManager::HOST_OS_TYPE_LINUX)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_OSTYPE_NOT_MATCH_COMMAND_CODE, HCode + SEPARATOR_CHAR_COLON + MyCmdCode);
            }
            return false;
        }
    }

    // 检查主机是否是windows或linux系统
    if ((items & Task::HOST_CHECK_ITEM_HOST_IS_WINDOWS_OR_LINUX) == Task::HOST_CHECK_ITEM_HOST_IS_WINDOWS_OR_LINUX)
    {
        // 检查主机是否为不是应急状态
        if (MyDbDataManager->GetHostOSType(HCode) != DbDataManager::HOST_OS_TYPE_WINDOWS && MyDbDataManager->GetHostOSType(HCode) != DbDataManager::HOST_OS_TYPE_LINUX)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_OSTYPE_NOT_MATCH_COMMAND_CODE, HCode + SEPARATOR_CHAR_COLON + MyCmdCode);
            }
            return false;
        }
    }

    // 检查主机是否为unix系统
    if ((items & Task::HOST_CHECK_ITEM_HOST_IS_UNIX) == Task::HOST_CHECK_ITEM_HOST_IS_UNIX)
    {
        // 检查主机是否为unix系统
        if (MyDbDataManager->GetHostOSType(HCode) != DbDataManager::HOST_OS_TYPE_AIX && MyDbDataManager->GetHostOSType(HCode) != DbDataManager::HOST_OS_TYPE_HPUX)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_OSTYPE_NOT_MATCH_COMMAND_CODE, HCode + SEPARATOR_CHAR_COLON + MyCmdCode);
            }
            return false;
        }
    }

    // 检查副本是否在使用中
    if ((items & Task::HOST_CHECK_ITEM_COPY_USED) == Task::HOST_CHECK_ITEM_COPY_USED)
    {
        // 检查副本是否在使用中
        if (MyDbDataManager->IsHostHasCopyUsed(HCode) == true)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_COPY_IS_USED, HCode);
            }
            return false;
        }
    }

    // 检查是否有数据库保护
    if ((items & Task::HOST_CHECK_ITEM_DATABASE_IS_PROTECTED) == Task::HOST_CHECK_ITEM_DATABASE_IS_PROTECTED)
    {
        // 检查是否有数据库保护
        if (MyDbDataManager->IsDbProtected(HCode) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_NOT_PROTECT_DATABASE, HCode);
            }
            return false;
        }
    }

    // 检查系统卷是否已经保护
    if ((items & Task::HOST_CHECK_ITEM_SYSTEM_VOLUME_IS_PROTECTED) == Task::HOST_CHECK_ITEM_SYSTEM_VOLUME_IS_PROTECTED)
    {
        // 检查系统卷策略是否存在
        string LMountPoint = "";
        switch (MyDbDataManager->GetHostOSType(hcode))
        {
            case DbDataManager::HOST_OS_TYPE_WINDOWS:
            {
                LMountPoint = DEFAULT_WINDOWS_SYSTEM_FLAG;
                break;
            }
            case DbDataManager::HOST_OS_TYPE_LINUX:
            {
                LMountPoint = DEFAULT_LINUX_SYSTEM_FLAG;
                break;
            }
            case DbDataManager::HOST_OS_TYPE_AIX:
            case DbDataManager::HOST_OS_TYPE_HPUX:
            {
                LMountPoint = DEFAULT_AIX_SYSTEM_FLAG;
                break;
            }
        }

        // 检查主机系统卷对应的策略是否存在
        if (MyDbDataManager->IsVolumeHasStrategy(HCode, LMountPoint) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_STRATEGY_NOT_EXIST, HCode + SEPARATOR_CHAR_COLON + LMountPoint);
            }
            return false;
        }

        // 检查应急副本是否存在
        string SystemEmergencyCopyId = MyDbDataManager->GetSystemEmergencyCopyId(HCode);
        if (MyDbDataManager->IsCopyExist(HCode, SystemEmergencyCopyId) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_COPY_EMERGENCY_NOT_EXIST, HCode + SEPARATOR_CHAR_COLON + LMountPoint);
            }
            return false;
        }
    }

    // 检查网启应急是否打开
    if ((items & Task::HOST_CHECK_ITEM_NET_EMERGENCY_IS_OPENED) == Task::HOST_CHECK_ITEM_NET_EMERGENCY_IS_OPENED)
    {
        // 检查主机网启应急功能是否打开
        if (MyDbDataManager->IsHostNetEmergencyOpen(HCode) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_NET_EMERGENCY_FUNCTION_IS_CLOSED, HCode);
            }
            return false;
        }
    }

    // 检查虚拟机应急是否打开
    if ((items & Task::HOST_CHECK_ITEM_VM_EMERGENCY_IS_OPENED) == Task::HOST_CHECK_ITEM_VM_EMERGENCY_IS_OPENED)
    {
        // 检查主机虚拟机应急功能是否打开
        if (MyDbDataManager->IsHostVmEmergencyOpen(HCode) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_VM_EMERGENCY_FUNCTION_IS_CLOSED, HCode);
            }
            return false;
        }
    }

    // 检查光纤卡应急是否打开
    if ((items & Task::HOST_CHECK_ITEM_FC_EMERGENCY_IS_OPENED) == Task::HOST_CHECK_ITEM_FC_EMERGENCY_IS_OPENED)
    {
        // 检查主机光纤卡应急功能是否打开
        if (MyDbDataManager->IsHostFcEmergencyOpen(HCode) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_FC_EMERGENCY_FUNCTION_IS_CLOSED, HCode);
            }
            return false;
        }
    }

    // 检查主机是否没有开启自动应急
    if ((items & Task::HOST_CHECK_ITEM_EA_IS_NOT_OPENED) == Task::HOST_CHECK_ITEM_EA_IS_NOT_OPENED)
    {
        if (MyDbDataManager->IsHostEAOpen(HCode) == true)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_EA_OPENED, HCode);
            }
            return false;
        }
    }

    // 检查主机是否开启自动应急
    if ((items & Task::HOST_CHECK_ITEM_EA_IS_OPENED) == Task::HOST_CHECK_ITEM_EA_IS_OPENED)
    {
        if (MyDbDataManager->IsHostEAOpen(HCode) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_EA_NOT_OPENED, HCode);
            }
            return false;
        }
    }

    // 检查主机是否有CDP保护
    if ((items & Task::HOST_CHECK_ITEM_HAS_CDP_PROTECTION) == Task::HOST_CHECK_ITEM_HAS_CDP_PROTECTION)
    {
        if (MyDbDataManager->IsHostHasCDPProtection(HCode) == false)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_NO_CDP_PROTECTION, HCode);
            }
            return false;
        }
    }

    // 检查主机是否有CDP保护
    if ((items & Task::HOST_CHECK_ITEM_NO_CDP_PROTECTION) == Task::HOST_CHECK_ITEM_NO_CDP_PROTECTION)
    {
        if (MyDbDataManager->IsHostHasCDPProtection(HCode) == true)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_HAS_CDP_PROTECTION, HCode);
            }
            return false;
        }
    }

    // 检查可信副本是否存在
    if ((items & Task::HOST_CHECK_ITEM_TRUSTED_COPY_EXIST) == Task::HOST_CHECK_ITEM_TRUSTED_COPY_EXIST)
    {
        // 检查主机的系统盘可信副本是否存在
        if (MyDbDataManager->IsCopyExist(HCode, MyDbDataManager->GetSystemTrustedCopyId(HCode)) == false)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_TRUSTED_NOT_EXIST);

            return false;
        }
    }

	if ((items & Task::HOST_CHECK_ITEM_HOST_IN_DOG) == Task::HOST_CHECK_ITEM_HOST_IN_DOG)
    {
    }

    // 检查虚拟机资源
    if ((items & Task::HOST_CHECK_ITEM_VM_RESOURCE) == Task::HOST_CHECK_ITEM_VM_RESOURCE)
    {
        // 检查虚拟服务器是否启用
        if (MyDbDataManager->IsVMHostUsed(HCode) == false)
        {
            // 检查虚拟机网卡
            string BrNicName = MyDbDataManager->GetHostBridgeNicName(HCode);
            if (CheckNicValidation(BrNicName, (Task::NicCheckItem)(Task::NIC_CHECK_ITEM_EXIST)) == false)
            {
                return false;
            }

            // 获取虚拟机内存
            int ActiveVmMemory = 0;

            // 活动虚拟机CPU
            int ActiveVmCPU = 0;

            sql::ResultSet * TempRs = MyDbDataManager->GetHostResultSet();
            if (TempRs != NULL && TempRs->rowsCount() > 0)
            {
                while (TempRs->next())
                {
                    // 虚拟机名称
                    string TempHCode = TempRs->getString("HCODE");

                    // 检查是否为当前主机
                    if (HCode == TempHCode)
                    {
                        continue;
                    }

                    // 检查该虚拟机是否运行
                    if (MyVmManager->IsRunning(GetVmName(TempHCode)) == true)
                    {
                        // 获取总的活动虚拟机内存
                        ActiveVmMemory = ActiveVmMemory + MyDbDataManager->GetHostVmMemory(TempHCode);

                        // 获取总的活动虚拟机CPU
                        ActiveVmCPU = ActiveVmCPU + MyDbDataManager->GetHostVmCPU(TempHCode);
                    }
                }
            }
            // 销毁结果集
            MyDbDataManager->DestroyResultSet(TempRs);

            // 获取主机虚拟机内存
            int HostVmMemory = MyDbDataManager->GetHostVmMemory(HCode);

            // 获取总的虚拟机内存
            int TotalVmMemory = MyDbDataManager->GetBesTotalVmMemory();

            // 检查内存
            if (HostVmMemory > (TotalVmMemory - ActiveVmMemory))
            {
                // 没有内存空间
                if (isseterror == true)
                {
                    MyLastError->SetError(BesError::BES_ERR_VM_NO_MEMORY, HCode);
                }
                return false;
            }

            // 主机的虚拟机CPU
            int HostVmCPU = MyDbDataManager->GetHostVmCPU(HCode);

            // 总的虚拟机CPU
            int TotalVmCPU = MyDbDataManager->GetBesTotalVmCPU();

            // 检查可用的CPU
            if (HostVmCPU > (TotalVmCPU - ActiveVmCPU))
            {
                // 没有可用的CPU
                if (isseterror == true)
                {

                    MyLastError->SetError(BesError::BES_ERR_VM_NO_CPU, HCode);
                }
                return false;
            }
        }
    }*/
    return true;
}

/*
 *  功能：
 *      检查主机的有效性
 *  参数
 *      hcode           :   主机编码
 *      items           :   检查项
 *      isseterror      :   是否设置错误
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckHostStateValidation(string hcode, Task::HostStateCheckItem items, bool isseterror)
{/*
    string HCode = hcode;

    // 发送查询主机状态任务
    string Parameter = BeCommand::CreateCommandParameter(1, HCode.c_str());
    StartSubTask(DEFAULT_COMMAND_QUERY_HOST_STATE, Parameter, HCode, MyHostIp, MyHostPort);

    // 获取主机状态
    DbDataManager::HostProtectedStatus HostStatus = MyDbDataManager->GetHostProtectedStatus(HCode);

    // 检查主机状态是否为本地
    if ((items & Task::HOST_STATE_CHECK_ITEM_IS_LOCAL) == Task::HOST_STATE_CHECK_ITEM_IS_LOCAL)
    {
        if (HostStatus != DbDataManager::HOST_PROTECTED_STATUS_LOCAL)
        {
            if (isseterror == true)
            {
                // 无效的主机状态
                MyLastError->SetError(BesError::BES_ERR_HOST_STATUS_IS_NOT_LOCAL);
            }
            return false;
        }
    }

    // 检查主机状态是否为离线
    if ((items & Task::HOST_STATE_CHECK_ITEM_IS_OFFLINE) == Task::HOST_STATE_CHECK_ITEM_IS_OFFLINE)
    {
        if (HostStatus != DbDataManager::HOST_PROTECTED_STATUS_OFFLINE  && HostStatus != DbDataManager::HOST_PROTECTED_STATUS_VMING)
        {
            if (isseterror == true)
            {
                // 无效的主机状态
                MyLastError->SetError(BesError::BES_ERR_HOST_STATUS_IS_NOT_OFFLINE);
            }
            return false;
        }
    }

    // 检查主机状态是否不为离线
    if ((items & Task::HOST_STATE_CHECK_ITEM_NOT_OFFLINE) == Task::HOST_STATE_CHECK_ITEM_NOT_OFFLINE)
    {
        if (HostStatus == DbDataManager::HOST_PROTECTED_STATUS_OFFLINE)
        {
            if (isseterror == true)
            {
                // 无效的主机状态
                MyLastError->SetError(BesError::BES_ERR_HOST_STATUS_IS_OFFLINE);
            }
            return false;
        }
    }

    // 检查主机状态是否为应急
    if ((items & Task::HOST_STATE_CHECK_ITEM_IS_EMERGENCY) == Task::HOST_STATE_CHECK_ITEM_IS_EMERGENCY)
    {
        if ((HostStatus != DbDataManager::HOST_PROTECTED_STATUS_NET)
                && (HostStatus != DbDataManager::HOST_PROTECTED_STATUS_VM)
                && (HostStatus != DbDataManager::HOST_PROTECTED_STATUS_FCBOOT)
                && (HostStatus != DbDataManager::HOST_PROTECTED_STATUS_DOCKER))
        {
            if (isseterror == true) MyLastError->SetError(BesError::BES_ERR_HOST_STATUS_IS_NOT_EMERGENCY);
            return false;
        }
    }

    // 检查主机状态是否不为应急
    if ((items & Task::HOST_STATE_CHECK_ITEM_NOT_EMERGENCY) == Task::HOST_STATE_CHECK_ITEM_NOT_EMERGENCY)
    {
        if ((HostStatus == DbDataManager::HOST_PROTECTED_STATUS_NET)
                || (HostStatus == DbDataManager::HOST_PROTECTED_STATUS_VM)
                || (HostStatus == DbDataManager::HOST_PROTECTED_STATUS_FCBOOT))
        {
            if (isseterror == true)
            {
                // 无效的主机状态
                MyLastError->SetError(BesError::BES_ERR_HOST_STATUS_IS_EMERGENCY);
            }
            return false;
        }
    }

    // 检查主机状态是否为虚拟机应急
    if ((items & Task::HOST_STATE_CHECK_ITEM_IS_VM_EMERGENCY) == Task::HOST_STATE_CHECK_ITEM_IS_VM_EMERGENCY)
    {
        if ((HostStatus != DbDataManager::HOST_PROTECTED_STATUS_VM))
        {
            if (isseterror == true)
            {
                // 无效的主机状态
                MyLastError->SetError(BesError::BES_ERR_HOST_STATUS_IS_NOT_VM);
            }
            return false;
        }
    }

    // 检查主机状态是否为网启应急
    if ((items & Task::HOST_STATE_CHECK_ITEM_IS_NET_EMERGENCY) == Task::HOST_STATE_CHECK_ITEM_IS_NET_EMERGENCY)
    {
        if ((HostStatus != DbDataManager::HOST_PROTECTED_STATUS_NET))
        {
            if (isseterror == true)
            {
                // 无效的主机状态

                MyLastError->SetError(BesError::BES_ERR_HOST_STATUS_IS_NOT_NET);
            }
            return false;
        }
    }

    // 检查主机状态是否为光纤应急
    if ((items & Task::HOST_STATE_CHECK_ITEM_IS_FC_EMERGENCY) == Task::HOST_STATE_CHECK_ITEM_IS_FC_EMERGENCY)
    {
        if ((HostStatus != DbDataManager::HOST_PROTECTED_STATUS_FCBOOT))
        {
            if (isseterror == true)
            {
                // 无效的主机状态
                MyLastError->SetError(BesError::BES_ERR_HOST_STATUS_IS_NOT_FC);
            }
            return false;
        }
    }*/

    return true;
}

/*
 *  功能：
 *      检查副本的有效性
 *  参数
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *      items           :   检查项
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckCopyValidation(string hcode, string copyid, Task::CopyCheckItem items)
{/*
    string HCode = hcode;
    string CopyId = copyid;

    if ((items & Task::COPY_CHECK_ITEM_EXIST) == Task::COPY_CHECK_ITEM_EXIST) // 副本是否存在
    {
        // 检查副本是否存在
        if (MyDbDataManager->IsCopyExist(HCode, CopyId) == false)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_NOT_EXIST, HCode + SEPARATOR_CHAR_COLON + CopyId);
            return false;
        }
    }

    // 副本是否不存在
    if ((items & Task::COPY_CHECK_ITEM_NOT_EXIST) == Task::COPY_CHECK_ITEM_NOT_EXIST)
    {
        // 检查副本是否不存在
        if (MyDbDataManager->IsCopyExist(HCode, CopyId) == true)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_EXIST, HCode + SEPARATOR_CHAR_COLON + CopyId);
            return false;
        }
    }

    // 副本是否未使用
    if ((items & Task::COPY_CHECK_ITEM_NOT_USED) == Task::COPY_CHECK_ITEM_NOT_USED)
    {
        // 检查副本是否在使用中
        if ((MyDbDataManager->IsStrategyUsedForCDP(HCode, MyDbDataManager->GetCopyLMountPoint(HCode, CopyId)) == false)
                && (MyDbDataManager->IsHostEmergency(HCode) == false)
                && (CheckCopyRealNotUsed(HCode, CopyId) == false))
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_IS_USED, HCode + SEPARATOR_CHAR_COLON + CopyId);
            return false;
        }
    }

    // 副本不是快照型
    if ((items & Task::COPY_CHECK_ITEM_NOT_SNAPSHOT_MODE) == Task::COPY_CHECK_ITEM_NOT_SNAPSHOT_MODE)
    {
        // 快照型副本不允许进行快照操作
        if (MyDbDataManager->GetCopyType(HCode, CopyId) == DbDataManager::COPY_TYPE_SNAPSHOT)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_TYPE_IS_SNAPSHOT_COPY, HCode + SEPARATOR_CHAR_COLON + CopyId);
            return false;
        }
    }

    // 副本不是应急副本
    if ((items & Task::COPY_CHECK_ITEM_NOT_EMERGENCY_COPY) == Task::COPY_CHECK_ITEM_NOT_EMERGENCY_COPY)
    {
        if (MyDbDataManager->IsCopyAsEmergencyCopy(HCode, CopyId) == true)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_BOOT_NOT_ALLOWED_DELETE, HCode + SEPARATOR_CHAR_COLON + CopyId);
            return false;
        }
    }

    // 副本不是可信副本
    if ((items & Task::COPY_CHECK_ITEM_NOT_TRUSTED_COPY) == Task::COPY_CHECK_ITEM_NOT_TRUSTED_COPY)
    {
        // 检查是否为可信副本
        if (MyDbDataManager->GetTrustedCopyId(HCode, MyDbDataManager->GetCopyLMountPoint(HCode, CopyId)) == CopyId)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_TRUSTED_NOT_ALLOWED_DELETE, HCode + SEPARATOR_CHAR_COLON + CopyId);
            return false;
        }
    }

    // 副本不是最后一个普通副本
    if ((items & Task::COPY_CHECK_ITEM_NOT_LAST_NORMAL_COPY) == Task::COPY_CHECK_ITEM_NOT_LAST_NORMAL_COPY)
    {
        // 检查是否为最后一个普通副本
        if ((MyDbDataManager->GetCopyType(HCode, CopyId) == DbDataManager::COPY_TYPE_NORMAL) && (MyDbDataManager->GetCopyCountWithType(HCode, DbDataManager::COPY_TYPE_NORMAL) == 1))
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_FINAL_NORMAL_NOT_ALLOWED_DELETE, HCode + SEPARATOR_CHAR_COLON + CopyId);
            return false;
        }
    }

    // 副本对应的快照是否存在
    if ((items & Task::COPY_CHECK_ITEM_HAS_SNAPSHOT) == Task::COPY_CHECK_ITEM_HAS_SNAPSHOT)
    {
        // 副本对应的快照是否存在
        if (MyDbDataManager->IsSnapshotExist(HCode, MyDbDataManager->GetCopySnapshotId(HCode, CopyId)) == false)
        {
            MyLastError->SetError(BesError::BES_ERR_SNAPSHOT_NOT_EXIST, HCode);
            return false;
        }
    }

    // 副本是应急副本
    if ((items & Task::COPY_CHECK_ITEM_IS_EMERGENCY_COPY) == Task::COPY_CHECK_ITEM_IS_EMERGENCY_COPY)
    {
        // 检查是否为网启副本
        if (MyDbDataManager->IsCopyAsEmergencyCopy(HCode, CopyId) == false)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_NOT_BOOT_COPY, HCode + SEPARATOR_CHAR_COLON + CopyId);

            return false;
        }
    }
*/
    return true;
}

/*
 *  功能：
 *      检查卷的有效性
 *  参数
 *      hcode           :   主机编码
 *      volumesymbol    :   卷名或策略id
 *      items           :   检查项
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckVolumeValidation(string hcode, string volumesymbol, Task::VolumeCheckItem items)
{
    return true;
}

/*
 *  功能：
 *      检查参数有效性
 *  参数
 *      validparanum    :   有效的参数个数
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckParameterValidation(int validparanum)
{
    // 如果为可变参数个数
    if (validparanum == DEFAULT_VARIABLE_COMMAND_PARAMETER_COUNT)
    {
        return true;
    }

    // 切割参数
    vector<string>ParaArray = BeGeneralLib::StringSplit(MyParameter, COMMAND_PARAMETER_SEPCHAR, BeGeneralLib::WITH_EMPTY_ITEM);

    // 检查参数个数是否正确
    if (ParaArray.size() != validparanum) // 参数：主机编码;盘符;
    {
        MyLastError->SetError(BesError::BES_ERR_COMMAND_INVALID_PARAMETER_COUNT, MyParameter);

        return false;
    }
    return true;
}

/*
 *  功能：
 *      检查快照有效性
 *  参数
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *      items           :   检查项
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckSnapshotValidation(string hcode, string snapshotid, Task::SnapshotCheckItem items)
{
    string HCode = hcode;
    string SnapshotId = snapshotid;

    // 检查快照是否存在
    if ((items & Task::SNAPSHOT_CHECK_ITEM_EXIST) == Task::SNAPSHOT_CHECK_ITEM_EXIST)
    {
        if (MyDbDataManager->IsSnapshotExist(HCode, SnapshotId) == false)
        {
            MyLastError->SetError(BesError::BES_ERR_SNAPSHOT_NOT_EXIST, SnapshotId);
            return false;
        }
    }

    // 检查快照对应的副本是否存在
    if ((items & Task::SNAPSHOT_CHECK_ITEM_HAS_COPY) == Task::SNAPSHOT_CHECK_ITEM_HAS_COPY)
    {
        if (MyDbDataManager->IsSnapshotHasCopy(HCode, SnapshotId) == false)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_NOT_EXIST, HCode);
            return false;
        }
    }

    // 检查快照对应的副本是否不存在
    if ((items & Task::SNAPSHOT_CHECK_ITEM_NO_COPY) == Task::SNAPSHOT_CHECK_ITEM_NO_COPY)
    {
        if (MyDbDataManager->IsSnapshotHasCopy(HCode, SnapshotId) == true)
        {
            MyLastError->SetError(BesError::BES_ERR_COPY_EXIST, HCode);
            return false;
        }
    }

    // 检查是否为快照零
    if ((items & Task::SNAPSHOT_CHECK_ITEM_NOT_FIRST) == Task::SNAPSHOT_CHECK_ITEM_NOT_FIRST)
    {
        if (MyDbDataManager->IsZeroSnapshot(HCode, SnapshotId) == true)
        {
            MyLastError->SetError(BesError::BES_ERR_SNAPSHOT_ZERO_NOT_ALLOWED_DELETE, HCode + SEPARATOR_CHAR_COLON + SnapshotId);
            return false;
        }
    }

    // 检查是否为最新快照
    if ((items & Task::SNAPSHOT_CHECK_ITEM_NOT_FINAL) == Task::SNAPSHOT_CHECK_ITEM_NOT_FINAL)
    {
        if (MyDbDataManager->IsFinalSnapshot(HCode, SnapshotId) == true)
        {
            MyLastError->SetError(BesError::BES_ERR_SNAPSHOT_NEW_NOT_ALLOWED_DELETE, HCode + SEPARATOR_CHAR_COLON + SnapshotId);

            return false;
        }
    }

    // 检查是否为可信快照
    if ((items & Task::SNAPSHOT_CHECK_ITEM_NOT_TRUSTED) == Task::SNAPSHOT_CHECK_ITEM_NOT_TRUSTED)
    {
        if (MyDbDataManager->IsTrustedSnapshot(HCode, SnapshotId) == true)
        {
            MyLastError->SetError(BesError::BES_ERR_SNAPSHOT_IS_TRUSTED, HCode + SEPARATOR_CHAR_COLON + SnapshotId);

            return false;
        }
    }

    return true;
}

/*
 *  功能：
 *      检查网卡有效性
 *  参数
 *      nicname         :   网卡名称
 *      items           :   检查项
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckNicValidation(string nicname, Task::NicCheckItem items)
{
    string NicName = nicname;

    // 检查网卡是否存在
    if ((items & Task::NIC_CHECK_ITEM_EXIST) == Task::NIC_CHECK_ITEM_EXIST)
    {
        if (MyDbDataManager->IsNicExist(NicName) == false)
        {
            MyLastError->SetError(BesError::BES_ERR_NIC_NOT_EXIST, NicName);

            return false;
        }
    }
    return true;
}

/*
 *  功能：
 *      检查启动协议有效性
 *  参数
 *      protocol        :   启动协议
 *      items           :   检查项
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckBootProtocolValidation(string hcode, string protocol, Task::BootProtocolCheckItem items, bool isseterror)
{
    string HCode = hcode;
    string BootProtocol = protocol;

    // 检查启动协议是否有效
    if ((items & Task::BOOT_PROTOCOL_CHECK_ITEM_VALID) == Task::BOOT_PROTOCOL_CHECK_ITEM_VALID)
    {
        if (BootProtocol != DEFAULT_BOOT_PROTOCOL_PXE
                && BootProtocol != DEFAULT_BOOT_PROTOCOL_SNSBOOT
                && BootProtocol != DEFAULT_BOOT_PROTOCOL_HBA
                && BootProtocol != DEFAULT_BOOT_PROTOCOL_FCBOOT
                && BootProtocol != DEFAULT_BOOT_PROTOCOL_VMBOOT)
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_HOST_INVALID_NETBOOT_PROTOCOL, HCode + SEPARATOR_CHAR_COLON + BootProtocol);
            }
            return false;
        }
    }

    // 是否为网络启动
    if ((items & Task::BOOT_PROTOCOL_CHECK_ITEM_IS_NETBOOT) == Task::BOOT_PROTOCOL_CHECK_ITEM_IS_NETBOOT)
    {
        // 检查主机协议是否为网启协议
        if ((BootProtocol != DEFAULT_BOOT_PROTOCOL_PXE) && (BootProtocol != DEFAULT_BOOT_PROTOCOL_SNSBOOT) && (BootProtocol != DEFAULT_BOOT_PROTOCOL_HBA))
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_BOOT_PROTOCOL_IS_NOT_NETBOOT, HCode + SEPARATOR_CHAR_COLON + BootProtocol);
            }
            return false;
        }
    }

    // 是否为虚拟机启动
    if ((items & Task::BOOT_PROTOCOL_CHECK_ITEM_IS_VMBOOT) == Task::BOOT_PROTOCOL_CHECK_ITEM_IS_VMBOOT)
    {
        // 检查主机协议是否为网启协议
        if ((BootProtocol != DEFAULT_BOOT_PROTOCOL_VMBOOT))
        {
            if (isseterror == true)
            {
                MyLastError->SetError(BesError::BES_ERR_BOOT_PROTOCOL_IS_NOT_VMBOOT, HCode + SEPARATOR_CHAR_COLON + BootProtocol);
            }
            return false;
        }
    }

    // 是否为光纤启动
    if ((items & Task::BOOT_PROTOCOL_CHECK_ITEM_IS_FCBOOT) == Task::BOOT_PROTOCOL_CHECK_ITEM_IS_FCBOOT)
    {
        if (BootProtocol != DEFAULT_BOOT_PROTOCOL_FCBOOT)
        {
            if (isseterror == true)
            {

                MyLastError->SetError(BesError::BES_ERR_BOOT_PROTOCOL_IS_NOT_FCBOOT, HCode + SEPARATOR_CHAR_COLON + BootProtocol);
            }
            return false;
        }
    }
    return true;
}

/*
 *  功能：
 *      检查主机授权的有效性
 *  参数
 *      hcode           :   主机编码
 *      items           :   检查项
 *      isseterror      :   是否设置错误
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckHostLicenseValidation(string hcode, Task::LicenseCheckItem items, bool isseterror)
{
    return CheckHostLicenseValidation(hcode, "", items, isseterror);
}

/*
 *  功能：
 *      检查主机授权的有效性
 *  参数
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符或策略编号
 *      items           :   检查项
 *      isseterror      :   是否设置错误
 *  返回：
 *      如果有效返回true,否则返回false
 */
bool Task::CheckHostLicenseValidation(string hcode, string volumesymbol, Task::LicenseCheckItem items, bool isseterror)
{
	return true;
}

/*
 *  功能：
 *      删除主机策略
 *  参数
 *      hcode           :   主机编码
 *      strategyid      :   盘符或策略编号
 *      isforce         :   是否强制删除
 *  返回：
 *      如果成功返回true,否则返回false
 */
bool Task::DeleteStrategyVolume(string hcode, string strategyid, bool isforce)
{
    return true;
}

/*
 *  功能：
 *      查询主机信息
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      无
 */
void Task::QueryHostInformation(string hcode)
{

    string HCode = hcode;

    // 获取Agent所需参数
    map<string, string> FieldValueList = MyDbDataManager->GetAgentParameter(MyDbDataManager->GetHostBusinessNicName(HCode));
    string UserName = FieldValueList["DBUSERNAME"];
    string Password = FieldValueList["DBPASSWORD"];
    string ServerIp = FieldValueList["IPADDR"];

    // 获取主机ip
    string HostIp = MyDbDataManager->GetHostIp(HCode);

    // 生成查询参数
    string Parameter = BeCommand::CreateCommandParameter(6, HCode.c_str(), HostIp.c_str(), UserName.c_str(), Password.c_str(), ServerIp.c_str(), MyHostPort.c_str());

    // 更新状态信息
    UpdateTaskStateDesc(TaskProcDesc::TASK_PROC_QUERY_HOST_INFO);
}

void Task::QueryHostStatus(string hcode)
{
    string HCode = hcode;

    // 获取主机ip
    string HostIp = MyDbDataManager->GetHostIp(HCode);

    // 生成查询参数
    string Parameter = BeCommand::CreateCommandParameter(1, HCode.c_str());

    // 更新状态信息
    UpdateTaskStateDesc(TaskProcDesc::TASK_PROC_QUERY_HOST_STATUS);
}

/*
 *  功能：
 *      查询license信息
 */
void Task::QueryLicense()
{
}

/*
 *  功能：
 *      查询主机卷类型
 *  参数
 *      hcode               :   主机编码
 *      strategyid          :   策略编号
 *      retry               :   重试次数
 *  返回：
 *      成功返回true，否则返回false
 */
bool Task::QueryHostVolumeType(string hcode, string strategyid, int retry)
{
	return true;
}

/*
 *  功能：
 *      获取主机默认文件系统
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      默认文件系统
 */
BaseFileSystem Task::GetHostDefaultFileSystem(string hcode)
{
    BaseFileSystem FileSystem;
    switch (MyDbDataManager->GetHostOSType(hcode))
    {
        case DbDataManager::HOST_OS_TYPE_WINDOWS:
        {
            FileSystem = BASE_FILESYSTEM_NTFS;
            break;
        }
        case DbDataManager::HOST_OS_TYPE_LINUX:
        {
            FileSystem = BASE_FILESYSTEM_EXT3;

            break;
        }
    }
    return FileSystem;
}

/*
 *  功能：
 *      设置网卡为桥接状态
 *  参数
 *      nicname          :   网卡名
 *  返回：
 *      设置成功返回true，否则返回false
 */
bool Task::SetNicBridged(string nicname)
{
    // 检查网卡名称有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(nicname) == true
            || BeGeneralLib::IsPhysicalNicExist(nicname) == false)
    {
        return false;
    }

    // 检查网卡是否为桥接模式
    if (MyDbDataManager->IsNicBridged(nicname) == true)
    {
        string BrName = DEFAULT_BRIDGE_NAME_PREFIX + nicname;
        if (BeGeneralLib::IsBridgeNicExist(BrName) == true)
        {
            return true;
        }
    }

    // 更新网卡模式为桥接
    MyDbDataManager->UpdateNicType(nicname, DbDataManager::NIC_TYPE_BRIDGED);

    string Parameter = BeCommand::CreateCommandParameter(1, nicname.c_str());

    return (StartSubTask(MyCmdHeader, DEFAULT_COMMAND_SET_BUSINESS_NIC_IP, Parameter, MyHostCode, MyHostIp, MyHostPort) == Task::TASK_EXEC_RESULT_SUCCESS);
}

/*
 *  功能：
 *      将CDP时间转换为分钟表示
 *  参数
 *      keeptime            :   连续快照保留时间
 *      keepunit            :   时间单位
 *  返回：
 *      转换后的分钟数
 */
int Task::ConvertCDPTime(int keeptime, int keepunit)
{
    int TotalMinutes = 0;
    switch (keepunit)
    {
        case 0: // 分钟
        {
            TotalMinutes = keeptime;
            break;
        }
        case 1: // 小时
        {
            TotalMinutes = keeptime * 60;
            break;
        }
        case 2: // 天
        {
            TotalMinutes = keeptime * 60 * 60;

            break;
        }
    }
    return TotalMinutes;
}

/*
 *  功能：
 *      自检
 *  返回：
 *      成功返回true，否则返回false
 */
bool Task::SelfCheck()
{
    BeGeneralLib::CreateEmptyFile(BeGeneralLib::GetSelfCheckLogFileName(), true);
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_BEGIN, DEFAULT_EMPTY_STRING, BesError::SELF_CHECK_RESULT_NONE);

    // 检查授权
    CheckLicense();

    // 检查日志
    BeGeneralLib::DeleteSystemLog();
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_LOG, DEFAULT_EMPTY_STRING, BesError::SELF_CHECK_RESULT_SUCCESS);

    // 检查存储
//  CheckStorage();

    // 检查数据库连接
    CheckDatabase();

    // 检查目录权限
    CheckPermission();

    // 检查安装包
    CheckPackage();

    // 检查端口
    CheckPort();

    // 检查服务
    CheckService();

    // 检查进程运行
    CheckProcess();

    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_END, DEFAULT_EMPTY_STRING, BesError::SELF_CHECK_RESULT_NONE);
}

/*
 *  功能：
 *      检查授权
 */
void Task::CheckLicense()
{
}

/*
 *  功能：
 *      检查端口
 *  参数：
 *      无
 *  返回：
 *      无
 */
void Task::CheckPort()
{
    // 检查8585端口
    CheckSinglePort(BeGeneralLib::SOCKET_PROTOCOL_TCP, BES_SERVER_PORT);

    // 检查8586端口
    CheckSinglePort(BeGeneralLib::SOCKET_PROTOCOL_TCP, BES_INTERNAL_PORT);

    // 检查8686端口
    CheckSinglePort(BeGeneralLib::SOCKET_PROTOCOL_TCP, DEFAULT_WEB_VNC_PORT);
}

/*
 *  功能：
 *      检查单一端口
 *  参数：
 *      protocol        :   协议
 *      port            :   端口
 *  返回：
 *      无
 */
void Task::CheckSinglePort(BeGeneralLib::SocketProtocol protocol, int port)
{
    // 检查端口

    BesError::SelfCheckResult ScResult = BeGeneralLib::GetListenPortProcessId(protocol, port).size() > 0 ? BesError::SELF_CHECK_RESULT_SUCCESS : BesError::SELF_CHECK_RESULT_FAILED;
    //    string s =;
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_PORT, BeGeneralLib::GetSocketProtocolName(protocol) + SEPARATOR_CHAR_MINUS_SIGN + BeGeneralLib::Number2String(port), ScResult);
}

/*
 *  功能：
 *      检查服务
 *  参数：
 *      无
 *  返回：
 *      无
 */
void Task::CheckService()
{
    // 检查httpd服务
    CheckSingleService(DEFAULT_SERVICE_NAME_HTTP);

    // 检查mysql服务
    CheckSingleService(DEFAULT_SERVICE_NAME_MYSQL);

    // 检查sshd服务
    CheckSingleService(DEFAULT_SERVICE_NAME_SSH);

    // 检查vncserver服务
    CheckSingleService(DEFAULT_SERVICE_NAME_VNC, DEFAULT_SERVICE_NAME_VNC_ALIAS);
}

/*
 *  功能：
 *      检查单一服务
 *  参数：
 *      name            :   服务名
 *  返回：
 *      无
 */
void Task::CheckSingleService(string name)
{
    return CheckSingleService(name, DEFAULT_EMPTY_STRING);
}

/*
 *  功能：
 *      检查单一服务
 *  参数：
 *      name            :   服务名
 *      alias           :   服务别名
 *  返回：
 *      无
 */
void Task::CheckSingleService(string name, string alias)
{
    BesError::SelfCheckResult ScResult = BesError::SELF_CHECK_RESULT_SUCCESS;

    // 检查服务是否存在
    if (BeGeneralLib::IsServiceExist(name) == false){
        ScResult = BesError::SELF_CHECK_RESULT_NOT_EXIST;
    }
    else{
        // 检查服务是否运行
        if (BeGeneralLib::IsServiceRunning(name) == false && (BeGeneralLib::IsServiceRunning(alias) == false))
        {
            ScResult = BeGeneralLib::CheckService(name, true) ? BesError::SELF_CHECK_RESULT_FIXED : BesError::SELF_CHECK_RESULT_FAILED;
        }
    }
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_SERVICE, name, ScResult);
}

//检查程序包
void Task::CheckPackage()
{
    // 检查mysqld
    CheckSinglePackage("mysql-server", "mysql");

    // 检查httpd
    CheckSinglePackage("httpd", "http");
}

/*
 *  功能：
 *      检查单一程序包
 *  参数：
 *      package         :   程序包
 *      alias           :   别名
 */
void Task::CheckSinglePackage(string package, string alias)
{
    BesError::SelfCheckResult ScResult = BeGeneralLib::IsPackageInstalled(package) ? BesError::SELF_CHECK_RESULT_SUCCESS : BesError::SELF_CHECK_RESULT_NOT_EXIST;
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_PACKAGE, alias, ScResult);
}

//检查ping
void Task::CheckPing()
{
}

//检查数据库
void Task::CheckDatabase()
{
    // 检查lsscsi是否安装
    BesError::SelfCheckResult ScResult = (MyDbDataManager->CheckDatabaseOK() == true) ? BesError::SELF_CHECK_RESULT_SUCCESS : BesError::SELF_CHECK_RESULT_FAILED;
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_DATABASE, BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_DBNAME), ScResult);
    if (ScResult == BesError::SELF_CHECK_RESULT_SUCCESS)
    {
        MyDbDataManager->Repair();
    }
}

//检查目录权限
void Task::CheckPermission()
{
    string Path = DEFAULT_EMPTY_STRING;

    // 检查phpMyAdmin
    Path = DEFAULT_PHPMYADMIN_PATH;
    BeGeneralLib::CheckAndChangeFilePermission(Path, DEFAULT_DIR_PERMISSION, true);
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_PERMISSION, Path, BesError::SELF_CHECK_RESULT_SUCCESS);

    // 检查upload
    Path = DEFAULT_UPGRADE_ZIP_PATH;
    BeGeneralLib::CheckAndChangeFilePermission(Path, DEFAULT_FULL_PERMISSION, true);
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_PERMISSION, Path, BesError::SELF_CHECK_RESULT_SUCCESS);
}

//检查进程
void Task::CheckProcess()
{
//  if (MyDbDataManager->HasEAOpenedHosts() == true) CheckSingleProcess(DEFAULT_SW_SERVER_DRHDAEON);	// 检查心跳

    // 检查异地容灾任务执行器
    if (MyDbDataManager->HasRdrOpenedHosts() == true)
    {
        CheckSingleProcess(DEFAULT_SW_SERVER_RDRTASKEXEC);
    }

    // 检查守护进程
    BesError::SelfCheckResult ScResult = (BeGeneralLib::IsProcessRunning(DEFAULT_SW_SERVER_BEDAEMON) == true) ? BesError::SELF_CHECK_RESULT_SUCCESS : BesError::SELF_CHECK_RESULT_NOT_RUNNING;
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_PROCESS, DEFAULT_SW_SERVER_BEDAEMON, ScResult);
}

/*
 *  功能：
 *      检查单一进程
 *  参数：
 *      name            ：  进程名
 *  返回：
 *      无
 */
void Task::CheckSingleProcess(string name)
{

    BesError::SelfCheckResult ScResult = (BeGeneralLib::CheckProcess(name) == true) ? BesError::SELF_CHECK_RESULT_SUCCESS : BesError::SELF_CHECK_RESULT_NOT_RUNNING;
    WriteSelfCheckLog(BesError::BES_ERR_SELF_CHECK_ITEM_PROCESS, name, ScResult);
}

/*
 *  功能：
 *      检查进程
 *  参数：
 *      无
 *  返回：
 *      无
 */
void Task::WriteSelfCheckLog(BesError::ErrorType type, string parameter, BesError::SelfCheckResult result)
{
    string CheckMessage = string("[") + BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_LONG) + string("] ") + BesError::GetErrorDescription(type, parameter, result);
    BeGeneralLib::WriteFile(BeGeneralLib::GetSelfCheckLogFileName(), CheckMessage, BeGeneralLib::WRITE_FILE_MODE_APPEND);

    MyDbDataManager->AddSelfCheckLog(BesError::GetErrorDescription(BeGeneralLib::GetDescriptionLang(), type, parameter), result);
}

/*
 *  功能：
 *      发送内部刷新主机状态命令
 *  参数：
 *      hcode           :   主机编码
 */
void Task::SendInternalRefreshHostStateCommand(string hcode)
{
    if (MyDbDataManager->IsServerExist(DbDataManager::REMOTE_SERVER_TYPE_MC) == false)
    {
        return;
    }

    // 发送内部查询主机保护状态命令
    string DataString = string(INTERNAL_REFRESH_HOST_STATE_COMMAND) + COMMAND_PARAMETER_SEPCHAR + hcode;
    BeGeneralLib::SendUdpData(DEFAULT_LOCALHOST_IP, BES_INTERNAL_PORT, DataString);
}

/*
 *  功能：
 *      当前命令字是否为调试打印的命令字
 *  参数：
 *      无
 *  返回：
 *      如果时返回ture，否则返回false
 */
bool Task::IsDebugCommand()
{
	return false;
}

/*
 *  功能：
 *      写入卷的本地挂载点（盘符）
 *  参数：
 *      device          :   设备
 *      hcode           :   主机编码
 *      copyid          :   副本编号
 *  返回：
 *      如果成功返回true，否则返回false
 */
bool Task::WriteVolumeLMountPoint(string device, string hcode, string copyid)
{
    // 检查输入有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(device) == true
            || BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true
            || BeGeneralLib::IsStringEmptyOrInvalidValue(copyid) == true)
    {
        return false;
    }

    // 获取卷挂点
    string LMountPoint = MyDbDataManager->GetCopyLMountPoint(hcode, copyid);

    // 副本挂载点
    string MountPoint = MyDbDataManager->GetCopyMountPoint(hcode, copyid);

    // 获取卷对应的文件系统
    BaseFileSystem FileSystem = BeGeneralLib::GetBaseFileSystem(MyDbDataManager->GetVolumeFileSystem(hcode, LMountPoint));

    // 创建挂载点
    BeGeneralLib::CheckAndCreateDir(MountPoint);

    // 挂载设备
    bool MountFlag = BeGeneralLib::MountLoopDevice(device, MountPoint, FileSystem, 1);
    if (MountFlag == false)
    {
        MountFlag = BeGeneralLib::MountLoopDevice(device, MountPoint, FileSystem, 0);
    }

    // 如果挂载成功
    if (MountFlag == true)
    {
        // 写入配置
        string FileName = BeGeneralLib::AddPathSeparator(MountPoint) + DEFAULT_VM_VOLUME_CONFIG_FILE;

        // 主机编码
        IniFile::WriteProfileString(FileName, DEFAULT_VM_VOLUME_SECION, DEFAULT_VM_VOLUME_KEY_HCODE, hcode);

        // 副本编号
        IniFile::WriteProfileString(FileName, DEFAULT_VM_VOLUME_SECION, DEFAULT_VM_VOLUME_KEY_COPYID, copyid);

        // 卷挂载点
        IniFile::WriteProfileString(FileName, DEFAULT_VM_VOLUME_SECION, DEFAULT_VM_VOLUME_KEY_LMOUNTPOINT, LMountPoint);
    }

    // 卸载设备
    BeGeneralLib::UmountDevice("", MountPoint, true);

    // 则删除循环设备分区
    BeGeneralLib::DeleteLoopDevicePartition(device);

    // 删除挂载点目录
    BeGeneralLib::ForceRemoveDirectory(MountPoint, true);

    return MountFlag;

}

/*
 *  功能：
 *      显示任务阶段信息
 *  参数
 *      无
 *  返回：
 *      无
 */
void Task::ShowTaskStageInformation()
{
    if (IsDebugCommand() == false)
    {
        return;
    }
    string StageString = GetTaskStageString();
    if (BeGeneralLib::IsStringEmpty(StageString) == true)
    {
        return;
    }

    BesLog::DebugPrint(string("[") + StageString + string("]****") + MyCmdCode + string(" CmdHeader=") + MyCmdHeader + string(" HCode=") + MyHostCode + string(" Parameter=") + MyParameter);
}

/*
 *  功能：
 *      获取任务阶段字符串
 *  参数
 *      无
 *  返回：
 *      无
 */
string Task::GetTaskStageString()
{
    string StageString = DEFAULT_EMPTY_STRING;
    switch (MyTaskExecStep)
    {
        case Task::TASK_EXEC_STEP_VALIDATION_CHECK:
        {
            StageString = string("CheckTaskValidation");
            break;
        }
        case Task::TASK_EXEC_STEP_PRE_PROCESSING:
        {
            StageString = string("PreProcessing");
            break;
        }
        case Task::TASK_EXEC_STEP_EXCUTE:
        {
            StageString = string("Execute");
            break;
        }
        case Task::TASK_EXEC_STEP_POST_PROCESSING:
        {
            StageString = string("PostProcessing");
            break;
        }
        case Task::TASK_EXEC_STEP_ERROR_PROCESSING:
        {
            StageString = string("ErrorProcessing");
            break;
        }
        case Task::TASK_EXEC_STEP_GOTO_FINISH:
        {
            StageString = string("GotoFinish");
            break;
        }
        case Task::TASK_EXEC_STEP_WAIT_PREPARE_RESULT:
        case Task::TASK_EXEC_STEP_WAIT_EXCUTING_RESULT:
        case Task::TASK_EXEC_STEP_FINISH:
        {
            break;
        }
    }
    return StageString;
}

/*
 *  功能：
 *      获取可用的网卡名
 *  参数
 *      无
 *  返回：
 *      网卡名
 */
string Task::GetAvailableNicName()
{
    string NicName = DEFAULT_EMPTY_STRING;

    // 获取网卡结果集
    sql::ResultSet * TempRs = MyDbDataManager->GetNetworkResultSet();
    if (TempRs != NULL || TempRs->rowsCount() > 0)
    {
        while (TempRs->next())
        {
            string Interface = TempRs->getString("NICNAME");

            // 检查物理网卡是否存在
            if (BeGeneralLib::IsPhysicalNicExist(Interface) == true)
            {
                NicName = Interface;
                break;
            }
        }
    }

    MyDbDataManager->DestroyResultSet(TempRs);

    return NicName;
}

//删除主机策略集
bool Task::DeleteHostStrategySet(string hcode)
{
    string HCode = hcode;
    sql::ResultSet * TempRs = MyDbDataManager->GetStrategyResultSet(HCode);
    if (TempRs != NULL && TempRs->rowsCount() > 0){
        while (TempRs->next()){
            string StrategyId = TempRs->getString("STRATEGYID");
            DeleteStrategyVolume(HCode, StrategyId, true);		// 删除策略
        }
    }
    MyDbDataManager->DestroyResultSet(TempRs);
    return true;
}

bool Task::CreateFormatDiskScript(string size) {
	if (size.empty() == true || atoi(size.c_str()) == 0) return false;

	long int Size = (((long int) atoi(size.c_str())) * 1000);
	char Vsize[DEFAULT_BUFFER_SIZE];
	bzero(Vsize, sizeof(Vsize));
	sprintf(Vsize, "+%ldM\n\0", Size);
	size = (string) Vsize;

	FILE *fd = fopen("/home/BES/fdisk.sh", "w+");
	if (fd == NULL) return false;
	fputs("#!/bin/bash\n", fd);
	fputs("CMD=\"fdisk\"\n", fd);
	fputs("$CMD $1 << EOF\n", fd);
	fputs("x\n", fd);
	fputs("h\n", fd);
	fputs("255\n", fd);
	fputs("s\n", fd);
	fputs("63\n", fd);
	fputs("c\n", fd);
	fputs("1048576\n", fd);
	fputs("r\n", fd);
	fputs("n\n", fd);
	fputs("p\n", fd);
	fputs("1\n", fd);
	fputs("\n", fd);
	fputs(size.c_str(), fd);
	fputs("a\n", fd);
	fputs("1\n", fd);
	fputs("wq\n", fd);
	fputs("EOF\n", fd);
	fclose(fd);
	string ShellCommand = string("chmod 755 /home/BES/fdisk.sh");
	BeGeneralLib::ExecuteSystem(ShellCommand, false);
	return true;
}

bool Task::CreateDiskPartion(string diskname, string size) {
	if (BeGeneralLib::IsDeviceExist(diskname) == false) return false;
	if (CreateFormatDiskScript(size) == false) {
		BesLog::DebugPrint("Create format scripts failed!", BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return false;
	}
	if (BeGeneralLib::IsFileExist("/home/BES/fdisk.sh") == false) return false;
	string ShellCommand = string("/home/BES/fdisk.sh ") + diskname;
	BeGeneralLib::ExecuteSystem(ShellCommand);
	sleep(5);
	return true;
}

void Task::CreateNotCopyDir(string mountpoint) {
}

string Task::SetHostBootProtocol(string bootprotocol, string mountpoint) {
	string Status = "";
	return Status;
}