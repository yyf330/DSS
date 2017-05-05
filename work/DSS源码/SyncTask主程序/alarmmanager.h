/* 
 * File:   AlarmManager.h
 * Author: root
 *
 * Created on 2013年12月24日, 上午10:27
 */

#ifndef ALARMMANAGER_H
#define	ALARMMANAGER_H

#include "dbdatamanager.h"
#include "smsmanager.h"
#include "mailmanager.h"
#include "socketex.h"
#include "socketexnb.h"
#include "becommand.h"

class AlarmManager
{
public:
    /*
     *  功能：
     *      构造函数
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *  返回：
     *      无
     */
    AlarmManager(DbDataManager * dbdatamanager);

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~AlarmManager();
public:

    /*
     *  功能：
     *      检查报警
     *  参数：
     *      无
     *  返回：
     *      如果成功返回true，否则返回false
     */
    virtual bool Check();
private:
    DbDataManager *MyDbDataManager;
    SmsManager *MySmsManager;
    MailManager *MyMailManager;
private:
#define DEFAULT_MAIL_SUBJECT                        "BES报警"
#define DEFAULT_MAX_REPORT_DAYS                     2
private:
    /*
     *  功能：
     *      发送BMS报警
     *  参数
     *      alarmid             :   报警编号
     *      alarmdata           :   报警数据
     *      serverrs            :   BMS服务器结果集
     *  返回：
     *      无
     */
    virtual bool SendBMSAlarm(string alarmid, string alarmdata, sql::ResultSet * serverrs);

    /*
     *  功能：
     *      发送报警
     *  参数
     *      alarmid             :   报警编号
     *      alarmdata           :   报警数据
     *      receiverrs          :   接收人结果集
     *      reportmode          :   上报方式
     *  返回：
     *      无
     */
    virtual void SendAlarm(string alarmid, string alarmdata, sql::ResultSet * receiverrs, DbDataManager::AlarmReportMode reportmode);

    /*
     *  功能：
     *      发送短信报警
     *  参数
     *      alarmid             :   报警编号
     *      alarmdata           :   报警数据
     *      receiverrs          :   短信接收人结果集
     *  返回：
     *      无
     */
    virtual bool SendSms(string phonenumber, string data);

    /*
     *  功能：
     *      发送邮件报警
     *  参数
     *      alarmid             :   报警编号
     *      alarmdata           :   报警数据
     *      receiverrs          :   短信接收人结果集
     *  返回：
     *      无
     */
    virtual bool SendMail(string mail, string data);

    //    virtual void SendSms(string phonenumber, string data);
    //
    //    virtual void SendMail(string mail, string data);

    /*
     *  功能：
     *      获取BMS报警数据
     *  参数
     *      hcode               :   主机编码
     *      alarmtime           :   报警时间
     *      event               :   事件
     *      content             :   报警内容
     *  返回：
     *      BMS报警数据
     */
    virtual string GetBMSAlarmData(string hcode, string alarmtime, string event, string content);
};

#endif	/* ALARMMANAGER_H */

