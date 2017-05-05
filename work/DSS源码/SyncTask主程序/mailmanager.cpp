/* 
 * File:   mailmanager.cpp
 * Author: root
 * 
 * Created on 2013年12月24日, 下午2:39
 */

#include "mailmanager.h"

/*
 *  功能：
 *      构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
MailManager::MailManager()
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
MailManager::~MailManager()
{
}

/*------------------------------------ 公有方法 --------------------------------*/

/*
 *  功能：
 *      发送邮件
 *  参数
 *      subject         :   主题
 *      conent          :   内容
 *      address         :   邮件地址
 *  返回：
 *      发送成功返回true，否则返回false
 */
bool MailManager::SendMail(string subject, string content, string address)
{
    vector<string> AddressList;
    AddressList.push_back(address);

    return SendMail(subject, content, AddressList);
}

/*
 *  功能：
 *      发送邮件
 *  参数
 *      subject         :   主题
 *      conent          :   内容
 *      addresslist     :   邮件地址列表
 *  返回：
 *      发送成功返回true，否则返回false
 */
bool MailManager::SendMail(string subject, string content, vector<string> addresslist)
{
    // 检查主题、内容、邮箱是否为空
    if (BeGeneralLib::IsStringEmpty(subject) == true
            || BeGeneralLib::IsStringEmpty(content) == true
            || addresslist.empty() == true)
    {
        return false;
    }

    // 拼接邮件地址
    string AddressString = CombineAddress(addresslist);
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(AddressString) == true)
    {
        return false;
    }

    // 文件名
    string FileName = string(DEFAULT_TEMP_PATH) + DEFAULT_MAIL_CONTENT_FILE_PREFIX + BeGeneralLib::GetTimeString(BeGeneralLib::TIME_FORMAT_COMMAND_HEADER);
    FILE *fp = fopen(FileName.c_str(), "w+");
    if (fp == NULL)
    {
        return false;
    }

    // 写入内容
    BeGeneralLib::WriteDataToFile(fp, content);

    // 关闭文件
    fclose(fp);

    // 发送邮件
    string ShellCommand = string("mail -s") + SEPARATOR_CHAR_SPACE + subject + SEPARATOR_CHAR_SPACE + AddressString + SEPARATOR_CHAR_SPACE + string("<") + SEPARATOR_CHAR_SPACE + FileName;
    BeGeneralLib::ExecuteSystem(ShellCommand);

    // 删除文件
    BeGeneralLib::ForceRemoveDirectory(FileName, false);

    return true;
}

/*------------------------------------ 私有方法 --------------------------------*/

/*
 *  功能：
 *      拼接邮件地址
 *  参数
 *      addresslist     :   邮件地址列表
 *  返回：
 *      邮件地址字符串
 */
string MailManager::CombineAddress(vector<string> addresslist)
{
    return CombineAddress(addresslist, SEPARATOR_CHAR_COMMA);
}

/*
 *  功能：
 *      拼接邮件地址
 *  参数
 *      addresslist     :   邮件地址列表
 *      seperator       :   邮件间的分割符
 *  返回：
 *      邮件地址字符串
 */
string MailManager::CombineAddress(vector<string> addresslist, char seperator)
{
    // 拼接邮件地址
    string AddressString = "";
    for (int i = 0; i < addresslist.size(); i++)
    {
        string EmailAddress = addresslist[i];
        if (BeGeneralLib::IsStringEmptyOrInvalidValue(EmailAddress) == true)
        {
            continue;
        }
        AddressString = EmailAddress + seperator;
    }
    return BeGeneralLib::StringTrim(AddressString, seperator);
}