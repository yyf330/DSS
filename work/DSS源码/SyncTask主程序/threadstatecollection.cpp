/* 
 * File:   threadstatecollection.cpp
 * Author: root
 * 
 * Created on 2010年4月26日, 下午5:37
 */

#include "threadstatecollection.h"

/*
 *  功能：
 *      构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
ThreadStateCollection::ThreadStateCollection()
{
    MyCounter = 0;
}


/*
 *  功能：
 *      析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
ThreadStateCollection::~ThreadStateCollection()
{
}

/*
 *  功能：
 *      增加线程状态对象
 *  参数：
 *      threadstate     :   线程状态对象
 *  返回：
 *      无
 */
void ThreadStateCollection::Add(ThreadState threadstate)
{
    MyKeyMap.insert(pair<int, string>(MyCounter, threadstate.Name()));
    MyThreadStateMap.insert(pair<string, ThreadState>(threadstate.Name(), threadstate));
    MyCounter++;
}

/*
 *  功能：
 *      清空集合
 *  参数：
 *      无
 *  返回：
 *      无
 */
void ThreadStateCollection::Clear()
{
    MyKeyMap.clear();
    MyThreadStateMap.clear();
    MyCounter = 0;
}

/*
 *  功能：
 *      根据index获取线程状态对象
 *  参数：
 *      name            :   线程名
 *  返回：
 *      线程状态对象
 */
ThreadState& ThreadStateCollection::GetThreadState(string name)
{
    return MyThreadStateMap.at(name);
}

/*
 *  功能：
 *      根据index获取线程状态对象
 *  参数：
 *      无
 *  返回：
 *      线程状态对象
 */
ThreadState& ThreadStateCollection::operator [](int index)
{
    if (index < MyKeyMap.size())
    {
        return MyThreadStateMap.at(MyKeyMap.at(index));
    }
}

/*
 *  功能：
 *      返回线程状态个数
 *  参数：
 *      无
 *  返回：
 *      个数
 */
int ThreadStateCollection::Size()
{
    return MyThreadStateMap.size();
}

