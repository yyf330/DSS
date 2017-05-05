#include "dbdatamanager.h"

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
DbDataManager::DbDataManager(string server, string user, string password, string dbname)
{
    SetParameter(server, user, password, dbname);
}

/*
 *  功能：
 *      析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
DbDataManager::~DbDataManager()
{
}

//---------------------------------- 公有方法 ----------------------------------//

//================================== 任务相关 ==================================//

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
bool DbDataManager::AddTask(string cmdheader, string cmdcode, string parameter, DbDataManager::TaskState state, DbDataManager::TaskResult result, string faildesc)
{
    if (BeGeneralLib::IsStringEmpty(cmdheader) == true)
    {
        cmdheader = DEFAULT_INVALID_RETURN_VALUE2;
    }
    if (BeGeneralLib::IsStringEmpty(faildesc) == true)
    {
        faildesc = DEFAULT_INVALID_RETURN_VALUE2;
    }
    string SqlString = string("INSERT INTO TBL_TASK (COMMANDHEADER, COMMANDCODE, PARAMETER, STATUS, RESULT, FAILDESC, NEXTTIME, STARTTIME, ENDTIME, CREATETIME, UPDATETIME) VALUES('") + cmdheader + "','" + cmdcode + "','" + parameter + "'," +  BeGeneralLib::Number2String((int) state) + "," + BeGeneralLib::Number2String((int) result) + ",'" + faildesc + "', NOW(), NOW(), NOW(), NOW(), NOW())";
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除指定主机编码下的任务
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteTask(string hcode)
{
    string SqlString = string("DELETE FROM TBL_TASK WHERE HCODE ='") + hcode + string("' AND STATUS = 3");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新任务头和状态
 *  参数：
 *      taskid          :   任务ID
 *      cmdheader       :   命令头
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateTaskHeaderAndStatus(string taskid, string cmdheader)
{
    if (BeGeneralLib::IsStringEmptyOrZero(taskid) == true)
    {
        return true;
    }
    string SqlString = string("UPDATE TBL_TASK SET COMMANDHEADER ='") + cmdheader + string("', STATUS = 2, UPDATETIME=NOW(), STARTTIME = NOW() WHERE TASKID = ") + taskid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateTaskStatus(string taskid, DbDataManager::TaskState state, DbDataManager::TaskResult result)
{
    return UpdateTaskStatus(taskid, state, result, "");
}

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
bool DbDataManager::UpdateTaskStatus(string taskid, DbDataManager::TaskState state, DbDataManager::TaskResult result, string faildesc)
{
    if (BeGeneralLib::IsStringEmptyOrZero(taskid) == true)
    {
        return true;
    }

    string SqlString = "";
    if (BeGeneralLib::IsStringEmpty(faildesc) == true)
    {
        SqlString = string("UPDATE TBL_TASK SET STATUS = ") + BeGeneralLib::Number2String(state) + string(", RESULT = ") + BeGeneralLib::Number2String(result) + ", UPDATETIME = NOW(), ENDTIME = NOW() WHERE TASKID = " + taskid;
    }
    else
    {
        SqlString = string("UPDATE TBL_TASK SET STATUS = ") + BeGeneralLib::Number2String(state) + string(", RESULT = ") + BeGeneralLib::Number2String(result) + ", FAILDESC = CONCAT(TRIM(LEADING '-' FROM TRIM(LEADING '?' FROM FAILDESC)), '" + faildesc + "'), UPDATETIME = NOW(), ENDTIME = NOW() WHERE TASKID = " + taskid;
    }
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新任务状态描述
 *  参数：
 *      taskid          :   任务id
 *      desc            :   状态信息
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateTaskStateDesc(string taskid, string desc)
{
    if (BeGeneralLib::IsStringEmptyOrZero(taskid) == true)
    {
        return true;
    }
    string SqlString = string("UPDATE TBL_TASK SET STATEDESC = '") + desc + string("', UPDATETIME = NOW() WHERE TASKID = ") + taskid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取指定任务状态的任务结果集
 *  参数：
 *      state           :   任务状态
 *  返回：
 *      任务结果集
 */
sql::ResultSet * DbDataManager::GetTaskResultSet(TaskState state)
{
    string SqlString = string("SELECT TASKID, COMMANDHEADER, COMMANDCODE, PARAMETER, STATUS, MAXCOUNTER FROM TBL_TASK WHERE STATUS = ") + BeGeneralLib::Number2String(state);
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取license更新任务
 *  参数：
 *      无
 *  返回：
 *      license更新任务字段
 */
map<string, string> DbDataManager::GetLicenseUpdateTask()
{
    string SqlString = string("SELECT TASKID, PARAMETER FROM TBL_TASK WHERE STATUS = 0 AND COMMANDCODE = 915");

    vector<string> FieldList;
    FieldList.push_back("TASKID");
    FieldList.push_back("PARAMETER");
    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取任务记录
 *  参数：
 *      taskid          :   任务编号
 *  返回：
 *      任务记录字段
 */
map<string, string> DbDataManager::GetTaskRecord(string taskid)
{
    string SqlString = string("SELECT T.TASKID, T.FAILDESC, S.DESCRIPTION FROM TBL_TASK T, TBLCOMMANDCODE S  WHERE T.COMMANDCODE = S.CCODE AND T.TASKID = ") + taskid;

    vector<string> FieldList;
    FieldList.push_back("TASKID");
    FieldList.push_back("FAILDESC");
    FieldList.push_back("DESCRIPTION");
    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取任务超时时间
 *  参数：
 *      taskid          :   任务编号
 *  返回：
 *      任务超时时间
 */
int DbDataManager::GetTaskTimeout(string taskid)
{
    if (BeGeneralLib::IsStringEmptyOrZero(taskid) == true)
    {
        return 0;
    }

    string SqlString = string("SELECT TIME_TO_SEC(TIMEDIFF(NOW(), UPDATETIME)) AS TIMEOUT FROM TBL_TASK WHERE TASKID =") + taskid;
    return GetFieldValueInt(SqlString, "TIMEOUT");
}

/*
 *  功能：
 *      获取删除主机任务结果集
 *  参数：
 *      无
 *  返回：
 *      任务结果集
 */
sql::ResultSet * DbDataManager::GetDeleteHostTask()
{
    string SqlString = string("SELECT TASKID, COMMANDHEADER, COMMANDCODE, PARAMETER, STATUS, MAXCOUNTER FROM TBL_TASK WHERE (STATUS = 0 OR (STATUS = 0 AND EXECTYPE = 1 AND NEXTTIME <= NOW())) AND (COMMANDCODE = '961' OR COMMANDCODE ='968') GROUP BY GROUPID, TASKID");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取任务状态
 *  参数：
 *      taskid          :   任务编号
 *  返回：
 *      任务状态
 */
DbDataManager::TaskState DbDataManager::GetTaskState(string taskid)
{
    string SqlString = string("SELECT STATUS FROM TBL_TASK WHERE TASKID = ") + taskid;
    return (DbDataManager::TaskState)(GetFieldValueInt(SqlString, "STATUS"));
}

/*
 *  功能：
 *      获取任务id
 *  参数：
 *      cmdheader        :   命令头
 *  返回：
 *      任务id
 */
string DbDataManager::GetTaskId(string cmdheader)
{
    string SqlString = string("SELECT TASKID FROM TBL_TASK WHERE COMMANDHEADER = '") + cmdheader + string("'");
    return GetFieldValue(SqlString, "TASKID");
}

/*
 *  功能：
 *      是否有未完成的任务
 *  参数：
 *      无
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::HasUncompletedTask()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBL_TASK WHERE STATUS = 0 OR(STATUS = 0 AND EXECTYPE = 1 AND NEXTTIME <= NOW())");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================== 网启相关 ==================================//

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
bool DbDataManager::UpdateEmergencyCopyId(string hcode, string strategyid, string copyid)
{
    string LMountPoint = GetHostMountPoint(hcode, strategyid);
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET SSCOPYID = ") + copyid + string(" WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新网启副本ID
 *  参数：
 *      hcode           :   主机编码
 *      lmountpoint     :   盘符
 *      copyid          :   副本ID
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateTrustedCopyId(string hcode, string strategyid, string copyid)
{
    string LMountPoint = GetHostMountPoint(hcode, strategyid);
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET TRUSTEDCOPYID = ") + copyid + string(" WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateCDPProtectionState(string hcode, string strategyid, int protectionstate)
{
    string LMountPoint = GetHostMountPoint(hcode, strategyid);
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET USEDFORCDP = ") + BeGeneralLib::Number2String(protectionstate) + string(" WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取网启id
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      网启id
 */
string DbDataManager::GetEmergencyCopyId(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT T.SSCOPYID FROM TBLSNAPSHOTCOPY T, (SELECT SSCOPYID FROM TBLCOPYSTRATEGY WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("')V WHERE T.SSCOPYID = V.SSCOPYID");
    return GetFieldValue(SqlString, "SSCOPYID");
}

/*
 *  功能：
 *      获取系统盘应急副本id
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      网启副本id
 */
string DbDataManager::GetSystemEmergencyCopyId(string hcode)
{
    string SqlString = string("SELECT T.SSCOPYID FROM TBLSNAPSHOTCOPY T, (SELECT SSCOPYID FROM TBLCOPYSTRATEGY WHERE HCODE = '") + hcode + string("' AND STRATEGYTYPE = 'S')V WHERE T.SSCOPYID = V.SSCOPYID");
    return GetFieldValue(SqlString, "SSCOPYID");
}

/*
 *  功能：
 *      获取可信副本id
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      可信副本id
 */
string DbDataManager::GetTrustedCopyId(string hcode, string volumesymbol)
{
    return GetStrategyFieldValue(hcode, volumesymbol, DbDataManager::STRATEGY_TABLE_FIELD_TRUSTEDCOPYID);
}

/*
 *  功能：
 *      获取系统盘可信副本id
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      系统盘可信副本id
 */
string DbDataManager::GetSystemTrustedCopyId(string hcode)
{
    string SqlString = string("SELECT TRUSTEDCOPYID FROM TBLCOPYSTRATEGY WHERE HCODE = '") + hcode + string("' AND STRATEGYTYPE = 'S'");
    return GetFieldValue(SqlString, "TRUSTEDCOPYID");
}

/*
 *  功能：
 *      获取主机其他网启结果集
 *  参数：
 *      无
 *  返回：
 *      主机网启结果集
 */
sql::ResultSet * DbDataManager::GetHostEmbootResultSet()
{
    string SqlString = string("SELECT T.HCODE, V.SSCOPYID, V.IQN, S.HOSTNAME, S.MACADDR FROM TBLCOPYSTRATEGY T, TBLSNAPSHOTCOPY V, TBLHOST S WHERE T.SSCOPYID = V.SSCOPYID AND T.HCODE=S.HCODE AND T.STRATEGYTYPE='S' AND S.NETBOOTPROTOCOL='SNSBOOT' AND (S.OSTYPE='100' OR S.OSTYPE='101' OR S.OSTYPE='102' OR S.OSTYPE='200')");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取主机挂载点
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      获取的主机挂载点
 */
string DbDataManager::GetHostMountPoint(string hcode, string strategyid)
{
    if (BeGeneralLib::IsAllDigits(strategyid) == false)
    {
        return strategyid;
    }
    string SqlString = string("SELECT LMOUNTPOINT FROM TBLCOPYSTRATEGY WHERE STRATEGYID = ") + strategyid;
    return GetFieldValue(SqlString, "LMOUNTPOINT");
}

/*
 *  功能：
 *      获取策略的CDP状态
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      CDP状态
 */
DbDataManager::CDPState DbDataManager::GetStrategyCDPState(string hcode, string strategyid)
{
    return (DbDataManager::CDPState)(BeGeneralLib::StringToInt(GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_CDPSTATE)));
}

/*
 *  功能：
 *      获取策略的CDP模式
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      CDP模式
 */
DbDataManager::CDPMode DbDataManager::GetStrategyCDPMode(string hcode, string strategyid)
{
    return (DbDataManager::CDPMode)(BeGeneralLib::StringToInt(GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_CDPMODE)));
}

/*
 *  功能：
 *      获取策略的设备号
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      设备号
 */
string DbDataManager::GetStrategyDeviceMinor(string hcode, string strategyid)
{
    return GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_DEVICEMINOR);
}

/*
 *  功能：
 *      检查策略是否用于CDP保护
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsStrategyUsedForCDP(string hcode, string strategyid)
{
    return BeGeneralLib::StringToBool(GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_USEDFORCDP));
}

//================================== 盘符相关 ==================================//

/*
 *  功能：
 *      增加卷记录
 *  参数：
 *      hcode           :   主机编码
 *      device          :   设备
 *      lmountpoint     :   挂载点
 *      volumetype      :   分区类型
 *      capacity        :   容量
 *      used            :   已用空间
 *      isbootvolume    :   是否为启动卷
 *  返回：
 *      增加成功返回true，否则返回false
 */
bool DbDataManager::AddVolume(string hcode, string device, string lmountpoint, string volumetype, string capacity, string usedspace, string isbootvolume)
{
    string SqlString = string("INSERT INTO TBLVOLUME(HCODE, DEVICE, LMOUNTPOINT, VOLUMETYPE, CAPACITY, USEDSPACE, ISBOOTVOLUME, CREATETIME, UPDATETIME) VALUES(") +
            string("'") + hcode + string("',") +
            string("'") + device + string("',") +
            string("'") + lmountpoint + string("',") +
            string("'") + volumetype + string("',") +
            capacity + string(",") +
            usedspace + string(",") +
            isbootvolume + string(", NOW(), NOW())");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新卷记录
 *  参数：
 *      hcode           :   主机编码
 *      device          :   设备
 *      lmountpoint     :   挂载点
 *      volumetype      :   分区类型
 *      capacity        :   容量
 *      used            :   已用空间
 *      isbootvolume    :   是否为启动卷
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateVolume(string hcode, string device, string mountpoint, string volumetype, string capacity, string usedspace, string isbootvolume)
{
    string SqlString = string("UPDATE TBLVOLUME SET DEVICE = '") + device + string("', LMOUNTPOINT = '") + mountpoint + string("', VOLUMETYPE = '") + volumetype + string("', CAPACITY = ") + capacity + string(", USEDSPACE = ") + usedspace + string(" , ISBOOTVOLUME = ") + isbootvolume + string(" WHERE HCODE = '") + hcode + string("'");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取盘符文件系统
 *  参数
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      盘符文件系统
 */
string DbDataManager::GetVolumeFileSystem(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT VOLUMETYPE FROM TBLVOLUME WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return GetFieldValue(SqlString, "VOLUMETYPE");
}

/*
 *  功能：
 *      获取主机卷结果集
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      主机卷结果集
 */
sql::ResultSet * DbDataManager::GetVolumeResultSet(string hcode)
{
    string SqlString = string("SELECT HCODE, DEVICE, LMOUNTPOINT, VOLUMETYPE, CAPACITY, USEDSPACE, ISBOOTVOLUME FROM TBLVOLUME  WHERE HCODE ='") + hcode + string("'");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取磁盘的已用空间大小
 *  参数
 *      hcode           :   主机编码、
 *      volumesymbol    :   盘符
 *  返回：
 *      磁盘已用空间大小
 */
string DbDataManager::GetVolumeUsedSpace(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT USEDSPACE FROM TBLVOLUME WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("'");
    return GetFieldValue(SqlString, "USEDSPACE");
}

/*
 *  功能：
 *      获取卷对应的设备
 *  参数
 *      hcode           :   主机编码
 *      volumesymbol    :   挂载点
 *  返回：
 *      卷对应的设备
 */
string DbDataManager::GetVolumeDevice(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT DEVICE FROM TBLVOLUME WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("'");
    return GetFieldValue(SqlString, "DEVICE");
}

/*
 *  功能：
 *      盘符是否存在
 *  参数
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsVolumeSymbolExist(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLVOLUME WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      根据盘符判断是否为启动分区类型
 *  参数
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsBootPartitionType(string hcode, string volumesymbol)
{
    bool Flag = false;

    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    switch (GetHostOSType(hcode))
    {
        case DbDataManager::HOST_OS_TYPE_WINDOWS:
        {
            Flag = (BeGeneralLib::StringToUpper(LMountPoint) == DEFAULT_WINDOWS_SYSTEM_FLAG);
            break;
        }
        case DbDataManager::HOST_OS_TYPE_LINUX:
        {
            Flag = (LMountPoint == DEFAULT_LINUX_SYSTEM_FLAG);
            break;
        }
        case DbDataManager::HOST_OS_TYPE_AIX:
        case DbDataManager::HOST_OS_TYPE_HPUX:
        {
            Flag = (LMountPoint == DEFAULT_AIX_SYSTEM_FLAG);
            break;
        }
    }
    return Flag;
}

//================================== 副本相关 ==================================//

/*
 *  功能：
 *      增加副本记录
 *  参数：
 *      snapshotid      :   快照ID
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *      type            :   副本类型
 *  返回：
 *      增加成功返回true，否则返回false
 */
bool DbDataManager::AddCopyRecord(string snapshotid, string hcode, string volumesymbol, DbDataManager::CopyType type)
{
    return AddCopyRecord(snapshotid, hcode, volumesymbol, "?", type);
}

/*
 *  功能：
 *      增加副本记录
 *  参数：
 *      snapshotid      :   快照ID
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *      copylunid       :   副本lunid
 *      type            :   副本类型
 *  返回：
 *      增加成功返回true，否则返回false
 */
bool DbDataManager::AddCopyRecord(string snapshotid, string hcode, string volumesymbol, string copylunid, DbDataManager::CopyType type)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string Capacity = GetVolumeCapacity(hcode, LMountPoint);
    string SqlString = string("INSERT INTO TBLSNAPSHOTCOPY(SNAPSHOTID, HCODE, LMOUNTPOINT, COPYLUNID, COPYTYPE, CAPACITY, CREATETIME, UPDATETIME) VALUES(") + snapshotid + string(", '") + hcode + string("','") + LMountPoint + string("','") + copylunid + string("',") + BeGeneralLib::Number2String(type) + string(",'") + Capacity + string("', NOW(), NOW())");
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::AddCopyRecord(string snapshotid, string hcode, string volumesymbol, string used, string iqn, string mountpoint, string copyid)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string Capacity = GetVolumeCapacity(hcode, LMountPoint);
    string SqlString = string("INSERT INTO TBLSNAPSHOTCOPY(SNAPSHOTID, HCODE, LMOUNTPOINT, USED, IQN, MOUNTPOINT, SSCOPYID, CAPACITY, CREATETIME, UPDATETIME) VALUES(") + snapshotid + string(", '") + hcode + string("','") + LMountPoint + string("','") + used + string("', '") + iqn + string("','") + mountpoint + string("',") + copyid + string(",'") + Capacity + string("', NOW(), NOW())");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除副本记录
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteCopy(string hcode, string copyid)
{
    string SqlString = string("DELETE FROM TBLSNAPSHOTCOPY WHERE SSCOPYID = ") + copyid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除副本记录集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteCopySet(string hcode)
{
    string SqlString = string("DELETE FROM TBLSNAPSHOTCOPY WHERE HCODE = ") + hcode;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateCopyStatus(string hcode, string copyid, DbDataManager::CopyState state)
{
    string SqlString = string("UPDATE TBLSNAPSHOTCOPY SET USED = ") + BeGeneralLib::Number2String(state) + string(" WHERE SSCOPYID =") + copyid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新副本根路径
 *  参数：
 *      copypath        :   副本根路径
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateCopyRootPath(string copypath)
{
    string SqlString = string("UPDATE TBLBESINFORMATION SET COPYPATH ='") + copypath + string("'");
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateCopy(string hcode, string copyid, string iqn, string mountpoint, string targetfile)
{
    string SqlString = string("UPDATE TBLSNAPSHOTCOPY SET IQN='") + iqn + string("', MOUNTPOINT='") + mountpoint + string("', TARGETFILE='") + targetfile + string("' WHERE SSCOPYID=") + copyid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateCopySnapshotId(string hcode, string copyid, string snapshotid)
{
    string SqlString = string("UPDATE TBLSNAPSHOTCOPY SET SNAPSHOTID = ") + snapshotid + string(" WHERE SSCOPYID = ") + copyid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateCopyLunId(string hcode, string copyid, string copylunid)
{
    string SqlString = string("UPDATE TBLSNAPSHOTCOPY SET COPYLUNID = '") + copylunid + string("' WHERE SSCOPYID = ") + copyid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateCopySenderCopyId(string hcode, string copyid, string sendercopyid)
{
    string SqlString = string("UPDATE TBLSNAPSHOTCOPY SET SENDERCOPYID = '") + sendercopyid + string("' WHERE SSCOPYID = ") + copyid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateCopyFsName(string hcode, string copyid, string fsname)
{
    string SqlString = string("UPDATE TBLSNAPSHOTCOPY SET FSNAME = '") + fsname + string("' WHERE SSCOPYID = ") + copyid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateCopyLoopDevice(string hcode, string copyid, string loopdevice)
{
    string SqlString = string("UPDATE TBLSNAPSHOTCOPY SET LOOPDEVICENAME = '") + loopdevice + string("' WHERE SSCOPYID = ") + copyid;
    return ExecuteSql(SqlString);
}

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
string DbDataManager::GetCopyId(string hcode, string value, DbDataManager::CopyIDRetrieveType type)
{
    string SqlString = "";
    switch (type)
    {
        case DbDataManager::COPY_ID_RETRIEVE_TYPE_SNAPSHOTID:
        {
            SqlString = string("SELECT SSCOPYID FROM TBLSNAPSHOTCOPY WHERE SNAPSHOTID =") + value;
            break;
        }
        case DbDataManager::COPY_ID_RETRIEVE_TYPE_IQN:
        {
            SqlString = string("SELECT SSCOPYID FROM TBLSNAPSHOTCOPY WHERE IQN ='") + value + string("'");

            break;
        }
    }
    return GetFieldValue(SqlString, "SSCOPYID");
}

/*
 *  功能：
 *      获取副本结果集
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      副本结果集
 */
sql::ResultSet * DbDataManager::GetCopyResultSet(string hcode, string volumesymbol)
{
    string SqlString = string("SELECT SSCOPYID, HCODE, LMOUNTPOINT, USED FROM TBLSNAPSHOTCOPY WHERE HCODE ='") + hcode + string("'");
    if (BeGeneralLib::IsStringEmpty(volumesymbol) == false)
    {
        SqlString = SqlString + string(" AND LMOUNTPOINT = '") + GetHostMountPoint(hcode, volumesymbol) + string("'");
    }
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取副本结果集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      副本结果集
 */
sql::ResultSet * DbDataManager::GetCopyResultSet(string hcode)
{
    return GetCopyResultSet(hcode, DEFAULT_EMPTY_STRING);
}

/*
 *  功能：
 *      获取副本记录字段值
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本记录字段值
 */
map<string, string> DbDataManager::GetCopyFieldValue(string hcode, string copyid)
{
    vector<string> FieldList;
    FieldList.push_back("SSCOPYID");
    FieldList.push_back("SNAPSHOTID");
    FieldList.push_back("LMOUNTPOINT");
    FieldList.push_back("USED");
    FieldList.push_back("IQN");
    FieldList.push_back("MOUNTPOINT");
    FieldList.push_back("TARGETFILE");
    FieldList.push_back("COPYLUNID");
    FieldList.push_back("COPYTYPE");
    FieldList.push_back("CAPACITY");
    FieldList.push_back("SENDERCOPYID");
    FieldList.push_back("FSNAME");
    FieldList.push_back("LOOPDEVICENAME");

    string FieldNameString = GetFieldNameString(FieldList);
    string SqlString = string("SELECT") + SEPARATOR_CHAR_SPACE + FieldNameString + SEPARATOR_CHAR_SPACE + string(" FROM TBLSNAPSHOTCOPY WHERE SSCOPYID =") + copyid;
    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取副本零id
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      副本零id
 */
string DbDataManager::GetZeroCopyId(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT MIN(SSCOPYID) AS SSCOPYID FROM TBLSNAPSHOTCOPY WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return GetFieldValue(SqlString, "SSCOPYID");
}

/*
 *  功能：
 *      获取最新副本id
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      最新副本id
 */
string DbDataManager::GetFinalCopyId(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT MAX(SSCOPYID) AS SSCOPYID FROM TBLSNAPSHOTCOPY WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return GetFieldValue(SqlString, "SSCOPYID");
}

/*
 *  功能：
 *      获取主机非系统网启副本结果集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机网启结果集
 */
sql::ResultSet * DbDataManager::GetNonSystemEmergencyCopyResultSet(string hcode)
{
    string SqlString = string("SELECT T.SSCOPYID, T.HCODE, T.IQN, T.LMOUNTPOINT, T.TARGETFILE, V.CAPACITY, V.STRATEGYID FROM TBLSNAPSHOTCOPY T, (SELECT STRATEGYID, SSCOPYID, CAPACITY FROM TBLCOPYSTRATEGY WHERE HCODE = '") + hcode + string("' AND STRATEGYTYPE !='S')V WHERE T.SSCOPYID = V.SSCOPYID");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取定时快照应急副本结果集
 *  参数：
 *      无
 *  返回：
 *      应急副本结果集
 */
sql::ResultSet * DbDataManager::GetTimingSnapshotEmergencyCopyResultSet()
{
    string SqlString = string("SELECT T.SSCOPYID, T.HCODE, T.LMOUNTPOINT, T.TARGETFILE, T.IQN, S.STRATEGYTYPE, S.STRATEGYID  FROM TBLSNAPSHOTCOPY T, TBLCOPYSTRATEGY S, TBLSTORAGESPACE R WHERE S.STGID = R.STGID AND R.FSTYPE = 'zfs' AND S.USEDFORCDP = 1 AND T.SSCOPYID = S.SSCOPYID AND T.HCODE = S.HCODE AND T.LMOUNTPOINT = S.LMOUNTPOINT");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取副本根路径
 *  参数：
 *      无
 *  返回：
 *      副本根路径
 */
string DbDataManager::GetCopyRootPath()
{
    string SqlString = string("SELECT COPYPATH FROM TBLBESINFORMATION");
    return GetFieldValue(SqlString, "COPYPATH");
}

/*
 *  功能：
 *      获取副本lunid
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本lunid
 */
string DbDataManager::GetCopyLunId(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_COPYLUNID);
}

/*
 *  功能：
 *      获取副本记录的快照id
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本记录的快照id
 */
string DbDataManager::GetCopySnapshotId(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_SNAPSHOTID);
}

/*
 *  功能：
 *      获取副本类新
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本类新
 */
DbDataManager::CopyType DbDataManager::GetCopyType(string hcode, string copyid)
{
    return (DbDataManager::CopyType)(BeGeneralLib::StringToInt(GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_COPYTYPE)));
}

/*
 *  功能：
 *      获取副本iqn
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本iqn
 */
string DbDataManager::GetCopyIqn(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_IQN);
}

/*
 *  功能：
 *      获取副本target文件
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本target文件
 */
string DbDataManager::GetCopyTargetFile(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_TARGETFILE);
}

/*
 *  功能：
 *      获取副本对应的卷标识
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本对应的卷标识
 */
string DbDataManager::GetCopyLMountPoint(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_LMOUNTPOINT);
}

/*
 *  功能：
 *      获取副本挂载点
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本挂载点
 */
string DbDataManager::GetCopyMountPoint(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_MOUNTPOINT);
}

/*
 *  功能：
 *      获取指定类型的副本个数
 *  参数：
 *      hcode           :   主机编码
 *      type            :   类型
 *  返回：
 *      副本个数
 */
int DbDataManager::GetCopyCountWithType(string hcode, DbDataManager::CopyType type)
{
    // 检查输入的有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true)
    {
        return 0;
    }
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOTCOPY WHERE HCODE = '") + hcode + string("' AND COPYTYPE = ") + BeGeneralLib::Number2String(type);
    return GetFieldValueInt(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      获取副本状态
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本状态
 */
DbDataManager::CopyState DbDataManager::GetCopyState(string hcode, string copyid)
{
    return (DbDataManager::CopyState)(BeGeneralLib::StringToInt(GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_USED)));
}

/*
 *  功能：
 *      获取发送端副本编号
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      发送端副本编号
 */
string DbDataManager::GetCopySenderCopyId(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_SENDERCOPYID);
}

/*
 *  功能：
 *      获取副本容量
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本容量
 */
int DbDataManager::GetCopyCapacity(string hcode, string copyid)
{
    return BeGeneralLib::StringToInt(GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_CAPACITY));
}

/*
 *  功能：
 *      获取副本容量
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      副本容量
 */
string DbDataManager::GetCopyFsName(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_FSNAME);
}

/*
 *  功能：
 *      获取副本loop设备名称
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      loop设备名称
 */
string DbDataManager::GetCopyLoopDevice(string hcode, string copyid)
{
    return GetCopyFieldValue(hcode, copyid, DbDataManager::COPY_TABLE_FIELD_LOOPDEVICENAME);
}

/*
 *  功能：
 *      检查副本id是否为应急副本
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本编号
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsCopyAsEmergencyCopy(string hcode, string copyid)
{
    // 检查输入的有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true || BeGeneralLib::IsStringEmptyOrZero(copyid) == true)
    {
        return false;
    }
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCOPYSTRATEGY T, (SELECT SSCOPYID, HCODE, LMOUNTPOINT FROM TBLSNAPSHOTCOPY WHERE SSCOPYID = ") + copyid + string(") V WHERE T.HCODE = V.HCODE AND T.LMOUNTPOINT = V.LMOUNTPOINT AND T.SSCOPYID = V.SSCOPYID");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      副本是否存在
 *  参数
 *      hcode           :   主机编码
 *      iqn             :   副本IQN
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsCopyExist(string hcode, string copyid)
{
    // 检查输入的有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true || BeGeneralLib::IsStringEmptyOrZero(copyid) == true)
    {
        return false;
    }
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOTCOPY WHERE SSCOPYID = ") + copyid;
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      副本是否使用中
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostHasCopyUsed(string hcode)
{
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true)
    {
        return false;
    }

    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOTCOPY WHERE HCODE ='") + hcode + string("' AND USED = 1");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      副本是否使用中
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsVolumeHasCopyUsed(string hcode, string volumesymbol)
{
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true || BeGeneralLib::IsStringEmptyOrZero(volumesymbol) == true)
    {
        return false;
    }

    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOTCOPY WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("' AND USED = 1");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      副本是否使用中
 *  参数：
 *      hcode           :   主机编码
 *      copyid          :   副本id
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsCopyUsed(string hcode, string copyid)
{
    return (GetCopyState(hcode, copyid) == DbDataManager::COPY_STATE_USED);
}

//================================== 快照相关 ==================================//

/*
 *  功能：
 *      增加快照记录
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *      snaptype        :   快照类型
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::AddSnapshotRecord(string hcode, string volumesymbol, DbDataManager::SnapType type)
{
    // 检查输入值是否为空
    if (BeGeneralLib::IsStringEmptyOrZero(hcode) == true || BeGeneralLib::IsStringEmptyOrZero(volumesymbol) == true)
    {
        return false;
    }

    // 获取主机挂载点
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);

    // 获取策略编号
    string StrategyId = GetStrategyId(hcode, volumesymbol, DbDataManager::STRATEGY_ID_SEARCH_TYPE_LMOUNTPOINT);

    // 插入快照记录
    string SqlString = string("INSERT INTO TBLSNAPSHOT(HCODE, STRATEGYID, LMOUNTPOINT, SNAPTYPE, STARTTIME) VALUES('") + hcode + string("','") + StrategyId + string("','") + LMountPoint + string("','") + GetSnapshotTypeString(type) + string("',NOW())");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::AddSnapshotRecord(string hcode, string volumesymbol, DbDataManager::SnapType type, string starttime, string endtime, string checkpointid, DbDataManager::SnapshotSource source, string synccopyid)
{
    return AddSnapshotRecord(hcode, volumesymbol, GetSnapshotTypeString(type), starttime, endtime, checkpointid, source, synccopyid);
}

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
bool DbDataManager::AddSnapshotRecord(string hcode, string volumesymbol, string snapshottype, string starttime, string endtime, string checkpointid, DbDataManager::SnapshotSource source, string synccopyid)
{
    return AddSnapshotRecord(hcode, volumesymbol, snapshottype, starttime, endtime, checkpointid, BeGeneralLib::Number2String(source), synccopyid);
}

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
bool DbDataManager::AddSnapshotRecord(string hcode, string volumesymbol, string snapshottype, string starttime, string endtime, string checkpointid, string source, string synccopyid)
{
    // 检查输入值是否为空
    if (BeGeneralLib::IsStringEmptyOrZero(hcode) == true || BeGeneralLib::IsStringEmptyOrZero(volumesymbol) == true)
    {
        return false;
    }

    // 获取主机挂载点
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);

    // 获取策略编号
    string StrategyId = GetStrategyId(hcode, volumesymbol, DbDataManager::STRATEGY_ID_SEARCH_TYPE_LMOUNTPOINT);

    // 插入快照记录
    string SqlString = string("INSERT INTO TBLSNAPSHOT(HCODE, STRATEGYID, LMOUNTPOINT, SNAPTYPE, STARTTIME, ENDTIME, CHECKPOINTID, SNAPSHOTSOURCE, SYNCCOPYID) VALUES('") + hcode + string("','") + StrategyId + string("','") + LMountPoint + string("','") + snapshottype + string("', '") + starttime + string("','") + endtime + string("','") + checkpointid + string("',") + source + string(",") + synccopyid + string(")");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除快照
 *  参数：
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteSnapshot(string hcode, string snapshotid)
{
    // 检查输入值是否为空
    if (BeGeneralLib::IsStringEmptyOrZero(hcode) == true || BeGeneralLib::IsStringEmptyOrZero(snapshotid) == true)
    {
        return false;
    }

    string SqlString = string("DELETE FROM TBLSNAPSHOT WHERE SNAPSHOTID = ") + snapshotid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除快照零记录
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteZeroSnapshot(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("DELETE FROM TBLSNAPSHOT WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("' AND SNAPTYPE ='Z'");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除所有快照
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteAllSnapshot(string hcode)
{
    string SqlString = string("DELETE FROM TBLSNAPSHOT WHERE HCODE ='") + hcode + string("'");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除最新快照记录
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteFinalSnapshot(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("DELETE T FROM TBLSNAPSHOT T, (SELECT MAX(SNAPSHOTID) AS SNAPSHOTID FROM TBLSNAPSHOT WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("')V WHERE T.SNAPSHOTID = V.SNAPSHOTID");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新快照根路径
 *  参数：
 *      snapshotpath    :   快照路径
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateSnapshotRootPath(string snapshotpath)
{
    string SqlString = string("UPDATE TBLBESINFORMATION SET SNAPSHOTPATH ='") + snapshotpath + string("'");
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateFinalSnapshotEndTime(string hcode, string volumesymbol, string snapshotlunid)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("UPDATE TBLSNAPSHOT T, (SELECT MAX(SNAPSHOTID) AS SNAPSHOTID FROM TBLSNAPSHOT WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("')V SET ENDTIME = NOW() WHERE T.SNAPSHOTID = V.SNAPSHOTID");
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateSnapshotTrusted(string hcode, string snapshotid, bool trustedflag)
{
    string SqlString = string("UPDATE TBLSNAPSHOT SET TRUSTED =") + BeGeneralLib::Number2String((int) trustedflag) + string(" WHERE SNAPSHOTID =") + snapshotid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateSnapshotEndTime(string hcode, string snapshotid, string copyid)
{
    string SqlString = string("UPDATE TBLSNAPSHOT SET ENDTIME = NOW(), SYNCCOPYID = ") + copyid + SEPARATOR_CHAR_SPACE + string(" WHERE SNAPSHOTID =") + snapshotid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateSnapshotEndTime(string hcode, string snapshotid, string copyid, string endtime, string checkpointid, DbDataManager::SnapshotSource source)
{
    string SqlString = string("UPDATE TBLSNAPSHOT SET ENDTIME ='") + endtime + string("', SYNCCOPYID = ") + copyid + string(", CHECKPOINTID = ") + checkpointid + string(", SNAPSHOTSOURCE =") + BeGeneralLib::Number2String(source) + SEPARATOR_CHAR_SPACE + string(" WHERE SNAPSHOTID =") + snapshotid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateSnapshotSyncState(string hcode, string snapshotid, DbDataManager::SnapshotSyncState state)
{
    string SqlString = string("UPDATE TBLSNAPSHOT SET SYNCSTATE ='") + BeGeneralLib::Number2String(state) + string("' WHERE SNAPSHOTID =") + snapshotid;
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateSnapshotSyncStateBySyncCopyId(string hcode, string synccopyid, DbDataManager::SnapshotSyncState state)
{
    string SqlString = string("UPDATE TBLSNAPSHOT SET SYNCSTATE ='") + BeGeneralLib::Number2String(state) + string("' WHERE HCODE = '") + hcode + string("' AND SYNCCOPYID =") + synccopyid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取快照根路径
 *  参数：
 *      无
 *  返回：
 *      快照根路径
 */
string DbDataManager::GetSnapshotRootPath()
{
    string SqlString = string("SELECT SNAPSHOTPATH FROM TBLBESINFORMATION");
    return GetFieldValue(SqlString, "SNAPSHOTPATH");
}

/*
 *  功能：
 *      获取快照个数
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      快照个数
 */
int DbDataManager::GetSnapshotCount(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT COUNT(*) AS SNAPCOUNT FROM TBLSNAPSHOT WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return (GetFieldValueInt(SqlString, "SNAPCOUNT"));
}

/*
 *  功能：
 *      获取快照零id
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      快照零id
 */
string DbDataManager::GetZeroSnapshotId(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT SNAPSHOTID FROM TBLSNAPSHOT WHERE HCODE='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("' AND SNAPTYPE ='Z'");
    return GetFieldValue(SqlString, "SNAPSHOTID");
}

/*
 *  功能：
 *      获取最新快照id
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符
 *  返回：
 *      快照id
 */
string DbDataManager::GetFinalSnapshotId(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT S.SNAPSHOTID FROM TBLSNAPSHOT S, (SELECT MAX(SNAPSHOTID) AS SNAPSHOTID FROM TBLSNAPSHOT WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("') V WHERE S.SNAPSHOTID = V.SNAPSHOTID");
    return GetFieldValue(SqlString, "SNAPSHOTID");
}

/*
 *  功能：
 *      获取快照lunid
 *  参数：
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *  返回：
 *      快照lunid
 */
string DbDataManager::GetSnapshotLunId(string hcode, string snapshotid)
{
    return GetSnapshotFieldValue(hcode, snapshotid, DbDataManager::SNAPSHOT_TABLE_FIELD_SNAPSHOTLUNID);
}

/*
 *  功能：
 *      获取指定盘符的快照结果集
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符或策略id
 *  返回：
 *      快照结果集
 */
sql::ResultSet * DbDataManager::GetSnapshotResultSet(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT SNAPSHOTID, SNAPSHOTLUNID FROM TBLSNAPSHOT WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("' ORDER BY SNAPSHOTID");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取该主机下的所有快照记录结果集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      快照结果集
 */
sql::ResultSet * DbDataManager::GetSnapshotResultSet(string hcode)
{
    string SqlString = string("SELECT SNAPSHOTID, SNAPSHOTLUNID FROM TBLSNAPSHOT WHERE HCODE = '") + hcode + string("' ORDER BY SNAPSHOTID");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取指定盘符、同步状态和同步副本id的快照结果集
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符或策略id
 *      synccopyid      :   同步副本id
 *      state           :   快照同步状态
 *  返回：
 *      快照结果集
 */
sql::ResultSet * DbDataManager::GetSnapshotResultSet(string hcode, string volumesymbol, DbDataManager::SnapshotSyncState state, string syncopyid)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT SNAPSHOTID, HCODE, LMOUNTPOINT, SYNCCOPYID, SYNCSTATE, SNAPTYPE, CHECKPOINTID, SNAPSHOTSOURCE, STARTTIME, ENDTIME FROM TBLSNAPSHOT WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("' AND SYNCCOPYID = ") + syncopyid + string(" AND SYNCSTATE = ") + BeGeneralLib::Number2String(state) + string(" ORDER BY STARTTIME");
    return GetResultSet(SqlString);
}

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
sql::ResultSet * DbDataManager::GetSnapshotResultSet(string hcode, string volumesymbol, string syncopyid)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT SNAPSHOTID, HCODE, LMOUNTPOINT, SYNCCOPYID, SYNCSTATE, SNAPTYPE, CHECKPOINTID, SNAPSHOTSOURCE, STARTTIME, ENDTIME FROM TBLSNAPSHOT WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("' AND SYNCCOPYID = ") + syncopyid + string(" ORDER BY STARTTIME");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取快照结束时间
 *  参数：
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *  返回：
 *      快照结束时间
 */
string DbDataManager::GetSnapshotEndTime(string hcode, string snapshotid)
{
    return GetSnapshotFieldValue(hcode, snapshotid, DbDataManager::SNAPSHOT_TABLE_FIELD_ENDTIME);
}

/*
 *  功能：
 *      获取快照对应的checkpoint编号
 *  参数：
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *  返回：
 *      快照对应的checkpoint编号
 */
string DbDataManager::GetSnapshotCheckPointId(string hcode, string snapshotid)
{
    return GetSnapshotFieldValue(hcode, snapshotid, DbDataManager::SNAPSHOT_TABLE_FIELD_CHECKPOINTID);
}

/*
 *  功能：
 *      获取快照同步副本id
 *  参数：
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *  返回：
 *      快照同步副本id
 */
string DbDataManager::GetSnapshotSyncCopyId(string hcode, string snapshotid)
{
    return GetSnapshotFieldValue(hcode, snapshotid, DbDataManager::SNAPSHOT_TABLE_FIELD_SYNCCOPYID);
}

/*
 *  功能：
 *      获取快照的远程同步状态
 *  参数：
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *  返回：
 *      远程同步状态
 */
DbDataManager::SnapshotSyncState DbDataManager::GetSnapshotSyncState(string hcode, string snapshotid)
{
    return (DbDataManager::SnapshotSyncState)(BeGeneralLib::StringToInt(GetSnapshotFieldValue(hcode, snapshotid, DbDataManager::SNAPSHOT_TABLE_FIELD_SYNCSTATE)));
}

/*
 *  功能：
 *      获取快照记录字段值
 *  参数：
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *  返回：
 *      快照记录字段值
 */
map<string, string> DbDataManager::GetSnapshotFieldValue(string hcode, string snapshotid)
{
    vector<string> FieldList;
    FieldList.push_back("SNAPSHOTID");
    FieldList.push_back("HCODE");
    FieldList.push_back("LMOUNTPOINT");
    FieldList.push_back("SNAPTYPE");
    FieldList.push_back("TRUSTED");
    FieldList.push_back("CHECKPOINTID");
    FieldList.push_back("SYNCCOPYID");
    FieldList.push_back("SNAPSHOTSOURCE");
    FieldList.push_back("SNAPSHOTLUNID");
    FieldList.push_back("SYNCSTATE");
    FieldList.push_back("STARTTIME");
    FieldList.push_back("ENDTIME");
    string FieldNameString = GetFieldNameString(FieldList);
    string SqlString = string("SELECT ") + FieldNameString + string(" FROM TBLSNAPSHOT WHERE HCODE = '") + hcode + string("' AND SNAPSHOTID = ") + snapshotid;
    return GetFieldValue(SqlString, FieldList);
}

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
string DbDataManager::GetLastSyncSnapshotId(string hcode, string volumesymbol, string synccopyid)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT MAX(SNAPSHOTID) AS SNAPSHOTID FROM TBLSNAPSHOT WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("' AND SYNCCOPYID = ") + synccopyid + string(" AND SYNCSTATE = ") + BeGeneralLib::Number2String(DbDataManager::SNAPSHOT_SYNC_STATE_COMPLETED);
    return GetFieldValue(SqlString, "SNAPSHOTID");
}

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
string DbDataManager::GetSnapshotId(string hcode, string volumesymbol, string synccopyid, string starttime, string endtime)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT SNAPSHOTID FROM TBLSNAPSHOT WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("' AND SYNCCOPYID = ") + synccopyid + string(" AND STARTTIME = '") + starttime + string("' AND ENDTIME = '") + endtime + string("'");
    return GetFieldValue(SqlString, "SNAPSHOTID");
}

/*
 *  功能：
 *      检查快照id是否为快照零
 *  参数：
 *      hcode           :   主机编码
 *      snapshotid      :   快照id
 *  返回：
 *      如果为快照零返回true，否则返回false
 */
bool DbDataManager::IsZeroSnapshot(string hcode, string snapshotid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOT WHERE SNAPSHOTID = ") + snapshotid + string(" AND SNAPTYPE ='Z'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查快照id是否为快照零
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   盘符或策略id
 *  返回：
 *      如果快照零存在返回true，否则返回false
 */
bool DbDataManager::IsZeroSnapshotExist(string hcode, string volumesymbol)
{
    string LMountPoint = GetHostMountPoint(hcode, volumesymbol);
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOT WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("' AND SNAPTYPE ='Z'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      快照是否存在
 *  参数
 *      hcode           :   主机编码
 *      snapshotid      :   快照ID
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsSnapshotExist(string hcode, string snapshotid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOT WHERE SNAPSHOTID = ") + snapshotid;
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

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
bool DbDataManager::IsSnapshotExist(string hcode, string lmountpoint, string starttime, string endtime, string synccopyid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOT WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT ='") + lmountpoint + string("' AND STARTTIME = '") + starttime + string("' AND ENDTIME = '") + endtime + string("' AND SYNCCOPYID = ") + synccopyid;
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      是否最新快照
 *  参数
 *      hcode           :   主机编码
 *      snapshotid      :   快照ID
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsFinalSnapshot(string hcode, string snapshotid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOT S, (SELECT MAX(SNAPSHOTID) AS SNAPSHOTID FROM TBLSNAPSHOT T , (SELECT HCODE, LMOUNTPOINT FROM TBLSNAPSHOT WHERE SNAPSHOTID = ") + snapshotid + string(") R  WHERE R.HCODE = T.HCODE AND R.LMOUNTPOINT = T.LMOUNTPOINT)V WHERE S.SNAPSHOTID = V.SNAPSHOTID AND S.SNAPSHOTID =") + snapshotid;
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      快照是否有对应的副本
 *  参数
 *      hcode           :   主机编码
 *      snapshotid      :   快照ID
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsSnapshotHasCopy(string hcode, string snapshotid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOTCOPY WHERE SNAPSHOTID = ") + snapshotid;
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      是否为可信快照
 *  参数
 *      hcode           :   主机编码
 *      snapshotid      :   快照ID
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsTrustedSnapshot(string hcode, string snapshotid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOT WHERE SNAPSHOTID = ") + snapshotid + string(" AND TRUSTED = 1");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      获取快照类型字符串
 *  参数
 *      type            :   快照类型
 *  返回：
 *      快照类型字符串
 */
string DbDataManager::GetSnapshotTypeString(DbDataManager::SnapType type)
{
    // 获取快照类型
    string SnapshotTypeString = "";
    switch (type)
    {
        case DbDataManager::SNAPSHOT_TYPE_ZERO:
        {
            SnapshotTypeString = string("Z");
            break;
        }
        case DbDataManager::SNAPSHOT_TYPE_VOLUME:
        {
            SnapshotTypeString = string("V");
            break;
        }
        case DbDataManager::SNAPSHOT_TYPE_DATABASE:
        {
            SnapshotTypeString = string("D");

            break;
        }
    }
    return SnapshotTypeString;
}

//================================== 策略相关 ==================================//

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
bool DbDataManager::AddStrategy(string hcode, string lmountpoint, string name, string type, string capacity, string copyid)
{
    string SqlInsert = "INSERT INTO TBLCOPYSTRATEGY (HCODE, LMOUNTPOINT, STRATEGYNAME, STRATEGYTYPE, CAPACITY, SSCOPYID) VALUES('" + hcode + string("',") +
            string("'") + lmountpoint + string("',") +
            string("'") + name + string("',") +
            string("'") + type + string("',") +
            string("'") + capacity + string("',") +
            string("'") + copyid + string("')");

    return ExecuteSql(SqlInsert);
}

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
bool DbDataManager::AddStrategy(string localserverid, string remoteserverid, string hcode, string lmountpoint, string name, string type, string isfullcopy, string iqn,
        string targetfile, string mountpoint, string capacity, string mirrorlunid, string mmlunid, string volumepairid, string lvn, string rvn, string lvoltag, string rvoltag,
        string usedforcdp, string cdpstate, string maxsnapshot, string usedfortimeout, string copytimeout, string cdpmode, string datasource, string deviceminor)
{
    string SqlString = string("INSERT INTO TBLCOPYSTRATEGY (ORIGINALSERVERID, SERVERID, HCODE, LMOUNTPOINT, STRATEGYNAME, STRATEGYTYPE, ISFULLCOPY, IQN, "
            "TARGETFILE, MOUNTPOINT, CAPACITY, MIRRORLUNID, MMLUNID, VOLUMEPAIRID, LVN, RVN, LVOLTAG, RVOLTAG,"
            "USEDFORCDP, CDPSTATE, MAXSNAPSHOT, USEDFORTIMEOUT, COPYTIMEOUT, CDPMODE, DATASOURCE, DEVICEMINOR) VALUES('") + localserverid + string("',") +
            string("'") + remoteserverid + string("',") +
            string("'") + hcode + string("',") +
            string("'") + lmountpoint + string("',") +
            string("'") + name + string("',") +
            string("'") + type + string("',") +
            isfullcopy + string(",") +
            string("'") + iqn + string("',") +
            string("'") + targetfile + string("',") +
            string("'") + mountpoint + string("',") +
            capacity + string(",") +
            string("'") + mirrorlunid + string("',") +
            string("'") + mmlunid + string("',") +
            string("'") + volumepairid + string("',") +
            string("'") + lvn + string("',") +
            string("'") + rvn + string("',") +
            string("'") + lvoltag + string("',") +
            string("'") + rvoltag + string("'") +
            string("'") + usedforcdp + string("'") +
            string("'") + cdpstate + string("'") +
            string("'") + maxsnapshot + string("'") +
            string("'") + usedfortimeout + string("'") +
            string("'") + copytimeout + string("'") +
            string("'") + cdpmode + string("'") +
            string("'") + datasource + string("'") +
            string("'") + deviceminor + string("'") +
            string(")");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::AddStrategyDetail(string localserverid, string remoteserverid, string strategyid, string scheduleid, string groupid, string operation, string foldername)
{
    string SqlString = string("INSERT INTO TBLCOPYDETAIL (ORIGINALSERVERID, SERVERID, STRATEGYID, SCHEDULEID, GROUPID, OPERATION, FOLDERNAME) VALUES ('") + localserverid + string("',") +
            string("'") + remoteserverid + string("',") +
            strategyid + string(",") +
            scheduleid + string(",") +
            groupid + string(",") +
            string("'") + operation + string("',") +
            string("'") + foldername + string("')");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除策略
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteStrategy(string hcode, string strategyid)
{
    string SqlDelete = string("DELETE FROM TBLCOPYSTRATEGY WHERE STRATEGYID =") + strategyid;
    return ExecuteSql(SqlDelete);
}

/*
 *  功能：
 *      删除策略集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteStrategySet(string hcode)
{
    string SqlDelete = string("DELETE FROM TBLCOPYSTRATEGY WHERE HCODE =") + hcode;
    return ExecuteSql(SqlDelete);
}

/*
 *  功能：
 *      删除策略明晰
 *  参数：
 *      strategyid      :   策略id
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteStrategyDetail(string strategyid)
{
    string SqlDelete = string("DELETE FROM TBLCOPYDETAIL WHERE STRATEGYID =") + strategyid;
    return ExecuteSql(SqlDelete);
}

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
bool DbDataManager::UpdateStrategy(string localserverid, string remoteserverid, string hcode, string lmountpoint, string name, string type, string isfullcopy, string iqn,
        string targetfile, string mountpoint, string capacity, string mirrorlunid, string mmlunid, string volumepairid, string lvn, string rvn, string lvoltag, string rvoltag,
        string usedforcdp, string cdpstate, string maxsnapshot, string usedfortimeout, string copytimeout, string cdpmode, string datasource, string deviceminor)
{
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET ORIGINALSERVERID = '") + localserverid + string("',") +
            string("SERVERID = '") + remoteserverid + string("',") +
            string("STRATEGYNAME = '") + name + string("',") +
            string("STRATEGYTYPE = '") + type + string("',") +
            string("ISFULLCOPY = ") + isfullcopy + string(",") +
            string("IQN = '") + iqn + string("',") +
            string("TARGETFILE = '") + targetfile + string("',") +
            string("MOUNTPOINT = '") + mountpoint + string("',") +
            string("CAPACITY = ") + capacity + string(",") +
            string("MIRRORLUNID = '") + mirrorlunid + string("', ") +
            string("MMLUNID = '") + mmlunid + string("',") +
            string("VOLUMEPAIRID = '") + volumepairid + string("',") +
            string("LVN = '") + lvn + string("',") +
            string("RVN = '") + rvn + string("',") +
            string("LVOLTAG = '") + lvoltag + string("',") +
            string("RVOLTAG = '") + rvoltag + string("',") +
            string("USEDFORCDP = '") + rvoltag + string("',") +
            string("CDPSTATE = '") + rvoltag + string("',") +
            string("MAXSNAPSHOT = '") + rvoltag + string("',") +
            string("USEDFORTIMEOUT = '") + rvoltag + string("',") +
            string("COPYTIMEOUT = '") + rvoltag + string("',") +
            string("CDPMODE = '") + rvoltag + string("',") +
            string("DATASOURCE = '") + rvoltag + string("',") +
            string("DEVICEMINOR = '") + rvoltag + string("'") +
            string(" WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT = '") + lmountpoint + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新策略
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略编号
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStrategyAndDetailServerId(string hcode, string strategyid)
{
    return UpdateStrategyAndDetailServerId(hcode, strategyid, GetLocalServerId(), GetLocalServerId());
}

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
bool DbDataManager::UpdateStrategyAndDetailServerId(string hcode, string strategyid, string localserverid, string remoteserverid)
{
    string SqlString = DEFAULT_EMPTY_STRING;
    vector<string> SqlList;
    string StrategyId = GetStrategyId(hcode, strategyid, DbDataManager::STRATEGY_ID_SEARCH_TYPE_LMOUNTPOINT);

    // 更新策略表
    SqlString = string("UPDATE TBLCOPYSTRATEGY SET ORIGINALSERVERID = '") + localserverid + string("', SERVERID = '") + remoteserverid + string("' WHERE ORIGINALSERVERID = '?' AND SERVERID = '?' AND STRATEGYID = ") + StrategyId;
    SqlList.push_back(SqlString);

    // 更新策略明细表
    SqlString = string("UPDATE TBLCOPYDETAIL SET ORIGINALSERVERID = '") + localserverid + string("', SERVERID = '") + remoteserverid + string("' WHERE ORIGINALSERVERID = '?' AND SERVERID = '?' AND STRATEGYID = ") + StrategyId;
    SqlList.push_back(SqlString);

    return ExecuteSql(SqlList);
}

/*
 *  功能：
 *      更新策略镜像卷id
 *  参数：
 *      strategyid      :   策略id
 *      volumeid        :   卷id
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStrategyMirrorVolumeId(string strategyid, string volumeid)
{
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET MIRRORLUNID = '") + volumeid + string("' WHERE STRATEGYID = ") + strategyid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新策略镜像卷的镜像卷id
 *  参数：
 *      strategyid      :   策略id
 *      volumeid        :   卷id
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStrategyMMVolumeId(string strategyid, string volumeid)
{
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET MMLUNID = '") + volumeid + string("' WHERE STRATEGYID = ") + strategyid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新策略卷对id
 *  参数：
 *      strategyid      :   策略id
 *      volumeid        :   卷对id
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStrategyVolumePairId(string strategyid, string pairid)
{
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET VOLUMEPAIRID = '") + pairid + string("' WHERE STRATEGYID = ") + strategyid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新策略存储空间id
 *  参数：
 *      strategyid      :   策略id
 *      storageid       :   存储空间id
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStrategyStorageId(string strategyid, string storageid)
{
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET STGID = ") + storageid + string(" WHERE STRATEGYID = ") + strategyid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新策略磁盘空间大小
 *  参数：
 *      strategyid      :   策略id
 *      newcapacity     :   新的磁盘大小
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStrategyVolumeCapacity(string strategyid, string newcapacity)
{
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET CAPACITY = ") + newcapacity + string(" WHERE STRATEGYID = ") + strategyid;
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新策略设备号
 *  参数：
 *      strategyid      :   策略id
 *      deviceminor     :   设备号
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStrategyDeviceMinor(string strategyid, string deviceminor)
{
    string SqlString = string("UPDATE TBLCOPYSTRATEGY SET DEVICEMINOR = ") + deviceminor + string(" WHERE STRATEGYID = ") + strategyid;
    return ExecuteSql(SqlString);
}

/*
sql::ResultSet * DbDataManager::GetStrategyResultSet()
{
    return GetStrategyResultSet(DEFAULT_EMPTY_STRING);
}
*/

/*
 *  功能：
 *      获取复制策略结果集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      复制策略结果集
 */
sql::ResultSet * DbDataManager::GetStrategyResultSet(string hcode)
{
    return GetStrategyResultSet(hcode, DbDataManager::STRATEGY_TYPE_UNKNOWN);
}

/*
 *  功能：
 *      根据策略类型获取策略结果集
 *  参数：
 *      hcode           :   主机编码
 *      strategytype    :   策略类型
 *  返回：
 *      复制策略结果集
 */
sql::ResultSet * DbDataManager::GetStrategyResultSet(string hcode, StrategyType strategytype)
{
    string TypeCode = "";

    switch (strategytype)
    {
        case DbDataManager::STRATEGY_TYPE_SYSTEM:
        {
            TypeCode = "S";
            break;
        }
        case DbDataManager::STRATEGY_TYPE_DATA:
        {
            TypeCode = "D";
            break;
        }
        case DbDataManager::STRATEGY_TYPE_RAC:
        {
            TypeCode = "R";
            break;
        }
        case DbDataManager::STRATEGY_TYPE_DB:
        {
            TypeCode = "B";
            break;
        }
        default:
        {
            break;
        }
    }

    string SqlString = string("SELECT STRATEGYID, HCODE, LMOUNTPOINT, STRATEGYNAME, STRATEGYTYPE, ISFULLCOPY, IQN, TARGETFILE, MOUNTPOINT, CAPACITY, SSCOPYID, TRUSTEDCOPYID, MIRRORLUNID, MMLUNID, VOLUMEPAIRID, STGID, CURRENTVOLUMETYPE, USEDFORCDP,USEDFORCDP, MAXSNAPSHOT,USEDFORTIMEOUT, COPYTIMEOUT, CDPMODE, DATASOURCE, DEVICEMINOR, LVN, RVN, LVOLTAG, RVOLTAG FROM TBLCOPYSTRATEGY");

    // 获取where子句
    string WhereClause = DEFAULT_EMPTY_STRING;
    if (BeGeneralLib::IsStringEmpty(hcode) == false)
    {
        WhereClause = string("HCODE = '") + hcode + string("'");
    }
    if (BeGeneralLib::IsStringEmpty(TypeCode) == false)
    {
        if (BeGeneralLib::IsStringEmpty(TypeCode) == false)
        {
            WhereClause = WhereClause + SEPARATOR_CHAR_SPACE + string("AND") + SEPARATOR_CHAR_SPACE;
        }
        WhereClause = WhereClause + string("STRATEGYTYPE ='") + TypeCode + string("'");
    }

    // 如果where子句不为空
    if (BeGeneralLib::IsStringEmpty(WhereClause) == false)
    {
        WhereClause = string("WHERE") + SEPARATOR_CHAR_SPACE + WhereClause;

        // 设置sql语句（带where）
        SqlString = SqlString + SEPARATOR_CHAR_SPACE + WhereClause;
    }
    //    cout << "Sql:" << SqlString << endl;
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取复制策略明细结果集
 *  参数：
 *      strategyid      :   策略id
 *  返回：
 *      复制策略明细结果集
 */
sql::ResultSet * DbDataManager::GetStrategyDetailResultSet(string strategyid)
{
    string SqlQuery = string("SELECT OPERATION, FOLDERNAME FROM TBLCOPYDETAIL WHERE STRATEGYID =") + strategyid;
    return GetResultSet(SqlQuery);
}

/*
 *  功能：
 *      获取复制策略明细结果集
 *  参数：
 *      strategyid      :   策略id
 *      scheduleid      :   调度计划id
 *  返回：
 *      复制策略明细结果集
 */
sql::ResultSet * DbDataManager::GetStrategyDetailResultSet(string strategyid, string scheduleid)
{
    string SqlQuery = string("SELECT OPERATION, FOLDERNAME FROM TBLCOPYDETAIL WHERE STRATEGYID =") + strategyid + string(" AND SCHEDULEID =") + scheduleid;
    return GetResultSet(SqlQuery);
}

/*
 *  功能：
 *      获取复制策略明细结果集
 *  参数：
 *      strategyid      :   策略id
 *      type            :   明细类型
 *  返回：
 *      复制策略明细结果集
 */
sql::ResultSet * DbDataManager::GetStrategyDetailResultSet(string strategyid, string scheduleid, DbDataManager::DetailType type)
{
    string Operation = "";
    if ((type & DbDataManager::DETAIL_TYPE_INCLUDE) == DbDataManager::DETAIL_TYPE_INCLUDE)
    {
        Operation = string(" OPERATION = '+' ");
    }
    if ((type & DbDataManager::DETAIL_TYPE_EXCLUDE) == DbDataManager::DETAIL_TYPE_EXCLUDE)
    {
        if (BeGeneralLib::IsStringEmpty(Operation) == false)
        {
            Operation = Operation + string(" OR ");
        }
        Operation = Operation + string(" OPERATION = '-' ");
    }
    if ((type & DbDataManager::DETAIL_TYPE_DATABASE) == DbDataManager::DETAIL_TYPE_DATABASE)
    {
        if (BeGeneralLib::IsStringEmpty(Operation) == false)
        {
            Operation = Operation + string(" OR ");
        }
        Operation = Operation + string(" OPERATION = 'D' ");
    }

    string SqlQuery = string("SELECT OPERATION, FOLDERNAME FROM TBLCOPYDETAIL WHERE STRATEGYID =") + strategyid + string(" AND SCHEDULEID =") + scheduleid;
    if (BeGeneralLib::IsStringEmpty(Operation) == false)
    {
        SqlQuery = SqlQuery + string(" AND ( ") + Operation + string(" )");
    }

    return GetResultSet(SqlQuery);
}

/*
 *  功能：
 *      获取复制策略明细结果集
 *  参数：
 *      strategyid      :   策略id
 *      type            :   明细类型
 *  返回：
 *      复制策略明细结果集
 */
sql::ResultSet * DbDataManager::GetStrategyDetailResultSet(string strategyid, DbDataManager::DetailType type)
{
    string Operation = "";
    if ((type & DbDataManager::DETAIL_TYPE_INCLUDE) == DbDataManager::DETAIL_TYPE_INCLUDE)
    {
        Operation = string(" OPERATION = '+' ");
    }
    if ((type & DbDataManager::DETAIL_TYPE_EXCLUDE) == DbDataManager::DETAIL_TYPE_EXCLUDE)
    {
        if (BeGeneralLib::IsStringEmpty(Operation) == false)
        {
            Operation = Operation + string(" OR ");
        }
        Operation = Operation + string(" OPERATION = '-' ");
    }
    if ((type & DbDataManager::DETAIL_TYPE_DATABASE) == DbDataManager::DETAIL_TYPE_DATABASE)
    {
        if (BeGeneralLib::IsStringEmpty(Operation) == false)
        {
            Operation = Operation + string(" OR ");
        }
        Operation = Operation + string(" OPERATION = 'D' ");
    }

    string SqlQuery = string("SELECT OPERATION, FOLDERNAME FROM TBLCOPYDETAIL WHERE STRATEGYID =") + strategyid;
    if (BeGeneralLib::IsStringEmpty(Operation) == false)
    {
        SqlQuery = SqlQuery + string(" AND ( ") + Operation + string(" )");
    }

    return GetResultSet(SqlQuery);
}

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
string DbDataManager::GetStrategyId(string hcode, string condition, DbDataManager::StrategyIdSearchType type)
{
    string SqlString = "";
    switch (type)
    {
        case DbDataManager::STRATEGY_ID_SEARCH_TYPE_LMOUNTPOINT: // 本地挂载点
        {
            if (BeGeneralLib::IsAllDigits(condition) == true)
            {
                return condition;
            }
            SqlString = string("SELECT STRATEGYID FROM TBLCOPYSTRATEGY WHERE HCODE = '") + hcode + string("' AND LMOUNTPOINT = '") + condition + string("'");
            break;
        }
        case DbDataManager::STRATEGY_ID_SEARCH_TYPE_SNAPSHOTID: // 快照id
        {
            SqlString = string("SELECT STRATEGYID FROM TBLCOPYSTRATEGY T, (SELECT LMOUNTPOINT FROM TBLSNAPSHOT WHERE SNAPSHOTID=") + condition + string(")V WHERE T.HCODE ='") + hcode + string("' AND T.LMOUNTPOINT = V.LMOUNTPOINT");
            break;
        }
    }
    return GetFieldValue(SqlString, "STRATEGYID");
}

/*
 *  功能：
 *      获取卷指定容量
 *  参数：
 *      hcode           :   主机编码
 *      volumesymbol    :   查找条件
 *  返回：
 *      卷指定容量
 */
string DbDataManager::GetVolumeCapacity(string hcode, string volumesymbol)
{
    return GetStrategyFieldValue(hcode, volumesymbol, DbDataManager::STRATEGY_TABLE_FIELD_CAPACITY);
}

/*
 *  功能：
 *      获取镜像卷id
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      镜像卷id
 */
string DbDataManager::GetMirrorLunId(string hcode, string strategyid)
{

    return GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_MIRRORLUNID);
}

/*
 *  功能：
 *      获取镜像卷的镜像卷id
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      镜像卷的镜像卷id
 */
string DbDataManager::GetMMLunId(string hcode, string strategyid)
{

    return GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_MMLUNID);
}

/*
 *  功能：
 *      获取卷对id
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      镜像卷对id
 */
string DbDataManager::GetVolumePairId(string hcode, string strategyid)
{

    return GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_VOLUMEPAIRID);
}

/*
 *  功能：
 *      获取策略的当前卷类型
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      当前卷类型
 */
DbDataManager::VolumeType DbDataManager::GetCurrentVolumeType(string hcode, string strategyid)
{
    return (DbDataManager::VolumeType)(BeGeneralLib::StringToInt(GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_CURRENTVOLUMETYPE)));
}

/*
 *  功能：
 *      获取策略存储路径
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      策略存储路径
 */
string DbDataManager::GetStrategyStoragePath(string hcode, string strategyid)
{
    return GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_STG_MOUNTPOINT);
}

/*
 *  功能：
 *      获取策略存储id
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      策略存储id
 */
string DbDataManager::GetStrategyStorageId(string hcode, string strategyid)
{

    return GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_STGID);
}

/*
 *  功能：
 *      获取策略存储设备
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      策略存储id
 */
string DbDataManager::GetStrategyStorageDevice(string hcode, string strategyid)
{

    return GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_STG_DEVICE);
}

/*
 *  功能：
 *      获取策略存储文件系统
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      策略存储文件系统
 */
string DbDataManager::GetStrategyStorageFileSystem(string hcode, string strategyid)
{
    return GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_STG_FSTYPE);
}

/*
 *  功能：
 *      获取策略最大快照数
 *  参数：
 *      hcode           :  主机编码
 *      strategyid      :  策略id
 *  返回：
 *      策略最大快照数
 */
int DbDataManager::GetStrategyMaxSnapshot(string hcode, string strategyid)
{
    return BeGeneralLib::StringToInt(GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_MAXSNAPSHOT));
}

/*
 *  功能：
 *      获取CDP个数
 *  参数：
 *      无
 *  返回：
 *      CDP个数
 */
int DbDataManager::GetCDPCount()
{
    return GetCDPCount("", "");
}

/*
 *  功能：
 *      获取CDP个数
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略编号
 *  返回：
 *      CDP个数
 */
int DbDataManager::GetCDPCount(string hcode, string strategyid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCOPYSTRATEGY WHERE USEDFORCDP = 1");
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == false && BeGeneralLib::IsStringEmptyOrInvalidValue(strategyid) == false)
    {
        string LMountPoint = GetHostMountPoint(hcode, strategyid);
        SqlString = SqlString + string(" AND (HCODE !='") + hcode + string("' AND LMOUNTPOINT != '") + LMountPoint + string("')");
    }
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      获取策略类型
 *  参数：
 *      hcode           :  主机编码
 *      strategyid      :  策略id
 *  返回：
 *      策略类型
 */
DbDataManager::StrategyType DbDataManager::GetStrategyType(string hcode, string strategyid)
{
    string StrategyType = GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_STRATEGYTYPE);
    if (StrategyType == "S")
    {
        return DbDataManager::STRATEGY_TYPE_SYSTEM;
    }
    else if (StrategyType == "R")
    {
        return DbDataManager::STRATEGY_TYPE_RAC;
    }
    else if (StrategyType == "B")
    {
        return DbDataManager::STRATEGY_TYPE_DB;
    }
    else
    {
        return DbDataManager::STRATEGY_TYPE_DATA;
    }
}

/*
 *  功能：
 *      获取策略复制超时时间（分钟）
 *  参数：
 *      hcode           :  主机编码
 *      strategyid      :  策略id
 *  返回：
 *      复制超时时间
 */
int DbDataManager::GetStrategyCopyTimeout(string hcode, string strategyid)
{
    return BeGeneralLib::StringToInt(GetStrategyFieldValue(hcode, strategyid, DbDataManager::STRATEGY_TABLE_FIELD_COPYTIMEOUT));
}

/*
 *  功能：
 *      策略是否存在
 *  参数
 *      hcode           :   主机编码
 *      strategyid      :   策略ID
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsStrategyExist(string hcode, string strategyid)
{
    string LMountPoint = GetHostMountPoint(hcode, strategyid);
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCOPYSTRATEGY WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查卷的策略是否存在
 *  参数
 *      hcode           :   主机编码
 *      localmountpoint :   策略ID
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsVolumeHasStrategy(string hcode, string localmountpoint)
{
    return IsStrategyExist(hcode, localmountpoint);
}

/*
 *  功能：
 *      检查策略是否存在数据库目录
 *  参数
 *      strategyid      :   策略ID
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsStrategyHasDb(string strategyid)
{
    return IsStrategyHasDb(strategyid, "");
}

/*
 *  功能：
 *      检查策略是否存在数据库目录
 *  参数
 *      strategyid      :   策略ID
 *      scheduleid      :   调度计划id
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsStrategyHasDb(string strategyid, string scheduleid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCOPYDETAIL WHERE STRATEGYID = ") + strategyid + string(" AND OPERATION = 'D'");
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(scheduleid) == false)
    {
        SqlString = SqlString + SEPARATOR_CHAR_SPACE + string(" AND SCHEDULEID = ") + scheduleid;
    }
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查策略是否存在全部复制操作
 *  参数
 *      strategyid      :   策略ID
 *      scheduleid      :   调度计划id
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsStrategyHasFull(string strategyid, string scheduleid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCOPYDETAIL WHERE STRATEGYID = ") + strategyid + string(" AND SCHEDULEID = ") + scheduleid + string(" AND OPERATION = '*'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查策略是否存在包含目录
 *  参数
 *      strategyid      :   策略ID
 *      scheduleid      :   调度计划id
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsStrategyHasInclude(string strategyid, string scheduleid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCOPYDETAIL WHERE STRATEGYID = ") + strategyid + string(" AND SCHEDULEID = ") + scheduleid + string(" AND OPERATION = '+'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查策略是否为rac
 *  参数
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsStrategyRac(string hcode, string strategyid)
{
    return (GetStrategyType(hcode, strategyid) == DbDataManager::STRATEGY_TYPE_RAC);
}

/*
 *  功能：
 *      检查策略是否用于复制超时
 *  参数
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsStrategyUsedForTimeout(string hcode, string strategyid)
{
    string LMountPoint = GetHostMountPoint(hcode, strategyid);
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCOPYSTRATEGY WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("' AND USEDFORTIMEOUT = 1");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      是否为该存储上的最后一个策略
 *  参数
 *      hcode           :   主机编码
 *      strategyid      :   策略id
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsFinalStrategyInStorage(string hcode, string strategyid)
{
    string LMountPoint = GetHostMountPoint(hcode, strategyid);
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCOPYSTRATEGY T, (SELECT STGID FROM TBLCOPYSTRATEGY WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT= '") + LMountPoint + string("') S WHERE T.HCODE ='") + hcode + string("' AND T.LMOUNTPOINT != '") + LMountPoint + string("' AND S.STGID = T.STGID");
    return (GetFieldValueBool(SqlString, "RECCOUNT") == false);
}

/*
 *  功能：
 *      检查策略是否有副本
 *  参数
 *      hcode           :   主机编码
 *      strategyid      :   策略ID
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsStrategyHasCopy(string hcode, string strategyid)
{
    string LMountPoint = GetHostMountPoint(hcode, strategyid);
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOTCOPY WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT = '") + LMountPoint + string("'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================== 主机相关 ==================================//

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
bool DbDataManager::AddHost(string hcode, string name, string ipaddr, string ostype, string netbootprotocol, string netdefaultprotocol, string macaddr, string wwn, string port, string businessnicname,
        string brnicname, string storageid, string copymode, string usedfornet, string usedforvm, string usedforfc, string racgroupid, string dbprotected, string fcname, string vncnumber)
{
    string SqlInsert = string("INSERT INTO TBLHOST (ORIGINALSERVERID, SERVERID, HCODE, HOSTNAME, IPADDR, OSTYPE, NETBOOTPROTOCOL, NETDEFAULTPROTOCOL, MACADDR, FCWWN, LISTENPORT, BUSINESSNICNAME, BRNICNAME, STGID, COPYMODE, USEDFORNET, USEDFORVM, USEDFORFC, RACGROUPID, DBPROTECTED, FCNAME, VNCNUMBER, CREATETIME, UPDATETIME) VALUES('") + GetLocalServerId() + string("',") +
            string("'") + GetLocalServerId() + string("',") +
            string("'") + hcode + string("',") +
            string("'") + name + string("',") +
            string("'") + ipaddr + string("',") +
            string("'") + ostype + string("',") +
            string("'") + netbootprotocol + string("',") +
            string("'") + netdefaultprotocol + string("',") +
            string("'") + macaddr + string("',") +
            string("'") + wwn + string("',") +
            string("'") + port + string("',") +
            string("'") + businessnicname + string("',") +
            string("'") + brnicname + string("',") +
            storageid + string(",") +
            copymode + string(",") +
            usedfornet + string(",") +
            usedforvm + string(",") +
            usedforfc + string(",") +
            racgroupid + string(",") +
            dbprotected + string(",") +
            string("'") + fcname + string("',") +
            vncnumber + string(",") +
            string("NOW(), NOW())");

    return ExecuteSql(SqlInsert);
}

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
bool DbDataManager::AddHost(string localserverid, string remoteserverid, string hcode, string name, string macaddr, string ipaddr, string bootfile, string status,
        string isactive, string hdnum, string usingrhost, string rhostmac, string hbaipaddr, string isnetboot, string netdiskcapacity, string dbprotected, string ostype,
        string netbootprotocol, string netdefaultprotocol, string netbootservice, string fcwwn, string systemdir, string configdir, string vmmemory, string vmcpu, string listenport,
        string usedforrdr, string usedforle, string businessnicname, string brnicname, string storageid, string copymode, string syncflag, string fcname, string fcchannel,
        string initiatorname, string usedfornet, string usedforvm, string usedforfc, string cdpstate, string usedforea, string eamode, string heartbeattimeout,
        string queryinterval, string virtualip, string virtualmac, string racgroupid, string emergencylogonmode, string usedformonitor, string isautotakeover, string vmbootcopymode, string vncnumber)
{
    string SqlString = string("INSERT INTO TBLHOST(ORIGINALSERVERID, SERVERID, HCODE, HOSTNAME, MACADDR, IPADDR, BOOTFILE, STATUS, ISACTIVE, HDNUM, USINGRHOST, RHOSTMAC, HBAIPADDR, ISNETBOOT, NETDISKCAPACITY, "
            "DBPROTECTED, OSTYPE, NETBOOTPROTOCOL, NETDEFAULTPROTOCOL, NETBOOTSERVICE, FCWWN, SYSTEMDIR, CONFIGDIR, VMMEMORY, VMCPU, LISTENPORT, USEDFORRDR, USEDFORLE, BUSINESSNICNAME, BRNICNAME, STGID, COPYMODE, SYNCFLAG, "
            "FCNAME, FCCHANNEL, INITIATORNAME, USEDFORNET, USEDFORVM, USEDFORFC, CDPSTATE, USEDFOREA, EAMODE, HEARTBEATTIMEOUT, QUERYINTERVAL, VIRTUALIP, VIRTUALMAC, RACGROUPID,EMERGENCYLOGONMODE,"
            "USEDFORMONITOR, ISAUTOTAKEOVER, VMBOOTCOPYMODE, VNCNUMBER, CREATETIME, UPDATETIME) VALUES(") +
            string("'") + localserverid + string("',") +
            string("'") + remoteserverid + string("',") +
            string("'") + hcode + string("',") +
            string("'") + name + string("',") +
            string("'") + macaddr + string("',") +
            string("'") + ipaddr + string("',") +
            string("'") + bootfile + string("',") +
            status + string(",") +
            isactive + string(",") +
            hdnum + string(",") +
            usingrhost + string(",") +
            string("'") + rhostmac + string("',") +
            string("'") + hbaipaddr + string("',") +
            isnetboot + string(",") +
            netdiskcapacity + string(",") +
            dbprotected + string(",") +
            string("'") + ostype + string("',") +
            string("'") + netbootprotocol + string("',") +
            string("'") + netdefaultprotocol + string("',") +
            netbootservice + string(",") +
            string("'") + fcwwn + string("',") +
            string("'") + systemdir + string("',") +
            string("'") + configdir + string("',") +
            string("'") + vmmemory + string("',") +
            string("'") + vmcpu + string("',") +
            listenport + string(",") +
            usedforrdr + string(",") +
            usedforle + string(",") +
            string("'") + businessnicname + string("',") +
            string("'") + brnicname + string("',") +
            storageid + string(",") +
            copymode + string(",") +
            syncflag + string(",") +
            string("'") + fcname + string("',") +
            fcchannel + string(",") +
            string("'") + initiatorname + string("',") +
            usedfornet + string(",") +
            usedforvm + string(",") +
            usedforfc + string(",") +
            cdpstate + string(",") +
            usedforea + string(",") +
            eamode + string(",") +
            heartbeattimeout + string(",") +
            queryinterval + string(",") +
            string("'") + virtualip + string("',") +
            string("'") + virtualmac + string("',") +
            racgroupid + string(",") +
            emergencylogonmode + string(",") +
            usedformonitor + string(",") +
            isautotakeover + string(",") +
            vmbootcopymode + string(",") +
            vncnumber + string(",") +
            string("NOW(), NOW())");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      增加主机网卡
 *  参数：
 *      originalserverid:   本地服务器id
 *      remoteserverid  :   远程服务器id
 *      hcode           :   主机编码
 *      mac             :   mac地址
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
bool DbDataManager::AddHostNic(string originalserverid, string remoteserverid, string hcode, string macaddr, string devicename, string caption, string gateway, string maindns, string subdns,
        string dhcpenabled, string isphysical, string status, string workmode, string usedforvm, string businessnic, string bridgenic, string virtualnetworkmode, string virtualmac)
{
    string SqlString = string("INSERT INTO TBLHOSTNIC(ORIGINALSERVERID, SERVERID, HCODE, MACADDRESS, DEVICENAME, CAPTION, GATEWAY, MAINDNS, SUBDNS, DHCPENABLED, ISPHYSICALADAPTER, STATUS, WORKMODE, USEDFORVM, BUSINESSNICNAME, BRNICNAME, VIRTUALNETWORKMODE, VIRTUALMAC, CREATETIME, UPDATETIME) VALUES(") +
            string("'") + originalserverid + string("',") +
            string("'") + remoteserverid + string("',") +
            string("'") + hcode + string("',") +
            string("'") + macaddr + string("',") +
            string("'") + devicename + string("',") +
            string("'") + caption + string("',") +
            string("'") + gateway + string("',") +
            string("'") + maindns + string("',") +
            string("'") + subdns + string("',") +
            dhcpenabled + string(",") +
            isphysical + string(",") +
            status + string(",") +
            workmode + string(",") +
            usedforvm + string(",") +
            string("'") + businessnic + string("',") +
            string("'") + bridgenic + string("',") +
            virtualnetworkmode + string(",") +
            string("'") + virtualmac + string("',") +
            string("NOW(), NOW())");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::AddHostIpAddress(string originalserverid, string remoteserverid, string hcode, string macaddr, string ip, string netmask, string ipv6, string netmaskv6, string workmode)
{
    string SqlString = string("INSERT INTO TBLHOSTIPADDRESS(ORIGINALSERVERID, SERVERID, HCODE, MACADDRESS, IPADDRESS, NETMASK, IPV6ADDRESS, IPV6NETMASK, WORKMODE, CREATETIME, UPDATETIME) VALUES(") +
            string("'") + originalserverid + string("',") +
            string("'") + remoteserverid + string("',") +
            string("'") + hcode + string("',") +
            string("'") + macaddr + string("',") +
            string("'") + ip + string("',") +
            string("'") + netmask + string("',") +
            string("'") + ipv6 + string("',") +
            string("'") + netmaskv6 + string("',") +
            workmode + string(",") +
            string("NOW(), NOW())");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除主机记录
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteHost(string hcode)
{
    string SqlDelete = string("DELETE FROM TBLHOST WHERE HCODE ='") + hcode + string("'");
    return ExecuteSql(SqlDelete);
}

/*
 *  功能：
 *      删除主机相关记录
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteHostRelativeRecord(string hcode)
{
    vector<string> SqlList;

    // 删除该主机已经完成的任务
    string SqlString = string("DELETE FROM TBL_TASK WHERE HCODE ='") + hcode + string("' AND STATUS = 3");
    SqlList.push_back(SqlString);

    // 删除该主机已经完成的远程任务
    SqlString = string("DELETE FROM TBLREMOTETASK WHERE HCODE ='") + hcode + string("' AND STATUS = 3");
    SqlList.push_back(SqlString);

    // 删除卷初始信息
    SqlString = string("DELETE FROM TBLVOLUMEINIT WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除卷信息表
    SqlString = string("DELETE FROM TBLVOLUME WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    SqlString = string("DELETE FROM TBLVOLUMEINIT WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    SqlString = string("DELETE FROM TBLVOLUMEMIRROR WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 监控接管相关
    SqlString = string("DELETE FROM TBLALARMRULE WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    SqlString = string("DELETE FROM TBLHOSTPORT WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    SqlString = string("DELETE FROM TBLHOSTSERVICE WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    SqlString = string("DELETE FROM TBLHOSTPROCESS WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除网卡信息
    SqlString = string("DELETE FROM TBLHOSTNIC WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除ip信息
    SqlString = string("DELETE FROM TBLHOSTIPADDRESS WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除关联的硬件应急配置
    SqlString = string("DELETE FROM TBLHWEHOST WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除报警
    SqlString = string("DELETE FROM TBLALARM WHERE HCODE = '") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除日志
    SqlString = string("DELETE FROM TBLBESLOG WHERE HCODE = '") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除主机软件版本
    SqlString = string("DELETE FROM TBLSOFTWAREVERSION WHERE SERVERCODE = '") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除虚拟主机
    SqlString = string("DELETE FROM TBLVMHOST WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 删除主机模板
    SqlString = string("DELETE FROM TBLHOSTTEMPLATE WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 主机应急信息表
    SqlString = string("DELETE FROM TBLHOSTEMERGENCYINFO WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 主机心跳监测项表
    SqlString = string("DELETE FROM TBLHOSTHBMONITORITEM WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 主机数据库表记录
    SqlString = string("DELETE FROM TBLHOSTDBTABLES WHERE DBID IN ( SELECT DBID FROM TBLHOSTDBSET WHERE HCODE ='") + hcode + string("')");
    SqlList.push_back(SqlString);

    // 主机数据库集
    SqlString = string("DELETE FROM TBLHOSTDBSET WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    SqlString = string("DELETE FROM TBLALARMRULE WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    SqlString = string("DELETE FROM TBLHOSTSERVICE WHERE HCODE ='") + hcode + string("'");
    SqlList.push_back(SqlString);

    // 执行sql
    return ExecuteSql(SqlList);
}

/*
 *  功能：
 *      删除主机记录集
 *  参数：
 *      无
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteHostSet()
{
    string SqlDelete = string("DELETE FROM TBLHOST");
    return ExecuteSql(SqlDelete);
}

/*
 *  功能：
 *      更新主机信息
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHost(string hcode)
{
    string SqlString = string("UPDATE TBLHOST SET MACADDR='?', ISACTIVE=0, HDNUM=0, USEDFOREA=0, USEDFORMONITOR=0, UPDATETIME=NOW() WHERE HCODE = '") + hcode + string("'");
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateHost(string hcode, string mac, string hdnum)
{
    string SqlString = string("UPDATE TBLHOST SET MACADDR = '") + mac + string("', HDNUM = ") + hdnum + string(", UPDATETIME = NOW() WHERE HCODE = '") + hcode + string("'");
    return ExecuteSql(SqlString);
}

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
 *      增加成功返回true，否则返回false
 */
bool DbDataManager::UpdateHost(string localserverid, string remoteserverid, string hcode, string name, string macaddr, string ipaddr, string bootfile, string status,
        string isactive, string hdnum, string usingrhost, string rhostmac, string hbaipaddr, string isnetboot, string netdiskcapacity, string dbprotected, string ostype,
        string netbootprotocol, string netdefaultprotocol, string netbootservice, string fcwwn, string systemdir, string configdir, string vmmemory, string vmcpu, string listenport,
        string usedforrdr, string usedforle, string businessnicname, string brnicname, string storageid, string copymode, string syncflag, string fcname, string fcchannel,
        string initiatorname, string usedfornet, string usedforvm, string usedforfc, string cdpstate, string usedforea, string eamode, string heartbeattimeout,
        string queryinterval, string virtualip, string virtualmac, string racgroupid, string emergencylogonmode, string usedformonitor, string isautotakeover, string vmbootcopymode, string vncnumber)
{
    string SqlString = string("UPDATE TBLHOST SET ORIGINALSERVERID ='") + localserverid + string("',") +
            string("SERVERID = '") + remoteserverid + string("',") +
            string("HOSTNAME = '") + name + string("',") +
            string("MACADDR = '") + macaddr + string("',") +
            string("IPADDR ='") + ipaddr + string("',") +
            string("BOOTFILE ='") + bootfile + string("',") +
            string("STATUS = ") + status + string(",") +
            string("ISACTIVE = ") + isactive + string(",") +
            string("HDNUM =") + hdnum + string(", ") +
            string("USINGRHOST = ") + usingrhost + string(",") +
            string("RHOSTMAC ='") + rhostmac + string("',") +
            string("HBAIPADDR = '") + hbaipaddr + string("',") +
            string("ISNETBOOT = ") + isnetboot + string(",") +
            string("NETDISKCAPACITY = ") + netdiskcapacity + string(",") +
            string("DBPROTECTED = ") + dbprotected + string(",") +
            string("OSTYPE ='") + ostype + string("',") +
            string("NETBOOTPROTOCOL ='") + netbootprotocol + string("',") +
            string("NETDEFAULTPROTOCOL ='") + netdefaultprotocol + string("',") +
            string("NETBOOTSERVICE =") + netbootservice + string(",") +
            string("FCWWN = '") + fcwwn + string("',") +
            string("SYSTEMDIR ='") + BeGeneralLib::EscapeString(systemdir) + string("',") +
            string("CONFIGDIR ='") + configdir + string("',") +
            string("VMMEMORY ='") + vmmemory + string("',") +
            string("VMCPU ='") + vmcpu + string("',") +
            string("LISTENPORT =") + listenport + string(", ") +
            string("USEDFORRDR = ") + usedforrdr + string(", ") +
            string("USEDFORLE =") + usedforle + string(",") +
            string("BUSINESSNICNAME = '") + businessnicname + string("',") +
            string("BRNICNAME = '") + brnicname + string("', ") +
            string("STGID = ") + storageid + string(", ") +
            string("COPYMODE = ") + copymode + string(",") +
            string("SYNCFLAG = ") + syncflag + string(",") +
            string("FCNAME = '") + fcname + string("',") +
            string("FCCHANNEL = ") + fcchannel + string(",") +
            string("INITIATORNAME = '") + initiatorname + string("',") +
            string("USEDFORNET = ") + usedfornet + string(",") +
            string("USEDFORVM = ") + usedforvm + string(",") +
            string("USEDFORFC = ") + usedforfc + string(",") +
            string("CDPSTATE = ") + cdpstate + string(",") +
            string("USEDFOREA = ") + usedforea + string(",") +
            string("EAMODE = ") + eamode + string(",") +
            string("HEARTBEATTIMEOUT = ") + heartbeattimeout + string(",") +
            string("QUERYINTERVAL = ") + queryinterval + string(",") +
            string("VIRTUALIP = '") + virtualip + string("',") +
            string("VIRTUALMAC = '") + virtualmac + string("',") +
            string("RACGROUPID = ") + racgroupid + string(",") +
            string("EMERGENCYLOGONMODE = ") + emergencylogonmode + string(",") +
            string("USEDFORMONITOR = ") + usedformonitor + string(",") +
            string("ISAUTOTAKEOVER = ") + isautotakeover + string(",") +
            string("VMBOOTCOPYMODE = ") + vmbootcopymode + string(",") +
            string("VNCNUMBER = ") + vncnumber + string(",") +
            string("UPDATETIME = NOW() WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);

}

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
bool DbDataManager::UpdateHostNic(string originalserverid, string remoteserverid, string hcode, string macaddr, string devicename, string caption, string gateway, string maindns, string subdns,
        string dhcpenabled, string isphysical, string status, string workmode, string usedforvm, string businessnic, string bridgenic, string virtualnetworkmode, string virtualmac)
{
    string SqlString = string("UPDATE TBLHOSTNIC SET ORIGINALSERVERID ='") + originalserverid + string("',") +
            string("SERVERID = '") + remoteserverid + string("',") +
            string("DEVICENAME ='") + devicename + string("',") +
            string("CAPTION = '") + caption + string("',") +
            string("GATEWAY = '") + gateway + string("',") +
            string("MAINDNS = '") + maindns + string("', ") +
            string("SUBDNS = '") + subdns + string("',") +
            string("DHCPENABLED = ") + dhcpenabled + string(",") +
            string("ISPHYSICALADAPTER = ") + isphysical + string(",") +
            string("STATUS = ") + status + string(",") +
            string("WORKMODE = ") + workmode + string(",") +
            string("USEDFORVM = ") + usedforvm + string(",") +
            string("BUSINESSNICNAME = '") + businessnic + string("',") +
            string("BRNICNAME = '") + bridgenic + string("',") +
            string("VIRTUALNETWORKMODE = ") + virtualnetworkmode + string(",") +
            string("VIRTUALMAC = '") + virtualmac + string("',") +
            string("UPDATETIME = NOW() WHERE HCODE ='") + hcode + string("' AND MACADDRESS = '") + macaddr + string("'");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateHostIpAddress(string originalserverid, string remoteserverid, string hcode, string macaddr, string ip, string netmask, string ipv6, string netmaskv6, string workmode)
{
    string SqlString = string("UPDATE TBLHOSTIPADDRESS SET ORIGINALSERVERID ='") + originalserverid + string("',") +
            string("SERVERID = '") + remoteserverid + string("',") +
            string("IPADDRESS ='") + ip + string("',") +
            string("NETMASK = '") + netmask + string("',") +
            string("IPV6ADDRESS = '") + ipv6 + string("',") +
            string("IPV6NETMASK = '") + netmaskv6 + string("', ") +
            string("WORKMODE = ") + workmode + string(",") +
            string("UPDATETIME = NOW() WHERE HCODE ='") + hcode + string("' AND MACADDRESS = '") + macaddr + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机状态
 *  参数：
 *      无
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostStatus()
{
    string SqlString = string("UPDATE TBLHOST SET ISACTIVE = 0 WHERE ISACTIVE != 0 && ISACTIVE != 999");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机在线状态
 *  参数：
 *      hcode           :   主机编码
 *      status          :   主机状态
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostOnlineStatus(string hcode, int status)
{
    string SqlString = string("UPDATE TBLHOST SET STATUS = ") + BeGeneralLib::Number2String(status) + string(" WHERE HCODE ='") + hcode + string("'");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机状态
 *  参数：
 *      hcode           :   主机编码
 *      status          :   主机状态
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostCanEmergencyStatus(string hcode, DbDataManager::HostCanEmergencyStatus status)
{
    string SqlString = string("UPDATE TBLHOST SET ISACTIVE = ") + BeGeneralLib::Number2String(status) + string(" WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机保护状态
 *  参数：
 *      hcode           :   主机编码
 *      status          :   主机状态
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostProtectedStatus(string hcode, DbDataManager::HostProtectedStatus status)
{
    string SqlString = string("UPDATE TBLHOST SET STATUS = ") + BeGeneralLib::Number2String(status) + string(" WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机启动协议
 *  参数：
 *      hcode           :   主机编码
 *      protocol        :   网启协议
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostBootProtocol(string hcode, string protocol)
{
    string SqlString = string("UPDATE TBLHOST SET NETBOOTPROTOCOL ='") + protocol + string("' WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机监听端口
 *  参数：
 *      hcode           :   主机编码
 *      port            :   端口号
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostPort(string hcode, string port)
{
    string SqlString = string("UPDATE TBLHOST SET LISTENPORT = ") + port + string(" WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机本地应急状态
 *  参数：
 *      hcode           :   主机编码
 *      status          :   本地应急状态
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostLocalEmergencyStatus(string hcode, string status)
{
    string SqlString = string("UPDATE TBLHOST SET USEDFORLE = ") + status + SEPARATOR_CHAR_SPACE + string("WHERE HCODE = '") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机业务网卡
 *  参数：
 *      hcode           :   主机编码
 *      nicname         :   网卡名
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostBusinessNic(string hcode, string nicname)
{
    string SqlString = string("UPDATE TBLHOST SET BUSINESSNICNAME = '") + nicname + string("' WHERE HCODE = '") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机桥接网卡
 *  参数：
 *      hcode           :   主机编码
 *      nicname         :   网卡名
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostBridgeNic(string hcode, string nicname)
{
    string SqlString = string("UPDATE TBLHOST SET BRNICNAME = '") + nicname + string("' WHERE HCODE = '") + hcode + string("'");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateHostReservedIp(string hcode, bool isusedreserved, string ipaddr)
{
    string SqlString = string("UPDATE TBLHOST SET USINGRHOST = ") + BeGeneralLib::Number2String((int) (isusedreserved)) + string(", HBAIPADDR ='") + ipaddr + string("' WHERE HCODE = '") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机mac地址
 *  参数：
 *      hcode           :   主机编码
 *      mac             :   mac地址
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostMacAddress(string hcode, string mac)
{
    vector<string> SqlList;

    // 获取原有的mac地址
    string OldMacAddress = GetHostMac(hcode);

    // 更新主机网卡表
    string SqlString = string("UPDATE TBLHOSTNIC SET MACADDRESS ='") + mac + string("' WHERE HCODE = '") + hcode + string("' AND MACADDRESS = '") + OldMacAddress + string("'");
    SqlList.push_back(SqlString);

    // 更新主机ip地址表
    SqlString = string("UPDATE TBLHOSTIPADDRESS SET MACADDRESS ='") + mac + string("' WHERE HCODE = '") + hcode + string("' AND MACADDRESS = '") + OldMacAddress + string("'");
    SqlList.push_back(SqlString);

    // 更新主机表
    SqlString = string("UPDATE TBLHOST SET MACADDR ='") + mac + string("' WHERE HCODE = '") + hcode + string("'");
    SqlList.push_back(SqlString);

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机自动应急状态
 *  参数：
 *      hcode           :   主机编码
 *      status          :   自动应急状态
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostEAStatus(string hcode, string status)
{
    string SqlString = string("UPDATE TBLHOST SET USEDFOREA = ") + status + SEPARATOR_CHAR_SPACE + string("WHERE HCODE = '") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机虚拟mac地址
 *  参数：
 *      hcode           :   主机编码
 *      virtualmac      :   虚拟mac地址
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostVirtualMac(string hcode, string virtualmac)
{
    string SqlString = string("UPDATE TBLHOST SET VIRTUALMAC = '") + virtualmac + string("' WHERE HCODE = '") + hcode + string("'");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateHostNicVirtualMac(string hcode, string mac, string virtualmac)
{
    string SqlString = string("UPDATE TBLHOSTNIC SET VIRTUALMAC = '") + virtualmac + string("' WHERE HCODE = '") + hcode + string("' AND MACADDRESS ='") + mac + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机网卡的业务网卡
 *  参数：
 *      hcode           :   主机编码
 *      nicname         :   网卡
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostNicBussinessNicName(string hcode, string nicname)
{
    return UpdateHostNicBussinessNicName(hcode, DEFAULT_EMPTY_STRING, nicname);
}

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
bool DbDataManager::UpdateHostNicBussinessNicName(string hcode, string mac, string nicname)
{
    string SqlString = string("UPDATE TBLHOSTNIC SET BUSINESSNICNAME ='") + nicname + string("' WHERE HCODE = '") + hcode + string("'");
    if (BeGeneralLib::IsStringEmpty(mac) == false)
    {
        SqlString = SqlString + SEPARATOR_CHAR_SPACE + string("AND MACADDRESS ='") + mac + string("'");
    }
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机网卡的桥接网卡
 *  参数：
 *      hcode           :   主机编码
 *      nicname         :   网卡
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostNicBridgeNicName(string hcode, string nicname)
{
    return UpdateHostNicBridgeNicName(hcode, DEFAULT_EMPTY_STRING, nicname);
}

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
bool DbDataManager::UpdateHostNicBridgeNicName(string hcode, string mac, string nicname)
{
    string SqlString = string("UPDATE TBLHOSTNIC SET BRNICNAME ='") + nicname + string("' WHERE HCODE = '") + hcode + string("'");
    if (BeGeneralLib::IsStringEmpty(mac) == false)
    {
        SqlString = SqlString + SEPARATOR_CHAR_SPACE + string("AND MACADDRESS ='") + mac + string("'");
    }
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateHostNicUsedForVm(string hcode, string mac, bool usedforvm)
{
    string SqlString = string("UPDATE TBLHOSTNIC SET USEDFORVM = ") + BeGeneralLib::Number2String((int) usedforvm) + string(" WHERE HCODE = '") + hcode + string("' AND MACADDRESS ='") + mac + string("'");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateHostVirtualNetworkMode(string hcode, string mac, VirtualNetworkMode mode)
{
    string SqlString = string("UPDATE TBLHOSTNIC SET VIRTUALNETWORKMODE = '") + BeGeneralLib::Number2String((int) mode) + string("' WHERE HCODE = '") + hcode + string("' AND MACADDRESS ='") + mac + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机复制方式
 *  参数：
 *      hcode           :   主机编码
 *      copymode        :   mac地址
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostCopyMode(string hcode, DbDataManager::HostCopyMode copymode)
{
    string SqlString = string("UPDATE TBLHOST SET COPYMODE = ") + BeGeneralLib::Number2String(copymode) + SEPARATOR_CHAR_SPACE + string("WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机光纤名
 *  参数：
 *      hcode           :   主机编码
 *      fcname          :   光纤名
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostFc(string hcode, string fcname)
{
    string SqlString = string("UPDATE TBLHOST SET FCNAME = '") + fcname + string("' WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateHostFc(string hcode, string fcname, string wwn)
{
    string SqlString = string("UPDATE TBLHOST SET FCNAME = '") + fcname + string("', FCWWN ='") + wwn + string("' WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机虚拟机启动副本模式
 *  参数：
 *      hcode           :   主机编码
 *      mode            :   启动副本模式
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostVmBootCopyMode(string hcode, DbDataManager::VmBootCopy mode)
{
    string SqlString = string("UPDATE TBLHOST SET VMBOOTCOPYMODE = '") + BeGeneralLib::Number2String(mode) + string("' WHERE HCODE ='") + hcode + string("'");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新主机的vnc号
 *  参数：
 *      hcode           :   主机编码
 *      vncnumber       :   vnc号
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostVncNumber(string hcode, int vncnumber)
{
    string SqlString = string("UPDATE TBLHOST SET VNCNUMBER = '") + BeGeneralLib::Number2String(vncnumber) + string("' WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      通知主机打开或关闭监控
 *  参数：
 *      hcode           :   主机编码
 *      isopen          :   是否打开
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateHostUsedForMonitor(string hcode, bool isopen)
{
    string SqlString = string("UPDATE TBLHOST SET USEDFORMONITOR ='") + BeGeneralLib::Number2String((int) isopen) + string("' WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      清除主机网启应急功能
 *  参数：
 *      无
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::ClearHostNetEmergency()
{
    string SqlString = string("UPDATE TBLHOST SET USEDFORNET = 0");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      清除主机虚拟机应急功能
 *  参数：
 *      无
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::ClearHostVmEmergency()
{
    string SqlString = string("UPDATE TBLHOST SET USEDFORVM = 0");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      清除主机光纤卡应急功能
 *  参数：
 *      无
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::ClearHostFcEmergency()
{
    string SqlString = string("UPDATE TBLHOST SET USEDFORFC = 0");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      清除主机数据库保护功能
 *  参数：
 *      无
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::ClearHostDbProtected()
{
    string SqlString = string("UPDATE TBLHOST SET DBPROTECTED = 0");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取主机结果集
 *  参数：
 *      无
 *  返回：
 *      主机结果集
 */
sql::ResultSet * DbDataManager::GetHostResultSet()
{

    return GetHostResultSet("", DbDataManager::HOST_RESULTSET_RETRIEVE_TYPE_ALL);
}

/*
 *  功能：
 *      获取主机结果集
 *  参数：
 *      nicname         :   网卡名
 *  返回：
 *      主机结果集
 */
sql::ResultSet * DbDataManager::GetHostResultSet(string nicname)
{

    return GetHostResultSet(nicname, DbDataManager::HOST_RESULTSET_RETRIEVE_TYPE_BUSINESS_NIC);
}

/*
 *  功能：
 *      获取主机结果集
 *  参数：
 *      searchvalue     :   查找值
 *      searchtype      :   查找类型
 *  返回：
 *      主机结果集
 */
sql::ResultSet * DbDataManager::GetHostResultSet(string searchvalue, DbDataManager::HostResultSetRetrieveType searchtype)
{
    string SqlString = "";
    string Condition = "";
    switch (searchtype)
    {
        case DbDataManager::HOST_RESULTSET_RETRIEVE_TYPE_BUSINESS_NIC:
        {
            Condition = string("BUSINESSNICNAME = BINARY '") + searchvalue + string("'");
            break;
        }
        case DbDataManager::HOST_RESULTSET_RETRIEVE_TYPE_ORIGINAL_SERVER_ID:
        {
            Condition = string("ORIGINALSERVERID = BINARY '") + searchvalue + string("'");
            break;
        }
        case DbDataManager::HOST_RESULTSET_RETRIEVE_TYPE_SERVER_ID:
        {
            Condition = string("SERVERID = '") + searchvalue + string("'");
            break;
        }
        case DbDataManager::HOST_RESULTSET_RETRIEVE_TYPE_FCNAME:
        {
            Condition = string("FCNAME = BINARY '") + searchvalue + string("'");
            break;
        }
        case DbDataManager::HOST_RESULTSET_RETRIEVE_TYPE_RACGROUPID:
        {
            Condition = string("RACGROUPID = '") + searchvalue + string("'");
            break;
        }
        case DbDataManager::HOST_RESULTSET_RETRIEVE_TYPE_ONLYFORBACKUP:
        {
            Condition = string("(USEDFORNET = 0 AND USEDFORVM = 0 AND USEDFORFC = 0)");
            break;
        }
        default:
            break;
    }

    //SqlString = string("SELECT ORIGINALSERVERID, SERVERID, HCODE, HOSTNAME, MACADDR, IPADDR, BOOTFILE, NETBOOTPROTOCOL, OSTYPE, LISTENPORT, BUSINESSNICNAME, BRNICNAME, USEDFORLE, STGID, USEDFORNET, USEDFORVM, USEDFORFC, USEDFOREA, EAMODE, HEARTBEATTIMEOUT, QUERYINTERVAL, VIRTUALIP, VIRTUALMAC FROM TBLHOST");
    SqlString = string("SELECT ORIGINALSERVERID, SERVERID, HCODE, HOSTNAME, MACADDR, IPADDR, BOOTFILE, STATUS, ISACTIVE, HDNUM, USINGRHOST, RHOSTMAC, HBAIPADDR, ISNETBOOT, NETDISKCAPACITY, DBPROTECTED, OSTYPE, NETBOOTPROTOCOL, NETBOOTSERVICE, FCWWN, SYSTEMDIR, CONFIGDIR, VMMEMORY, VMCPU, LISTENPORT, USEDFORRDR, USEDFORLE, BUSINESSNICNAME, BRNICNAME, STGID, COPYMODE, SYNCFLAG, USEDFOREA, EAMODE, HEARTBEATTIMEOUT, QUERYINTERVAL, VIRTUALIP, VIRTUALMAC, RACGROUPID, EMERGENCYLOGONMODE, USEDFORMONITOR, ISAUTOTAKEOVER, VMBOOTCOPYMODE, VNCNUMBER, CREATETIME FROM TBLHOST");
    if (BeGeneralLib::IsStringEmpty(Condition) == false)
        SqlString = SqlString + string(" WHERE ") + Condition + string(" ORDER BY HCODE");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取主机个数
 *  参数：
 *      无
 *  返回：
 *      主机个数
 */
int DbDataManager::GetHostCount()
{

    return GetHostCount(DbDataManager::HOST_COUNT_RETRIEVE_TYPE_ALL);
}

/*
 *  功能：
 *      获取主机个数
 *  参数：
 *      type            :   主机个数获取类型
 *  返回：
 *      主机个数
 */
int DbDataManager::GetHostCount(DbDataManager::HostCountRetrieveType type)
{

    return GetHostCount(type, "");
}

/*
 *  功能：
 *      获取主机个数
 *  参数：
 *      type            :   主机个数获取类型
 *      excepthcode     :   排除的主机编码
 *  返回：
 *      主机个数
 */
int DbDataManager::GetHostCount(DbDataManager::HostCountRetrieveType type, string excepthcode)
{
    string Condition = "";
    switch (type)
    {
        case DbDataManager::HOST_COUNT_RETRIEVE_TYPE_NET_EMERGENCY:
        {
            Condition = string("USEDFORNET = 1");
            break;
        }
        case DbDataManager::HOST_COUNT_RETRIEVE_TYPE_VM_EMERGENCY:
        {
            Condition = string("USEDFORVM = 1");
            break;
        }
        case DbDataManager::HOST_COUNT_RETRIEVE_TYPE_FC_EMERGENCY:
        {
            Condition = string("USEDFORFC = 1");
            break;
        }
        case DbDataManager::HOST_COUNT_RETRIEVE_TYPE_DB_PROTECTED:
        {
            Condition = string("DBPROTECTED = 1");
            break;
        }
        case DbDataManager::HOST_COUNT_RETRIEVE_TYPE_EA:
        {
            Condition = string("USEDFOREA = 1");
            break;
        }
        default:
        {
            break;
        }
    }

    // 检查排除的主机编码是否为空
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(excepthcode) == false)
    {
        if (BeGeneralLib::IsStringEmpty(Condition) == true)
        {
            Condition = string("HCODE != '") + excepthcode + string("'");
        }
        else
        {
            Condition = Condition + SEPARATOR_CHAR_SPACE + string("AND") + SEPARATOR_CHAR_SPACE + string("HCODE != '") + excepthcode + string("'");
        }
    }

    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST");
    if (BeGeneralLib::IsStringEmpty(Condition) == false)
    {

        SqlString = SqlString + SEPARATOR_CHAR_SPACE + string("WHERE") + SEPARATOR_CHAR_SPACE + Condition;
    }

    return (GetFieldValueInt(SqlString, "RECCOUNT"));
}

/*
 *  功能：
 *      获取主机操作系统类型
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      操作系统类型
 */
DbDataManager::HostOSType DbDataManager::GetHostOSType(string hcode)
{

    return ConvertHostOsCodeToOsType(GetHostOSTypeCode(hcode));
}

/*
 *  功能：
 *      获取主机ip地址
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机的ip地址
 */
string DbDataManager::GetHostIp(string hcode)
{
    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_IPADDR);
}

string DbDataManager::GetHostMirrorMode(string hcode)
{
    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_MIRRORMODE);
}

string DbDataManager::GetHostMirrorState(string hcode)
{
    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_MIRRORSTATE);
}

/*
 *  功能：
 *      获取主机端口
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机的监听端口
 */
string DbDataManager::GetHostPort(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_LISTENPORT);
}

/*
 *  功能：
 *      根据主机编码获取主机名称
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机名称
 */
string DbDataManager::GetHostName(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_HOSTNAME);
}

/*
 *  功能：
 *      根据主机编码获取wwn
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      wwn
 */
string DbDataManager::GetHostWWN(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_FCWWN);
}

/*
 *  功能：
 *      根据主机编码获取启动协议
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      启动协议
 */
string DbDataManager::GetHostBootProtocol(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_NETBOOTPROTOCOL);
}

/*
 *  功能：
 *      获取主机默认网启协议
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      默认网启协议
 */
string DbDataManager::GetHostNetDefaultProtocol(string hcode)
{
    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_NETDEFAULTPROTOCOL);
}

/*
 *  功能：
 *      根据主机编码获取虚拟机内存
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机的虚拟机内存
 */
int DbDataManager::GetHostVmMemory(string hcode)
{

    return BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_VMMEMORY));
}

/*
 *  功能：
 *      根据主机编码获取虚拟机CPU
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机的虚拟机CPU
 */
int DbDataManager::GetHostVmCPU(string hcode)
{

    return BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_VMCPU));
}

/*
 *  功能：
 *      获取活动虚拟机内存总和
 *  参数：
 *      无
 *  返回：
 *      活动虚拟机内存总和
 */
int DbDataManager::GetActiveVmMemory()
{
    string SqlString = string("SELECT SUM(VMMEMORY) AS SVMMEMORY FROM TBLHOST WHERE STATUS = 3");

    return (GetFieldValueInt(SqlString, "SVMMEMORY"));
}

/*
 *  功能：
 *      获取活动虚拟机内存CPU
 *  参数：
 *      无
 *  返回：
 *      活动虚拟机内存CPU
 */
int DbDataManager::GetActiveVmCPU()
{
    string SqlString = string("SELECT SUM(VMCPU) AS SVMCPU FROM TBLHOST WHERE STATUS = 3");

    return (GetFieldValueInt(SqlString, "SVMCPU"));
}

/*
 *  功能：
 *      获取主机mac地址
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机mac地址
 */
string DbDataManager::GetHostMac(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_MACADDR);
}

/*
 *  功能：
 *      获取主机系统目录
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机系统目录
 */
string DbDataManager::GetHostSystemDir(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_SYSTEMDIR);
}

/*
 *  功能：
 *      获取主机配置目录
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机配置目录
 */
string DbDataManager::GetHostConfigDir(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_CONFIGDIR);
}

/*
 *  功能：
 *      主机编码是否存在
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsHostExist(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE = '") + hcode + string("'");

    return (GetFieldValueBool(SqlString, "RECCOUNT"));
}

/*
 *  功能：
 *      获取主机保护状态
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机保护状态
 */
DbDataManager::HostProtectedStatus DbDataManager::GetHostProtectedStatus(string hcode)
{
//  return (DbDataManager::HostProtectedStatus)(BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_STATUS)));
    string SqlString = string("SELECT STATUS FROM TBLHOST WHERE HCODE = '") + hcode + string("'");
    return (DbDataManager::HostProtectedStatus)(BeGeneralLib::StringToInt(GetFieldValue(SqlString, "STATUS")));
}

/*
 *  功能：
 *      获取主机业务网卡名
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机业务网卡名
 */
string DbDataManager::GetHostBusinessNicName(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_BUSINESSNICNAME);
}

/*
 *  功能：
 *      获取主机桥接网卡名
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机桥接网卡名
 */
string DbDataManager::GetHostBridgeNicName(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_BRNICNAME);
}

/*
 *  功能：
 *      获取主机操作系统类型编码
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机操作系统类型编码
 */
string DbDataManager::GetHostOSTypeCode(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_OSTYPE);
}

/*
 *  功能：
 *      获取主机字段值
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机字段值
 */
map<string, string> DbDataManager::GetHostFieldValue(string hcode)
{
    vector<string> FieldList;
    FieldList.push_back("ORIGINALSERVERID");
    FieldList.push_back("SERVERID");
    FieldList.push_back("HCODE");
    FieldList.push_back("HOSTNAME");
    FieldList.push_back("MACADDR");
    FieldList.push_back("IPADDR");
    FieldList.push_back("BOOTFILE");
    FieldList.push_back("STATUS");
    FieldList.push_back("ISACTIVE");
    FieldList.push_back("HDNUM");
    FieldList.push_back("USINGRHOST");
    FieldList.push_back("RHOSTMAC");
    FieldList.push_back("HBAIPADDR");
    FieldList.push_back("ISNETBOOT");
    FieldList.push_back("NETDISKCAPACITY");
    FieldList.push_back("DBPROTECTED");
    FieldList.push_back("OSTYPE");
    FieldList.push_back("NETBOOTPROTOCOL");
    FieldList.push_back("NETDEFAULTPROTOCOL");
    FieldList.push_back("NETBOOTSERVICE");
    FieldList.push_back("FCWWN");
    FieldList.push_back("SYSTEMDIR");
    FieldList.push_back("CONFIGDIR");
    FieldList.push_back("VMMEMORY");
    FieldList.push_back("VMCPU");
    FieldList.push_back("LISTENPORT");
    FieldList.push_back("USEDFORRDR");
    FieldList.push_back("USEDFORLE");
    FieldList.push_back("BUSINESSNICNAME");
    FieldList.push_back("BRNICNAME");
    FieldList.push_back("STGID");
    FieldList.push_back("COPYMODE");
    FieldList.push_back("SYNCFLAG");
    FieldList.push_back("FCNAME");
    FieldList.push_back("FCCHANNEL");
    FieldList.push_back("INITIATORNAME");
    FieldList.push_back("USEDFORNET");
    FieldList.push_back("USEDFORVM");
    FieldList.push_back("USEDFORFC");
    FieldList.push_back("CDPSTATE");
    FieldList.push_back("USEDFOREA");
    FieldList.push_back("EAMODE");
    FieldList.push_back("HEARTBEATTIMEOUT");
    FieldList.push_back("QUERYINTERVAL");
    FieldList.push_back("VIRTUALIP");
    FieldList.push_back("VIRTUALMAC");
    FieldList.push_back("RACGROUPID");
    FieldList.push_back("EMERGENCYLOGONMODE");
    FieldList.push_back("USEDFORMONITOR");
    FieldList.push_back("ISAUTOTAKEOVER");
    FieldList.push_back("VMBOOTCOPYMODE");
    FieldList.push_back("VNCNUMBER");

    string FieldNameString = GetFieldNameString(FieldList);
    string SqlString = string("SELECT ") + FieldNameString + string(" FROM TBLHOST WHERE HCODE = '") + hcode + string("'");

    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取主机应急副本结果集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机应急副本结果集
 */
sql::ResultSet * DbDataManager::GetHostEmergencyCopyResultSet(string hcode)
{
    string SqlString = string("SELECT T.SSCOPYID, T.HCODE, T.IQN, T.USED, T.TARGETFILE, V.LMOUNTPOINT, V.STRATEGYID FROM TBLSNAPSHOTCOPY T, (SELECT SSCOPYID, STRATEGYID, LMOUNTPOINT FROM TBLCOPYSTRATEGY WHERE HCODE = '") + hcode + string("')V WHERE T.SSCOPYID = V.SSCOPYID AND T.LMOUNTPOINT = V.LMOUNTPOINT");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取主机存储路径
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机存储路径
 */
string DbDataManager::GetHostStoragePath(string hcode)
{
    string SqlString = string("SELECT S.MOUNTPOINT FROM TBLHOST T, TBLSTORAGESPACE S WHERE T.HCODE = '") + hcode + string("' AND T.STGID = S.STGID");

    return GetFieldValue(SqlString, "MOUNTPOINT");
}

/*
 *  功能：
 *      获取主机复制方式
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机复制方式
 */
DbDataManager::HostCopyMode DbDataManager::GetHostCopyMode(string hcode)
{

    return (DbDataManager::HostCopyMode)(BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_COPYMODE)));
}

/*
 *  功能：
 *      获取主机备机ip
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机备机ip
 */
string DbDataManager::GetHostReservedIp(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_HBAIPADDR);
}

/*
 *  功能：
 *      获取主机FC名称
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机FC名称
 */
string DbDataManager::GetHostFCName(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_FCNAME);
}

/*
 *  功能：
 *      获取主机FC通道号
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机FC通道号
 */
string DbDataManager::GetHostFCChannel(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_FCCHANNEL);
}

/*
 *  功能：
 *      获取主机initiator名称
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      主机initiator名称
 */
string DbDataManager::GetHostInitiatorName(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_INITIATORNAME);
}

/*
 *  功能：
 *      获取主机网卡结果集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      网卡结果集
 */
sql::ResultSet * DbDataManager::GetHostNetworkAdapterResultSet(string hcode)
{
    string SqlString = string("SELECT NICID, ORIGINALSERVERID, SERVERID, HCODE, MACADDRESS, DEVICENAME, CAPTION, GATEWAY, MAINDNS, SUBDNS, DHCPENABLED, ISPHYSICALADAPTER, STATUS, WORKMODE, USEDFORVM, BUSINESSNICNAME, BRNICNAME, VIRTUALNETWORKMODE, VIRTUALMAC FROM TBLHOSTNIC WHERE HCODE='") + hcode + string("'");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取主机网卡个数
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      网卡个数
 */
int DbDataManager::GetHostNetworkAdapterCount(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOSTNIC WHERE HCODE='") + hcode + string("'");

    return (GetFieldValueInt(SqlString, "RECCOUNT"));
}

/*
 *  功能：
 *      获取主机ip地址结果集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      IP地址结果集
 */
sql::ResultSet * DbDataManager::GetHostIpAddressResultSet(string hcode)
{
    string SqlString = string("SELECT T.ORIGINALSERVERID, T.SERVERID, T.HCODE, T.MACADDRESS, IPADDRESS, NETMASK, S.WORKMODE FROM TBLHOSTIPADDRESS T, TBLHOSTNIC S WHERE S.HCODE='") + hcode + string("' AND T.MACADDRESS = S.MACADDRESS AND T.HCODE = S.HCODE");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取主机ip地址结果集
 *  参数：
 *      hcode           :   主机编码
 *      mac             :   mac地址
 *  返回：
 *      IP地址结果集
 */
sql::ResultSet * DbDataManager::GetHostIpAddressResultSet(string hcode, string mac)
{
    string SqlString = string("SELECT ORIGINALSERVERID, SERVERID, HCODE, MACADDRESS, IPADDRESS, NETMASK, IPV6ADDRESS, IPV6NETMASK, WORKMODE FROM TBLHOSTIPADDRESS WHERE HCODE='") + hcode + string("' AND MACADDRESS ='") + mac + string("'");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取主机自动应急模式
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      主机自动应急模式
 */
DbDataManager::HostEAMode DbDataManager::GetHostEAMode(string hcode)
{

    return (DbDataManager::HostEAMode)(BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_EAMODE)));
}

/*
 *  功能：
 *      获取主机心跳超时时间
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      主机心跳超时时间
 */
int DbDataManager::GetHostHeartbeatTimeout(string hcode)
{

    return BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_HEARTBEATTIMEOUT));
}

/*
 *  功能：
 *      获取主机虚ip
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      主机虚ip
 */
string DbDataManager::GetHostVirtualIp(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_VIRTUALIP);
}

/*
 *  功能：
 *      获取主机虚mac
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      主机虚mac
 */
string DbDataManager::GetHostVirtualMac(string hcode)
{

    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_VIRTUALMAC);
}

/*
 *  功能：
 *      获取主机通讯网卡的虚拟mac
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      主机通讯网卡的虚拟mac
 */
string DbDataManager::GetHostCommunicationNicVirtualMac(string hcode)
{
    string SqlString = string("SELECT S.VIRTUALMAC FROM TBLHOSTNIC S, TBLHOSTIPADDRESS T WHERE S.HCODE ='") + hcode + string("' AND S.HCODE = T.HCODE AND S.MACADDRESS = T.MACADDRESS AND T.WORKMODE = 1");

    return GetFieldValue(SqlString, "VIRTUALMAC");
}

/*
 *  功能：
 *      获取主机通讯网卡的网络模式
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      主机通讯网卡的网络模式
 */
string DbDataManager::GetHostCommunicationNicNetworkMode(string hcode)
{
    string SqlString = string("SELECT S.VIRTUALNETWORKMODE FROM TBLHOSTNIC S, TBLHOSTIPADDRESS T WHERE S.HCODE ='") + hcode + string("' AND S.HCODE = T.HCODE AND S.MACADDRESS = T.MACADDRESS AND T.WORKMODE = 1");

    return GetFieldValue(SqlString, "VIRTUALNETWORKMODE");
}

/*
 *  功能：
 *      获取主机rac组编号
 *  返回：
 *      rac组编号
 */
string DbDataManager::GetHostRacGroupId(string hcode)
{
    return GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_RACGROUPID);
}

/*
 *  功能：
 *      获取主机应急时副本挂载方式
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      应急时副本挂载方式
 */
DbDataManager::EmergencyLogonMode DbDataManager::GetHostEmergencyLogonMode(string hcode)
{

    return (DbDataManager::EmergencyLogonMode)(BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_EMERGENCYLOGONMODE)));
}

/*
 *  功能：
 *      获取主机虚拟机启动副本方式
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      虚拟机启动副本方式
 */
DbDataManager::VmBootCopy DbDataManager::GetHostVmBootCopyMode(string hcode)
{
    return (DbDataManager::VmBootCopy)(BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_VMBOOTCOPYMODE)));
}

/*
 *  功能：
 *      获取主机vnc号
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      vnc号
 */
int DbDataManager::GetHostVncNumber(string hcode)
{

    return BeGeneralLib::StringToInt(GetHostFieldValue(hcode, DbDataManager::HOST_TABLE_FIELD_VNCNUMBER));
}

/*
 *  功能：
 *      将主机操作系统代码转换为操作系统类型
 *  参数
 *      oscode           :   操作系统代码
 *  返回：
 *      操作系统类型
 */
DbDataManager::HostOSType DbDataManager::ConvertHostOsCodeToOsType(string oscode)
{
    if (oscode == DEFAULT_OSTYPE_WINDOWS ||
            oscode == DEFAULT_OSTYPE_WIN2K_XP_2K3 ||
            oscode == DEFAULT_OSTYPE_WIN2K8)
    {
        return DbDataManager::HOST_OS_TYPE_WINDOWS;
    }

    if (oscode == DEFAULT_OSTYPE_LINUX)
    {
        return DbDataManager::HOST_OS_TYPE_LINUX;
    }

    if (oscode == DEFAULT_OSTYPE_AIX)
    {
        return DbDataManager::HOST_OS_TYPE_AIX;
    }
    if (oscode == DEFAULT_OSTYPE_HPUX)
    {

        return DbDataManager::HOST_OS_TYPE_HPUX;
    }

    return DbDataManager::HOST_OS_TYPE_WINDOWS;
}

/*
 *  功能：
 *      主机名称是否存在
 *  参数
 *      name            :   主机名称
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsHostNameExist(string name)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HOSTNAME ='") + name + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      主机ip是否存在
 *  参数
 *      ipaddr          :   主机ip
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsHostIpExist(string ipaddr)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE IPADDR = '") + ipaddr + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      主机是否为网启
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果为网启返回true，否则返回false
 */
bool DbDataManager::IsHostEmergency(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND (STATUS = 2 OR STATUS = 3 OR STATUS = 4)");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查该网卡是否用于虚拟机启动
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostNetworkAdapterForVm(string hcode, string mac)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOSTNIC WHERE HCODE ='") + hcode + string("' AND MACADDRESS ='") + mac + string("' AND USEDFORVM = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查该网卡是否存在
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostNetworkAdapterExist(string hcode, string mac)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOSTNIC WHERE HCODE ='") + hcode + string("' AND MACADDRESS ='") + mac + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

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
bool DbDataManager::IsHostNetworkAdapterIpExist(string hcode, string mac, string ip, string mask)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOSTIPADDRESS WHERE HCODE ='") + hcode + string("' AND MACADDRESS ='") + mac + string("' AND IPADDRESS = '") + ip + string("' AND NETMASK ='") + mask + string("'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机本地应急功能打开
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostLocalEmergencyOpen(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND USEDFORLE = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机网启应急功能打开
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostNetEmergencyOpen(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND USEDFORNET = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机虚拟机应急功能打开
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostVmEmergencyOpen(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND USEDFORVM = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机光纤卡应急功能打开
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostFcEmergencyOpen(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND USEDFORFC = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机是否仅备份
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostOnlyForBackup(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND (USEDFORNET = 0 AND USEDFORVM = 0 AND USEDFORFC = 0)");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机自动应急功能是否打开
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostEAOpen(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND USEDFOREA = 1");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

bool DbDataManager::IsHostMonitorOpen(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND USEDFORMONITOR = 1");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机是否有业务任务运行
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsHostHasBusinessTaskRunning(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBL_TASK WHERE TASKTYPE = 1 AND (STATUS = 1 OR STATUS = 2) AND HCODE ='") + hcode + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机硬件应急功能是否打开
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostHWEmergencyOpen(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST T, TBLHWEHOST S, TBLHWECONFIG R WHERE T.HCODE = S.HCODE AND S.USEDFORHWE = 1 AND S.KEYID <= R.MAXKEYNUMBER AND T.HCODE ='") + hcode + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机是否为RAC组的首选主机
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostRacGroupMaster(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST T, TBLRACGROUP S WHERE T.RACGROUPID = S.RACGROUPID AND T.HCODE = S.MASTERHCODE AND T.HCODE ='") + hcode + string("'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机是否已经做过初始复制
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostInitializeCopyed(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOTCOPY WHERE HCODE ='") + hcode + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机是否已经做过初始复制
 *  参数
 *      hcode           :   主机编码
 *      lmountpoint     :   本地挂载点
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostInitializeCopyed(string hcode, string lmountpoint)
{
    string LMountPoint = GetHostMountPoint(hcode, lmountpoint);
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSNAPSHOTCOPY WHERE HCODE ='") + hcode + string("' AND LMOUNTPOINT ='") + LMountPoint + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机远程容灾功能是否打开
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsHostRDROpen(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND USEDFORRDR = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查主机是否有CDP保护
 *  参数
 *      hcode           :   主机编码
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsHostHasCDPProtection(string hcode)
{
    string SqlString = string("SELECT COUNT( * ) AS RECCOUNT FROM TBLHOST T, TBLCOPYSTRATEGY S, TBLSNAPSHOTCOPY R WHERE T.HCODE = S.HCODE AND S.HCODE = R.HCODE AND S.LMOUNTPOINT = R.LMOUNTPOINT AND S.SSCOPYID = R.SSCOPYID AND S.USEDFORCDP =1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查虚拟mac地址是否已存在
 *  参数
 *      mac             :   mac地址
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsVirtualMacExists(string mac)
{
    string SqlString = string("SELECT COUNT( * ) AS RECCOUNT FROM TBLHOST WHERE VIRTUALMAC = '") + mac + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================= 数据库相关 =================================//

/*
 *  功能：
 *      删除数据库策略明细
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteDbStrategyDetail(string hcode)
{
    string SqlDelete = string("DELETE FROM TBLCOPYDETAIL  USING  TBLCOPYDETAIL, TBLCOPYSTRATEGY WHERE TBLCOPYSTRATEGY.HCODE = '") + hcode + string("' AND TBLCOPYDETAIL.OPERATION = 'D' AND TBLCOPYDETAIL.STRATEGYID = TBLCOPYSTRATEGY.STRATEGYID");

    return ExecuteSql(SqlDelete);
}

/*
 *  功能：
 *      删除数据库任务计划
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteDbSchedule(string hcode)
{
    string SqlDelete = string("DELETE FROM TBLSCHEDULE WHERE HCODE ='") + hcode + string("' AND COMMANDCODE = '107' AND (STATUS = 1 OR STATUS = 2)");

    return ExecuteSql(SqlDelete);
}

/*
 *  功能：
 *      删除数据库任务
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteDbTask(string hcode)
{
    string SqlDelete = string("DELETE FROM TBL_TASK WHERE HCODE ='") + hcode + string("' AND COMMANDCODE = '107' AND STATUS = 0");

    return ExecuteSql(SqlDelete);
}

/*
 *  功能：
 *      更新数据库保护状态
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateDbProtectedStatus(string hcode, int status)
{
    string SqlString = string("UPDATE TBLHOST SET DBPROTECTED = ") + BeGeneralLib::Number2String(status) + string(" WHERE HCODE = '") + hcode + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取数据库保护数
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      数据库保护数
 */
int DbDataManager::GetDbCount(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS DBCOUNT FROM TBLHOST WHERE DBPROTECTED = 1 AND HCODE != '") + hcode + string("'");

    return GetFieldValueInt(SqlString, "DBCOUNT");
}

/*
 *  功能：
 *      获取数据库保护数
 *  参数：
 *      无
 *  返回：
 *      数据库保护数
 */
int DbDataManager::GetDbCount()
{
    string SqlString = string("SELECT COUNT(*) AS DBCOUNT FROM TBLHOST WHERE DBPROTECTED = 1");

    return GetFieldValueInt(SqlString, "DBCOUNT");
}

/*
 *  功能：
 *      获取主机数据库保护状态
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      数据库保护状态
 */
int DbDataManager::GetDbProtectedStatus(string hcode)
{
    string SqlString = string("SELECT DBPROTECTED FROM TBLHOST WHERE HCODE = '") + hcode + string("'");

    return GetFieldValueInt(SqlString, "DBPROTECTED");
}

/*
 *  功能：
 *      主机是否进行了数据库保护
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsDbProtected(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE HCODE ='") + hcode + string("' AND DBPROTECTED = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      是否有自动应急功能打开的主机
 *  参数：
 *      无
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::HasEAOpenedHosts()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE USEDFOREA = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      是否有异地容灾功能打开的主机
 *  参数：
 *      无
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::HasRdrOpenedHosts()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE USEDFORRDR = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================== 计划相关 ==================================//

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
bool DbDataManager::AddSchedule(string localserverid, string remoteserverid, string schedulename, string commandcode, string parameter, string hcode, string tasktype, string exectype, string execinterval, string intervalunit, string starttime, string nexttime, string status, string lasttime)
{
    string SqlString = string("INSERT INTO TBLSCHEDULE (ORIGINALSERVERID, SERVERID, SCHEDULENAME, COMMANDCODE, PARAMETER, HCODE, TASKTYPE, EXECTYPE, EXECINTERVAL, INTERVALUNIT, STARTTIME, NEXTTIME, STATUS, LASTTIME, CREATETIME, CREATORID) VALUES('") + localserverid + string("',") +
            string("'") + remoteserverid + string("',") +
            string("'") + schedulename + string("',") +
            string("'") + commandcode + string("',") +
            string("'") + parameter + string("',") +
            string("'") + hcode + string("',") +
            tasktype + string(",") +
            exectype + string(",") +
            execinterval + string(",") +
            intervalunit + string(",") +
            string("'") + starttime + string("',") +
            string("'") + nexttime + string("',") +
            status + string(",") +
            string("'") + lasttime + string("')");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      删除指定主机编码下的任务计划
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool DbDataManager::DeleteSchedule(string hcode)
{
    string SqlDelete = string("DELETE FROM TBLSCHEDULE WHERE HCODE ='") + hcode + string("'");

    return ExecuteSql(SqlDelete);
}

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
bool DbDataManager::UpdateSchedule(string localserverid, string remoteserverid, string schedulename, string commandcode, string parameter, string hcode, string tasktype, string exectype,
        string execinterval, string intervalunit, string starttime, string nexttime, string status, string lasttime)
{
    string SqlString = string("UPDATE TBLSCHEDULE SET ORIGINALSERVERID = '") + localserverid + string("',") +
            string("SERVERID = '") + remoteserverid + string("',") +
            string("SCHEDULENAME = '") + schedulename + string("',") +
            string("COMMANDCODE = '") + commandcode + string("',") +
            string("PARAMETER = '") + parameter + string("',") +
            string("HCODE = '") + hcode + string("',") +
            string("TASKTYPE = ") + tasktype + string(",") +
            string("EXECTYPE = ") + exectype + string(",") +
            string("EXECINTERVAL = ") + execinterval + string(",") +
            string("INTERVALUNIT = ") + intervalunit + string(",") +
            string("STARTTIME = '") + starttime + string("',") +
            string("NEXTTIME = '") + nexttime + string("',") +
            string("STATUS = ") + status + string(",") +
            string("LASTTIME = '") + lasttime + string("' WHERE ORIGINALSERVERID = '") + localserverid + string("' AND SCHEDULENAME ='") + schedulename + string("'");

    return ExecuteSql(SqlString);

}

/*
 *  功能：
 *      获取调度计划结果集
 *  参数：
 *      无
 *  返回：
 *      调度计划结果集
 */
sql::ResultSet * DbDataManager::GetScheduleResultSet()
{

    return GetScheduleResultSet(GetLocalServerId());
}

/*
 *  功能：
 *      获取调度计划结果集
 *  参数：
 *      serverid        :   主机编码
 *  返回：
 *      调度计划结果集
 */
sql::ResultSet * DbDataManager::GetScheduleResultSet(string serverid)
{
    string SqlString = string("SELECT ORIGINALSERVERID, SERVERID, SCHEDULEID, SCHEDULENAME, COMMANDCODE, PARAMETER, HCODE, TASKTYPE, EXECTYPE, EXECINTERVAL, INTERVALUNIT, STARTTIME, NEXTTIME, STATUS, LASTTIME FROM TBLSCHEDULE WHERE SERVERID ='") + serverid + string("'");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取调度计划字段值
 *  参数：
 *      scheduleid      :   计划id
 *  返回：
 *      调度计划字段值
 */
map<string, string> DbDataManager::GetScheduleFieldValue(string scheduleid)
{

    return GetScheduleFieldValue(scheduleid, "");
}

/*
 *  功能：
 *      获取调度计划字段值
 *  参数：
 *      serverid        :   计划id
 *      name            :   计划名称
 *  返回：
 *      调度计划字段值
 */
map<string, string> DbDataManager::GetScheduleFieldValue(string serverid, string schedulename)
{
    vector<string> FieldList;
    FieldList.push_back("ORIGINALSERVERID");
    FieldList.push_back("SERVERID");
    FieldList.push_back("SCHEDULEID");
    FieldList.push_back("SCHEDULENAME");
    FieldList.push_back("COMMANDCODE");
    FieldList.push_back("PARAMETER");
    FieldList.push_back("HCODE");
    FieldList.push_back("TASKTYPE");
    FieldList.push_back("EXECTYPE");
    FieldList.push_back("EXECINTERVAL");
    FieldList.push_back("INTERVALUNIT");
    FieldList.push_back("STARTTIME");
    FieldList.push_back("NEXTTIME");
    FieldList.push_back("STATUS");
    FieldList.push_back("LASTTIME");
    string FieldNameString = GetFieldNameString(FieldList);
    string SqlString = string("SELECT ") + FieldNameString + string(" FROM TBLSCHEDULE WHERE ORIGINALSERVERID = '") + serverid + string("'");
    if (BeGeneralLib::IsStringEmpty(schedulename) == false)
    {

        SqlString = SqlString + string(" AND SCHEDULENAME ='") + schedulename + string("'");
    }
    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      将符合条件任务计划压入任务表
 *  参数：
 *      sqlconn         :   数据库连接对象
 *  返回：
 *      无
 */
void DbDataManager::MoveScheduleToTask(sql::Connection * sqlconn)
{
//	BesLog::DebugPrint("MoveScheduleToTask...", BesLog::LOG_MESSAGE_TYPE_INFO, false);
    string CommandCode = "";
    string Parameter = "";

    // 更新任务的开始时间为当前时间
    string SqlString = string("UPDATE TBL_TASK SET STARTTIME = NOW() WHERE STATUS = 0");
    ExecuteSql(sqlconn, SqlString);

    // 更新任务的开始时间为当前时间
    SqlString = string("UPDATE TBLREMOTETASK SET STARTTIME = NOW() WHERE STATUS = 0");
    ExecuteSql(sqlconn, SqlString);

    // 更新计划中下次时间为0000-00-00 00:00:00为开始时间
//  SqlString = string("UPDATE TBLSCHEDULE SET NEXTTIME = STARTTIME WHERE TO_DAYS(NEXTTIME) IS NULL");
    SqlString = string("UPDATE TBLSCHEDULE SET NEXTTIME = STARTTIME WHERE NEXTTIME = '1900-01-01 00:00:00'");
    ExecuteSql(sqlconn, SqlString);

    // 将符合条件的任务压入到任务队列
    string SqlStringSchedule = string("SELECT SCHEDULEID, EXECTYPE, EXECINTERVAL, NEXTTIME, NOW() AS NOWTIME, STATUS FROM TBLSCHEDULE WHERE NEXTTIME <= NOW() AND ((EXECTYPE = 0 AND NEXTTIME != LASTTIME) OR (EXECTYPE != 0 AND EXECINTERVAL != 0)) AND STATUS = 1");
    sql::ResultSet *TempRs = GetResultSet(sqlconn, SqlStringSchedule);
    if (TempRs != NULL && TempRs->rowsCount() > 0)
    {
        while (TempRs->next())
        {
            string ScheduleId = TempRs->getString("SCHEDULEID");
            int ExecInterval = TempRs->getInt("EXECINTERVAL");
            int ExecType = TempRs->getInt("EXECTYPE");
            int Status = TempRs->getInt("STATUS");
//          BesLog::DebugPrint(string("SCHEDULEID STATUS")+ScheduleId+" "+BeGeneralLib::Number2String(Status), BesLog::LOG_MESSAGE_TYPE_INFO, false);

            // 更新下次执行时间
            string NextTime = "";
            if (ExecType == 0)
            {
                NextTime = TempRs->getString("NEXTTIME");
            }
            else
            {
                NextTime = BeGeneralLib::CalcCycleTime(TempRs->getString("NEXTTIME"), TempRs->getString("NOWTIME"), (BeGeneralLib::TimeCycle)ExecType, ExecInterval);
            }

            // 更新计划表中的执行时间
            SqlString = string("UPDATE TBLSCHEDULE SET LASTTIME = NEXTTIME, NEXTTIME = '") + NextTime + string("' WHERE SCHEDULEID = ") + ScheduleId;
            ExecuteSql(sqlconn, SqlString);

            // 获取复制策略结果集
            SqlString = string("SELECT T.STRATEGYID, HCODE, LMOUNTPOINT FROM TBLCOPYSTRATEGY T, (SELECT STRATEGYID FROM TBLCOPYDETAIL WHERE SCHEDULEID =") + ScheduleId + string(")V WHERE T.STRATEGYID = V.STRATEGYID GROUP BY STRATEGYID ORDER BY STRATEGYID");
            sql::ResultSet * StrategyRs = GetResultSet(SqlString);
            if (StrategyRs != NULL && StrategyRs->rowsCount() > 0)
            {
                while (StrategyRs->next())
                {
                    string StrategyId = StrategyRs->getString("STRATEGYID");
                    string HCode = StrategyRs->getString("HCODE");
                    string LMountPoint = StrategyRs->getString("LMOUNTPOINT");

                    // 检查主机是否存在
                    if (IsHostExist(HCode) == false)
                    {
                        continue;
                    }

                    CommandCode = "";
                    Parameter = "";

                    // 检查明细是否是否有(*)
                    if (IsStrategyHasFull(StrategyId, ScheduleId))
                    {
                        switch (GetHostOSType(HCode))
                        {
                            case DbDataManager::HOST_OS_TYPE_WINDOWS:
                            {
                                CommandCode = DEFAULT_COMMAND_WINDOWS_SNAPSHOT_COPY;
                                Parameter = BeCommand::CreateCommandParameter(2, HCode.c_str(), LMountPoint.c_str());
                                break;
                            }
                            case DbDataManager::HOST_OS_TYPE_LINUX:
                            {
                                CommandCode = DEFAULT_COMMAND_LINUX_SNAPSHOT_COPY;
                                Parameter = BeCommand::CreateCommandParameter(2, HCode.c_str(), StrategyId.c_str());
                                break;
                            }
                            case DbDataManager::HOST_OS_TYPE_AIX:
                            case DbDataManager::HOST_OS_TYPE_HPUX:
                            {
                                CommandCode = DEFAULT_COMMAND_AIX_SNAPSHOT_COPY;
                                Parameter = BeCommand::CreateCommandParameter(2, HCode.c_str(), StrategyId.c_str());
                                break;
                            }
                        }
                    }
                    else if (IsStrategyHasDb(StrategyId, ScheduleId) == true || IsStrategyHasInclude(StrategyId, ScheduleId) == true)
                    {
                        DbDataManager::HostOSType OSType = GetHostOSType(HCode);
                        if (OSType == DbDataManager::HOST_OS_TYPE_AIX
                                || OSType == DbDataManager::HOST_OS_TYPE_HPUX)
                        {
                            CommandCode = DEFAULT_COMMAND_AIX_SCHEDULE_SNAPSHOT_COPY; // AIX调度快照复制
                        }
                        else
                        {
                            CommandCode = DEFAULT_COMMAND_SCHEDULE_SNAPSHOT_COPY; // Linux调度快照复制
                        }
                        Parameter = BeCommand::CreateCommandParameter(3, HCode.c_str(), StrategyId.c_str(), ScheduleId.c_str());
                    }

                    // 检查当前任务列表中的复制任务条数是否已经超过最大调度任务条数
                    string SqlQuery = string("SELECT COUNT(*) AS RECCOUNT FROM TBL_TASK WHERE HCODE = '") + HCode + string("' AND (STATUS = 0 OR STATUS  = 1 OR STATUS = 2) AND (COMMANDCODE = 104 OR COMMANDCODE = 122 OR COMMANDCODE = 123) AND (UPPER(SUBSTRING_INDEX(TRIM(';' FROM TRIM(LEADING '") + HCode + string("' FROM PARAMETER)), ';', 1)) = UPPER('") + LMountPoint + string("') OR UPPER(SUBSTRING_INDEX(TRIM(';' FROM TRIM(LEADING '") + HCode + string("' FROM PARAMETER)), ';', 1)) = UPPER('") + StrategyId + string("'))");
                    if (GetFieldValueInt(SqlQuery, "RECCOUNT") >= DEFAULT_MAX_SCHEDULE_TASK_COUNT)
                    {
                        continue;
                    }

                    // 增加到任务表
                    if (BeGeneralLib::IsStringEmptyOrInvalidValue(CommandCode) == false)
                    {
                        string SqlInsert = string("INSERT INTO TBL_TASK (COMMANDCODE, PARAMETER, STARTTIME, ENDTIME, CREATETIME, UPDATETIME) VALUES('")
                                + CommandCode + string("','") + Parameter + string("', NOW(), NOW(), NOW(), NOW())");
                        ExecuteSql(SqlInsert);
                    }
                }
            }

            // 销毁结果集
            DestroyResultSet(StrategyRs);


            // 获取远程容灾结果集
            SqlString = string("SELECT T.HCODE, V.SERVERID AS REMOTESERVERID, V.SERVERIP, V.PORT, V.MAXALLOWNUM, S.SERVERID AS LOCALSERVERID FROM TBLSYNCSCHEDULE T, TBLREMOTESERVER V, TBLBESINFORMATION S WHERE T.SCHEDULEID = ") + ScheduleId + string(" AND V.SERVERTYPE = 1 AND T.SERVERID = V.SERVERID AND V.SERVERID != S.SERVERID");
            sql::ResultSet * SyncTaskRs = GetResultSet(sqlconn, SqlString);
            while (SyncTaskRs->next())
            {
                CommandCode = DEFAULT_COMMAND_SYNC_HOST_INFORMATION;
                string HCode = SyncTaskRs->getString("HCODE");
                string LocalServerId = SyncTaskRs->getString("LOCALSERVERID");
                string RemoteServerId = SyncTaskRs->getString("REMOTESERVERID");
                Parameter = BeCommand::CreateCommandParameter(3, RemoteServerId.c_str(), LocalServerId.c_str(), HCode.c_str());

                // 插入
                string SqlInsert = string("INSERT INTO TBLREMOTETASK (COMMANDCODE, PARAMETER, TASKTYPE, HCODE, STARTTIME, ENDTIME, CREATETIME, UPDATETIME) VALUES('")
                        + CommandCode + string("','") + Parameter + string("', 1 , '") + HCode + string("', NOW(), NOW(), NOW(), NOW())");
                ExecuteSql(SqlInsert);
            }

            // 销毁结果集
            DestroyResultSet(SyncTaskRs);


            // 更新只执行一次的任务为完成
            if (ExecType == 0)
            {
                string SqlUpdate = string("UPDATE TBLSCHEDULE SET STATUS = 0 WHERE SCHEDULEID = ") + ScheduleId;
                ExecuteSql(SqlUpdate);
            }

        }
    }

    // 销毁结果集
    DestroyResultSet(TempRs);
}

/*
 *  功能：
 *      检查计划是否存在
 *  参数：
 *      hcode           :   主机编码
 *      scheduleid      :   计划id
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool DbDataManager::IsScheduleExist(string scheduleid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSCHEDULE WHERE SCHEDULEID =") + scheduleid;

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查计划是否存在
 *  参数：
 *      serverid        :   服务器id
 *      name            :   计划名称
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool DbDataManager::IsScheduleExist(string serverid, string name)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSCHEDULE WHERE ORIGINALSERVERID = '") + serverid + string("' AND SCHEDULENAME = '") + name + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================== 网络相关 ==================================//

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
bool DbDataManager::AddSubnet(string subnet, string mask, string rangelow, string rangehigh, string serverip)
{
    string SqlString = string("INSERT INTO TBLDHCPSUBNET (SUBNET, MASK, RANGELOW, RANGEHIGH, SERVERIP, ROUTER, MAINDNS, SUBDNS, UPDATETIME) VALUES('") + subnet + string("',")
            + string("'") + mask + string("',")
            + string("'") + rangelow + string("',")
            + string("'") + rangehigh + string("',")
            + string("'") + serverip + string("', '', '', '',NOW())");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新网卡类型
 *  参数：
 *      nicname         :   网卡名
 *      nictype         :   网卡类型
 *  返回：
 *      如果成功返回true，否则返回false
 */
bool DbDataManager::UpdateNicType(string nicname, DbDataManager::NicType nictype)
{
    string SqlString = string("UPDATE TBLBESNIC SET NICTYPE =") + BeGeneralLib::Number2String(nictype) + SEPARATOR_CHAR_SPACE + string("WHERE NICNAME = BINARY '") + nicname + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取网卡信息结果集
 *  参数：
 *      无
 *  返回：
 *      网卡信息结果集
 */
sql::ResultSet * DbDataManager::GetNetworkResultSet()
{
    string SqlString = string("SELECT NICNAME, IPADDR, NETMASK, MACADDR, GATEWAY, MAINDNS, SUBDNS, ROUTESUBNET, ROUTENETMASK, NICTYPE FROM TBLBESNIC ORDER BY NICNAME");
    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取网络参数
 *  参数：
 *      nicname         :   网卡名称
 *  返回：
 *      网络参数
 */
map<string, string> DbDataManager::GetNetworkParameter(string nicname)
{
    string SqlString = string("SELECT IPADDR, NETMASK, MACADDR, GATEWAY, MAINDNS, SUBDNS, ROUTESUBNET, ROUTENETMASK, NICTYPE FROM TBLBESNIC WHERE NICNAME = BINARY '") + nicname + string("'");

    vector<string> FieldList;
    FieldList.push_back("IPADDR");
    FieldList.push_back("NETMASK");
    FieldList.push_back("MACADDR");
    FieldList.push_back("GATEWAY");
    FieldList.push_back("MAINDNS");
    FieldList.push_back("SUBDNS");
    FieldList.push_back("ROUTESUBNET");
    FieldList.push_back("ROUTENETMASK");
    FieldList.push_back("NICTYPE");

    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取子网结果集
 *  参数：
 *      无
 *  返回：
 *      子网结果集
 */
sql::ResultSet * DbDataManager::GetSubnetResultSet()
{
    string SqlString = string("SELECT NETID, NCODE, SUBNET, MASK, ROUTER, RANGELOW, RANGEHIGH, DEFLEASETIME, MAXLEASETIME, MAINDNS, SUBDNS, SERVERIP FROM TBLDHCPSUBNET ORDER BY NCODE");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取路由结果集
 *  参数：
 *      无
 *  返回：
 *      路由结果集
 */
sql::ResultSet * DbDataManager::GetRouteResultSet()
{
    string SqlString = string("SELECT ROUTEID, SUBNET, MASK, GATEWAY, NICDEVICE, ISALLOWED, CHANGETIME FROM TBLROUTES");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取网卡ip
 *  参数：
 *      nicname         :   网卡名称
 *  返回：
 *      网卡ip
 */
string DbDataManager::GetNicIpAddr(string nicname)
{
    map<string, string> FieldValueList = GetNetworkParameter(nicname);
    if (FieldValueList.size() == 0)
    {

        return DEFAULT_EMPTY_STRING;
    }
    return FieldValueList["IPADDR"];
}

/*
 *  功能：
 *      网卡是否存在
 *  参数：
 *      nicname         :   网卡名
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsNicExist(string nicname)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLBESNIC WHERE NICNAME = BINARY '") + nicname + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      服务器ip是否存在
 *  参数：
 *      ipaddr          :   ip地址
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsServerIpExist(string ipaddr)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLBESNIC WHERE IPADDR = BINARY '") + ipaddr + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查子网是否存在
 *  参数：
 *      subnet          :   子网
 *      mask            :   子网掩码
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsSubnetExist(string subnet, string mask)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLDHCPSUBNET WHERE SUBNET= BINARY '") + subnet + string("' AND MASK = BINARY '") + mask + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      网卡是否为桥接
 *  参数：
 *      nicname         :   网卡名
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::IsNicBridged(string nicname)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLBESNIC WHERE NICTYPE = 1 AND NICNAME = BINARY '") + nicname + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================ 远程服务器相关 ============================//

/*
 *  功能：
 *      更新原始服务器编号
 *  参数：
 *      newserverid         :   新的服务器编号
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateOriginalServerId(string newserverid)
{
    vector<string> SqlList;

    string NewServerId = newserverid;
    string OriginalServerId = GetLocalServerId();

    // BES主机信息表
    string SqlString = string("UPDATE TBLBESINFORMATION SET SERVERID ='") + NewServerId + string("'");
    SqlList.push_back(SqlString);

    // 主机表
    SqlString = string("UPDATE TBLHOST SET ORIGINALSERVERID ='") + NewServerId + string("' WHERE ORIGINALSERVERID ='") + OriginalServerId + string("'");
    SqlList.push_back(SqlString);

    // 复制策略表
    SqlString = string("UPDATE TBLCOPYSTRATEGY T, TBLHOST S SET T.ORIGINALSERVERID = S.ORIGINALSERVERID WHERE T.HCODE = S.HCODE");
    SqlList.push_back(SqlString);

    // 策略明细表
    SqlString = string("UPDATE TBLCOPYDETAIL T, TBLCOPYSTRATEGY S SET T.ORIGINALSERVERID = S.ORIGINALSERVERID WHERE T.STRATEGYID = S.STRATEGYID");
    SqlList.push_back(SqlString);

    // 主机应急信息表
    SqlString = string("UPDATE TBLHOSTEMERGENCYINFO T, TBLHOST S SET T.ORIGINALSERVERID = S.ORIGINALSERVERID WHERE T.HCODE = S.HCODE");
    SqlList.push_back(SqlString);

    // 主机心跳监测项表
    SqlString = string("UPDATE TBLHOSTHBMONITORITEM T, TBLHOST S SET T.ORIGINALSERVERID = S.ORIGINALSERVERID WHERE T.HCODE = S.HCODE");
    SqlList.push_back(SqlString);

    // 主机IP地址表
    SqlString = string("UPDATE TBLHOSTIPADDRESS T, TBLHOST S SET T.ORIGINALSERVERID = S.ORIGINALSERVERID WHERE T.HCODE = S.HCODE");
    SqlList.push_back(SqlString);

    // 主机网卡信息表
    SqlString = string("UPDATE TBLHOSTNIC T, TBLHOST S SET T.ORIGINALSERVERID = S.ORIGINALSERVERID WHERE T.HCODE = S.HCODE");
    SqlList.push_back(SqlString);

    // 任务计划表
    SqlString = string("UPDATE TBLSCHEDULE SET ORIGINALSERVERID ='") + NewServerId + string("' WHERE ORIGINALSERVERID ='") + OriginalServerId + string("'");
    SqlList.push_back(SqlString);

    // 模板动作表
    SqlString = string("UPDATE TBLTEMPLATEACTION SET ORIGINALSERVERID ='") + NewServerId + string("' WHERE ORIGINALSERVERID ='") + OriginalServerId + string("'");
    SqlList.push_back(SqlString);

    // 导航模板表
    SqlString = string("UPDATE TBLWIZARDTEMPLATE SET ORIGINALSERVERID ='") + NewServerId + string("' WHERE ORIGINALSERVERID ='") + OriginalServerId + string("'");
    SqlList.push_back(SqlString);

    // 软件版本表
    SqlString = string("UPDATE TBLSOFTWAREVERSION SET SERVERCODE = '") + NewServerId + string("' WHERE SWUSEDON = 1 AND SERVERCODE = '") + OriginalServerId + string("'");
    SqlList.push_back(SqlString);

    return ExecuteSql(SqlList);
}

/*
 *  功能：
 *      获取服务器列表
 *  参数：
 *      type            :   远程服务器类型
 *  返回：
 *      服务器列表
 */
sql::ResultSet * DbDataManager::GetServerList(DbDataManager::RemoteServerType type)
{
    string SqlString = string("SELECT SERVERID, SERVERIP, PORT, SERVERDESC, SCHEDULEID, MAXALLOWNUM FROM TBLREMOTESERVER WHERE SERVERTYPE =") + BeGeneralLib::Number2String(type);

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取指定serverid的字段信息
 *  参数：
 *      serverid            :   服务器id
 *      servertype          :   远程服务器类型
 *  返回：
 *      字段列表
 */
map<string, string> DbDataManager::GetServerFieldValue(string serverid, DbDataManager::RemoteServerType servertype)
{
    vector<string> FieldList;
    FieldList.push_back("SERVERID");
    FieldList.push_back("SERVERIP");
    FieldList.push_back("PORT");
    FieldList.push_back("SERVERTYPE");
    FieldList.push_back("MAXALLOWNUM");
    string FieldNameString = GetFieldNameString(FieldList);
    string SqlString = string("SELECT ") + FieldNameString + string(" FROM TBLREMOTESERVER WHERE SERVERID = '") + serverid + string("' AND SERVERTYPE =") + BeGeneralLib::Number2String(servertype);

    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取指定serverid和类型的ip
 *  参数：
 *      serverid            :   服务器id
 *      servertype          :   远程服务器类型
 *  返回：
 *      服务器ip
 */
string DbDataManager::GetServerIp(string serverid, DbDataManager::RemoteServerType servertype)
{
    string SqlString = string("SELECT SERVERIP FROM TBLREMOTESERVER WHERE SERVERID = '") + serverid + string("' AND SERVERTYPE =") + BeGeneralLib::Number2String(servertype);

    return GetFieldValue(SqlString, "SERVERIP");
}

/*
 *  功能：
 *      获取指定serverid和类型的端口
 *  参数：
 *      serverid            :   服务器id
 *      servertype          :   远程服务器类型
 *  返回：
 *      服务器端口
 */
string DbDataManager::GetServerPort(string serverid, DbDataManager::RemoteServerType servertype)
{
    string SqlString = string("SELECT PORT FROM TBLREMOTESERVER WHERE SERVERID = '") + serverid + string("' AND SERVERTYPE =") + BeGeneralLib::Number2String(servertype);

    return GetFieldValue(SqlString, "PORT");
}

/*
 *  功能：
 *      获取本机服务器id
 *  参数：
 *      无
 *  返回：
 *      本机服务器id
 */
string DbDataManager::GetLocalServerId()
{
    string SqlString = string("SELECT SERVERID FROM TBLBESINFORMATION");

    return GetFieldValue(SqlString, "SERVERID");
}

/*
 *  功能：
 *      获取指定serverid的字段信息
 *  参数：
 *      serverid         :   服务器id
 *  返回：
 *      字段列表
 */
bool DbDataManager::IsServerExist(string serverid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLREMOTESERVER WHERE SERVERID = '") + serverid + string("'");

    return (GetFieldValueBool(SqlString, "RECCOUNT"));
}

/*
 *  功能：
 *      检查指定serverid和类型的服务器是否存在
 *  参数：
 *      serverid            :   服务器id
 *      servertype          :   服务器类型
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsServerExist(string serverid, DbDataManager::RemoteServerType servertype)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLREMOTESERVER WHERE SERVERID = '") + serverid + string("' AND SERVERTYPE =") + BeGeneralLib::Number2String(servertype);

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查指定类型的服务器是否存在
 *  参数：
 *      servertype          :   服务器类型
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsServerExist(DbDataManager::RemoteServerType servertype)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLREMOTESERVER WHERE SERVERTYPE =") + BeGeneralLib::Number2String(servertype);

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查设备编号是否已经存在
 *  参数：
 *      serverid        :   服务器id
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsServerIdExistInLocal(string serverid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST WHERE ORIGINALSERVERID ='") + serverid + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================== 报警相关 ==================================//

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
bool DbDataManager::AddAlarm(string hcode, string event, string content, DbDataManager::AlarmLevel level, string starttime, string endtime)
{
    int AlarmState = 0;
    if (level == DbDataManager::ALARM_LEVEL_INFORMATION || level == DbDataManager::ALARM_LEVEL_ATTENTION)
    {
        AlarmState = 1;
    }

    string SqlInsert = string("INSERT INTO TBLALARM(HCODE, EVENT, CONTENT, LEVEL, STATE, STARTTIME, ENDTIME, REPORTSTATE, CREATETIME, UPDATETIME) VALUES('") + hcode + string("',") +
            string("'") + event + string("',") +
            string("'") + content + string("',") +
            BeGeneralLib::Number2String(level) + string(",") +
            BeGeneralLib::Number2String(AlarmState) + string(",") +
            string("'") + starttime + string("',") +
            string("'") + endtime + string("', 0, NOW(), NOW())");

    return ExecuteSql(SqlInsert);
}

/*
 *  功能：
 *      更新报警
 *  参数：
 *      alarmid         :   报警id
 *      state           :   状态
 *  返回：
 *      操作成功返回ture，否则返回false
 */
bool DbDataManager::UpdateAlarm(string alarmid, int reportstate)
{
    string SqlString = string("UPDATE TBLALARM SET REPORTSTATE = ") + BeGeneralLib::Number2String(reportstate) + string(", UPDATETIME = NOW() WHERE ALARMID = ") + alarmid;

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateAlarmReportState(DbDataManager::AlarmReportMode reportmode, string alarmid, DbDataManager::AlarmReportState reportstate)
{
    string SqlString = "";
    switch (reportmode)
    {
        case DbDataManager::ALARM_REPORT_MODE_BMS:
        {
            SqlString = string("UPDATE TBLALARM SET REPORTSTATE = ") + BeGeneralLib::Number2String((int) reportstate) + string(", UPDATETIME = NOW() WHERE ALARMID = ") + alarmid;
            break;
        }
        case DbDataManager::ALARM_REPORT_MODE_SMS:
        {
            SqlString = string("UPDATE TBLALARM SET SMSREPORTSTATE = ") + BeGeneralLib::Number2String((int) reportstate) + string(", UPDATETIME = NOW() WHERE ALARMID = ") + alarmid;
            break;
        }
        case DbDataManager::ALARM_REPORT_MODE_MAIL:
        {
            SqlString = string("UPDATE TBLALARM SET EMAILREPORTSTATE = ") + BeGeneralLib::Number2String((int) reportstate) + string(", UPDATETIME = NOW() WHERE ALARMID = ") + alarmid;

            break;
        }

    }
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取报警记录
 *  参数：
 *      无
 *  返回：
 *      报警结果集
 */
sql::ResultSet * DbDataManager::GetAlarmResultSet()
{
    string SqlString =
    		string("SELECT T.ALARMID, T.HCODE, T.EVENT, T.CONTENT, T.STATE, T.LEVEL, T.REPORTSTATE, T.SMSREPORTSTATE, T.EMAILREPORTSTATE, T.STARTTIME, T.ENDTIME, H.IPADDR, S.ALARMMODE, DATEDIFF(NOW(), T.STARTTIME) AS DIFFDAYS ") +
    		string("  FROM TBLALARM T, TBLALARMCONFIG S, TBLHOST H ") +
    		string(" WHERE T.STATE = 0 ") +
    		string("   AND ((T.REPORTSTATE = 0) OR (T.SMSREPORTSTATE = 0 AND (S.ALARMMODE = 1 OR S.ALARMMODE = 3)) OR (T.EMAILREPORTSTATE = 0 AND (S.ALARMMODE = 2 OR S.ALARMMODE = 3))) ") +
    		string("   AND T.LEVEL >= S.DEFAULTSHOWLEVEL ") +
    		string("   AND T.HCODE = H.HCODE ") +
    		string("ORDER BY T.ALARMID ASC");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取报警接收人结果集
 *  参数：
 *      无
 *  返回：
 *      报警接收人结果集
 */
sql::ResultSet * DbDataManager::GetAlarmReceiverResultSet()
{
    string SqlString = string("SELECT RECEIVER, PHONENUMBER, EMAIL FROM TBLALARMRECEIVER");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取报警类型
 *  参数：
 *      无
 *  返回：
 *      报警类型
 */
DbDataManager::AlarmMode DbDataManager::GetAlarmMode()
{
    string SqlString = string("SELECT ALARMMODE FROM TBLALARMCONFIG");

    return (DbDataManager::AlarmMode)(GetFieldValueInt(SqlString, "ALARMMODE"));
}

/*
 *  功能：
 *      检查是否有未上报的报警
 *  参数：
 *      无
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::HasUnreportedAlarm()
{
    string SqlString = string("SELECT COUNT( * ) AS RECCOUNT FROM TBLALARM T, TBLALARMCONFIG S, (SELECT COUNT( * ) AS REC FROM TBLALARMRECEIVER WHERE LENGTH(TRIM(PHONENUMBER)) >1) P, (SELECT COUNT( * ) AS REC FROM TBLALARMRECEIVER WHERE LENGTH(TRIM(EMAIL)) >1) Q, "
            "(SELECT COUNT( * ) AS SMSCOUNT FROM TBLREMOTESERVER WHERE SERVERTYPE =4 )R , (SELECT COUNT( * ) AS BMSCOUNT FROM TBLREMOTESERVER WHERE SERVERTYPE =2)M "
            "WHERE (T.SMSREPORTSTATE =0 AND T.LEVEL >= S.DEFAULTSHOWLEVEL AND (S.ALARMMODE =1 OR S.ALARMMODE =3) AND R.SMSCOUNT >0 AND P.REC > 0) "
            "OR (T.REPORTSTATE =0 AND M.BMSCOUNT >0) OR (T.EMAILREPORTSTATE =0 AND T.LEVEL >= S.DEFAULTSHOWLEVEL AND (S.ALARMMODE =2 OR S.ALARMMODE =3)  AND Q.REC > 0)");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================ 存储空间相关 =============================//

/*
 *  功能：
 *      更新存储空间
 *  参数：
 *      无
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStorageSpace()
{
    //string SqlString = "UPDATE TBLSTORAGESPACE V SET USEDSPACE = (SELECT SUM(S.CAPACITY) FROM TBLCOPYSTRATEGY S, TBLHOST T WHERE S.HCODE = T.HCODE AND T.STGID = V.STGID)";
    // string SqlString = string("UPDATE TBLSTORAGESPACE V SET USEDSPACE = ( SELECT SUM( CAPACITY ) FROM ((SELECT T.CAPACITY,T.STGID FROM TBLCOPYSTRATEGY T, TBLSNAPSHOTCOPY S WHERE T.HCODE = S.HCODE AND T.LMOUNTPOINT = S.LMOUNTPOINT)) W WHERE W.STGID = V.STGID) + (V.CAPACITY * 0.3)");
    //string SqlString = string("UPDATE TBLSTORAGESPACE V SET USEDSPACE = ( SELECT SUM( CAPACITY ) FROM ((SELECT T.CAPACITY,T.STGID FROM TBLCOPYSTRATEGY T, TBLSNAPSHOTCOPY S WHERE T.HCODE = S.HCODE AND T.LMOUNTPOINT = S.LMOUNTPOINT)) W WHERE W.STGID = V.STGID) + (V.CAPACITY *(IF(TRIM(LOWER(V.FSTYPE)) = 'zfs',0, 0.3)))");
    string SqlString = string("UPDATE TBLSTORAGESPACE V SET USEDSPACE = (SELECT IFNULL(SUM( CAPACITY ),0) AS TOTALCAPACITY FROM ((SELECT T.CAPACITY,T.STGID FROM TBLCOPYSTRATEGY T, TBLSNAPSHOTCOPY S, TBLSTORAGESPACE R WHERE T.HCODE = S.HCODE AND T.LMOUNTPOINT = S.LMOUNTPOINT AND R.STGID = T.STGID)  UNION ALL (SELECT X.CAPACITY,X.STGID FROM TBLCOPYSTRATEGY X, TBLSTORAGESPACE Y WHERE  Y.STGID = X.STGID AND X.STRATEGYTYPE = 'S' AND TRIM(LOWER(Y.FSTYPE)) != 'zfs'))W WHERE W.STGID = V.STGID)");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新存储空间设备名词
 *  参数：
 *      storageid       :   存储空间id
 *      devicename      :   设备名称
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStorageDeviceName(string storageid, string devicename)
{
    string SqlString = string("UPDATE TBLSTORAGESPACE SET DEVICE = '") + devicename + string("' WHERE STGID = ") + storageid;

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新存储空间文件系统
 *  参数：
 *      storageid       :   存储空间id
 *      filesystem      :   文件系统
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStorageFileSystem(string storageid, string filesystem)
{
    string SqlString = string("UPDATE TBLSTORAGESPACE SET FSTYPE = '") + filesystem + string("' WHERE STGID = ") + storageid;

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新存储空间挂载点
 *  参数：
 *      storageid       :   存储空间id
 *      path            :   路径
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStorageMountPoint(string storageid, string path)
{
    string SqlString = string("UPDATE TBLSTORAGESPACE SET MOUNTPOINT = '") + path + string("' WHERE STGID = ") + storageid;

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新存储空间设备名词
 *  参数：
 *      storageid       :   存储空间id
 *      cleaned         :   是否清理过的标志
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStorageCleanState(string storageid, bool cleaned)
{
    string SqlString = string("UPDATE TBLSTORAGESPACE SET CLEANEDSTATE =") + BeGeneralLib::Number2String(cleaned) + SEPARATOR_CHAR_SPACE + string(" WHERE STGID = ") + storageid;

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新存储空间清理启动值
 *  参数：
 *      storageid       :   存储空间id
 *      limit           :   启动门限值
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateStorageCleanLimit(string storageid, int limit)
{
    string SqlString = string("UPDATE TBLSTORAGESPACE SET CLEANLIMIT =") + BeGeneralLib::Number2String(limit) + SEPARATOR_CHAR_SPACE + string(" WHERE STGID = ") + storageid;

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取存储空间字段值
 *  参数：
 *      storageid       :   存储空间id
 *  返回：
 *      存储空间字段值
 */
map<string, string> DbDataManager::GetStorageFieldValue(string storageid)
{
    string SqlString = string("SELECT STGID, DEVICE, MOUNTPOINT, FSTYPE, BLOCKID, STARTLOCATION, ENDLOCATION, CAPACITY, USEDSPACE, USEDLIMIT, CLEANLIMIT, CLEANSTEP, CLEANSTATE, LASTCLEANTIME FROM TBLSTORAGESPACE WHERE STGID =") + storageid;
    vector<string> FieldList;
    FieldList.push_back("STGID");
    FieldList.push_back("DEVICE");
    FieldList.push_back("MOUNTPOINT");
    FieldList.push_back("FSTYPE");
    FieldList.push_back("BLOCKID");
    FieldList.push_back("STARTLOCATION");
    FieldList.push_back("ENDLOCATION");
    FieldList.push_back("CAPACITY");
    FieldList.push_back("USEDSPACE");
    FieldList.push_back("USEDLIMIT");
    FieldList.push_back("CLEANLIMIT");
    FieldList.push_back("CLEANSTEP");
    FieldList.push_back("CLEANSTATE");
    FieldList.push_back("LASTCLEANTIME");

    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      根据存储空间id获取已用存储空间
 *  参数：
 *      storageid      :   存储空间id
 *  返回：
 *      已用存储空间
 */
string DbDataManager::GetUsedStorageSpace(string storageid)
{
    //string SqlString = string("SELECT TOTALCAPACITY FROM (((SELECT SUM(CAPACITY * 2) AS TOTALCAPACITY FROM TBLCOPYSTRATEGY WHERE STRATEGYTYPE = 'S' AND STGID = ") + storageid + string(") UNION (SELECT SUM(CAPACITY) AS TOTALCAPACITY FROM TBLCOPYSTRATEGY WHERE STRATEGYTYPE = 'D' AND STGID = ") + storageid + string(")) V)");
    //    string SqlString = string("SELECT SUM(S.CAPACITY * (IF(TRIM(LOWER(T.FSTYPE)) != 'zfs' AND S.STRATEGYTYPE = 'S', 2, 1))) AS TOTALCAPACITY FROM TBLCOPYSTRATEGY S, TBLSTORAGESPACE T WHERE T.STGID = S.STGID AND S.STGID = ") + storageid;

    // 更新存储使用空间
    UpdateStorageSpace();

    // 查询已用空间
    string SqlString = string("SELECT USEDSPACE FROM TBLSTORAGESPACE WHERE STGID = ") + storageid;

    return GetFieldValue(SqlString, "USEDSPACE");
}

/*
 *  功能：
 *      获取存储空间结果集
 *  参数：
 *      无
 *  返回：
 *      存储空间结果集
 */
sql::ResultSet * DbDataManager::GetStorageSpaceResultSet()
{
    string SqlString = string("SELECT STGID, DEVICE, MOUNTPOINT, FSTYPE, BLOCKID, STARTLOCATION, ENDLOCATION, CAPACITY, USEDSPACE, USEDLIMIT, CLEANLIMIT, CLEANSTEP, CLEANSTATE, LASTCLEANTIME FROM TBLSTORAGESPACE ORDER BY STGID");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      检查存储空间是否存在
 *  参数：
 *      alarmid         :   报警id
 *      state           :   状态
 *  返回：
 *      操作成功返回ture，否则返回false
 */
bool DbDataManager::IsStorageExist(string storageid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSTORAGESPACE WHERE STGID = ") + storageid;

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查存储空间是否有CDP保护
 *  参数：
 *      storageid       :   存储空间id
 *  返回：
 *      如果有返回ture，否则返回false
 */
bool DbDataManager::IsStorageHasCDP(string storageid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLSTORAGESPACE T, TBLCOPYSTRATEGY S WHERE T.STGID = ") + storageid + string(" AND T.STGID = S.STGID AND S.USEDFORCDP = 1");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查存储空间是否需要做定时快照
 *  参数：
 *      无
 *  返回：
 *      如果有返回ture，否则返回false
 */
bool DbDataManager::IsStorageNeedTimingSnapshot()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST T, TBLBESINFORMATION S, TBLCOPYSTRATEGY R, TBLCDPCONFIG M, TBLSTORAGESPACE N WHERE M.KEEPTIME > 0 AND M.CDPNUMBER > 0 AND S.MAXCDP > 0 AND S.MAXCDPKEEPTIME > 0 AND R.USEDFORCDP = 1 AND T.HCODE = R.HCODE AND R.STGID = N.STGID AND N.FSTYPE = 'zfs'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================ 光纤卡相关 ================================//

bool DbDataManager::ClearFCAdapter()
{
    string SqlString = string("DELETE FROM TBLBESFC");
    return ExecuteSql(SqlString);
}

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
bool DbDataManager::AddFCAdapter(string fcname, string wwn, string desc)
{
    string SqlString = string("INSERT INTO TBLBESFC (FCNAME, DESCRIPTION, WWN, UPDATETIME) VALUES('") + fcname + string("', '") + desc + string("','") + wwn + string("', NOW())");
    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取光纤卡结果集
 *  参数：
 *      无
 *  返回：
 *      光纤卡结果集
 */
sql::ResultSet * DbDataManager::GetFCResultSet()
{
    string SqlString = string("SELECT FCNAME, DESCRIPTION, WWN, UPDATETIME FROM TBLBESFC ORDER BY FCNAME");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      获取存储空间字段值
 *  参数：
 *      fcname          :   光纤卡名称
 *  返回：
 *      存储空间字段值
 */
map<string, string> DbDataManager::GetFCFieldValue(string fcname)
{
    // 设置sql语句
    string SqlString = string("SELECT FCNAME, DESCRIPTION, WWN, UPDATETIME FROM TBLBESFC WHERE FCNAME = BINARY '") + fcname + string("'");

    // 设置字段列表
    vector<string> FieldList;
    FieldList.push_back("FCNAME");
    FieldList.push_back("DESCRIPTION");
    FieldList.push_back("WWN");
    FieldList.push_back("UPDATETIME");

    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取指定fc的WWN
 *  参数：
 *      fcname          :   光纤卡名称
 *  返回：
 *      fc的WWN
 */
string DbDataManager::GetFCWWN(string fcname)
{
    string SqlString = string("SELECT WWN FROM TBLBESFC WHERE FCNAME = BINARY '") + fcname + string("'");

    return GetFieldValue(SqlString, "WWN");
}

/*
 *  功能：
 *      检查光纤卡是否存在
 *  参数：
 *      fcname          :   光纤卡名称
 *  返回：
 *      如果存在返回ture，否则返回false
 */
bool DbDataManager::IsFCNameExist(string fcname)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLBESFC WHERE FCNAME = BINARY '") + fcname + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查光纤卡是否存在
 *  参数：
 *      wwn             :   光纤卡wwn
 *  返回：
 *      如果存在返回ture，否则返回false
 */
bool DbDataManager::IsFCWWNExist(string wwn)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLBESFC WHERE WWN =BINARY '") + wwn + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================ 虚拟服务器相关 ================================//

/*
 *  功能：
 *      获取虚拟主机结果集
 *  参数：
 *      无
 *  返回：
 *      虚拟主机结果集
 */
sql::ResultSet * DbDataManager::GetVMHostResultSet()
{
    string SqlString = string("SELECT VMHOSTID, SERVERID, HCODE, USED, CREATETIME, UPDATETIME FROM TBLVMHOST");

    return GetResultSet(SqlString);
}

/*
 *  功能：
 *      检查虚拟主机对应的虚拟服务器id
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      虚拟服务器id
 */
string DbDataManager::GetVMServerId(string hcode)
{
    string SqlString = string("SELECT SERVERID FROM TBLVMHOST WHERE HCODE ='") + hcode + string("'");

    return GetFieldValue(SqlString, "SERVERID");
}

/*
 *  功能：
 *      检查虚拟主机是否可用
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      如果可用返回ture，否则返回false
 */
bool DbDataManager::IsVMHostUsed(string hcode)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLVMHOST T, TBLREMOTESERVER V WHERE T.SERVERID = V.SERVERID AND V.SERVERTYPE = 3  AND T.USED = 1 AND T.HCODE = '") + hcode + string("'");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

//================================== CDP个数 ==================================//

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
bool DbDataManager::AddCDPConfig(int cdpnumber, int keeptime, int keepunit)
{
    string SqlString = string("INSERT INTO TBLCDPCONFIG (ORIGINALSERVERID,SERVERID,CDPNUMBER,KEEPTIME,KEEPUNIT,CREATETIME,UPDATETIME) VALUES(") + GetLocalServerId() +
            string(",") + GetLocalServerId() +
            string(",") + BeGeneralLib::Number2String(cdpnumber) +
            string(",") + BeGeneralLib::Number2String(keeptime) +
            string(",") + BeGeneralLib::Number2String(keepunit) +
            string(",NOW(), NOW())");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取设置的CDP个数
 *  参数：
 *      无
 *  返回：
 *      设置的CDP个数
 */
int DbDataManager::GetCDPNumber()
{
    string SqlString = string("SELECT CDPNUMBER FROM TBLCDPCONFIG");

    return GetFieldValueInt(SqlString, "CDPNUMBER");
}

/*
 *  功能：
 *      获取设置的CDP连续快照时间
 *  参数：
 *      无
 *  返回：
 *      设置的CDP连续快照时间
 */
int DbDataManager::GetCDPKeepTime()
{
    string SqlString = string("SELECT KEEPTIME FROM TBLCDPCONFIG");

    return GetFieldValueInt(SqlString, "KEEPTIME");
}

/*
 *  功能：
 *      获取设置的CDP连续快照时间单位
 *  参数：
 *      无
 *  返回：
 *      设置的CDP连续快照时间单位
 */
int DbDataManager::GetCDPKeepUnit()
{
    string SqlString = string("SELECT KEEPUNIT FROM TBLCDPCONFIG");

    return GetFieldValueInt(SqlString, "KEEPUNIT");
}

/*
 *  功能：
 *      获取cdp连续恢复点时间间隔
 *  参数：
 *      无
 *  返回：
 *      cdp连续恢复点时间间隔
 */
int DbDataManager::GetCDPCheckpointInterval()
{
    string SqlString = string("SELECT CHECKPOINTINTERVAL FROM TBLCDPCONFIG");

    return GetFieldValueInt(SqlString, "CHECKPOINTINTERVAL");
}

/*
 *  功能：
 *      检查CDP配置是否存在
 *  参数：
 *      无
 *  返回：
 *      存在返回true，否则返回false
 */
bool DbDataManager::IsCDPConfigExist()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLCDPCONFIG");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      更新cdp连续快照保留时间
 *  参数：
 *      keeptime        :   保留时间
 *      keepunit        :   时间单位
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateCDPSnapshotKeepTime(int keeptime, int keeptimeunit)
{
    string SqlString = string("UPDATE TBLCDPCONFIG SET KEEPTIME = ") + BeGeneralLib::Number2String(keeptime) + string(", KEEPUNIT = ") + BeGeneralLib::Number2String(keeptimeunit);

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新cdp个数
 *  参数：
 *      number          :   cdp个数
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateCDPNumber(int number)
{
    string SqlString = string("UPDATE TBLCDPCONFIG SET CDPNUMBER = ") + BeGeneralLib::Number2String(number);

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新cdp连续快照保留时间
 *  参数：
 *      number          :   cdp个数
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateCDPCheckpointInterval(int interval)
{
    string SqlString = string("UPDATE TBLCDPCONFIG SET CHECKPOINTINTERVAL = ") + BeGeneralLib::Number2String(interval);

    return ExecuteSql(SqlString);
}

//================================ 心跳相关 ================================//

/*
 *  功能：
 *      获取主机心跳监测项结果集
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      成功返回主机心跳监测项结果集，否则返回NULL
 */
sql::ResultSet * DbDataManager::GetHostHeartbeatMonitorItemResultSet(string hcode)
{
    string SqlString = string("SELECT S.HCODE, S.QUERYINTERVAL, S.RETRYNUM, S.MAXTIMEOUT, S.MINTIMEOUT, S.UPPERLIMIT, S.LOWERLIMIT, S.MONITORORDER, T.ITEMNAME, T.APPNAME, T.PORTTYPE, T.PORTNUMBER   FROM TBLHOSTHBMONITORITEM S, TBLHEARTBEATITEMDIC T WHERE S.MONITORITEM = T.ITEMNAME AND S.STATE = 1 AND S.HCODE ='") + hcode + string("' ORDER BY MONITORORDER");

    return GetResultSet(SqlString);
}

//================================ 硬件应急相关 ================================//

/*
 *  功能：
 *      获取硬件应急配置参数
 *  参数：
 *      无
 *  返回：
 *      配置参数列表
 */
map<string, string> DbDataManager::GetHWEmergencyConfig()
{
    string SqlString = string("SELECT MAXKEYNUMBER, COMPORT, BAUDRATE, PARITY, DATABITS, STOPBITS FROM TBLHWECONFIG");
    vector<string> FieldList;
    FieldList.push_back("MAXKEYNUMBER");
    FieldList.push_back("COMPORT");
    FieldList.push_back("BAUDRATE");
    FieldList.push_back("PARITY");
    FieldList.push_back("DATABITS");
    FieldList.push_back("STOPBITS");

    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取按键绑定的主机编码
 *  参数：
 *      keyid           :   按键编号
 *  返回：
 *      如果成功返回主机编码，否则返回空
 */
string DbDataManager::GetHCodeBindByKeyId(string keyid)
{
    string SqlString = string("SELECT T.HCODE FROM TBLHOST T, TBLHWEHOST S WHERE T.HCODE = S.HCODE AND S.USEDFORHWE = 1 AND S.KEYID = ") + keyid;

    return GetFieldValue(SqlString, "HCODE");
}

/*
 *  功能：
 *      获取主机编码绑定的按键编号
 *  参数：
 *      hcode           :   主机编码
 *  返回：
 *      如果成功返回按键编号，否则返回空
 */
string DbDataManager::GetKeyIdBindByHCode(string hcode)
{
    string SqlString = string("SELECT S.KEYID FROM TBLHOST T, TBLHWEHOST S WHERE T.HCODE = S.HCODE AND S.USEDFORHWE = 1 AND S.HCODE = '") + hcode + string("'");

    return GetFieldValue(SqlString, "KEYID");
}

/*
 *  功能：
 *      是否有硬件应急配置参数
 *  参数：
 *      无
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::HasHWEmergencyConfig()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHWECONFIG");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      检查指定键编号是否用于主机硬件应急
 *  参数：
 *      keyid           :   按键编号
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsKeyUsedForHost(string keyid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHWEHOST WHERE KEYID = ") + keyid;

    return GetFieldValueBool(SqlString, "RECCOUNT");
}

/*
 *  功能：
 *      是否有硬件应急绑定
 *  参数：
 *
 *  返回：
 *      如果有返回true，否则返回false
 */
bool DbDataManager::IsHWEmergencyBind()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLHOST T, TBLHWEHOST S, TBLHWECONFIG R WHERE T.HCODE = S.HCODE AND S.USEDFORHWE = 1 AND S.KEYID <= R.MAXKEYNUMBER");

    return GetFieldValueBool(SqlString, "RECCOUNT");
}


//================================ 命令相关 ================================//

/*
 *  功能：
 *      获取命令字描述信息
 *  参数：
 *      cmdcode         :   命令字描述信息
 *  返回：
 *      命令字描述信息
 */
string DbDataManager::GetCommandDescription(string cmdcode)
{
    string SqlString = string("SELECT DESCRIPTION FROM TBLCOMMANDCODE WHERE CCODE = '") + cmdcode + string("'");

    return GetFieldValue(SqlString, "DESCRIPTION");
}

//================================== 其他相关 ==================================//

/*
 *  功能：
 *      更新任务许可文件
 *  参数：
 *      license         :   许可文件
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateLicenseName(string license)
{
    string SqlString = string("UPDATE TBLBESINFORMATION SET LICENSENAME = '") + license + string("', LICENSEALARM = '?'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      更新license报警信息
 *  参数：
 *      alarm           :   报警信息
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool DbDataManager::UpdateLicenseAlarm(string alarm)
{
    string SqlString = string("UPDATE TBLBESINFORMATION SET LICENSEALARM = '") + alarm + string("'");

    return ExecuteSql(SqlString);
}

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
bool DbDataManager::UpdateLicenseNumber(int host, int db, int snapshot, int net, int vm, int fc, int cdp, int cdpkeeptime, int cdpkeepunit, int eanumber, string expireddate)
{
    string SqlString = string("UPDATE TBLBESINFORMATION SET MAXHOST=") + BeGeneralLib::Number2String(host)
            + string(", MAXDATABASE=") + BeGeneralLib::Number2String(db)
            + string(", MAXSNAPSHOT=") + BeGeneralLib::Number2String(snapshot)
            + string(", MAXNET =") + BeGeneralLib::Number2String(net)
            + string(", MAXVM=") + BeGeneralLib::Number2String(vm)
            + string(", MAXFC=") + BeGeneralLib::Number2String(fc)
            + string(", MAXCDP=") + BeGeneralLib::Number2String(cdp)
            + string(", MAXCDPKEEPTIME=") + BeGeneralLib::Number2String(cdpkeeptime)
            + string(", MAXCDPKEEPUNIT=") + BeGeneralLib::Number2String(cdpkeepunit)
            + string(", MAXEANUMBER=") + BeGeneralLib::Number2String(eanumber)
            + string(", EXPIREDDATE='") + expireddate + string("'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      获取Agent参数
 *  参数：
 *      nicname         :   网卡名
 *  返回：
 *      Agent参数
 */
map<string, string> DbDataManager::GetAgentParameter(string nicname)
{
    string SqlString = string("SELECT T.DBUSERNAME, T.DBPASSWORD, V.IPADDR FROM TBLBESINFORMATION T, TBLBESNIC V WHERE V.NICNAME = BINARY '") + nicname + string("'");
    vector<string> FieldList;
    FieldList.push_back("DBUSERNAME");
    FieldList.push_back("DBPASSWORD");
    FieldList.push_back("IPADDR");

    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      获取Bes类型
 *  参数：
 *      无
 *  返回：
 *      Bes类型
 */
DbDataManager::BesType DbDataManager::GetBesType()
{
    string SqlString = string("SELECT BESTYPE FROM TBLBESINFORMATION");
    string Type = GetFieldValue(SqlString, "BESTYPE");
    if (Type == "100")
    {
        return DbDataManager::BES_TYPE_SERVER;
    }
    else if (Type == "200")
    {
        return DbDataManager::BES_TYPE_STARTER;
    }
    else
    {

        return DbDataManager::BES_TYPE_SERVER;
    }
}

/*
 *  功能：
 *      获取远程服务器ip
 *  参数：
 *      无
 *  返回：
 *      远程服务器ip
 */
string DbDataManager::GetRemoteServerIp()
{
    string SqlString = string("SELECT RSERVERIP FROM TBLBESINFORMATION");

    return GetFieldValue(SqlString, "RSERVERIP");
}

/*
 *  功能：
 *      获取Bes总的虚拟机内存
 *  参数：
 *      无
 *  返回：
 *      Bes总的虚拟机内存
 */
int DbDataManager::GetBesTotalVmMemory()
{
    string SqlString = string("SELECT TOTALVMMEMORY FROM TBLBESINFORMATION");

    return GetFieldValueInt(SqlString, "TOTALVMMEMORY");
}

/*
 *  功能：
 *      获取Bes总的虚拟机cpu
 *  参数：
 *      无
 *  返回：
 *      Bes总的虚拟机cpu
 */
int DbDataManager::GetBesTotalVmCPU()
{
    string SqlString = string("SELECT TOTALVMCPU FROM TBLBESINFORMATION");

    return GetFieldValueInt(SqlString, "TOTALVMCPU");
}

/*
 *  功能：
 *      获取授权最大主机数
 *  参数：
 *      无
 *  返回：
 *      授权最大主机数
 */
int DbDataManager::GetMaxHost()
{
    string SqlString = string("SELECT MAXHOST FROM TBLBESINFORMATION");

    return GetFieldValueInt(SqlString, "MAXHOST");
}

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
void DbDataManager::SetParameter(string server, string user, string password, string dbname)
{

    MyServer = server;
    MyUser = user;
    MyPassword = password;
    MyDbName = dbname;
}

/*
 *  功能：
 *      检查数据库是否OK
 *  参数：
 *      无
 *  返回：
 *      如果是返回true，否则返回false
 */
bool DbDataManager::CheckDatabaseOK()
{

    return CheckDatabaseOK(MyServer, MyUser, MyPassword, MyDbName);
}

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
bool DbDataManager::CheckDatabaseOK(string server, string user, string password, string dbname)
{
    try
    {
        sql::Driver *SqlDriver; // MySql驱动
        sql::Connection *SqlConn; // MySql连接对象

        // 获取mysql驱动
        SqlDriver = get_driver_instance();

        // 创建连接
        SqlConn = SqlDriver->connect(server, user, password);

        // 连接到指定的数据库
        SqlConn->setSchema(dbname);

        // 关闭连接
        SqlConn->close();

        // 删除连接对象
        delete SqlConn;
    }
    catch (sql::SQLException &e)
    {
        return false;
    }
    return true;
}

/*
 *  功能：
 *      获取用户名
 *  参数：
 *      无
 *  返回：
 *      用户名
 */
string DbDataManager::User()
{

    return MyUser;
}

/*
 *  功能：
 *      获取密码
 *  参数：
 *      无
 *  返回：
 *      密码
 */
string DbDataManager::Password()
{

    return MyPassword;
}

/*
 *  功能：
 *      获取数据库名
 *  参数：
 *      无
 *  返回：
 *      数据库名
 */
string DbDataManager::DbName()
{

    return MyDbName;
}

/*
 *  功能：
 *      导出数据库
 *  参数：
 *      无
 *  返回：
 *      导出成功返回true，否则返回false
 */
bool DbDataManager::Export()
{

    return Export(BeGeneralLib::GetExecutePath());
}

/*
 *  功能：
 *      导出数据库
 *  参数：
 *      path            ：  导出到指定目录
 *  返回：
 *      数据库名
 */
bool DbDataManager::Export(string path)
{
    // 使用默认root密码1检查数据库是否OK
    if (CheckDatabaseOK(MyServer, DEFAULT_BES_ROOT_USER_NAME, DEFAULT_BES_ROOT_PASSWORD_1, MyDbName) == true)
    {
        return Export(DEFAULT_BES_ROOT_USER_NAME, DEFAULT_BES_ROOT_PASSWORD_1, path);
    }
    else if (CheckDatabaseOK(MyServer, DEFAULT_BES_ROOT_USER_NAME, DEFAULT_BES_ROOT_PASSWORD_2, MyDbName) == true) // 使用默认root密码2检查数据库是否OK
    {
        return Export(DEFAULT_BES_ROOT_USER_NAME, DEFAULT_BES_ROOT_PASSWORD_1, path);
    }
    else // 使用默认实例的用户名和密码检查数据库是否OK
    {
        return Export(MyUser, MyPassword, path);
    }
}

/*
 *  功能：
 *      获取数据库名
 *  参数：
 *      user            :   用户名
 *      password        :   密码
 *      path            ：  导出到指定目录
 *  返回：
 *      数据库名
 */
bool DbDataManager::Export(string user, string password, string path)
{
    // 检查数据库是否OK
    if (CheckDatabaseOK(MyServer, user, password, MyDbName) == false) return false;

    // 检查并创建目录
    BeGeneralLib::CheckAndCreateDir(path);

    // 设置导出文件名
    string FileName = path + LINUX_PATH_SEPCHAR + MyDbName + DEFAULT_DATABASE_DUMP_EXTENSION;

    // 导出文件
    string ShellCommand = string("mysqldump -u ") + user + string(" -p") + password + string(" -h ") + MyServer + SEPARATOR_CHAR_SPACE + MyDbName + string(" > ") + FileName;
    BeGeneralLib::ExecuteSystem(ShellCommand, false, false);

    // 检查文件是否存在
    return BeGeneralLib::IsFileExist(FileName);
}

/*
 *  功能：
 *      修复数据库
 *  返回：
 *      修复成功返回true，否则返回false
 */
bool DbDataManager::Repair()
{
    return Repair(MyUser, MyPassword);
}

/*
 *  功能：
 *      修复数据库
 *  参数：
 *      user            :   用户名
 *      password        :   密码
 *  返回：
 *      修复成功返回true，否则返回false
 */
bool DbDataManager::Repair(string user, string password)
{
    // 检查数据库是否OK
    if (CheckDatabaseOK(MyServer, user, password, MyDbName) == false) return false;

    // 设置修复日志名
    string FileName = BeGeneralLib::GetExecutePath() + LINUX_PATH_SEPCHAR + DEFAULT_DATABASE_REPAIR_LOG;

    // 修复数据库
    string ShellCommand = string("mysqlcheck -u ") + user + string(" -p") + password + string(" -h ") + MyServer + SEPARATOR_CHAR_SPACE + MyDbName + string(" -r"); // + string(" > ") + FileName;
    BeGeneralLib::ExecuteSystem(ShellCommand, true, false);

    return true;
}

/*
 *  功能：
 *      重置gui的admin密码
 *  返回：
 *      重置成功返回true，否则返回false
 */
bool DbDataManager::ResetAdminPassword()
{
    // 重置admin密码
    string SqlString = string("UPDATE users SET password ='$P$BArjSq24hmwIIn2AFEPq4J4YbwxmP.0' WHERE username = 'admin'");
    ExecuteSql(SqlString);

    // 重置administrator密码
    SqlString = string("UPDATE users SET password ='$P$Bu2mGwvi1j8U7c6nzg.V77lAnV9psf1' WHERE username = 'administrator'");

    return ExecuteSql(SqlString);
}

/*
 *  功能：
 *      备份本地数据库
 */
void DbDataManager::BackupDatabase()
{
    // 读取配置文件中备份文件个数
    int BackupNum = BeGeneralLib::StringToInt(BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_DBBACKUPNUM));
    if (BackupNum <= 0)
    {
        BackupNum = DEFAULT_DATABASE_BACKUP_NUM;
    }

    // 检查原有文件是否存在
    string Path = BeGeneralLib::GetExecutePath();

    // 检查路径是否以路径分割符结尾，如果不是，则添加后缀路径分割符
    Path = BeGeneralLib::AddPathSeparator(Path) + DEFAULT_DATABASE_DUMP_DIRECTORY;

    string FileName = DbName() + DEFAULT_DATABASE_DUMP_EXTENSION;

    // 检查备份目录
    CheckDatabaseBackupPath(Path, FileName, BackupNum);

    // 导出数据库
    Export(Path);
}

/*
 *  功能：
 *      销毁结果集
 *  参数
 *      rs              :   结果集对象
 *  返回：
 *      无
 */
void DbDataManager::DestroyResultSet(sql::ResultSet * rs)
{
    // 检查结果集是否为空
    if (rs == NULL)
    {
        return;
    }

    try
    {
        rs->close();
        delete rs;
    }
    catch (...)
    {
    }
}

//---------------------------------- 私有方法 ----------------------------------//

/*
 *  功能：
 *      获取字段值
 *  参数：
 *      sqlstring       :   sql语句
 *      field           :   字段名
 *  返回：
 *      执行成功返回字段值，否则返回空
 */
string DbDataManager::GetFieldValue(string sqlstring, string field)
{
    vector<string> FieldList;
    FieldList.push_back(field);

    map<string, string> FieldValueList = GetFieldValue(sqlstring, FieldList);
    if (FieldValueList.empty())
    {
        return DEFAULT_FIELD_RETURN_VALUE;
    }
    else
    {
        return FieldValueList[field];
    }
}

/*
 *  功能：
 *      获取字段的整数值
 *  参数：
 *      sqlstring       :   sql语句
 *      field           :   字段名
 *  返回：
 *      执行成功返回字段整数值，否则返回0
 */
int DbDataManager::GetFieldValueInt(string sqlstring, string field)
{
    return BeGeneralLib::StringToInt(GetFieldValue(sqlstring, field));
}

/*
 *  功能：
 *      获取字段的bool值
 *  参数：
 *      sqlstring       :   sql语句
 *      field           :   字段名
 *  返回：
 *      执行成功返回字段bool值，否则返回false
 */
bool DbDataManager::GetFieldValueBool(string sqlstring, string field)
{
    return BeGeneralLib::StringToBool(GetFieldValue(sqlstring, field));
}

/*
 *  功能：
 *      获取字段值
 *  参数：
 *      sqlstring       :   sql语句
 *      fldlist         :   字段列表
 *  返回：
 *      执行成功返回字段值列表
 */
map<string, string> DbDataManager::GetFieldValue(string sqlstring, vector<string> fldlist)
{
    map<string, string> FieldValueList;

    if (BeGeneralLib::IsStringEmpty(sqlstring) == true || fldlist.size() == 0)
    {
        return FieldValueList;
    }

    sql::ResultSet * TempRs = GetResultSet(sqlstring);
    try
    {
        if (TempRs != NULL && TempRs->rowsCount() > 0 && TempRs->next())
        {
            for (int i = 0; i < fldlist.size(); i++)
            {
                string FieldName = fldlist[i];
                FieldValueList.insert(pair<string, string > (FieldName, TempRs->getString(FieldName)));
            }
        }
    }
    catch (sql::SQLException &e)
    {
        string ErrInfo = string("In GetFieldValue() Sql:") + sqlstring + string("\n#ERR: ") + e.what() + string("\n");
        BesLog::DebugPrint(ErrInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
    }

    // 销毁结果集
    DestroyResultSet(TempRs);

    return FieldValueList;
}

map<string, string> DbDataManager::GetFieldValue(sql::Connection * sqlconn, string sqlstring, vector<string> fldlist)
{
    map<string, string> FieldValueList;
    if (BeGeneralLib::IsStringEmpty(sqlstring) == true || fldlist.size() == 0) return FieldValueList;

    sql::ResultSet * TempRs = GetResultSet(sqlconn, sqlstring);
    try{
        if (TempRs != NULL && TempRs->rowsCount() > 0 && TempRs->next()){
            for (int i = 0; i < fldlist.size(); i++){
                string FieldName = fldlist[i];
                FieldValueList.insert(pair<string, string > (FieldName, TempRs->getString(FieldName)));
            }
        }
    }
    catch (sql::SQLException &e){
        string ErrInfo = string("In GetFieldValue() Sql:") + sqlstring + string("\n#ERR: ") + e.what() + string("\n");
        BesLog::DebugPrint(ErrInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
    }
    DestroyResultSet(TempRs);

    return FieldValueList;
}

/*
 *  功能：
 *      获取结果集
 *  参数：
 *      sqlstring       :   sql语句
 *  返回：
 *      结果集
 */
sql::ResultSet * DbDataManager::GetResultSet(string sqlstring)
{
    if (BeGeneralLib::IsStringEmpty(sqlstring) == true)
    {
        return NULL;
    }

    // 打开数据库连接
    sql::Connection *SqlConn = OpenConnection();

    // 获取结果集
    sql::ResultSet *SqlResultSet = GetResultSet(SqlConn, sqlstring);

    // 关闭数据库连接
    CloseConnection(SqlConn);

    return SqlResultSet;
}

/*
 *  功能：
 *      获取结果集
 *  参数：
 *      sqlconn         :   数据库连接
 *      sqlstring       :   sql语句
 *  返回：
 *      结果集
 */
sql::ResultSet * DbDataManager::GetResultSet(sql::Connection * sqlconn, string sqlstring)
{
    sql::Statement *SqlStatement = NULL; // 查询对象
    sql::ResultSet *SqlResultSet = NULL; // 结果集
    try
    {
        // 创建查询对象
        SqlStatement = CreateStatement(sqlconn);

        // 执行查询返回结果集
        if (SqlStatement != NULL)
        {
            SqlResultSet = SqlStatement->executeQuery(sqlstring);
        }

        // 关闭查询对象
        CloseStatement(SqlStatement);
    }
    catch (sql::SQLException &e)
    {
        string ErrInfo = string("Sql:") + sqlstring + string("\n#ERR: ") + e.what() + string("\n");
        BesLog::DebugPrint(ErrInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);

        return NULL;
    }
    return SqlResultSet;
}

/*
 *  功能：
 *      执行sql语句
 *  参数：
 *      sqlstring       :   sql语句
 *  返回：
 *      执行成功返回true，否则返回false
 */
bool DbDataManager::ExecuteSql(string sqlstring)
{
    vector<string> SqlList;
    SqlList.push_back(sqlstring);

    return ExecuteSql(SqlList);
}

/*
 *  功能：
 *      执行sql语句
 *  参数：
 *      sqllist         :   sql语句列表
 *  返回：
 *      执行成功返回true，否则返回false
 */
bool DbDataManager::ExecuteSql(vector<string> sqllist)
{
    // 检查sql语句列表是否为空
    if (sqllist.empty() == true)
    {
        return false;
    }

    // 打开数据库连接
    sql::Connection *SqlConn = OpenConnection();

    // 执行sql语句
    bool Flag = ExecuteSql(SqlConn, sqllist);

    // 关闭数据库连接
    CloseConnection(SqlConn);

    return Flag;
}

/*
 *  功能：
 *      执行sql语句
 *  参数：
 *      sqlstring       :   sql语句
 *      sqlconn         :   数据库连接
 *  返回：
 *      执行成功返回true，否则返回false
 */
bool DbDataManager::ExecuteSql(sql::Connection * sqlconn, string sqlstring)
{
    vector<string> SqlList;
    SqlList.push_back(sqlstring);

    return ExecuteSql(sqlconn, SqlList);
}

/*
 *  功能：
 *      执行sql语句
 *  参数：
 *      sqllist         :   sql语句列表
 *      sqlconn         :   数据库连接
 *  返回：
 *      执行成功返回true，否则返回false
 */
bool DbDataManager::ExecuteSql(sql::Connection * sqlconn, vector<string> sqllist)
{
    if (sqlconn == NULL || sqllist.empty())
    {
    	BesLog::DebugPrint("execute sql failed", BesLog::LOG_MESSAGE_TYPE_INFO, false);
        return false;
    }
    sql::Statement *SqlStatement = NULL; // 查询对象
    try
    {
        // 创建查询对象
        SqlStatement = CreateStatement(sqlconn);
        if (SqlStatement != NULL)
        {
            // 执行更新
            for (int i = 0; i < sqllist.size(); i++)
            {
                SqlStatement->executeUpdate(sqllist[i]);
            }
        }
        CloseStatement(SqlStatement);
    }
    catch (sql::SQLException &e)
    {
        string SqlString = "";
        for (int i = 0; i < sqllist.size(); i++)
        {
            SqlString = SqlString + sqllist[i] + string("\n");
        }

        string ErrInfo = string("Sql:") + SqlString + string("\n#ERR: ") + e.what() + string("\n");
        BesLog::DebugPrint(ErrInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);

        return false;
    }
    return true;
}

/*
 *  功能：
 *      打开数据库连接
 *  参数：
 *      无
 *  返回：
 *      如果打开成功返回数据库连接对象，否则返回null
 */
sql::Connection * DbDataManager::OpenConnection()
{
    return OpenConnection(MyServer, MyUser, MyPassword, MyDbName);
}

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
sql::Connection * DbDataManager::OpenConnection(string server, string user, string password, string dbname)
{
    sql::Driver *SqlDriver = NULL; // MySql驱动
    sql::Connection *SqlConn = NULL; // MySql连接对象
    try
    {
        // 获取mysql驱动
        SqlDriver = get_driver_instance();

        /// 创建连接
        SqlConn = SqlDriver->connect(server, user, password);

        // 连接到指定的数据库
        SqlConn->setSchema(dbname);
		SqlConn->setAutoCommit(true);
    }
    catch (sql::SQLException &e)
    {
        string ErrInfo = string("Open connection failed #ERR: ") + e.what() + string("\n");
        BesLog::DebugPrint(ErrInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);

        return NULL;
    }
    return SqlConn;
}

/*
 *  功能：
 *      关闭数据库连接
 *  参数：
 *      sqlconn         :   数据库连接对象
 *  返回：
 *      无
 */
void DbDataManager::CloseConnection(sql::Connection * sqlconn)
{
    if (sqlconn == NULL)
    {
        return;
    }

    try
    {
        sqlconn->close();
        delete sqlconn;
    }
    catch (sql::SQLException &e)
    {

        string ErrInfo = string("Close connection failed #ERR: ") + e.what() + string("\n");
        BesLog::DebugPrint(ErrInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
    }
}

/*
 *  功能：
 *      创建statement对象
 *  参数：
 *      sqlconn         :   数据库连接对象
 *  返回：
 *      如果成功返回statement对象，否则返回null
 */
sql::Statement * DbDataManager::CreateStatement(sql::Connection * sqlconn)
{
    if (sqlconn == NULL)
    {
        return NULL;
    }

    sql::Statement * SqlStatement = NULL;
    try
    {
        // 创建查询对象
        SqlStatement = sqlconn->createStatement();

        // 设置字符集
        SqlStatement->execute("set names utf8");
    }
    catch (sql::SQLException &e)
    {

        string ErrInfo = string("Create statement failed #ERR: ") + e.what() + string("\n");
        BesLog::DebugPrint(ErrInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);

        return NULL;
    }
    return SqlStatement;
}

/*
 *  功能：
 *      关闭statement对象
 *  参数：
 *      sqlstatement    :   statement对象
 *  返回：
 *      无
 */
void DbDataManager::CloseStatement(sql::Statement * sqlstatement)
{
    if (sqlstatement == NULL)
    {
        return;
    }

    try
    {
        sqlstatement->close();
        delete sqlstatement;
    }
    catch (sql::SQLException &e)
    {

        string ErrInfo = string("Close statement failed #ERR: ") + e.what() + string("\n");
        BesLog::DebugPrint(ErrInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
    }
}

/*
 *  功能：
 *      获取快照表字段名
 *  参数：
 *      field           :   字段
 *  返回：
 *      字段名
 */
string DbDataManager::GetSnapshotFieldName(DbDataManager::SnapshotTableField field)
{
    string FieldName = "";
    switch (field)
    {
        case DbDataManager::SNAPSHOT_TABLE_FIELD_SNAPSHOTID: // 快照编号
        {
            FieldName = string("SNAPSHOTID");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_LMOUNTPOINT: // 主机挂载点
        {
            FieldName = string("LMOUNTPOINT");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_SNAPTYPE: // 快照类型
        {
            FieldName = string("SNAPTYPE");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_TRUSTED: // 是否可信
        {
            FieldName = string("TRUSTED");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_CHECKPOINTID: // 检查点id
        {
            FieldName = string("CHECKPOINTID");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_SYNCCOPYID: // 同步副本id
        {
            FieldName = string("SYNCCOPYID");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_SNAPSHOTSOURCE: // 快照源
        {
            FieldName = string("SNAPSHOTSOURCE");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_SNAPSHOTLUNID: // 快照lun编号
        {
            FieldName = string("SNAPSHOTSOURCE");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_STARTTIME: // 开始时间
        {
            FieldName = string("STARTTIME");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_ENDTIME: // 结束时间
        {
            FieldName = string("ENDTIME");
            break;
        }
        case DbDataManager::SNAPSHOT_TABLE_FIELD_SYNCSTATE: // 远程同步状态
        {
            FieldName = string("SYNCSTATE");

            break;
        }
    }
    return FieldName;
}

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
string DbDataManager::GetSnapshotFieldValue(string hcode, string snapshotid, DbDataManager::SnapshotTableField field)
{
    string FieldName = GetSnapshotFieldName(field);

    // 检查输入的有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true
            || BeGeneralLib::IsStringEmptyOrZero(snapshotid) == true
            || BeGeneralLib::IsStringEmptyOrZero(FieldName) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    // 获取副本字段值
    map<string, string> FieldValueList = GetSnapshotFieldValue(hcode, snapshotid);

    return FieldValueList[FieldName];
}

/*
 *  功能：
 *      获取副本表字段名
 *  参数：
 *      field           :   字段
 *  返回：
 *      字段名
 */
string DbDataManager::GetCopyFieldName(DbDataManager::CopyTableField field)
{
    string FieldName = "";
    switch (field)
    {
        case DbDataManager::COPY_TABLE_FIELD_SNAPSHOTID: // 快照编号
        {
            FieldName = string("SNAPSHOTID");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_LMOUNTPOINT: // 主机挂载点
        {
            FieldName = string("LMOUNTPOINT");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_IQN: // iqn
        {
            FieldName = string("IQN");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_TARGETFILE: // target文件
        {
            FieldName = string("TARGETFILE");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_MOUNTPOINT: // 挂载点
        {
            FieldName = string("MOUNTPOINT");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_USED: // 副本状态
        {
            FieldName = string("USED");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_COPYTYPE: // 副本类型
        {
            FieldName = string("COPYTYPE");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_COPYLUNID: // 副本lun编号
        {
            FieldName = string("COPYLUNID");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_CAPACITY: // 副本容量
        {
            FieldName = string("CAPACITY");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_SENDERCOPYID: // 发送端副本编号
        {
            FieldName = string("SENDERCOPYID");
            break;
        }
        case DbDataManager::COPY_TABLE_FIELD_FSNAME: // zfs文件系统名称
        {
            FieldName = string("FSNAME");

            break;
        }
    }
    return FieldName;
}

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
string DbDataManager::GetCopyFieldValue(string hcode, string copyid, DbDataManager::CopyTableField field)
{
    string FieldName = GetCopyFieldName(field);

    // 检查输入的有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true
            || BeGeneralLib::IsStringEmptyOrZero(copyid) == true
            || BeGeneralLib::IsStringEmptyOrZero(FieldName) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    // 获取副本字段值
    map<string, string> FieldValueList = GetCopyFieldValue(hcode, copyid);

    return FieldValueList[FieldName];
}

/*
 *  功能：
 *      获取主机表字段名
 *  参数：
 *      field           :   字段
 *  返回：
 *      字段名
 */
string DbDataManager::GetHostFieldName(DbDataManager::HostTableField field)
{
    string FieldName = "";
    switch (field)
    {
        case DbDataManager::HOST_TABLE_FIELD_HOSTNAME: // 主机名
        {
            FieldName = string("HOSTNAME");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_MACADDR: // 主机mac地址
        {
            FieldName = string("MACADDR");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_IPADDR: // 主机ip地址
        {
            FieldName = string("IPADDR");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_STATUS: // 保护状态
        {
            FieldName = string("STATUS");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_ISACTIVE: // 是否为激活
        {
            FieldName = string("ISACTIVE");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_HBAIPADDR: // HBA卡的ip地址
        {
            FieldName = string("HBAIPADDR");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_DBPROTECTED: // 数据库是否保护标志
        {
            FieldName = string("DBPROTECTED");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_OSTYPE: // 操作系统类型
        {
            FieldName = string("OSTYPE");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_NETBOOTPROTOCOL: // 启动协议
        {
            FieldName = string("NETBOOTPROTOCOL");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_NETDEFAULTPROTOCOL: // 默认网启协议
        {
            FieldName = string("NETDEFAULTPROTOCOL");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_FCWWN: // 主机光纤卡WWN
        {
            FieldName = string("FCWWN");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_SYSTEMDIR: // 系统目录
        {
            FieldName = string("SYSTEMDIR");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_CONFIGDIR: // 配置目录
        {
            FieldName = string("CONFIGDIR");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_VMMEMORY: // 虚拟机内存
        {
            FieldName = string("VMMEMORY");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_VMCPU: // 虚拟机CPU个数
        {
            FieldName = string("VMCPU");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_LISTENPORT: // 监听端口
        {
            FieldName = string("LISTENPORT");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_USEDFORRDR: // 用于容灾
        {
            FieldName = string("USEDFORRDR");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_USEDFORLE: // 用于本地应急
        {
            FieldName = string("USEDFORLE");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_BUSINESSNICNAME: // 业务网卡名
        {
            FieldName = string("BUSINESSNICNAME");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_BRNICNAME: // 桥接网卡名
        {
            FieldName = string("BRNICNAME");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_COPYMODE: // 复制方式
        {
            FieldName = string("COPYMODE");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_SYNCFLAG: // 同步主机信息标志
        {
            FieldName = string("SYNCFLAG");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_FCNAME: // 光纤卡名称
        {
            FieldName = string("FCNAME");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_FCCHANNEL: // 光纤通道号
        {
            FieldName = string("FCCHANNEL");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_INITIATORNAME: // initiator名称
        {
            FieldName = string("INITIATORNAME");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_USEDFORNET: // 用于网启应急
        {
            FieldName = string("USEDFORNET");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_USEDFORVM: // 用于虚拟机应急
        {
            FieldName = string("USEDFORVM");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_USEDFORFC: // 用于光纤卡应急
        {
            FieldName = string("USEDFORFC");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_USEDFOREA: // 是否用于自动应急
        {
            FieldName = string("USEDFOREA");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_EAMODE: // 自动应急方式
        {
            FieldName = string("EAMODE");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_HEARTBEATTIMEOUT: // 心跳超时时间
        {
            FieldName = string("HEARTBEATTIMEOUT");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_QUERYINTERVAL: // 查询间隔
        {
            FieldName = string("QUERYINTERVAL");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_VIRTUALIP: // 虚拟ip
        {
            FieldName = string("VIRTUALIP");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_VIRTUALMAC: // 虚拟mac地址
        {
            FieldName = string("VIRTUALMAC");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_RACGROUPID: // rac组编号
        {
            FieldName = string("RACGROUPID");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_EMERGENCYLOGONMODE: // 应急时副本挂载方式
        {
            FieldName = string("EMERGENCYLOGONMODE");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_USEDFORMONITOR: // 是否用于监控
        {
            FieldName = string("USEDFORMONITOR");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_ISAUTOTAKEOVER: // 是否自动接管
        {
            FieldName = string("ISAUTOTAKEOVER");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_VMBOOTCOPYMODE: // 虚拟机启动副本模式
        {
            FieldName = string("VMBOOTCOPYMODE");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_VNCNUMBER: // vnc号
        {
            FieldName = string("VNCNUMBER");
            break;
        }
        case DbDataManager::HOST_TABLE_FIELD_MIRRORMODE: // 镜像类型
        {
        	FieldName = string("MIRRORMODE");
        	break;
        }
        case DbDataManager::HOST_TABLE_FIELD_MIRRORSTATE: // 镜像状态
        {
        	FieldName = string("MIRRORSTATE");
        	break;
        }
    }
    return FieldName;
}

/*
 *  功能：
 *      获取主机表字段名
 *  参数：
 *      hcode           :   主机编码
 *      field           :   字段
 *  返回：
 *      字段名
 */
string DbDataManager::GetHostFieldValue(string hcode, DbDataManager::HostTableField field)
{
    string FieldName = GetHostFieldName(field);

    // 检查输入的有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true
            || BeGeneralLib::IsStringEmptyOrZero(FieldName) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    // 获取主机字段值
    map<string, string> FieldValueList = GetHostFieldValue(hcode);

    return FieldValueList[FieldName];
}

/*
 *  功能：
 *      获取策略表字段名
 *  参数：
 *      field           :   字段
 *  返回：
 *      字段名
 */
string DbDataManager::GetStrategyFieldName(DbDataManager::StrategyTableField field)
{
    string FieldName = "";
    switch (field)
    {
        case DbDataManager::STRATEGY_TABLE_FIELD_STRATEGYTYPE: // 策略类型
        {
            FieldName = string("STRATEGYTYPE");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_CAPACITY: // 策略卷大小
        {
            FieldName = string("CAPACITY");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_SSCOPYID: // 应急副本id
        {
            FieldName = string("SSCOPYID");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_TRUSTEDCOPYID: // 可信副本id
        {
            FieldName = string("TRUSTEDCOPYID");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_MIRRORLUNID: // MIRROR盘的LUNID
        {
            FieldName = string("MIRRORLUNID");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_MMLUNID: // MIRROR的MIRROR盘的LUNID
        {
            FieldName = string("MMLUNID");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_VOLUMEPAIRID: // 卷对id
        {
            FieldName = string("VOLUMEPAIRID");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_CURRENTVOLUMETYPE: // 当前卷类型
        {
            FieldName = string("CURRENTVOLUMETYPE");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_USEDFORCDP: // 是否用于CDP
        {
            FieldName = string("USEDFORCDP");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_CDPSTATE: // CDP状态
        {
            FieldName = string("CDPSTATE");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_MAXSNAPSHOT: // 最大快照数
        {
            FieldName = string("MAXSNAPSHOT");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_USEDFORTIMEOUT: // 是否设置复制超时
        {
            FieldName = string("USEDFORTIMEOUT");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_COPYTIMEOUT: // 复制超时时间
        {
            FieldName = string("COPYTIMEOUT");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_CDPMODE: // CDP模式
        {
            FieldName = string("CDPMODE");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_DEVICEMINOR: // 设备号
        {
            FieldName = string("DEVICEMINOR");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_STGID:// 存储空间编号
        {
            FieldName = string("STGID");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_STG_DEVICE:// 存储空间设备
        {
            FieldName = string("DEVICE");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_STG_MOUNTPOINT:// 存储空间挂载点
        {
            FieldName = string("MOUNTPOINT");
            break;
        }
        case DbDataManager::STRATEGY_TABLE_FIELD_STG_FSTYPE: // 存储空间文件系统
        {
            FieldName = string("FSTYPE");

            break;
        }
    }
    return FieldName;
}

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
string DbDataManager::GetStrategyFieldValue(string hcode, string strategyid, DbDataManager::StrategyTableField field)
{
    string FieldName = GetStrategyFieldName(field);

    // 检查输入的有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(hcode) == true
            || BeGeneralLib::IsStringEmptyOrZero(strategyid) == true
            || BeGeneralLib::IsStringEmptyOrZero(FieldName) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    // 获取策略字段值
    map<string, string> FieldValueList = GetStrategyFieldValue(hcode, strategyid);

    return FieldValueList[FieldName];
}

/*
 *  功能：
 *      获取策略表字段值
 *  参数：
 *      hcode           :   主机编码
 *      strategyid      :   策略编号
 *  返回：
 *      字段值
 */
map<string, string> DbDataManager::GetStrategyFieldValue(string hcode, string strategyid)
{
    string LMountPoint = GetHostMountPoint(hcode, strategyid);
    string SqlString = string("SELECT T.STRATEGYID, T.HCODE, T.LMOUNTPOINT, T.STRATEGYNAME, T.STRATEGYTYPE, T.ISFULLCOPY, T.CAPACITY, T.SSCOPYID, T.TRUSTEDCOPYID, T.MIRRORLUNID, T.MMLUNID, T.VOLUMEPAIRID,T.CURRENTVOLUMETYPE,T.USEDFORCDP,T.CDPSTATE, T.MAXSNAPSHOT, T.USEDFORTIMEOUT, T.COPYTIMEOUT, T.CDPMODE, T.DEVICEMINOR, T.STGID, S.DEVICE, S.MOUNTPOINT, S.FSTYPE FROM TBLCOPYSTRATEGY T, TBLSTORAGESPACE S WHERE T.STGID = S.STGID AND HCODE = BINARY '") + hcode + string("' AND LMOUNTPOINT = BINARY '") + LMountPoint + string("'");
    vector<string> FieldList;
    FieldList.push_back("STRATEGYID");
    FieldList.push_back("HCODE");
    FieldList.push_back("LMOUNTPOINT");
    FieldList.push_back("STRATEGYNAME");
    FieldList.push_back("STRATEGYTYPE");
    FieldList.push_back("CAPACITY");
    FieldList.push_back("SSCOPYID");
    FieldList.push_back("TRUSTEDCOPYID");
    FieldList.push_back("MIRRORLUNID");
    FieldList.push_back("MMLUNID");
    FieldList.push_back("VOLUMEPAIRID");
    FieldList.push_back("CURRENTVOLUMETYPE");
    FieldList.push_back("USEDFORCDP");
    FieldList.push_back("CDPSTATE");
    FieldList.push_back("MAXSNAPSHOT");
    FieldList.push_back("USEDFORTIMEOUT");
    FieldList.push_back("COPYTIMEOUT");
    FieldList.push_back("CDPMODE");
    FieldList.push_back("DEVICEMINOR");
    FieldList.push_back("STGID");
    FieldList.push_back("DEVICE");
    FieldList.push_back("MOUNTPOINT");
    FieldList.push_back("FSTYPE");

    return GetFieldValue(SqlString, FieldList);
}

/*
 *  功能：
 *      根据字段列表获取连接后的字符串
 *  参数：
 *      fieldlist       :   字段列表
 *  返回：
 *      字段名字符串
 */
string DbDataManager::GetFieldNameString(vector<string> fieldlist)
{

    return GetFieldNameString(fieldlist, SEPARATOR_CHAR_COMMA);
}

/*
 *  功能：
 *      根据字段列表获取连接后的字符串
 *  参数：
 *      fieldlist       :   字段列表
 *      seperator       :   分隔符
 *  返回：
 *      字段名字符串
 */
string DbDataManager::GetFieldNameString(vector<string> fieldlist, char seperator)
{

    return GetFieldNameString(fieldlist, string(1, seperator));
}

/*
 *  功能：
 *      根据字段列表获取连接后的字符串
 *  参数：
 *      fieldlist       :   字段列表
 *      seperator       :   分隔符
 *  返回：
 *      字段名字符串
 */
string DbDataManager::GetFieldNameString(vector<string> fieldlist, string seperator)
{
    string FieldNameString = "";

    // 检查列表是否为空
    if (fieldlist.size() == 0)
    {
        return FieldNameString;
    }

    for (int i = 0; i < fieldlist.size(); i++)
    {
        FieldNameString = FieldNameString + fieldlist[i] + seperator;
    }


    if (BeGeneralLib::IsStringEmpty(seperator) == false)
    {
        FieldNameString = BeGeneralLib::StringTrim(FieldNameString, seperator.c_str()[0]);
    }
    if (BeGeneralLib::IsStringEmpty(FieldNameString) == true)
    {

        FieldNameString = SEPARATOR_CHAR_SPACE + FieldNameString + SEPARATOR_CHAR_SPACE;
    }
    return FieldNameString;
}

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
void DbDataManager::CheckDatabaseBackupPath(string path, string filename, int backupnum)
{
    // 检查路径是否以路径分割符结尾，如果不是，则添加后缀路径分割符
    path = BeGeneralLib::AddPathSeparator(path);

    // 检查目录是否存在
    BeGeneralLib::CheckAndCreateDir(path);

    string SourceFile = path + filename;
    if (BeGeneralLib::IsFileExist(SourceFile) == false)
    {
        return;
    }

    // 获取当前目录下备份的文件数
    string ShellCommand = string("find") + SEPARATOR_CHAR_SPACE + path + SEPARATOR_CHAR_SPACE + string("-maxdepth 1 | grep") + SEPARATOR_CHAR_SPACE + filename + SEPARATOR_CHAR_UNDERLINE + SEPARATOR_CHAR_SPACE + string("| wc -l");
    int TotalLogNum = BeGeneralLib::StringToInt(BeGeneralLib::ReadShellReturnValue(ShellCommand));
    if (TotalLogNum > backupnum)
    {
        // 删除多余的备份文件

        int Difference = TotalLogNum - backupnum;
        ShellCommand = string("find") + SEPARATOR_CHAR_SPACE + path + SEPARATOR_CHAR_SPACE + string("-maxdepth 1 | grep") + SEPARATOR_CHAR_SPACE + filename + SEPARATOR_CHAR_UNDERLINE + SEPARATOR_CHAR_SPACE + string("| sort | head -") + BeGeneralLib::Number2String(Difference)+(" | xargs rm -f");
        BeGeneralLib::ExecuteSystem(ShellCommand, false, false);
    }

    string DestFile = SourceFile + SEPARATOR_CHAR_UNDERLINE + BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_FULL_DATETIME);
    BeGeneralLib::MoveFile(SourceFile, DestFile, true, false);
}

/*
 *  功能：
 *      根据主机编码和表类型获取表名
 *  参数
 *      hcode               :   主机编码
 *      type                :   表类型
 *  返回：
 *      表名
 */
string DbDataManager::GetTableName(string hcode, DbDataManager::TableType type)
{
    HostOSType OsType = GetHostOSType(hcode);

    string TableName = "";
    switch (type)
    {
        case DbDataManager::TABLE_TYPE_VOLUME:
        {
            switch (OsType)
            {
                case DbDataManager::HOST_OS_TYPE_LINUX:
                {
                    TableName = string("TBLVOLUME_L");
                    break;
                }
                default:
                {
                    TableName = string("TBLVOLUME");
                    break;
                }
            }
            break;
        }
        case DbDataManager::TABLE_TYPE_SNAPSHOT:
        {
            switch (OsType)
            {
                case DbDataManager::HOST_OS_TYPE_WINDOWS:
                {
                    TableName = string("TBLSNAPSHOT");
                    break;
                }
                case DbDataManager::HOST_OS_TYPE_LINUX:
                {
                    TableName = string("TBLSNAPSHOT_L");
                    break;
                }
                case DbDataManager::HOST_OS_TYPE_AIX:
                case DbDataManager::HOST_OS_TYPE_HPUX:
                {
                    TableName = string("TBLSNAPSHOT_A");
                    break;
                }
                default:
                {
                    break;
                }
            }
            break;
        }
        case DbDataManager::TABLE_TYPE_SNAPSHOTCOPY:
        {
            switch (OsType)
            {
                case DbDataManager::HOST_OS_TYPE_WINDOWS:
                {
                    TableName = string("TBLSNAPSHOTCOPY");
                    break;
                }
                case DbDataManager::HOST_OS_TYPE_LINUX:
                {
                    TableName = string("TBLSNAPSHOTCOPY_L");
                    break;
                }
                case DbDataManager::HOST_OS_TYPE_AIX:
                case DbDataManager::HOST_OS_TYPE_HPUX:
                {
                    TableName = string("TBLSNAPSHOTCOPY_A");
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
    return TableName;
}

// 获取报警条件
map<string, string> DbDataManager::GetAlarmRule(string hcode)
{
    vector<string> FieldList;
    FieldList.push_back("BASEMONITOR");		//按数据库中的字段顺序排列
    FieldList.push_back("BASEFAILEDRELAY");
    FieldList.push_back("CPUUPLIMIT");
    FieldList.push_back("MEMUPLIMIT");
    FieldList.push_back("DISKUPLIMIT");
    FieldList.push_back("CPURELAYLIMIT");
    FieldList.push_back("CPURELAYTIMESPAN");
    FieldList.push_back("MEMRELAYLIMIT");
    FieldList.push_back("MEMRELAYTIMESPAN");
    FieldList.push_back("DISKRELAYLIMIT");
    FieldList.push_back("DISKRELAYTIMESPAN");
    FieldList.push_back("PINGMONITOR");
    FieldList.push_back("PINGFAILEDRELAYFLAG");
    FieldList.push_back("PINGDELAYTIME");
    FieldList.push_back("HBMONITOR");
	FieldList.push_back("HEARTBEATTIMEOUT");
	FieldList.push_back("HBFAILEDRELAY");

    string FieldNameString = GetFieldNameString(FieldList);
    string SqlString = string("SELECT ") + FieldNameString + string(" FROM TBLALARMRULE WHERE HCODE = '") + hcode + "'";
    return GetFieldValue(SqlString, FieldList);
}

sql::ResultSet * DbDataManager::GetMonitorPort(string hcode)
{
    string SqlString = string("SELECT PORTCODE,FAILEDRELAYFLAG,FAILEDRELAYTIME FROM TBLHOSTPORT WHERE PORTMONITOR=1 AND PORTCODE>0 AND HCODE='" + hcode + "'");
    return GetResultSet(SqlString);
}

sql::ResultSet * DbDataManager::GetMonitorService(string hcode)
{
    string SqlString = string("SELECT SERVICENAME,FAILEDRELAYFLAG,FAILEDRELAYTIME FROM TBLHOSTSERVICE WHERE MONITOR=1 AND HCODE='" + hcode + "'");
    return GetResultSet(SqlString);
}

sql::ResultSet * DbDataManager::GetMonitorProcess(string hcode)
{
    string SqlString = string("SELECT PROCESSNAME,FAILEDRELAYFLAG,FAILEDRELAYTIME FROM TBLHOSTPROCESS WHERE MONITOR=1 AND HCODE='" + hcode + "'");
    return GetResultSet(SqlString);
}

map<string, string> DbDataManager::GetSoftOffParam(string hcode){
    string SqlString = string("SELECT AUTORELAYMODE,SOFTOFFTIMEOUT FROM TBLHOST WHERE HCODE='") + hcode + string("'");
    vector<string> FieldList;
    FieldList.push_back("AUTORELAYMODE");
    FieldList.push_back("SOFTOFFTIMEOUT");
    return GetFieldValue(SqlString, FieldList);
}

bool DbDataManager::AddSelfCheckLog(string content, BesError::SelfCheckResult state){
	string State="";
	switch(state){
	case BesError::SELF_CHECK_RESULT_NONE:			State="0";break;
	case BesError::SELF_CHECK_RESULT_SUCCESS:		State="1";break;
	case BesError::SELF_CHECK_RESULT_FAILED:		State="2";break;
	case BesError::SELF_CHECK_RESULT_NOT_RUNNING:	State="3";break;
	case BesError::SELF_CHECK_RESULT_FIXED:			State="4";break;
	case BesError::SELF_CHECK_RESULT_NOT_EXIST:		State="5";break;
	default: State="0";break;
	}
	return ExecuteSql("INSERT INTO TBLSELFCHECK (CREATETIME, CONTENT, STATE) VALUES (now(), '" + content + "', '" + State + "')");
}

bool DbDataManager::AddDockerImage(string hcode, string hostip, string imagename, string imageid, string copyid, string targetfile, string mountpoint)
{
    string SqlString = string("INSERT INTO TBLDOCKERIMG(HCODE,HOSTIP,DOCKERIMGNAME,DOCKERIMGID,COPYID,TARGETFILE,MOUNTPOINT,CREATETIME,UPDATETIME) VALUES ('")+hcode
    						+string("','")+hostip+string("','")+imagename+string("','")+imageid+string("',")+copyid+string(",'")+targetfile+string("','")+mountpoint+string("',now(),now())");
    return ExecuteSql(SqlString);
}

bool DbDataManager::DeleteDockerImage(string imageid)
{
    string SqlString = string("DELETE FROM TBLDOCKERIMG WHERE DOCKERIMGID='")+ imageid + string("'");
    return ExecuteSql(SqlString);
}

bool DbDataManager::AddDockerContainer(string hcode, string containerid, string containername, string hostip, string imageid, string state)
{
    string SqlString = string("INSERT INTO TBLDOCKERCONTAINER(HCODE,CONTAINERID,CONTAINERNAME,HOSTIP,DOCKERIMGID,CONTAINERSTATE,CREATETIME,UPDATETIME) VALUES ('")+hcode
    						+string("','")+containerid+string("','")+containername+string("','")+hostip+string("','")+imageid+string("','")+state+string("',now(),now())");
    return ExecuteSql(SqlString);
}

bool DbDataManager::DeleteDockerContainer(string containerid){
	return ExecuteSql("DELETE FROM TBLDOCKERCONTAINER WHERE CONTAINERID='" + containerid + "'");
}

map<string, string> DbDataManager::GetDockerImageValue(string hcode, string copyid){
    string SqlString = string("SELECT DOCKERIMGID,COPYID,TARGETFILE,MOUNTPOINT FROM TBLDOCKERIMG WHERE HCODE='") + hcode + string("'");
    if(copyid.empty() == false) SqlString += string(" AND COPYID=") + copyid;
    vector<string> FieldList;
    FieldList.push_back("DOCKERIMGID");
    FieldList.push_back("COPYID");
    FieldList.push_back("TARGETFILE");
    FieldList.push_back("MOUNTPOINT");
    return GetFieldValue(SqlString, FieldList);
}

bool DbDataManager::IsDockerImageExist(string imageid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLDOCKERIMG WHERE DOCKERIMGID = '") + imageid + string("'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

bool DbDataManager::IsDockerImageHasContainer(string imageid){
	string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLDOCKERCONTAINER WHERE DOCKERIMGID = '") + imageid + string("'");
	return GetFieldValueBool(SqlString, "RECCOUNT");
}

bool DbDataManager::IsDockerContainerExist(string containerid)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLDOCKERCONTAINER WHERE CONTAINERID = '") + containerid + string("'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

string DbDataManager::GetVmTaskoverMethod(string hcode)
{
    string SqlString = string("SELECT VMEAMANNER FROM TBLHOST WHERE HCODE = '") + hcode + string("'");
    return GetFieldValue(SqlString, "VMEAMANNER");
}

string DbDataManager::GetMirrorServerIP(string hcode)
{
    string SqlString = string("SELECT MIRRORSERVERIP FROM TBLHOST WHERE HCODE = '") + hcode + string("'");
    return GetFieldValue(SqlString, "MIRRORSERVERIP");
}

bool DbDataManager::UpdateDbImageProtect(string hcode, string mirrorserverip, string mirrordbip, string mirrormode, DbDataManager::DBMirrorState mirrorstate)
{
    string SqlString = string("UPDATE TBLHOST SET MIRRORSERVERIP='")+mirrorserverip+string("',MIRRORDBIP='")+mirrordbip
    						+string("',MIRRORMODE=")+mirrormode+string(",MIRRORSTATE=")+BeGeneralLib::Number2String(mirrorstate)
    						+string(" WHERE HCODE='")+hcode+string("'");
    return ExecuteSql(SqlString);
}

bool DbDataManager::UpdateDbMirrorState(string hcode, DbDataManager::DBMirrorState mirrorstate)
{
    string SqlString = string("UPDATE TBLHOST SET MIRRORSTATE=")+BeGeneralLib::Number2String(mirrorstate)+string(" WHERE HCODE='")+hcode+string("'");
    return ExecuteSql(SqlString);
}

map<string, string> DbDataManager::GetMirrorInfo(string hcode){
    string SqlString = string("SELECT DBPROTECTED,MIRRORSERVERIP,MIRRORDBIP,MIRRORMODE,MIRRORSTATE FROM TBLHOST WHERE HCODE='") + hcode + string("'");
    vector<string> FieldList;
    FieldList.push_back("DBPROTECTED");
    FieldList.push_back("MIRRORSERVERIP");
    FieldList.push_back("MIRRORDBIP");
    FieldList.push_back("MIRRORMODE");
    FieldList.push_back("MIRRORSTATE");
    return GetFieldValue(SqlString, FieldList);
}

//硬件应急面板类型(按键、触摸屏)  如果表中无PANELTYPE字段,返回的是0
int DbDataManager::GetPanelType(){
	string SqlString = string("SELECT PANELTYPE FROM TBLHWECONFIG");
	return GetFieldValueInt(SqlString, "PANELTYPE");
}

//删除主机的rac磁盘
bool DbDataManager::DeleteHostRacDisk(string hcode)
{
    string SqlString = string("DELETE FROM TBLDISK WHERE HCODE = '") + hcode + string("'");
    return ExecuteSql(SqlString);
}

//清除rac组首选主机
bool DbDataManager::ClearRacGroupMasterHost(string groupid)
{
    return UpdateRacGroupMasterHost(groupid, "?");
}

//更新rac组首选主机
bool DbDataManager::UpdateRacGroupMasterHost(string groupid, string hcode)
{
    string SqlString = string("UPDATE TBLRACGROUP SET MASTERHCODE ='") + hcode + string("' WHERE RACGROUPID = ") + groupid;
    return ExecuteSql(SqlString);
}

//获取rac组首选主机的主机编码
string DbDataManager::GetRacGroupMasterHost(string groupid)
{
    string SqlString = string("SELECT MASTERHCODE FROM TBLRACGROUP WHERE RACGROUPID = ") + groupid;
    return GetFieldValue(SqlString, "MASTERHCODE");
}

bool DbDataManager::IsRacGroupExist(string groupid)
{
   string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLRACGROUP WHERE RACGROUPID = ") + groupid;
   return GetFieldValueBool(SqlString, "RECCOUNT");
}

//检查rac的磁盘是否存在
bool DbDataManager::IsRacDiskExist(string hcode, string disksysmbol)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBLDISK WHERE HCODE = '") + hcode + string("' AND DISKSYMBOL ='") + disksysmbol + string("'");
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

bool DbDataManager::ChangeHostIpAddr(string hcode, string newip)
{
	string SqlString = string("SELECT IPADDR FROM TBLHOST WHERE HCODE = '") + hcode + string("'");
	string oldip = GetFieldValue(SqlString, "IPADDR");
    if (BeGeneralLib::IsStringEmpty(oldip)) return false;
	SqlString = string("update TBLHOST set IPADDR='") + newip + string("' where HCODE='") + hcode + string("' and IPADDR='") + oldip + string("'");
	bool flag = ExecuteSql(SqlString);
	if(!flag) return false;
	SqlString = string("update TBLHOSTIPADDRESS set IPADDRESS='") + newip + string("' where HCODE='") + hcode + string("' and IPADDRESS='") + oldip + string("' and WORKMODE=1");
	flag= ExecuteSql(SqlString);
	return flag;
}

string DbDataManager::GetHostCreateTime(string hcode)
{
    string SqlString = string("SELECT CREATETIME FROM TBLHOST WHERE HCODE = '") + hcode + string("'");
    return GetFieldValue(SqlString, "CREATETIME");
}

string DbDataManager::GetServerIPAddress(string hcode)
{
	string SqlString = string("select a.IPADDR IPADDR from TBLBESNIC a,TBLHOST b where a.NICNAME=b.BUSINESSNICNAME and b.HCODE='") + hcode + string("'");
    return GetFieldValue(SqlString, "IPADDR");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool DbDataManager::DeleteExpiredTask()
{
    string SqlString = string("DELETE FROM TBL_TASK WHERE STATUS = 3 AND DATEDIFF(NOW(), CREATETIME) > 30");
    return ExecuteSql(SqlString);
}

//准备执行、正在执行-->完成+失败
bool DbDataManager::UpdateUncompletedTask()
{
    string SqlString = string("UPDATE TBL_TASK SET STATUS = 3, RESULT = 0, UPDATETIME = NOW() WHERE STATUS = 1 or STATUS = 2");
    return ExecuteSql(SqlString);
}

//获取任务结果集
sql::ResultSet * DbDataManager::GetTaskResultSet()
{
    string SqlString = string("SELECT TASKID, COMMANDHEADER, COMMANDCODE, PARAMETER, STATUS, MAXCOUNTER FROM TBL_TASK WHERE STATUS = 0 ORDER BY TASKID");
    return GetResultSet(SqlString);
}

bool DbDataManager::CreateSourceTable(string source)
{
	vector<string> SqlList;
	string SqlString = string("CREATE TABLE IF NOT EXISTS TBL_TRUNKDATA_") + source + string("(PID int(11) NOT NULL AUTO_INCREMENT, SCN int(11) NOT NULL, CSCN int(11) NOT NULL, OPERATION_CODE int(11) NOT NULL, ")+
		               string("SEG_OWNER varchar(32) CHARACTER SET utf8 NOT NULL, USERNAME varchar(30) CHARACTER SET utf8 NOT NULL, XIDUSN int(11) NOT NULL, XIDSLT int(11) NOT NULL, XIDSQN int(11) NOT NULL, ")+
	                   string("RS_ID varchar(32) CHARACTER SET utf8 NOT NULL, SSN int(11) NOT NULL, ROLLBACK int(11) NOT NULL, CSF int(11) NOT NULL, SQL_REDO varchar(4000) CHARACTER SET utf8 NOT NULL, PRIMARY KEY(PID)) ENGINE=InnoDB AUTO_INCREMENT=1");
	SqlList.push_back(SqlString);

	SqlString = string("UPDATE TBL_SOURCE_DB SET TRUNK_TABLE='TBL_TRUNKDATA_") + source + string("' WHERE SID=") + source + string(" AND TRUNK_TABLE='?'");
	SqlList.push_back(SqlString);

	return ExecuteSql(SqlList);
}

bool DbDataManager::CreateStrategyTable(string strategy)
{
	vector<string> SqlList;
	string SqlString = string("CREATE TABLE IF NOT EXISTS TBL_DONETRANS_") + strategy + string("(DID int(11) NOT NULL AUTO_INCREMENT, SCN int(11) NOT NULL DEFAULT '0', ")+
					   string("XIDUSN int(11) NOT NULL DEFAULT '0', XIDSLT int(11) NOT NULL DEFAULT '0', XIDSQN int(11) NOT NULL DEFAULT '0', PRIMARY KEY(DID)) ENGINE=InnoDB AUTO_INCREMENT=1");
	SqlList.push_back(SqlString);

	SqlString = string("CREATE TABLE IF NOT EXISTS TBL_ERRORSQL_") + strategy + string("(SCN int(11) DEFAULT NULL, XIDUSN int(11) DEFAULT NULL, XIDSLT int(11) DEFAULT NULL, XIDSQN int(11) DEFAULT NULL, RSID varchar(40) DEFAULT NULL, ")+
	            string("ERR_TYPE varchar(200) CHARACTER SET utf8 NOT NULL, REDO_SQL varchar(4000) CHARACTER SET utf8 NOT NULL) ENGINE=InnoDB");
	SqlList.push_back(SqlString);

	SqlString = string("UPDATE TBL_STRATEGY SET DONE_TABLE='TBL_DONETRANS_") + strategy + string("' WHERE STRATEGY_ID=") + strategy + string(" AND DONE_TABLE='?'");
	SqlList.push_back(SqlString);

	SqlString = string("UPDATE TBL_STRATEGY SET ERROR_TABLE='TBL_ERRORSQL_") + strategy + string("' WHERE STRATEGY_ID=") + strategy + string(" AND ERROR_TABLE='?'");
	SqlList.push_back(SqlString);

	return ExecuteSql(SqlList);
}

bool DbDataManager::UpdateExtractorProcess(string source, string pid)
{
	string SqlString = string("UPDATE TBL_SOURCE_DB SET PROCESS=") + pid + string(" WHERE SID=") + source;
	return ExecuteSql(SqlString);
}

bool DbDataManager::UpdateLoaderProcess(string strategy, string pid)
{
	string SqlString = string("UPDATE TBL_STRATEGY SET PROCESS=") + pid + string(" WHERE STRATEGY_ID=") + strategy;
	return ExecuteSql(SqlString);
}

string DbDataManager::GetExtractorProcess(string source)
{
	string SqlString = string("SELECT PROCESS FROM TBL_SOURCE_DB WHERE SID=") + source;
    return GetFieldValue(SqlString, "PROCESS");
}

string DbDataManager::GetLoaderProcess(string strategy)
{
	string SqlString = string("SELECT PROCESS FROM TBL_STRATEGY WHERE STRATEGY_ID=") + strategy;
    return GetFieldValue(SqlString, "PROCESS");
}

sql::ResultSet * DbDataManager::GetSourceDBResultSet()
{
    string SqlString = string("SELECT SID,DBTYPE,TRUNK_TABLE FROM TBL_SOURCE_DB ORDER BY SID");
    return GetResultSet(SqlString);
}

//自动启动的源库ID
sql::ResultSet * DbDataManager::GetAutostartSourceDBResultSet()
{
//  string SqlString = string("SELECT SID FROM TBL_SOURCE_DB ORDER BY SID");
	string SqlString = string("SELECT DISTINCT SID from TBL_STRATEGY WHERE AUTOSTART=1");
    return GetResultSet(SqlString);
}

sql::ResultSet * DbDataManager::GetStrategyResultSet()
{
    string SqlString = string("SELECT STRATEGY_ID,CREATETIME FROM TBL_STRATEGY ORDER BY STRATEGY_ID");
    return GetResultSet(SqlString);
}

sql::ResultSet * DbDataManager::GetAutostartStrategyResultSet()
{
    string SqlString = string("SELECT STRATEGY_ID FROM TBL_STRATEGY WHERE AUTOSTART=1 ORDER BY STRATEGY_ID");
    return GetResultSet(SqlString);
}

bool DbDataManager::IsSourceDBExist(string source)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBL_SOURCE_DB WHERE SID=") + source;
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

bool DbDataManager::IsStrategyExist(string strategy)
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBL_STRATEGY WHERE STRATEGY_ID=") + strategy;
    return GetFieldValueBool(SqlString, "RECCOUNT");
}

int DbDataManager::GetStrategyCount()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBL_STRATEGY");
    return GetFieldValueInt(SqlString, "RECCOUNT");
}

int DbDataManager::GetSourceDBCount()
{
    string SqlString = string("SELECT COUNT(*) AS RECCOUNT FROM TBL_SOURCE_DB");
    return GetFieldValueInt(SqlString, "RECCOUNT");
}

bool DbDataManager::DeleteExpiredTrunkData(string source)
{
	string SqlString = string("DELETE FROM TBL_TRUNKDATA_") + source + string(" WHERE CSCN < (SELECT MIN(SCN) FROM TBL_STRATEGY WHERE SID=") + source + string(")");
	return ExecuteSql(SqlString);
}

bool DbDataManager::UpdateExtractorStatus(string source, string status)
{
	string SqlString = string("UPDATE TBL_STRATEGY SET CAP_STATE=") + status + string(" WHERE SID=") + source + string(" AND CAP_STATE!=") + status;
	return ExecuteSql(SqlString);
}

bool DbDataManager::UpdateLoaderStatus(string strategy, string status)
{
	string SqlString = string("UPDATE TBL_STRATEGY SET LOAD_STATE=") + status + string(" WHERE STRATEGY_ID=") + strategy + string(" AND LOAD_STATE!=") + status;
	return ExecuteSql(SqlString);
}

bool DbDataManager::AddTask(string cmdcode, string parameter)
{
    string SqlString = string("INSERT INTO TBL_TASK(COMMANDCODE, PARAMETER, CREATETIME) VALUES('") + cmdcode + "','" + parameter + "',NOW())";
    return ExecuteSql(SqlString);
}

bool DbDataManager::DeleteStrategy(string strategy)
{
	string SqlString = string("DELETE FROM TBL_STRATEGY WHERE STRATEGY_ID=") + strategy;
	return ExecuteSql(SqlString);
}

bool DbDataManager::AddStrategy(string strategyname,string sid,string did,string scn,string syncusers,string interval,string errorskip,string autostart)
{
	string SqlString = string("INSERT INTO TBL_STRATEGY(`STRATEGY_NAME`,`SID`,`DID`,`SCN`,`SYNC_USERS`,`INTERVAL`,`ERRORSKIP`,`AUTOSTART`,`CREATETIME`) VALUES('")
						+ strategyname + string("',") + sid + string(",") + did + string(",") + scn + string(",'") + syncusers + string("',") + interval + string(",") + errorskip + string(",") + autostart + string(",now())");
	return ExecuteSql(SqlString);
}

bool DbDataManager::DeleteSourceDB(string source)
{
	string SqlString = string("DELETE FROM TBL_SOURCE_DB WHERE SID=") + source;
	return ExecuteSql(SqlString);
}

bool DbDataManager::AddSourceDB(string dbtype,string sip,string instancename,string port,string username,string password,string tsname,string trunkscn,string branchscn,string interval)
{
	string SqlString = string("INSERT INTO TBL_SOURCE_DB(`DBTYPE`,`SIP`,`INSTANCENAME`,`PORT`,`USERNAME`,`PASSWORD`,`TSNAME`,`TRUNK_SCN`,`BRANCH_SCN`,`INTERVAL`) VALUES(")
						+ dbtype + string(",'") + sip + string("','") + instancename + string("',") + port + string(",'") + username + string("','")
						+ password + string("','") + tsname + string("',") + trunkscn + string(",") + branchscn + string(",") + interval + string(")");
	return ExecuteSql(SqlString);
}

string DbDataManager::GetStrategyCreateTime(string strategy)
{
    string SqlString;
	if(strategy != "")
		SqlString = string("SELECT CREATETIME FROM TBL_STRATEGY WHERE STRATEGY_ID=") + strategy;
	else
		SqlString = string("SELECT MAX(CREATETIME) AS CREATETIME FROM TBL_STRATEGY");

    return GetFieldValue(SqlString, "CREATETIME");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
