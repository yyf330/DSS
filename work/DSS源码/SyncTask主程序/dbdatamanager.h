#ifndef _DBDATAMANAGER_H
#define	_DBDATAMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "begenerallib.h"
#include "beslog.h"
#include "beserror.h"
#include "becommand.h"

using namespace std;

class DbDataManager
{
public:

    /*
     *  功能：
     *      构造函数
     *  参数：
     *      server          :   服务器
     *      user            :   用户名
     *      password        :   密码
     *      dbname          :   数据库
     *  返回：
     *      无
     */
    DbDataManager(string server, string user, string password, string dbname);

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~DbDataManager();
public:

    // 快照结果集类型

    enum SnapshotResType
    {
        SNAPSHOT_RES_TYPE_VOLUMESYMBOL,
        SNAPSHOT_RES_TYPE_SNAPSHOTID,
    };

    // 盘符获取方式

    enum VolumeSymbolRetrieveType
    {
        VOLUME_SYMBOL_RETRIEVE_TYPE_SNAPSHOTID,
        VOLUME_SYMBOL_RETRIEVE_TYPE_IQN,
    };

    // 副本id获取方式

    enum CopyIDRetrieveType
    {
        COPY_ID_RETRIEVE_TYPE_SNAPSHOTID,
        COPY_ID_RETRIEVE_TYPE_IQN,
    };

    // 操作系统类型

    enum HostOSType
    {
        HOST_OS_TYPE_WINDOWS,
        HOST_OS_TYPE_LINUX,
        HOST_OS_TYPE_AIX,
        HOST_OS_TYPE_HPUX,
    };

    // 快照类型

    enum SnapType
    {
        SNAPSHOT_TYPE_ZERO,
        SNAPSHOT_TYPE_VOLUME,
        SNAPSHOT_TYPE_DATABASE,
    };

    // 快照源

    enum SnapshotSource
    {
        SNAPSHOT_SOURCE_NORMAL = 0,
        SNAPSHOT_SOURCE_CDP = 1,
    };

    // 快照同步状态

    enum SnapshotSyncState
    {
        SNAPSHOT_SYNC_STATE_NONE = 0, // 未同步
        SNAPSHOT_SYNC_STATE_COMPLETED = 1, // 已同步
        SNAPSHOT_SYNC_STATE_SYNCING = 2, // 同步中
        SNAPSHOT_SYNC_STATE_CHANGING_NAME = 3, // 修改快照名
    };


    // 明细类型

    enum DetailType
    {
        DETAIL_TYPE_INCLUDE = 1,
        DETAIL_TYPE_EXCLUDE = 2,
        DETAIL_TYPE_DATABASE = 4,
    };

    // 数据库类型

    enum DatabaseType
    {
        DATABASE_TYPE_MYSQL,
    };

    // Bes类型

    enum BesType
    {
        BES_TYPE_SERVER, // 服务器
        BES_TYPE_STARTER, // 启动器
    };

    enum StrategyIdSearchType
    {
        STRATEGY_ID_SEARCH_TYPE_LMOUNTPOINT,
        STRATEGY_ID_SEARCH_TYPE_SNAPSHOTID,
    };

    enum HostProtectedStatus
    {
        HOST_PROTECTED_STATUS_OFFLINE = 0, // 离线
        HOST_PROTECTED_STATUS_LOCAL = 1, // 本地
        HOST_PROTECTED_STATUS_NET = 2, // 网启
        HOST_PROTECTED_STATUS_VM = 3, // 虚拟机
        HOST_PROTECTED_STATUS_FCBOOT = 4, // 光纤    5是livecd
        HOST_PROTECTED_STATUS_DOCKER = 6, // DOCKER应急
		HOST_PROTECTED_STATUS_VMING = 7, //正在启动虚拟机
    };

    enum HostCanEmergencyStatus
    {
        HOST_CAN_EMERGENCY_STATUS_NO = 0,
        HOST_CAN_EMERGENCY_STATUS_YES = 999,

    };

    enum CopyType
    {
        COPY_TYPE_NORMAL = 0,
        COPY_TYPE_SNAPSHOT = 1,
    };

    // 副本状态

    enum CopyState
    {
        COPY_STATE_UNUSED = 0, // 未使用
        COPY_STATE_USED = 1, // 使用
        //        COPY_STATE_OPEN_UNLOGON = 2, // 打开未挂载，
        //        COPY_STATE_LOGOFF_UNCLOSED = 3, // 卸载未关闭
    };

    enum RemoteServerType
    {
        REMOTE_SERVER_TYPE_DC = 0, // 数据中心
        REMOTE_SERVER_TYPE_RC = 1, // 容灾中心
        REMOTE_SERVER_TYPE_BMS = 2, // BMS服务器
        REMOTE_SERVER_TYPE_VMS = 3, // VMS服务器
        REMOTE_SERVER_TYPE_SMS = 4, // 短信服务器
        REMOTE_SERVER_TYPE_VSS = 5, // 虚拟存储服务器
        REMOTE_SERVER_TYPE_MC = 6, // 集中管理器
    };

    enum HostResultSetRetrieveType
    {
        HOST_RESULTSET_RETRIEVE_TYPE_ALL,
        HOST_RESULTSET_RETRIEVE_TYPE_BUSINESS_NIC,
        HOST_RESULTSET_RETRIEVE_TYPE_ORIGINAL_SERVER_ID,
        HOST_RESULTSET_RETRIEVE_TYPE_SERVER_ID,
        HOST_RESULTSET_RETRIEVE_TYPE_FCNAME,
        HOST_RESULTSET_RETRIEVE_TYPE_RACGROUPID,
        HOST_RESULTSET_RETRIEVE_TYPE_ONLYFORBACKUP,
    };

    // 主机复制方式

    enum HostCopyMode
    {
        HOST_COPY_MODE_ISCSI = 0, // iscsi挂载磁盘
        HOST_COPY_MODE_RDS = 1, // rsync
        HOST_COPY_MODE_SHARE = 2, // 共享目录
        HOST_COPY_MODE_FC = 3, // 光纤卡
    };

    // 任务状态

    enum TaskState
    {
        TASK_STATE_IDLE = 0, // 空闲
        TASK_STATE_READY = 1, // 准备执行
        TASK_STATE_EXECUTING = 2, // 正在执行
        TASK_STATE_FINISH = 3, // 完成
    };

    // 任务结果

    enum TaskResult
    {
        TASK_RESULT_FAILED = 0, // 失败
        TASK_RESULT_SUCCESS = 1, // 成功
        TASK_RESULT_END = 2, // 结束
    };

    // 任务类型

    enum TaskType
    {
        TASK_TYPE_SYSTEM = 0, // 系统任务
        TASK_TYPE_BUSINESS = 1, // 业务任务
        TASK_TYPE_MONITOR = 2, // 监控任务
    };

    // 卷类型

    enum VolumeType
    {
        VOLUME_TYPE_LOCAL,
        VOLUME_TYPE_ISCSI,
        VOLUME_TYPE_FC,
    };

    // 策略类型

    enum StrategyType
    {
        STRATEGY_TYPE_UNKNOWN,
        STRATEGY_TYPE_SYSTEM,
        STRATEGY_TYPE_DATA,
        STRATEGY_TYPE_RAC,
        STRATEGY_TYPE_DB,
    };

    // 主机数获取方式

    enum HostCountRetrieveType
    {
        HOST_COUNT_RETRIEVE_TYPE_ALL,
        HOST_COUNT_RETRIEVE_TYPE_NET_EMERGENCY,
        HOST_COUNT_RETRIEVE_TYPE_VM_EMERGENCY,
        HOST_COUNT_RETRIEVE_TYPE_FC_EMERGENCY,
        HOST_COUNT_RETRIEVE_TYPE_DB_PROTECTED,
        HOST_COUNT_RETRIEVE_TYPE_EA,
    };

    // CDP连续快照时间单位

    enum CDPTimeUnit
    {
        CDP_TIME_UNIT_MINUTE = 0,
        CDP_TIME_UNIT_HOUR = 1,
        CDP_TIME_UNIT_DAY = 2,
    };

    // CDP状态
    enum CDPState
    {
        CDP_STATE_NOT_SYNC = 0,
        CDP_STATE_ALREADY_SYNC = 1,
        CDP_STATE_IS_SYNCING = 2,
    };

    // CDP模式
    enum CDPMode
    {
        CDP_MODE_DRBD = 0,
    };

    // 主机自动应急模式
    enum HostEAMode
    {
        HOST_EA_MODE_COLD_BOOT,
        HOST_EA_MODE_HOT_BOOT,
    };

    // 应急时主机副本挂载方式
    enum EmergencyLogonMode
    {
        EMERGENCY_LOGON_MODE_BY_BOOT_PROTOCOL,
        EMERGENCY_LOGON_MODE_BY_COPY_MODE,
    };

    // 报警级别
    enum AlarmLevel
    {
        ALARM_LEVEL_INFORMATION,
        ALARM_LEVEL_ATTENTION,
        ALARM_LEVEL_WARNING,
        ALARM_LEVEL_ERROR,
        ALARM_LEVEL_CRITICAL,
    };

    // 报警上报方式
    enum AlarmReportMode
    {
        ALARM_REPORT_MODE_BMS,
        ALARM_REPORT_MODE_SMS,
        ALARM_REPORT_MODE_MAIL,
    };

    // 报警上报状态
    enum AlarmReportState
    {
        ALARM_REPORT_STATE_NONE = 0,
        ALARM_REPORT_STATE_SUCCESS = 1,
        ALARM_REPORT_STATE_FAIL = 2,
        ALARM_REPORT_STATE_ERROR = 3,
    };

    // 报警模式
    enum AlarmMode
    {
        ALARM_MODE_NONE,
        ALARM_MODE_SMS,
        ALARM_MODE_MAIL,
        ALARM_MODE_SMS_AND_MAIL,
    };

    enum NicType
    {
        NIC_TYPE_STANDARD,
        NIC_TYPE_BRIDGED,
    };

    // 虚拟机启动副本类型
    enum VmBootCopy
    {
        VM_BOOT_COPY_EMERGENCY = 0,
        VM_BOOT_COPY_TRUSTED,
    };

    // 虚拟网络方式
    enum VirtualNetworkMode
    {
        VIRTUAL_NETWORK_MODE_ORIGINAL = 0,
        VIRTUAL_NETWORK_MODE_VIRTUALMAC,
        VIRTUAL_NETWORK_MODE_VIRTUALIP,
        VIRTUAL_NETWORK_MODE_BOTH_VIRTUAL,
    };

    enum DBMirrorState
    {
    	DB_MIRROR_STATE_UNUSED = 0,
    	DB_MIRROR_STATE_USED,
    	DB_MIRROR_STATE_TAKEOVER,
    };
public:


    //================================ 任务相关 ================================//

    /*
     *  功能：
     *      增加任务
     *  参数：
     *      cmdheader       :   命令头
     *      cmdcode         :   命令编码
     *      parameter       :   参数
     *      tasktype        :   任务类型
     *      state           :   状态
     *      hcode           :   主机编码
     *      result          :   结果
     *      faildesc        :   失败描述
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool AddTask(string cmdheader, string cmdcode, string parameter, DbDataManager::TaskState state, DbDataManager::TaskResult result, string faildesc);

    virtual bool AddDockerImage(string hcode, string hostip, string imagename, string imgid, string copyid, string targetfile, string mountpoint);
    virtual bool DeleteDockerImage(string imageid);
    virtual bool AddDockerContainer(string hcode, string containerid, string containername, string hostip, string imageid, string state);
    virtual map<string, string> GetDockerImageValue(string hcode, string copyid="");
    virtual bool DeleteDockerContainer(string containerid);
    virtual bool IsDockerImageExist(string imageid);
    virtual bool IsDockerImageHasContainer(string imageid);
    virtual bool IsDockerContainerExist(string containerid);
    virtual string GetVmTaskoverMethod(string hcode);
    virtual string GetMirrorServerIP(string hcode);
    virtual bool UpdateDbImageProtect(string hcode, string mirrorserverip, string mirrordbip, string mirrormode, DbDataManager::DBMirrorState mirrorstate);
    virtual bool UpdateDbMirrorState(string hcode, DbDataManager::DBMirrorState mirrorstate);
    virtual map<string, string> GetMirrorInfo(string hcode);
    virtual int GetPanelType();		//硬件应急面板类型，0为按钮，1为触摸屏

    virtual bool DeleteHostRacDisk(string hcode);
    virtual bool ClearRacGroupMasterHost(string groupid);
    virtual bool UpdateRacGroupMasterHost(string groupid, string hcode);
    virtual string GetRacGroupMasterHost(string groupid);
    virtual bool IsRacGroupExist(string groupid);
    virtual bool IsRacDiskExist(string hcode, string disksysmbol);
	virtual bool ChangeHostIpAddr(string hcode, string newip);

	virtual bool CreateSourceTable(string source);
	virtual bool CreateStrategyTable(string strategy);
	virtual bool UpdateExtractorProcess(string source, string pid);
	virtual bool UpdateLoaderProcess(string strategy, string pid);
	virtual string GetExtractorProcess(string source);
	virtual string GetLoaderProcess(string strategy);
	virtual sql::ResultSet * GetSourceDBResultSet();
	//获取复制策略结果集
	virtual sql::ResultSet * GetStrategyResultSet();
	virtual bool IsSourceDBExist(string source);
	virtual bool IsStrategyExist(string strategy);
	virtual int GetStrategyCount();
	virtual int GetSourceDBCount();
    virtual bool DeleteExpiredTrunkData(string source);
	virtual bool UpdateExtractorStatus(string source, string status);
	virtual bool UpdateLoaderStatus(string strategy, string status);
	virtual sql::ResultSet * GetAutostartSourceDBResultSet();
	virtual sql::ResultSet * GetAutostartStrategyResultSet();
	virtual bool AddTask(string cmdcode, string parameter);
	virtual bool DeleteStrategy(string strategy);
	virtual bool DeleteSourceDB(string source);
	virtual bool AddStrategy(string strategyname,string sid,string did,string scn,string syncusers,string interval,string errorskip,string autostart);
	virtual bool AddSourceDB(string dbtype,string sip,string instancename,string port,string username,string password,string tsname,string trunkscn,string branchscn,string interval);
	virtual string GetStrategyCreateTime(string strategy);

    /*
     *  功能：
     *      删除指定主机编码下的任务
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteTask(string hcode);

    /*
     *  功能：
     *      更新任务头和状态
     *  参数：
     *      taskid          :   任务ID
     *      cmdheader       :   命令头
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateTaskHeaderAndStatus(string taskid, string cmdheader);

    /*
     *  功能：
     *      更新任务状态
     *  参数：
     *      taskid          :   任务id
     *      status          :   任务状态
     *      result          :   任务结果
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateTaskStatus(string taskid, DbDataManager::TaskState state, DbDataManager::TaskResult result);

    /*
     *  功能：
     *      更新任务状态
     *  参数：
     *      taskid          :   任务id
     *      status          :   任务状态
     *      result          :   任务结果
     *      faildesc        :   失败描述
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateTaskStatus(string taskid, DbDataManager::TaskState state, DbDataManager::TaskResult result, string faildesc);

    /*
     *  功能：
     *      更新任务状态描述
     *  参数：
     *      taskid          :   任务id
     *      desc            :   状态信息
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateTaskStateDesc(string taskid, string desc);

    /*
     *  功能：
     *      更新未完成的任务
     */
    virtual bool UpdateUncompletedTask();

    /*
     *  功能：
     *      获取任务结果集
     */
    virtual sql::ResultSet * GetTaskResultSet();

    /*
     *  功能：
     *      获取指定任务状态的任务结果集
     *  参数：
     *      state           :   任务状态
     *  返回：
     *      任务结果集
     */
    virtual sql::ResultSet * GetTaskResultSet(TaskState state);

    /*
     *  功能：
     *      获取license更新任务
     *  参数：
     *      无
     *  返回：
     *      license更新任务字段
     */
    virtual map<string, string> GetLicenseUpdateTask();

    /*
     *  功能：
     *      获取任务记录
     *  参数：
     *      taskid          :   任务编号
     *  返回：
     *      任务记录字段
     */
    virtual map<string, string> GetTaskRecord(string taskid);

    /*
     *  功能：
     *      获取任务超时时间
     *  参数：
     *      taskid          :   任务编号
     *  返回：
     *      任务超时时间
     */
    virtual int GetTaskTimeout(string taskid);

    /*
     *  功能：
     *      获取删除主机任务结果集
     *  参数：
     *      无
     *  返回：
     *      任务结果集
     */
    virtual sql::ResultSet * GetDeleteHostTask();

    /*
     *  功能：
     *      获取任务状态
     *  参数：
     *      taskid          :   任务编号
     *  返回：
     *      任务状态
     */
    virtual DbDataManager::TaskState GetTaskState(string taskid);

    /*
     *  功能：
     *      获取任务id
     *  参数：
     *      cmdheader        :   命令头
     *  返回：
     *      任务id
     */
    virtual string GetTaskId(string cmdheader);

    /*
     *  功能：
     *      是否有未完成的任务
     *  参数：
     *      无
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool HasUncompletedTask();

    //================================ 网启相关 ================================//

    /*
     *  功能：
     *      更新应急副本id
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略编号
     *      copyid          :   副本ID
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateEmergencyCopyId(string hcode, string strategyid, string copyid);

    /*
     *  功能：
     *      更新网启副本ID
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略编号
     *      copyid          :   副本ID
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateTrustedCopyId(string hcode, string strategyid, string copyid);

    /*
     *  功能：
     *      更新CDP保护状态
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略编号
     *      protectionstate :   保护状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCDPProtectionState(string hcode, string strategyid, int protectionstate);

    //    /*
    //     *  功能：
    //     *      获取网启副本iqn
    //     *  参数：
    //     *      hcode           :   主机编码
    //     *      volumesymbol    :   盘符
    //     *  返回：
    //     *      网启副本iqn
    //     */
    //    virtual string GetNetbootCopyIqn(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取应急副本id
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      应急副本id
     */
    virtual string GetEmergencyCopyId(string hcode, string volumesymbol);

    //    /*
    //     *  功能：
    //     *      获取系统盘网启副本iqn
    //     *  参数：
    //     *      hcode           :   主机编码
    //     *  返回：
    //     *      网启副本iqn
    //     */
    //    virtual string GetSystemNetbootCopyIqn(string hcode);

    /*
     *  功能：
     *      获取系统盘应急副本id
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      网启副本id
     */
    virtual string GetSystemEmergencyCopyId(string hcode);

    /*
     *  功能：
     *      获取可信副本id
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      可信副本id
     */
    virtual string GetTrustedCopyId(string hcode, string lmountpoint);

    /*
     *  功能：
     *      获取系统盘可信副本id
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      系统盘可信副本id
     */
    virtual string GetSystemTrustedCopyId(string hcode);

    /*
     *  功能：
     *      获取主机emboot启动协议结果集
     *  参数：
     *      无
     *  返回：
     *      主机emboot启动协议结果集
     */
    virtual sql::ResultSet * GetHostEmbootResultSet();

    /*
     *  功能：
     *      获取主机挂载点
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      获取的主机挂载点
     */
    virtual string GetHostMountPoint(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略的CDP状态
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      CDP状态
     */
    virtual DbDataManager::CDPState GetStrategyCDPState(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略的CDP模式
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      CDP模式
     */
    virtual DbDataManager::CDPMode GetStrategyCDPMode(string hcode, string strategyid);


    /*
     *  功能：
     *      获取策略的设备号
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      设备号
     */
    virtual string GetStrategyDeviceMinor(string hcode, string strategyid);

    /*
     *  功能：
     *      检查策略是否用于CDP保护
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsStrategyUsedForCDP(string hcode, string strategyid);

    //    /*
    //     *  功能：
    //     *      网启副本是否使用中
    //     *  参数：
    //     *      hcode           :   主机编码
    //     *      snapshotid      :   快照编码
    //     *  返回：
    //     *      如果是返回true，否则返回false
    //     */
    //    virtual bool IsNetbootCopyUsed(string hcode, string snapshotid);

    //================================ 盘符相关 ================================//

    /*
     *  功能：
     *      增加卷记录
     *  参数：
     *      hcode           :   主机编码
     *      device          :   设备
     *      mountpoint      :   挂载点
     *      volumetype      :   分区类型
     *      capacity        :   容量
     *      used            :   已用空间
     *      isbootvolume    :   是否为启动卷
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddVolume(string hcode, string device, string mountpoint, string volumetype, string capacity, string usedspace, string isbootvolume);

    /*
     *  功能：
     *      增加卷记录
     *  参数：
     *      hcode           :   主机编码
     *      device          :   设备
     *      mountpoint      :   挂载点
     *      volumetype      :   分区类型
     *      capacity        :   容量
     *      used            :   已用空间
     *      isbootvolume    :   是否为启动卷
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool UpdateVolume(string hcode, string device, string mountpoint, string volumetype, string capacity, string usedspace, string isbootvolume);

    /*
     *  功能：
     *      获取盘符文件系统
     *  参数
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      盘符文件系统
     */
    virtual string GetVolumeFileSystem(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取主机卷结果集
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      主机卷结果集
     */
    virtual sql::ResultSet * GetVolumeResultSet(string hcode);

    /*
     *  功能：
     *      获取磁盘的已用空间大小
     *  参数
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      磁盘已用空间大小
     */
    virtual string GetVolumeUsedSpace(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取卷对应的设备
     *  参数
     *      hcode           :   主机编码
     *      volumesymbol    :   挂载点
     *  返回：
     *      卷对应的设备
     */
    virtual string GetVolumeDevice(string hcode, string volumesymbol);

    /*
     *  功能：
     *      盘符是否存在
     *  参数
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsVolumeSymbolExist(string hcode, string volumesymbol);

    /*
     *  功能：
     *      根据副本iqn判断是否为启动分区类型
     *  参数
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsBootPartitionType(string hcode, string volumesymbol);

    //================================ 副本相关 ================================//
    /*
     *  功能：
     *      增加副本记录
     *  参数：
     *      snapshotid      :   快照ID
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      copytype        :   副本类型
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddCopyRecord(string snapshotid, string hcode, string volumesymbol, DbDataManager::CopyType type = DbDataManager::COPY_TYPE_NORMAL);

    /*
     *  功能：
     *      增加副本记录
     *  参数：
     *      snapshotid      :   快照ID
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      copylunid       :   副本lunid
     *      copytype        :   副本类型
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddCopyRecord(string snapshotid, string hcode, string volumesymbol, string copylunid, DbDataManager::CopyType type = DbDataManager::COPY_TYPE_NORMAL);

    /*
     *  功能：
     *      增加副本记录
     *  参数：
     *      snapshotid      :   快照ID
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      used            :   副本使用标志
     *      iqn             :   iqn
     *      mountpoint      :   挂载点
     *      copyid          :   副本id
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddCopyRecord(string snapshotid, string hcode, string volumesymbol, string used, string iqn, string mountpoint, string copyid);

    /*
     *  功能：
     *      删除副本记录
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteCopy(string hcode, string copyid);

    /*
     *  功能：
     *      删除副本记录集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteCopySet(string hcode);

    /*
     *  功能：
     *      根据副本id更新副本状态
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      state          :   状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCopyStatus(string hcode, string copyid, DbDataManager::CopyState state);

    /*
     *  功能：
     *      更新副本根路径
     *  参数：
     *      snapshotpath    :   快照路径
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCopyRootPath(string copypath);

    /*
     *  功能：
     *      更新副本记录
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      iqn             :   iqn
     *      mountpoint      :   挂载点
     *      targetfile      :   target文件
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCopy(string hcode, string copyid, string iqn, string mountpoint, string targetfile);

    /*
     *  功能：
     *      更新指定副本的快照id
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      snapshotid      :   快照id
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCopySnapshotId(string hcode, string copyid, string snapshotid);

    /*
     *  功能：
     *      更新指定副本的副本卷id
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      copylunid       :   副本卷id
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCopyLunId(string hcode, string copyid, string copylunid);

    /*
     *  功能：
     *      更新指定副本的发送端副本编号
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      sendercopyid    :   发送端副本编号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCopySenderCopyId(string hcode, string copyid, string sendercopyid);

    /*
     *  功能：
     *      更新指定副本的zfs文件系统名称
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      fsname          :   文件系统名称
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCopyFsName(string hcode, string copyid, string fsname);

    /*
     *  功能：
     *      更新指定副本的loop设备名称
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      loopdevice      :   loop设备
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCopyLoopDevice(string hcode, string copyid, string loopdevice);

    /*
     *  功能：
     *      根据值获取对应副本ID
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照ID
     *      type            :   获取类型
     *  返回：
     *      副本ID
     */
    virtual string GetCopyId(string hcode, string value, DbDataManager::CopyIDRetrieveType type);

    /*
     *  功能：
     *      获取副本结果集
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      副本结果集
     */
    virtual sql::ResultSet * GetCopyResultSet(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取副本结果集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      副本结果集
     */
    virtual sql::ResultSet * GetCopyResultSet(string hcode);

    /*
     *  功能：
     *      获取副本记录字段值
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本记录字段值
     */
    virtual map<string, string> GetCopyFieldValue(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本零id
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      副本零id
     */
    virtual string GetZeroCopyId(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取最新副本id
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      最新副本id
     */
    virtual string GetFinalCopyId(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取主机非系统应急副本结果集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机网启结果集
     */
    virtual sql::ResultSet * GetNonSystemEmergencyCopyResultSet(string hcode);

    /*
     *  功能：
     *      获取定时快照应急副本结果集
     *  参数：
     *      无
     *  返回：
     *      应急副本结果集
     */
    virtual sql::ResultSet * GetTimingSnapshotEmergencyCopyResultSet();

    /*
     *  功能：
     *      获取副本根路径
     *  参数：
     *      无
     *  返回：
     *      副本根路径
     */
    virtual string GetCopyRootPath();

    /*
     *  功能：
     *      获取副本lunid
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本lunid
     */
    virtual string GetCopyLunId(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本记录的快照id
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本记录的快照id
     */
    virtual string GetCopySnapshotId(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本类型
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本类型
     */
    virtual DbDataManager::CopyType GetCopyType(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本iqn
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本iqn
     */
    virtual string GetCopyIqn(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本target文件
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本target文件
     */
    virtual string GetCopyTargetFile(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本对应的卷标识
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本对应的卷标识
     */
    virtual string GetCopyLMountPoint(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本挂载点
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本挂载点
     */
    virtual string GetCopyMountPoint(string hcode, string copyid);

    /*
     *  功能：
     *      获取指定类型的副本个数
     *  参数：
     *      hcode           :   主机编码
     *      type            :   类型
     *  返回：
     *      副本个数
     */
    virtual int GetCopyCountWithType(string hcode, DbDataManager::CopyType type);

    /*
     *  功能：
     *      获取副本状态
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本状态
     */
    virtual DbDataManager::CopyState GetCopyState(string hcode, string copyid);

    /*
     *  功能：
     *      获取发送端副本编号
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      发送端副本编号
     */
    virtual string GetCopySenderCopyId(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本容量
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      副本容量
     */
    virtual int GetCopyCapacity(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本zfs文件系统名称
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      zfs文件系统名称
     */
    virtual string GetCopyFsName(string hcode, string copyid);

    /*
     *  功能：
     *      获取副本loop设备名称
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      loop设备名称
     */
    virtual string GetCopyLoopDevice(string hcode, string copyid);

    /*
     *  功能：
     *      检查副本id是否为应急副本
     *  参数：
     *      hcode           :   主机编码
     *      id              :   副本编号
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsCopyAsEmergencyCopy(string hcode, string copyid);

    /*
     *  功能：
     *      副本是否存在
     *  参数
     *      hcode           :   主机编码
     *      iqn             :   副本IQN
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsCopyExist(string hcode, string copyid);

    /*
     *  功能：
     *      副本是否使用中
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostHasCopyUsed(string hcode);

    /*
     *  功能：
     *      副本是否使用中
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsVolumeHasCopyUsed(string hcode, string volumesymbol);

    /*
     *  功能：
     *      副本是否使用中
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsCopyUsed(string hcode, string copyid);

    //================================ 快照相关 ================================//

    /*
     *  功能：
     *      增加快照记录
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      snaptype        :   快照类型
     *      ostype          :   操作系统类型
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddSnapshotRecord(string hcode, string volumesymbol, DbDataManager::SnapType type);

    /*
     *  功能：
     *      增加快照记录
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      snaptype        :   快照类型
     *      starttime       :   开始时间
     *      endtime         :   结束时间
     *      checkpointid    :   checkpoint编号
     *      source          :   快照源
     *      synccopyid      :   同步副本id
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddSnapshotRecord(string hcode, string volumesymbol, DbDataManager::SnapType type, string starttime, string endtime, string checkpointid, DbDataManager::SnapshotSource source, string synccopyid);

    /*
     *  功能：
     *      增加快照记录
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      snapshottype    :   快照类型
     *      starttime       :   开始时间
     *      endtime         :   结束时间
     *      checkpointid    :   checkpoint编号
     *      source          :   快照源
     *      synccopyid      :   同步副本id
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddSnapshotRecord(string hcode, string volumesymbol, string snapshottype, string starttime, string endtime, string checkpointid, DbDataManager::SnapshotSource source, string synccopyid);


    /*
     *  功能：
     *      增加快照记录
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      snapshottype    :   快照类型
     *      starttime       :   开始时间
     *      endtime         :   结束时间
     *      checkpointid    :   checkpoint编号
     *      source          :   快照源
     *      synccopyid      :   同步副本id
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddSnapshotRecord(string hcode, string volumesymbol, string snapshottype, string starttime, string endtime, string checkpointid, string source, string synccopyid);

    /*
     *  功能：
     *      删除快照
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteSnapshot(string hcode, string snapshotid);

    /*
     *  功能：
     *      删除快照零记录
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteZeroSnapshot(string hcode, string volumesymbol);

    /*
     *  功能：
     *      删除所有快照
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteAllSnapshot(string hcode);

    /*
     *  功能：
     *      删除最新快照记录
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteFinalSnapshot(string hcode, string volumesymbol);

    /*
     *  功能：
     *      更新快照根路径
     *  参数：
     *      snapshotpath    :   快照路径
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateSnapshotRootPath(string snapshotpath);

    //    /*
    //     *  功能：
    //     *      更新最新快照结束时间
    //     *  参数：
    //     *      hcode           :   主机编码
    //     *      volumesymbol    :   盘符
    //     *  返回：
    //     *      更新成功返回true，否则返回false
    //     */
    //    virtual bool UpdateFinalSnapshotEndTime(string hcode, string volumesymbol);

    /*
     *  功能：
     *      更新最新快照结束时间
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      snapshotlunid   :   快照lunid
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateFinalSnapshotEndTime(string hcode, string volumesymbol, string snapshotlunid);

    /*
     *  功能：
     *      更新快照为可信/取消可信
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照编号
     *      trustedflag     :   可信标志
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateSnapshotTrusted(string hcode, string snapshotid, bool trustedflag);

    /*
     *  功能：
     *      更新快照的结束时间
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照编号
     *      copyid          :   副本编号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateSnapshotEndTime(string hcode, string snapshotid, string copyid);

    /*
     *  功能：
     *      更新快照的结束时间
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照编号
     *      endtime         :   结束时间
     *      checkpointid    :   checkpoint编号
     *      source          :   快照源
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateSnapshotEndTime(string hcode, string snapshotid, string copyid, string endtime, string checkpointid, DbDataManager::SnapshotSource source);

    /*
     *  功能：
     *      更新快照的远程同步状态
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照编号
     *      state           :   远程同步状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateSnapshotSyncState(string hcode, string snapshotid, DbDataManager::SnapshotSyncState state);

    /*
     *  功能：
     *      更新同步副本对应快照的远程同步状态
     *  参数：
     *      hcode           :   主机编码
     *      synccopyid      :   同步副本id
     *      state           :   远程同步状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateSnapshotSyncStateBySyncCopyId(string hcode, string synccopyid, DbDataManager::SnapshotSyncState state);

    /*
     *  功能：
     *      获取快照路径
     *  参数：
     *      无
     *  返回：
     *      快照路径
     */
    virtual string GetSnapshotRootPath();

    /*
     *  功能：
     *      获取快照个数
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *      ostype          :   操作系统类型
     *  返回：
     *      快照个数
     */
    virtual int GetSnapshotCount(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取快照零id
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      快照零id
     */
    virtual string GetZeroSnapshotId(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取最新快照id
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符
     *  返回：
     *      快照id
     */
    virtual string GetFinalSnapshotId(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取快照lunid
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *  返回：
     *      快照lunid
     */
    virtual string GetSnapshotLunId(string hcode, string snapshotid);

    /*
     *  功能：
     *      获取指定盘符的快照结果集
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符或策略id
     *  返回：
     *      快照结果集
     */
    virtual sql::ResultSet * GetSnapshotResultSet(string hcode, string volumesymbol);

    /*
     *  功能：
     *      获取该主机下的所有快照记录结果集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      快照结果集
     */
    virtual sql::ResultSet * GetSnapshotResultSet(string hcode);

    /*
     *  功能：
     *      获取指定盘符、同步状态和同步副本id的快照结果集
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符或策略id
     *      state           :   快照同步状态
     *      synccopyid      :   同步副本id
     *  返回：
     *      快照结果集
     */
    virtual sql::ResultSet * GetSnapshotResultSet(string hcode, string volumesymbol, DbDataManager::SnapshotSyncState state, string syncopyid);

    /*
     *  功能：
     *      获取指定盘符、同步状态和同步副本id的快照结果集
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符或策略id
     *      synccopyid      :   同步副本id
     *  返回：
     *      快照结果集
     */
    virtual sql::ResultSet * GetSnapshotResultSet(string hcode, string volumesymbol, string syncopyid);

    /*
     *  功能：
     *      获取快照结束时间
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *  返回：
     *      快照结束时间
     */
    virtual string GetSnapshotEndTime(string hcode, string snapshotid);

    /*
     *  功能：
     *      获取快照对应的checkpoint编号
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *  返回：
     *      快照对应的checkpoint编号
     */
    virtual string GetSnapshotCheckPointId(string hcode, string snapshotid);

    /*
     *  功能：
     *      获取快照同步副本id
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *  返回：
     *      快照同步副本id
     */
    virtual string GetSnapshotSyncCopyId(string hcode, string snapshotid);

    /*
     *  功能：
     *      获取快照的远程同步状态
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *  返回：
     *      远程同步状态
     */
    virtual DbDataManager::SnapshotSyncState GetSnapshotSyncState(string hcode, string snapshotid);

    /*
     *  功能：
     *      获取快照记录字段值
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *  返回：
     *      快照记录字段值
     */
    virtual map<string, string> GetSnapshotFieldValue(string hcode, string snapshotid);

    /*
     *  功能：
     *      获取上次同步快照id
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符或策略id
     *      synccopyid      :   同步副本id
     *  返回：
     *      快照记录字段值
     */
    virtual string GetLastSyncSnapshotId(string hcode, string volumesymbol, string synccopyid);

    /*
     *  功能：
     *      获取快照id
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符或策略id
     *      synccopyid      :   同步副本id
     *      starttime       :   开始时间
     *      endtime         :   结束时间
     *  返回：
     *      快照id
     */
    virtual string GetSnapshotId(string hcode, string volumesymbol, string synccopyid, string starttime, string endtime);

    /*
     *  功能：
     *      检查快照id是否为快照零
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *  返回：
     *      如果为快照零返回true，否则返回false
     */
    virtual bool IsZeroSnapshot(string hcode, string snapshotid);

    /*
     *  功能：
     *      检查快照id是否为快照零
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符或策略id
     *  返回：
     *      如果快照零存在返回true，否则返回false
     */
    virtual bool IsZeroSnapshotExist(string hcode, string volumesymbol);

    /*
     *  功能：
     *      快照是否存在
     *  参数
     *      hcode           :   主机编码
     *      snapshotid      :   快照ID
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsSnapshotExist(string hcode, string snapshotid);


    /*
     *  功能：
     *      快照是否存在
     *  参数
     *      hcode           :   主机编码
     *      lmountpoint     :   本地挂载点
     *      starttime       :   开始时间
     *      endtime         :   结束时间
     *      synccopyid      :   同步副本id
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsSnapshotExist(string hcode, string lmountpoint, string starttime, string endtime, string synccopyid);

    /*
     *  功能：
     *      是否最新快照
     *  参数
     *      hcode           :   主机编码
     *      snapshotid      :   快照ID
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsFinalSnapshot(string hcode, string snapshotid);

    /*
     *  功能：
     *      快照是否有对应的副本
     *  参数
     *      hcode           :   主机编码
     *      snapshotid      :   快照ID
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsSnapshotHasCopy(string hcode, string snapshotid);

    /*
     *  功能：
     *      是否为可信快照
     *  参数
     *      hcode           :   主机编码
     *      snapshotid      :   快照ID
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsTrustedSnapshot(string hcode, string snapshotid);

    /*
     *  功能：
     *      获取快照类型字符串
     *  参数
     *      type            :   快照类型
     *  返回：
     *      快照类型字符串
     */
    virtual string GetSnapshotTypeString(DbDataManager::SnapType type);

    //================================ 策略相关 ================================//

    /*
     *  功能：
     *      增加策略
     *  参数：
     *      hcode           :   主机编码
     *      lmountpoint     :   本地挂载点
     *      name            :   策略名
     *      type            :   策略类型
     *      capacity        :   容量
     *      copyid          :   网启副本id
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddStrategy(string hcode, string lmountpoint, string name, string type, string capacity, string copyid);

    /*
     *  功能：
     *      增加策略
     *  参数：
     *      localserverid   :   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      hcode           :   主机编码
     *      lmountpoint     :   本地挂载点
     *      name            :   策略名
     *      type            :   策略类型
     *      isfullcopy      :   是否全盘复制
     *      iqn             :   iqn
     *      targetfile      :   磁盘文件
     *      mountpoint      :   挂载点
     *      capacity        :   容量
     *      mirrorlunid     :   镜像卷映射id
     *      mmlunid         :   镜像卷的镜像卷映射id
     *      volumepairid    :   卷对id
     *      lvn             :   本地卷名
     *      rvn             :   网盘对应卷名
     *      lvoltag         :   本地卷标
     *      rvoltag         :   网盘对应卷表
     *      usedforcdp      :   是否用于CDP
     *      cdpstate        :   CDP状态
     *      maxsnapshot     :   最大允许快照数
     *      usedfortimeout  :   是否设置复制超时
     *      copytimeout     :   复制超时时间
     *      cdpmode         :   CDP模式
     *      datasource      :   数据源
     *      deviceminor     :   设备号
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddStrategy(string localserverid, string remoteserverid, string hcode, string lmountpoint, string name, string type, string isfullcopy, string iqn, string targetfile, string mountpoint,
            string capacity, string mirrorlunid, string mmlunid, string volumepairid, string lvn, string rvn, string lvoltag, string rvoltag,
            string usedforcdp, string cdpstate, string maxsnapshot, string usedfortimeout, string copytimeout, string cdpmode, string datasource, string deviceminor);

    /*
     *  功能：
     *      增加策略
     *  参数：
     *      localserverid   :   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      strategyid      :   策略id
     *      scheduleid      :   本地挂载点
     *      groupid         :   策略名
     *      operation       :   操作
     *      foldername      :   目录
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddStrategyDetail(string localserverid, string remoteserverid, string strategyid, string scheduleid, string groupid, string operation, string foldername);

    /*
     *  功能：
     *      删除策略
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteStrategy(string hcode, string strategyid);

    /*
     *  功能：
     *      删除策略集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteStrategySet(string hcode);

    /*
     *  功能：
     *      删除策略明晰
     *  参数：
     *      strategyid      :   策略id
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteStrategyDetail(string strategyid);

    /*
     *  功能：
     *      更新策略
     *  参数：
     *      localserverid   :   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      hcode           :   主机编码
     *      lmountpoint     :   本地挂载点
     *      name            :   策略名
     *      type            :   策略类型
     *      isfullcopy      :   是否全盘复制
     *      iqn             :   iqn
     *      targetfile      :   磁盘文件
     *      mountpoint      :   挂载点
     *      capacity        :   容量
     *      mirrorlunid     :   镜像卷映射id
     *      mmlunid         :   镜像卷的镜像卷映射id
     *      volumepairid    :   卷对id
     *      lvn             :   本地卷名
     *      rvn             :   网盘对应卷名
     *      lvoltag         :   本地卷标
     *      rvoltag         :   网盘对应卷表
     *      usedforcdp      :   是否用于CDP
     *      cdpstate        :   CDP状态
     *      maxsnapshot     :   最大允许快照数
     *      usedfortimeout  :   是否设置复制超时
     *      copytimeout     :   复制超时时间
     *      cdpmode         :   CDP模式
     *      datasource      :   数据源
     *      deviceminor     :   设备号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategy(string localserverid, string remoteserverid, string hcode, string lmountpoint, string name, string type, string isfullcopy, string iqn, string targetfile, string mountpoint,
            string capacity, string mirrorlunid, string mmlunid, string volumepairid, string lvn, string rvn, string lvoltag, string rvoltag,
            string usedforcdp, string cdpstate, string maxsnapshot, string usedfortimeout, string copytimeout, string cdpmode, string datasource, string deviceminor);

    /*
     *  功能：
     *      更新策略
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略编号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategyAndDetailServerId(string hcode, string strategyid);

    /*
     *  功能：
     *      更新策略
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略编号
     *      localserverid   :   本地服务器id
     *      remoteserverid  :   远程服务器id
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategyAndDetailServerId(string hcode, string strategyid, string localserverid, string remoteserverid);

    /*
     *  功能：
     *      更新策略镜像卷id
     *  参数：
     *      strategyid      :   策略id
     *      volumeid        :   卷id
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategyMirrorVolumeId(string strategyid, string volumeid);

    /*
     *  功能：
     *      更新策略镜像卷的镜像卷id
     *  参数：
     *      strategyid      :   策略id
     *      volumeid        :   卷id
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategyMMVolumeId(string strategyid, string volumeid);

    /*
     *  功能：
     *      更新策略卷对id
     *  参数：
     *      strategyid      :   策略id
     *      volumeid        :   卷对id
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategyVolumePairId(string strategyid, string pairid);

    /*
     *  功能：
     *      更新策略存储空间id
     *  参数：
     *      strategyid      :   策略id
     *      storageid       :   存储空间id
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategyStorageId(string strategyid, string storageid);

    /*
     *  功能：
     *      更新策略磁盘空间大小
     *  参数：
     *      strategyid      :   策略id
     *      newcapacity     :   新的磁盘大小
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategyVolumeCapacity(string strategyid, string newcapacity);

    /*
     *  功能：
     *      更新策略设备号
     *  参数：
     *      strategyid      :   策略id
     *      deviceminor     :   设备号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStrategyDeviceMinor(string strategyid, string deviceminor);

    /*
     *  功能：
     *      获取复制策略结果集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      复制策略结果集
     */
    virtual sql::ResultSet * GetStrategyResultSet(string hcode);

    /*
     *  功能：
     *      根据策略类型获取策略结果集
     *  参数：
     *      hcode           :   主机编码
     *      strategytype    :   策略类型
     *  返回：
     *      复制策略结果集
     */
    virtual sql::ResultSet * GetStrategyResultSet(string hcode, StrategyType strategytype);

    /*
     *  功能：
     *      获取复制策略明细结果集
     *  参数：
     *      strategyid      :   策略id
     *  返回：
     *      复制策略明细结果集
     */
    virtual sql::ResultSet * GetStrategyDetailResultSet(string strategyid);

    /*
     *  功能：
     *      获取复制策略明细结果集
     *  参数：
     *      strategyid      :   策略id
     *      scheduleid      :   调度计划id
     *  返回：
     *      复制策略明细结果集
     */
    virtual sql::ResultSet * GetStrategyDetailResultSet(string strategyid, string scheduleid);

    /*
     *  功能：
     *      获取复制策略明细结果集
     *  参数：
     *      strategyid      :   策略id
     *      scheduleid      :   调度计划id
     *  返回：
     *      复制策略明细结果集
     */
    virtual sql::ResultSet * GetStrategyDetailResultSet(string strategyid, string scheduleid, DbDataManager::DetailType type);

    /*
     *  功能：
     *      获取复制策略明细结果集
     *  参数：
     *      strategyid      :   策略id
     *      type            :   明细类型
     *  返回：
     *      复制策略明细结果集
     */
    virtual sql::ResultSet * GetStrategyDetailResultSet(string strategyid, DbDataManager::DetailType type);

    /*
     *  功能：
     *      获取复制策略id
     *  参数：
     *      hcode           :   主机编码
     *      condition       :   查找条件
     *      type            :   查找类型
     *  返回：
     *      复制策略id
     */
    virtual string GetStrategyId(string hcode, string condition, DbDataManager::StrategyIdSearchType type);

    /*
     *  功能：
     *      获取卷指定容量
     *  参数：
     *      hcode           :   主机编码
     *      volumesymbol    :   查找条件
     *  返回：
     *      卷指定容量
     */
    virtual string GetVolumeCapacity(string hcode, string volumesymbol);


    /*
     *  功能：
     *      获取镜像卷的id
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      镜像卷id
     */
    virtual string GetMirrorLunId(string hcode, string strategyid);

    /*
     *  功能：
     *      获取镜像卷的镜像卷id
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      镜像卷的镜像卷id
     */
    virtual string GetMMLunId(string hcode, string strategyid);

    /*
     *  功能：
     *      获取卷对id
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      镜像卷对id
     */
    virtual string GetVolumePairId(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略的当前卷类型
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      当前卷类型
     */
    virtual DbDataManager::VolumeType GetCurrentVolumeType(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略存储路径
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      策略存储路径
     */
    virtual string GetStrategyStoragePath(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略存储id
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      策略存储id
     */
    virtual string GetStrategyStorageId(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略存储设备
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      策略存储id
     */
    virtual string GetStrategyStorageDevice(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略存储文件系统
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      策略存储文件系统
     */
    virtual string GetStrategyStorageFileSystem(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略最大快照数
     *  参数：
     *      hcode           :  主机编码
     *      strategyid      :  策略id
     *  返回：
     *      策略最大快照数
     */
    virtual int GetStrategyMaxSnapshot(string hcode, string strategyid);

    /*
     *  功能：
     *      获取CDP个数
     *  参数：
     *      无
     *  返回：
     *      CDP个数
     */
    virtual int GetCDPCount();

    /*
     *  功能：
     *      获取CDP个数
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略编号
     *  返回：
     *      CDP个数
     */
    virtual int GetCDPCount(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略类型
     *  参数：
     *      hcode           :  主机编码
     *      strategyid      :  策略id
     *  返回：
     *      策略类型
     */
    virtual DbDataManager::StrategyType GetStrategyType(string hcode, string strategyid);

    /*
     *  功能：
     *      获取策略复制超时时间（分钟）
     *  参数：
     *      hcode           :  主机编码
     *      strategyid      :  策略id
     *  返回：
     *      复制超时时间
     */
    virtual int GetStrategyCopyTimeout(string hcode, string strategyid);

    /*
     *  功能：
     *      策略是否存在
     *  参数
     *      hcode           :   主机编码
     *      strategyid      :   策略ID
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsStrategyExist(string hcode, string strategyid);

    /*
     *  功能：
     *      检查卷的策略是否存在
     *  参数
     *      hcode           :   主机编码
     *      localmountpoint :   策略ID
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsVolumeHasStrategy(string hcode, string localmountpoint);

    /*
     *  功能：
     *      检查策略是否存在数据库目录
     *  参数
     *      strategyid      :   策略ID
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsStrategyHasDb(string strategyid);

    /*
     *  功能：
     *      检查策略是否存在数据库目录
     *  参数
     *      strategyid      :   策略ID
     *      scheduleid      :   调度计划id
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsStrategyHasDb(string strategyid, string sheduleid);

    /*
     *  功能：
     *      检查策略是否存在全部复制操作
     *  参数
     *      strategyid      :   策略ID
     *      scheduleid      :   调度计划id
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsStrategyHasFull(string strategyid, string scheduleid);

    /*
     *  功能：
     *      检查策略是否存在包含目录
     *  参数
     *      strategyid      :   策略ID
     *      scheduleid      :   调度计划id
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsStrategyHasInclude(string strategyid, string scheduleid);


    /*
     *  功能：
     *      检查策略是否为rac
     *  参数
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsStrategyRac(string hcode, string strategyid);

    /*
     *  功能：
     *      检查策略是否用于复制超时
     *  参数
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsStrategyUsedForTimeout(string hcode, string strategyid);

    /*
     *  功能：
     *      是否为该存储上的最后一个策略
     *  参数
     *      hcode           :   主机编码
     *      strategyid      :   策略id
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsFinalStrategyInStorage(string hcode, string strategyid);

    /*
     *  功能：
     *      检查策略是否有副本
     *  参数
     *      hcode           :   主机编码
     *      strategyid      :   策略ID
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsStrategyHasCopy(string hcode, string strategyid);

    //================================ 主机相关 ================================//


    /*
     *  功能：
     *      增加主机
     *  参数：
     *      hcode           :   主机编码
     *      name            :   主机名称
     *      ipaddr          :   主机ip
     *      ostype          :   操作系统类型
     *      networkprotocol :   网启协议
     *      netdefaultprotocol  :   默认网启协议
     *      macaddr         :   MAC地址
     *      wwn             :   FCWWN
     *      port            :   端口
     *      businessnicname :   业务网卡名
     *      brnicname       :   桥接网卡名
     *      storageid       :   存储空间id
     *      copymode        :   复制方式
     *      usedfornet      :   用于网启
     *      usedforvm       :   用于虚拟机
     *      usedforfc       :   用于光纤
     *      racgroupid      :   rac组id
     *      dbprotected     :   是否保护数据库
     *      fcname          :   业务光纤卡
     *      vncnumber       :   vnc号
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddHost(string hcode, string name, string ipaddr, string ostype, string netbootprotocol, string netdefaultprotocol, string macaddr, string wwn, string port, string businessnicname,
            string brnicname, string storageid, string copymode, string usedfornet, string usedforvm, string usedforfc, string racgroupid, string dbprotected, string fcname, string vncnumber);
    /*
     *  功能：
     *      增加主机
     *  参数：
     *      localserverid   :   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      hcode           :   主机编码
     *      name            :   主机名称
     *      macaddr         :   MAC地址
     *      ipaddr          :   主机ip
     *      bootfile        :   启动文件
     *      status          :   启动状态
     *      isactive        :   是否可应急
     *      hdnum           :   硬盘数量
     *      usingrhost      :   是否使用备机
     *      rhosmac         :   备机MAC地址
     *      hbaipaddr       :   HBA卡ip地址
     *      isnetboot       :   是否为网启
     *      netdiskcapactity:   网启盘大小
     *      dbprotected     :   是否为数据库保护
     *      ostype          :   操作系统类型
     *      netbootprotocol :   网启协议
     *      netdefaultprotocol  :   默认网启协议
     *      netbootservice  :   网启启动服务标志
     *      fcwwn           :   FCWWN
     *      systemdir       :   系统目录
     *      configdir       :   配置文件目录
     *      vmmemory        :   虚拟机内存
     *      vmcpu           :   虚拟机CPU
     *      listenport      :   监听端口
     *      usedforrdr      :   是否用于容灾
     *      usedforle      :   是否用于本地应急
     *      businessnicname :   业务网卡名
     *      brnicname       :   桥接网卡名
     *      storageid       :   存储空间id
     *      copymode        :   复制方式
     *      syncflag        :   同步主机信息标志
     *      fcname          :   光纤卡名称
     *      fcchannel       :   光纤通道号
     *      initiatorname   :   initiator名称
     *      usedfornet      :   用于网启应急
     *      usedforvm       :   用于虚拟机应急
     *      usedforfc       :   用于光纤卡应急
     *      cdpstate        :   CDP状态
     *      usedforea       :   用于自动应急
     *      eamode          :   自动应急方式
     *      heartbeattimeout:   心跳超时时间
     *      queryinterval   :   查询间隔
     *      virtualip       :   虚拟ip
     *      virtualmac      :   虚拟mac
     *      racgroupid      :   RAC组编号
     *      emergencylogonmode:   应急时副本挂载方式
     *      usedformonitor  :   是否打开监控
     *      isautotakeover  :   是否自动接管
     *      vmbootcopymode  :   虚拟机启动副本模式
     *      vncnumber       :   VNC号
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddHost(string localserverid, string remoteserverid, string hcode, string name, string macaddr, string ipaddr, string bootfile, string status,
            string isactive, string hdnum, string usingrhost, string rhostmac, string hbaipaddr, string isnetboot, string netdiskcapacity, string dbprotected, string ostype,
            string netbootprotocol, string netdefaultprotocol, string netbootservice, string fcwwn, string systemdir, string configdir, string vmmemory, string vmcpu, string listenport,
            string usedforrdr, string usedforle, string businessnicname, string brnicname, string storageid, string copymode, string syncflag, string fcname, string fcchannel,
            string initiatorname, string usedfornet, string usedforvm, string usedforfc, string cdpstate, string usedforea, string eamode, string heartbeattimeout,
            string queryinterval, string virtualip, string virtualmac, string racgroupid, string emergencylogonmode, string usedformonitor, string isautotakeover, string vmbootcopymode, string vncnumber);
    /*
     *  功能：
     *      增加主机网卡
     *  参数：
     *      originalserverid:   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      hcode           :   主机编码
     *      macaddr         :   mac地址
     *      devicename      :   设备名
     *      caption         :   描述
     *      gateway         :   网关
     *      maindns         :   主dns
     *      subdns          :   备用dns
     *      dhcpenabled     :   是否为dhcp
     *      isphysical      :   是否物理网卡
     *      status          :   网卡状态
     *      workmode        :   工作模式
     *      usedforvm       :   是否用于虚拟机
     *      businessnic     :   业务网卡
     *      bridgenic       :   桥接网卡
     *      virtualnetworkmode  :   虚拟网络方式
     *      virtualmac      :   虚拟mac
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool AddHostNic(string originalserverid, string remoteserverid, string hcode, string macaddr, string devicename, string caption, string gateway, string maindns, string subdns,
            string dhcpenabled, string isphysical, string status, string workmode, string usedforvm, string businessnic, string bridgenic, string virtualnetworkmode, string virtualmac);
    /*
     *  功能：
     *      增加主机ip地址
     *  参数：
     *      originalserverid:   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      hcode           :   主机编码
     *      macaddr         :   mac地址
     *      ip              :   ip地址
     *      netmask         :   掩码
     *      ipv6            :   ipv6地址
     *      netmaskv6       :   掩码v6
     *      workmode        :   工作模式
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool AddHostIpAddress(string originalserverid, string remoteserverid, string hcode, string macaddr, string ip, string netmask, string ipv6, string netmaskv6, string workmode);

    /*
     *  功能：
     *      删除主机记录
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteHost(string hcode);

    /*
     *  功能：
     *      删除主机相关记录
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteHostRelativeRecord(string hcode);

    /*
     *  功能：
     *      删除主机记录
     *  参数：
     *      无
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteHostSet();

    /*
     *  功能：
     *      更新主机信息
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHost(string hcode);

    /*
     *  功能：
     *      更新主机信息
     *  参数：
     *      hcode           :   主机编码
     *      mac             :   MAC地址
     *      hdnum           :   硬盘数量
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHost(string hcode, string mac, string hdnum);

    /*
     *  功能：
     *      更新主机信息
     *  参数：
     *      localserverid   :   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      hcode           :   主机编码
     *      name            :   主机名称
     *      macaddr         :   MAC地址
     *      ipaddr          :   主机ip
     *      bootfile        :   启动文件
     *      status          :   启动状态
     *      isactive        :   是否可应急
     *      hdnum           :   硬盘数量
     *      usingrhost      :   是否使用备机
     *      rhosmac         :   备机MAC地址
     *      hbaipaddr       :   HBA卡ip地址
     *      isnetboot       :   是否为网启
     *      netdiskcapactity:   网启盘大小
     *      dbprotected     :   是否为数据库保护
     *      ostype          :   操作系统类型
     *      netbootprotocol :   网启协议
     *      netdefaultprotocol  :   默认网启协议
     *      netbootservice  :   网启启动服务标志
     *      fcwwn           :   FCWWN
     *      systemdir       :   系统目录
     *      configdir       :   配置文件目录
     *      vmmemory        :   虚拟机内存
     *      vmcpu           :   虚拟机CPU
     *      listenport      :   监听端口
     *      usedforrdr      :   是否用于容灾
     *      usedforle       :   是否用于本地应急
     *      businessnicname :   业务网卡名
     *      brnicname       :   桥接网卡名
     *      storageid       :   存储空间id
     *      copymode        :   复制方式
     *      syncflag        :   同步主机信息标志
     *      fcname          :   光纤卡名称
     *      fcchannel       :   光纤通道号
     *      initiatorname   :   initiator名称
     *      usedfornet      :   用于网启应急
     *      usedforvm       :   用于虚拟机应急
     *      usedforfc       :   用于光纤卡应急
     *      cdpstate        :   CDP状态
     *      usedforea       :   用于自动应急
     *      eamode          :   自动应急方式
     *      heartbeattimeout:   心跳超时时间
     *      queryinterval   :   查询间隔
     *      virtualip       :   虚拟ip
     *      virtualmac      :   虚拟mac
     *      racgroupid      :   RAC组编号
     *      emergencylogonmode:   应急时副本挂载方式
     *      usedformonitor  :   是否打开监控
     *      isautotakeover  :   是否自动接管
     *      vmbootcopymode  :   虚拟机启动副本模式
     *      vncnumber       :   VNC号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHost(string localserverid, string remoteserverid, string hcode, string name, string macaddr, string ipaddr, string bootfile, string status,
            string isactive, string hdnum, string usingrhost, string rhostmac, string hbaipaddr, string isnetboot, string netdiskcapacity, string dbprotected, string ostype,
            string netbootprotocol, string netdefaultprotocol, string netbootservice, string fcwwn, string systemdir, string configdir, string vmmemory, string vmcpu, string listenport,
            string usedforrdr, string usedforle, string businessnicname, string brnicname, string storageid, string copymode, string syncflag, string fcname, string fcchannel,
            string initiatorname, string usedfornet, string usedforvm, string usedforfc, string cdpstate, string usedforea, string eamode, string heartbeattimeout,
            string queryinterval, string virtualip, string virtualmac, string racgroupid, string emergencylogonmode, string usedformonitor, string isautotakeover, string vmbootcopymode, string vncnumber);

    /*
     *  功能：
     *      更新主机网卡
     *  参数：
     *      originalserverid:   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      hcode           :   主机编码
     *      macaddr         :   mac地址
     *      devicename      :   设备名
     *      caption         :   描述
     *      gateway         :   网关
     *      maindns         :   主dns
     *      subdns          :   备用dns
     *      dhcpenabled     :   是否为dhcp
     *      isphysical      :   是否物理网卡
     *      status          :   网卡状态
     *      workmode        :   工作模式
     *      usedforvm       :   是否用于虚拟机
     *      businessnic     :   业务网卡
     *      bridgenic       :   桥接网卡
     *      virtualnetworkmode  :   虚拟网络方式
     *      virtualmac      :   虚拟mac
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool UpdateHostNic(string originalserverid, string remoteserverid, string hcode, string macaddr, string devicename, string caption, string gateway, string maindns, string subdns,
            string dhcpenabled, string isphysical, string status, string workmode, string usedforvm, string businessnic, string bridgenic, string virtualnetworkmode, string virtualmac);
    /*
     *  功能：
     *      更新主机ip地址
     *  参数：
     *      originalserverid:   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      hcode           :   主机编码
     *      macaddr         :   mac地址
     *      ip              :   ip地址
     *      netmask         :   掩码
     *      ipv6            :   ipv6地址
     *      netmaskv6       :   掩码v6
     *      workmode        :   工作模式
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool UpdateHostIpAddress(string originalserverid, string remoteserverid, string hcode, string macaddr, string ip, string netmask, string ipv6, string netmaskv6, string workmode);

    /*
     *  功能：
     *      更新主机状态
     *  参数：
     *      无
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostStatus();

    /*
     *  功能：
     *      更新主机在线状态
     *  参数：
     *      hcode           :   主机编码
     *      status          :   主机状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostOnlineStatus(string hcode, int status);

    /*
     *  功能：
     *      更新主机状态
     *  参数：
     *      hcode           :   主机编码
     *      status          :   主机状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostCanEmergencyStatus(string hcode, DbDataManager::HostCanEmergencyStatus status);

    /*
     *  功能：
     *      更新主机保护状态
     *  参数：
     *      hcode           :   主机编码
     *      status          :   主机状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostProtectedStatus(string hcode, DbDataManager::HostProtectedStatus status);

    /*
     *  功能：
     *      更新主机网启协议
     *  参数：
     *      hcode           :   主机编码
     *      protocol        :   网启协议
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostBootProtocol(string hcode, string protocol);

    /*
     *  功能：
     *      更新主机监听端口
     *  参数：
     *      hcode           :   主机编码
     *      port            :   端口号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostPort(string hcode, string port);

    /*
     *  功能：
     *      更新主机本地应急状态
     *  参数：
     *      hcode           :   主机编码
     *      status          :   本地应急状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostLocalEmergencyStatus(string hcode, string status);

    /*
     *  功能：
     *      更新主机业务网卡
     *  参数：
     *      hcode           :   主机编码
     *      nicname         :   网卡名
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostBusinessNic(string hcode, string nicname);

    /*
     *  功能：
     *      更新主机桥接网卡
     *  参数：
     *      hcode           :   主机编码
     *      nicname         :   网卡名
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostBridgeNic(string hcode, string nicname);

    /*
     *  功能：
     *      更新主机备用ip
     *  参数：
     *      hcode           :   主机编码
     *      isusedreserved  :   是否使用备机
     *      ipaddr          :   备机ip
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostReservedIp(string hcode, bool isusedreserved, string ipaddr);

    /*
     *  功能：
     *      更新主机mac地址
     *  参数：
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostMacAddress(string hcode, string mac);

    /*
     *  功能：
     *      更新主机自动应急状态
     *  参数：
     *      hcode           :   主机编码
     *      status          :   自动应急状态
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostEAStatus(string hcode, string status);

    /*
     *  功能：
     *      更新主机虚拟mac地址
     *  参数：
     *      hcode           :   主机编码
     *      virtualmac      :   虚拟mac地址
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostVirtualMac(string hcode, string virtualmac);

    /*
     *  功能：
     *      更新主机虚拟mac地址
     *  参数：
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *      virtualmac      :   虚拟mac地址
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostNicVirtualMac(string hcode, string mac, string virtualmac);

    /*
     *  功能：
     *      更新主机网卡用于虚拟机
     *  参数：
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *      usedforvm       :   用于虚拟机应急
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostNicUsedForVm(string hcode, string mac, bool usedforvm);

    /*
     *  功能：
     *      更新主机网卡的业务网卡
     *  参数：
     *      hcode           :   主机编码
     *      nicname         :   网卡
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostNicBussinessNicName(string hcode, string nicname);

    /*
     *  功能：
     *      更新主机网卡的业务网卡
     *  参数：
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *      nicname         :   网卡
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostNicBussinessNicName(string hcode, string mac, string nicname);

    /*
     *  功能：
     *      更新主机网卡的桥接网卡
     *  参数：
     *      hcode           :   主机编码
     *      nicname         :   网卡
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostNicBridgeNicName(string hcode, string nicname);

    /*
     *  功能：
     *      更新主机网卡的桥接网卡
     *  参数：
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *      nicname         :   网卡
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostNicBridgeNicName(string hcode, string mac, string nicname);

    /*
     *  功能：
     *      更新主机虚拟网络方式
     *  参数：
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *      mode            :   虚拟网络方式
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostVirtualNetworkMode(string hcode, string mac, VirtualNetworkMode mode);


    /*
     *  功能：
     *      更新主机复制方式
     *  参数：
     *      hcode           :   主机编码
     *      copymode        :   mac地址
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostCopyMode(string hcode, DbDataManager::HostCopyMode copymode);

    /*
     *  功能：
     *      更新主机光纤名
     *  参数：
     *      hcode           :   主机编码
     *      fcname          :   光纤名
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostFc(string hcode, string fcname);

    /*
     *  功能：
     *      更新主机光纤名
     *  参数：
     *      hcode           :   主机编码
     *      fcname          :   光纤名
     *      wwn             :   wwn
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostFc(string hcode, string fcname, string wwn);

    /*
     *  功能：
     *      更新主机虚拟机启动副本模式
     *  参数：
     *      hcode           :   主机编码
     *      mode            :   启动副本模式
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostVmBootCopyMode(string hcode, DbDataManager::VmBootCopy mode);

    /*
     *  功能：
     *      更新主机的vnc号
     *  参数：
     *      hcode           :   主机编码
     *      vncnumber       :   vnc号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostVncNumber(string hcode, int vncnumber);

    /*
     *  功能：
     *      通知主机打开或关闭监控
     *  参数：
     *      hcode           :   主机编码
     *      isopen          :   是否打开
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateHostUsedForMonitor(string hcode, bool isopen);

    /*
     *  功能：
     *      清除主机网启应急功能
     *  参数：
     *      无
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool ClearHostNetEmergency();

    /*
     *  功能：
     *      清除主机虚拟机应急功能
     *  参数：
     *      无
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool ClearHostVmEmergency();

    /*
     *  功能：
     *      清除主机光纤卡应急功能
     *  参数：
     *      无
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool ClearHostFcEmergency();

    /*
     *  功能：
     *      清除主机数据库保护功能
     *  参数：
     *      无
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool ClearHostDbProtected();

    /*
     *  功能：
     *      获取主机结果集
     *  参数：
     *      无
     *  返回：
     *      主机结果集
     */
    virtual sql::ResultSet * GetHostResultSet();

    /*
     *  功能：
     *      获取主机结果集
     *  参数：
     *      nicname         :   网卡名
     *  返回：
     *      主机结果集
     */
    virtual sql::ResultSet * GetHostResultSet(string nicname);

    /*
     *  功能：
     *      获取主机结果集
     *  参数：
     *      searchvalue     :   查找值
     *      searchtype      :   查找类型
     *  返回：
     *      主机结果集
     */
    virtual sql::ResultSet * GetHostResultSet(string searchvalue, DbDataManager::HostResultSetRetrieveType type);

    /*
     *  功能：
     *      获取主机个数
     *  参数：
     *      无
     *  返回：
     *      主机个数
     */
    virtual int GetHostCount();

    /*
     *  功能：
     *      获取主机个数
     *  参数：
     *      type            :   主机个数获取类型
     *  返回：
     *      主机个数
     */
    virtual int GetHostCount(DbDataManager::HostCountRetrieveType type);

    /*
     *  功能：
     *      获取主机个数
     *  参数：
     *      type            :   主机个数获取类型
     *      excepthcode     :   排除的主机编码
     *  返回：
     *      主机个数
     */
    virtual int GetHostCount(DbDataManager::HostCountRetrieveType type, string excepthcode);


    /*
     *  功能：
     *      获取主机操作系统类型
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      操作系统类型
     */
    virtual DbDataManager::HostOSType GetHostOSType(string hcode);

    /*
     *  功能：
     *      获取主机ip地址
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机的ip地址
     */
    virtual string GetHostIp(string hcode);

    virtual string GetHostMirrorMode(string hcode);
    virtual string GetHostMirrorState(string hcode);

    /*
     *  功能：
     *      获取主机端口
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机的监听端口
     */
    virtual string GetHostPort(string hcode);

    /*
     *  功能：
     *      根据主机编码获取主机名称
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机名称
     */
    virtual string GetHostName(string hcode);

    /*
     *  功能：
     *      根据主机编码获取wwn
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      wwn
     */
    virtual string GetHostWWN(string hcode);

    /*
     *  功能：
     *      根据主机编码获取启动协议
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      启动协议
     */
    virtual string GetHostBootProtocol(string hcode);

    /*
     *  功能：
     *      获取主机默认网启协议
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      默认网启协议
     */
    virtual string GetHostNetDefaultProtocol(string hcode);

    /*
     *  功能：
     *      根据主机编码获取虚拟机内存
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机的虚拟机内存
     */
    virtual int GetHostVmMemory(string hcode);

    /*
     *  功能：
     *      根据主机编码获取虚拟机CPU
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机的虚拟机CPU
     */
    virtual int GetHostVmCPU(string hcode);

    /*
     *  功能：
     *      获取活动虚拟机内存总和
     *  参数：
     *      无
     *  返回：
     *      活动虚拟机内存总和
     */
    virtual int GetActiveVmMemory();

    /*
     *  功能：
     *      获取活动虚拟机内存CPU
     *  参数：
     *      无
     *  返回：
     *      活动虚拟机内存CPU
     */
    virtual int GetActiveVmCPU();

    /*
     *  功能：
     *      获取主机mac地址
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机mac地址
     */
    virtual string GetHostMac(string hcode);

    /*
     *  功能：
     *      获取主机系统目录
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机系统目录
     */
    virtual string GetHostSystemDir(string hcode);

    /*
     *  功能：
     *      获取主机配置目录
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机配置目录
     */
    virtual string GetHostConfigDir(string hcode);

    /*
     *  功能：
     *      获取主机保护状态
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机保护状态
     */
    virtual DbDataManager::HostProtectedStatus GetHostProtectedStatus(string hcode);

    /*
     *  功能：
     *      获取主机业务网卡名
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机业务网卡名
     */
    virtual string GetHostBusinessNicName(string hcode);

    /*
     *  功能：
     *      获取主机桥接网卡名
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机桥接网卡名
     */
    virtual string GetHostBridgeNicName(string hcode);

    /*
     *  功能：
     *      获取主机操作系统类型编码
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机操作系统类型编码
     */
    virtual string GetHostOSTypeCode(string hcode);

    /*
     *  功能：
     *      获取主机字段值
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机字段值
     */
    virtual map<string, string> GetHostFieldValue(string hcode);

    /*
     *  功能：
     *      获取主机应急副本结果集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机应急副本结果集
     */
    virtual sql::ResultSet * GetHostEmergencyCopyResultSet(string hcode);

    /*
     *  功能：
     *      获取主机存储路径
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机存储路径
     */
    virtual string GetHostStoragePath(string hcode);

    /*
     *  功能：
     *      获取主机复制方式
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机复制方式
     */
    virtual DbDataManager::HostCopyMode GetHostCopyMode(string hcode);

    /*
     *  功能：
     *      获取主机备机ip
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机备机ip
     */
    virtual string GetHostReservedIp(string hcode);

    /*
     *  功能：
     *      获取主机FC名称
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机FC名称
     */
    virtual string GetHostFCName(string hcode);

    /*
     *  功能：
     *      获取主机FC通道号
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机FC通道号
     */
    virtual string GetHostFCChannel(string hcode);

    /*
     *  功能：
     *      获取主机initiator名称
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      主机initiator名称
     */
    virtual string GetHostInitiatorName(string hcode);

    /*
     *  功能：
     *      获取主机网卡结果集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      网卡结果集
     */
    virtual sql::ResultSet * GetHostNetworkAdapterResultSet(string hcode);

    /*
     *  功能：
     *      获取主机网卡个数
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      网卡个数
     */
    virtual int GetHostNetworkAdapterCount(string hcode);

    /*
     *  功能：
     *      获取主机ip地址结果集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      IP地址结果集
     */
    virtual sql::ResultSet * GetHostIpAddressResultSet(string hcode);

    /*
     *  功能：
     *      获取主机ip地址结果集
     *  参数：
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *  返回：
     *      IP地址结果集
     */
    virtual sql::ResultSet * GetHostIpAddressResultSet(string hcode, string mac);

    /*
     *  功能：
     *      获取主机自动应急模式
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      主机自动应急模式
     */
    virtual DbDataManager::HostEAMode GetHostEAMode(string hcode);

    /*
     *  功能：
     *      获取主机心跳超时时间
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      主机心跳超时时间
     */
    virtual int GetHostHeartbeatTimeout(string hcode);

    /*
     *  功能：
     *      获取主机虚ip
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      主机虚ip
     */
    virtual string GetHostVirtualIp(string hcode);

    /*
     *  功能：
     *      获取主机虚mac
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      主机虚mac
     */
    virtual string GetHostVirtualMac(string hcode);

    /*
     *  功能：
     *      获取主机通讯网卡的虚拟mac
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      主机通讯网卡的虚拟mac
     */
    virtual string GetHostCommunicationNicVirtualMac(string hcode);

    /*
     *  功能：
     *      获取主机通讯网卡的网络模式
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      主机通讯网卡的网络模式
     */
    virtual string GetHostCommunicationNicNetworkMode(string hcode);

    /*
     *  功能：
     *      获取主机rac组编号
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      rac组编号
     */
    virtual string GetHostRacGroupId(string hcode);

    /*
     *  功能：
     *      获取主机应急时副本挂载方式
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      应急时副本挂载方式
     */
    virtual DbDataManager::EmergencyLogonMode GetHostEmergencyLogonMode(string hcode);

    /*
     *  功能：
     *      获取主机虚拟机启动副本方式
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      虚拟机启动副本方式
     */
    virtual DbDataManager::VmBootCopy GetHostVmBootCopyMode(string hcode);

    /*
     *  功能：
     *      获取主机vnc号
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      vnc号
     */
    virtual int GetHostVncNumber(string hcode);

    /*
     *  功能：
     *      将主机操作系统代码转换为操作系统类型
     *  参数
     *      oscode           :   操作系统代码
     *  返回：
     *      操作系统类型
     */
    virtual DbDataManager::HostOSType ConvertHostOsCodeToOsType(string oscode);

    /*
     *  功能：
     *      主机编码是否存在
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsHostExist(string hcode);

    /*
     *  功能：
     *      主机名称是否存在
     *  参数
     *      name            :   主机名称
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsHostNameExist(string name);

    /*
     *  功能：
     *      主机ip是否存在
     *  参数
     *      ipaddr          :   主机ip
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsHostIpExist(string ipaddr);

    /*
     *  功能：
     *      主机是否为网启
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果为网启返回true，否则返回false
     */
    virtual bool IsHostEmergency(string hcode);

    /*
     *  功能：
     *      检查该网卡是否用于虚拟机启动
     *  参数
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostNetworkAdapterForVm(string hcode, string mac);

    /*
     *  功能：
     *      检查该网卡是否存在
     *  参数
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostNetworkAdapterExist(string hcode, string mac);

    /*
     *  功能：
     *      检查该网卡对应的ip是否存在
     *  参数
     *      hcode           :   主机编码
     *      mac             :   mac地址
     *      ip              :   ip地址
     *      mask            :   子网掩码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostNetworkAdapterIpExist(string hcode, string mac, string ip, string mask);

    /*
     *  功能：
     *      检查主机本地应急功能打开
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostLocalEmergencyOpen(string hcode);

    /*
     *  功能：
     *      检查主机网启应急功能打开
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostNetEmergencyOpen(string hcode);

    /*
     *  功能：
     *      检查主机虚拟机应急功能打开
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostVmEmergencyOpen(string hcode);

    /*
     *  功能：
     *      检查主机光纤卡应急功能打开
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostFcEmergencyOpen(string hcode);

    /*
     *  功能：
     *      检查主机是否仅备份
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostOnlyForBackup(string hcode);

    /*
     *  功能：
     *      检查主机自动应急功能是否打开
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostEAOpen(string hcode);

    virtual bool IsHostMonitorOpen(string hcode);

    /*
     *  功能：
     *      检查主机是否有业务任务运行
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsHostHasBusinessTaskRunning(string hcode);

    /*
     *  功能：
     *      检查主机硬件应急功能是否打开
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostHWEmergencyOpen(string hcode);


    /*
     *  功能：
     *      检查主机是否为RAC组的首选主机
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostRacGroupMaster(string hcode);

    /*
     *  功能：
     *      检查主机是否已经做过初始复制
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostInitializeCopyed(string hcode);

    /*
     *  功能：
     *      检查主机是否已经做过初始复制
     *  参数
     *      hcode           :   主机编码
     *      lmountpoint     :   本地挂载点
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostInitializeCopyed(string hcode, string lmountpoint);

    /*
     *  功能：
     *      检查主机远程容灾功能是否打开
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsHostRDROpen(string hcode);

    /*
     *  功能：
     *      检查主机是否有CDP保护
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsHostHasCDPProtection(string hcode);

    /*
     *  功能：
     *      检查虚拟mac地址是否已存在
     *  参数
     *      mac             :   mac地址
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsVirtualMacExists(string mac);

    //=============================== 数据库相关 ===============================//

    /*
     *  功能：
     *      删除数据库策略明细
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteDbStrategyDetail(string hcode);

    /*
     *  功能：
     *      删除数据库任务计划
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteDbSchedule(string hcode);

    /*
     *  功能：
     *      删除数据库任务
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteDbTask(string hcode);

    /*
     *  功能：
     *      更新数据库保护状态
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateDbProtectedStatus(string hcode, int status);

    /*
     *  功能：
     *      获取数据库保护数
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      数据库保护数
     */
    virtual int GetDbCount(string hcode);

    /*
     *  功能：
     *      获取数据库保护数
     *  参数：
     *      无
     *  返回：
     *      数据库保护数
     */
    virtual int GetDbCount();

    /*
     *  功能：
     *      获取主机数据库保护状态
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      数据库保护状态
     */
    virtual int GetDbProtectedStatus(string hcode);

    /*
     *  功能：
     *      主机是否进行了数据库保护
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsDbProtected(string hcode);

    /*
     *  功能：
     *      是否有自动应急功能打开的主机
     *  参数：
     *      无
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool HasEAOpenedHosts();

    /*
     *  功能：
     *      是否有异地容灾功能打开的主机
     *  参数：
     *      无
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool HasRdrOpenedHosts();


    //================================ 计划相关 ================================//

    /*
     *  功能：
     *      增加计划
     *  参数：
     *      localserverid   :   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      schedulename    :   计划名称
     *      commandcode     :   命令字
     *      parameter       :   参数
     *      hcode           :   主机编码
     *      tasktype        :   任务类型
     *      exectype        :   执行方式
     *      execinterval    :   执行周期
     *      intervalunit    :   执行单位
     *      starttime       :   开始时间
     *      nexttime        :   下次执行时间
     *      status          :   状态
     *      lasttime        :   上次时间
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool AddSchedule(string localserverid, string remoteserverid, string schedulename, string commandcode, string parameter, string hcode, string tasktype, string exectype,
            string execinterval, string intervalunit, string starttime, string nexttime, string status, string lasttime);

    /*
     *  功能：
     *      删除指定主机编码下的任务计划
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      删除成功返回true，否则返回false
     */
    virtual bool DeleteSchedule(string hcode);

    /*
     *  功能：
     *      增加计划
     *  参数：
     *      localserverid   :   本地服务器id
     *      remoteserverid  :   远程服务器id
     *      schedulename    :   计划名称
     *      commandcode     :   命令字
     *      parameter       :   参数
     *      hcode           :   主机编码
     *      tasktype        :   任务类型
     *      exectype        :   执行方式
     *      execinterval    :   执行周期
     *      intervalunit    :   执行单位
     *      starttime       :   开始时间
     *      nexttime        :   下次执行时间
     *      status          :   状态
     *      lasttime        :   上次时间
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool UpdateSchedule(string localserverid, string remoteserverid, string schedulename, string commandcode, string parameter, string hcode, string tasktype, string exectype,
            string execinterval, string intervalunit, string starttime, string nexttime, string status, string lasttime);

    /*
     *  功能：
     *      获取调度计划结果集
     *  参数：
     *      无
     *  返回：
     *      调度计划结果集
     */
    virtual sql::ResultSet * GetScheduleResultSet();

    /*
     *  功能：
     *      获取调度计划结果集
     *  参数：
     *      serverid        :   主机编码
     *  返回：
     *      调度计划结果集
     */
    virtual sql::ResultSet * GetScheduleResultSet(string serverid);

    /*
     *  功能：
     *      获取调度计划字段值
     *  参数：
     *      scheduleid      :   计划id
     *  返回：
     *      调度计划字段值
     */
    virtual map<string, string> GetScheduleFieldValue(string scheduleid);

    /*
     *  功能：
     *      获取调度计划字段值
     *  参数：
     *      serverid        :   计划id
     *      name            :   计划名称
     *  返回：
     *      调度计划字段值
     */
    virtual map<string, string> GetScheduleFieldValue(string serverid, string schedulename);

    /*
     *  功能：
     *      将符合条件任务计划压入任务表
     *  参数：
     *      sqlconn         :   数据库连接对象
     *  返回：
     *      无
     */
    virtual void MoveScheduleToTask(sql::Connection *sqlconn);

    /*
     *  功能：
     *      检查计划是否存在
     *  参数：
     *      scheduleid      :   计划id
     *  返回：
     *      如果存在返回true，否则返回false
     */
    virtual bool IsScheduleExist(string scheduleid);

    /*
     *  功能：
     *      检查计划是否存在
     *  参数：
     *      serverid        :   服务器id
     *      name            :   计划名称
     *  返回：
     *      如果存在返回true，否则返回false
     */
    virtual bool IsScheduleExist(string serverid, string name);

    //================================ 网络相关 ================================//

    /*
     *  功能：
     *      增加子网
     *  参数：
     *      subnet          :   子网
     *      mask            :   子网掩码
     *      rangelow        :   开始地址
     *      rangehigh       :   结束地址
     *      serverip        :   服务器ip
     *  返回：
     *      如果成功返回true，否则返回false
     */
    virtual bool AddSubnet(string subnet, string mask, string rangelow, string rangehigh, string serverip);

    /*
     *  功能：
     *      更新网卡类型
     *  参数：
     *      nicname         :   网卡名
     *      nictype         :   网卡类型
     *  返回：
     *      如果成功返回true，否则返回false
     */
    virtual bool UpdateNicType(string nicname, DbDataManager::NicType nictype);

    /*
     *  功能：
     *      获取网卡信息结果集
     *  参数：
     *      无
     *  返回：
     *      网卡信息结果集
     */
    virtual sql::ResultSet * GetNetworkResultSet();

    /*
     *  功能：
     *      获取网卡参数
     *  参数：
     *      nicname         :   网卡名称
     *  返回：
     *      网卡参数
     */
    virtual map<string, string> GetNetworkParameter(string nicname);

    //    /*
    //     *  功能：
    //     *      获取业务ip地址
    //     *  参数：
    //     *      无
    //     *  返回：
    //     *      业务ip地址
    //     */
    //    virtual string GetBusinessIpAddr();
    //
    //    /*
    //     *  功能：
    //     *      获取管理ip地址
    //     *  参数：
    //     *      无
    //     *  返回：
    //     *      管理ip地址
    //     */
    //    virtual string GetManageIpAddr();

    /*
     *  功能：
     *      获取子网结果集
     *  参数：
     *      无
     *  返回：
     *      子网结果集
     */
    virtual sql::ResultSet * GetSubnetResultSet();

    /*
     *  功能：
     *      获取路由结果集
     *  参数：
     *      无
     *  返回：
     *      路由结果集
     */
    virtual sql::ResultSet * GetRouteResultSet();


    /*
     *  功能：
     *      获取网卡ip
     *  参数：
     *      nicname         :   网卡名称
     *  返回：
     *      网卡ip
     */
    virtual string GetNicIpAddr(string nicname);

    /*
     *  功能：
     *      网卡是否存在
     *  参数：
     *      nicname         :   网卡名
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsNicExist(string nicname);

    /*
     *  功能：
     *      服务器ip是否存在
     *  参数：
     *      ipaddr          :   ip地址
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsServerIpExist(string ipaddr);

    /*
     *  功能：
     *      检查子网是否存在
     *  参数：
     *      subnet          :   子网
     *      mask            :   子网掩码
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsSubnetExist(string subnet, string mask);

    /*
     *  功能：
     *      网卡是否为桥接
     *  参数：
     *      nicname         :   网卡名
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool IsNicBridged(string nicname);

    //================================ 远程服务器相关 ============================//

    /*
     *  功能：
     *      更新原始服务器编号
     *  参数：
     *      newserverid         :   新的服务器编号
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateOriginalServerId(string newserverid);

    /*
     *  功能：
     *      获取服务器列表
     *  参数：
     *      type            :   远程服务器类型
     *  返回：
     *      服务器列表
     */
    virtual sql::ResultSet * GetServerList(RemoteServerType type);

    /*
     *  功能：
     *      获取指定serverid的字段信息
     *  参数：
     *      serverid            :   服务器id
     *      servertype          :   远程服务器类型
     *  返回：
     *      字段列表
     */
    virtual map<string, string> GetServerFieldValue(string serverid, DbDataManager::RemoteServerType servertype);

    /*
     *  功能：
     *      获取指定serverid和类型的ip
     *  参数：
     *      serverid            :   服务器id
     *      servertype          :   远程服务器类型
     *  返回：
     *      服务器ip
     */
    virtual string GetServerIp(string serverid, DbDataManager::RemoteServerType servertype);

    /*
     *  功能：
     *      获取指定serverid和类型的端口
     *  参数：
     *      serverid            :   服务器id
     *      servertype          :   远程服务器类型
     *  返回：
     *      服务器端口
     */
    virtual string GetServerPort(string serverid, DbDataManager::RemoteServerType servertype);

    /*
     *  功能：
     *      获取本机服务器id
     *  参数：
     *      无
     *  返回：
     *      本机服务器id
     */
    virtual string GetLocalServerId();

    /*
     *  功能：
     *      检查指定serverid的服务器是否存在
     *  参数：
     *      serverid         :   服务器id
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsServerExist(string serverid);

    /*
     *  功能：
     *      检查指定serverid和类型的服务器是否存在
     *  参数：
     *      serverid        :   服务器id
     *      servertype      :   服务器类型
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsServerExist(string serverid, DbDataManager::RemoteServerType servertype);

    /*
     *  功能：
     *      检查指定类型的服务器是否存在
     *  参数：
     *      servertype      :   服务器类型
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsServerExist(DbDataManager::RemoteServerType servertype);


    /*
     *  功能：
     *      检查设备编号是否已经存在
     *  参数：
     *      serverid        :   服务器id
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsServerIdExistInLocal(string serverid);

    //================================ 报警相关 ================================//

    /*
     *  功能：
     *      增加报警
     *  参数：
     *      hcode           :   主机编码
     *      event           :   报警事件
     *      content         :   报警内容
     *      level           :   报警级别
     *      starttime       :   开始时间
     *      endtime         :   结束时间
     *  返回：
     *      操作成功返回ture，否则返回false
     */
    virtual bool AddAlarm(string hcode, string event, string content, DbDataManager::AlarmLevel level, string starttime, string endtime);

    /*
     *  功能：
     *      更新报警
     *  参数：
     *      alarmid         :   报警id
     *      state           :   状态
     *  返回：
     *      操作成功返回ture，否则返回false
     */
    virtual bool UpdateAlarm(string alarmid, int reportstate);

    /*
     *  功能：
     *      更新报警
     *  参数：
     *      reportmode      :   上报类型
     *      alarmid         :   报警id
     *      state           :   状态
     *  返回：
     *      操作成功返回ture，否则返回false
     */
    virtual bool UpdateAlarmReportState(DbDataManager::AlarmReportMode reportmode, string alarmid, DbDataManager::AlarmReportState reportstate);

    /*
     *  功能：
     *      获取报警记录
     *  参数：
     *      无
     *  返回：
     *      报警结果集
     */
    virtual sql::ResultSet * GetAlarmResultSet();

    /*
     *  功能：
     *      获取报警接收人结果集
     *  参数：
     *      无
     *  返回：
     *      报警接收人结果集
     */
    virtual sql::ResultSet * GetAlarmReceiverResultSet();

    /*
     *  功能：
     *      获取报警类型
     *  参数：
     *      无
     *  返回：
     *      报警类型
     */
    virtual DbDataManager::AlarmMode GetAlarmMode();

    /*
     *  功能：
     *      检查是否有未上报的报警
     *  参数：
     *      无
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool HasUnreportedAlarm();

    //================================ 存储空间相关 =============================//

    /*
     *  功能：
     *      更新存储空间
     *  参数：
     *      无
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStorageSpace();

    /*
     *  功能：
     *      更新存储空间设备名称
     *  参数：
     *      storageid       :   存储空间id
     *      devicename      :   设备名称
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStorageDeviceName(string storageid, string devicename);

    /*
     *  功能：
     *      更新存储空间文件系统
     *  参数：
     *      storageid       :   存储空间id
     *      filesystem      :   文件系统
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStorageFileSystem(string storageid, string filesystem);

    /*
     *  功能：
     *      更新存储空间挂载点
     *  参数：
     *      storageid       :   存储空间id
     *      path            :   路径
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStorageMountPoint(string storageid, string path);

    /*
     *  功能：
     *      更新存储空间设备名词
     *  参数：
     *      storageid       :   存储空间id
     *      cleaned         :   是否清理过的标志
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStorageCleanState(string storageid, bool cleaned);

    /*
     *  功能：
     *      更新存储空间清理启动值
     *  参数：
     *      storageid       :   存储空间id
     *      limit           :   启动门限值
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateStorageCleanLimit(string storageid, int limit);

    /*
     *  功能：
     *      获取存储空间字段值
     *  参数：
     *      storageid       :   存储空间id
     *  返回：
     *      存储空间字段值
     */
    virtual map<string, string> GetStorageFieldValue(string storageid);

    /*
     *  功能：
     *      根据存储空间id获取已用存储空间
     *  参数：
     *      storageid      :   存储空间id
     *  返回：
     *      已用存储空间
     */
    virtual string GetUsedStorageSpace(string storageid);

    /*
     *  功能：
     *      获取存储空间结果集
     *  参数：
     *      无
     *  返回：
     *      存储空间结果集
     */
    virtual sql::ResultSet * GetStorageSpaceResultSet();

    /*
     *  功能：
     *      检查存储空间是否存在
     *  参数：
     *      storageid       :   存储空间id
     *  返回：
     *      如果存在返回ture，否则返回false
     */
    virtual bool IsStorageExist(string storageid);

    /*
     *  功能：
     *      检查存储空间是否有CDP保护
     *  参数：
     *      storageid       :   存储空间id
     *  返回：
     *      如果有返回ture，否则返回false
     */
    virtual bool IsStorageHasCDP(string storageid);

    /*
     *  功能：
     *      检查存储空间是否需要做定时快照
     *  参数：
     *      无
     *  返回：
     *      如果有返回ture，否则返回false
     */
    virtual bool IsStorageNeedTimingSnapshot();

    //================================ 光纤卡相关 ================================//

    virtual bool ClearFCAdapter();
    /*
     *  功能：
     *      增加光纤卡记录
     *  参数：
     *      fcname          :   光纤名称
     *      wwn             :   wwn
     *      desc            :   描述
     *  返回：
     *      成功返回true，否则返回false
     */
    virtual bool AddFCAdapter(string fcname, string wwn, string desc);

    /*
     *  功能：
     *      获取光纤卡结果集
     *  参数：
     *      无
     *  返回：
     *      光纤卡结果集
     */
    virtual sql::ResultSet * GetFCResultSet();

    /*
     *  功能：
     *      获取光纤卡记录字段值
     *  参数：
     *      fcname          :   光纤卡名称
     *  返回：
     *      光纤卡记录字段值
     */
    virtual map<string, string> GetFCFieldValue(string fcname);

    /*
     *  功能：
     *      获取指定fc的WWN
     *  参数：
     *      fcname          :   光纤卡名称
     *  返回：
     *      fc的WWN
     */
    virtual string GetFCWWN(string fcname);

    /*
     *  功能：
     *      检查光纤卡是否存在
     *  参数：
     *      fcname          :   光纤卡名称
     *  返回：
     *      如果存在返回ture，否则返回false
     */
    virtual bool IsFCNameExist(string fcname);

    /*
     *  功能：
     *      检查光纤卡是否存在
     *  参数：
     *      wwn             :   光纤卡wwn
     *  返回：
     *      如果存在返回ture，否则返回false
     */
    virtual bool IsFCWWNExist(string wwn);

    //================================ 虚拟服务器相关 ================================//

    /*
     *  功能：
     *      获取虚拟主机结果集
     *  参数：
     *      无
     *  返回：
     *      虚拟主机结果集
     */
    virtual sql::ResultSet * GetVMHostResultSet();

    /*
     *  功能：
     *      检查虚拟主机对应的虚拟服务器id
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      虚拟服务器id
     */
    virtual string GetVMServerId(string hcode);

    /*
     *  功能：
     *      检查虚拟主机是否可用
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      如果可用返回ture，否则返回false
     */
    virtual bool IsVMHostUsed(string hcode);

    //================================ CDP相关 ================================//

    /*
     *  功能：
     *      增加CDP配置记录
     *  参数：
     *      cdpnumber       :   cdp个数
     *      keeptime        :   快照保留时间
     *      keepunit        :   时间单位
     *  返回：
     *      设置的CDP个数
     */
    virtual bool AddCDPConfig(int cdpnumber, int keeptime, int keepunit);

    /*
     *  功能：
     *      获取设置的CDP个数
     *  参数：
     *      无
     *  返回：
     *      设置的CDP个数
     */
    virtual int GetCDPNumber();

    /*
     *  功能：
     *      获取设置的CDP连续快照时间
     *  参数：
     *      无
     *  返回：
     *      设置的CDP连续快照时间
     */
    virtual int GetCDPKeepTime();


    /*
     *  功能：
     *      获取设置的CDP连续快照时间单位
     *  参数：
     *      无
     *  返回：
     *      设置的CDP连续快照时间单位
     */
    virtual int GetCDPKeepUnit();

    /*
     *  功能：
     *      获取cdp连续恢复点时间间隔
     *  参数：
     *      无
     *  返回：
     *      cdp连续恢复点时间间隔
     */
    virtual int GetCDPCheckpointInterval();

    /*
     *  功能：
     *      检查CDP配置是否存在
     *  参数：
     *      无
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsCDPConfigExist();

    /*
     *  功能：
     *      更新cdp连续快照保留时间
     *  参数：
     *      keeptime        :   保留时间
     *      keepunit        :   时间单位
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCDPSnapshotKeepTime(int keeptime, int keeptimeunit);

    /*
     *  功能：
     *      更新cdp个数
     *  参数：
     *      number          :   cdp个数
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCDPNumber(int number);

    /*
     *  功能：
     *      更新cdp连续快照保留时间
     *  参数：
     *      number          :   cdp个数
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateCDPCheckpointInterval(int interval);

    //================================ 心跳相关 ================================//

    /*
     *  功能：
     *      获取主机心跳监测项结果集
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      成功返回主机心跳监测项结果集，否则返回NULL
     */
    virtual sql::ResultSet * GetHostHeartbeatMonitorItemResultSet(string hcode);

    //================================ 硬件应急相关 ================================//



    /*
     *  功能：
     *      获取硬件应急配置参数
     *  参数：
     *      无
     *  返回：
     *      配置参数列表
     */
    virtual map<string, string> GetHWEmergencyConfig();

    /*
     *  功能：
     *      获取按键绑定的主机编码
     *  参数：
     *      keyid           :   按键编号
     *  返回：
     *      如果成功返回主机编码，否则返回空
     */
    virtual string GetHCodeBindByKeyId(string keyid);

    /*
     *  功能：
     *      获取主机编码绑定的按键编号
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      如果成功返回按键编号，否则返回空
     */
    virtual string GetKeyIdBindByHCode(string hcode);

    /*
     *  功能：
     *      是否有硬件应急配置参数
     *  参数：
     *      无
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool HasHWEmergencyConfig();

    /*
     *  功能：
     *      检查指定键编号是否用于主机硬件应急
     *  参数：
     *      keyid           :   按键编号
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsKeyUsedForHost(string keyid);

    /*
     *  功能：
     *      是否有硬件应急绑定
     *  参数：
     *
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool IsHWEmergencyBind();

    //================================ 命令相关 ================================//

    /*
     *  功能：
     *      获取命令字描述信息
     *  参数：
     *      cmdcode         :   命令字描述信息
     *  返回：
     *      命令字描述信息
     */
    virtual string GetCommandDescription(string cmdcode);

    //================================ 其他相关 ================================//

    /*
     *  功能：
     *      更新license文件
     *  参数：
     *      license         :   许可文件
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateLicenseName(string license);

    /*
     *  功能：
     *      更新license报警信息
     *  参数：
     *      alarm           :   报警信息
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateLicenseAlarm(string alarm);

    /*
     *  功能：
     *      更新license数
     *  参数：
     *      host            :   主机数
     *      db              :   数据库保护数
     *      snapshot        :   快照数
     *      net             :   网启数
     *      vm              :   虚拟机数
     *      fc              :   光纤卡数
     *      cdp             :   CDP数
     *      cdpkeeptime     :   CDP连续快照时间
     *      cdpkeepunit     :   CDP连续快照时间单位
     *      eanumber        :   自动应急个数
     *      expireddate     :   许可到期日
     *  返回：
     *      更新成功返回true，否则返回false
     */
    virtual bool UpdateLicenseNumber(int host, int db, int snapshot, int net, int vm, int fc, int cdp, int cdpkeeptime, int cdpkeepunit, int eanumber, string expireddate);

    /*
     *  功能：
     *      获取Agent参数
     *  参数：
     *      nicname         :   网卡名
     *  返回：
     *      Agent参数
     */
    virtual map<string, string> GetAgentParameter(string nicname);

    /*
     *  功能：
     *      获取Bes类型
     *  参数：
     *      无
     *  返回：
     *      Bes类型
     */
    virtual DbDataManager::BesType GetBesType();

    /*
     *  功能：
     *      获取远程服务器ip
     *  参数：
     *      无
     *  返回：
     *      远程服务器ip
     */
    virtual string GetRemoteServerIp();

    /*
     *  功能：
     *      获取Bes总的虚拟机内存
     *  参数：
     *      无
     *  返回：
     *      Bes总的虚拟机内存
     */
    virtual int GetBesTotalVmMemory();

    /*
     *  功能：
     *      获取Bes总的虚拟机cpu
     *  参数：
     *      无
     *  返回：
     *      Bes总的虚拟机cpu
     */
    virtual int GetBesTotalVmCPU();

    /*
     *  功能：
     *      获取授权最大主机数
     *  参数：
     *      无
     *  返回：
     *      授权最大主机数
     */
    virtual int GetMaxHost();

    /*
     *  功能：
     *      设置数据库参数
     *  参数：
     *      server          :   服务器
     *      user            :   用户名
     *      password        :   密码
     *      dbname          :   数据库名
     *  返回：
     *      无
     */
    virtual void SetParameter(string server, string user, string password, string dbname);

    /*
     *  功能：
     *      检查数据库是否OK
     *  参数：
     *      无
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool CheckDatabaseOK();

    /*
     *  功能：
     *      检查数据库是否OK
     *  参数：
     *      server          :   服务器
     *      user            :   用户名
     *      password        :   密码
     *      dbname          :   数据库名
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool CheckDatabaseOK(string server, string user, string password, string dbname);

    /*
     *  功能：
     *      获取用户名
     *  参数：
     *      无
     *  返回：
     *      用户名
     */
    virtual string User();

    /*
     *  功能：
     *      获取密码
     *  参数：
     *      无
     *  返回：
     *      密码
     */
    virtual string Password();

    /*
     *  功能：
     *      获取数据库名
     *  参数：
     *      无
     *  返回：
     *      数据库名
     */
    virtual string DbName();

    /*
     *  功能：
     *      导出数据库
     *  参数：
     *      无
     *  返回：
     *      导出成功返回true，否则返回false
     */
    virtual bool Export();

    /*
     *  功能：
     *      导出数据库
     *  参数：
     *      path            ：  导出到指定目录
     *  返回：
     *      导出成功返回true，否则返回false
     */
    virtual bool Export(string path);

    /*
     *  功能：
     *      导出数据库
     *  参数：
     *      user            :   用户名
     *      password        :   密码
     *      path            ：  导出到指定目录
     *  返回：
     *      导出成功返回true，否则返回false
     */
    virtual bool Export(string user, string password, string path);

    /*
     *  功能：
     *      获取数据库名
     *  参数：
     *      无
     *  返回：
     *      修复成功返回true，否则返回false
     */
    virtual bool Repair();

    /*
     *  功能：
     *      获取数据库名
     *  参数：
     *      user            :   用户名
     *      password        :   密码
     *  返回：
     *      修复成功返回true，否则返回false
     */
    virtual bool Repair(string user, string password);

    /*
     *  功能：
     *      重置gui的admin密码
     *  参数：
     *      无
     *  返回：
     *      重置成功返回true，否则返回false
     */
    virtual bool ResetAdminPassword();

    /*
     *  功能：
     *      备份本地数据库
     *  参数
     *      无
     *  返回：
     *      无
     */
    virtual void BackupDatabase();

    /*
     *  功能：
     *      销毁结果集
     *  参数
     *      rs              :   结果集对象
     *  返回：
     *      无
     */
    virtual void DestroyResultSet(sql::ResultSet * rs);
    virtual bool AddSelfCheckLog(string content, BesError::SelfCheckResult state);


    /*
     *  功能：
     *      获取报警条件
     *  参数
     *      hcode               :   主机编码
     *  返回：
     *
     */
    virtual map<string, string> GetAlarmRule(string hcode);
	virtual sql::ResultSet * GetMonitorPort(string hcode);
	virtual sql::ResultSet * GetMonitorService(string hcode);
	virtual sql::ResultSet * GetMonitorProcess(string hcode);
    virtual map<string, string> GetSoftOffParam(string hcode);
	virtual string GetHostCreateTime(string hcode);
	virtual string GetServerIPAddress(string hcode);

    virtual bool DeleteExpiredTask();

private:
    string MyServer;
    string MyUser;
    string MyPassword;
    string MyDbName;
private:
#define DEFAULT_BES_DB_NAME                                 "BES"                       // 默认bes数据库名称
#define BES_DATABASE_HOST                                   "127.0.0.1"
#define BES_USER_NAME                                       "besns"
#define BES_PASSWORD                                        "sns123"
private:

    // 快照表字段

    enum SnapshotTableField
    {
        SNAPSHOT_TABLE_FIELD_SNAPSHOTID,
        SNAPSHOT_TABLE_FIELD_LMOUNTPOINT,
        SNAPSHOT_TABLE_FIELD_SNAPTYPE,
        SNAPSHOT_TABLE_FIELD_TRUSTED,
        SNAPSHOT_TABLE_FIELD_CHECKPOINTID,
        SNAPSHOT_TABLE_FIELD_SYNCCOPYID,
        SNAPSHOT_TABLE_FIELD_SNAPSHOTSOURCE,
        SNAPSHOT_TABLE_FIELD_SNAPSHOTLUNID,
        SNAPSHOT_TABLE_FIELD_STARTTIME, // 快照开始时间
        SNAPSHOT_TABLE_FIELD_ENDTIME, // 快照结束时间
        SNAPSHOT_TABLE_FIELD_SYNCSTATE, // 远程同步状态
    };


    // 副本表字段

    enum CopyTableField
    {
        COPY_TABLE_FIELD_SNAPSHOTID, // 快照编号
        COPY_TABLE_FIELD_LMOUNTPOINT, // 主机挂载点
        COPY_TABLE_FIELD_IQN, // iqn
        COPY_TABLE_FIELD_TARGETFILE, // target文件
        COPY_TABLE_FIELD_USED, // 副本状态
        COPY_TABLE_FIELD_MOUNTPOINT, // 挂载点
        COPY_TABLE_FIELD_COPYLUNID, // 副本lun编号
        COPY_TABLE_FIELD_COPYTYPE, // 副本类型
        COPY_TABLE_FIELD_CAPACITY, // 副本大小
        COPY_TABLE_FIELD_SENDERCOPYID, // 发送端副本编号
        COPY_TABLE_FIELD_FSNAME, // zfs文件系统名称
        COPY_TABLE_FIELD_LOOPDEVICENAME, // loop设备名称
    };

    // 主机表字段

    enum HostTableField
    {
        HOST_TABLE_FIELD_HOSTNAME,
        HOST_TABLE_FIELD_MACADDR,
        HOST_TABLE_FIELD_IPADDR,
        HOST_TABLE_FIELD_STATUS,
        HOST_TABLE_FIELD_ISACTIVE,
        HOST_TABLE_FIELD_HBAIPADDR,
        HOST_TABLE_FIELD_DBPROTECTED,
        HOST_TABLE_FIELD_OSTYPE,
        HOST_TABLE_FIELD_NETBOOTPROTOCOL,
        HOST_TABLE_FIELD_NETDEFAULTPROTOCOL, // 默认网启协议
        HOST_TABLE_FIELD_FCWWN,
        HOST_TABLE_FIELD_SYSTEMDIR,
        HOST_TABLE_FIELD_CONFIGDIR,
        HOST_TABLE_FIELD_VMMEMORY,
        HOST_TABLE_FIELD_VMCPU,
        HOST_TABLE_FIELD_LISTENPORT,
        HOST_TABLE_FIELD_USEDFORRDR,
        HOST_TABLE_FIELD_USEDFORLE,
        HOST_TABLE_FIELD_BUSINESSNICNAME,
        HOST_TABLE_FIELD_BRNICNAME,
        HOST_TABLE_FIELD_COPYMODE,
        HOST_TABLE_FIELD_SYNCFLAG,
        HOST_TABLE_FIELD_FCNAME,
        HOST_TABLE_FIELD_FCCHANNEL,
        HOST_TABLE_FIELD_INITIATORNAME,
        HOST_TABLE_FIELD_USEDFORNET,
        HOST_TABLE_FIELD_USEDFORVM,
        HOST_TABLE_FIELD_USEDFORFC,
        HOST_TABLE_FIELD_USEDFOREA,
        HOST_TABLE_FIELD_EAMODE,
        HOST_TABLE_FIELD_HEARTBEATTIMEOUT,
        HOST_TABLE_FIELD_QUERYINTERVAL,
        HOST_TABLE_FIELD_VIRTUALIP,
        HOST_TABLE_FIELD_VIRTUALMAC,
        HOST_TABLE_FIELD_RACGROUPID,
        HOST_TABLE_FIELD_EMERGENCYLOGONMODE,
        HOST_TABLE_FIELD_USEDFORMONITOR,
        HOST_TABLE_FIELD_ISAUTOTAKEOVER,
        HOST_TABLE_FIELD_VMBOOTCOPYMODE,
        HOST_TABLE_FIELD_VNCNUMBER, // VNC号
        HOST_TABLE_FIELD_MIRRORMODE,
        HOST_TABLE_FIELD_MIRRORSTATE,
    };


    // 策略表字段

    enum StrategyTableField
    {
        STRATEGY_TABLE_FIELD_STRATEGYTYPE, // 策略类型
        STRATEGY_TABLE_FIELD_CAPACITY, // 容量
        STRATEGY_TABLE_FIELD_SSCOPYID, // 应急副本id
        STRATEGY_TABLE_FIELD_TRUSTEDCOPYID, // 可信副本id
        STRATEGY_TABLE_FIELD_MIRRORLUNID, // 
        STRATEGY_TABLE_FIELD_MMLUNID,
        STRATEGY_TABLE_FIELD_VOLUMEPAIRID, // 卷对编号
        STRATEGY_TABLE_FIELD_CURRENTVOLUMETYPE, // 当前卷类型
        STRATEGY_TABLE_FIELD_USEDFORCDP, // 是否CDP保护
        STRATEGY_TABLE_FIELD_CDPSTATE, // CDP状态
        STRATEGY_TABLE_FIELD_MAXSNAPSHOT, // 最大快照数
        STRATEGY_TABLE_FIELD_USEDFORTIMEOUT, // 是否设置超时
        STRATEGY_TABLE_FIELD_COPYTIMEOUT, // 复制超时时间
        STRATEGY_TABLE_FIELD_CDPMODE, // CDP模式
        STRATEGY_TABLE_FIELD_DEVICEMINOR, // 设备号
        STRATEGY_TABLE_FIELD_STGID, // 存储id
        STRATEGY_TABLE_FIELD_STG_DEVICE, // 存储设备
        STRATEGY_TABLE_FIELD_STG_MOUNTPOINT, // 存储挂载点
        STRATEGY_TABLE_FIELD_STG_FSTYPE, // 文件系统类型
    };

    // 表类型

    enum TableType
    {
        TABLE_TYPE_VOLUME, // 卷表
        TABLE_TYPE_SNAPSHOT, // 快照表
        TABLE_TYPE_SNAPSHOTCOPY, // 副本表
    };
private:
#define DEFAULT_FIELD_RETURN_VALUE              ""          // 默认字段返回值（空）
#define DEFAULT_BES_ROOT_USER_NAME              "root"
#define DEFAULT_BES_ROOT_PASSWORD_1             "pcbtrams"
#define DEFAULT_BES_ROOT_PASSWORD_2             "sns123"
#define DEFAULT_DATABASE_DUMP_EXTENSION         ".sql"
#define DEFAULT_DATABASE_DUMP_DIRECTORY         "sql"               // 默认数据库导出目录
#define DEFAULT_DATABASE_REPAIR_LOG             "repair.log"
#define DEFAULT_MAX_SCHEDULE_TASK_COUNT         2                   // 默认最大调度任务数
private:

    /*
     *  功能：
     *      获取字段值
     *  参数：
     *      sqlstring       :   sql语句
     *      field           :   字段名
     *  返回：
     *      执行成功返回字段值，否则返回空
     */
    virtual string GetFieldValue(string sqlstring, string field);

    /*
     *  功能：
     *      获取字段的整数值
     *  参数：
     *      sqlstring       :   sql语句
     *      field           :   字段名
     *  返回：
     *      执行成功返回字段整数值，否则返回0
     */
    virtual int GetFieldValueInt(string sqlstring, string field);

    /*
     *  功能：
     *      获取字段的bool值
     *  参数：
     *      sqlstring       :   sql语句
     *      field           :   字段名
     *  返回：
     *      执行成功返回字段bool值，否则返回false
     */
    virtual bool GetFieldValueBool(string sqlstring, string field);

    /*
     *  功能：
     *      获取字段值
     *  参数：
     *      sqlstring       :   sql语句
     *      fldlist         :   字段列表
     *  返回：
     *      执行成功返回字段值列表
     */
    virtual map<string, string> GetFieldValue(string sqlstring, vector<string> fldlist);

    virtual map<string, string> GetFieldValue(sql::Connection * sqlconn, string sqlstring, vector<string> fldlist);
    /*
     *  功能：
     *      获取结果集
     *  参数：
     *      sqlstring       :   sql语句
     *  返回：
     *      结果集
     */
    virtual sql::ResultSet * GetResultSet(string sqlstring);

    /*
     *  功能：
     *      获取结果集
     *  参数：
     *      sqlconn         :   数据库连接
     *      sqlstring       :   sql语句
     *  返回：
     *      结果集
     */
    virtual sql::ResultSet * GetResultSet(sql::Connection * sqlconn, string sqlstring);

    /*
     *  功能：
     *      执行sql语句
     *  参数：
     *      sqlstring       :   sql语句
     *  返回：
     *      执行成功返回true，否则返回false
     */
    virtual bool ExecuteSql(string sqlstring);

    /*
     *  功能：
     *      执行sql语句
     *  参数：
     *      sqllist         :   sql语句列表
     *  返回：
     *      执行成功返回true，否则返回false
     */
    virtual bool ExecuteSql(vector<string> sqllist);

    /*
     *  功能：
     *      执行sql语句
     *  参数：
     *      sqlstring       :   sql语句
     *      sqlconn         :   数据库连接
     *  返回：
     *      执行成功返回true，否则返回false
     */
    virtual bool ExecuteSql(sql::Connection * sqlconn, string sqlstring);

    /*
     *  功能：
     *      执行sql语句
     *  参数：
     *      sqllist         :   sql语句列表
     *      sqlconn         :   数据库连接
     *  返回：
     *      执行成功返回true，否则返回false
     */
    virtual bool ExecuteSql(sql::Connection * sqlconn, vector<string> sqllist);

    /*
     *  功能：
     *      打开数据库连接
     *  参数：
     *      无
     *  返回：
     *      如果打开成功返回数据库连接对象，否则返回null
     */
    virtual sql::Connection * OpenConnection();

    /*
     *  功能：
     *      打开数据库连接
     *  参数：
     *      server          :   服务器
     *      user            :   用户名
     *      password        :   密码
     *      dbname          :   数据库名
     *  返回：
     *      如果打开成功返回数据库连接对象，否则返回null
     */
    virtual sql::Connection * OpenConnection(string server, string user, string password, string dbname);

    /*
     *  功能：
     *      关闭数据库连接
     *  参数：
     *      sqlconn         :   数据库连接对象
     *  返回：
     *      无
     */
    virtual void CloseConnection(sql::Connection * sqlconn);

    /*
     *  功能：
     *      创建statement对象
     *  参数：
     *      sqlconn         :   数据库连接对象
     *  返回：
     *      如果成功返回statement对象，否则返回null
     */
    virtual sql::Statement *CreateStatement(sql::Connection *sqlconn);

    /*
     *  功能：
     *      关闭statement对象
     *  参数：
     *      sqlstatement    :   statement对象
     *  返回：
     *      无
     */
    virtual void CloseStatement(sql::Statement * sqlstatement);

    /*
     *  功能：
     *      获取快照表字段名
     *  参数：
     *      field           :   字段
     *  返回：
     *      字段名
     */
    virtual string GetSnapshotFieldName(DbDataManager::SnapshotTableField field);

    /*
     *  功能：
     *      获取快照表字段值
     *  参数：
     *      hcode           :   主机编码
     *      snapshotid      :   副本id
     *      field           :   字段
     *  返回：
     *      快照记录字段值
     */
    virtual string GetSnapshotFieldValue(string hcode, string snapshotid, DbDataManager::SnapshotTableField field);


    /*
     *  功能：
     *      获取副本表字段名
     *  参数：
     *      field           :   字段
     *  返回：
     *      字段名
     */
    virtual string GetCopyFieldName(DbDataManager::CopyTableField field);

    /*
     *  功能：
     *      获取副本表字段值
     *  参数：
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      field           :   字段
     *  返回：
     *      副本记录字段值
     */
    virtual string GetCopyFieldValue(string hcode, string copyid, DbDataManager::CopyTableField field);

    /*
     *  功能：
     *      获取主机表字段名
     *  参数：
     *      field           :   字段
     *  返回：
     *      字段名
     */
    virtual string GetHostFieldName(DbDataManager::HostTableField field);

    /*
     *  功能：
     *      获取主机表字段名
     *  参数：
     *      hcode           :   主机编码
     *      field           :   字段
     *  返回：
     *      字段名
     */
    virtual string GetHostFieldValue(string hcode, DbDataManager::HostTableField field);

    /*
     *  功能：
     *      获取策略表字段名
     *  参数：
     *      field           :   字段
     *  返回：
     *      字段名
     */
    virtual string GetStrategyFieldName(DbDataManager::StrategyTableField field);

    /*
     *  功能：
     *      获取策略表字段名
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略编号
     *      field           :   字段
     *  返回：
     *      字段名
     */
    virtual string GetStrategyFieldValue(string hcode, string strategyid, DbDataManager::StrategyTableField field);


    /*
     *  功能：
     *      获取策略表字段名
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   策略编号
     *      field           :   字段
     *  返回：
     *      字段名
     */
    virtual map<string, string> GetStrategyFieldValue(string hcode, string strategyid);

    /*
     *  功能：
     *      根据字段列表获取连接后的字符串
     *  参数：
     *      fieldlist       :   字段列表
     *  返回：
     *      字段名字符串
     */
    virtual string GetFieldNameString(vector<string> fieldlist);

    /*
     *  功能：
     *      根据字段列表获取连接后的字符串
     *  参数：
     *      fieldlist       :   字段列表
     *      seperator       :   分隔符
     *  返回：
     *      字段名字符串
     */
    virtual string GetFieldNameString(vector<string> fieldlist, char seperator);

    /*
     *  功能：
     *      根据字段列表获取连接后的字符串
     *  参数：
     *      fieldlist       :   字段列表
     *      seperator       :   分隔符
     *  返回：
     *      字段名字符串
     */
    virtual string GetFieldNameString(vector<string> fieldlist, string seperator);

    /*
     *  功能：
     *      检查数据库备份目录
     *  参数
     *      path                :   备份路径
     *      filename            :   备份文件名
     *      backupnum           :   备份个数
     *  返回：
     *      无
     */
    virtual void CheckDatabaseBackupPath(string path, string filename, int backupnum);

    /*
     *  功能：
     *      根据主机编码和表类型获取表名
     *  参数
     *      hcode               :   主机编码
     *      type                :   表类型
     *  返回：
     *      表名
     */
    virtual string GetTableName(string hcode, DbDataManager::TableType type);

};


#endif	/* _DBDATAMANAGER_H */