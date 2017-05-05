/*******************************************************************************
 *
 ********************* 程序文件:  TaskQueue               ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-04-21              ***********************
 ********************* 完成时间:  2010-05-28              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *
 *
//================================== 修改结束 ==================================*/

#ifndef _TASKQUEUE_H
#define	_TASKQUEUE_H
#include <queue>
#include <functional>
#include <map>
#include <pthread.h>
#include "task.h"

class TaskQueue
{
public:

    enum QueueType
    {
        QUEUE_TYPE_PRIORITY,
        QUEUE_TYPE_NORMAL,
    };

    // 按何种方式查找

    enum SearchBy
    {
        SEARCH_BY_KEY,
        SEARCH_BY_HCODE,
    };
public:


    /*
     *  功能：
     *      默认构造函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    TaskQueue();


    /*
     *  功能：
     *      默认构造函数
     *  参数：
     *      type            :   队列类型
     *  返回：
     *      无
     */
    TaskQueue(QueueType type);

    /*
     *  功能：
     *      默认析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~TaskQueue();

    /*
     *  功能：
     *      返回队列头对象
     *  参数：
     *      无
     *  返回：
     *      队列头对象
     */
    virtual const Task & Top();

    /*
     *  功能：
     *      将任务压入队列
     *  参数：
     *      task        :   任务引用
     *  返回：
     *      无
     */
    virtual void Push(Task task);

    /*
     *  功能：
     *      删除第一个对象
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Pop();

    /*
     *  功能：
     *      任务队列是否为空
     *  参数：
     *      无
     *  返回：
     *      为空返回true，否则返回false
     */
    virtual bool Empty();

    /*
     *  功能：
     *      任务队列个数
     *  参数：
     *      无
     *  返回：
     *      任务个数
     */
    virtual int Size();


    /*
     *  功能：
     *      指定键值的任务是否存在
     *  参数：
     *      key             :   键值
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool Exist(string key);

    /*
     *  功能：
     *      指定条件的任务是否存在
     *  参数：
     *      condition       :   查找条件
     *      bywhat          :   查找方式
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool Exist(string condition, TaskQueue::SearchBy bywhat);

    /*
     *  功能：
     *      清空队列
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Clear();

    /*
     *  功能：
     *      显示队列情况
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Show();

    /*
     *  功能：
     *      移动到等待队列
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void MoveToFailQueue(Task& task);
private:
    priority_queue<Task> MyPriorityQueue; // 优先队列
    queue<Task> MyWaitQueue; // 等待队列
    map<string, string> MyQueueMap; // 队列映射
    pthread_mutex_t MyQueueMutex; // 队列互斥锁
    bool MyQueueMutexOK; // 互斥锁是否OK
    TaskQueue::QueueType MyQueueType; // 队列类型
    list<Task> MyListQueue; // 列表式队列
private:

    /*
     *  功能：
     *      初始化
     *  参数：
     *      type            :   队列类型
     *  返回：
     *      无
     */
    virtual void Initialize(QueueType type);

    /*
     *  功能：
     *      设置锁
     *  参数：
     *      无
     *  返回：
     *      成功返回true，否则返回false
     */
    virtual bool Lock();

    /*
     *  功能：
     *      解锁
     *  参数：
     *      无
     *  返回：
     *      成功返回true，否则返回false
     */
    virtual bool Unlock();

    /*
     *  功能：
     *      设置锁
     *  参数：
     *      islock          :   是否锁定
     *  返回：
     *      成功返回true，否则返回false
     */
    virtual bool SetLock(bool islock);

    /*
     *  功能：
     *      指定条件的任务是否存在于优先级队列
     *  参数：
     *      condition       :   查找条件
     *      bywhat          :   查找方式
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsInPriorityQueue(string condition, TaskQueue::SearchBy bywhat);

    /*
     *  功能：
     *      指定条件的任务是否存在于列表队列
     *  参数：
     *      condition       :   查找条件
     *      bywhat          :   查找方式
     *  返回：
     *      存在返回true，否则返回false
     */
    virtual bool IsInListQueue(string condition, TaskQueue::SearchBy bywhat);
};

#endif	/* _TASKQUEUE_H */

