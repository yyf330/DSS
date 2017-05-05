/*******************************************************************************
 *
 ********************* 程序文件:  CommandParser           ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-04-30              ***********************
 ********************* 完成时间:  2010-05-28              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *
 *
//================================== 修改结束 ==================================*/

#ifndef _COMMANDPARSER_H
#define	_COMMANDPARSER_H
#include <vector>
#include <iostream>
#include <sys/time.h>
#include "besgeneralheader.h"
#include "begenerallib.h"
#include "beslog.h"

using namespace std;

class CommandParser
{
public:
    // 命令类型

    enum CommandType
    {
        COMMAND_TYPE_COMMAND, // 命令
        COMMAND_TYPE_RESPONSE, // 回应
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
    CommandParser();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~CommandParser();
public:

    /*
     *  功能：
     *      解析结果
     *  参数：
     *      result          :   命令结果
     *  返回：
     *      解析成功返回true，否则返回false
     */
    virtual bool Parse(const char *command);

    /*
     *  功能：
     *      获取命令头
     *  参数：
     *      无
     *  返回：
     *      命令头
     */
    virtual string CommandHeader();

    /*
     *  功能：
     *      获取命令类型
     *  参数：
     *      无
     *  返回：
     *      命令类型
     */
    virtual CommandType Type();

    /*
     *  功能：
     *      获取命令字
     *  参数：
     *      无
     *  返回：
     *      命令字
     */
    virtual string CommandCode();

    /*
     *  功能：
     *      获取命令参数
     *  参数：
     *      无
     *  返回：
     *      命令参数
     */
    virtual string Parameter();

    /*
     *  功能：
     *      获取结果头
     *  参数：
     *      无
     *  返回：
     *      结果头
     */
    virtual string ResultHeader();

    /*
     *  功能：
     *      获取命令结果
     *  参数：
     *      无
     *  返回：
     *      命令结果
     */
    virtual string Result();

    /*
     *  功能：
     *      获取命令结果内容
     *  参数：
     *      无
     *  返回：
     *      命令结果内容
     */
    virtual string Content();

    /*
     *  功能：
     *      创建命令
     *  参数：
     *      code            :   命令字
     *      parameter       :   参数
     *  返回：
     *      命令内容
     */
    virtual string CreateCommand(string code, string parameter);

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
    virtual string CreateResult(string header, string result, string content);

private:
    string MyCmdHeader; // 命令头
    CommandType MyType; // 命令类型
    string MyCommandCode; // 命令字
    string MyParameter; // 参数
    string MyResultHeader; // 结果头
    string MyResult; // 结果
    string MyContent; // 结果内容
private:

    /*
     *  功能：
     *      初始化
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Init();

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
    virtual string CreateStandardFormat(string header, string type, string code, string parameter);

    /*
     *  功能：
     *      打印调试信息
     *  参数：
     *      msg             :   信息
     *      tolog           :   输出到日志
     *  返回：
     *      无
     */
    void DebugPrint(string msg, bool tolog = true);
    
};

#endif	/* _COMMANDPARSER_H */

