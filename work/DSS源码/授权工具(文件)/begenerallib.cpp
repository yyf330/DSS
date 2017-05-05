/* 
 * File:   begenerallib.cpp
 * Author: root
 * 
 * Created on 2010年6月28日, 上午9:26
 */

#include <string>

#include "begenerallib.h"

/*------------------------------------ 公有方法 --------------------------------*/

/*
 *  功能：
 *      构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
BeGeneralLib::BeGeneralLib()
{
}

/*
 *  功能：
 *      析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
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
    if (str.empty())
    {
        return "";
    }

    string ResultStr = "";
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
    if (str.empty())
    {
        return "";
    }

    string ResultStr = "";
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
    if (str.empty())
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
    if (str.empty())
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
    string DestStr = "";
    if (str.empty())
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
    return (str.empty() == true || str == DEFAULT_INVALID_RETURN_VALUE);
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
 *      读取shell返回值
 *  参数
 *      shellcommand    :   shell命令
 *  返回：
 *      shell返回值
 */
string BeGeneralLib::ReadShellReturnValue(string shellcommand)
{
    vector<string> ValueList = ReadShellReturnValueAll(shellcommand);
    if (ValueList.size() == 0)
    {
        return "";
    }
    else
    {
        return ValueList[0];
    }
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
    vector<string> ValueList;
    FILE *fp;
    if ((fp = popen(shellcommand.c_str(), "r")) == NULL)
    {
        return ValueList;
    }
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    while (!feof(fp))
    {
        bzero(Buf, sizeof (Buf));
        fgets(Buf, sizeof (Buf), fp);
        Buf[strlen(Buf) - 1] = DEFAULT_C_STRING_END_FLAG;
        if (!string(Buf).empty())
        {
            ValueList.push_back(string(Buf));
        }
    }
    pclose(fp);
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
    return atoi(ReadShellReturnValue(ShellCommand).c_str());
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
    string ShellCommand = string("sed -i") + SEPARATOR_CHAR_SPACE + string("/^") + key + separator + string("/c\\") + key + separator + value + SEPARATOR_CHAR_SPACE + filename;
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
 *      redirect        :   重定向
 *      debug           :   调试标志
 *  返回：
 *      无
 */
bool BeGeneralLib::ExecuteSystem(string shellcommand, bool redirect, bool debug)
{
    string CommandStr = shellcommand;

    // 重定向
    if (redirect)
    {
        CommandStr = CommandStr + SEPARATOR_CHAR_SPACE + string("&> /dev/null");
    }

    if (debug)
    {
        DebugPrint("SCommand:" + CommandStr);
    }

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
    string FileName = string("/etc/init.d/") + servicename;
    return IsFileExist(FileName);
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
    string ShellCommand = "";
    string ActionString = "";
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
    DebugPrint("ECommand:" + procstring);

    // 分解执行参数
    vector<string> ParaArray = StringSplit(procstring, SEPARATOR_CHAR_SPACE, BeGeneralLib::REMOVE_EMPTY_ITEM);
    if (ParaArray.size() == 0)
    {
        return false;
    }

    // 进程路径
    string ProcPath = ParaArray[0];
    if (access(ProcPath.c_str(), F_OK) != 0)
        return false;

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
            DebugPrint("The fork failed!");
            Flag = false;
            break;
        }
        case 0:
        {
            // pid为0，子进程
            if (execvp(ProcPath.c_str(), Argv) < 0)
            {
                DebugPrint("Fork process failed!");
            }
            // 但是做完后, 我们需要做类似下面：
            _exit(0);
            break;
        }
        default:
        {
            // pid大于0，为父进程得到的子进程号
            DebugPrint("Child process id:" + Number2String(ProcessID));
            Flag = true;
            break;
        }
    }
    return Flag;
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
    if (IsFileExist(path) == true)
    {
        return;
    }

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
 *  返回：
 *      无
 */
void BeGeneralLib::ForceRemoveDirectory(string path)
{
    string ShellCommand = string("rm -rf") + SEPARATOR_CHAR_SPACE + path;
    BeGeneralLib::ExecuteSystem(ShellCommand);
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
    return (atoi(BeGeneralLib::ReadShellReturnValue(ShellCommand).c_str()) - 1);
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
    if (file.empty())
    {
        return "";
    }
    string DirName = "";
    if (access(file.c_str(), F_OK) == 0)
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
        case BeGeneralLib::TIME_FORMAT_COMMAND_HEADER:
        {
            FormatString = string("%d%H%M%S");
            break;
        }
    }

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
    string FormatString = string("%Y%m%d%H%M%S");

    // 获取本地时间
    struct tm ConvertTime;

    // 将字符串结构的时间转换为tm结构的时间
    strptime(timestring.c_str(), FormatString.c_str(), &ConvertTime);

    // 转换为
    time_t NewTime = mktime(&ConvertTime);
    stime(&NewTime);
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
    return ReadShellReturnValue(ShellCommand);
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
string BeGeneralLib::CreateStandardCommand(string type, string code, string parameter)
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
string BeGeneralLib::CreateStandardCommand(string header, string type, string code, string parameter)
{
    string StandardStr = header;
    //    StandardStr += COMMAND_SEGMENT_SEPCHAR;
    //    StandardStr += type;
    //    StandardStr += COMMAND_SEGMENT_SEPCHAR;
    //    StandardStr += code;
    //    StandardStr += COMMAND_SEGMENT_SEPCHAR;
    //    StandardStr += parameter;
    //    StandardStr += COMMAND_SEGMENT_SEPCHAR;
    //    StandardStr += COMMAND_TAILOR;
    return StandardStr;
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
    if (str.empty())
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
    if (str.empty())
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
        int SegmentValue = atoi(IpSegmentArray[i].c_str());
        if (SegmentValue < MIN_IP_SEGMENT_VALUE || SegmentValue > MAX_IP_SEGMENT_VALUE)
        {
            return false;
        }
    }

    return true;
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
    return (find(list.begin(), list.end(), value) != list.end());
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
    if (access(configfile.c_str(), F_OK) == 0)
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
    if (mountpoint.empty() == true)
    {
        return IsDeviceMounted(device);
    }

    // 检查是否挂载
    string ShellCommand = string("cat /proc/mounts | gawk '{if($2==\"") + mountpoint + string("\") print $1}'");
    return (device == BeGeneralLib::ReadShellReturnValue(ShellCommand));
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
        return false;
    }

    // 检查挂载点是否存在
    if (mountpoint.empty() == true || (createmountpoint == false && BeGeneralLib::IsFileExist(mountpoint) == false))
    {
        return false;
    }

    // 创建mount目录
    BeGeneralLib::CheckAndCreateDir(mountpoint);

    // 设置文件系统
    string FileSystemName = GetFileSystemName(filesystem);

    bool ProcFlag = false;
    time_t StartTime;
    time(&StartTime);
    while (true)
    {
        // 生成挂载指令
        string ShellCommand = string("mount -t") + SEPARATOR_CHAR_SPACE + FileSystemName + SEPARATOR_CHAR_SPACE + device + SEPARATOR_CHAR_SPACE + mountpoint;
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
        if (difftime(NowTime, StartTime) > 30)
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
    if (mountpoint.empty() == false)
    {
        if (BeGeneralLib::IsDeviceMounted(device, mountpoint) == false)
        {
            return true;
        }
    }

    bool ProcFlag = false;
    time_t StartTime;
    time(&StartTime);
    while (true)
    {
        // 卸载设备
        string ShellCommand = "";
        if (mountpoint.empty() == false)
        {
            ShellCommand = string("umount") + SEPARATOR_CHAR_SPACE + mountpoint;
        }
        else
        {
            ShellCommand = string("umount") + SEPARATOR_CHAR_SPACE + device;
        }
        BeGeneralLib::ExecuteSystem(ShellCommand);

        sleep(5);

        // 检查磁盘是否已经卸载
        if (BeGeneralLib::IsDeviceMounted(device, mountpoint) == false)
        {
            // 删除挂载目录
            if (removemountpoint && mountpoint.empty() == false)
            {
                BeGeneralLib::ForceRemoveDirectory(mountpoint);
            }

            ProcFlag = true;
            break;
        }

        // 检查超时
        time_t NowTime;
        time(&NowTime);
        if (difftime(NowTime, StartTime) > 30)
        {
            ProcFlag = false;
            break;
        }
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
    string FileSystemString = "";

    // 检查设备是否存在
    if (IsDeviceExist(device) == false)
    {
        return FileSystemString;
    }

    // 检查是否挂载
    string ShellCommand = string("cat /proc/mounts | gawk '{if($1==\"") + device + string("\") print $3}'");
    FileSystemString = BeGeneralLib::ReadShellReturnValue(ShellCommand);

    if (FileSystemString.empty() == true)
    {
        ShellCommand = string("blkid -o export") + SEPARATOR_CHAR_SPACE + device + string("| gawk -F= '{if($1==\"TYPE\") print $2}'");
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
        case BASE_FILESYSTEM_NILFS2:
        {
            FileSystemName = DEFAULT_FILESYSTEM_TYPE_NILFS2;
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
 *      释放指定的loop设备
 *  参数：
 *      loopdevice      :   loop设备名
 *  返回：
 *      无
 */
void BeGeneralLib::FreeLoopDevice(string loopdevice)
{
    if (IsStringEmptyOrInvalidValue(loopdevice) == true)
    {
        return;
    }

    // 释放/dev/loop设备
    string ShellCommand = string("losetup -d") + SEPARATOR_CHAR_SPACE + loopdevice;
    BeGeneralLib::ExecuteSystem(ShellCommand);
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
    if (path.empty())
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
    if (path.empty())
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
 *     获取指定文件大小
 *  参数：
 *      filename        :   文件名
 *  返回：
 *      文件大小
 */
long int BeGeneralLib::GetFileSize(string filename)
{
    // 检查文件是否存在
    if (IsFileExist(filename) == false)
    {
        return 0;
    }

    struct stat buf;
    if (stat(filename.c_str(), &buf) < 0)
    {
        return 0;
    }
    return (long) buf.st_size;
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
 *      打印调试信息
 *  参数：
 *      msg             :   信息
 *      tolog           :   输出到日志
 *  返回：
 *      无
 */
void BeGeneralLib::DebugPrint(string msg, bool iserror)
{
    //    BesLog::LogMessageType MsgType = BesLog::LOG_MESSAGE_TYPE_INFO;
    //    if (iserror == true)
    //    {
    //        MsgType = BesLog::LOG_MESSAGE_TYPE_ERROR;
    //    }
    //
    //    BesLog::Output(msg, BesLog::LOG_TYPE_BOTH_STDOUT_AND_FILE, BesLog::LOG_FORMAT_WITH_DATETIME, MsgType);
    cout << msg << endl;
}