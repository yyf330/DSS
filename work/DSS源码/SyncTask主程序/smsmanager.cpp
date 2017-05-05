/* 
 * File:   smsmanager.cpp
 * Author: root
 * 
 * Created on 2013年2月21日, 上午9:35
 */

#include "smsmanager.h"
#include <errno.h>
/*------------------------------------ 公有方法 --------------------------------*/

/*
 *  功能：
 *      构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
SmsManager::SmsManager()
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
SmsManager::~SmsManager()
{
}

/*
 *  功能：
 *      连接短信服务器
 *  参数：
 *      serverip        :   短信服务器ip
 *      port            :   短信服务器端口
 *  返回：
 *      连接成功返回true，否则返回false
 */
bool SmsManager::Connect(string serverip, int port)
{
	bool Flag = false;		//是否connect成功

    MySock = socket(AF_INET, SOCK_STREAM, 0);
    if (MySock < 0) return false;

    // 初始化套接字地址
    bzero(&MySockAddr, sizeof (MySockAddr));
    MySockAddr.sin_family = AF_INET;

    MySockAddr.sin_addr.s_addr = inet_addr(serverip.c_str());
    MySockAddr.sin_port = htons((unsigned short) port);
    bzero(&(MySockAddr.sin_zero), 8);

	int flags = fcntl(MySock, F_GETFL);
	fcntl(MySock, F_SETFL, flags | O_NONBLOCK);

	if (connect(MySock, (struct sockaddr *)&MySockAddr, sizeof (struct sockaddr)) != 0)
	{
		if (errno == EINPROGRESS)
		{
			int error=-1, len;
			len = sizeof(int);
			timeval tm;
			tm.tv_sec  = 10;
			tm.tv_usec = 0;
			fd_set set;
			FD_ZERO(&set);
			FD_SET(MySock, &set);
			if (select(MySock+1, NULL, &set, NULL, &tm) > 0)
			{
				getsockopt(MySock, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
				if (error == 0)
				{
					Flag = true;
				}
			}
		}
		else
		{
			perror("SmsManager connect()");
		}
	}
	else
	{
		Flag = true;
	}

	return Flag;

/*
    bool Flag = (connect(MySock, (struct sockaddr *) & MySockAddr, sizeof (struct sockaddr)) == 0);
    if (Flag == false)
    {
        perror("SmsManager connect()");
    }
    return Flag;
*/
}

/*
 *  功能：
 *      发送短信
 *  参数：
 *      phonenum        :   手机号
 *      msg             :   短信
 *  返回：
 *      发送成功返回true，否则返回false
 */
bool SmsManager::SendSMS(string phonenum, string msg)
{
    // 转换数据编码
    char ConvertBuffer[DEFAULT_BUFFER_SIZE];
    bzero(ConvertBuffer, sizeof(ConvertBuffer));
    int ConvertLength = BeGeneralLib::Utf8ToUnicode(GenerateSmsData(phonenum, msg), ConvertBuffer, sizeof (ConvertBuffer));
    if (ConvertLength == -1)
    {
        cout << "Convert failed!" << endl;
        return false;
    }

    // 删除开头的ff fe字节
    char SmsData[DEFAULT_BUFFER_SIZE];
    bzero(SmsData, sizeof (SmsData));
    for (int i = 0; i < ConvertLength - 2; i++)
    {
        SmsData[i] = ConvertBuffer[i + 2];
    }
//    cout << "ConvertLength:" << ConvertLength << endl;
//    for (int i = 0; i < ConvertLength; i++)
//    {
//        printf("%02X\t", (unsigned char) ConvertBuffer[i]);
//    }
//    cout << endl;

    // 设置发送超时时间
    struct timeval Timeout = {10, 0};
    setsockopt(MySock, SOL_SOCKET, SO_SNDTIMEO, (char *) & Timeout, sizeof (struct timeval));

    // 发送数据
    if (send(MySock, SmsData, ConvertLength - 2, 0) < 0)
    {
        cout << "Send sms data failed!" << endl;
        return false;
    }

    // 设置接收超时时间
    Timeout.tv_sec = 5;
    Timeout.tv_usec = 0;
    setsockopt(MySock, SOL_SOCKET, SO_RCVTIMEO, (char *) & Timeout, sizeof (struct timeval));

    // 接收数据
    char MsgBuf[DEFAULT_BUFFER_SIZE + 1];
    bzero(MsgBuf, sizeof (MsgBuf));

    int RecvLen = -1;
    while(RecvLen < 0)
    {
    	RecvLen = recv(MySock, MsgBuf, sizeof (MsgBuf) - 1, 0);
    	if(EAGAIN != errno) break;
    }
    if (RecvLen < 0)
    {
        perror("SmsManager recv()");
        return false;
    }

//    cout << "RecvLen:" << RecvLen << endl;
//    for (int i = 0; i < RecvLen; i++)
//    {
//        printf("%02X\t", (unsigned char) MsgBuf[i]);
//    }
//    cout << endl;
    string RpString = "";
    ConvertLength = BeGeneralLib::UnicodeToUtf8(MsgBuf, RecvLen, RpString);
//    cout << "RpString:" << RpString << endl;
    if (ConvertLength == -1 || RpString.substr(0, 1) != "1")
    {
        cout << "Convert failed or receive failed message! " << RpString << endl;
        return false;
    }

    return true;
}

/*
 *  功能：
 *      关闭套接字
 *  参数：
 *      无
 *  返回：
 *      无
 */
void SmsManager::Close()
{
	if (MySock > 0) BeGeneralLib::CloseSock(MySock);
}

/*------------------------------------ 私有方法 --------------------------------*/

/*
 *  功能：
 *      生成短信数据
 *  参数：
 *      phonenum        :   手机号
 *      msg             :   消息内容
 *  返回：
 *      短信数据
 */
string SmsManager::GenerateSmsData(string phonenum, string msg)
{
    string SmsData = string("0") + BeGeneralLib::Number2String(phonenum.size()) + phonenum + msg;
    return SmsData;
}
