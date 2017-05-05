#ifndef _SOCKETEXNB_H
#define	_SOCKETEXNB_H
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include "besgeneralheader.h"

using namespace std;

class SocketExNB
{
public:

    // 套接字类型
    enum SocketType
    {
        SOCKETEX_TYPE_TCP_CLIENT, // TCP客户端
    };

    // 套接字状态
    enum SocketState
    {
        SOCKETEX_STATE_CLOSED, // 已关闭
        SOCKETEX_STATE_OPENED, // 已打开
    };
public:
    /*
     *  功能：
     *      构造函数
     *  参数：
     *      type            :   套接字类型
     *      port            :   端口
     *  返回：
     *      无
     */
    SocketExNB(SocketType type, int port);

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~SocketExNB();
protected:
    /*
     *  功能：
     *      构造函数
     *  参数：
     *      sock            :   套接字
     *      addr            :   地址
     *  返回：
     *      无
     */
    SocketExNB(int sock, struct sockaddr_in addr);
public:
    /*
     *  功能：
     *      打开套接字
     *  参数：
     *      无
     *  返回：
     *      打开成功返回true,否则返回false
     */
    virtual bool Open();

    /*
     *  功能：
     *      关闭套接字
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Close();

    /*
     *  功能：
     *      接收数据
     *  参数：
     *      无
     *  返回：
     *      接收的数据字符串
     */
    virtual string Receive();

    /*
     *  功能：
     *      接收数据
     *  参数：
     *      timeout         :   接收超时时间（s）
     *  返回：
     *      接收的数据字符串
     */
    virtual string Receive(int timeout);

    /*
     *  功能：
     *      接收数据
     *  参数：
     *      buf             :   接收缓冲
     *      size            :   缓冲大小
     *      timeout         :   接收超时时间（s）
     *  返回：
     *      接收的数据长度
     */
    virtual int Receive(char *buf, int size, int timeout);

    /*
     *  功能：
     *      发送数据
     *  参数：
     *      data            :   发送的数据
     *  返回：
     *      发送成功返回true，否则返回false
     */
    virtual bool Send(string data);

    /*
     *  功能：
     *      发送数据
     *  参数：
     *      ipaddr          :   服务端ip
     *      data            :   发送的数据
     *  返回：
     *      发送成功返回true，否则返回false
     */
    virtual bool Send(string ipaddr, string data);

    /*
     *  功能：
     *      发送数据
     *  参数：
     *      ipaddr          :   服务端ip
     *      port            :   服务端端口
     *      data            :   发送的数据
     *  返回：
     *      发送成功返回true，否则返回false
     */
    virtual bool Send(string ipaddr, int port, string data);

    /*
     *  功能：
     *      发送数据
     *  参数：
     *      ipaddr          :   服务端ip
     *      port            :   服务端端口
     *      data            :   发送的数据
     *      timeout         :   接收超时时间（s）
     *  返回：
     *      发送成功返回true，否则返回false
     */
    virtual bool Send(string ipaddr, int port, string data, int timeout);

    /*
     *  功能：
     *      发送数据
     *  参数：
     *      ipaddr          :   服务端ip
     *      port            :   服务端端口
     *      data            :   发送的数据
     *      size            :   发送数据大小
     *      timeout         :   接收超时时间（s）
     *  返回：
     *      发送成功返回true，否则返回false
     */
    virtual bool Send(string ipaddr, int port, const char * data, int size, int timeout);

    /*
     *  功能：
     *      套接字状态
     *  参数：
     *      无
     *  返回：
     *      套接字状态
     */
    virtual SocketState State();

    /*
     *  功能：
     *      套接字ip
     *  参数：
     *      无
     *  返回：
     *      套接字ip地址
     */
    virtual string IpAddress();

    /*
     *  功能：
     *      端口
     *  参数：
     *      无
     *  返回：
     *      端口
     */
    virtual int Port();

    /*
     *  功能：
     *      获取套接字
     *  参数：
     *      无
     *  返回：
     *      套接字
     */
    virtual int Sock();
private:
    int MySock;
    struct sockaddr_in MySockAddr;
    int MyPort;
    SocketType MySockType;
    SocketState MyState;
};

#endif	/* _SOCKETEXNB_H */
