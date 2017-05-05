/* 
 * File:   threadstate.cpp
 * Author: root
 * 
 * Created on 2010年4月22日, 下午1:08
 */

#include "threadstate.h"

/*
 *  功能：
 *      构造函数
 *  参数：
 *      name            ：   线程名
 *      func            ：   线程函数指针
 *  返回：
 *      无
 */
ThreadState::ThreadState(string name, ThreadFunc func)
{
    MyThreadId = 0;
    MyThreadName = name;
    MyFunc = func;
    MyRunningFlag = false;
}

/*
 *  功能：
 *      析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
ThreadState::~ThreadState()
{
}

/*
 *  功能：
 *      返回线程名称
 *  参数：
 *      无
 *  返回：
 *      线程名称
 */
string ThreadState::Name()
{
    return MyThreadName;
}

/*
 *  功能：
 *      创建线程
 *  参数：
 *      无
 *  返回：
 *      创建成功返回true，否则返回false
 */
bool ThreadState::Create()
{
    MyRunningFlag = true;
    int ret = pthread_create(&MyThreadId, NULL, MyFunc, NULL);
    if (ret != 0)
    {
        MyRunningFlag = false;
        BesLog::DebugPrint("Create " + MyThreadName + " failed!");
        return false;
    }
    BesLog::DebugPrint("Create " + MyThreadName + " OK.");
    time(&MyCreateTime);
    return true;
}

/*
 *  功能：
 *      线程运行标志
 *  参数：
 *      无
 *  返回：
 *      运行返回true，否则返回false
 */
bool ThreadState::Running()
{
    return MyRunningFlag;
}

/*
 *  功能：
 *      线程停止
 *  参数：
 *      无
 *  返回：
 *      无
 */
void ThreadState::Stop()
{
    MyRunningFlag = false;
}

/*
 *  功能：
 *      返回线程ID
 *  参数：
 *      无
 *  返回：
 *      无
 */
pthread_t ThreadState::ThreadId()
{
    return MyThreadId;
}
