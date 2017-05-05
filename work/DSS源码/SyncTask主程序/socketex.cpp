/* 
 * File:   socketex.cpp
 * Author: root
 * 
 * Created on 2010年4月29日, 下午4:56
 */

#include "socketex.h"
#include <errno.h>
//---------------------------------- 公有方法 ----------------------------------//

/*
 *  功能：
 *      构造函数
 *  参数：
 *      type            :   套接字类型
 *      port            :   端口
 *  返回：
 *      无
 */
SocketEx::SocketEx(SocketType type, int port)
{
    MySockType = type;
    MySockAddr.sin_addr.s_addr = inet_addr(DEFAULT_LOCALHOST_IP);
    MyPort = port;
    MyState = SOCKETEX_STATE_CLOSED;
}

/*
 *  功能：
 *      析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
SocketEx::~SocketEx()
{
    Close();
}

/*
 *  功能：
 *      打开套接字
 *  参数：
 *      无
 *  返回：
 *      打开成功返回true,否则返回false
 */
bool SocketEx::Open()
{
    return Open(1);
}

/*
 *  功能：
 *      打开套接字
 *  参数：
 *      clients         :   可连接的客户端个数
 *  返回：
 *      打开成功返回true,否则返回false
 */
bool SocketEx::Open(int clients)
{
    if (clients < 1)
    {
        clients = 1;
    }

    if (MyState == SOCKETEX_STATE_OPENED)
    {
        return true;
    }
    switch (MySockType)
    {
        case SOCKETEX_TYPE_TCP_SERVER:
        case SOCKETEX_TYPE_TCP_CLIENT:
        {
            MySock = socket(AF_INET, SOCK_STREAM, 0);
            break;
        }
        case SOCKETEX_TYPE_UDP_SERVER:
        {
            MySock = socket(AF_INET, SOCK_DGRAM, 0);
            break;
        }
    }
    if (MySock < 0)
    {
        cout << "Open sock failed!" << endl;
        MySock = 0;
        MyState = SOCKETEX_STATE_CLOSED;
        return false;
    }

    // 初始化套接字地址
    bzero(&MySockAddr, sizeof (MySockAddr));
    MySockAddr.sin_family = AF_INET;
    MySockAddr.sin_port = htons((unsigned short) MyPort);

    if ((MySockType == SOCKETEX_TYPE_TCP_SERVER || MySockType == SOCKETEX_TYPE_UDP_SERVER))
    {
        // 将运行程序机器的 IP 填充入 s_addr
        MySockAddr.sin_addr.s_addr = INADDR_ANY;

        // 将结构剩下的部分清零
        bzero(&(MySockAddr.sin_zero), 8);

        int on = 1;
        setsockopt(MySock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));
        if (bind(MySock, (struct sockaddr *) & MySockAddr, sizeof (struct sockaddr)) == -1)
        {
            perror("Bind Sock error:");
            Close();
            MyState = SOCKETEX_STATE_BIND_FAILED;
            MySock = 0;
            return false;
        }

        // 对于tcp服务端
        if (MySockType == SOCKETEX_TYPE_TCP_SERVER)
        {
            // 侦听Socket
            if (listen(MySock, clients) == -1)
            {
                Close();
                MyState = SOCKETEX_STATE_LISTEN_FAILED;
                MySock = 0;
                return false;
            }
        }
    }
    MyState = SOCKETEX_STATE_OPENED;
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
void SocketEx::Close()
{
    shutdown(MySock, SHUT_RDWR);
    close(MySock);
    MyState = SOCKETEX_STATE_CLOSED;
}

/*
 *  功能：
 *      等待客户端连接
 *  参数：
 *      无
 *  返回：
 *      连接成功返回套接字对象，否则返回null
 */
SocketEx* SocketEx::Accept()
{
    if (MySockType == SOCKETEX_TYPE_TCP_SERVER && State() == SOCKETEX_STATE_OPENED)
    {
        int ClientSock = 0;
        struct sockaddr_in ClientAddr;
        socklen_t Size = sizeof (struct sockaddr_in);
        if ((ClientSock = accept(MySock, (struct sockaddr *) & ClientAddr, &Size)) != -1)
        {
            return new SocketEx(ClientSock, ClientAddr);
        }
        else
        {
            perror("accept:");
        }
    }
    return NULL;
}

/*
 *  功能：
 *      接收数据
 *  参数：
 *      无
 *  返回：
 *      接收的数据字符串
 */
string SocketEx::Receive()
{
    return Receive(0);
}

/*
 *  功能：
 *      接收数据
 *  参数：
 *      timeout         :   接收超时时间（s）
 *  返回：
 *      接收的数据字符串
 */
string SocketEx::Receive(int timeout)
{
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    bzero(Buf, sizeof (Buf));
    int RecvLen = Receive(Buf, sizeof (Buf) - 1, timeout);
    if (RecvLen > 0)
    {
        return string(Buf);
    }
    else
    {
        return "";
    }
}

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
int SocketEx::Receive(char *buf, int size, int timeout)
{
    if (State() != SOCKETEX_STATE_OPENED)
    {
        return 0;
    }
    if (timeout > 0)
    {
        struct timeval Timeout;
        Timeout.tv_sec = timeout;
        Timeout.tv_usec = 0;
        setsockopt(MySock, SOL_SOCKET, SO_RCVTIMEO, (char *) & Timeout, sizeof (struct timeval));
    }
    return (recv(MySock, buf, size, 0));
}

/*
 *  功能：
 *      接收udp数据
 *  参数：
 *      client          :   客户端套接字
 *  返回：
 *      接收的数据长度
 */
string SocketEx::ReceiveFrom()
{
    struct sockaddr_in client;
    bzero(&client, sizeof (client));

    return ReceiveFrom(&client);
}

/*
 *  功能：
 *      接收udp数据
 *  参数：
 *      client          :   客户端套接字
 *  返回：
 *      接收的数据长度
 */
string SocketEx::ReceiveFrom(struct sockaddr_in * client)
{
    return ReceiveFrom(0, client);
}

/*
 *  功能：
 *      接收udp数据
 *  参数：
 *      timeout         :   超时时间
 *      client          :   客户端套接字
 *  返回：
 *      接收的数据长度
 */
string SocketEx::ReceiveFrom(int timeout, struct sockaddr_in * client)
{
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    bzero(Buf, sizeof (Buf));
    int RecvLen = ReceiveFrom(Buf, sizeof (Buf) - 1, timeout, client);
    if (RecvLen > 0)
    {
        return string(Buf);
    }
    else
    {
        return "";
    }
}

/*
 *  功能：
 *      接收udp数据
 *  参数：
 *      buf             :   缓冲
 *      size            :   缓冲大小
 *      timeout         :   超时时间
 *      client          :   客户端套接字
 *  返回：
 *      接收的数据长度
 */
int SocketEx::ReceiveFrom(char *buf, int size, int timeout, struct sockaddr_in * client)
{
    if (State() != SOCKETEX_STATE_OPENED)
    {
        return 0;
    }
    if (timeout > 0)
    {
        struct timeval Timeout;
        Timeout.tv_sec = timeout;
        Timeout.tv_usec = 0;
        setsockopt(MySock, SOL_SOCKET, SO_RCVTIMEO, (char *) & Timeout, sizeof (struct timeval));
    }
    //    unsigned int Len;
    struct sockaddr_in s;
    socklen_t Len = (socklen_t) (sizeof (s));

    return (recvfrom(MySock, buf, size, 0, (struct sockaddr *) client, &Len));
}

/*
 *  功能：
 *      发送数据
 *  参数：
 *      data            :   发送的数据
 *  返回：
 *      发送成功返回true，否则返回false
 */
bool SocketEx::Send(string data)
{
    return Send(IpAddress(), data);
}

/*
 *  功能：
 *      发送数据
 *  参数：
 *      ipaddr          :   服务端ip
 *      data            :   发送的数据
 *  返回：
 *      发送成功返回true，否则返回false
 */
bool SocketEx::Send(string ipaddr, string data)
{
    return Send(ipaddr, MyPort, data);
}

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
bool SocketEx::Send(string ipaddr, int port, string data)
{
    return Send(ipaddr, port, data, 0);
}

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
bool SocketEx::Send(string ipaddr, int port, string data, int timeout)
{
    return Send(ipaddr, port, data.c_str(), data.size(), timeout);
}

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
bool SocketEx::Send(string ipaddr, int port, const char * data, int size, int timeout)
{
	if (State() != SOCKETEX_STATE_OPENED)
    {
        return false;
    }

    //    if (ipaddr != "127.0.0.1")
    //    {
    //        cout << "SendIp:" << ipaddr << endl;
    //        cout << "SendPort:" << port << endl;
    //    }

    // 设置端口号
	MySockAddr.sin_port = htons((unsigned short) port);

	// 设置主机地址
	MySockAddr.sin_addr.s_addr = inet_addr(ipaddr.c_str());

	// 将结构剩下的部分清零
	bzero(&(MySockAddr.sin_zero), 8);

	struct timeval Timeout;
	if (timeout > 0)
	{
		Timeout.tv_sec = timeout;
		Timeout.tv_usec = 0;
	}
	else
	{
		Timeout.tv_sec = DEFAULT_SOCKET_CONNECT_TIMEOUT;
		Timeout.tv_usec = 0;
	}
	setsockopt(MySock, SOL_SOCKET, SO_SNDTIMEO, (char *) & Timeout, sizeof (struct timeval));

	if (send(MySock, data, size, 0) <= 0)
	{
		int Res = connect(MySock, (struct sockaddr *) & MySockAddr, sizeof (struct sockaddr));
		if (Res == 0)
		{
			send(MySock, data, size, 0);
			return true;
		}
		else return false;
	}
	else return true;
}

/*
 *  功能：
 *      套接字状态
 *  参数：
 *      无
 *  返回：
 *      套接字状态
 */
SocketEx::SocketState SocketEx::State()
{
    return MyState;
}

/*
 *  功能：
 *      套接字ip
 *  参数：
 *      无
 *  返回：
 *      套接字ip地址
 */
string SocketEx::IpAddress()
{
    return string(inet_ntoa(MySockAddr.sin_addr));
}

/*
 *  功能：
 *      端口号
 *  参数：
 *      无
 *  返回：
 *      端口号
 */
int SocketEx::Port()
{
    return MyPort;
}

/*
 *  功能：
 *      获取套接字
 *  参数：
 *      无
 *  返回：
 *      套接字
 */
int SocketEx::Sock()
{
    return MySock;
}

//---------------------------------- 保护方法 ----------------------------------//

/*
 *  功能：
 *      构造函数
 *  参数：
 *      sock            :   套接字
 *      addr            :   地址
 *  返回：
 *      无
 */
SocketEx::SocketEx(int sock, struct sockaddr_in addr)
{
    MySock = sock;
    MySockAddr = addr;
    MyPort = ntohs(addr.sin_port);
    MyState = SOCKETEX_STATE_OPENED;
}
