/* 
 * File:   becommand.cpp
 * Author: root
 * 
 * Created on 2012年5月25日, 下午4:15
 */

#include "becommand.h" 

/*
 *  功能：
 *      默认构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
BeCommand::BeCommand()
{
    InitCommandCollection();
}

/*
 *  功能：
 *      默认析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
BeCommand::~BeCommand()
{
    // 删除元素
    DestroyCommandCollection();
}

/*
 *  功能：
 *      创建标准格式命令
 *  参数：
 *      type            :   类型
 *      code            :   命令字
 *      parameter       :   参数
 *  返回：
 *      命令内容
 */
string BeCommand::CreateStandardCommand(string type, string code, string parameter)
{
    return CreateStandardCommand(BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_COMMAND_HEADER), type, code, parameter);
}

/*
 *  功能：
 *      创建标准格式命令
 *  参数：
 *      header          :   命令头
 *      type            :   类型
 *      code            :   命令字
 *      parameter       :   参数
 *  返回：
 *      命令内容
 */
string BeCommand::CreateStandardCommand(string header, string type, string code, string parameter)
{
    string StandardStr = header;
    StandardStr += COMMAND_SEGMENT_SEPCHAR;
    StandardStr += type;
    StandardStr += COMMAND_SEGMENT_SEPCHAR;
    StandardStr += code;
    StandardStr += COMMAND_SEGMENT_SEPCHAR;
    StandardStr += parameter;
    StandardStr += COMMAND_SEGMENT_SEPCHAR;
    StandardStr += COMMAND_TAILOR;
    return StandardStr;
}

/*
 *  功能：
 *      创建命令参数
 *  参数：
 *      num             :   参数个数
 *      ...             :   可变参数
 *  返回：
 *      命令参数
 */
string BeCommand::CreateCommandParameter(int num, ...)
{
    string ParameterString = "";
    va_list args;
    va_start(args, num);
    for (int i = 0; i < num; i++)
    {
        ParameterString = ParameterString + va_arg(args, char*) +COMMAND_PARAMETER_SEPCHAR;
    }
    va_end(args);
    return ParameterString;
}

/*
 *  功能：
 *      获取命令字所属类别
 *  参数：
 *      cmdcodestring   :   命令字
 *  返回：
 *      命令字所属类别
 */
BeCommand::CommandSet BeCommand::GetCommandSet(string cmdcodestring)
{
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(cmdcodestring) == true)
    {
        return BeCommand::COMMAND_SET_UNKNOWN;
    }

    string FirstCode = cmdcodestring.substr(0, 1);
    if (BeGeneralLib::IsAllDigits(FirstCode) == false)
    {
        return BeCommand::COMMAND_SET_UNKNOWN;
    }

    return (BeCommand::CommandSet)(BeGeneralLib::StringToInt(FirstCode));
}

/*
 *  功能：
 *      获取命令字枚举标识返回命令字字符串
 *  参数：
 *      cmdcode         :   命令字枚举量
 *  返回：
 *      命令字字符串
 */
string BeCommand::GetCommandCodeString(BeCommand::CommandCode cmdcode)
{
    string CommandCodeString = BeGeneralLib::Number2String((int) cmdcode);
    CommandCodeString = string(COMMAND_CODE_SIZE - CommandCodeString.size(), '0') + CommandCodeString;
    return CommandCodeString;
}

/*
 *  功能：
 *      根据命令字字符串返回命令字枚举标识
 *  参数：
 *      cmdcodestring   :   命令字
 *  返回：
 *      命令字枚举标识
 */
BeCommand::CommandCode BeCommand::GetCommandCode(string cmdcodestring)
{
    int CmdCodeValue = BeGeneralLib::StringToInt(cmdcodestring);

    if (CmdCodeValue <= 0)
    {
        return BeCommand::COMMAND_CODE_UNKNOWN;
    }

    return (BeCommand::CommandCode)(CmdCodeValue);
}

/*
 *  功能：
 *      获取指定命令字的参数个数
 *  参数：
 *      cmdcode             :   命令字
 *  返回：
 *      参数参数
 */
int BeCommand::GetParameterCount(string cmdcode)
{
    if (IsCommandCodeValid(cmdcode) == false)
    {
        return -1;
    }

    return MyCommandCollection[cmdcode]->ParameterCount();
}

/*
 *  功能：
 *      检查命令字是否有效
 *  参数：
 *      cmdcode             :   命令字
 *  返回：
 *      如果有效返回true，否则返回false
 */
bool BeCommand::IsCommandCodeValid(string cmdcode)
{
    //return (MyCommandCollection.find(cmdcode) != MyCommandCollection.end());
    return (GetCommandObject(cmdcode) != NULL);
}

/*
 *  功能：
 *      获取命令对象
 *  参数：
 *      cmdcode             :   命令字
 *  返回：
 *      成功返回命令对象，否则返回NULL
 */
CommandUnit * BeCommand::GetCommandObject(string cmdcode)
{
    map<string, CommandUnit*>::iterator iter = MyCommandCollection.find(cmdcode);
    if (iter == MyCommandCollection.end())
    {
        return NULL;
    }
    return (iter->second);
}

/*
 *  功能：
 *      解析结果
 *  参数：
 *      cmdstring       :   命令
 *      cmd             :   命令结构
 *  返回：
 *      解析成功返回true，否则返回false
 */
bool BeCommand::Parse(string cmdstring, CommandStruct *cmd)
{
    // 检查输入有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(cmdstring) == true || cmd == NULL)
    {
//	cout<<"[BeCommand::Parse]cmdstring is empty"<<endl;
        return false;
    }

    // 分解命令
    vector<string> RpSubArray = BeGeneralLib::StringSplit(cmdstring, COMMAND_SEGMENT_SEPCHAR, BeGeneralLib::WITH_EMPTY_ITEM);
    if (RpSubArray.size() != COMMAND_SEGMENT_NUM)
    {
//	cout<<"[BeCommand::Parse]segment num is "<<RpSubArray.size()<<endl;
        return false;
    }

    string CmdHeader = RpSubArray[0];
    if (CmdHeader.size() != COMMAND_HEADER_SIZE)
    {
//	cout<<"[BeCommand::Parse]command header size error"<<endl;
        return false;
    }

    string CmdType = RpSubArray[1];
    if (CmdType != COMMAND_CODE_ID && (CmdType != COMMAND_RESPONSE_ID))
    {
//	cout<<"[BeCommand::Parse]command type error"<<endl;
        return false;
    }

    string CodeOrResult = RpSubArray[2];
    if (CmdType == COMMAND_CODE_ID && BeGeneralLib::IsStringEmptyOrInvalidValue(CodeOrResult) == true)
    {
//	cout<<"[BeCommand::Parse]CodeOrResult error"<<endl;
        return false;
    }

    string ParameterOrContent = RpSubArray[3];
    string Tailor = RpSubArray[4];
    if (RpSubArray[4] != "\r\n")
    {
//	cout<<"[BeCommand::Parse]command tailor error! tailor="<<RpSubArray[4]<<"tailor size="<<RpSubArray[4].size()<<endl;
        return false;
    }

    // 赋值
    cmd->Header = CmdHeader;
    cmd->Tailor = Tailor;
    cmd->Type = ConvertToCommandType(CmdType);
    switch (cmd->Type)
    {
        case BeCommand::COMMAND_TYPE_COMMAND:
        {
            cmd->Code = CodeOrResult;
            break;
        }
        case BeCommand::COMMAND_TYPE_RESPONSE:
        {
            cmd->Result = ConvertToCommandResult(CodeOrResult);
            break;
        }
        default:
        {
            break;
        }
    }
    cmd->ParaOrContent = ParameterOrContent;
    return true;
}

/*
 *  功能：
 *      初始化命令结构
 *  参数：
 *      cmd             :   命令结构
 *  返回：
 *      无
 */
void BeCommand::InitCommandStruct(CommandStruct *cmd)
{
    if (cmd == NULL)
    {
        return;
    }

    cmd->Header = DEFAULT_EMPTY_STRING;
    cmd->Code = DEFAULT_EMPTY_STRING;
    cmd->ParaOrContent = DEFAULT_EMPTY_STRING;
    cmd->Tailor = DEFAULT_EMPTY_STRING;
    cmd->Type = COMMAND_TYPE_UNKNOWN;
    cmd->Result = COMMAND_RESULT_UNKNOWN;
}

/*
 *  功能：
 *      是否为windows或linux复制命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsWindowsOrLinuxCopyCommand(string cmdcode)
{
    return (IsWindowsOrLinuxInitCopyCommand(cmdcode) == true || IsWindowsOrLinuxSnapshotCopyCommand(cmdcode) == true || cmdcode == DEFAULT_COMMAND_FULL_COPY);
}

/*
 *  功能：
 *      是否为windows或linux初始复制命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsWindowsOrLinuxInitCopyCommand(string cmdcode)
{
    return (cmdcode == DEFAULT_COMMAND_WINDOWS_INITIALIZE_COPY // windows初始复制
            || cmdcode == DEFAULT_COMMAND_LINUX_INITIALIZE_COPY); // Linux初始复制
}

/*
 *  功能：
 *      是否为windows或linux快照复制命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsWindowsOrLinuxSnapshotCopyCommand(string cmdcode)
{
    return (cmdcode == DEFAULT_COMMAND_WINDOWS_SNAPSHOT_COPY // windows快照复制
            || cmdcode == DEFAULT_COMMAND_WINDOWS_DATABASE_COPY // windows数据库复制
            || cmdcode == DEFAULT_COMMAND_LINUX_SNAPSHOT_COPY // linux快照复制
            || cmdcode == DEFAULT_COMMAND_LINUX_DATABASE_COPY // linux数据库复制
            || cmdcode == DEFAULT_COMMAND_SCHEDULE_SNAPSHOT_COPY // 调度快照复制
            || cmdcode == DEFAULT_COMMAND_FILES_SNAPSHOT_COPY); // 文件快照复制（参数：主机编码;策略id;）
}

/*
 *  功能：
 *      是否为windows恢复命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsWindowsRestoreCommand(string cmdcode)
{
    return (cmdcode == DEFAULT_COMMAND_WINDOWS_SYSTEM_RECOVER // windows系统盘恢复
            || cmdcode == DEFAULT_COMMAND_WINDOWS_DATA_RECOVER // windows数据盘恢复
            || cmdcode == DEFAULT_COMMAND_WINDOWS_SPECIFIC_DATA_RECOVER // windows指定盘符恢复
            || cmdcode == DEFAULT_COMMAND_WINDOWS_MULTIPLE_FILES_RECOVER // windows多文件恢复
            || cmdcode == DEFAULT_COMMAND_WINDOWS_SPECIFIC_FILE_RECOVER); // windows指定文件恢复
}

/*
 *  功能：
 *      是否为linux恢复命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsLinuxRestoreCommand(string cmdcode)
{
    return (cmdcode == DEFAULT_COMMAND_LINUX_SYSTEM_RECOVER // linux系统盘恢复
            || cmdcode == DEFAULT_COMMAND_LINUX_DATA_RECOVER // linux数据盘恢复
            || cmdcode == DEFAULT_COMMAND_LINUX_FULL_DISK_RECOVER); // linux全盘恢复
}

/*
 *  功能：
 *      是否为windows或linux恢复命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsWindowsOrLinuxRestoreCommand(string cmdcode)
{
    return (IsWindowsRestoreCommand(cmdcode) == true || IsLinuxRestoreCommand(cmdcode) == true);
}

/*
 *  功能：
 *      是否为unix复制命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsUnixCopyCommand(string cmdcode)
{
    return (cmdcode == DEFAULT_COMMAND_AIX_INITIALIZE_COPY
            || cmdcode == DEFAULT_COMMAND_AIX_SNAPSHOT_COPY
            || cmdcode == DEFAULT_COMMAND_AIX_SCHEDULE_SNAPSHOT_COPY
            || cmdcode == DEFAULT_COMMAND_AIX_DATABASE_COPY);
}

/*
 *  功能：
 *      是否为远程同步命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsRemoteSyncCommand(string cmdcode)
{
    return (cmdcode == DEFAULT_COMMAND_SYNC_HOST_INFORMATION // 同步主机信息
            || cmdcode == DEFAULT_COMMAND_SYNC_HOST_COPY /// 同步主机副本
            || cmdcode == DEFAULT_COMMAND_CREATE_REMOTE_COPY // 创建远程副本
            || cmdcode == DEFAULT_COMMAND_CREATE_REMOTE_SNAPSHOT // 创建远程快照
            || cmdcode == DEFAULT_COMMAND_CHANGE_SNAPSHOT_NAME); // 修改快照名 （参数:主机编码;快照id;）
}

/*
 *  功能：
 *      是否为监控上报命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsMonitorReportCommand(string cmdcode)
{
    return (cmdcode == DEFAULT_COMMAND_QUERY_EXTRACTOR_STATE
            || cmdcode == DEFAULT_COMMAND_QUERY_LOADER_STATE
            );
}

/*
 *  功能：
 *      是否为应急命令
 *  参数：
 *      cmdcode         :   命令字
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeCommand::IsEmergencyCommand(string cmdcode)
{
    return (cmdcode == DEFAULT_COMMAND_START_VM // 启动虚拟机
            || cmdcode == DEFAULT_COMMAND_START_VM_ONE_KEY // 一键虚拟机应急
            || cmdcode == DEFAULT_COMMAND_STOP_VM // 关闭虚拟机
            || cmdcode == DEFAULT_COMMAND_STOP_VM_ONE_KEY // 一键虚拟机关闭
            );
}

//---------------------------------- 私有方法 ----------------------------------//

/*
 *  功能：
 *      将字符串转换为命令类型
 *  参数：
 *      type            :   命令类型
 *  返回：
 *      命令类型
 */
BeCommand::CommandType BeCommand::ConvertToCommandType(string type)
{
    if (type == COMMAND_CODE_ID)
    {
        return BeCommand::COMMAND_TYPE_COMMAND;
    }
    else if (type == COMMAND_RESPONSE_ID)
    {
        return BeCommand::COMMAND_TYPE_RESPONSE;
    }
    return BeCommand::COMMAND_TYPE_UNKNOWN;
}

/*
 *  功能：
 *      将字符串转换为命令结果
 *  参数：
 *      result          :   命令结果
 *  返回：
 *      命令结果
 */
BeCommand::CommandResult BeCommand::ConvertToCommandResult(string result)
{
    CommandResult CmdResult = BeCommand::COMMAND_RESULT_UNKNOWN;
    if (result == DEFAULT_COMMAND_RESULT_FINISH)
    {
        CmdResult = BeCommand::COMMAND_RESULT_FINISH;
    }
    else if (result == DEFAULT_COMMAND_RESULT_ERROR)
    {
        CmdResult = BeCommand::COMMAND_RESULT_ERROR;
    }
    else if (result == DEFAULT_COMMAND_RESULT_PROCESSING)
    {
        CmdResult = BeCommand::COMMAND_RESULT_PROCESSING;
    }
    else if (result == DEFAULT_COMMAND_RESULT_BUSY)
    {
        CmdResult = BeCommand::COMMAND_RESULT_BUSY;
    }
    else if (result == DEFAULT_COMMAND_RESULT_FAIL)
    {
        CmdResult = BeCommand::COMMAND_RESULT_FAIL;
    }
    else if (result == DEFAULT_COMMAND_RESULT_ACK)
    {
        CmdResult = BeCommand::COMMAND_RESULT_ACK;
    }
    return CmdResult;
}

/*
 *  功能：
 *      初始化命令集合
 *  参数：
 *      无
 *  返回：
 *      无
 */
void BeCommand::InitCommandCollection()
{
    //================================== 查询类 ==================================//

    //#define DEFAULT_COMMAND_START_EXTRACTOR                        "001"               // 启动抓取         （参数：源库id;）
    AddCommandUnitToCollection(DEFAULT_COMMAND_START_EXTRACTOR, "源库id;", "启动抓取", "start extractor");

    //#define DEFAULT_COMMAND_STOP_EXTRACTOR                  "002"               // 停止抓取             （参数：源库id;）
    AddCommandUnitToCollection(DEFAULT_COMMAND_STOP_EXTRACTOR, "源库id;", "停止抓取", "stop extractor");

    //#define DEFAULT_COMMAND_START_LOADER                      "003"               // 启动装载             （参数：策略id;）
    AddCommandUnitToCollection(DEFAULT_COMMAND_START_LOADER, "策略id;", "启动装载", "start loader");

    //#define DEFAULT_COMMAND_STOP_LOADER                  "004"               // 停止装载           （参数：策略id;）
    AddCommandUnitToCollection(DEFAULT_COMMAND_STOP_LOADER, "策略id;", "停止装载", "stop loader");

    //#define DEFAULT_COMMAND_QUERY_EXTRACTOR_STATE             "005"               // 上报挖取进程状态        （参数：源库号;状态;）
    AddCommandUnitToCollection(DEFAULT_COMMAND_QUERY_EXTRACTOR_STATE, "源库号;状态;", "上报挖取进程状态", "query extractor process state");

    //#define DEFAULT_COMMAND_QUERY_LOADER_STATE                    "006"               // 上报装载进程状态         （参数：策略号;状态;）
    AddCommandUnitToCollection(DEFAULT_COMMAND_QUERY_LOADER_STATE, "策略号;状态;", "上报装载进程状态", "quert loader process state");

	//#define DEFAULT_COMMAND_ADD_SOURCEDB                            "007"               // 增加源库                 （参数：DBTYPE;SIP;INSTANCENAME;PORT;USERNAME;PASSWORD;TSNAME;TRUNK_SCN;BRANCH_SCN;INTERVAL;）
	AddCommandUnitToCollection(DEFAULT_COMMAND_ADD_SOURCEDB, "数据库类型;源库IP;实例名;端口号;日志分析用户名;日志分析用户密码;日志分析表空间;TRUNK_SCN;BRANCH_SCN;挖取间隔;", "增加源库", "add sourcedb");

	//#define DEFAULT_COMMAND_DEL_SOURCEDB                            "008"               // 删除源库                 （参数：源库id;)
	AddCommandUnitToCollection(DEFAULT_COMMAND_DEL_SOURCEDB, "源库id;", "删除源库", "del sourcedb");

	//#define DEFAULT_COMMAND_ADD_STRATEGY                            "009"               // 增加策略                 （参数：STRATEGY_NAME;SID;DID;SCN;SYNC_USERS;INTERVAL;ERRORSKIP;AUTOSTART;）
	AddCommandUnitToCollection(DEFAULT_COMMAND_ADD_STRATEGY, "策略名;源库id;目标库id;SCN;同步用户列表;装载间隔;跳过错误;是否自启;", "增加策略", "add strategy");

	//#define DEFAULT_COMMAND_DEL_STRATEGY                            "010"               // 删除策略                 （参数：策略id;）
	AddCommandUnitToCollection(DEFAULT_COMMAND_DEL_STRATEGY, "策略id;", "删除策略", "del strategy");

    //================================== 复制类 ==================================//

    //================================== 恢复类 ==================================//

    //================================== 网盘类 ==================================//

    // 执行任务（901）（参数:无）
    //#define DEFAULT_COMMAND_EXECUTE_TASK                            "901"               // 执行任务                 （参数：无）
    AddCommandUnitToCollection(DEFAULT_COMMAND_EXECUTE_TASK, "", "执行任务", "execute task");

    // 设置主机授权（903）（参数:无）
    //#define DEFAULT_COMMAND_SET_LICENSE                             "903"               // 设置主机授权             （参数：无）
    AddCommandUnitToCollection(DEFAULT_COMMAND_SET_LICENSE, "", "设置主机授权", "set host license");

    // 查询授权信息（904）（参数:无）
    //#define DEFAULT_COMMAND_QUERY_LICENSE                           "904"               // 查询授权信息             （参数：无）
    AddCommandUnitToCollection(DEFAULT_COMMAND_QUERY_LICENSE, "", "查询授权信息", "query license");
}

/*
 *  功能：
 *      将命令单元增加到集合
 *  参数：
 *      cmd             :   命令字
 *      paracount       :   参数个数
 *      chnname         :   中文名称
 *      engname         :   英文名称
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeCommand::AddCommandUnitToCollection(string code, string paradesc, string chnname, string engname)
{
    MyCommandCollection.insert(pair<string, CommandUnit*>(code, new CommandUnit(code, paradesc, chnname, engname)));
}

/*
 *  功能：
 *      销毁命令集和
 *  参数：
 *      无
 *  返回：
 *      无
 */
void BeCommand::DestroyCommandCollection()
{
    map<string, CommandUnit*>::iterator iter;
    for (iter = MyCommandCollection.begin(); iter != MyCommandCollection.end();)
    {
        if (iter->second != NULL)
        {
            delete iter->second;
        }
        MyCommandCollection.erase(iter++);
    }
}


