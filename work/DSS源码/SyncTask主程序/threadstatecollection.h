/*******************************************************************************
 *
 ********************* 程序文件:  ThreadStateCollection   ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-05-26              ***********************
 ********************* 完成时间:  2010-05-28              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *
 *
//================================== 修改结束 ==================================*/

#ifndef _THREADSTATECOLLECTION_H
#define	_THREADSTATECOLLECTION_H

#include <map>
#include <iostream>
#include "threadstate.h"

using namespace std;

class ThreadStateCollection
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
    ThreadStateCollection();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~ThreadStateCollection();
public:
    /*
     *  功能：
     *      增加线程状态对象
     *  参数：
     *      threadstate     :   线程状态对象
     *  返回：
     *      无
     */
    virtual void Add(ThreadState threadstate);

    /*
     *  功能：
     *      清空集合
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Clear();

    /*
     *  功能：
     *      根据index获取线程状态对象
     *  参数：
     *      name            :   线程名
     *  返回：
     *      线程状态对象
     */
    virtual ThreadState& GetThreadState(string name);

    /*
     *  功能：
     *      根据index获取线程状态对象
     *  参数：
     *      无
     *  返回：
     *      线程状态对象
     */
    virtual ThreadState & operator[](int index);

    /*
     *  功能：
     *      返回线程状态个数
     *  参数：
     *      无
     *  返回：
     *      个数
     */
    virtual int Size();
private:
    map<string, ThreadState> MyThreadStateMap;  // 线程状态队列
    map<int, string> MyKeyMap;                  // 主键队列
    int MyCounter;                              // 计数器
};

#endif	/* _THREADSTATECOLLECTION_H */

