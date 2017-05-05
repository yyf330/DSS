/* 
 * File:   beslog.cpp
 * Author: root
 * 
 * Created on 2010年6月12日, 上午10:09
 */
#include "beslog.h"
#include "begenerallib.h"
extern int ZwgDebugLevel;

//---------------------------------- 公有方法 ----------------------------------//

/*
 *  功能：
 *      构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
BesLog::BesLog()
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
BesLog::~BesLog()
{
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
void BesLog::DebugPrint(string msg, bool iserror)
{
    BesLog::LogMessageType MsgType = BesLog::LOG_MESSAGE_TYPE_INFO;
    if (iserror == true)
    {
        MsgType = BesLog::LOG_MESSAGE_TYPE_ERROR;
    }

    BesLog::Output(msg, BesLog::LOG_TYPE_BOTH_STDOUT_AND_FILE, BesLog::LOG_FORMAT_WITH_DATETIME, MsgType);
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
void BesLog::DebugPrint(string msg, BesLog::LogMessageType msgtype, bool tolog)
{
    BesLog::LogType LogMode = BesLog::LOG_TYPE_STDOUT;
    if (tolog)
    {
        LogMode = BesLog::LOG_TYPE_BOTH_STDOUT_AND_FILE;
    }

    BesLog::Output(msg, LogMode, BesLog::LOG_FORMAT_WITH_DATETIME, msgtype);
}

/*
 *  功能：
 *      输出日志信息
 *  参数：
 *      msg             :   信息
 *      type            :   日志类型
 *      format          :   日志格式
 *  返回：
 *      无
 */
void BesLog::Output(string msg, BesLog::LogType type, BesLog::LogFormat format, BesLog::LogMessageType msgtype)
{
    bool IsErrorMessage = false;
    if (BeGeneralLib::IsStringEmpty(msg) == true || (msgtype < (BesLog::LogMessageType)ZwgDebugLevel))
    {
        return;
    }

    bool WithLeadingCharFlag = false;
    string FileMsg = msg;

    switch (msgtype)
    {
        case BesLog::LOG_MESSAGE_TYPE_VERBOSE:
        case BesLog::LOG_MESSAGE_TYPE_NORMAL:
        case BesLog::LOG_MESSAGE_TYPE_INFO:
        {
            FileMsg = string("<I>") + SEPARATOR_CHAR_SPACE + FileMsg;
            break;
        }
        case BesLog::LOG_MESSAGE_TYPE_WARN:
        {
            FileMsg = string("<W>") + SEPARATOR_CHAR_SPACE + FileMsg;
            break;
        }
        case BesLog::LOG_MESSAGE_TYPE_ERROR:
        {
            IsErrorMessage = true;
            FileMsg = string("<E>") + SEPARATOR_CHAR_SPACE + FileMsg;
            break;
        }
    }

    // 解析格式
    string ConsoleMsg = FileMsg;
    if ((format & LOG_FORMAT_WITH_DATETIME) == LOG_FORMAT_WITH_DATETIME) // 日期
    {
        ConsoleMsg = string("[") + BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_LONG) + string("]") + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
    }
    if ((format & LOG_FORMAT_WITH_DATE) == LOG_FORMAT_WITH_DATE) // 日期
    {
        ConsoleMsg = BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_DATE) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
    }
    if ((format & LOG_FORMAT_WITH_TIME) == LOG_FORMAT_WITH_TIME) // 时间
    {
        ConsoleMsg = BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_TIME) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
    }
    if ((format & LOG_FORMAT_WITH_USER) == LOG_FORMAT_WITH_USER) // 用户
    {

    }
    if ((format & LOG_FORMAT_WITH_PID) == LOG_FORMAT_WITH_PID) // 进程号
    {
        ConsoleMsg = BeGeneralLib::Number2String(getpid()) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
    }

    if ((format & LOG_FORMAT_WITH_LEADING_MINUS_SIGN) == LOG_FORMAT_WITH_LEADING_MINUS_SIGN) // 减号
    {
        if (!WithLeadingCharFlag)
        {
            ConsoleMsg = string(DEFAULT_LEADING_SIZE, SEPARATOR_CHAR_MINUS_SIGN) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
        }
        WithLeadingCharFlag = true;
    }
    else if ((format & LOG_FORMAT_WITH_LEADING_PLUS_SIGN) == LOG_FORMAT_WITH_LEADING_PLUS_SIGN) // 加号
    {
        if (!WithLeadingCharFlag)
        {
            ConsoleMsg = string(DEFAULT_LEADING_SIZE, SEPARATOR_CHAR_PLUS_SIGN) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
        }
        WithLeadingCharFlag = true;
    }
    else if ((format & LOG_FORMAT_WITH_LEADING_ASTERISK) == LOG_FORMAT_WITH_LEADING_ASTERISK) // 星号
    {
        if (!WithLeadingCharFlag)
        {
            ConsoleMsg = string(DEFAULT_LEADING_SIZE, SEPARATOR_CHAR_ASTERISK) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
            WithLeadingCharFlag = true;
        }
    }
    else if ((format & LOG_FORMAT_WITH_LEADING_SLASH) == LOG_FORMAT_WITH_LEADING_SLASH) // 斜线
    {
        if (!WithLeadingCharFlag)
        {
            ConsoleMsg = string(DEFAULT_LEADING_SIZE, SEPARATOR_CHAR_SLASH) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
        }
        WithLeadingCharFlag = true;
    }
    else if ((format & LOG_FORMAT_WITH_LEADING_BACKLASH) == LOG_FORMAT_WITH_LEADING_BACKLASH) // 反斜线
    {
        if (!WithLeadingCharFlag)
        {
            ConsoleMsg = string(DEFAULT_LEADING_SIZE, SEPARATOR_CHAR_BACKSLASH) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
        }
        WithLeadingCharFlag = true;
    }
    else if ((format & LOG_FORMAT_WITH_LEADING_WELL) == LOG_FORMAT_WITH_LEADING_WELL) // 井号
    {
        if (!WithLeadingCharFlag)
        {
            ConsoleMsg = string(DEFAULT_LEADING_SIZE, SEPARATOR_CHAR_WELL) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
        }
        WithLeadingCharFlag = true;
    }
    else if ((format & LOG_FORMAT_WITH_LEADING_EQUAL_SIGN) == LOG_FORMAT_WITH_LEADING_EQUAL_SIGN) // 等号
    {
        if (!WithLeadingCharFlag)
        {
            ConsoleMsg = string(DEFAULT_LEADING_SIZE, SEPARATOR_CHAR_EQUAL_SIGN) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
        }
        WithLeadingCharFlag = true;
    }
    else if ((format & LOG_FORMAT_WITH_LEADING_DOT) == LOG_FORMAT_WITH_LEADING_DOT) // 点
    {
        if (!WithLeadingCharFlag)
        {
            ConsoleMsg = string(DEFAULT_LEADING_SIZE, SEPARATOR_CHAR_DOT) + DEFAULT_LEADING_SEPARATOR + ConsoleMsg;
        }
        WithLeadingCharFlag = true;
    }

    // 发送网络日志
    SendNetworkLog(ConsoleMsg);

    // 解析日志类型
    if ((type & LOG_TYPE_STDOUT) == LOG_TYPE_STDOUT)
    {
        if (IsErrorMessage == true)
        {
            cout << "\33[31m";
        }
        if (ConsoleMsg[ConsoleMsg.size() - 1] != '\n')
        {
            cout << ConsoleMsg << endl;
        }
        else
        {
            cout << ConsoleMsg;
        }
        if (IsErrorMessage == true)
        {
            cout << "\33[0m";
        }
    }
    if ((type & LOG_TYPE_LOG_FILE) == LOG_TYPE_LOG_FILE)
    {
        // 设置日志目录
        string LogPath = BeGeneralLib::GetExecutePath() + DEFAULT_LOG_DIRECTORY + LINUX_PATH_SEPCHAR;
        BeGeneralLib::CheckAndCreateDir(LogPath);

        // 日志文件名
        string LogFileName = LogPath + BeGeneralLib::GetApplicationName() + DEFAULT_LOG_FILE_EXTENSION;

        // 打开文件
        FILE *fp;
        if ((fp = fopen(LogFileName.c_str(), "a+")) == NULL)
        {
            return;
        }

        FileMsg = string("[") + BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_LONG) + string("]") + SEPARATOR_CHAR_SPACE + FileMsg;
        if (FileMsg[FileMsg.size() - 1] != '\n')
        {
            FileMsg = FileMsg + "\n";
        }
        BeGeneralLib::WriteDataToFile(fp, FileMsg);
        fclose(fp);
    }
}

/*
 *  功能：
 *      检查日志文件
 *  参数：
 *      无
 *  返回：
 *      无
 */
void BesLog::CheckLogFile()
{
    // 设置日志目录
    string LogPath = BeGeneralLib::GetExecutePath() + DEFAULT_LOG_DIRECTORY + LINUX_PATH_SEPCHAR;

    string LogFileName = LogPath + BeGeneralLib::GetApplicationName() + DEFAULT_LOG_FILE_EXTENSION;

    // 检查log文件是否存在，并且检查log文件是否已经超过最大限制
    if (BeGeneralLib::IsFileExist(LogFileName) == false || (BeGeneralLib::GetFileSize(LogFileName) <= MAX_LOG_FILE_SIZE))
    {
        return;
    }

    // 备份log文件
    string BackupLogFileName = LogPath + BeGeneralLib::GetApplicationName() + SEPARATOR_CHAR_UNDERLINE + BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_FULL_DATETIME) + DEFAULT_LOG_FILE_EXTENSION;
    BeGeneralLib::MoveFile(LogFileName, BackupLogFileName, true);

    // 获取当前目录下备份的文件数
    string ShellCommand = string("find") + SEPARATOR_CHAR_SPACE + LogPath + SEPARATOR_CHAR_SPACE + string("-maxdepth 1 | grep") + SEPARATOR_CHAR_SPACE + BeGeneralLib::GetApplicationName() + SEPARATOR_CHAR_UNDERLINE + SEPARATOR_CHAR_SPACE + string("| wc -l");
    int TotalLogNum = BeGeneralLib::StringToInt(BeGeneralLib::ReadShellReturnValue(ShellCommand));
    if (TotalLogNum <= MAX_LOG_FILE_NUM)
    {
        return;
    }

    // 删除多余的备份文件
    int Difference = TotalLogNum - MAX_LOG_FILE_NUM;
    ShellCommand = string("find") + SEPARATOR_CHAR_SPACE + LogPath + SEPARATOR_CHAR_SPACE + string("-maxdepth 1 | grep") + SEPARATOR_CHAR_SPACE + BeGeneralLib::GetApplicationName() + SEPARATOR_CHAR_UNDERLINE + SEPARATOR_CHAR_SPACE + string("| sort | head -") + BeGeneralLib::Number2String(Difference)+(" | xargs rm -f");
    BeGeneralLib::ExecuteSystem(ShellCommand, false, BesLog::LOG_MESSAGE_TYPE_NORMAL);
}

///*
// *  功能：
// *      输出日志信息
// *  参数：
// *      format          :   可变格式（即sprintf形式）
// *  返回：
// *      无
// */
//void BesLog::Output(const char* format, ...)
//{
//    char Buf[DEFAULT_BUFFER_SIZE + 1];
//    bzero(Buf, sizeof (Buf));
//    va_list Args;
//    va_start(Args, format);
//    //vsnprintf(Buf, INT_MAX, format, Args);
//    vsprintf(Buf, format, Args);
//    va_end(Args);
//
//    Output(string(Buf));
//}

//---------------------------------- 私有方法 ----------------------------------//

/*
 *  功能：
 *      发送网络日志信息
 */
void BesLog::SendNetworkLog(string msg)
{
    string AppName = BeGeneralLib::GetApplicationName();
    int NetworkPort = 0;
    if (AppName == DEFAULT_SW_SERVER_BETASKEXEC)
    {
        NetworkPort = DEFAULT_LOG_UDP_PORT_BETASKEXEC;
    }
    else if (AppName == DEFAULT_SW_SERVER_BEDAEMON)
    {
        NetworkPort = DEFAULT_LOG_UDP_PORT_BEDAEMON;
    }
    else if (AppName == DEFAULT_SW_SERVER_RDRTASKEXEC)
    {
        NetworkPort = DEFAULT_LOG_UDP_PORT_RDRTASKEXEC;
    }
    else if (AppName == DEFAULT_SW_SERVER_DRHDAEON)
    {
        NetworkPort = DEFAULT_LOG_UDP_PORT_DRHDAEMON;
    }
    else
    {
        return;
    }

    // 发送UDP数据包
    BeGeneralLib::SendUdpData(DEFAULT_LOCALHOST_IP, NetworkPort, BeGeneralLib::StringTrim(msg, '\n'));
}