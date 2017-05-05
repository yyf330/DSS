/* 
 * File:   commandparser.cpp
 * Author: root
 * 
 * Created on 2010年4月30日, 上午10:01
 */

#include "commandparser.h"

//------------------------------------ 公有方法 --------------------------------//

/*
 *  功能：
 *      构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
CommandParser::CommandParser()
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
CommandParser::~CommandParser()
{
}

/*
 *  功能：
 *      解析结果
 *  参数：
 *      result          :   命令结果
 *  返回：
 *      解析成功返回true，否则返回false
 */
bool CommandParser::Parse(const char *result)
{

    string RpString = result;

    //    DebugPrint("RpString:" + RpString);

    vector<string> RpSubArray;
    string::size_type Pos, LastPos;
    Pos = RpString.find(COMMAND_SEGMENT_SEPCHAR);
    LastPos = 0;
    while (Pos < RpString.size())
    {
        RpSubArray.push_back(RpString.substr(LastPos, Pos - LastPos));
        LastPos = Pos + 1;
        Pos = RpString.find(COMMAND_SEGMENT_SEPCHAR, LastPos);
    }
    if (LastPos != RpString.size())
        RpSubArray.push_back(RpString.substr(LastPos));


    if (RpSubArray.size() != COMMAND_SEGMENT_NUM)
    {
        DebugPrint("Rp Segment is invalid!");
        return false;
    }

    // 处理结果头
    if (RpSubArray[1] == COMMAND_CODE_ID)
    {
        //        DebugPrint("Rp is a command!");
        MyCmdHeader = RpSubArray[0];

        MyType = COMMAND_TYPE_COMMAND;

        // 命令字或应答
        MyCommandCode = RpSubArray[2];

        // 参数或应答内容
        MyParameter = RpSubArray[3];

    }
    else if (RpSubArray[1] == COMMAND_RESPONSE_ID)
    {
        //        DebugPrint("Rp is a response!");
        MyResultHeader = RpSubArray[0];

        MyType = COMMAND_TYPE_RESPONSE;

        // 命令字或应答
        MyResult = RpSubArray[2];

        // 参数或应答内容
        MyContent = RpSubArray[3];

    }
    else
    {
        DebugPrint("Rp is unknown!");
        return false;

    }

    // 结束符
    if (RpSubArray[4] != "\r\n")
    {
        DebugPrint("Rp tailor is invalid!");
        return false;
    }
    return true;
}

/*
 *  功能：
 *      获取命令头
 *  参数：
 *      无
 *  返回：
 *      命令头
 */
string CommandParser::CommandHeader()
{
    return MyCmdHeader;
}

/*
 *  功能：
 *      获取命令类型
 *  参数：
 *      无
 *  返回：
 *      命令类型
 */
CommandParser::CommandType CommandParser::Type()
{
    return MyType;
}

/*
 *  功能：
 *      获取命令字
 *  参数：
 *      无
 *  返回：
 *      命令字
 */
string CommandParser::CommandCode()
{
    return MyCommandCode;
}

/*
 *  功能：
 *      获取命令参数
 *  参数：
 *      无
 *  返回：
 *      命令参数
 */
string CommandParser::Parameter()
{
    return MyParameter;
}

/*
 *  功能：
 *      获取结果头
 *  参数：
 *      无
 *  返回：
 *      结果头
 */
string CommandParser::ResultHeader()
{
    return MyResultHeader;
}

/*
 *  功能：
 *      获取命令结果
 *  参数：
 *      无
 *  返回：
 *      命令结果
 */
string CommandParser::Result()
{
    return MyResult;
}

/*
 *  功能：
 *      获取命令结果内容
 *  参数：
 *      无
 *  返回：
 *      命令结果内容
 */
string CommandParser::Content()
{
    return MyContent;
}

/*
 *  功能：
 *      创建命令
 *  参数：
 *      code            :   命令字
 *      parameter       :   参数
 *  返回：
 *      命令内容
 */
string CommandParser::CreateCommand(string code, string parameter)
{
    char Header[15];
    struct timeval CmdTime;
    gettimeofday(&CmdTime, NULL);
    struct tm * NowTime;
    NowTime = localtime(&(CmdTime.tv_sec));
    //cout << CmdTime.tv_sec << "\t" << CmdTime.tv_usec << endl;
    sprintf(Header, "%02d%02d%02d%02d%06ld\0", NowTime->tm_mday, NowTime->tm_hour, NowTime->tm_min, NowTime->tm_sec, CmdTime.tv_usec);

    MyCmdHeader = string(Header);
    MyType = COMMAND_TYPE_COMMAND;
    MyCommandCode = code;
    MyParameter = parameter;
    return CreateStandardFormat(string(Header), COMMAND_CODE_ID, code, parameter);
}

/*
 *  功能：
 *      创建命令回应
 *  参数：
 *      header          :   回应头
 *      result          :   结果
 *      content         :   结果内容
 *  返回：
 *      命令回应内容
 */
string CommandParser::CreateResult(string header, string result, string content)
{
    MyResultHeader = header;
    MyType = COMMAND_TYPE_RESPONSE;
    MyResult = result;
    MyContent = content;
    return CreateStandardFormat(header, COMMAND_RESPONSE_ID, result, content);
}

/*
 *  功能：
 *      初始化
 *  参数：
 *      无
 *  返回：
 *      无
 */
void CommandParser::Init()
{

}

/*
 *  功能：
 *      创建标准格式命令
 *  参数：
 *      header          :   回应头
 *      type            :   类型
 *      code            :   命令字
 *      parameter       :   参数
 *  返回：
 *      命令内容
 */
string CommandParser::CreateStandardFormat(string header, string type, string code, string parameter)
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
 *      打印调试信息
 *  参数：
 *      msg             :   信息
 *      tolog           :   输出到日志
 *  返回：
 *      无
 */
void CommandParser::DebugPrint(string msg, bool tolog)
{
//    cout << "+++++++++++++:" + msg << endl;
    if (tolog)
    {
        BesLog::Output(msg, BesLog::LOG_TYPE_BOTH_STDOUT_AND_FILE, BesLog::LOG_FORMAT_WITH_LEADING_PLUS_SIGN);
    }
    else
    {
        BesLog::Output(msg, BesLog::LOG_TYPE_STDOUT, BesLog::LOG_FORMAT_WITH_LEADING_PLUS_SIGN);
    }
}
