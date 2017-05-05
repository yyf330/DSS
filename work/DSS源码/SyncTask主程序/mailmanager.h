/* 
 * File:   mailmanager.h
 * Author: root
 *
 * Created on 2013年12月24日, 下午2:39
 */

#ifndef MAILMANAGER_H
#define	MAILMANAGER_H

#include "begenerallib.h"

class MailManager
{
public:
    /*
     *  功能：
     *      构造函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    MailManager();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~MailManager();
public:

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
    virtual bool SendMail(string subject, string content, string address);

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
    virtual bool SendMail(string subject, string content, vector<string> addresslist);
private:
#define DEFAULT_MAIL_CONTENT_FILE_PREFIX        "zwmail"
private:
    /*
     *  功能：
     *      拼接邮件地址
     *  参数
     *      addresslist     :   邮件地址列表
     *  返回：
     *      邮件地址字符串
     */
    virtual string CombineAddress(vector<string> addresslist);

    /*
     *  功能：
     *      拼接邮件地址
     *  参数
     *      addresslist     :   邮件地址列表
     *      seperator       :   邮件间的分割符
     *  返回：
     *      邮件地址字符串
     */
    virtual string CombineAddress(vector<string> addresslist, char seperator);
};

#endif	/* MAILMANAGER_H */

