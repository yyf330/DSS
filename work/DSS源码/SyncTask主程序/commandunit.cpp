/* 
 * File:   CommandUnit.cpp
 * Author: root
 * 
 * Created on 2015年3月12日, 上午10:20
 */

#include "commandunit.h"

/*------------------------------------ 公有方法 --------------------------------*/

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
CommandUnit::CommandUnit(string cmd, string paradesc, string chnname, string engname)
{
    MyCommand = cmd;
    MyParameterDescription = paradesc;
    MyParameterCount =ParseParameterCount(paradesc);
    MyNameList.insert(pair<DescriptionLang, string > (DESCRIPTION_LANG_CN, chnname));
    MyNameList.insert(pair<DescriptionLang, string > (DESCRIPTION_LANG_EN, engname));
}

/*
 *  功能：
 *      析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
CommandUnit::~CommandUnit()
{
}

/*------------------------------------ 公有方法 --------------------------------*/

/*
 *  功能：
 *      获取命令字
 *  参数：
 *      无
 *  返回：
 *      命令字
 */
string CommandUnit::Command()
{
    return MyCommand;
}

/*
 *  功能：
 *      获取命令代码
 *  参数：
 *      无
 *  返回：
 *      命令代码
 */
int CommandUnit::Code()
{
    return MyCode;
}

/*
 *  功能：
 *      获取参数个数
 *  参数：
 *      无
 *  返回：
 *      参数个数
 */
int CommandUnit::ParameterCount()
{
    return MyParameterCount;
}

/*
 *  功能：
 *      获取参数描述
 *  参数：
 *      无
 *  返回：
 *      参数描述
 */
string CommandUnit::ParameterDescription()
{
    return MyParameterDescription;
}

/*
 *  功能：
 *      获取命令名称
 *  参数：
 *      无
 *  返回：
 *      命令名称
 */
string CommandUnit::Name()
{
    return Name(BeGeneralLib::GetDescriptionLang());
}

/*
 *  功能：
 *      获取指定语言对应的命令名称
 *  参数：
 *      lang
 *  返回：
 *      命令名称
 */
string CommandUnit::Name(DescriptionLang lang)
{
    return MyNameList[lang];
}

/*
 *  功能：
 *      设置命令名称
 *  参数：
 *      lang            :   语言
 *      name            :   命令名
 *  返回：
 *      无
 */
void CommandUnit::Name(DescriptionLang lang, string name)
{
    //MyNameList.find(lang)
}

/*
 *  功能：
 *      显示命令信息
 *  参数：
 *      无
 *  返回：
 *      无
 */
void CommandUnit::Show()
{
    cout << Name(DESCRIPTION_LANG_CN) + string(" (") + MyCommand + string(")[参数:") + string("") + MyParameterDescription + string("]") << endl;
}

/*------------------------------------ 私有方法 --------------------------------*/

/*
 *  功能：
 *      解析参数个数
 *  参数：
 *      paradesc        :   参数描述
 *  返回：
 *      参数个数
 */
int CommandUnit::ParseParameterCount(string paradesc)
{
    vector<string> ParameterArray = BeGeneralLib::StringSplit(BeGeneralLib::StringTrim(paradesc, COMMAND_PARAMETER_SEPCHAR), COMMAND_PARAMETER_SEPCHAR, BeGeneralLib::REMOVE_EMPTY_ITEM);
    for (int i = 0; i < ParameterArray.size(); i++)
    {
        if (ParameterArray[i] == "...") // 如果包含...，则标识为可变参数
        {
            return DEFAULT_VARIABLE_COMMAND_PARAMETER_COUNT;
        }
    }

    return ParameterArray.size();
}