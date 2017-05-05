/* 
 * File:   AlarmManager.cpp
 * Author: root
 * 
 * Created on 2013年12月24日, 上午10:27
 */

#include "alarmmanager.h"

/*
 *  功能：
 *      构造函数
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *  返回：
 *      无
 */
AlarmManager::AlarmManager(DbDataManager * dbdatamanager)
{
    MyDbDataManager = dbdatamanager;
    MySmsManager = new SmsManager();
    MyMailManager = new MailManager();
}

/*
 *  功能：
 *      析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
AlarmManager::~AlarmManager()
{
    if (MySmsManager != NULL)
    {
        delete MySmsManager;
    }

    if (MyMailManager != NULL)
    {
        delete MyMailManager;
    }
}

/*
 *  功能：
 *      检查报警
 *  参数：
 *      无
 *  返回：
 *      如果成功返回true，否则返回false
 */
bool AlarmManager::Check()
{
    if (MyDbDataManager == NULL || MyDbDataManager->HasUnreportedAlarm() == false)
    {
        return false;
    }
    sql::ResultSet * AlarmRs = MyDbDataManager->GetAlarmResultSet();

    if (AlarmRs != NULL && AlarmRs->rowsCount() > 0)
    {
        // 检查是否要发送BMS报警
        bool IsSendingBMSAlarm = false;
        sql::ResultSet * BMSServerRs = MyDbDataManager->GetServerList(DbDataManager::REMOTE_SERVER_TYPE_BMS);
        if (BMSServerRs != NULL && BMSServerRs->rowsCount() > 0)
        {
        	BMSServerRs->next();
        	string BMSServerIP = BMSServerRs->getString("SERVERIP");
        	if (BeGeneralLib::IsPingOK(BMSServerIP, 2, 3) == true)
        	{
        		IsSendingBMSAlarm = true;
        	}
        }

        // 检查是否要发送短信报警
        bool IsSendingSmsAlarm = false;
        sql::ResultSet * SmsServerRs = MyDbDataManager->GetServerList(DbDataManager::REMOTE_SERVER_TYPE_SMS);
        sql::ResultSet * AlarmReceiverRs = MyDbDataManager->GetAlarmReceiverResultSet();
        if ((SmsServerRs != NULL && SmsServerRs->rowsCount() > 0) && (AlarmReceiverRs != NULL && AlarmReceiverRs->rowsCount() > 0))
        {
            SmsServerRs->next();
            string SmsServerIP = SmsServerRs->getString("SERVERIP");
            int SmsServerPort = SmsServerRs->getInt("PORT");
            if (BeGeneralLib::IsPingOK(SmsServerIP, 2, 3) == true)
            {
            	if (MySmsManager->Connect(SmsServerIP, SmsServerPort))
            	{
            		IsSendingSmsAlarm = true;
            	}
            	else
            	{
            		MySmsManager->Close();
            	}
            }
        }

        // 对于每一个报警，需要发送到每一个服务器上
        int count1=0, count2=0;
        while (AlarmRs->next() && count1 <= 10 && count2 <=10)	//最多发送11条报警
        {
        	string AlarmMode = AlarmRs->getString("ALARMMODE");
            string AlarmId = AlarmRs->getString("ALARMID");
            string HCode = AlarmRs->getString("HCODE");
            string IPAddr = AlarmRs->getString("IPAddr");
            string AlarmTime = AlarmRs->getString("STARTTIME");
            string Event = AlarmRs->getString("EVENT");
            string Content = AlarmRs->getString("CONTENT");
            int DiffDays = AlarmRs->getInt("DIFFDAYS");
            bool IsAlarmOverReport = (DiffDays > DEFAULT_MAX_REPORT_DAYS);

            // 如果超过时间则更新上报状态为错误
            if (IsAlarmOverReport == true)
            {
            	MyDbDataManager->UpdateAlarmReportState(DbDataManager::ALARM_REPORT_MODE_BMS, AlarmId, DbDataManager::ALARM_REPORT_STATE_ERROR);
            	MyDbDataManager->UpdateAlarmReportState(DbDataManager::ALARM_REPORT_MODE_SMS, AlarmId, DbDataManager::ALARM_REPORT_STATE_ERROR);
            	MyDbDataManager->UpdateAlarmReportState(DbDataManager::ALARM_REPORT_MODE_MAIL, AlarmId, DbDataManager::ALARM_REPORT_STATE_ERROR);
            	continue;
            }

            // 发送BMS报警
            if (IsSendingBMSAlarm == true && AlarmRs->getInt("REPORTSTATE") == 0)
            {
            	BesLog::DebugPrint("Sending BMSAlarm...", BesLog::LOG_MESSAGE_TYPE_INFO, false);
                if (SendBMSAlarm(AlarmId, GetBMSAlarmData(HCode, AlarmTime, Event, Content), BMSServerRs) == false)
                {
                	IsSendingBMSAlarm = false;	// 发送失败则不再发送
                	BesLog::DebugPrint("Sending BMSAlarm failed", BesLog::LOG_MESSAGE_TYPE_INFO, false);
                }
                else
                {
                	BesLog::DebugPrint("Sending BMSAlarm success", BesLog::LOG_MESSAGE_TYPE_INFO, false);
                }
            }

//          string AlarmData = "[" + AlarmTime + "]" + MyDbDataManager->GetHostName(HCode) + " " + IPAddr + SEPARATOR_CHAR_COLON + Event + " " + Content;
            string AlarmData = "[" + AlarmTime + "]" + HCode + " " + IPAddr + SEPARATOR_CHAR_COLON + Event + " " + Content;

            // 发送短信报警
            if ( (AlarmMode == "1" || AlarmMode == "3") && IsSendingSmsAlarm == true && AlarmRs->getInt("SMSREPORTSTATE") == 0)
            {
            	BesLog::DebugPrint("Sending SMSAlarm...", BesLog::LOG_MESSAGE_TYPE_INFO, false);
                SendAlarm(AlarmId, AlarmData, AlarmReceiverRs, DbDataManager::ALARM_REPORT_MODE_SMS);
                BesLog::DebugPrint("Sending SMSAlarm finish", BesLog::LOG_MESSAGE_TYPE_INFO, false);
                count1++;
            }

            // 发送邮件报警
            if ( (AlarmMode == "2" || AlarmMode == "3") && AlarmRs->getInt("EMAILREPORTSTATE") == 0)
            {
            	BesLog::DebugPrint("Sending EmailAlarm...", BesLog::LOG_MESSAGE_TYPE_INFO, false);
                SendAlarm(AlarmId, AlarmData, AlarmReceiverRs, DbDataManager::ALARM_REPORT_MODE_MAIL);
                BesLog::DebugPrint("Sending EmailAlarm finish", BesLog::LOG_MESSAGE_TYPE_INFO, false);
                count2++;
            }
        }//while结束

        // 销毁结果集
        MyDbDataManager->DestroyResultSet(BMSServerRs);
        MyDbDataManager->DestroyResultSet(SmsServerRs);
        MyDbDataManager->DestroyResultSet(AlarmReceiverRs);

        if (IsSendingSmsAlarm == true)
        {
            MySmsManager->Close();
        }
    }

    MyDbDataManager->DestroyResultSet(AlarmRs);
}

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
bool AlarmManager::SendBMSAlarm(string alarmid, string alarmdata, sql::ResultSet * serverrs)
{
    string AlarmId = alarmid;
    sql::ResultSet * ServerRs = serverrs;

    // 检查输入项是否有效
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(AlarmId) == true || ServerRs == NULL || ServerRs->rowsCount() == 0)
    {
        return false;
    }

    // 创建套接字
    SocketExNB * Sock = new SocketExNB(SocketExNB::SOCKETEX_TYPE_TCP_CLIENT, BES_AGENT_PORT);

    int ErrorCounter = 0;

    // 遍历每一个服务器
    ServerRs->beforeFirst();
    while (ServerRs->next())
    {
        string ServerIp = ServerRs->getString("SERVERIP");
        string ServerPort = ServerRs->getString("PORT");

        // 检查服务器ip和端口号的合法性

        // 发送报警数据
        string RecvString = "";
        if(Sock->Open())
        {
        	if(Sock->Send(ServerIp, BeGeneralLib::StringToInt(ServerPort), alarmdata))
        	{
        		RecvString = Sock->Receive(3);
        	}
        	Sock->Close();
        }

        // 如果返回信息为空，则错误计数加1
        if (BeGeneralLib::IsStringEmpty(RecvString) == true)
        {
            ErrorCounter++;
        }
    }

    // 更新报警记录的上报状态
    if (ErrorCounter == 0)
    {
        MyDbDataManager->UpdateAlarmReportState(DbDataManager::ALARM_REPORT_MODE_BMS, AlarmId, DbDataManager::ALARM_REPORT_STATE_SUCCESS);
        return true;
    }
    else
    {
        MyDbDataManager->UpdateAlarmReportState(DbDataManager::ALARM_REPORT_MODE_BMS, AlarmId, DbDataManager::ALARM_REPORT_STATE_FAIL);
        return false;
    }
}

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
void AlarmManager::SendAlarm(string alarmid, string alarmdata, sql::ResultSet * receiverrs, DbDataManager::AlarmReportMode reportmode)
{
	string AlarmId = alarmid;
    string AlarmData = alarmdata;
    sql::ResultSet * AlarmReceiverRs = receiverrs;

    // 检查输入项是否有效
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(AlarmId) == true
            || BeGeneralLib::IsStringEmptyOrInvalidValue(AlarmData) == true
            || AlarmReceiverRs == NULL || AlarmReceiverRs->rowsCount() == 0)
    {
        return;
    }

    int ErrorCounter = 0;
    AlarmReceiverRs->beforeFirst();
    while (AlarmReceiverRs->next())
    {
        bool Flag = true;
        switch (reportmode)
        {
            case DbDataManager::ALARM_REPORT_MODE_SMS:
            {
                Flag = SendSms(AlarmReceiverRs->getString("PHONENUMBER"), AlarmData);
                break;
            }
            case DbDataManager::ALARM_REPORT_MODE_MAIL:
            {
                Flag = SendMail(AlarmReceiverRs->getString("EMAIL"), AlarmData);
                break;
            }
        }
        if (Flag == false)
        {
            ErrorCounter++;
        }
    }

    // 更新报警记录
    DbDataManager::AlarmReportState ReportState = DbDataManager::ALARM_REPORT_STATE_NONE;
    if (ErrorCounter == 0)
    {
        ReportState = DbDataManager::ALARM_REPORT_STATE_SUCCESS;
    }
    else
    {
    	ReportState = (ErrorCounter == AlarmReceiverRs->rowsCount() ? DbDataManager::ALARM_REPORT_STATE_FAIL : DbDataManager::ALARM_REPORT_STATE_ERROR);
    }
    MyDbDataManager->UpdateAlarmReportState(reportmode, AlarmId, ReportState);
}

/*
 *  功能：
 *      发送短信
 *  参数
 *      alarmid             :   报警编号
 *      alarmdata           :   报警数据
 *      receiverrs          :   短信接收人结果集
 *  返回：
 *      无
 */
bool AlarmManager::SendSms(string phonenumber, string data)
{
    bool Flag = true;

    // 检查号码有效性
    string PhoneNumber = BeGeneralLib::StringTrim(phonenumber, SEPARATOR_CHAR_COMMA);
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(PhoneNumber) == true)
    {
        return false;
    }

    vector<string>PhoneNumList = BeGeneralLib::StringSplit(PhoneNumber, SEPARATOR_CHAR_COMMA, BeGeneralLib::REMOVE_EMPTY_ITEM);
    for (int i = 0; i < PhoneNumList.size(); i++)
    {
        if (MySmsManager->SendSMS(PhoneNumList[i], data) == false)
        {
            Flag = false;
        }
        sleep(1);
    }
    return Flag;
}

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
bool AlarmManager::SendMail(string mail, string data)
{
    // 检查号码有效性
    string MailAddress = BeGeneralLib::StringTrim(mail, SEPARATOR_CHAR_COMMA);
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(MailAddress) == true)
    {
        return false;
    }

    return MyMailManager->SendMail(DEFAULT_MAIL_SUBJECT, data, MailAddress);
}

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
string AlarmManager::GetBMSAlarmData(string hcode, string alarmtime, string event, string content)
{
    // 设置参数
    string HCode = hcode;
    string AlarmTime = alarmtime;
    string Event = event;
    string Content = content;

    // 读取配置文件中的用户名和密码
    string MonitorUser = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_MONITORUSER);
    string MonitorPassword = BeGeneralLib::GetConfigItemValue(DEFAULT_CONFIG_ITEM_MONITORPWD);


    // 对用户名和密码加密
    MonitorUser = BeGeneralLib::Cipher(BeGeneralLib::StringReverse(MonitorUser), MonitorUser.length() + BeGeneralLib::SumStringChars("602"), true);
    MonitorPassword = BeGeneralLib::Cipher(BeGeneralLib::StringReverse(MonitorPassword), MonitorPassword.length() + BeGeneralLib::SumStringChars("602"), true);

    // 设置参数
    string Parameter = BeCommand::CreateCommandParameter(6, MonitorUser.c_str(), MonitorPassword.c_str(), HCode.c_str(), AlarmTime.c_str(), Event.c_str(), Content.c_str());

    return BeCommand::CreateStandardCommand(COMMAND_CODE_ID, "602", Parameter);
}
