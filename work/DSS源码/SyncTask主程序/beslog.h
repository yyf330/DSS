/*******************************************************************************
 *
 ********************* 程序文件:  BesLog                  ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-06-12              ***********************
 ********************* 完成时间:  2010-06-12              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *
 *
 *
 *
 *
 *
 *
//================================== 修改结束 ==================================*/

#ifndef _BESLOG_H
#define	_BESLOG_H

#include <iostream>
#include <string>
#include <vector>
#include <stdarg.h>
#include <syslog.h>
#include "besgeneralheader.h"


using namespace std;

class BesLog
{
public:

    enum LogType
    {
        LOG_TYPE_STDOUT = 1,
        LOG_TYPE_LOG_FILE = 2,
        LOG_TYPE_BOTH_STDOUT_AND_FILE = (LOG_TYPE_STDOUT | LOG_TYPE_LOG_FILE),
    };

    enum LogMessageType
    {
        LOG_MESSAGE_TYPE_VERBOSE = 1,
        LOG_MESSAGE_TYPE_NORMAL = 2,
        LOG_MESSAGE_TYPE_INFO = 3,
        LOG_MESSAGE_TYPE_WARN = 4,
        LOG_MESSAGE_TYPE_ERROR = 5,
    };

    enum LogFormat
    {
        LOG_FORMAT_WITH_DATETIME = (1 << 0),
        LOG_FORMAT_WITH_DATE = (1 << 1),
        LOG_FORMAT_WITH_TIME = (1 << 2),
        LOG_FORMAT_WITH_USER = (1 << 3),
        LOG_FORMAT_WITH_PID = (1 << 4),
        LOG_FORMAT_WITH_LEADING_MINUS_SIGN = (1 << 5),
        LOG_FORMAT_WITH_LEADING_PLUS_SIGN = (1 << 6),
        LOG_FORMAT_WITH_LEADING_ASTERISK = (1 << 7),
        LOG_FORMAT_WITH_LEADING_SLASH = (1 << 8),
        LOG_FORMAT_WITH_LEADING_BACKLASH = (1 << 9),
        LOG_FORMAT_WITH_LEADING_WELL = (1 << 10),
        LOG_FORMAT_WITH_LEADING_EQUAL_SIGN = (1 << 11),
        LOG_FORMAT_WITH_LEADING_DOT = (1 << 12),
    };
public:
    /*
     *  功能：
     *      构造函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    BesLog();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~BesLog();
public:

    /*
     *  功能：
     *      打印调试信息
     *  参数：
     *      msg             :   信息
     *      tolog           :   输出到日志
     *  返回：
     *      无
     */
    static void DebugPrint(string msg, bool iserror);

    /*
     *  功能：
     *      打印调试信息
     *  参数：
     *      msg             :   信息
     *      tolog           :   输出到日志
     *  返回：
     *      无
     */
    static void DebugPrint(string msg, BesLog::LogMessageType msgtype = BesLog::LOG_MESSAGE_TYPE_INFO, bool tolog = true);

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
    static void Output(string msg, BesLog::LogType type = LOG_TYPE_BOTH_STDOUT_AND_FILE, BesLog::LogFormat format = LOG_FORMAT_WITH_DATETIME, BesLog::LogMessageType msgtype = BesLog::LOG_MESSAGE_TYPE_INFO);

    //    /*
    //     *  功能：
    //     *      输出日志信息
    //     *  参数：
    //     *      format          :   可变格式（即sprintf形式）
    //     *  返回：
    //     *      无
    //     */
    //    static void Output(const char *format, ...);
    /*
     *  功能：
     *      检查日志文件
     *  参数：
     *      无
     *  返回：
     *      无
     */
    static void CheckLogFile();
private:
#define DEFAULT_LEADING_SIZE            16                      // 默认前导字符大小
#define DEFAULT_LEADING_SEPARATOR       (SEPARATOR_CHAR_SPACE)  // 默认前导符
#define DEFAULT_LOG_DIRECTORY           "log"
#define DEFAULT_LOG_FILE_EXTENSION      ".log"
#define MAX_LOG_FILE_SIZE               2097152
#define MAX_LOG_FILE_NUM                10
private:
    /*
     *  功能：
     *      发送网络日志信息
     *  参数：
     *      msg             :   信息
     *  返回：
     *      无
     */
    static void SendNetworkLog(string msg);
};

#endif	/* _BESLOG_H */

