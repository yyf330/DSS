/* 
 * File:   smsmanager.h
 * Author: root
 *
 * Created on 2013年2月21日, 上午9:35
 */

#ifndef SMSMANAGER_H
#define	SMSMANAGER_H

#include "begenerallib.h"

class SmsManager
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
    SmsManager();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~SmsManager();
public:
    // 短信服务器型号

    enum SmsServerModel
    {
        SMS_SERVER_MODEL_EIC_NS20,
    };

public:

    /*
     *  功能：
     *      连接短信服务器
     *  参数：
     *      serverip        :   短信服务器ip
     *      port            :   短信服务器端口
     *  返回：
     *      连接成功返回true，否则返回false
     */
    virtual bool Connect(string serverip, int port);

    /*
     *  功能：
     *      发送短信
     *  参数：
     *      phonenum        :   手机号
     *      msg             :   短信
     *  返回：
     *      发送成功返回true，否则返回false
     */
    virtual bool SendSMS(string phonenum, string msg);

    /*
     *  功能：
     *      关闭套接字
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Close();
private:
    string MySmsServerIp; // 短信服务器ip
    int MySmsServerPort; // 短信服务器端口
    int MySock;
    struct sockaddr_in MySockAddr;
private:

    /*
     *  功能：
     *      生成短信数据
     *  参数：
     *      phonenum        :   手机号
     *      msg             :   消息内容
     *  返回：
     *      短信数据
     */
    virtual string GenerateSmsData(string phonenum, string msg);
};

#endif	/* SMSMANAGER_H */

