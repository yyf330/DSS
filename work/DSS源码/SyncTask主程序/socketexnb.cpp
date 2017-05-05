#include "socketexnb.h"
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
SocketExNB::SocketExNB(SocketType type, int port)
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
SocketExNB::~SocketExNB()
{
    Close();
}

/*
 *  功能：
 *      打开套接字
 *  参数：
 *  返回：
 *      打开成功返回true,否则返回false
 */
bool SocketExNB::Open()
{
    if (MyState == SOCKETEX_STATE_OPENED)
    {
        return true;
    }
    switch (MySockType)
    {
        case SOCKETEX_TYPE_TCP_CLIENT:
        {
            MySock = socket(AF_INET, SOCK_STREAM, 0);
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
void SocketExNB::Close()
{
    shutdown(MySock, SHUT_RDWR);
    close(MySock);
    MyState = SOCKETEX_STATE_CLOSED;
}

/*
 *  功能：
 *      接收数据
 *  参数：
 *      无
 *  返回：
 *      接收的数据字符串
 */
string SocketExNB::Receive()
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
string SocketExNB::Receive(int timeout)
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
int SocketExNB::Receive(char *buf, int size, int timeout)
{
    if (State() != SOCKETEX_STATE_OPENED)
    {
        return 0;
    }

    int RecvLen = -1;
    while(RecvLen < 0)
    {
    	RecvLen = recv(MySock, buf, size, 0);
    	if(EAGAIN != errno) break;
    }
    if (RecvLen < 0)
    {
        perror("SocketExNB Receive() recv()");
        return RecvLen;
    }
}

/*
 *  功能：
 *      发送数据
 *  参数：
 *      data            :   发送的数据
 *  返回：
 *      发送成功返回true，否则返回false
 */
bool SocketExNB::Send(string data)
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
bool SocketExNB::Send(string ipaddr, string data)
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
bool SocketExNB::Send(string ipaddr, int port, string data)
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
bool SocketExNB::Send(string ipaddr, int port, string data, int timeout)
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
bool SocketExNB::Send(string ipaddr, int port, const char * data, int size, int timeout)
{
    if (State() != SOCKETEX_STATE_OPENED)
    {
        return false;
    }

    MySockAddr.sin_port = htons((unsigned short) port);
    MySockAddr.sin_addr.s_addr = inet_addr(ipaddr.c_str());
    bzero(&(MySockAddr.sin_zero), 8);

    struct timeval Timeout;
    Timeout.tv_usec = 0;
    if (timeout > 0)
    {
        Timeout.tv_sec = timeout;
    }
    else
    {
        Timeout.tv_sec = DEFAULT_SOCKET_CONNECT_TIMEOUT;
    }

    bool ReturnFlag = false;

	int flags = fcntl(MySock, F_GETFL);
	fcntl(MySock, F_SETFL, flags | O_NONBLOCK);	//设成非阻塞模式

    bool Flag = false;		//是否connect成功
    if (connect(MySock, (struct sockaddr *)&MySockAddr, sizeof (struct sockaddr)) != 0)
    {
    	if (errno == EINPROGRESS)
    	{
    		int error=-1, len;
    		len = sizeof(int);
    		fd_set set;
    		FD_ZERO(&set);
    		FD_SET(MySock, &set);
    		if (select(MySock+1, NULL, &set, NULL, &Timeout) > 0)
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
    		perror("SocketExNB::Send connect()");
    	}
    }
    else
    {
    	Flag = true;
    }
    fcntl(MySock, F_SETFL, flags);

    if (Flag == true)
    {
    	if (send(MySock, data, size, 0) > 0)
    	{
    		ReturnFlag = true;
    	}
	}

    return ReturnFlag;
}

/*
 *  功能：
 *      套接字状态
 *  参数：
 *      无
 *  返回：
 *      套接字状态
 */
SocketExNB::SocketState SocketExNB::State()
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
string SocketExNB::IpAddress()
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
int SocketExNB::Port()
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
int SocketExNB::Sock()
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
SocketExNB::SocketExNB(int sock, struct sockaddr_in addr)
{
    MySock = sock;
    MySockAddr = addr;
    MyPort = ntohs(addr.sin_port);
    MyState = SOCKETEX_STATE_OPENED;
}
