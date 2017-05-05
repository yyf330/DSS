/* 
 * File:   begenerallib.cpp
 * Author: root
 * 
 * Created on 2010年6月28日, 上午9:26
 */

#include <ctype.h>
#include <errno.h>
#include "dog_api.h"
#include "dog_vcode.h"
#include "begenerallib.h" 

/*------------------------------------ 公有方法 --------------------------------*/

BeGeneralLib::BeGeneralLib()
{
}

BeGeneralLib::~BeGeneralLib()
{
}

/*
 *  功能：
 *      数字转换为字符串
 *  参数：
 *      num             :   数字
 *  返回：
 *      转换后的字符串形式
 */
string BeGeneralLib::Number2String(long num)
{
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    bzero(Buf, sizeof (Buf));
    sprintf(Buf, "%ld\0", num);
    return string(Buf);
}

/*
 *  功能：
 *      数字转换为字符串
 *  参数：
 *      num             :   数字
 *      precision       :   精度
 *  返回：
 *      转换后的字符串形式
 */
string BeGeneralLib::Number2String(float num, int precision)
{
    std::ostringstream OStringStream;
    if (precision <= 0)
    {
        OStringStream << num;
    }
    else
    {
        OStringStream << setprecision(precision) << num;
    }
    return OStringStream.str();
}

/*
 *  功能：
 *      字符转换为字符串
 *  参数：
 *      c               :   字符
 *  返回：
 *      转换后的字符串
 */
string BeGeneralLib::Char2String(char c)
{
    string s = DEFAULT_EMPTY_STRING;
    s.push_back(c);
    return s;
}

/*
 *  功能：
 *      字符串转换为整数
 *  参数：
 *      str             :   字符串
 *  返回：
 *      转换后的整数
 */
int BeGeneralLib::StringToInt(string str)
{
    return (atoi(str.c_str()));
}

/*
 *  功能：
 *      字符串转换为浮点数
 *  参数：
 *      str             :   字符串
 *  返回：
 *      转换后的浮点数
 */
float BeGeneralLib::StringToFloat(string str)
{
    return (atof(str.c_str()));
}

/*
 *  功能：
 *      字符串转换为bool
 *  参数：
 *      str             :   字符串
 *  返回：
 *      转换后的bool值
 */
bool BeGeneralLib::StringToBool(string str)
{
    return (bool)(StringToInt(str));
}

/*
 *  功能：
 *      字符串转为大写
 *  参数
 *      str         :   输入字符串
 *  返回：
 *      命令字符串
 */
string BeGeneralLib::StringToUpper(string str)
{
    if (IsStringEmpty(str) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    string ResultStr = DEFAULT_EMPTY_STRING;
    const char *StrPointer = str.c_str();
    for (int i = 0; i < str.size(); i++)
    {
        ResultStr += (char) toupper(StrPointer[i]);
    }
    return ResultStr;
}

/*
 *  功能：
 *      字符串转为小写
 *  参数
 *      str         :   输入字符串
 *  返回：
 *      命令字符串
 */
string BeGeneralLib::StringToLower(string str)
{
    if (IsStringEmpty(str) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    string ResultStr = DEFAULT_EMPTY_STRING;
    const char *StrPointer = str.c_str();
    for (int i = 0; i < str.size(); i++)
    {
        ResultStr += (char) tolower(StrPointer[i]);
    }
    return ResultStr;
}

/*
 *  功能：
 *      按指定的字符对字符串进行分割
 *  参数
 *      str             :   要操作的字符串
 *      ch              :   要分割的字符
 *  返回：
 *      分割后的矢量列表
 */
vector<string> BeGeneralLib::StringSplit(string str, char ch, SplitOptions option)
{
    vector<string> SubArray;
    string::size_type Pos, LastPos;
    Pos = str.find(ch);
    LastPos = 0;
    while (Pos < str.size())
    {
        if (option == BeGeneralLib::REMOVE_EMPTY_ITEM)
        {
            if (Pos - LastPos != 0)
            {
                SubArray.push_back(str.substr(LastPos, Pos - LastPos));
            }
        }
        else if (option == BeGeneralLib::WITH_EMPTY_ITEM)
        {
            SubArray.push_back(str.substr(LastPos, Pos - LastPos));
        }
        LastPos = Pos + 1;
        Pos = str.find(ch, LastPos);
    }
    if (LastPos != str.size())
    {
        SubArray.push_back(str.substr(LastPos));
    }
    return SubArray;
}

/*
 *  功能：
 *      删除字符串两端空格
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      删除后的字符串
 */
string BeGeneralLib::StringTrim(string str)
{
    return StringTrim(str, SEPARATOR_CHAR_SPACE);
}

/*
 *  功能：
 *      删除字符串两端指定字符
 *  参数
 *      str             :   要操作的字符串
 *      ch              :   要删除字符
 *  返回：
 *      删除后的字符串
 */
string BeGeneralLib::StringTrim(string str, char ch)
{
    return StringRTrim(StringLTrim(str, ch), ch);
}

/*
 *  功能：
 *      删除字符串左端指定空格
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      删除后的字符串
 */
string BeGeneralLib::StringLTrim(string str)
{
    return StringLTrim(str, SEPARATOR_CHAR_SPACE);
}

/*
 *  功能：
 *      删除字符串左端指定字符
 *  参数
 *      str             :   要操作的字符串
 *      ch              :   要删除字符
 *  返回：
 *      删除后的字符串
 */
string BeGeneralLib::StringLTrim(string str, char ch)
{
    if (IsStringEmpty(str) == true)
    {
        return str;
    }

    int Index = str.find_first_not_of(ch);
    if (Index == -1)
    {
        return str;
    }
    else
    {
        return str.substr(Index, str.size() - Index);
    }
}

/*
 *  功能：
 *      删除字符串右端指定空格
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      删除后的字符串
 */
string BeGeneralLib::StringRTrim(string str)
{
    return StringRTrim(str, SEPARATOR_CHAR_SPACE);
}

/*
 *  功能：
 *      删除字符串右端指定字符
 *  参数
 *      str             :   要操作的字符串
 *      ch              :   要删除字符
 *  返回：
 *      删除后的字符串
 */
string BeGeneralLib::StringRTrim(string str, char ch)
{
    if (IsStringEmpty(str) == true)
    {
        return str;
    }
    int Index = str.find_last_not_of(ch);
    if (Index == -1)
    {
        return str;
    }
    else
    {
        return str.substr(0, Index + 1);
    }
}

/*
 *  功能：
 *      将str中所有的src替换为dest
 *  参数
 *      str             :   要操作的字符串
 *      src             :   源子串
 *      dest            :   替换为的子串
 *  返回：
 *      替换后的字符串
 */
string BeGeneralLib::StringReplace(string str, string src, string dest)
{
    if (str.length() == 0 || src.length() == 0)
    {
        return str;
    }

    while (true)
    {
        int Position = str.find(src);
        if (Position == -1)
        {
            break;
        }
        str = str.replace(Position, src.length(), dest);
    }
    return str;
}

/*
 *  功能：
 *      将str进行反转
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      反转后的字符串
 */
string BeGeneralLib::StringReverse(string str)
{
    string DestStr = DEFAULT_EMPTY_STRING;
    if (IsStringEmpty(str) == true)
    {
        return DestStr;
    }

    for (int i = 0; i < str.length(); i++)
    {
        DestStr += str.substr(str.length() - 1 - i, 1);
    }
    return DestStr;
}

/*
 *  功能：
 *      检查字段值是否为空或无效值
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BeGeneralLib::IsStringEmptyOrInvalidValue(string str)
{
    return (str.empty() == true || str == DEFAULT_INVALID_RETURN_VALUE || str == DEFAULT_INVALID_RETURN_VALUE2);
}

/*
 *  功能：
 *      检查字段值是否为空或无效值
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BeGeneralLib::IsStringEmptyOrZero(string str)
{
    return (IsStringEmpty(str) == true || str == "0");
}

/*
 *  功能：
 *      检查字段值是否为空或无效值
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BeGeneralLib::IsStringEmpty(string str)
{
    return (str.empty() == true);
}

/*
 *  功能：
 *      替换无效值
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      如果str是无效值，则替换为另一个无效值，否则返回源值
 */
string BeGeneralLib::ReplaceInvalidValue(string str)
{
    return ReplaceInvalidValue(str, DEFAULT_INVALID_RETURN_VALUE2);
}

/*
 *  功能：
 *      替换无效值
 *  参数
 *      str             :   要操作的字符串
 *      newvalue        :   新值
 *  返回：
 *      如果str是无效值，则替换为newvalue，否则返回源值
 */
string BeGeneralLib::ReplaceInvalidValue(string str, string newvalue)
{
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(str) == false)
    {
        return str;
    }
    return newvalue;
}

/*
 *  功能：
 *      替换str中的转义字符
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      转义后的字符串
 */
string BeGeneralLib::EscapeString(string str)
{
    int LastMatchPosition = -1; //记录上次匹配的位置
    int Offset = 0; //目标字符串游标
    int CopyNumber = 0; //本次需要成 copy的字节数

    // 设置目标字符串
    char DestStr[DEFAULT_BUFFER_SIZE + 1];
    bzero(DestStr, sizeof (DestStr));

    // 设置目标指针
    char *DestPtr = DestStr;

    // 设置源指针
    const char * SourcePtr = str.c_str();

    // 设置临时指针
    const char * TempPtr = str.c_str();

    // 开始转义操作
    while (*TempPtr)
    {
        // 检查是否为转义字符
        if (*TempPtr == '\\' || *TempPtr == '\'' || *TempPtr == '"')
        {
            CopyNumber = TempPtr - SourcePtr - LastMatchPosition - 1;
            memcpy(DestPtr + Offset, SourcePtr + LastMatchPosition + 1, CopyNumber);
            Offset += CopyNumber;
            *(DestPtr + Offset) = '\\';
            *(DestPtr + Offset + 1) = *TempPtr;
            Offset += 2;
            LastMatchPosition = TempPtr - SourcePtr;
        }
        TempPtr++;
    }
    if (Offset > 0)//有匹配到，copy最后的字符串
    {
        memcpy(DestPtr + Offset, SourcePtr + LastMatchPosition + 1, TempPtr - SourcePtr - LastMatchPosition);
        return string(DestStr);
    }
    return "";
}

/*
 *  功能：
 *      读取shell返回值
 *  参数
 *      shellcommand    :   shell命令
 *  返回：
 *      shell返回值
 */
string BeGeneralLib::ReadShellReturnValue(string shellcommand)
{
    return ReadShellReturnValue(shellcommand, true);
}

/*
 *  功能：
 *      读取shell返回值
 *  参数
 *      shellcommand    :   shell命令
 *      tolog           :   输出到日志
 *  返回：
 *      shell返回值
 */
string BeGeneralLib::ReadShellReturnValue(string shellcommand, bool tolog)
{
    vector<string> ValueList = ReadShellReturnValueAll(shellcommand, tolog);
    if (ValueList.size() == 0)
    {
        return "";
    }
    return ValueList[0];
}

/*
 *  功能：
 *      读取shell所有返回值
 *  参数
 *      shellcommand    :   shell命令
 *  返回：
 *      shell返回值
 */
vector<string> BeGeneralLib::ReadShellReturnValueAll(string shellcommand)
{
    return ReadShellReturnValueAll(shellcommand, true);
}

/*
 *  功能：
 *      读取shell所有返回值
 *  参数
 *      shellcommand    :   shell命令
 *  返回：
 *      shell返回值
 */
vector<string> BeGeneralLib::ReadShellReturnValueAll(string shellcommand, bool tolog)
{
//	cout<<"[debug]ReadShellReturnValueAll:"<<shellcommand<<endl;
    vector<string> ValueList;
    FILE *fp;
    //    if (tolog == true)
    //    {
    //        BesLog::DebugPrint("ShellCommand:" + shellcommand, BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
    //    }

    if ((fp = popen(shellcommand.c_str(), "r")) == NULL)
    {
        //        if (tolog == true)
        //        {
        //            BesLog::DebugPrint("Open shellcommand failed!\n", BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
        //        }
        return ValueList;
    }
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    while (!feof(fp))
    {
        bzero(Buf, sizeof (Buf));
        fgets(Buf, sizeof (Buf), fp);
        Buf[strlen(Buf) - 1] = DEFAULT_C_STRING_END_FLAG;
        if (IsStringEmpty(string(Buf)) == false)
        {
            ValueList.push_back(string(Buf));
        }
        //        else
        //        {
        //            if (tolog == true)
        //            {
        //                BesLog::DebugPrint("Buffer is empty!\n", BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
        //            }
        //        }
    }
    pclose(fp);

    //    if (tolog == true)
    //    {
    //        BesLog::DebugPrint("ShellValue Size:" + BeGeneralLib::Number2String(ValueList.size()) + string("\n"), BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
    //        for (int i = 0; i < ValueList.size(); i++)
    //        {
    //            BesLog::DebugPrint("ShellValue:" + ValueList[i] + string("\n"), BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
    //        }
    //    }

    return ValueList;
}

/*
 *  功能：
 *      获取执行路径名
 *  参数：
 *      无
 *  返回：
 *      执行路径名
 */
string BeGeneralLib::GetExecutePath()
{
    char LinkPath[DEFAULT_BUFFER_SIZE + 1];
    bzero(LinkPath, sizeof (LinkPath));
    string ExecPath = "";
    string FileName = string("/proc/") + Number2String(getpid()) + string("/exe");
    int Res = readlink(FileName.c_str(), LinkPath, sizeof (LinkPath));
    if (Res != -1)
    {
        LinkPath[Res] = DEFAULT_C_STRING_END_FLAG;
        ExecPath = LinkPath;
        ExecPath = ExecPath.substr(0, ExecPath.find_last_of(LINUX_PATH_SEPCHAR) + 1);
    }
    return ExecPath;
}

/*
 *  功能：
 *      获取目录使用空间
 *  参数：
 *      folder              :   目录（绝对路径）
 *  返回：
 *      使用空间（MB）
 */
int BeGeneralLib::GetFolderSpace(string folder)
{
    string ShellCommand = string("du -s --block-size=M") + SEPARATOR_CHAR_SPACE + folder;
    return BeGeneralLib::StringToInt(ReadShellReturnValue(ShellCommand));
}

/*
 *  功能：
 *      检测实例是否存在
 *  参数
 *      无
 *  返回：
 *      存在返回true，否则返回false
 */
bool BeGeneralLib::IsInstanceExist()
{
    string Command = string("pgrep $(ps -e | grep ") + BeGeneralLib::Number2String(getpid()) + string(" | gawk '{print $4}')");
    vector<string> PidList = ReadShellReturnValueAll(Command);
    return (PidList.size() > 1);
}

/*
 *  功能：
 *      从文件中替换指定键的键值
 *  参数
 *      filename        :   文件名
 *      key             :   键名
 *      separator       :   键名和键值分割符
 *      新的键值
 *  返回：
 *      无
 */
bool BeGeneralLib::IsFileContainConfigValue(string filename, string key, string separator, string value)
{
    string ShellCommand = string("cat") + SEPARATOR_CHAR_SPACE + filename + string("| grep") + SEPARATOR_CHAR_SPACE + key + separator + value;
    return (IsStringEmpty(ReadShellReturnValue(ShellCommand)) == false);
}

/*
 *  功能：
 *      从文件中获取指定键值
 *  参数
 *      filename        :   文件名
 *      key             :   键名
 *      separator       :   键名和键值分割符
 *  返回：
 *      键值
 */
string BeGeneralLib::GetValueFromFile(string filename, string key, string separator)
{
    string ShellCommand = string("cat") + SEPARATOR_CHAR_SPACE + filename + string("| grep") + SEPARATOR_CHAR_SPACE + key + string("| gawk -F") + separator + SEPARATOR_CHAR_SPACE + string("'{print $2}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      从文件中替换指定键的键值
 *  参数
 *      filename        :   文件名
 *      key             :   键名
 *      separator       :   键名和键值分割符
 *      新的键值
 *  返回：
 *      无
 */
void BeGeneralLib::ReplaceValueFromFile(string filename, string key, string separator, string value)
{
    string ShellCommand = "";
    if (IsFileContainConfigValue(filename, key, separator, value) == false)
    {
        ShellCommand = string("sed -i") + SEPARATOR_CHAR_SPACE + string("$ a\\") + key + separator + value + separator + filename;
    }
    else
    {
        ShellCommand = string("sed -i") + SEPARATOR_CHAR_SPACE + string("/^") + key + separator + string("/c\\") + key + separator + value + SEPARATOR_CHAR_SPACE + filename;
    }
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      获取指定进行名的执行路径
 *  参数：
 *      processname     :   进程名
 *  返回：
 *      执行路径名
 */
string BeGeneralLib::GetProcessExecutePath(string processname)
{
    string ShellCommand = string("procname=$(pgrep -x ") + processname + string("| gawk '{print \"/proc/\"$1\"/exe\"}') && ls -l --time-style='long-iso' $procname | gawk '{print $10}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      将字符串类型数据写入文件中
 *  参数：
 *      fp          :   文件指针
 *      datastr     :   字符串数据
 *  返回：
 *      无
 */
void BeGeneralLib::WriteDataToFile(FILE *fp, string datastr)
{
    if (fp == NULL)
    {
        return;
    }
    fwrite(datastr.c_str(), strlen(datastr.c_str()), 1, fp);
    fflush(fp);
}

/*
 *  功能：
 *      执行系统命令
 *  参数
 *      shellcommand    :   执行命令串
 *  返回：
 *      无
 */
bool BeGeneralLib::ExecuteSystem(string shellcommand)
{
    return ExecuteSystem(shellcommand, true);
}

/*
 *  功能：
 *      执行系统命令
 *  参数
 *      shellcommand    :   执行命令串
 *      redirect        :   重定向
 *  返回：
 *      无
 */
bool BeGeneralLib::ExecuteSystem(string shellcommand, bool redirect)
{
    return ExecuteSystem(shellcommand, redirect, BesLog::LOG_MESSAGE_TYPE_NORMAL);
}

/*
 *  功能：
 *      执行系统命令
 *  参数
 *      shellcommand    :   执行命令串
 *      redirect        :   重定向
 *      debug           :   打印调试信息
 *  返回：
 *      无
 */
bool BeGeneralLib::ExecuteSystem(string shellcommand, bool redirect, bool debug)
{
    return ExecuteSystem(shellcommand, redirect, (debug == false) ? BesLog::LOG_MESSAGE_TYPE_VERBOSE : BesLog::LOG_MESSAGE_TYPE_NORMAL);
}

/*
 *  功能：
 *      执行系统命令
 *  参数
 *      shellcommand    :   执行命令串
 *      redirect        :   重定向
 *      msgtype         :   信息类型
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::ExecuteSystem(string shellcommand, bool redirect, BesLog::LogMessageType msgtype)
{
    string CommandStr = shellcommand;

//  redirect = false;		//调试用

    // 重定向
    if (redirect == true)
    {
        CommandStr = CommandStr + SEPARATOR_CHAR_SPACE + string("&> /dev/null");
    }

    BesLog::DebugPrint("ShellCommand:" + CommandStr, msgtype, true);

    pid_t Status = system(CommandStr.c_str());

    bool Flag = false;
    if (-1 == Status)
    {
        Flag = false;
    }
    else
    {
        if (WIFEXITED(Status))
        {
            if (0 == WEXITSTATUS(Status))
            {
                Flag = true;
            }
            else
            {
                Flag = false;
            }
        }
        else
        {
            Flag = false;
        }
    }
    return Flag;
}

/*
 *  功能：
 *      关闭套接字
 *  参数
 *      sock            :   执行命令串
 *  返回：
 *      无
 */
void BeGeneralLib::CloseSock(int sock)
{
    shutdown(sock, SHUT_RDWR);
    close(sock);
}

/*
 *  功能：
 *      检查服务是否存在
 *  参数
 *      servicename     :   服务名
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool BeGeneralLib::IsServiceExist(string servicename)
{
//此处要修改，centos7中
//    string FileName = string("/etc/init.d/") + servicename;
//    return IsFileExist(FileName);
	return true;
}

/*
 *  功能：
 *      检查服务是否正在运行
 *  参数
 *      servicename     :   服务名
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BeGeneralLib::IsServiceRunning(string servicename)
{
    // 检查输入的有效性
    if (IsStringEmptyOrInvalidValue(servicename) == true) return false;

    string PidFile1 = string(DEFAULT_VAR_RUN_PATH) + servicename + string(DEFAULT_PID_FILE_EXTENTSION);
    string PidFile2 = string(DEFAULT_VAR_RUN_PATH) + servicename + LINUX_PATH_SEPCHAR + servicename + string(DEFAULT_PID_FILE_EXTENTSION);
    return (IsStringEmpty(BeGeneralLib::GetProcessExecutePath(servicename)) == false
            || BeGeneralLib::IsProcessRunning(servicename) == true
            || IsFileExist(PidFile1) == true
            || IsFileExist(PidFile2) == true);
}

/*
 *  功能：
 *      操作服务
 *  参数
 *      action          :   动作
 *      servicename     :   服务名
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool BeGeneralLib::OperateService(BeGeneralLib::ServiceAction action, string servicename)
{
    string ShellCommand = DEFAULT_EMPTY_STRING;
    string ActionString = DEFAULT_EMPTY_STRING;
    switch (action)
    {
        case BeGeneralLib::SERVICE_ACTION_START:
        {
            ActionString = string("start");
            break;
        }
        case BeGeneralLib::SERVICE_ACTION_STOP:
        {
            ActionString = string("stop");
            break;
        }
        case BeGeneralLib::SERVICE_ACTION_RESTART:
        {
            ActionString = string("restart");
            break;
        }
    }

    // 执行命令
    ShellCommand = string("service") + SEPARATOR_CHAR_SPACE + servicename + SEPARATOR_CHAR_SPACE + ActionString;
    BeGeneralLib::ExecuteSystem(ShellCommand, true, false);

    return true;
}

/*
 *  功能：
 *      检查服务是否启动，如果没有根据启动标志启动
 *  参数：
 *      servicename     :   服务名称
 *      startflag       :   是否启动标志
 *  返回：
 *      如果启动返回true，否则返回false
 */
bool BeGeneralLib::CheckService(string servicename, bool startflag)
{
    // 检查虚拟机服务是否存在
    if (BeGeneralLib::IsServiceExist(servicename) == false) return false;
    // 检查服务是否运行
    if (IsServiceRunning(servicename) == true) return true;
    // 启动服务
    if (startflag == true) OperateService(BeGeneralLib::SERVICE_ACTION_START, servicename);

    return (IsServiceRunning(servicename) == true);
}

/*
 *  功能：
 *      加密
 *  参数
 *      input           :   输入
 *      enflag          :   加密标志
 *  返回：
 *      加密后的字符串
 */
string BeGeneralLib::Cipher(string input, bool enflag)
{
    return Cipher(input, input.length(), enflag);
}

/*
 *  功能：
 *      加密
 *  参数
 *      input           :   输入
 *      factor          :   加密因子
 *      enflag          :   加密标志
 *  返回：
 *      加密后的字符串
 */
string BeGeneralLib::Cipher(string input, int factor, bool enflag)
{
    string Output = "";
    for (int i = 0; i < input.length(); i++)
    {
        Output = Output + TransferTo(input.c_str()[i], factor, (enflag ? (i % 2) : !(i % 2)));
    }
    return Output;
}

/*
 *  功能：
 *      执行进程
 *  参数
 *      procstring      :   执行命令串
 *  返回：
 *      返回执行结果
 */
bool BeGeneralLib::StartupProcess(string procstring)
{
    BesLog::DebugPrint("ECommand:" + procstring);

    // 分解执行参数
    vector<string> ParaArray = StringSplit(procstring, SEPARATOR_CHAR_SPACE, BeGeneralLib::REMOVE_EMPTY_ITEM);
    if (ParaArray.size() == 0)
    {
        return false;
    }


    // 进程路径
    string ProcPath = ParaArray[0];
    if (IsFileExist(ProcPath) == false)
    {
        return false;
    }

    // 获取进程名
    string ProcName = "";
    int LastIndex = ProcPath.find_last_of(LINUX_PATH_SEPCHAR);
    if (LastIndex == -1)
    {
        ProcName = ProcPath;
    }
    else
    {
        ProcName = ProcPath.substr(LastIndex + 1, ProcPath.size() - LastIndex - 1);
    }

    // 设置执行参数
    char* Argv[MAX_EXEC_PARAMETER_NUMBER];
    Argv[0] = (char*) ProcName.c_str();
    for (int i = 1; i < ParaArray.size(); i++)
    {
        Argv[i] = (char*) ParaArray[i].c_str();
    }
    Argv[ParaArray.size()] = NULL;

    bool Flag = true;

    signal(SIGCLD, SIG_IGN);

    // 派生子进程执行
    pid_t ProcessID;
    switch (ProcessID = fork())
    {
        case -1:
        {
            // 这里pid为-1，fork函数失败,一些可能的原因是进程数或虚拟内存用尽
            BesLog::DebugPrint("The fork failed!");
            Flag = false;
            break;
        }
        case 0:
        {
            // pid为0，子进程
            if (execvp(ProcPath.c_str(), Argv) < 0)
            {
                BesLog::DebugPrint("Fork process failed!");
            }
            // 但是做完后, 我们需要做类似下面：
            _exit(0);
            break;
        }
        default:
        {
            // pid大于0，为父进程得到的子进程号
            BesLog::DebugPrint("Child process id:" + Number2String(ProcessID));
            Flag = true;
            break;
        }
    }
    return Flag;
}

/*
 *  功能：
 *      执行进程
 *  参数
 *      procstring      :   执行命令串
 *      isbackground    :   是否后台运行
 *  返回：
 *      执行成功返回true，否则返回false
 */
bool BeGeneralLib::StartupProcess(string procstring, bool isbackground)
{
    // 检查进程名是否为空
    if (IsStringEmptyOrInvalidValue(procstring) == true)
    {
        return false;
    }

    BesLog::DebugPrint("start process:" + procstring);

    // 设置执行命令字符串
    string ShellCommand = procstring;
    if (isbackground == true)
    {
        ShellCommand = ShellCommand + SEPARATOR_CHAR_SPACE + string("&");
    }
    return ExecuteSystem(ShellCommand, false, false);
}

/*
 *  功能：
 *      检查并创建目录
 *  参数
 *      path            :   路径名
 *  返回：
 *      无
 */
void BeGeneralLib::CheckAndCreateDir(string path)
{
    // 检查目录是否存在
    if (IsFileExist(path) == true) return;

    // 创建目录
    string ShellCommand = string("mkdir -p") + SEPARATOR_CHAR_SPACE + path;
    ExecuteSystem(ShellCommand, false, false);
    return;
}

/*
 *  功能：
 *      强制删除目录
 *  参数
 *      path            :   路径名
 *      todebug         :   是否输出
 *  返回：
 *      无
 */
void BeGeneralLib::ForceRemoveDirectory(string path, bool todebug)
{
    string ShellCommand = string("rm -rf") + SEPARATOR_CHAR_SPACE + path;
    BeGeneralLib::ExecuteSystem(ShellCommand, true, todebug);
}

/*
 *  功能：
 *      检测目录下文件个数
 *  参数：
 *      folder          :   目录
 *  返回：
 *      文件个数(含目录)
 */
int BeGeneralLib::CheckFileNumberInFolder(string folder)
{
    string ShellCommand = string("ls -l") + SEPARATOR_CHAR_SPACE + folder + string("| wc -l");
    return (BeGeneralLib::StringToInt(BeGeneralLib::ReadShellReturnValue(ShellCommand)) - 1);
}

/*
 *  功能：
 *      根据文件名获取上级目录名
 *  参数：
 *      file            :   文件名
 *  返回：
 *      目录名
 */
string BeGeneralLib::GetDirectoryName(string file)
{
    if (IsStringEmpty(file) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }
    string DirName = DEFAULT_EMPTY_STRING;
    if (IsFileExist(file) == true)
    {
        struct stat StatBuf;
        if (stat(file.c_str(), &StatBuf) == 0)
        {
            if (S_ISDIR(StatBuf.st_mode))
            {
                file = StringRTrim(file, LINUX_PATH_SEPCHAR) + LINUX_PATH_SEPCHAR;
            }
            else
            {
                file = StringRTrim(file, LINUX_PATH_SEPCHAR);
            }
        }
    }

    if (file.c_str()[file.size() - 1] == LINUX_PATH_SEPCHAR)
    {
        DirName = file;
    }
    else
    {
        DirName = ReadShellReturnValue(string("dirname") + SEPARATOR_CHAR_SPACE + file) + LINUX_PATH_SEPCHAR;
    }

    return DirName;
}

/*
 *  功能：
 *      根据文件全路径获取文件名
 *  参数：
 *      path            :   文件
 *  返回：
 *      文件名
 */
string BeGeneralLib::GetFileName(string path)
{
    string FileName = DEFAULT_EMPTY_STRING;

    // 检查该文件是否为普通文件
    if (GetFileType(path) == BeGeneralLib::FILE_TYPE_REGULAR)
    {
        // 读取文件的文件名
        string ShellCommand = string("basename") + SEPARATOR_CHAR_SPACE + path;
        FileName = ReadShellReturnValue(ShellCommand);
    }
    return FileName;
}

/*
 *  功能：
 *      获取文件类型
 *  参数：
 *      path            :   文件
 *  返回：
 *      文件类型
 */
BeGeneralLib::FileType BeGeneralLib::GetFileType(string path)
{
    BeGeneralLib::FileType RetFileType = BeGeneralLib::FILE_TYPE_UNKNOWN;

    // 获取该文件相关的信息
    struct stat StatBuf;
    bzero(&StatBuf, sizeof (StatBuf));

    // 检查文件是否存在
    if (IsFileExist(path) == false || stat(path.c_str(), &StatBuf) != 0)
    {
        return RetFileType;
    }

    // 检查文件类型
    switch (StatBuf.st_mode & S_IFMT)
    {
        case S_IFBLK: // 块设备
        {
            RetFileType = BeGeneralLib::FILE_TYPE_BLOCK;
            break;
        }
        case S_IFCHR: // 字符设备
        {
            RetFileType = BeGeneralLib::FILE_TYPE_CHARDEVICE;
            break;
        }
        case S_IFDIR: // 目录
        {
            RetFileType = BeGeneralLib::FILE_TYPE_DIRECTORY;
            break;
        }
        case S_IFIFO: // 先入先出设备或管道
        {
            RetFileType = BeGeneralLib::FILE_TYPE_FIFO;
            break;
        }
        case S_IFLNK: // 符号链接
        {
            RetFileType = BeGeneralLib::FILE_TYPE_SYMLINK;
            break;
        }
        case S_IFREG: // 普通文件
        {
            RetFileType = BeGeneralLib::FILE_TYPE_REGULAR;
            break;
        }
        case S_IFSOCK: // 套接字
        {
            RetFileType = BeGeneralLib::FILE_TYPE_SOCKET;
            break;
        }
        default:
        {
            break;
        }
    }
    return RetFileType;
}

/*
 *  功能：
 *     获取指定文件大小
 *  参数：
 *      filename        :   文件
 *  返回：
 *      文件大小
 */
long int BeGeneralLib::GetFileSize(string path)
{
    // 获取该文件相关的信息
    struct stat StatBuf;
    bzero(&StatBuf, sizeof (StatBuf));

    // 检查文件是否存在
    if (IsFileExist(path) == false || stat(path.c_str(), &StatBuf) != 0)
    {
        return 0;
    }
    return ((long) StatBuf.st_size);
}

/*
 *  功能：
 *      获取文件上次访问时间
 *  参数：
 *      path            :   文件
 *  返回：
 *      成功返回上次访问时间，否则返回-1
 */
time_t BeGeneralLib::GetFileAccessTime(string path)
{
    // 获取该文件相关的信息
    struct stat StatBuf;
    bzero(&StatBuf, sizeof (StatBuf));

    // 检查文件是否存在
    if (IsFileExist(path) == false || stat(path.c_str(), &StatBuf) != 0)
    {
        return -1;
    }

    return StatBuf.st_atim.tv_sec;
}

/*
 *  功能：
 *      获取文件上次修改时间
 *  参数：
 *      path            :   文件
 *  返回：
 *      成功返回上次修改时间，否则返回-1
 */
time_t BeGeneralLib::GetFileModificationTime(string path)
{
    // 获取该文件相关的信息
    struct stat StatBuf;
    bzero(&StatBuf, sizeof (StatBuf));

    // 检查文件是否存在
    if (IsFileExist(path) == false || stat(path.c_str(), &StatBuf) != 0)
    {
        return -1;
    }

    return StatBuf.st_mtim.tv_sec;
}

/*
 *  功能：
 *      获取文件上次状态修改时间
 *  参数：
 *      path            :   文件
 *  返回：
 *      成功返回上次状态修改时间，否则返回-1
 */
time_t BeGeneralLib::GetFileStatusChangeTime(string path)
{
    // 获取该文件相关的信息
    struct stat StatBuf;
    bzero(&StatBuf, sizeof (StatBuf));

    // 检查文件是否存在
    if (IsFileExist(path) == false || stat(path.c_str(), &StatBuf) != 0)
    {
        return -1;
    }

    return StatBuf.st_ctim.tv_sec;
}

/*
 *  功能：
 *      移动文件
 *  参数：
 *      source          :   源文件名
 *      dest            :   目标文件名
 *      overwrite       :   是否覆盖
 *  返回：
 *      操作成功返回true,否则返回false
 */
bool BeGeneralLib::MoveFile(string source, string dest, bool overwrite, bool debug)
{
    //    // 检查源文件是否存在
    //    if (BeGeneralLib::IsFileExist(source) == false && (source.find_last_of(SEPARATOR_CHAR_ASTERISK) != source.size() - 1))
    //    {
    //        return false;
    //    }
    //
    //    // 如果不覆盖，并且目标文件存在，则返回
    //    if (overwrite == false && BeGeneralLib::IsFileExist(dest) == true)
    //    {
    //        return true;
    //    }
    //
    //    // 如果覆盖，并且目标文件存在，则删除
    //    if (overwrite == true && BeGeneralLib::IsFileExist(dest) == true)
    //    {
    ////        BeGeneralLib::ForceRemoveDirectory(dest);
    //    }

    // 复制文件
    string ShellCommand = string("mv -f") + SEPARATOR_CHAR_SPACE + source + SEPARATOR_CHAR_SPACE + dest;
    BeGeneralLib::ExecuteSystem(ShellCommand, true, debug);

    // 检查目标文件是否存在
    return BeGeneralLib::IsFileExist(dest);
}

/*
 *  功能：
 *      复制文件
 *  参数：
 *      source          :   源文件名
 *      dest            :   目标文件名
 *      overwrite       :   是否覆盖
 *  返回：
 *      操作成功返回true,否则返回false
 */
bool BeGeneralLib::CopyFile(string source, string dest, bool overwrite, bool debug)
{
    //    // 检查源文件是否存在
    //    if (BeGeneralLib::IsFileExist(source) == false && (source.find_last_of(SEPARATOR_CHAR_ASTERISK) != source.size() - 1))
    //    {
    //        return false;
    //    }
    //
    //    // 如果不覆盖，并且目标文件存在，则返回
    //    if (overwrite == false && BeGeneralLib::IsFileExist(dest) == true)
    //    {
    //        return true;
    //    }
    //
    //    // 如果覆盖，并且目标文件存在，则删除
    //    if (overwrite == true && BeGeneralLib::IsFileExist(dest) == true)
    //    {
    //        //  BeGeneralLib::ForceRemoveDirectory(dest);
    //    }

    // 复制文件
    string ShellCommand = string("\\cp -f") + SEPARATOR_CHAR_SPACE + source + SEPARATOR_CHAR_SPACE + dest;
    BeGeneralLib::ExecuteSystem(ShellCommand, true, debug);

    // 检查目标文件是否存在
    return BeGeneralLib::IsFileExist(dest);
}

/*
 *  功能：
 *      修改文件时间戳
 *  参数：
 *      path            :   文件名
 *  返回：
 *      无
 */
void BeGeneralLib::ChangeFileTimestamp(string path)
{
    string ShellCommand = string("touch") + SEPARATOR_CHAR_SPACE + path;
    BeGeneralLib::ExecuteSystem(ShellCommand, true, false);
}

/*
 *  功能：
 *      获取当前系统时间
 *  参数：
 *      无
 *  返回：
 *      系统时间值
 */
time_t BeGeneralLib::GetSystemTime()
{
    // 获取当前时间
    time_t NowTime = 0;
    time(&NowTime);
    return NowTime;
}

/*
 *  功能：
 *      获取当前时间格式
 *  参数：
 *      format           :   时间格式
 *  返回：
 *      时间字符串
 */
string BeGeneralLib::GetTimeString(BeGeneralLib::TimeFormat format)
{
    // 获取当前时间
    time_t NowTime;
    time(&NowTime);

    return GetTimeString(NowTime, format);
}

/*
 *  功能：
 *      获取当前时间格式
 *  参数：
 *      time            :   时间
 *      format          :   时间格式
 *  返回：
 *      时间字符串
 */
string BeGeneralLib::GetTimeString(time_t time, BeGeneralLib::TimeFormat format)
{
    return GetTimeString(localtime(&time), format);
}

/*
 *  功能：
 *      获取当前时间格式
 *  参数：
 *      tmptr           :   时间结构指针
 *      format          :   时间格式
 *  返回：
 *      时间字符串
 */
string BeGeneralLib::GetTimeString(struct tm *tmptr, BeGeneralLib::TimeFormat format)
{
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    bzero(Buf, sizeof (Buf));

    // 获取本地时间
    struct tm * CurrentTime = tmptr;

    string FormatString = GetTimeFormatString(format);

    // 转换为YYYYMMDDHHMMSS格式
    strftime(Buf, sizeof (Buf) - 1, FormatString.c_str(), CurrentTime);
    string TimeString = string(Buf);
    if (format == BeGeneralLib::TIME_FORMAT_COMMAND_HEADER)
    {
        struct timeval NowTime;
        gettimeofday(&NowTime, NULL);
        bzero(Buf, sizeof (Buf));
        sprintf(Buf, "%06ld", NowTime.tv_usec);
        TimeString = TimeString + string(Buf);
    }
    return TimeString;
}

/*
 *  功能：
 *      获取当前时间格式
 *  参数：
 *      tmptr           :   时间结构指针
 *      format          :   时间格式
 *  返回：
 *      时间字符串
 */
bool BeGeneralLib::SetSystemTime(string timestring)
{
    //    string FormatString = string("%Y%m%d%H%M%S");
    //
    //    // 获取本地时间
    //    struct tm ConvertTime;
    //
    //    // 将字符串结构的时间转换为tm结构的时间
    //    strptime(timestring.c_str(), FormatString.c_str(), &ConvertTime);
    //
    //    // 转换为
    time_t NewTime = ConvertStringToTime(timestring, BeGeneralLib::TIME_FORMAT_FULL_DATETIME);
    stime(&NewTime);
}

/*
 *  功能：
 *      将时间字符串转换为时间类型
 *  参数：
 *      timestring      :   时间结构指针
 *      format          :   时间格式
 *  返回：
 *      时间类型
 */
time_t BeGeneralLib::ConvertStringToTime(string timestring, BeGeneralLib::TimeFormat format)
{
    string FormatString = GetTimeFormatString(format);

    // 获取本地时间
    struct tm ConvertTime;
    bzero(&ConvertTime, sizeof (ConvertTime));

    // 将字符串结构的时间转换为tm结构的时间
    strptime(timestring.c_str(), FormatString.c_str(), &ConvertTime);

    // 转换为
    time_t NewTime = mktime(&ConvertTime);
    return NewTime;
}

/*
 *  功能：
 *      计算周期时间
 *  参数：
 *      starttime       :   开始时间
 *      currenttime     :   当前时间
 *      cycle           :   周期
 *      interval        :   间隔
 *  返回：
 *      下次运行时间
 */
string BeGeneralLib::CalcCycleTime(string starttime, string currenttime, BeGeneralLib::TimeCycle cycle, int interval)
{
    if (interval == 0)
    {
        return starttime;
    }

    time_t StartTime = ConvertStringToTime(starttime, BeGeneralLib::TIME_FORMAT_LONG);
    time_t CurrentTime = ConvertStringToTime(currenttime, BeGeneralLib::TIME_FORMAT_LONG);
    time_t NewTime;
    switch (cycle)
    {
        case BeGeneralLib::TIME_CYCLE_MINUTE:
        case BeGeneralLib::TIME_CYCLE_HOUR:
        case BeGeneralLib::TIME_CYCLE_DAY:
        {
            TimeUnit TmUnit = TIME_UNIT_HOUR;
            if (cycle == BeGeneralLib::TIME_CYCLE_DAY)
            {
                TmUnit = BeGeneralLib::TIME_UNIT_DAY;
            }
            else if (cycle == BeGeneralLib::TIME_CYCLE_MINUTE)
            {
                TmUnit = BeGeneralLib::TIME_UNIT_MINUTE;
            }
            NewTime = StartTime;
            for (;;)
            {
                NewTime = DatetimeAdd(NewTime, TmUnit, interval);
                if (NewTime >= CurrentTime || NewTime < 0)
                {
                    break;
                }
            }
            break;
        }
        case BeGeneralLib::TIME_CYCLE_WEEK:
        case BeGeneralLib::TIME_CYCLE_MONTH:
        {
            string TimeString = GetTimeString(CurrentTime, BeGeneralLib::TIME_FORMAT_FULL_DATE) + GetTimeString(StartTime, BeGeneralLib::TIME_FORMAT_FULL_TIME);
            //            cout << "TimeString:" << TimeString << endl;
            NewTime = ConvertStringToTime(TimeString, BeGeneralLib::TIME_FORMAT_FULL_DATETIME);
            //ShowTime(NewTime);
            int CurrentIndex = 0;
            if (cycle == BeGeneralLib::TIME_CYCLE_WEEK)
            {
                CurrentIndex = GetWeekday(CurrentTime);
            }
            else
            {
                CurrentIndex = GetMonthDay(CurrentTime);
            }

            int Difference = 0;
            if (CurrentIndex == interval)
            {
                if (CompareTime(NewTime, CurrentTime) >= 0)
                {
                    break;
                }
                else
                {
                    Difference = 1;
                }
            }
            for (;;)
            {
                NewTime = DatetimeAdd(NewTime, BeGeneralLib::TIME_UNIT_DAY, 1);
                int TempIndex = 0;
                if (cycle == BeGeneralLib::TIME_CYCLE_WEEK)
                {
                    TempIndex = GetWeekday(NewTime);
                }
                else
                {
                    TempIndex = GetMonthDay(NewTime);
                }
                if (TempIndex == interval || NewTime < 0)
                {
                    break;
                }
                Difference++;
            }
            break;
        }
    }
    return BeGeneralLib::GetTimeString(NewTime, BeGeneralLib::TIME_FORMAT_LONG);
}

/*
 *  功能：
 *      日期时间加减
 *  参数：
 *      time            :   时间
 *      unit            :   时间单位
 *      interval        :   间隔（可以为负值）
 *  返回：
 *      运算后的时间
 */
time_t BeGeneralLib::DatetimeAdd(time_t time, BeGeneralLib::TimeUnit unit, int interval)
{
    return DatetimeAdd(BeGeneralLib::GetTimeString(time, BeGeneralLib::TIME_FORMAT_LONG), unit, interval);
}

/*
 *  功能：
 *      日期时间加减
 *  参数：
 *      time            :   时间
 *      unit            :   时间单位
 *      interval        :   间隔（可以为负值）
 *  返回：
 *      运算后的时间
 */
time_t BeGeneralLib::DatetimeAdd(string time, BeGeneralLib::TimeUnit unit, int interval)
{
    int Negative = (interval < 0 ? 1 : 0);
    interval = abs(interval);

    string Option = time + SEPARATOR_CHAR_SPACE + BeGeneralLib::Number2String(interval) + SEPARATOR_CHAR_SPACE + GetTimeUnitString(unit);
    if (Negative)
    {
        Option = Option + SEPARATOR_CHAR_SPACE + string("ago");
    }
    string ShellCommand = string("date \"+%F %T\" -d '") + Option + string("'");
    return ConvertStringToTime(ReadShellReturnValue(ShellCommand), BeGeneralLib::TIME_FORMAT_LONG);
}

/*
 *  功能：
 *      返回日期间隔的小时数（time1 - time2）
 *  参数：
 *      time1           :   时间1
 *      time2           :   时间2
 *  返回：
 *      间隔的小时数
 */
double BeGeneralLib::DiffTimeToHours(time_t time1, time_t time2)
{
    return (difftime(time1, time2) / 3600);
}

/*
 *  功能：
 *      返回日期间隔的天数（time1 - time2）
 *  参数：
 *      time1           :   时间1
 *      time2           :   时间2
 *  返回：
 *      间隔的天数
 */
double BeGeneralLib::DiffTimeToDays(time_t time1, time_t time2)
{
    return (difftime(time1, time2) / 86400);
}

/*
 *  功能：
 *      获取转换后的时间星期几索引(星期一-1，星期二-2...星期日-7)
 *  参数：
 *      time            :   时间
 *  返回：
 *      星期几索引
 */
int BeGeneralLib::GetWeekday(time_t time)
{
    return GetWeekday(localtime(&time));
}

/*
 *  功能：
 *      获取转换后的时间星期几索引(星期一-1，星期二-2...星期日-7)
 *  参数：
 *      tmptr           :   时间结构
 *  返回：
 *      星期几索引
 */
int BeGeneralLib::GetWeekday(struct tm * tmptr)
{
    if (tmptr == NULL)
    {
        return 1;
    }
    if (tmptr->tm_wday == 0)
    {
        return 7;
    }
    return tmptr->tm_wday;
}

/*
 *  功能：
 *      获取指定日期中月的第几天
 *  参数：
 *      time            :   时间
 *  返回：
 *      月的第几天
 */
int BeGeneralLib::GetMonthDay(time_t time)
{
    return GetMonthDay(localtime(&time));
}

/*
 *  功能：
 *      获取指定日期中月的第几天
 *  参数：
 *      tmptr           :   时间结构
 *  返回：
 *      月的第几天
 */
int BeGeneralLib::GetMonthDay(struct tm * tmptr)
{
    return (tmptr->tm_mday);
}

/*
 *  功能：
 *      比较两个给定时间的时间部分大小
 *  参数：
 *      time1           :   时间1
 *      time2           :   时间2
 *  返回：
 *      如果返回大于0，则time1 > time2
 *      如果等于0，则time1 = time2
 *      如果小于0，则time1 < time2
 */
int BeGeneralLib::CompareTime(time_t time1, time_t time2)
{
    time_t NewTime1 = ConvertStringToTime(BeGeneralLib::GetTimeString(time1, BeGeneralLib::TIME_FORMAT_FULL_DATE) + BeGeneralLib::GetTimeString(time1, BeGeneralLib::TIME_FORMAT_FULL_TIME), BeGeneralLib::TIME_FORMAT_FULL_DATETIME);
    time_t NewTime2 = ConvertStringToTime(BeGeneralLib::GetTimeString(time1, BeGeneralLib::TIME_FORMAT_FULL_DATE) + BeGeneralLib::GetTimeString(time2, BeGeneralLib::TIME_FORMAT_FULL_TIME), BeGeneralLib::TIME_FORMAT_FULL_DATETIME);

    return difftime(NewTime1, NewTime2);
}

/*
 *  功能：
 *      计算给定时间和当前时间的时间绝对值（单位秒）
 *  参数：
 *      time1           :   时间1
 *  返回：
 *      时间差
 */
double BeGeneralLib::DiffTime(time_t time1)
{
    time_t NowTime;
    time(&NowTime);
    return DiffTime(time1, NowTime, true);
}

/*
 *  功能：
 *      计算两个给定时间的时间差（单位秒）
 *  参数：
 *      time1           :   时间1
 *      time2           :   时间2
 *      isabs           :   是否为绝对值
 *  返回：
 *      时间差
 */
double BeGeneralLib::DiffTime(time_t time1, time_t time2, bool isabs)
{
    double TotalSeconds = difftime(time1, time2);
    if (isabs == true)
    {
        TotalSeconds = fabs(TotalSeconds);
    }
    return TotalSeconds;
}

/*
 *  功能：
 *      文件是否存在
 *  参数：
 *      file            :   文件名
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool BeGeneralLib::IsFileExist(string file)
{
    return (access(file.c_str(), F_OK) == 0);
}

/*
 *  功能：
 *      获取应用程序名
 *  参数：
 *      无
 *  返回：
 *      应用程序名
 */
string BeGeneralLib::GetApplicationName()
{
    string ShellCommand = string("ps -e | grep") + SEPARATOR_CHAR_SPACE + BeGeneralLib::Number2String(getpid()) + string("| gawk '{print $4}'");
    return ReadShellReturnValue(ShellCommand, false);
}

/*
 *  功能：
 *      对字符串的每一位字符进行求和
 *  参数：
 *      str             :   输入字符串
 *  返回：
 *      返回求和值
 */
int BeGeneralLib::SumStringChars(string str)
{
    int Sum = 0;

    // 如果为空则返回0
    if (IsStringEmpty(str) == true)
    {
        return 0;
    }

    // 对每一位数字进行求和
    for (int i = 0; i < str.length(); i++)
    {
        char ch = str.c_str()[i];
        if (isdigit(ch))
        {
            Sum += ch - DEFAULT_NUMBER_START_CHAR;
        }
    }
    return Sum;
}

/*
 *  功能：
 *      检查字符串是否全部为数字
 *  参数：
 *      str             :   待检查的字符串
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeGeneralLib::IsAllDigits(string str)
{
    if (IsStringEmpty(str) == true)
    {
        return false;
    }

    bool Flag = true;
    for (int i = 0; i < str.length(); i++)
    {
        if (!isdigit(str.data()[i]))
        {
            Flag = false;
        }
    }
    return Flag;
}

/*
 *  功能：
 *      检查字符串是否全部为十六进制数
 *  参数：
 *      str             :   待检查的字符串
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeGeneralLib::IsAllHexDigits(string str)
{
    // 检查字符串是否为空
    if (IsStringEmpty(str) == true)
    {
        return false;
    }

    // 检查每一位字符是否为十六进制数
    for (int i = 0; i < str.length(); i++)
    {
        if (isxdigit(str.data()[i]) == false)
        {
            return false;
        }
    }
    return true;
}

/*
 *  功能：
 *      检查字符串是否为有效的IP
 *  参数：
 *      ipstr           :   ip字符串
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeGeneralLib::IsValidIp(string ipstr)
{
    // 拆分字符串
    vector<string> IpSegmentArray = BeGeneralLib::StringSplit(ipstr, SEPARATOR_CHAR_DOT, WITH_EMPTY_ITEM);

    // 检查ip段数
    if (IpSegmentArray.size() != DEFAULT_IP_SEGMENT_NUM)
    {
        return false;
    }

    for (int i = 0; i < IpSegmentArray.size(); i++)
    {
        // 检查每段是否为数字
        if (IsAllDigits(IpSegmentArray[i]) == false)
        {
            return false;
        }

        // 检查每段的值是否位于0~255之间
        int SegmentValue = BeGeneralLib::StringToInt(IpSegmentArray[i]);
        if (SegmentValue < MIN_IP_SEGMENT_VALUE || SegmentValue > MAX_IP_SEGMENT_VALUE)
        {
            return false;
        }
    }

    return true;
}

/*
 *  功能：
 *      检查字符串是否为有效的mac地址
 *  参数：
 *      macstr           :   mac字符串
 *  返回：
 *      如果是则返回true，否则返回false
 */
bool BeGeneralLib::IsValidMacAddress(string macstr)
{
    // 验证mac地址的长度
    if (macstr.size() != DEFAULT_MAC_ADDRESS_LENGTH)
    {
        return false;
    }

    // 以(:)分割mac地址
    vector<string > MacSegmentArray = BeGeneralLib::StringSplit(macstr, SEPARATOR_CHAR_COLON, BeGeneralLib::WITH_EMPTY_ITEM);
    if (MacSegmentArray.size() != DEFAULT_MAC_SEGMENT_NUM)
    {
        // 以(-)分割mac地址
        MacSegmentArray = BeGeneralLib::StringSplit(macstr, SEPARATOR_CHAR_MINUS_SIGN, BeGeneralLib::WITH_EMPTY_ITEM);
    }

    // 验证mac地址段数
    if (MacSegmentArray.size() != DEFAULT_MAC_SEGMENT_NUM)
    {
        return false;
    }

    // 验证每段是否合法
    for (int i = 0; i < MacSegmentArray.size(); i++)
    {
        string SingleSegment = MacSegmentArray[i];
        if (SingleSegment.size() != DEFAULT_MAC_SINGLE_SEGMENT_LENGTH || IsAllHexDigits(SingleSegment) == false)
        {
            return false;
        }
    }
    return true;
}

/*
 *  功能：
 *      生成kvm虚拟机的MAC地址
 *  参数
 *      无
 *  返回：
 *      成功返回MAC地址，否则返回空
 */
string BeGeneralLib::GenerateKvmMac()
{
    string ShellCommand = string("MACADDR=\"52:54:$(dd if=/dev/urandom count=1 2>/dev/null | md5sum | sed 's/^\\(..\\)\\(..\\)\\(..\\)\\(..\\).*$/\\1:\\2:\\3:\\4/')\" && echo $MACADDR");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      检查值是否存在于列表中
 *  参数：
 *      list            :   列表
 *      value           :   值
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool BeGeneralLib::IsValueExist(vector<string> list, string value)
{
    return (list.empty() == false && find(list.begin(), list.end(), value) != list.end());
}

/*
 *  功能：
 *      获取配置项值
 *  参数：
 *      item            :   项名称
 *  返回：
 *      配置项值
 */
string BeGeneralLib::GetConfigItemValue(string item)
{
    // 读取配置文件中的用户名和密码
    string ConfigFileName = BeGeneralLib::GetExecutePath() + DEFAULT_CONFIG_FILE_NAME;
    return GetConfigItemValue(ConfigFileName, item);
}

/*
 *  功能：
 *      获取配置项值
 *  参数：
 *      configfile      :   配置文件名
 *      item            :   项名称
 *  返回：
 *      配置项值
 */
string BeGeneralLib::GetConfigItemValue(string configfile, string item)
{
    return GetConfigItemValue(configfile, item, "=");
}

/*
 *  功能：
 *      获取配置项值
 *  参数：
 *      configfile      :   配置文件名
 *      item            :   项名称
 *      separator       :   分割符
 *  返回：
 *      配置项值
 */
string BeGeneralLib::GetConfigItemValue(string configfile, string item, string separator)
{
    string ItemValue = "";

    // 读取配置文件中的项
    if (IsFileExist(configfile) == true)
    {
        ItemValue = BeGeneralLib::GetValueFromFile(configfile, item, separator);
    }
    return ItemValue;
}

/*
 *  功能：
 *     检查设备是否挂载
 *  参数：
 *      device          :   设备
 *      mountpoint      :   挂载点
 *  返回：
 *      如果挂载返回true，否则返回false
 */
bool BeGeneralLib::IsDeviceMounted(string device)
{
    return IsDeviceMounted(device, BeGeneralLib::MOUNTED_CHECK_ITEM_DEVICE);
}

/*
 *  功能：
 *     检查设备是否挂载
 *  参数：
 *      mountedvalue    :   挂载值
 *      item            :   检查项
 *  返回：
 *      如果挂载返回true，否则返回false
 */
bool BeGeneralLib::IsDeviceMounted(string mountedvalue, BeGeneralLib::MountedCheckItem item)
{
    // 有效值检查
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(mountedvalue) == true)
    {
        return false;
    }

    // 检查是否挂载
    string ShellCommand = "";
    switch (item)
    {
        case BeGeneralLib::MOUNTED_CHECK_ITEM_DEVICE:
        {
            ShellCommand = string("cat /proc/mounts | gawk '{if($1==\"") + mountedvalue + string("\") print $1}'");
            break;
        }
        case BeGeneralLib::MOUNTED_CHECK_ITEM_MOUNTPOINT:
        {
            mountedvalue = BeGeneralLib::RemovePathSeparator(mountedvalue);
            ShellCommand = string("cat /proc/mounts | gawk '{if($2==\"") + mountedvalue + string("\") print $2}'");
            break;
        }
    }

    return (mountedvalue == BeGeneralLib::ReadShellReturnValue(ShellCommand));
}

/*
 *  功能：
 *     检查设备是否挂载
 *  参数：
 *      device          :   设备
 *      mountpoint      :   挂载点
 *  返回：
 *      如果挂载返回true，否则返回false
 */
bool BeGeneralLib::IsDeviceMounted(string device, string mountpoint)
{
    // 删除后缀分割符
    mountpoint = RemovePathSeparator(mountpoint);

    // 如果挂载点为空
    if (IsStringEmpty(mountpoint) == true)
    {
        return IsDeviceMounted(device);
    }

    // 检查设备是否为空
    if (IsStringEmpty(device) == true)
    {
        return IsDeviceMounted(mountpoint, BeGeneralLib::MOUNTED_CHECK_ITEM_MOUNTPOINT);
    }

    // 检查是否挂载
    string ShellCommand = string("cat /proc/mounts | gawk '{if($2==\"") + mountpoint + string("\") print $1}'");
    string MountDevice = BeGeneralLib::ReadShellReturnValue(ShellCommand);

    // 检查该挂载点的设备是否和输入设备一致
    return ((device == MountDevice) || (GetLinkFileFullPath(device) == MountDevice) || (MountDevice == GetDeviceName(device)));
}

/*
 *  功能：
 *      挂载设备
 *  参数：
 *      device          :   设备
 *      mountpoint      :   挂载点
 *      filesystem      :   文件系统
 *      cratemountpoint :   是否创建挂载点
 *  返回：
 *      如果挂载成功返回true，否则返回false
 */
bool BeGeneralLib::MountDevice(string device, string mountpoint, BaseFileSystem filesystem, bool createmountpoint)
{
    return MountDevice(device, mountpoint, GetFileSystemName(filesystem), createmountpoint);
}

/*
 *  功能：
 *      挂载设备
 *  参数：
 *      device          :   设备
 *      mountpoint      :   挂载点
 *      filesystem      :   文件系统
 *      cratemountpoint :   是否创建挂载点
 *  返回：
 *      如果挂载成功返回true，否则返回false
 */
bool BeGeneralLib::MountDevice(string device, string mountpoint, string filesystem, bool createmountpoint)
{
    // 删除后缀分割符
    mountpoint = RemovePathSeparator(mountpoint);

    // 检查磁盘是否已经挂载
    if (BeGeneralLib::IsDeviceMounted(device, mountpoint) == true)
    {
        return true;
    }

    // 检查设备是否存在
    if (BeGeneralLib::IsDeviceExist(device) == false)
    {
        BesLog::DebugPrint("Device does not exist!\n");
        return false;
    }

    // 检查挂载点是否存在
    if (IsStringEmpty(mountpoint) == true || (createmountpoint == false && BeGeneralLib::IsFileExist(mountpoint) == false))
    {
    	BesLog::DebugPrint(string("mount point wrong : ")+mountpoint, BesLog::LOG_MESSAGE_TYPE_ERROR, false);
        return false;
    }

    // 创建mount目录
    BeGeneralLib::CheckAndCreateDir(mountpoint);

    // 设置文件系统
    string FileSystemName = filesystem;

    bool ProcFlag = false;
    time_t StartTime;
    time(&StartTime);
    while (true)
    {
        string ShellCommand = DEFAULT_EMPTY_STRING;

        // 生成挂载指令
        if (IsStringEmptyOrInvalidValue(FileSystemName) == false)
        {
            ShellCommand = string("mount -t") + SEPARATOR_CHAR_SPACE + FileSystemName + SEPARATOR_CHAR_SPACE + device + SEPARATOR_CHAR_SPACE + mountpoint;
            BeGeneralLib::ExecuteSystem(ShellCommand);

            sleep(5);

            // 检查设备是否已经挂载
            if (IsDeviceMounted(device, mountpoint) == true)
            {
                ProcFlag = true;
                break;
            }
        }

        // 如果未挂载，则设置使用mount方法进行操作
        if (mount(device.c_str(), mountpoint.c_str(), FileSystemName.c_str(), MS_MGC_VAL, NULL) == 0)
        {
            sleep(5);
            ProcFlag = true;
            break;
        }

        // 生成挂载指令，没有使用文件系统
        ShellCommand = string("mount") + SEPARATOR_CHAR_SPACE + device + SEPARATOR_CHAR_SPACE + mountpoint;
        BeGeneralLib::ExecuteSystem(ShellCommand);

        sleep(5);

        // 检查设备是否已经挂载
        if (IsDeviceMounted(device, mountpoint) == true)
        {
            ProcFlag = true;
            break;
        }

        // 检查超时
        time_t NowTime;
        time(&NowTime);
        if (DiffTime(NowTime, StartTime) > 30)
        {
            ProcFlag = false;
            break;
        }
    }
    return ProcFlag;
}

/*
 *  功能：
 *      根据挂载点获取挂载设备
 *  参数：
 *      mountpoint      :   挂载点
 *  返回：
 *      挂载的设备名
 */
string BeGeneralLib::GetMountedDevice(string mountpoint)
{
    string ShellCommand = string("cat /proc/mounts | gawk '{if ($2 == \"") + BeGeneralLib::RemovePathSeparator(mountpoint) + string("\") print $1}'");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      卸载设备
 *  参数：
 *      device          :   设备
 *      mountpoint      :   挂载点
 *      removemountpoint    :   是否删除挂载点
 *  返回：
 *      如果卸载成功返回true，否则返回false
 */
bool BeGeneralLib::UmountDevice(string device, string mountpoint, bool removemountpoint)
{
    // 删除后缀分割符
    mountpoint = RemovePathSeparator(mountpoint);

    // 检查磁盘是否已经卸载
    if (BeGeneralLib::IsDeviceMounted(device, mountpoint) == false)
    {
        return true;
    }

    bool ProcFlag = false;
    time_t StartTime;
    time(&StartTime);
    while (true)
    {
        // 卸载设备
        string ShellCommand = "";
        if (IsStringEmpty(mountpoint) == false)
        {
            ShellCommand = string("umount") + SEPARATOR_CHAR_SPACE + mountpoint;
        }
        else
        {
            ShellCommand = string("umount") + SEPARATOR_CHAR_SPACE + device;
        }
        BeGeneralLib::ExecuteSystem(ShellCommand, true, false);

        sleep(5);

        // 检查磁盘是否已经卸载
        if (BeGeneralLib::IsDeviceMounted(device, mountpoint) == false)
        {
            ProcFlag = true;
            break;
        }

        // 如果卸载未成功，则通过umount2方法
        if (umount2(mountpoint.c_str(), MNT_FORCE) == 0)
        {
            ProcFlag = true;
            break;
        }

        // 检查超时
        time_t NowTime;
        time(&NowTime);
        if (DiffTime(NowTime, StartTime) > 30)
        {
            ProcFlag = false;
            break;
        }
    }

    // 删除挂载目录
    if (removemountpoint == true && IsStringEmpty(mountpoint) == false)
    {
        BeGeneralLib::ForceRemoveDirectory(mountpoint, false);
    }

    return ProcFlag;
}

/*
 *  功能：
 *      获取指定设备的文件系统
 *  参数：
 *      device          :   设备
 *  返回：
 *      如果成功返回文件系统，否则返回空
 */
string BeGeneralLib::GetDeviceFileSystem(string device)
{
    string FileSystemString = DEFAULT_EMPTY_STRING;

    // 检查设备是否存在
    if (IsDeviceExist(device) == false)
    {
        return FileSystemString;
    }

    // 检查是否挂载
    string ShellCommand = string("cat /proc/mounts | gawk '{if($1==\"") + device + string("\") print $3}'");
    FileSystemString = BeGeneralLib::ReadShellReturnValue(ShellCommand);

    if (IsStringEmpty(FileSystemString) == true)
    {
        ShellCommand = string("blkid -o export") + SEPARATOR_CHAR_SPACE + device + string("| gawk -F= '{if($1==\"TYPE\") print $2}'");
        FileSystemString = BeGeneralLib::ReadShellReturnValue(ShellCommand);
    }

    if (IsStringEmpty(FileSystemString) == true)
    {
        ShellCommand = string("cat /proc/mounts | gawk '{if($1==\"") + GetDeviceName(device) + string("\") print $3}'");
        FileSystemString = BeGeneralLib::ReadShellReturnValue(ShellCommand);
    }
    return FileSystemString;
}

/*
 *  功能：
 *      获取文件系统类型名称
 *  参数：
 *      filesystem       :   设备
 *  返回：
 *      如果成功返回文件系统，否则返回空
 */
string BeGeneralLib::GetFileSystemName(BaseFileSystem filesystem)
{
    // 设置文件系统
    string FileSystemName = "";
    switch (filesystem)
    {
        case BASE_FILESYSTEM_EXT3:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_EXT3;
            break;
        }
        case BASE_FILESYSTEM_EXT4:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_EXT4;
            break;
        }
        case BASE_FILESYSTEM_BTRFS:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_BTRFS;
            break;
        }
        case BASE_FILESYSTEM_FAT32:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_FAT32;
            break;
        }
        case BASE_FILESYSTEM_NTFS:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_NTFS_3G;
            break;
        }
        case BASE_FILESYSTEM_RAW:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_RAW;
            break;
        }
        case BASE_FILESYSTEM_NILFS2:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_NILFS2;
            break;
        }
        case BASE_FILESYSTEM_REISERFS:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_REISERFS;
            break;
        }
    }
    return FileSystemName;
}

/*
 *  功能：
 *      根据文件系统名获取基本文件系统类型
 *  参数：
 *      filesystemname   :   文件系统名
 *  返回：
 *      基本文件系统类型
 */
BaseFileSystem BeGeneralLib::GetBaseFileSystem(string filesystemname)
{
    string FileSystemName = BeGeneralLib::StringToLower(filesystemname);
    if (FileSystemName == DEFAULT_FILESYSTEM_TYPE_EXT3)
    {
        return BASE_FILESYSTEM_EXT3;
    }
    else if (FileSystemName == DEFAULT_FILESYSTEM_TYPE_EXT4)
    {
        return BASE_FILESYSTEM_EXT4;
    }
    else if (FileSystemName == DEFAULT_FILESYSTEM_TYPE_BTRFS)
    {
        return BASE_FILESYSTEM_BTRFS;
    }
    else if (FileSystemName == DEFAULT_FILESYSTEM_TYPE_FAT32)
    {
        return BASE_FILESYSTEM_FAT32;
    }
    else if (FileSystemName == DEFAULT_FILESYSTEM_TYPE_NILFS2)
    {
        return BASE_FILESYSTEM_NILFS2;
    }
    else if (FileSystemName == DEFAULT_FILESYSTEM_TYPE_NTFS || FileSystemName == DEFAULT_FILESYSTEM_TYPE_NTFS_3G)
    {
        return BASE_FILESYSTEM_NTFS;
    }
    else if (FileSystemName == DEFAULT_FILESYSTEM_TYPE_RAW)
    {
        return BASE_FILESYSTEM_RAW;
    }
    else if (FileSystemName == DEFAULT_FILESYSTEM_TYPE_REISERFS)
    {
        return BASE_FILESYSTEM_REISERFS;
    }
    return BASE_FILESYSTEM_UNKNOWN;
}

/*
 *  功能：
 *      检查设备是否存在
 *  参数：
 *      device          :   设备
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool BeGeneralLib::IsDeviceExist(string device)
{
    return BeGeneralLib::IsFileExist(device);
}

/*
 *  功能：
 *      根据设备链接获取设备名
 *  参数：
 *      devlink         :   设备链接
 *  返回：
 *      成功返回设备名，否则返回空
 */
string BeGeneralLib::GetDeviceName(string devlink)
{
    string ShellCommand = string("udevadm info -q property -n") + SEPARATOR_CHAR_SPACE + devlink + string("| gawk -F= '$0~/^DEVNAME/ {print $2}'");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      根据设备名获取设备链接
 *  参数：
 *      devname         :   设备名
 *  返回：
 *      设备链接
 */
vector<string> BeGeneralLib::GetDeviceLinks(string devname)
{
    string ShellCommand = string("udevadm info -q property -n") + SEPARATOR_CHAR_SPACE + devname + string("| gawk -F= '$0~/^DEVLINKS/ {print $2}'  | gawk -F" " '{for(i=1;i<=NF;i++) print $i}'");
    return BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
}

/*
 *  功能：
 *      获取空闲的loop设备
 *  参数：
 *      无
 *  返回：
 *      返回空闲的loop设备
 */
string BeGeneralLib::GetFreeLoopDevice()
{
    // 检查是否存在空闲可用的loop设备
    string ShellCommand = string("losetup -f");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      设置image对应的loop设备
 *  参数：
 *      imagefile       :   image文件
 *  返回：
 *      返回关联的loop设备
 */
string BeGeneralLib::SetLoopDevice(string imagefile)
{
    return SetLoopDevice(imagefile, "");
}

/*
 *  功能：
 *      设置image文件关联的loop设备
 *  参数：
 *      imagefile       :   image文件
 *      offset          :   偏移量
 *  返回：
 *      返回关联的loop设备
 */
string BeGeneralLib::SetLoopDevice(string imagefile, string offset)
{
    string LoopDevice = "";

    // 检查image文件是否存在
    if (IsStringEmptyOrInvalidValue(imagefile) == true)
    {
        return LoopDevice;
    }

    string OptionString = "";
    if (IsStringEmptyOrInvalidValue(offset) == false)
    {
        OptionString = string("-o") + SEPARATOR_CHAR_SPACE + offset;
    }

    // 检查是否有未用的loop设备
    if (HasUnusedLoopDevice() == false)
    {
        // 创建循环设备节点
        CreateLoopDeviceNode();
    }

    string ShellCommand = string("losetup") + SEPARATOR_CHAR_SPACE + OptionString + SEPARATOR_CHAR_SPACE + string("--show -f") + SEPARATOR_CHAR_SPACE + imagefile;
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      释放指定的loop设备
 *  参数：
 *      loopdevice      :   loop设备名
 *  返回：
 *      无
 */
void BeGeneralLib::FreeLoopDevice(string loopdevice)
{
    FreeLoopDevice(loopdevice, false);
}

/*
 *  功能：
 *      释放指定的loop设备
 *  参数：
 *      loopdevice      :   loop设备名
 *      isdelpartition  :   是否删除分区
 *  返回：
 *      无
 */
void BeGeneralLib::FreeLoopDevice(string loopdevice, bool isdelpartition)
{
    // 检查是否为空
    if (IsStringEmptyOrInvalidValue(loopdevice) == true)
    {
        return;
    }

    // 是否删除分区
    if (isdelpartition == true)
    {
        // 删除loop设备分区
        DeleteLoopDevicePartition(loopdevice);
    }

    string ShellCommand = string("losetup -d") + SEPARATOR_CHAR_SPACE + loopdevice;
    for (int i = 0; i < DEFAULT_FREE_LOOP_DEVICE_RETRY_NUM; i++)
    {
        BeGeneralLib::ExecuteSystem(ShellCommand, true, true);

        // 检查循环设备是否绑定
        if (IsLoopDeviceBind(loopdevice) == false)
        {
            break;
        }
        sleep(3);
    }
}

/*
 *  功能：
 *      释放指定的loop设备
 *  参数：
 *      imagefile       :   image文件
 *  返回：
 *      无
 */
void BeGeneralLib::FreeFileLoopDevice(string imagefile)
{
    FreeFileLoopDevice(imagefile, false);
}

/*
 *  功能：
 *      释放指定的loop设备
 *  参数：
 *      imagefile       :   image文件
 *      isdelpartition  :   是否删除分区
 *  返回：
 *      无
 */
void BeGeneralLib::FreeFileLoopDevice(string imagefile, bool isdelpartition)
{
    vector<string> DeviceList = GetFileLoopDevices(imagefile);
    for (int i = 0; i < DeviceList.size(); i++)
    {
        FreeLoopDevice(DeviceList[i], isdelpartition);
    }
}

/*
 *  功能：
 *      获取镜像文件对应的loop设备
 *  参数：
 *      imagefile       :   image文件
 *  返回：
 *      loop设备列表
 */
vector<string> BeGeneralLib::GetFileLoopDevices(string imagefile)
{
    //    cout << "Imagefile:" << imagefile << endl;
    vector<string> LoopDeviceList;
    if (IsStringEmptyOrInvalidValue(imagefile) == true)
    {
        //        cout << "ImageFile is empty!" << endl;
        return LoopDeviceList;
    }

    string ShellCommand = string("losetup -j") + SEPARATOR_CHAR_SPACE + imagefile + SEPARATOR_CHAR_SPACE + string("2>/dev/null | gawk -F: '{print $1}'");
    LoopDeviceList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
    //    cout << "Sh:" << ShellCommand << endl;
    //    cout << "LoopD:" << LoopDeviceList.size() << endl;
    if (LoopDeviceList.empty() == true)
    {
        ShellCommand = string("losetup -a | gawk '{if($3~\"") + imagefile + string("\") print $0}' | gawk -F: '{print $1}'");
        LoopDeviceList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
    }
    //    cout << "LoopD2:" << LoopDeviceList.size() << endl;
    //    for (int i = 0; i < LoopDeviceList.size(); i++)
    //    {
    //        cout << "Loop:" << LoopDeviceList[i] << endl;
    //    }
    return LoopDeviceList;
}

/*
 *  功能：
 *      获取loop设备对应的映像文件
 *  参数：
 *      loopdevice      :   loop设备
 *  返回：
 *      映像文件
 */
string BeGeneralLib::GetLoopDeviceFile(string loopdevice)
{
    string ShellCommand = string("losetup") + SEPARATOR_CHAR_SPACE + loopdevice + string("|gawk '{if($1~\"") + loopdevice + string("\") print $3}' | gawk -F\"(\" '{print $2}' | gawk -F\")\" '{print $1}'");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      检查指定的设备是否为loop设备
 *  参数：
 *      device          :   设备名
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BeGeneralLib::IsLoopDevice(string device)
{
    // 检查设备是否为空
    if (IsStringEmptyOrInvalidValue(device) == true)
    {
        return false;
    }

    // 检查设备是否为/dev/loop开头
    string ShellCommand = string("echo") + SEPARATOR_CHAR_SPACE + device + string("|grep '^/dev/loop'");
    return (IsStringEmpty(ReadShellReturnValue(ShellCommand)) == false);
}

/*
 *  功能：
 *      检查loop设备是否绑定
 *  参数：
 *      loopdevice      :   loop设备
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BeGeneralLib::IsLoopDeviceBind(string loopdevice)
{
    if (IsStringEmptyOrInvalidValue(loopdevice) == true)
    {
        return false;
    }

    // 检查loop设备是否绑定
    string ShellCommand = string("losetup -a | grep ^") + loopdevice;
    return (IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false);
}

/*
 *  功能：
 *      是否有未用的循环设备
 *  参数：
 *      无
 *  返回：
 *      如果有返回true，否则返回false
 */
bool BeGeneralLib::HasUnusedLoopDevice()
{
    return (IsStringEmpty(GetFreeLoopDevice()) == false);
}

/*
 *  功能：
 *      绑定loop设备
 *  参数：
 *      imagefile       :   image文件
 *      isexist         :   设备是否存在的标志
 *  返回：
 *      如果绑定成功返回设备名，否则返回空
 */
string BeGeneralLib::BindLoopDevice(string imagefile, bool &isexist)
{
    string DeviceName = DEFAULT_EMPTY_STRING;
    isexist = false;
    vector<string> DeviceList = BeGeneralLib::GetFileLoopDevices(imagefile);
    if (DeviceList.size() != 0)
    {
        DeviceName = DeviceList[0];
        isexist = true;
    }
    else
    {
        DeviceName = BeGeneralLib::SetLoopDevice(imagefile);
    }
    return DeviceName;
}

/*
 *  功能：
 *      删除loop设备分区
 *  参数：
 *      loopdevice      :   loop设备
 *  返回：
 *      无
 */
void BeGeneralLib::DeleteLoopDevicePartition(string loopdevice)
{
    // 获取虚拟磁盘分区
    vector<string> PartitionList = BeGeneralLib::GetVirtualDevicePartition(loopdevice);
    for (int i = 0; i < PartitionList.size(); i++)
    {
        string Device = PartitionList[i];
        BeGeneralLib::UmountDevice(Device, "", true);

        // 卸载链接设备
        BeGeneralLib::UmountDevice(GetLinkFileFullPath(Device), "", true);
    }
    DeleteVirtualDevicePartition(loopdevice);

    if(PartitionList.size() == 0) BeGeneralLib::UmountDevice(loopdevice, "", true);

    // 获取虚拟设备映射
    vector<string> DeviceMapperList = BeGeneralLib::GetVirtualDeviceMapper(loopdevice);
    for (int i = 0; i < DeviceMapperList.size(); i++)
    {
        string Device = DeviceMapperList[i];
        DeleteVirtualDeviceMapper(Device);
    }
}

/*
 *  功能：
 *      创建loop设备节点
 *  参数：
 *      无
 *  返回：
 *      无
 */
void BeGeneralLib::CreateLoopDeviceNode()
{
    string ShellCommand = string("MAKEDEV -a") + SEPARATOR_CHAR_SPACE + DEFAULT_LOOP_DEVICE_NAME;
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      将loop设备挂载到指定的挂载点
 *  参数：
 *      device          :   loop设备
 *      mountpoint      :   挂载点
 *      filesystem      :   文件系统
 *      partitionnum    :   分区号
 *  返回：
 *      挂载成功返回true，否则返回false
 */
bool BeGeneralLib::MountLoopDevice(string device, string mountpoint, BaseFileSystem filesystem, int partitionnum)
{
    string Device = device;
    string MountPoint = mountpoint;
    BaseFileSystem FileSystem = filesystem;
    int PartitionNum = partitionnum;
    bool MountFlag = false;

    // 检查输入有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(Device) == true
            || BeGeneralLib::IsStringEmptyOrInvalidValue(MountPoint) == true)
    {
    	BesLog::DebugPrint(string("mount loop device : input empty :")+Device+string(",")+MountPoint, BesLog::LOG_MESSAGE_TYPE_ERROR, false);
        return false;
    }

    // 如果分区号不为零
    if (PartitionNum != 0)
    {
    	BesLog::DebugPrint(string("is system partition"), BesLog::LOG_MESSAGE_TYPE_VERBOSE, false);
        // 获取虚拟磁盘分区
        vector<string> PartitionList;

        // 增加虚拟磁盘分区
        BeGeneralLib::AddVirtualDevicePartition(Device, PartitionList);
        if (PartitionList.size() != 0)
        {
            Device = PartitionList[0];
            BesLog::DebugPrint(string("virtual partition:")+Device, BesLog::LOG_MESSAGE_TYPE_VERBOSE, false);
        }
    }

    // 挂载文件系统
    MountFlag = BeGeneralLib::MountDevice(Device, MountPoint, FileSystem);
    if (MountFlag == false && PartitionNum != 0)
    {
        MountFlag = BeGeneralLib::MountDevice(Device, MountPoint, FileSystem, PartitionNum);
    }
    if (MountFlag == false) BesLog::DebugPrint(string("mount loop device failed"), BesLog::LOG_MESSAGE_TYPE_ERROR, false);

    return MountFlag;
}

/*
 *  功能：
 *      增加虚拟磁盘分区
 *  参数：
 *      device          :   设备名
 *  返回：
 *      无
 */
void BeGeneralLib::AddVirtualDevicePartition(string device)
{
    string ShellCommand = string("kpartx -av") + SEPARATOR_CHAR_SPACE + device;
    ExecuteSystem(ShellCommand, true, false);
    sleep(5);										//调试用
}

/*
 *  功能：
 *      增加虚拟磁盘分区
 *  参数：
 *      device          :   设备名
 *      partlist        :   磁盘分区列表
 *  返回：
 *      无
 */
void BeGeneralLib::AddVirtualDevicePartition(string device, vector<string>& partlist)
{
    // 增加虚拟设备分区
    AddVirtualDevicePartition(device);

    // 获取虚拟设备分区列表
    partlist = GetVirtualDevicePartition(device, true);

    // 如果分区列表不为空
    if (partlist.size() != 0)
    {
        return;
    }

    // 获取
    vector<string> VirtualDeviceMapperList = BeGeneralLib::GetVirtualDeviceMapper(device);
    for (int i = 0; i < VirtualDeviceMapperList.size(); i++)
    {
        BeGeneralLib::DeleteVirtualDeviceMapper(VirtualDeviceMapperList[i]);
    }

    // 增加虚拟设备分区
    AddVirtualDevicePartition(device);

    sleep(3);

    // 获取虚拟设备分区列表
    partlist = GetVirtualDevicePartition(device, true);
}

/*
 *  功能：
 *      删除虚拟磁盘分区
 *  参数：
 *      device          :   设备名
 *  返回：
 *      无
 */
void BeGeneralLib::DeleteVirtualDevicePartition(string device)
{
    for (int i = 0; i < 3; i++)
    {
        string ShellCommand = string("kpartx -dv") + SEPARATOR_CHAR_SPACE + device;
        ExecuteSystem(ShellCommand, true, false);
        usleep(500000);
    }
}

/*
 *  功能：
 *      获取虚拟磁盘分区
 *  参数：
 *      device          :   设备名
 *      withvirtualpath :   是否含有虚拟路径
 *  返回：
 *      虚拟磁盘分区列表
 */
vector<string> BeGeneralLib::GetVirtualDevicePartition(string device, bool withvirtualpath)
{
    string OptionString = "";
    if (withvirtualpath == true)
    {
        OptionString = DEFAULT_VIRTUAL_MAPPER_PATH;
    }

    // 查询设备分区表的分区信息（尚未真正增加）
    string ShellCommand = string("kpartx -lv") + SEPARATOR_CHAR_SPACE + device + SEPARATOR_CHAR_SPACE + string("| gawk '{if($5==\"") + device + string("\") print \"") + OptionString + string("\"$1}'");
    vector<string> PartitionList = ReadShellReturnValueAll(ShellCommand);

//	cout<<"Cmd:"<<ShellCommand<<" "<<PartitionList.size()<<endl;	//调试用

    if (withvirtualpath == false)
    {
        return PartitionList;
    }
    vector<string> ValidPartionList;
    for (int i = 0; i < PartitionList.size(); i++)
    {
        string PartitionName = PartitionList[i];
//		cout<<i<<":"<<PartitionName<<endl;			//调试用
//		ExecuteSystem(string("ls ")+PartitionName, false, false);		//调试用

        if (BeGeneralLib::IsFileExist(PartitionName) == true)
        {
            ValidPartionList.push_back(PartitionName);
//			cout<<PartitionName<<" exist, push back"<<endl;		//调试用
        }
    }
    return ValidPartionList;

}

/*
 *  功能：
 *      删除虚拟设备映射
 *  参数：
 *      device          :   设备名
 *  返回：
 *      无
 */
void BeGeneralLib::DeleteVirtualDeviceMapper(string device)
{
    // 检查设备是否为空
    if (IsStringEmptyOrInvalidValue(device) == true)
    {
        return;
    }

    // 删除虚拟设备映射
    string ShellCommand = string("dmsetup remove") + SEPARATOR_CHAR_SPACE + device;
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      获取指定设备的虚拟设备映射
 *  参数：
 *      device          :   设备名
 *      withvirtualpath :   是否含有虚拟路径
 *  返回：
 *      虚拟设备映射列表
 */
vector<string> BeGeneralLib::GetVirtualDeviceMapper(string device, bool withvirtualpath)
{
    string OptionString = "";
    if (withvirtualpath == true)
    {
        OptionString = DEFAULT_VIRTUAL_MAPPER_PATH;
    }

    int LastIndex = device.find_last_of(LINUX_PATH_SEPCHAR);
    if (LastIndex != -1)
    {
        device = device.substr(LastIndex + 1, device.size() - LastIndex - 1);
    }

    string ShellCommand = string("ls -1") + SEPARATOR_CHAR_SPACE + DEFAULT_VIRTUAL_MAPPER_PATH + SEPARATOR_CHAR_SPACE + string("| gawk '{if($1 ~ /^") + device + string("/) print \"") + OptionString + string("\"$1}'");

    return ReadShellReturnValueAll(ShellCommand);
}

/*
 *  功能：
 *     在给定的路径后增加路径分割符
 *  参数：
 *      path            :   路径
 *  返回：
 *      返回处理后的路径
 */
string BeGeneralLib::AddPathSeparator(string path)
{
    // 检查是否为空
    if (IsStringEmpty(path) == true)
    {
        return path;
    }

    // 如果路径最后一个字符不是路径分割符，则添加
    if (path.find_last_of(LINUX_PATH_SEPCHAR) != path.size() - 1)
    {
        path = path + LINUX_PATH_SEPCHAR;
    }

    return path;
}

/*
 *  功能：
 *     在给定的路径后删除路径分割符
 *  参数：
 *      path            :   路径
 *  返回：
 *      返回处理后的路径
 */
string BeGeneralLib::RemovePathSeparator(string path)
{
    // 检查是否为空
    if (IsStringEmpty(path) == true)
    {
        return path;
    }

    // 是否为根
    if (path.size() == 1 && path.c_str()[0] == LINUX_PATH_SEPCHAR)
    {
        return path;
    }

    // 去掉路径右端的路径分割符
    return StringRTrim(path, LINUX_PATH_SEPCHAR);
}

/*
 *  功能：
 *     获取磁盘使用百分比
 *  参数：
 *      disk             :   磁盘名称
 *  返回：
 *      如果磁盘存在返回实际使用百分比，否则返回-1
 */
int BeGeneralLib::GetDiskUsedPercentage(string disk)
{
    // 如果磁盘不存在或磁盘为挂载，则返回-1
    if (BeGeneralLib::IsDeviceExist(disk) == false || BeGeneralLib::IsDeviceMounted(disk) == false)
    {
        return -1;
    }

    // 读取磁盘使用率
    string ShellCommand = string("df -h | gawk '{if($1==\"") + disk + string("\") print $5}' | gawk -F% '{print $1}')");
    return StringToInt(BeGeneralLib::ReadShellReturnValue(ShellCommand));
}

/*
 *  功能：
 *     按照指定的单位获取磁盘可用空间
 *  参数：
 *      disk            :   磁盘名称
 *      unit            :   单位
 *  返回：
 *      磁盘可用空间
 */
int BeGeneralLib::GetDiskAvailableSize(string disk, BeGeneralLib::DiskSizeUnit unit)
{
    // 检查磁盘是否为空
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(disk) == true)
    {
        return 0;
    }

    string UnitString = "";
    switch (unit)
    {
        case BeGeneralLib::DISK_SIZE_UNIT_B:
        case BeGeneralLib::DISK_SIZE_UNIT_K:
        {
            UnitString = string("K");
            break;
        }
        case BeGeneralLib::DISK_SIZE_UNIT_M:
        {
            UnitString = string("M");
            break;
        }
        case BeGeneralLib::DISK_SIZE_UNIT_G:
        {
            UnitString = string("G");
            break;
        }
        case BeGeneralLib::DISK_SIZE_UNIT_T:
        {
            UnitString = string("T");
            break;
        }
        case BeGeneralLib::DISK_SIZE_UNIT_P:
        {
            UnitString = string("P");
            break;
        }
        case BeGeneralLib::DISK_SIZE_UNIT_E:
        {
            UnitString = string("E");
            break;
        }
        case BeGeneralLib::DISK_SIZE_UNIT_Z:
        {
            UnitString = string("Z");
            break;
        }
        case BeGeneralLib::DISK_SIZE_UNIT_Y:
        {
            UnitString = string("Y");
            break;
        }
    }

    string ShellCommand = string("df --block-size=") + UnitString + SEPARATOR_CHAR_SPACE + disk + string("| gawk '{if($1==\"") + disk + string("\") print int($4)}'");
    string ReturnValue = BeGeneralLib::ReadShellReturnValue(ShellCommand);
    if (IsStringEmpty(ReturnValue) == true)
    {
        string MountPoint = disk;
        if (disk != DEFAULT_LINUX_SYSTEM_FLAG)
        {
            MountPoint = RemovePathSeparator(MountPoint);
        }

        ShellCommand = string("df --block-size=") + UnitString + SEPARATOR_CHAR_SPACE + MountPoint + string("| gawk '{if($6==\"") + MountPoint + string("\") print int($4)}'");
        ReturnValue = BeGeneralLib::ReadShellReturnValue(ShellCommand);
    }
    BesLog::DebugPrint("Disk Available:"+ReturnValue, BesLog::LOG_MESSAGE_TYPE_INFO, false);
    return BeGeneralLib::StringToInt(ReturnValue);
}

/*
 *  功能：
 *     获取系统盘名称
 *  参数：
 *      无
 *  返回：
 *      系统盘名称
 */
string BeGeneralLib::GetSystemDiskName()
{
    // 首先检查/boot分区
    string ShellCommand = string("df -h | gawk '$6 == \"/boot\"  {print $1}'");
    string DiskName = ReadShellReturnValue(ShellCommand);

    // 如果不存在，则检查根分区
    if (IsStringEmpty(DiskName) == true)
    {
        ShellCommand = string("df -h | gawk '$6 == \"/\"  {print $1}'");
        DiskName = ReadShellReturnValue(ShellCommand);
    }

    return DiskName;
}

/*
 *  功能：
 *     获取系内核版本
 *  参数：
 *      无
 *  返回：
 *      内核版本
 */
string BeGeneralLib::GetKernelVersion()
{
    string ShellCommand = string("uname -r");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     获取系内核版本
 *  参数：
 *      linuxos         :   操作系统类型
 *      withrebuild     :   是否将内核版本重构，以点（.）为分割符
 *  返回：
 *      内核版本
 */
string BeGeneralLib::GetKernelVersion(BeGeneralLib::LinuxOSType linuxos, bool withrebuild)
{
    if (withrebuild == false)
    {
        return ReadShellReturnValue("uname -r");
    }

    string ShellCommand = "";
    switch (linuxos)
    {
        case BeGeneralLib::LINUX_OS_TYPE_RHEL:
        {
            ShellCommand = string("uname -r | gawk -F[.] '{print $1\".\"$2\".\"$3}' | gawk -F[-] '{print $1\".\"$2}'");
            break;
        }
        case BeGeneralLib::LINUX_OS_TYPE_SUSE:
        {
            ShellCommand = string("uname -r | gawk -F[-] '{print $1\".\"$2}'");
            break;
        }
        default:
        {
            ShellCommand = string("uname -r | gawk -F[.] '{print $1\".\"$2\".\"$3}' | gawk -F[-] '{print $1\".\"$2}'");
            break;
        }
    }
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     获取系内核版本主号
 *  参数：
 *      无
 *  返回：
 *      内核版本主号
 */
string BeGeneralLib::GetKernelVersionMajor()
{
    string ShellCommand = string("uname -r | gawk -F[.] '{print $2}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     获取系内核版本次号
 *  参数：
 *      无
 *  返回：
 *      内核版本次号
 */
string BeGeneralLib::GetKernelVersionMinor()
{

    string ShellCommand = string("uname -r | gawk -F[.] '{print $2}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     获取系内核版本补丁版本
 *  参数：
 *      无
 *  返回：
 *      内核版本补丁版本
 */
string BeGeneralLib::GetKernelVersionPatch()
{
    string ShellCommand = string("uname -r | gawk -F[-] '{print $1}' | gawk -F[.] '{print $3}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     获取系内核版本打包信息
 *  参数：
 *      无
 *  返回：
 *      内核版本打包信息
 */
string BeGeneralLib::GetKernelVersionBuild()
{
    string ShellCommand = string("uname -r | gawk -F[-] '{print $2}' | gawk -F[.] '{print $1}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     获取系内核版本描述
 *  参数：
 *      无
 *  返回：
 *      内核版本主号
 */
string BeGeneralLib::GetKernelVersionDescription()
{
    string ShellCommand = string("uname -r | gawk -F[-] '{print $2}' | gawk -F[.] '{print $2}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     获取系内核版本归档信息
 *  参数：
 *      无
 *  返回：
 *      内核版本归档信息
 */
string BeGeneralLib::GetKernelVersionArchive()
{
    string ShellCommand = string("uname -r | gawk -F[-] '{print $2}' | gawk -F[.] '{print $3}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     判断被保护主机内核是否是xxxx-xen内核
 *  参数：
 *      xenversion      :   返回xen的版本
 *  返回：
 *      如果是，则返回true，并设置xenversion，否则返回false
 */
bool BeGeneralLib::IsXenKernel(string& xenversion)
{
    bool Flag = false;

    xenversion = "";
    string KernelVersion = GetKernelVersion();

    if (KernelVersion.substr(KernelVersion.size() - 3, 3).compare("xen") == 0)
    {
        xenversion = KernelVersion.substr(0, KernelVersion.size() - 3);
        Flag = true;
    }
    return Flag;
}

/*
 *  功能：
 *      根据ip地址获取对应网卡名称
 *  参数：
 *      ipaddr          :   ip地址
 *  返回：
 *      网卡名称
 */
string BeGeneralLib::GetNicName(string ipaddr)
{
    // 获取网卡名
    string ShellCommand = string("ip addr list | grep \"inet\" | grep \"" + ipaddr + "\" | gawk '{print $7}'");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      根据ip地址获取对应MAC地址
 *  参数：
 *      ipaddr          :   ip地址
 *  返回：
 *      MAC地址
 */
string BeGeneralLib::GetMacAddress(string ipaddr)
{
    string ShellCommand = string("ifconfig $(ip addr list | grep \"inet\" | grep \"") + ipaddr + string("\" | gawk '{print $7}')  | grep \"HWaddr\" | gawk '{print $5}'");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     获取initiator名称
 *  参数：
 *      无
 *  返回：
 *      initiator名称
 */
string BeGeneralLib::GetInitiatorName()
{
    string ConfigFileName = DEFAULT_INITIATOR_CONF;
    if (IsFileExist(ConfigFileName) == false)
    {
        ConfigFileName = DEFAULT_INITIATOR_CONF2;
    }
    string ShellCommand = string("cat") + SEPARATOR_CHAR_SPACE + ConfigFileName + string(" | grep ^") + DEFAULT_INITIATOR_NAME + string("| gawk -F= '{print $2}'");
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *     比较内核版本大小
 *  参数：
 *      kernelver1      :   内核版本1
 *      kernelver2      :   内核版本2
 *  返回：
 *      如果kernelver1 > kernelver2返回true，否则返回false
 */
bool BeGeneralLib::GreaterThanKernelVersion(string kernelver1, string kernelver2)
{
    vector <string> KernelVersionList1 = BeGeneralLib::StringSplit(kernelver1, '.', BeGeneralLib::WITH_EMPTY_ITEM);
    vector <string> KernelVersionList2 = BeGeneralLib::StringSplit(kernelver2, '.', BeGeneralLib::WITH_EMPTY_ITEM);

    int Length = KernelVersionList1.size() < KernelVersionList2.size() ? KernelVersionList1.size() : KernelVersionList2.size();

    for (int i = 0; i < Length; i++)
    {
        if (BeGeneralLib::StringToInt(KernelVersionList1[i]) < BeGeneralLib::StringToInt(KernelVersionList2[i]))
        {
            return false;
        }
    }
    return true;
}

/*
 *  功能：
 *      格式化磁盘
 *  参数：
 *      device          :   磁盘设备
 *      filesystem      :   文件系统
 *  返回：
 *      格式化成功返回true，否则返回false
 */
bool BeGeneralLib::FormatDisk(string device, string filesystem)
{
    return FormatDisk(device, filesystem, "");
}

/*
 *  功能：
 *      格式化磁盘
 *  参数：
 *      device          :   磁盘设备
 *      filesystem      :   文件系统
 *      label           :   卷标
 *  返回：
 *      格式化成功返回true，否则返回false
 */
bool BeGeneralLib::FormatDisk(string device, string filesystem, string label)
{
    return FormatDisk(device, filesystem, label, false);
}

/*
 *  功能：
 *      格式化磁盘
 *  参数：
 *      device          :   磁盘设备
 *      filesystem      :   文件系统
 *      label           :   卷标
 *      isforce         :   是否强制格式化
 *  返回：
 *      格式化成功返回true，否则返回false
 */
bool BeGeneralLib::FormatDisk(string device, string filesystem, string label, bool isforce)
{
    // 检查设备是否存在，或文件系统是否为空
    if (IsDeviceExist(device) == false || IsStringEmpty(filesystem) == true)
    {
        return false;
    }

    string ShellCommand = string("mkfs") + SEPARATOR_CHAR_SPACE + string("-t") + SEPARATOR_CHAR_SPACE + filesystem;
    string OptionString = "";
    if (filesystem == DEFAULT_FILESYSTEM_TYPE_EXT3 || filesystem == DEFAULT_FILESYSTEM_TYPE_EXT4)
    {
        if (isforce == true)
        {
            OptionString = OptionString + SEPARATOR_CHAR_SPACE + string("-F");
        }

        if (IsStringEmpty(label) == false)
        {
            OptionString = OptionString + SEPARATOR_CHAR_SPACE + string("-L") + SEPARATOR_CHAR_SPACE + label;
        }
    }
    else if (filesystem == DEFAULT_FILESYSTEM_TYPE_REISERFS)
    {
        if (isforce == true)
        {
            OptionString = OptionString + SEPARATOR_CHAR_SPACE + string("-f");
        }

        if (IsStringEmpty(label) == false)
        {
            OptionString = OptionString + SEPARATOR_CHAR_SPACE + string("-l") + SEPARATOR_CHAR_SPACE + label;
        }
    }

    // 设置格式化命令
    ShellCommand = ShellCommand + SEPARATOR_CHAR_SPACE + OptionString + SEPARATOR_CHAR_SPACE + device;
    ExecuteSystem(ShellCommand);
    return true;
}

/*
 *  功能：
 *      将设备格式化为reiserfs
 *  参数
 *      device          :   设备
 *  返回：
 *      如果成功返回true，否则返回false
 */
bool BeGeneralLib::FormatReiserfs(string device)
{
    // 检查输入有效性
    if (BeGeneralLib::IsFileExist(device) == false)
    {
        return false;
    }

    // 生成格式化脚本文件
    string ScriptFile = BeGeneralLib::AddPathSeparator(BeGeneralLib::GetExecutePath()) + DEFAULT_MKFS_REISERFS_SCRIPT;
    FILE *fp = fopen(ScriptFile.c_str(), "w+");
    if (fp == NULL)
    {
        return false;
    }
    fputs("#!/bin/bash\n", fp);
    fputs("CMD=\"mkreiserfs\"\n", fp);
    fputs("$CMD -q $1 << EOF\n", fp);
    fputs("y\n", fp);
    fputs("EOF\n", fp);
    fclose(fp);

    // 修改文件执行权限
    string ShellCommand = string("chmod 755") + SEPARATOR_CHAR_SPACE + ScriptFile;
    BeGeneralLib::ExecuteSystem(ShellCommand);

    // 检查脚本文件是否存在
    if (BeGeneralLib::IsFileExist(ScriptFile) == false)
    {
        return false;
    }

    // 格式化
    ShellCommand = string(ScriptFile) + SEPARATOR_CHAR_SPACE + device;
    BeGeneralLib::ExecuteSystem(ShellCommand);

    sleep(3);

    return true;
}

/*
 *  功能：
 *      清零磁盘头
 *  参数：
 *      disk            :   磁盘设备
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::ZeroDiskHeader(string disk)
{
    return ZeroDiskHeader(disk, 256);
}

/*
 *  功能：
 *      清零磁盘头
 *  参数：
 *      disk            :   磁盘设备
 *      sectors         :   扇区数
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::ZeroDiskHeader(string disk, long sectors)
{
    // 检查输入有效性
    if (IsStringEmpty(disk) == true
            || sectors <= 0)
    {
        return false;
    }

    // 清零磁盘头
    string ShellCommand = string("dd if=/dev/zero bs=512 count=") + Number2String(sectors) + SEPARATOR_CHAR_SPACE + string("of=") + disk + SEPARATOR_CHAR_SPACE + string("conv=notruc");
    ExecuteSystem(ShellCommand);

    return true;
}

/*
 *  功能：
 *      创建空文件
 *  参数：
 *      filename        :   文件名
 *  返回：
 *      创建成功返回true，否则返回false
 */
bool BeGeneralLib::CreateEmptyFile(string filename)
{
    return CreateEmptyFile(filename, false);
}

/*
 *  功能：
 *      创建空文件
 *  参数：
 *      filename        :   文件名
 *      isforce         :   是否强制创建
 *  返回：
 *      创建成功返回true，否则返回false
 */
bool BeGeneralLib::CreateEmptyFile(string filename, bool isforce)
{
    if (isforce == false && IsFileExist(filename) == true)
    {
        return false;
    }

    // 如果强制创建，并且文件存在，则清空
    if (isforce == true && IsFileExist(filename) == true)
    {
        WriteFile(filename, DEFAULT_EMPTY_STRING, BeGeneralLib::WRITE_FILE_MODE_CLEAR);
        return true;
    }

    // 否则创建空文件
    string ShellCommand = string("touch") + SEPARATOR_CHAR_SPACE + filename;
    ExecuteSystem(ShellCommand);
    return IsFileExist(filename);
}

/*
 *  功能：
 *      追加数据到现有文件
 *  参数：
 *      filename        :   文件名
 *      data            :   数据内容
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::AppendToFile(string filename, string data)
{
    return WriteFile(filename, data, BeGeneralLib::WRITE_FILE_MODE_APPEND);
}

/*
 *  功能：
 *      将数据到现有文件
 *  参数：
 *      filename        :   文件名
 *      data            :   数据内容
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::WriteFile(string filename, string data, BeGeneralLib::WriteFileMode mode)
{
    // 检查输入有效性
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(filename) == true)
    {
        return false;
    }

    string ShellCommand = DEFAULT_EMPTY_STRING;
    switch (mode)
    {
        case BeGeneralLib::WRITE_FILE_MODE_CLEAR:
        {
            ShellCommand = string(">") + filename;
            break;
        }
        case BeGeneralLib::WRITE_FILE_MODE_APPEND:
        {
            ShellCommand = string("echo \"") + data + string("\" >> ") + filename;
            break;
        }
        case BeGeneralLib::WRITE_FILE_MODE_CREATE:
        {
            ShellCommand = string("echo \"") + data + string("\" > ") + filename;
            break;
        }
    }

    // 执行命令
    BeGeneralLib::ExecuteSystem(ShellCommand, false, true);
    return (IsFileExist(filename) == true);
}

/*
 *  功能：
 *      从文件中读取数据
 *  参数：
 *      filename        :   文件名
 *      data            :   数据内容
 *  返回：
 *      成功返回true，否则返回false
 */
string BeGeneralLib::ReadFile(string filename)
{
    string FileData = DEFAULT_EMPTY_STRING;

    if (IsFileExist(filename) == false)
    {
        return FileData;
    }

    FILE *fp;
    if ((fp = fopen(filename.c_str(), "r")) == NULL)
    {
        return FileData;
    }
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    while (!feof(fp))
    {
        bzero(Buf, sizeof (Buf));
        fgets(Buf, sizeof (Buf), fp);
        FileData = FileData + (string(Buf));
    }
    fclose(fp);
    return FileData;
}

/*
 *  功能：
 *      获取启动设备名
 *  参数：
 *      withprefix      ：  是否含有前缀
 *  返回：
 *      如果成功返回设备名，否则返回空
 */
string BeGeneralLib::GetBootDevice(bool withprefix)
{
    // 使用boot分区进行启动盘判定
    string ShellCommand = string("df -h | gawk '{if($6==\"/boot\") print $1}' | gawk -F/ '{print substr($3, 1, length($3) - 1)}'");
    string BootDiskName = BeGeneralLib::ReadShellReturnValue(ShellCommand);

    // 如果磁盘为空，则使用/分区进行启动盘判定
    if (IsStringEmpty(BootDiskName) == true)
    {
        ShellCommand = string("df -h | gawk '{if($6==\"/\") print $1}' | gawk -F/ '{print substr($3, 1, length($3) - 1)}'");
        BootDiskName = BeGeneralLib::ReadShellReturnValue(ShellCommand);
    }

    // 如果磁盘名为空，则返回空
    if (IsStringEmpty(BootDiskName) == true)
    {
        return BootDiskName;
    }

    // 如果带前缀，则在磁盘前增加
    if (withprefix == true)
    {
        BootDiskName = string("/dev/") + BootDiskName;
    }
    return BootDiskName;
}

/*
 *  功能：
 *      刷新光纤scsi
 *  参数：
 *      wwn             :   如果为空，则刷新所有，否则刷新指定wwn的光纤scsi
 *  返回：
 *      如果刷新成功返回true，否则返回false
 */
bool BeGeneralLib::RefreshFCScsi(string wwn)
{
    string ShellCommand = string("ls") + SEPARATOR_CHAR_SPACE + DEFAULT_FC_HOST_PATH + string(" | grep ^host | gawk '{print substr($1, 5, length($1) - 4)}'");
    vector<string> HostNumList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
    if (HostNumList.empty() == true)
    {
        return false;
    }

    bool Flag = false;
    for (int i = 0; i < HostNumList.size(); i++)
    {
        if ((IsStringEmpty(wwn) == false) && (wwn != GetFCHostWWN(HostNumList[i], true)) && (wwn != GetFCHostWWN(HostNumList[i], false)))
        {
            continue;
        }
        ShellCommand = string("echo \"- - -\" > /sys/class/scsi_host/host") + HostNumList[i] + LINUX_PATH_SEPCHAR + string("scan");
        BeGeneralLib::ExecuteSystem(ShellCommand);
        Flag = true;
    }

    return false;
}

/*
 *  功能：
 *      获取指定编号的光纤的wwn
 *  参数：
 *      hostnum         :   编号
 *      withsepchar     :   是否带分割符
 *  返回：
 *      如果成功返回wwn，否则返回空
 */
string BeGeneralLib::GetFCHostWWN(string hostnum, bool withsepchar)
{
    string WWNString = "";

    // 检查fc路径是否存在
    string FCHostPath = DEFAULT_FC_HOST_PATH;
    if (BeGeneralLib::IsFileExist(FCHostPath) == false)
    {
        return WWNString;
    }

    string ShellCommand = string("ls") + SEPARATOR_CHAR_SPACE + FCHostPath + string(" | grep ^host | gawk '{print substr($1, 5, length($1) - 4)}'");
    vector<string> HostNumList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
    if (HostNumList.empty() == true)
    {
        return WWNString;
    }

    for (int i = 0; i < HostNumList.size(); i++)
    {
        if (hostnum == HostNumList[i])
        {
            string FileName = DEFAULT_FC_HOST_PATH + string("host") + HostNumList[i] + LINUX_PATH_SEPCHAR + string("port_name");
            ShellCommand = string("cat") + SEPARATOR_CHAR_SPACE + FileName + string("| gawk -F[x] '{print $2}'");
            WWNString = BeGeneralLib::ReadShellReturnValue(ShellCommand);
            if (withsepchar == true)
            {
                string TempWWNString = WWNString.substr(0, 2);
                for (int j = 2; j < WWNString.size(); j = j + 2)
                {
                    TempWWNString = TempWWNString + SEPARATOR_CHAR_COLON + WWNString.substr(j, 2);
                }
                WWNString = TempWWNString;
            }

            break;
        }
    }
    return WWNString;
}

/*
 *  功能：
 *      列举光纤卡wwn
 *  参数：
 *      无
 *  返回：
 *      光纤卡wwn列表
 */
vector<string> BeGeneralLib::ListFCWWN()
{
	vector<string> ValueList = BeGeneralLib::ReadShellReturnValueAll("sed -e 's/[0-9A-Fa-f]\\{2\\}/&:/g' -e 's/:$//' /sys/class/fc_host/host*/port_name | gawk -Fx '{print $2}' | sort");
	if(ValueList.empty()) ValueList = BeGeneralLib::ReadShellReturnValueAll("ls -1 /sys/kernel/scst_tgt/targets/* | grep ':[0-9A-Fa-f]'");
	return ValueList;
}

/*
 *  功能：
 *      删除块设备
 *  参数：
 *      devicename      :   块设备名
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::DeleteBlockDevice(string devicename)
{
    string ShellCommand = string("echo 1 > /sys/block/") + devicename + LINUX_PATH_SEPCHAR + string("device/delete");
    BeGeneralLib::ExecuteSystem(ShellCommand);

    return true;
}

/*
 *  功能：
 *      加载驱动
 *  参数：
 *      driverpath      :   驱动路径
 *      dirvername      :   驱动名称
 *      modulename      :   加载后的模块名称
 *  返回：
 *      如果启动返回true，否则返回false
 */
bool BeGeneralLib::LoadOneDriver(string driverpath, string drivername, string modulename)
{
    // 去掉结尾的ko后缀
    string ShellCommand = string("echo") + SEPARATOR_CHAR_SPACE + modulename + string("| gawk -F. '{print $1}'");
    string ModuleName = BeGeneralLib::ReadShellReturnValue(ShellCommand);

    // 去掉结尾的ko后缀
    ShellCommand = string("echo") + SEPARATOR_CHAR_SPACE + drivername + string("| gawk -F. '{print $1}'");
    string DriverName = BeGeneralLib::ReadShellReturnValue(ShellCommand);

    // 检查驱动是否加载
    if (IsDriverLoaded(ModuleName) == true)
    {
        return true;
    }

    int Counter = 0;
    while (Counter++ < MAX_LOAD_DRIVER_RETRY_NUM)
    {
        // 加载驱动
        ShellCommand = string("modprobe") + SEPARATOR_CHAR_SPACE + DriverName;
        BeGeneralLib::ExecuteSystem(ShellCommand);

        sleep(1);

        // 检查驱动是否加载
        if (IsDriverLoaded(ModuleName) == true)
        {
            return true;
        }
        else
        {
            // 检查路径是否存在
            if (BeGeneralLib::IsFileExist(driverpath) == true)
            {
                // 通过文件加载驱动
                ShellCommand = string("insmod") + SEPARATOR_CHAR_SPACE + driverpath + LINUX_PATH_SEPCHAR + DriverName + DEFAULT_DRIVER_EXTENSION;
                BeGeneralLib::ExecuteSystem(ShellCommand);

                sleep(1);

                // 检查驱动是否加载
                if (IsDriverLoaded(ModuleName) == true)
                {
                    return true;
                }
            }
        }
    }

    return IsDriverLoaded(ModuleName);
}

/*
 *  功能：
 *      删除驱动
 *  参数：
 *      modulename      :   模块名称
 *  返回：
 *      如果删除返回true，否则返回false
 */
bool BeGeneralLib::RemoveDriver(string modulename)
{

}

/*
 *  功能：
 *      根据驱动名获取驱动全路径
 *  参数：
 *      drivername      :   驱动名
 *  返回：
 *      成功返回驱动全路径，否则返回空
 */
string BeGeneralLib::GetDriverFullPath(string drivername)
{
    return GetDriverFullPath(drivername, GetKernelVersion());
}

/*
 *  功能：
 *      根据驱动名在指定的内核下获取驱动全路径
 *  参数：
 *      drivername      :   驱动名
 *      kernelversion   :   内核版本
 *  返回：
 *      成功返回驱动全路径，否则返回空
 */
string BeGeneralLib::GetDriverFullPath(string drivername, string kernelversion)
{
    // 检查驱动名是否为空
    if (IsStringEmptyOrInvalidValue(drivername) == true)
    {
        return "";
    }

    // 检查驱动名是否以驱动扩展名结尾,则增加
    if (drivername.find_last_of(DEFAULT_DRIVER_EXTENSION) == -1)
    {
        drivername = drivername + DEFAULT_DRIVER_EXTENSION;
    }

    // 获取内核目录下的驱动全路径
    string ShellCommand = string("find /lib/modules/") + kernelversion + string("/ -name") + SEPARATOR_CHAR_SPACE + drivername;
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      检查驱动是否加载
 *  参数：
 *      无
 *  返回：
 *      如果加载返回true，否则返回false
 */
bool BeGeneralLib::IsDriverLoaded(string drivername)
{
    // 去掉结尾的ko后缀
    string ShellCommand = string("echo") + SEPARATOR_CHAR_SPACE + drivername + string("| gawk -F. '{print $1}'");
    string DriverName = BeGeneralLib::ReadShellReturnValue(ShellCommand);

    //  检查驱动是否存在
    ShellCommand = string("lsmod | gawk '{if($1==\"") + DriverName + string("\") print $1}'");
    return (BeGeneralLib::ReadShellReturnValue(ShellCommand) == DriverName);
}

/*
 *  功能：
 *      发送udp数据
 *  参数
 *      ip              :   ip地址
 *      port            :   端口
 *      data            :   数据
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::SendUdpData(string ip, string port, string data)
{
    return SendUdpData(ip, BeGeneralLib::StringToInt(port), data);
}

/*
 *  功能：
 *      发送udp数据
 *  参数
 *      ip              :   ip地址
 *      port            :   端口
 *      data            :   数据
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::SendUdpData(string ip, int port, string data)
{
    // 创建UDP的sock
    int Sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (Sock < 0)
    {
        return false;
    }

    // 初始化套接字地址
    struct sockaddr_in SockAddr;
    bzero(&SockAddr, sizeof (SockAddr));
    SockAddr.sin_family = AF_INET;

    // 设置主机地址
    SockAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 设置端口号
    SockAddr.sin_port = htons((unsigned short) port);

    // 发送数据
    int Length = sendto(Sock, data.c_str(), data.size(), 0, (struct sockaddr *) &SockAddr, sizeof (struct sockaddr_in));

    // 关闭套接字
    CloseSock(Sock);

    return (Length >= 0);
}

/*
 *  功能：
 *      发送tcp数据
 *  参数
 *      ip              :   ip地址
 *      port            :   端口
 *      data            :   数据
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::SendTcpData(string ip, string port, string data)
{
    return SendTcpData(ip, BeGeneralLib::StringToInt(port), data);
}

/*
 *  功能：
 *      发送tcp数据
 *  参数
 *      ip              :   ip地址
 *      port            :   端口
 *      data            :   数据
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::SendTcpData(string ip, int port, string data)
{
    bool Flag = false;

    // 创建套接字
    int Sock = socket(AF_INET, SOCK_STREAM, 0);
    if (Sock < 0)
    {
        return false;
    }

    // 初始化套接字地址
    struct sockaddr_in SockAddr;
    bzero(&SockAddr, sizeof (SockAddr));
    SockAddr.sin_family = AF_INET;

    // 设置主机地址
    SockAddr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 设置端口号
    SockAddr.sin_port = htons((unsigned short) port);

    // 将结构剩下的部分清零
    bzero(&(SockAddr.sin_zero), 8);

    // 设置连接超时时间
    struct timeval Timeout = {DEFAULT_SOCKET_CONNECT_TIMEOUT, 0};
    setsockopt(Sock, SOL_SOCKET, SO_SNDTIMEO, (char *) & Timeout, sizeof (struct timeval));
    int Res = connect(Sock, (struct sockaddr *) & SockAddr, sizeof (struct sockaddr));
    if (Res == 0) // 连接成功
    {
        // 发送数据
        if (send(Sock, data.c_str(), data.size(), 0) >= 0)
        {
            Flag = true;
        }
    }

    // 关闭socket
    BeGeneralLib::CloseSock(Sock);

    return Flag;
}

//检查TCP端口是否打开
bool BeGeneralLib::IsTcpPortOpen(string ip, int port)
{
    bool Flag = false;

    // 创建套接字
    int Sock = socket(AF_INET, SOCK_STREAM, 0);
    if (Sock < 0) return false;

    // 初始化套接字地址
    struct sockaddr_in SockAddr;
    bzero(&SockAddr, sizeof (SockAddr));
    SockAddr.sin_family = AF_INET;

    SockAddr.sin_addr.s_addr = inet_addr(ip.c_str());	// 设置主机地址
    SockAddr.sin_port = htons((unsigned short) port);	// 设置端口号

    // 将结构剩下的部分清零
    bzero(&(SockAddr.sin_zero), 8);

    // 设置连接超时时间
    struct timeval Timeout = {DEFAULT_SOCKET_CONNECT_TIMEOUT, 0};
    setsockopt(Sock, SOL_SOCKET, SO_SNDTIMEO, (char *) & Timeout, sizeof (struct timeval));
    int Res = connect(Sock, (struct sockaddr *) & SockAddr, sizeof (struct sockaddr));
    if (Res == 0) Flag = true;		// 连接成功
    BeGeneralLib::CloseSock(Sock);	// 关闭socket

    return Flag;
}

bool BeGeneralLib::IsTcpPortOpenNonBlock(string ip, int port)
{
    bool Flag = false;

    int Sock = socket(AF_INET, SOCK_STREAM, 0); if (Sock < 0) return false;

    struct sockaddr_in SockAddr;
    bzero(&SockAddr, sizeof (SockAddr));
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_addr.s_addr = inet_addr(ip.c_str());
    SockAddr.sin_port = htons((unsigned short) port);
    bzero(&(SockAddr.sin_zero), 8);

    int flags = fcntl(Sock, F_GETFL);
    fcntl(Sock, F_SETFL, flags | O_NONBLOCK);

    if (connect(Sock, (struct sockaddr *)&SockAddr, sizeof (struct sockaddr)) != 0)
   	{
   		if (errno == EINPROGRESS)
   		{
   			int error=-1, len;
   			len = sizeof(int);
   			timeval tm; tm.tv_sec=10; tm.tv_usec=0;
   			fd_set set;
   			FD_ZERO(&set);
   			FD_SET(Sock, &set);
   			if (select(Sock+1, NULL, &set, NULL, &tm) > 0){
   				getsockopt(Sock, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
   				if (error == 0) Flag = true;
   			}
   		}
   		else Flag = false;
   	}
   	else Flag = true;

    BeGeneralLib::CloseSock(Sock);
    return Flag;
}

/*
 *  功能：
 *      获取监听端口进程id
 *  参数
 *      protocol        :   协议
 *      port            :   端口
 *  返回：
 *      进程id
 */
string BeGeneralLib::GetListenPortProcessId(BeGeneralLib::SocketProtocol protocol, int port)
{
    string ProtocolOption = DEFAULT_EMPTY_STRING;
    switch (protocol)
    {
        case BeGeneralLib::SOCKET_PROTOCOL_TCP:
        {
            ProtocolOption = string("-t");
            break;
        }
        case BeGeneralLib::SOCKET_PROTOCOL_UDP:
        {
            ProtocolOption = string("-u");
            break;
        }
    }

    string ShellCommand = string("netstat -lnp") + SEPARATOR_CHAR_SPACE + ProtocolOption + SEPARATOR_CHAR_SPACE + string("| gawk '{if($4~/") + BeGeneralLib::Number2String(port) + string("$/) print $7}' | gawk -F \"/\" '{print $1}'");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      获取套接字协议名
 *  参数
 *      protocol        :   协议
 *  返回：
 *      协议名
 */
string BeGeneralLib::GetSocketProtocolName(BeGeneralLib::SocketProtocol protocol)
{
    string ProtocolName = DEFAULT_EMPTY_STRING;

    switch (protocol)
    {
        case BeGeneralLib::SOCKET_PROTOCOL_TCP:
        {
            ProtocolName = string("TCP");
            break;
        }
        case BeGeneralLib::SOCKET_PROTOCOL_UDP:
        {
            ProtocolName = string("UDP");
            break;
        }
    }
    return ProtocolName;
}

/*
 *  功能：
 *      检查端口是否被占用
 *  参数
 *      protocol        :   协议
 *      port            :   端口
 *  返回：
 *      如果时返回true，否则返回false
 */
bool BeGeneralLib::IsPortUsed(BeGeneralLib::SocketProtocol protocol, string port)
{
    return BeGeneralLib::IsPortUsed(protocol, BeGeneralLib::StringToInt(port));
}

/*
 *  功能：
 *      检查端口是否被占用
 *  参数
 *      protocol        :   协议
 *      port            :   端口
 *  返回：
 *      如果时返回true，否则返回false
 */
bool BeGeneralLib::IsPortUsed(BeGeneralLib::SocketProtocol protocol, int port)
{
    string ProtocolOption = DEFAULT_EMPTY_STRING;
    switch (protocol)
    {
        case BeGeneralLib::SOCKET_PROTOCOL_TCP:
        {
            ProtocolOption = string("-t");
            break;
        }
        case BeGeneralLib::SOCKET_PROTOCOL_UDP:
        {
            ProtocolOption = string("-u");
            break;
        }
    }

    string ShellCommand = string("netstat -lnp") + SEPARATOR_CHAR_SPACE + ProtocolOption + SEPARATOR_CHAR_SPACE + string("| gawk '{print $4}' | gawk -F: '{if ($NF == \"") + BeGeneralLib::Number2String(port) + string("\") print $NF}'");
    return (BeGeneralLib::ReadShellReturnValueAll(ShellCommand).size() > 0);
}

/*
 *  功能：
 *      检查ping是否OK
 *  参数
 *      ip                  :   ip地址
 *      count               :   ping包个数
 *      interval            :   间隔
 *  返回：
 *      如果ping成功返回true，否则返回false
 */
bool BeGeneralLib::IsPingOK(string ip, int count, float interval)
{
    string ShellCommand = string("ping") + SEPARATOR_CHAR_SPACE + ip + SEPARATOR_CHAR_SPACE + string("-c") + SEPARATOR_CHAR_SPACE + Number2String(count) + SEPARATOR_CHAR_SPACE + string("-i") + SEPARATOR_CHAR_SPACE + Number2String(interval, 0) + SEPARATOR_CHAR_SPACE + string("2>/dev/null | grep received | gawk -F, '{print $2}' | gawk '{print $1}'");
    return (BeGeneralLib::StringToInt(ReadShellReturnValue(ShellCommand)) != 0);
}

/*
 *  功能：
 *      将ip字符串转换为ip结构
 *  参数
 *      ip              :   ip地址
 *  返回：
 *      ip结构
 */
struct in_addr BeGeneralLib::ConvertIpStringToStruct(char * ip)
{
    struct in_addr NetAddr; /*IP地址的二进制表示形式*/
    bzero(&NetAddr, sizeof (NetAddr));
    if (inet_pton(AF_INET, ip, &NetAddr) < 0)/*地址由字符串转换为二级制数*/
    {

        perror("fail to convert");
    }
    return NetAddr;
}

/*
 *  功能：
 *      将ip结构转换为ip字符串
 *  参数
 *      addr            :   ip结构
 *  返回：
 *      ip结构
 */
string BeGeneralLib::ConvertStructToIpString(struct in_addr addr)
{
    char IpString[DEFAULT_BUFFER_SIZE]; /*IP地址的点分十进制字符串表示形式*/
    bzero(IpString, sizeof (IpString));
    if (inet_ntop(AF_INET, &addr, IpString, (socklen_t)sizeof (IpString)) == NULL) /*地址由二进制数转换为点分十进制*/
    {

        return string("");
    }
    return string(IpString);
}

/*
 *  功能：
 *      获取子网
 *  参数
 *      ip              :   ip地址
 *      mask            :   子网掩码
 *  返回：
 *      获取子网
 */
string BeGeneralLib::GetSubnet(string ip, string mask)
{
    struct in_addr NetAddr;
    bzero(&NetAddr, sizeof (NetAddr));
    NetAddr.s_addr = ntohl(inet_network(ip.c_str())) & ntohl(inet_network(mask.c_str()));

    return ConvertStructToIpString(NetAddr);
}

/*
 *  功能：
 *      生成ip地址
 *  参数
 *      subnet          :   子网
 *      host            :   主机
 *  返回：
 *      ip地址
 */
string BeGeneralLib::MakeIpAddr(string subnet, unsigned int host)
{
    struct in_addr NetAddr, HostAddr, IpAddr;

    // 初始化变量
    bzero(&NetAddr, sizeof (NetAddr));
    bzero(&HostAddr, sizeof (HostAddr));
    bzero(&IpAddr, sizeof (IpAddr));

    // 获取子网的网络字节次序
    inet_aton(subnet.c_str(), &NetAddr);

    // 获取主机的网络字节次序
    inet_aton(Number2String(host).c_str(), &HostAddr);

    // 生成ip地址
    IpAddr.s_addr = NetAddr.s_addr | HostAddr.s_addr;

    return inet_ntoa(IpAddr);
}

/*
 *  功能：
 *      删除桥接
 *  参数
 *      bridge          :   桥接名
 *  返回：
 *      无
 */
void BeGeneralLib::DeleteBridge(string bridge)
{
    // 检查桥接是否有效
    if (IsStringEmptyOrInvalidValue(bridge) == true)
    {
        return;
    }

    // 停用网卡
    DeactivateNetworkInterface(bridge);

    // 删除桥接
    string ShellCommand = string("brctl delbr") + SEPARATOR_CHAR_SPACE + bridge;
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      激活网卡
 *  参数
 *      interface       :   网卡
 *  返回：
 *      无
 */
void BeGeneralLib::ActivateNetworkInterface(string interface)
{
    // 检查桥接是否有效
    if (IsStringEmptyOrInvalidValue(interface) == true)
    {
        return;
    }

    // 激活网卡
    string ShellCommand = string("ifconfig") + SEPARATOR_CHAR_SPACE + interface + SEPARATOR_CHAR_SPACE + string("up");
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      停用网卡
 *  参数
 *      interface       :   网卡
 *  返回：
 *      无
 */
void BeGeneralLib::DeactivateNetworkInterface(string interface)
{
    // 检查桥接是否有效
    if (IsStringEmptyOrInvalidValue(interface) == true)
    {
        return;
    }

    // 停用网卡
    string ShellCommand = string("ifconfig") + SEPARATOR_CHAR_SPACE + interface + SEPARATOR_CHAR_SPACE + string("down");
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      桥接网卡是否存在
 *  参数
 *      interface       :   网卡
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool BeGeneralLib::IsBridgeNicExist(string interface)
{
    // 检查桥接是否有效
    if (IsStringEmptyOrInvalidValue(interface) == true)
    {
        return false;
    }

    // 检查桥接网卡是否存在
    string ShellCommand = string("brctl show| gawk '{if ($1==\"") + interface + string("\") print $0}'");
    return (BeGeneralLib::IsStringEmptyOrInvalidValue(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false);
}

/*
 *  功能：
 *      物理网卡是否存在
 *  参数
 *      interface       :   网卡
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool BeGeneralLib::IsPhysicalNicExist(string interface)
{
    // 检查是否有效
    if (IsStringEmptyOrInvalidValue(interface) == true)
    {
        return false;
    }

    // 检查网卡是否存在
    string ShellCommand = string("cat /proc/net/dev | gawk -F: '{if($1~\"") + interface + ("\") print $1}'  | gawk '{if($1==\"") + interface + ("\") print $1}'");
    return (BeGeneralLib::ReadShellReturnValue(ShellCommand) == interface);
}

/*
 *  功能：
 *      将utf-8转换为unicode编码
 *  参数
 *      instring        :   输入的字符串
 *      outbuffer       :   输出缓冲
 *      outbuffersize   :   输出缓冲大小
 *  返回：
 *      转换的字节数
 */
int BeGeneralLib::Utf8ToUnicode(string instring, char * outbuffer, size_t outbuffersize)
{
    return CharsetConvert("utf-8", "unicode", (char *) instring.c_str(), instring.size(), outbuffer, outbuffersize);
}

/*
 *  功能：
 *      将unicode转换为utf-8编码
 *  参数
 *      inbuffer        :   输入缓冲
 *      inbuffersize    :   输入缓冲大小
 *      outstring       :   输出字符串
 *  返回：
 *      转换的字节数
 */
int BeGeneralLib::UnicodeToUtf8(char * inbuffer, size_t inbuffersize, string &outstring)
{
    char OutBuffer[DEFAULT_BUFFER_SIZE];
    int ConvertLength = CharsetConvert("unicode", "utf-8", inbuffer, inbuffersize, OutBuffer, DEFAULT_BUFFER_SIZE);
    OutBuffer[ConvertLength] = DEFAULT_C_STRING_END_FLAG;
    outstring = OutBuffer;
    return ConvertLength;
}

/*
 *  功能：
 *      更新image文件扇区数
 *  参数
 *      imagefile       :   镜像文件名
 *      size            :   磁盘大小
 *  返回：
 *      如果更新成功返回true，否则返回false
 */
bool BeGeneralLib::UpdateImageFileSectors(string imagefile, int size)
{
    // 计算总的扇区数

    long long TotalSector = (((size * 1024LL)*1024)*1024) / 512;

    // 定义联合体

    union
    {
        int Sectors;
        unsigned char SectorHex[4];
    } SectorUnion;
    SectorUnion.Sectors = htonl(TotalSector);

    // 将总扇区信息写入文件
    int fd;
    if ((fd = open(imagefile.c_str(), O_RDWR)) != 0)
    {
        for (int j = 0; j < 4; j++)
        {
            lseek(fd, DEFAULT_MBR_SECTOR_OFFSET - j, SEEK_SET);
            write(fd, &SectorUnion.SectorHex[j], 1);
        }
    }
    close(fd);

    return true;
}

/*
 *  功能：
 *      更新image文件磁盘签名
 *  参数
 *      imagefile       :   镜像文件名
 *  返回：
 *      如果更新成功返回true，否则返回false
 */
bool BeGeneralLib::UpdateImageFileSignature(string imagefile)
{
    // 使用unix系统时间作为磁盘签名
    time_t NowTime;
    time(&NowTime);

    // 定义联合体

    union
    {
        int Value;
        unsigned char ByteArray[4];
    } IntByteUnion;
    IntByteUnion.Value = NowTime;

    // 将签名信息写入文件
    int fd;
    if ((fd = open(imagefile.c_str(), O_RDWR)) != 0)
    {
        for (int j = 0; j < sizeof (IntByteUnion); j++)
        {

            lseek(fd, DEFAULT_MBR_SIGNATURE_OFFSET + j, SEEK_SET);
            write(fd, &IntByteUnion.ByteArray[j], 1);
        }
    }
    close(fd);
}

/*
 *  功能：
 *      更新image文件文件系统
 *  参数
 *      imagefile       :   镜像文件名
 *      fs              :   文件系统
 *  返回：
 *      如果更新成功返回true，否则返回false
 */
bool BeGeneralLib::UpdateImageFileSystem(string imagefile, BaseFileSystem fs)
{
    return UpdateImageFileSystem(imagefile, 0, fs);
}

/*
 *  功能：
 *      更新image文件文件系统
 *  参数
 *      imagefile       :   镜像文件名
 *      partition       :   分区
 *      fs              :   文件系统
 *  返回：
 *      如果更新成功返回true，否则返回false
 */
bool BeGeneralLib::UpdateImageFileSystem(string imagefile, int partition, BaseFileSystem fs)
{
    if (partition < 0 || partition > 3)
    {
        return false;
    }

    unsigned char FileSystemId = 0;
    switch (fs)
    {
        case BASE_FILESYSTEM_NTFS:
        {
            FileSystemId = 0x07;
            break;
        }
        case BASE_FILESYSTEM_RAW:
        {
            FileSystemId = 0x06;
            break;
        }
        default:
        {
            return false;
        }
    }

    // 将签名信息写入文件
    int fd;
    if ((fd = open(imagefile.c_str(), O_RDWR)) != 0)
    {
        lseek(fd, DEFAULT_MBR_PARTITION_OFFSET + (partition * DEFAULT_MBR_PARTITION_LENGTH) + DEFAULT_MBR_PARTITION_FILESYSTEM_OFFSET, SEEK_SET);
        write(fd, &FileSystemId, 1);
    }
    close(fd);

    return true;
}

/*
 *  功能：
 *      导入镜像文件头
 *  参数：
 *      imagefile       :   镜像文件
 *      headerfile      :   头文件
 *  返回：
 *      导入成功返回true，否则返回false
 */
bool BeGeneralLib::ImportImageHeader(string imagefile, string headerfile)
{
    // 检查文件是否存在
    if (BeGeneralLib::IsFileExist(imagefile) == false)
    {
        return false;
    }

    // 检查头文件是否存在
    if (BeGeneralLib::IsFileExist(headerfile) == false)
    {
        headerfile = BeGeneralLib::GetExecutePath() + headerfile;
        if (BeGeneralLib::IsFileExist(headerfile) == false)
        {
            //            MyLastError->SetError(BesError::BES_ERR_FILE_NOT_EXIST, headerfile, DEBUGARGS, false);
            return false;
        }
    }

    // 设置导入命令
    string ShellCommand = string("dd if=") + headerfile + SEPARATOR_CHAR_SPACE + string("of=") + imagefile + SEPARATOR_CHAR_SPACE + string("bs=512 count=1 conv=notrunc");

    // 执行命令操作
    BeGeneralLib::ExecuteSystem(ShellCommand, true, false);

    return true;
}

/*
 *  功能：
 *      创建镜像文件
 *  参数
 *      filename        :   文件名
 *      capacity        :   容量
 *  返回：
 *      创建成功返回true，否则返回false
 */
bool BeGeneralLib::CreateImageFile(string filename, int capacity)
{
    // 磁盘大小
    long int DiskSize = (capacity * 16) + 1;

    // 生成image文件
    string ShellCommand = string("dd if=/dev/zero of=") + filename + SEPARATOR_CHAR_SPACE + string("bs=64M count=0 seek=") + BeGeneralLib::Number2String(DiskSize);
    BeGeneralLib::ExecuteSystem(ShellCommand, true, true);

    // 检查文件是否创建
    return (BeGeneralLib::IsFileExist(filename) == true);
}

/*
 *  功能：
 *      获取主机名
 *  参数
 *      无
 *  返回：
 *      主机名
 */
string BeGeneralLib::GetSystemHostName()
{
    string HostName = GetConfigItemValue(DEFAULT_HOST_NAME_CONFIG, "HOSTNAME");
    if (IsStringEmptyOrInvalidValue(HostName) == true)
    {
        string ShellCommand = string("hostname");
        HostName = BeGeneralLib::ExecuteSystem(ShellCommand);
    }
    return HostName;
}

/*
 *  功能：
 *      重置终端颜色
 *  参数
 *      无
 *  返回：
 *      无
 */
void BeGeneralLib::ResetConsoleColor()
{
    cout << "\33[0m";
}

/*
 *  功能：
 *      删除系统日志
 *  参数
 *      无
 *  返回：
 *      无
 */
void BeGeneralLib::DeleteSystemLog()
{
    // 检查转储文件
    string ShellCommand = string("ls -l --block-size=M /var/crash/ 2>/dev/null | gawk '{if (int($5)>500) print $9}'| sort  | xargs rm -f");
    BeGeneralLib::ExecuteSystem(ShellCommand, true, BesLog::LOG_MESSAGE_TYPE_VERBOSE);

    // 删除系统日志
    ShellCommand = string("ls -lh /var/log/messages* 2>/dev/null | gawk '{print $9}' | sort  | gawk '{if (NR>2) print $1}' | xargs rm -f");
    BeGeneralLib::ExecuteSystem(ShellCommand, true, BesLog::LOG_MESSAGE_TYPE_VERBOSE);

    // 检查当前系统日志是否大于500MB
    ShellCommand = string("ls -l --block-size=M /var/log/messages 2>/dev/null | gawk '{if (int($5)>500) print $9}' | xargs rm -f");
    BeGeneralLib::ExecuteSystem(ShellCommand, true, BesLog::LOG_MESSAGE_TYPE_VERBOSE);

    // 删除http访问日志
    ShellCommand = string("ls -lh /var/log/httpd/access_log* 2>/dev/null | gawk '{print $9}' | sort  | gawk '{if (NR>2) print $1}' | xargs rm -f");
    BeGeneralLib::ExecuteSystem(ShellCommand, true, BesLog::LOG_MESSAGE_TYPE_VERBOSE);

    // 检查当前http访问日志是否大于500MB
    ShellCommand = string("ls -l --block-size=M /var/log/access_log 2>/dev/null | gawk '{if (int($5)>500) print $9}' | xargs rm -f");
    BeGeneralLib::ExecuteSystem(ShellCommand, true, BesLog::LOG_MESSAGE_TYPE_VERBOSE);

    // 删除http错误日志
    ShellCommand = string("ls -lh /var/log/httpd/error_log* 2>/dev/null | gawk '{print $9}' | sort  | gawk '{if (NR>2) print $1}' | xargs rm -f");
    BeGeneralLib::ExecuteSystem(ShellCommand, true, BesLog::LOG_MESSAGE_TYPE_VERBOSE);

    // 检查当前http错误日志是否大于500MB
    ShellCommand = string("ls -l --block-size=M /var/log/error_log 2>/dev/null | gawk '{if (int($5)>500) print $9}' | xargs rm -f");
    BeGeneralLib::ExecuteSystem(ShellCommand, true, BesLog::LOG_MESSAGE_TYPE_VERBOSE);
}

/*
 *  功能：
 *      获取符号链接文件对应文件的绝对路径
 *  参数
 *      linkfile        :   符号链接文件
 *  返回：
 *      符号链接文件对应文件的绝对路径
 */
string BeGeneralLib::GetLinkFileFullPath(string linkfile)
{
    if (IsStringEmptyOrInvalidValue(linkfile) == true)
    {
        return "";
    }
    string ShellCommand = string("readlink -e") + SEPARATOR_CHAR_SPACE + linkfile;
    return ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      配置动态链接
 *  参数
 *      无
 *  返回：
 *      无
 */
void BeGeneralLib::ConfigureDynamicLinker()
{
    string ShellCommand = string("ldconfig");
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      创建符号链接
 *  参数
 *      target              :   目标文件
 *      linker              :   链接名
 *  返回：
 *      创建成功返回true，否则返回false
 */
bool BeGeneralLib::CreateSymbolLinks(string target, string linker)
{
    // 检查输入有效性
    if (IsFileExist(target) == false
            || IsStringEmptyOrInvalidValue(linker) == true)
    {
        return false;
    }

    // 检查软链接是否存在
    if (IsFileExist(linker) == false)
    {
        string ShellCommand = string("ln -s") + SEPARATOR_CHAR_SPACE + target + SEPARATOR_CHAR_SPACE + linker;
        ExecuteSystem(ShellCommand);
    }

    return (IsFileExist(linker) == true);
}

/*
 *  功能：
 *      生成动态链接库文件名
 *  参数
 *      dynamiclinkname     :   动态链接库名
 *  返回：
 *      成功返回文件名，否则空
 */
string BeGeneralLib::GenerateDynamicLinkFileName(string dynamiclinkname)
{
    // 检查输入
    if (IsStringEmpty(dynamiclinkname) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    // 返回动态链接库文件名
    return (string(DEFAULT_DYNAMIC_LINKER_PREFIX) + dynamiclinkname + string(DEFAULT_DYNAMIC_LINKER_FILE_EXTENSION));
}

/*
 *  功能：
 *      检查包是否安装
 *  参数
 *      无
 *  返回：
 *      如果已安装则返回true，否则返回false
 */
bool BeGeneralLib::IsPackageInstalled(string name)
{
    // 检查输入的有效性
    if (IsStringEmptyOrInvalidValue(name) == true)
    {
        return false;
    }

    // 检查指定包是否已安装
    string ShellCommand = string("rpm -qa | grep ^") + name;
    return (BeGeneralLib::IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false);
}

/*
 *  功能：
 *      检查安装包
 *  参数
 *      无
 *  返回：
 *      无
 */
void BeGeneralLib::CheckPackage()
{
    // 检查lsscsi是否安装
    string ShellCommand = string("rpm -qa | grep ^lsscsi");
    if (BeGeneralLib::IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false)
    {
        BesLog::DebugPrint("The lsscsi package is OK.", false);
    }
    else
    {
        BesLog::DebugPrint("The lsscsi package is not installed!", false);
    }

    // 检查mysqld
    ShellCommand = string("rpm -qa | grep ^mysql-server");
    if (BeGeneralLib::IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false)
    {
        BesLog::DebugPrint("The mysql package is OK.", false);
    }
    else
    {
        BesLog::DebugPrint("The mysql package is not installed!", false);
    }

    // 检查httpd
    ShellCommand = string("rpm -qa | grep ^httpd");
    if (BeGeneralLib::IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false)
    {
        BesLog::DebugPrint("The http package is OK.", false);
    }
    else
    {
        BesLog::DebugPrint("The http package is not installed!", false);
    }

    // 检查dhcp
    ShellCommand = string("rpm -qa | grep ^dhcp");
    if (BeGeneralLib::IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false)
    {
        BesLog::DebugPrint("The dhcp package is OK.", false);
    }
    else
    {
        BesLog::DebugPrint("The dhcp package is not installed!", false);
    }

    // 检查ntfs
    ShellCommand = string("rpm -qa | grep ^ntfs-3g");
    if (BeGeneralLib::IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false)
    {
        BesLog::DebugPrint("The ntfs package is OK.", false);
    }
    else
    {
        BesLog::DebugPrint("The ntfs package is not installed!", false);
    }

    // 检查tftp
    ShellCommand = string("rpm -qa | grep ^tftp-server");
    if (BeGeneralLib::IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false)
    {
        BesLog::DebugPrint("The tftp package is OK.", false);
    }
    else
    {
        BesLog::DebugPrint("The tftp package is not installed!", false);
    }

    // 检查emboot
    ShellCommand = string("rpm -qa | grep ^emnbid");
    if (BeGeneralLib::IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == false)
    {
        BesLog::DebugPrint("The emboot package is OK.", false);
    }
    else
    {
        BesLog::DebugPrint("The emboot package is not installed!", false);
    }
}

/*
 *  功能：
 *      检查端口是否使用，如果使用，则将使用的进程杀死
 *  参数
 *      port            :   端口号
 *  返回：
 *      无
 */
void BeGeneralLib::CheckPort(int port)
{
    int Counter = 0;
    while (Counter++ < 10)
    {
        string ShellCommand = string("netstat -tlnp |gawk '$0~/") + BeGeneralLib::Number2String(port) + string("/ {print $7}' | gawk -F/ '{print $2}'");
        string ProcessName = BeGeneralLib::ReadShellReturnValue(ShellCommand);
        if (IsStringEmpty(ProcessName) == true)
        {
            break;
        }

        if (ProcessName == DEFAULT_SERVICE_NAME_DHCP
                || ProcessName == DEFAULT_SERVICE_NAME_EMBOOT
                || ProcessName == DEFAULT_SERVICE_NAME_STGT
                || ProcessName == DEFAULT_SERVICE_NAME_MYSQL
                || ProcessName == DEFAULT_SERVICE_NAME_ISCSID)
        {
            //            if (ProcessName == DEFAULT_SERVICE_NAME_STGT)
            //            {
            //                StgtManager::DeleteDeadSession();
            //            }
            BeGeneralLib::OperateService(BeGeneralLib::SERVICE_ACTION_RESTART, ProcessName);
        }
        else
        {
            KillProcess(ProcessName, true);
        }
        sleep(1);
    }
}

/*
 *  功能：
 *      杀死进程
 *  参数
 *      name            :   进程名
 *      isforce         :   是否强制杀死
 *  返回：
 *      无
 */
void BeGeneralLib::KillProcess(string name, bool isforce)
{
    int Signal = SIGINT;
    if (isforce == true)
    {
        Signal = SIGKILL;
    }
    string ShellCommand = string("killall -e -s") + SEPARATOR_CHAR_SPACE + Number2String(Signal) + SEPARATOR_CHAR_SPACE + name;
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      杀死进程
 *  参数
 *      pid             :   进程id
 *      isforce         :   是否强制杀死
 *  返回：
 *      无
 */
void BeGeneralLib::KillProcessById(string pid, bool isforce)
{
    int Signal = SIGINT;
    if (isforce == true)
    {
        Signal = SIGKILL;
    }
    string ShellCommand = string("kill -s") + SEPARATOR_CHAR_SPACE + Number2String(Signal) + SEPARATOR_CHAR_SPACE + pid;
    ExecuteSystem(ShellCommand);
}

/*
 *  功能：
 *      检查环境变量
 *  参数
 *      无
 *  返回：
 *      无
 */
void BeGeneralLib::CheckEnvVariables()
{
    // 检查PATH
    string ShellCommand = string("env | grep ^PATH | grep /usr/local/sbin");
    //    if (IsStringEmpty(BeGeneralLib::ReadShellReturnValue(ShellCommand)) == true)
    {
        ShellCommand = string("source /etc/profile");
        BeGeneralLib::ExecuteSystem(ShellCommand);
    }
}

/*
 *  功能：
 *      检查进程是否运行
 *  参数
 *      processname     :   进程名
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool BeGeneralLib::IsProcessRunning(string processname)
{
    return (GetProcessId(processname).size() != 0);
}

/*
 *  功能：
 *      检查进程是否运行
 *  参数
 *      processname     :   进程名
 *  返回：
 *      如果存在返回true，否则返回false
 */
vector<string> BeGeneralLib::GetProcessId(string processname)
{
    vector<string> PidList;

    // 检查进程名是否为空
    if (IsStringEmptyOrInvalidValue(processname) == true)
    {
        return PidList;
    }

    // 使用pgrep命令查看
    string ShellCommand = string("pgrep -x \"") + processname + string("\"");
    PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);

    // 使用pidof
    if (PidList.size() == 0)
    {
        ShellCommand = string("pidof \"") + processname + string("\"| gawk '{for(i=1;i<=NF;i++) print $i}'");
        PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
    }

    // 使用pidof
    if (PidList.size() == 0)
    {
        ShellCommand = string("ps -eo pid,cmd | gawk '{if($0~\"") + processname + string("\") print $0}'") + SEPARATOR_CHAR_SPACE + string("| grep -v gawk | gawk '{print $1}'");
        PidList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);
    }
    return PidList;
}

/*
 *  功能：
 *      检查进程
 *  参数：
 *      processname     :   进程名
 *  返回：
 *      检查成功返回true，否则返回false
 */
bool BeGeneralLib::CheckProcess(string processname)
{
    // 检查程序是否运行
    if (BeGeneralLib::IsProcessRunning(processname) == false)
    {
        BeGeneralLib::StartupProcess(BeGeneralLib::GetExecutePath() + processname, true);
        sleep(5);
    }

    // 再次检查程序是否运行
    return (BeGeneralLib::IsProcessRunning(processname) == true);
}

/*
 *  功能：
 *      检查文件权限
 *  参数：
 *      path            :   路径名
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::CheckFilePermission(string path)
{
    return CheckFilePermission(path, DEFAULT_FILE_PERMISSION);
}

/*
 *  功能：
 *      检查文件权限
 *  参数：
 *      path            :   路径名
 *      mode            :   文件模式
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::CheckFilePermission(string path, int mode)
{
    // 检查路径是否为无效值
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(path) == true)
    {
        return false;
    }

    struct stat StatBuf;
    sizeof (StatBuf, sizeof (StatBuf));

    // 查看文件状态
    if (stat(path.c_str(), &StatBuf) == -1)
    {
        return false;
    }

    // 检查文件模式是否和指定的模式相同
    return (StatBuf.st_mode & mode == mode);
}

/*
 *  功能：
 *      修改文件权限
 *  参数：
 *      path            :   路径名
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::ChangeFilePermission(string path)
{
    return ChangeFilePermission(path, DEFAULT_FILE_PERMISSION);
}

/*
 *  功能：
 *      修改文件权限
 *  参数：
 *      path            :   路径名
 *      mode            :   文件模式
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::ChangeFilePermission(string path, int mode)
{
    return ChangeFilePermission(path, mode, false);
}

/*
 *  功能：
 *      修改文件权限
 *  参数：
 *      path            :   路径名
 *      mode            :   文件模式
 *      recursive       :   是否递归
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::ChangeFilePermission(string path, int mode, bool recursive)
{
    bool Flag = false;

    // 检查路径是否为无效值
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(path) == true)
    {
        return false;
    }

    // 检查文件是否存在
    if (BeGeneralLib::IsFileExist(path) == false)
    {
        return false;
    }

    switch (GetFileType(path))
    {
        case BeGeneralLib::FILE_TYPE_REGULAR:
        {
            Flag = chmod(path.c_str(), mode);
            break;
        }
        case BeGeneralLib::FILE_TYPE_DIRECTORY:
        {
            string OptionString = "";
            if (recursive == true)
            {
                OptionString = string("-R");
            }
            string ShellCommand = string("chmod") + SEPARATOR_CHAR_SPACE + OptionString + SEPARATOR_CHAR_SPACE + BeGeneralLib::ConvertNumberBase(BeGeneralLib::Number2String(mode), BeGeneralLib::NUMBER_BASE_8) + SEPARATOR_CHAR_SPACE + path;
            BeGeneralLib::ExecuteSystem(ShellCommand);
            Flag = true;
            break;
        }
        default:
        {
            break;
        }
    }
    return Flag;
}

/*
 *  功能：
 *      检查并修改文件权限
 *  参数：
 *      path            :   路径名
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::CheckAndChangeFilePermission(string path)
{
    return CheckAndChangeFilePermission(path, DEFAULT_FILE_PERMISSION);
}

/*
 *  功能：
 *      检查并修改文件权限
 *  参数：
 *      path            :   路径名
 *      mode            :   文件模式
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::CheckAndChangeFilePermission(string path, int mode)
{
    return CheckAndChangeFilePermission(path, mode, false);
}

/*
 *  功能：
 *      检查并修改文件权限
 *  参数：
 *      path            :   路径名
 *      mode            :   文件模式
 *      recursive       :   是否递归
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::CheckAndChangeFilePermission(string path, int mode, bool recursive)
{
    // 检查文件权限
    if (CheckFilePermission(path, mode) == true)
    {
        return false;
    }

    // 修改文件权限
    return (ChangeFilePermission(path, mode, recursive) == true);
}

/*
 *  功能：
 *      检查并修改可执行文件权限
 *  参数：
 *      file            :   文件名
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::CheckAndChangeBinFilePermission(string file)
{
    // 检查是否为无效值
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(file) == true)
    {
        return false;
    }

    // 检查文件是否存在
    string FileName = file;
    if (IsFileExist(FileName) == true)
    {
        FileName = BeGeneralLib::GetFileName(FileName);
    }

    // 检查/usr/bin下的文件
    string BinFileName = DEFAULT_USR_BIN_PATH + FileName;
    CheckAndChangeFilePermission(BinFileName);

    // 检查/usr/sbin下的文件
    BinFileName = DEFAULT_USR_SBIN_PATH + FileName;
    CheckAndChangeFilePermission(BinFileName);

    // 检查/usr/local/bin下的文件
    BinFileName = DEFAULT_USR_LOCAL_BIN_PATH + FileName;
    CheckAndChangeFilePermission(BinFileName);

    // 检查/usr/loca/sbin下的文件
    BinFileName = DEFAULT_USR_LOCAL_SBIN_PATH + FileName;
    CheckAndChangeFilePermission(BinFileName);
    return true;
}

/*
 *  功能：
 *      设置文件为锁定状态
 *  参数：
 *      path            :   文件名
 *      islock          :   是否锁定
 *      recursive       :   是否递归
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::SetFileLocked(string path, bool islock)
{
    return SetFileLocked(path, islock, false);
}

/*
 *  功能：
 *      设置文件为锁定状态
 *  参数：
 *      path            :   文件名
 *      islock          :   是否锁定
 *      recursive       :   是否递归
 *  返回：
 *      成功返回true，否则返回false
 */
bool BeGeneralLib::SetFileLocked(string path, bool islock, bool recursive)
{
    // 检查文件是否存在
    if (IsFileExist(path) == false)
    {
        return false;
    }

    string OptionString = "";

    // 设置递归
    if (recursive == true)
    {
        OptionString = string("-R");
    }

    // 设置是否为锁定
    if (islock == true)
    {
        OptionString = OptionString + SEPARATOR_CHAR_SPACE + string("+i");
    }
    else
    {
        OptionString = OptionString + SEPARATOR_CHAR_SPACE + string("-i");
    }

    string ShellCommand = string("chattr") + SEPARATOR_CHAR_SPACE + OptionString + SEPARATOR_CHAR_SPACE + path;
    return ExecuteSystem(ShellCommand, true, false);
}

/*
 *  功能：
 *      根据提示获取控制台输入
 *  参数：
 *      prompt          :   提示信息
 *  返回：
 *      控制台输入
 */
string BeGeneralLib::ConsoleInput(string prompt)
{
    return ConsoleInput(prompt, "", 0);
}

/*
 *  功能：
 *      根据提示获取控制台输入
 *  参数：
 *      prompt          :   提示信息
 *      defaultvalue    :   默认值
 *      length          :   输入长度
 *      withendline     :   是否有结尾符
 *  返回：
 *      控制台输入
 */
string BeGeneralLib::ConsoleInput(string prompt, string defaultvalue, int length, bool withendline)
{
    string s = "";
    cin.clear();
    cin.sync();

    cout << endl << prompt;
    if (withendline)
    {
        cout << endl;
    }
    getline(cin, s);
    if (!cin) //当超长时出错处理
    {
        cin.clear();
        while (cin.get() != '\n') //清除掉剩余的字符串，以免影响下一个输入
        {
            continue;
        }
    }

    if (s.length() == 0)
    {
        s = defaultvalue;
    }

    if (length != 0 && s.length() >= length)
    {
        s = s.substr(0, length);
    }
    return s;
}

/*
 *  功能：
 *      转换进制
 *  参数：
 *      input           :   输入
 *      obase           :   输出的进制
 *  返回：
 *      成功返回转换后的进制，否则为空
 */
string BeGeneralLib::ConvertNumberBase(string input, NumberBase obase)
{
    return ConvertNumberBase(input, BeGeneralLib::NUMBER_BASE_10, obase);
}

/*
 *  功能：
 *      转换进制
 *  参数：
 *      input           :   输入
 *      ibase           :   输入的进制
 *      obase           :   输出的进制
 *  返回：
 *      成功返回转换后的进制，否则为空
 */
string BeGeneralLib::ConvertNumberBase(string input, BeGeneralLib::NumberBase ibase, BeGeneralLib::NumberBase obase)
{
    string NewBaseString = "";
    if (IsStringEmptyOrInvalidValue(input) == true)
    {
        return NewBaseString;
    }

    // 进制转换
    string ShellCommand = string("echo 'obase=") + Base2String(obase) + string("; ibase=") + Base2String(ibase) + string(";") + input + string("'| bc");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      十六进制转换为十进制
 *  参数：
 *      str             :   十六进制字符串
 *  返回：
 *      转换后的十进制数
 */
int BeGeneralLib::HexToDecimal(string str)
{
    str = StringToUpper(str);
    int Size = str.size();
    int DecValue = 0;
    for (int i = 0; i < Size; i++)
    {
        unsigned char Val = 0;
        char Ch = str.substr(i, 1).c_str()[0];
        switch (Ch)
        {
            case 'F':
            case 'E':
            case 'D':
            case 'C':
            case 'B':
            case 'A':
            {
                Val = Ch - 'A' + 10;
                break;
            }
            default:
            {
                Val = BeGeneralLib::StringToInt(str.substr(i, 1));
                break;
            }
        }
        DecValue = DecValue + (int) Val * (int) pow(16, Size - i - 1);
    }
    return DecValue;
}

/*
 *  功能：
 *      将数组转换为十六进制字符串
 *  参数：
 *      arr             :   数组
 *      len             :   数组长度
 *  返回：
 *      转换后的十六进制字符串
 */
string BeGeneralLib::ArrayToHexString(unsigned char *arr, int len)
{
    if (arr == NULL || len <= 0)
    {
        return "";
    }
    //    cout << "len:" << len << endl;
    string HexString = "";
    for (int i = 0; i < len; i++)
    {
        HexString = HexString + ByteToHexString((unsigned char) arr[i], true);
    }
    return HexString;
}

/*
 *  功能：
 *      将字节转换为十六进制字符串
 *  参数：
 *      byte            :   字节
 *      isupper         :   是否大写
 *  返回：
 *      转换后的十六进制字符串
 */
string BeGeneralLib::ByteToHexString(unsigned char ch, bool isupper)
{
    unsigned char buf[3];
    bzero(buf, sizeof (buf));
    string format = (isupper == true) ? string("%02X\0") : string("%02x\0");
    sprintf((char*) buf, format.c_str(), ch);
    return string((const char *) buf);
}

/*
 *  功能：
 *      获取最后一个分割符后的字符串
 *  参数：
 *      str             :   字符串
 *      seperator       :   输入的进制
 *      obase           :   输出的进制
 *  返回：
 *      成功返回转换后的进制，否则为空
 */
string BeGeneralLib::GetStringAfterFinalSeperator(string str, char seperator)
{
    // 检查字符串是否为空
    if (IsStringEmptyOrInvalidValue(str) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    string ShellCommand = string("echo \"") + str + string("\"| gawk -F\"") + seperator + string("\" '{print $NF}'");
    return BeGeneralLib::ReadShellReturnValue(ShellCommand);
}

/*
 *  功能：
 *      获取描述语言
 *  参数：
 *      无
 *  返回：
 *      描述语言
 */
DescriptionLang BeGeneralLib::GetDescriptionLang()
{
    // 获取配置语言项
    string LangStr = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_LANG);

    // 转换为大写形式
    LangStr = BeGeneralLib::StringToUpper(LangStr);

    // 判断语言项
    DescriptionLang Lang = DESCRIPTION_LANG_CN;
    if (LangStr == DEFAULT_CHINESE_LANG_ID)
    {
        Lang = DESCRIPTION_LANG_CN;
    }
    else if (LangStr == DEFAULT_ENGLISH_LANG_ID)
    {
        Lang = DESCRIPTION_LANG_EN;
    }
    else
    {
        Lang = DESCRIPTION_LANG_CN;
    }
    return Lang;
}

/*
 *  功能：
 *      获取自检日志文件名
 *  参数：
 *      无
 *  返回：
 *      target基础
 */
string BeGeneralLib::GetSelfCheckLogFileName()
{
    return (GetExecutePath() + string(DEFAULT_SELF_CHECK_LOG_FILE_NAME));
}

/*
 *  功能：
 *      获取字符串中ascii字符个数
 *  参数：
 *      str
 *  返回：
 *      ascii字符个数
 */
int BeGeneralLib::AsciiCharCount(string str)
{
    int Counter = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (isascii(str.c_str()[i]))
        {
            Counter++;
        }
    }
    return Counter++;
}

/*------------------------------------ 私有方法 --------------------------------*/

/*
 *  功能：
 *      字符转换
 *  参数
 *      char            :   输入字符
 *      len             :   字符串长度
 *      isleft          :   转换方向
 *  返回：
 *      转换后的字符
 */
char BeGeneralLib::TransferTo(char ch, int len, bool isleft)
{
    // 如果为字母或数字
    if (isalnum(ch))
    {
        char StartChar = DEFAULT_C_STRING_END_FLAG;
        int CharCount = 0;
        if (isupper(ch))
        {
            StartChar = DEFAULT_UPPER_START_CHAR;
            CharCount = DEFAULT_ALPHACHAR_COUNT;
        }
        else if (islower(ch))
        {
            StartChar = DEFAULT_LOWER_START_CHAR;
            CharCount = DEFAULT_ALPHACHAR_COUNT;
        }
        else if (isdigit(ch))
        {

            StartChar = DEFAULT_NUMBER_START_CHAR;
            CharCount = DEFAULT_ALPHANUM_COUNT;
        }
        ch = StartChar + (((ch - StartChar) + (int) pow(-1, (double) isleft) * (len % CharCount)) + CharCount) % CharCount;
    }
    return ch;
}

/*
 *  功能：
 *      获取日期格式字符串
 *  参数
 *      format          :   日期格式
 *  返回：
 *      日期格式字符串
 */
string BeGeneralLib::GetTimeFormatString(BeGeneralLib::TimeFormat format)
{
    string FormatString = "";
    switch (format)
    {
        case BeGeneralLib::TIME_FORMAT_LONG:
        {
            FormatString = string("%Y-%m-%d %H:%M:%S");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_DATE:
        {
            FormatString = string("%Y-%m-%d");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_TIME:
        {
            FormatString = string("%H:%M:%S");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_FULL_DATETIME:
        {
            FormatString = string("%Y%m%d%H%M%S");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_FULL_DATE:
        {
            FormatString = string("%Y%m%d");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_FULL_TIME:
        {
            FormatString = string("%H%M%S");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_YEAR:
        {
            FormatString = string("%Y");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_MONTH:
        {
            FormatString = string("%m");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_DAY:
        {
            FormatString = string("%d");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_HOUR:
        {
            FormatString = string("%H");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_MINUTE:
        {
            FormatString = string("%M");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_SECOND:
        {
            FormatString = string("%S");
            break;
        }
        case BeGeneralLib::TIME_FORMAT_COMMAND_HEADER:
        {
            FormatString = string("%d%H%M%S");
            break;
        }
    }
    return FormatString;
}

/*
 *  功能：
 *      获取时间单位字符串
 *  参数
 *      unit            :   日期单位
 *  返回：
 *      时间字符串
 */
string BeGeneralLib::GetTimeUnitString(BeGeneralLib::TimeUnit unit)
{
    string TimeUnitString = "";
    switch (unit)
    {
        case BeGeneralLib::TIME_UNIT_SECOND:
        {
            TimeUnitString = string("second");
            break;
        }
        case BeGeneralLib::TIME_UNIT_MINUTE:
        {
            TimeUnitString = string("minute");
            break;
        }
        case BeGeneralLib::TIME_UNIT_HOUR:
        {
            TimeUnitString = string("hour");
            break;
        }
        case BeGeneralLib::TIME_UNIT_DAY:
        {
            TimeUnitString = string("day");
            break;
        }
        case BeGeneralLib::TIME_UNIT_MONTH:
        {
            TimeUnitString = string("month");
            break;
        }
        case BeGeneralLib::TIME_UNIT_YEAR:
        {
            TimeUnitString = string("year");
            break;
        }
        case BeGeneralLib::TIME_UNIT_WEEK:
        {
            TimeUnitString = string("week");

            break;
        }
    }
    return TimeUnitString;
}

/*
 *  功能：
 *      字符集转换
 *  参数
 *      fromcharset     :   源字符集
 *      tocharset       :   目标字符集
 *      intputstring    :   输入字符串
 *      inputlength     :   输入长度
 *      outbuffer       :   输出缓冲
 *      outbufferlen    :   输出缓存长度
 *  返回：
 *      转换字节数
 */
int BeGeneralLib::CharsetConvert(string fromcharset, string tocharset, char* inputstring, size_t inputlength, char *outbuffer, size_t outbufferlen)
{
    // 设置转换字符串指针
    char * pFromString = inputstring;

    // 查看
    size_t FromStringLength = inputlength;
    size_t outleft = outbufferlen;
    iconv_t cd;
    cd = iconv_open(tocharset.c_str(), fromcharset.c_str());
    if (cd == (iconv_t) - 1)
    {
        perror("iconv_open()");
        return -1;
    }
    memset(outbuffer, 0, outbufferlen);
    if (iconv(cd, &pFromString, &FromStringLength, &outbuffer, &outleft) == -1)
    {
        perror("iconv()");
        return -1;
    }
    iconv_close(cd);

    return (outbufferlen - outleft);
}

//int BeGeneralLib::CharsetConvert(char* inputstring, size_t inputlength, char *outbuffer, size_t outbufferlen)
//{
//    return CharsetConvert(string("utf-8"), string("unicode"), inputstring, inputlength, outbuffer, outbufferlen);
//}
//
//int BeGeneralLib::CharsetConvert(string inputstring, char *outbuffer, size_t outbufferlen)
//{
//    return CharsetConvert((char *) inputstring.c_str(), inputstring.size(), outbuffer, outbufferlen);
//}

/*
 *  功能：
 *      将进制转换为字符串
 *  参数
 *      base            :   进制
 *  返回：
 *      字符串表示的进制
 */
string BeGeneralLib::Base2String(BeGeneralLib::NumberBase base)
{
    string BaseString = "";
    switch (base)
    {
        case BeGeneralLib::NUMBER_BASE_2: // 二进制
        {
            BaseString = string(DEFAULT_BASE_2_STRING);
            break;
        }
        case BeGeneralLib::NUMBER_BASE_8: // 八进制
        {
            BaseString = string(DEFAULT_BASE_8_STRING);
            break;
        }
        case BeGeneralLib::NUMBER_BASE_10: // 十进制
        {
            BaseString = string(DEFAULT_BASE_10_STRING);
            break;
        }
        case BeGeneralLib::NUMBER_BASE_16: // 十六进制
        {
            BaseString = string(DEFAULT_BASE_16_STRING);
            break;
        }
    }
    return BaseString;
}

/*
 *  功能：
 *       将UInt32转换为字符串
 *  参数
 *      str        	:	转换后的字符串
 *      value	:	数值
 *  返回：
 *      转换成功返回true，否则返回false
 */
bool BeGeneralLib::UInt32ToString(char *str, int value) {
	if (str == NULL) {
		return false;
	}
	sprintf(str, "%d\0", value);
}

/*
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions for dog

//检测加密狗是否连接
bool BeGeneralLib::CheckDogExist(){
	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}

	//login failed
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return false;
	}

	//login succeed
	dog_logout(handle);
	return true;
}

//返回-1表示出错，>=0表示成功
int BeGeneralLib::ReadDogHostCount(){
	char c[17] = {0};

	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;

	//login
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return -1;
	}

	//login succeed
	status = dog_read(handle, 2, 0, 16, &c[0]);
	switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
		case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
		case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
		case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
		default:				ErrorInfo = string("read failed");
	}
    if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		dog_logout(handle);
		return -1;
	}

	//read succeed
	status = dog_decrypt(handle, c, 16);
	switch (status){
		case DOG_STATUS_OK:	break;
	    case DOG_INV_HND:	ErrorInfo = string("handle not active"); break;
	    case DOG_TOO_SHORT:	ErrorInfo = string("data length too short"); break;
	    default:			ErrorInfo = string("decryption failed");
	}
	dog_logout(handle);
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return -1;
	}

	//decrypt succeed
	string output = string(c);
    if (output.empty()) return -1;
	int Length = StringToInt(output.substr(0, 1));
	if (output.size() < 1 + Length) return -1;
	return atoi((output.substr(1, Length)).c_str());
}

bool BeGeneralLib::WriteDogHostCount(int cnt){
	if(cnt < 0) return false;
    string HostCount = Number2String(cnt);
    string HostCountSize = Number2String(HostCount.size());
    string Input = HostCountSize + HostCount;

    char c[17] = {0};
	strcpy(c, Input.c_str());

	string LicenseString;

	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;

	//login
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return false;
	}

	//login succeed
	status = dog_encrypt(handle, c, 16);
	switch (status){
        case DOG_STATUS_OK:		break;
        case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
        case DOG_TOO_SHORT:		ErrorInfo = string("data length too short"); break;
        case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
        default:				ErrorInfo = string("encryption failed");
	}
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		dog_logout(handle);
		return false;
	}

	//encrypt succeed
	LicenseString = string(c);
    if (LicenseString.empty()) { dog_logout(handle); return false; }

	status = dog_write(handle, 2, 0, 16, LicenseString.c_str());
	switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
		case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
		case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
		case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
		default:				ErrorInfo = string("write failed");
    }
	dog_logout(handle);
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return false;
	}
	return true;
}

//l_createtime最大为991231235959，即00000000 00000000 00000000 11100110 11001001 11111100 01010111 01110111
string BeGeneralLib::LongToString5(long l_createtime){
	union{
		unsigned long l;		// 64位系统：long占8字节
		char c[8];
	} CreateTime;
	CreateTime.l=l_createtime;
	return string(CreateTime.c);
}

long BeGeneralLib::String5ToLong(string s_createtime){
	union{
		unsigned long l;
		char c[8];
	} CreateTime;
	strcpy(CreateTime.c,s_createtime.c_str());
	CreateTime.c[5]=CreateTime.c[6]=CreateTime.c[7]=0;
	return CreateTime.l;
}

//"2016-06-06 16:19:10"(19位)-->"16-06-06 16:19:10"-->"160606161910"(12位)
long BeGeneralLib::ConvertDogCreateTime(string s_createtime){
	char from[20];
	char to[13];
	int i=0,j=0;
	strcpy(from, s_createtime.c_str());
	for(i=2; i<=18; i++){
		if((i-4)%3 == 0) i++;
		to[j++]=from[i];
	}
	to[12]=0;
	return atol(to);
}

//取得createtime在明细文件中对应的index，如果不存在则返回-1
int BeGeneralLib::GetDogIndex(string s_createtime){
	if(s_createtime.empty()) return -1;
	long l_createtime = ConvertDogCreateTime(s_createtime);
	int count = ReadDogHostCount(); if(count == -1) return -1;
	char c[6] = {0};

	//login
	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return -1;
	}

	//login succeed
	for(int i=0; i<count; i++){
		//read
		status = dog_read(handle, 3, i*5, 5, &c[0]);
		switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
		case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
		case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
		case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
		default:				ErrorInfo = string("read failed");
		}
		if (status){
			BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
			break;
		}

		//read succeed
		string output = string(c);
		if(output.empty()) continue;		//正常来说不会出现empty，如果出现则忽略该错误
		if(String5ToLong(output) == l_createtime){
			//find succeed!
			dog_logout(handle);
			return i;
		}
	}

	//haven't found
	dog_logout(handle);
	return -1;
}

//删除明细文件中index对应的createtime(明细在[index*5,index*5+4]处)
//删除之后不需要修改计数器，由task.cpp负责修改
bool BeGeneralLib::DelDogTime(int index){
	if(index<0) return false;
	int count = ReadDogHostCount(); if(count == -1) return false;
	//如果删的不是最后一个，则把最后1个复制到index处，从而实现删除index  即把[(count-1)*5,(count-1)*5+4]复制到[index*5,index*5+4]
	if(index != count-1){
		char c[6] = {0};
		string ErrorInfo = string("");
		dog_status_t status;
		dog_handle_t handle;

		//login
		status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
		switch (status){
		    case DOG_STATUS_OK:			break;
		    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
		    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
		    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
		    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
			default:                    ErrorInfo = string("login failed");
		}
		if (status){
			BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
			return false;
		}

		//login succeed, begin read
		status = dog_read(handle, 3, (count-1)*5, 5, &c[0]);
		switch (status){
			case DOG_STATUS_OK:		break;
			case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
			case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
			case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
			case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
			default:				ErrorInfo = string("read failed");
		}
		if (status){
			BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
			dog_logout(handle);
			return false;
		}

		string output = string(c); if(output.empty()) return false;

		//write
		status = dog_write(handle, 3, index*5, 5, &c[0]);
		switch (status){
			case DOG_STATUS_OK:		break;
			case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
			case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
			case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
			case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
			default:				ErrorInfo = string("write failed");
	    }

		dog_logout(handle);
		if (status){
			BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
			return false;
		}
	}

	return true;
}

//把createtime增加到明细文件中
//增加之后不需要修改计数器，由task.cpp负责修改
bool BeGeneralLib::AddDogTime(string s_createtime){
	int count = ReadDogHostCount(); if(count == -1) return false;
	string s = LongToString5(ConvertDogCreateTime(s_createtime)); if(s.empty()) return false;

	//login
	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return false;
	}

	//login succeed, begin write
	status = dog_write(handle, 3, count*5, 5, s.c_str());	//把s复制到[count*5,count*5+4]处
	switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
		case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
		case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
		case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
		default:				ErrorInfo = string("write failed");
    }

	dog_logout(handle);
	if (status){
		BesLog::DebugPrint(ErrorInfo, BesLog::LOG_MESSAGE_TYPE_ERROR, true);
		return false;
	}

	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
*/