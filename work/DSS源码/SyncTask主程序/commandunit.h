/* 
 * File:   commandunit.h
 * Author: root
 *
 * Created on 2015年3月12日, 上午10:20
 */

#ifndef COMMANDUNIT_H
#define	COMMANDUNIT_H

#include "besgeneralheader.h"
#include "begenerallib.h"

class CommandUnit
{
public:
    /*
     *  功能：
     *      构造函数
     *  参数：
     *      cmd             :   命令字
     *      paracount       :   参数个数
     *      chnname         :   中文名称
     *      engname         :   英文名称
     *  返回：
     *      无
     */
    CommandUnit(string cmd, string paradesc, string chnname, string engname);

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~CommandUnit();
public:

    /*
     *  功能：
     *      获取命令字
     *  参数：
     *      无
     *  返回：
     *      命令字
     */
    virtual string Command();

    /*
     *  功能：
     *      获取命令代码
     *  参数：
     *      无
     *  返回：
     *      命令代码
     */
    virtual int Code();

    /*
     *  功能：
     *      获取参数个数
     *  参数：
     *      无
     *  返回：
     *      参数个数
     */
    virtual int ParameterCount();

    /*
     *  功能：
     *      获取参数描述
     *  参数：
     *      无
     *  返回：
     *      参数描述
     */
    virtual string ParameterDescription();

    /*
     *  功能：
     *      获取命令名称
     *  参数：
     *      无
     *  返回：
     *      命令名称
     */
    virtual string Name();

    /*
     *  功能：
     *      获取指定语言对应的命令名称
     *  参数：
     *      lang
     *  返回：
     *      命令名称
     */
    virtual string Name(DescriptionLang lang);

    /*
     *  功能：
     *      设置命令名称
     *  参数：
     *      lang            :   语言
     *      name            :   命令名
     *  返回：
     *      无
     */
    virtual void Name(DescriptionLang lang, string name);

    /*
     *  功能：
     *      显示命令信息
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Show();
public:
#define     DEFAULT_VARIABLE_COMMAND_PARAMETER_COUNT            9999                // 可变参数个数
private:
    string MyCommand;
    int MyCode;
    int MyParameterCount;
    string MyParameterDescription;
    map<DescriptionLang, string> MyNameList;
private:
#define COMMAND_PARAMETER_SEPCHAR                               (SEPARATOR_CHAR_SEMICOLON)  // 参数分割符
private:

    /*
     *  功能：
     *      解析参数个数
     *  参数：
     *      paradesc        :   参数描述
     *  返回：
     *      参数个数
     */
    virtual int ParseParameterCount(string paradesc);
};

#endif	/* COMMANDUNIT_H */

