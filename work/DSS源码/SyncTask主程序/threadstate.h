/*******************************************************************************
 *
 ********************* 程序文件:  ThreadState             ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-04-22              ***********************
 ********************* 完成时间:  2010-05-28              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *20100612  LATIS   1)增加了日志处理
 *
 *
 *
 *
 *
 *
//================================== 修改结束 ==================================*/

#ifndef _THREADSTATE_H
#define	_THREADSTATE_H
#include <iostream>
#include <pthread.h>
#include <time.h>
#include "begenerallib.h"
#include "beslog.h"


using namespace std;

class ThreadState
{
public:
    typedef void * (*ThreadFunc)(void *);
public:

    /*
     *  功能：
     *      构造函数
     *  参数：
     *      name            ：   线程名
     *      func            ：   线程函数指针
     *  返回：
     *      无
     */
    ThreadState(string name, ThreadFunc func);

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~ThreadState();
public:

    /*
     *  功能：
     *      返回线程名称
     *  参数：
     *      无
     *  返回：
     *      线程名称
     */
    virtual string Name();

    /*
     *  功能：
     *      创建线程
     *  参数：
     *      无
     *  返回：
     *      创建成功返回true，否则返回false
     */
    virtual bool Create();

    /*
     *  功能：
     *      线程运行标志
     *  参数：
     *      无
     *  返回：
     *      运行返回true，否则返回false
     */
    virtual bool Running();
    /*
     *  功能：
     *      线程停止
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Stop();
    /*
     *  功能：
     *      返回线程ID
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual pthread_t ThreadId();
private:
    pthread_t MyThreadId;
    time_t MyCreateTime;
    bool MyRunningFlag;
    string MyThreadName;
    ThreadFunc MyFunc;
};

#endif	/* _THREADSTATE_H */

