/* 
 * File:   taskqueue.cpp
 * Author: root
 * 
 * Created on 2010年4月21日, 下午3:25
 */

#include "taskqueue.h"

//------------------------------------ 公有方法 --------------------------------//

/*
 *  功能：
 *      默认构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
TaskQueue::TaskQueue()
{
    Initialize(TaskQueue::QUEUE_TYPE_PRIORITY);
}

/*
 *  功能：
 *      默认构造函数
 *  参数：
 *      type            :   队列类型
 *  返回：
 *      无
 */
TaskQueue::TaskQueue(QueueType type)
{
    Initialize(type);
}

/*
 *  功能：
 *      默认析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
TaskQueue::~TaskQueue()
{
    if (MyQueueMutexOK == true)
    {
        pthread_mutex_destroy(&MyQueueMutex);
    }
}

/*
 *  功能：
 *      返回队列头对象
 *  参数：
 *      无
 *  返回：
 *      队列头对象
 */
const Task& TaskQueue::Top()
{
    switch (MyQueueType)
    {
        case TaskQueue::QUEUE_TYPE_PRIORITY:
        {
            return MyPriorityQueue.top();
            break;
        }
        case TaskQueue::QUEUE_TYPE_NORMAL:
        {
            return MyListQueue.front();
            break;
        }
    }
}

/*
 *  功能：
 *      将任务压入队列
 *  参数：
 *      task        :   任务引用
 *  返回：
 *      无
 */
void TaskQueue::Push(Task task)
{
    // 锁定
    Lock();
    switch (MyQueueType)
    {
        case TaskQueue::QUEUE_TYPE_PRIORITY:
        {
            // 压入任务
            MyPriorityQueue.push(task);
            MyQueueMap.insert(pair<string, string > (task.Id(), task.HCode()));
            break;
        }
        case TaskQueue::QUEUE_TYPE_NORMAL:
        {
            MyListQueue.push_back(task);
            break;
        }
    }
    // 解锁
    Unlock();
}

/*
 *  功能：
 *      删除第一个对象
 *  参数：
 *      无
 *  返回：
 *      无
 */
void TaskQueue::Pop()
{
    // 锁定
    Lock();


    switch (MyQueueType)
    {
        case TaskQueue::QUEUE_TYPE_PRIORITY:
        {
            // 获取任务
            Task Tp = MyPriorityQueue.top();
            MyQueueMap.erase(Tp.Id());
            MyPriorityQueue.pop();
            break;
        }
        case QUEUE_TYPE_NORMAL:
        {
            MyListQueue.pop_front();
            break;
        }
    }
    //    if (!MyWaitQueue.empty())
    //    {
    //        Task T = MyWaitQueue.front();
    //        T.AdjustPriority(Task::TASK_PRIORITY_NORMAL);
    //        Push(T);
    //        MyWaitQueue.pop();
    //    }
    // 解锁
    Unlock();

    return;
}

/*
 *  功能：
 *      任务队列是否为空
 *  参数：
 *      无
 *  返回：
 *      为空返回true，否则返回false
 */
bool TaskQueue::Empty()
{
    bool IsEmpty = false;
    switch (MyQueueType)
    {
        case TaskQueue::QUEUE_TYPE_PRIORITY:
        {
            IsEmpty = MyPriorityQueue.empty();
            break;
        }
        case TaskQueue::QUEUE_TYPE_NORMAL:
        {
            IsEmpty = MyListQueue.empty();
            break;
        }
    }
    return IsEmpty;
}

/*
 *  功能：
 *      任务队列个数
 *  参数：
 *      无
 *  返回：
 *      任务个数
 */
int TaskQueue::Size()
{
    int ElementSize = 0;
    switch (MyQueueType)
    {
        case TaskQueue::QUEUE_TYPE_PRIORITY:
        {
            ElementSize = MyPriorityQueue.size();
            break;
        }
        case TaskQueue::QUEUE_TYPE_NORMAL:
        {
            ElementSize = MyListQueue.size();
            break;
        }
    }
    return ElementSize;
}

/*
 *  功能：
 *      指定键值的任务是否存在
 *  参数：
 *      key             :   键值
 *  返回：
 *      存在返回true，否则返回false
 */
bool TaskQueue::Exist(string key)
{
    return Exist(key, TaskQueue::SEARCH_BY_KEY);
}

/*
 *  功能：
 *      指定条件的任务是否存在
 *  参数：
 *      condition       :   查找条件
 *      bywhat          :   查找方式
 *  返回：
 *      存在返回true，否则返回false
 */
bool TaskQueue::Exist(string condition, SearchBy bywhat)
{
    bool Flag = false;
    switch (MyQueueType)
    {
        case TaskQueue::QUEUE_TYPE_PRIORITY:
        {
            Flag = IsInPriorityQueue(condition, bywhat);
            break;
        }
        case TaskQueue::QUEUE_TYPE_NORMAL:
        {
            Flag = IsInListQueue(condition, bywhat);
            break;
        }
    }
    return Flag;


    return false;

}

/*
 *  功能：
 *      清空队列
 *  参数：
 *      无
 *  返回：
 *      无
 */
void TaskQueue::Clear()
{
    // 锁定
    Lock();


    switch (MyQueueType)
    {
        case TaskQueue::QUEUE_TYPE_PRIORITY:
        {
            while (!MyPriorityQueue.empty())
            {
                MyPriorityQueue.pop();
            }
            while (!MyWaitQueue.empty())
            {
                MyWaitQueue.pop();
            }
            MyQueueMap.clear();
            break;
        }
        case TaskQueue::QUEUE_TYPE_NORMAL:
        {
            MyListQueue.clear();
            break;
        }
    }

    // 解锁
    Unlock();
}

/*
 *  功能：
 *      显示队列情况
 *  参数：
 *      无
 *  返回：
 *      无
 */
void TaskQueue::Show()
{
    // 锁定
    Lock();

    cout << "TaskCounter:" << Size() << endl;
    switch (MyQueueType)
    {
        case TaskQueue::QUEUE_TYPE_PRIORITY:
        {
            break;
        }
        case TaskQueue::QUEUE_TYPE_NORMAL:
        {
            for (list<Task>::iterator iter = MyListQueue.begin(); iter != MyListQueue.end(); iter++)
            {
                cout << "Id:" << (*iter).Id() << "\tHCode:" << (*iter).HCode() << "\tCmdCode:" << (*iter).CommandCode() << endl;
            }
            break;
        }
    }

    // 解锁
    Unlock();
}

/*
 *  功能：
 *      移动到等待队列
 *  参数：
 *      无
 *  返回：
 *      无
 */
void TaskQueue::MoveToFailQueue(Task & task)
{
    if (MyPriorityQueue.empty())
    {
        Push(task);
    }
    else
    {
        MyWaitQueue.push(task);
    }
}

//---------------------------------- 私有方法 ----------------------------------//

/*
 *  功能：
 *      初始化
 *  参数：
 *      type            :   队列类型
 *  返回：
 *      无
 */
void TaskQueue::Initialize(QueueType type)
{
    MyQueueType = type;
    int res = pthread_mutex_init(&MyQueueMutex, NULL);
    MyQueueMutexOK = !res;
}

/*
 *  功能：
 *      设置锁
 *  参数：
 *      无
 *  返回：
 *      成功返回true，否则返回false
 */
bool TaskQueue::Lock()
{
    return SetLock(true);
}

/*
 *  功能：
 *      解锁
 *  参数：
 *      无
 *  返回：
 *      成功返回true，否则返回false
 */
bool TaskQueue::Unlock()
{
    return SetLock(false);
}

/*
 *  功能：
 *      设置锁
 *  参数：
 *      islock          :   是否锁定
 *  返回：
 *      成功返回true，否则返回false
 */
bool TaskQueue::SetLock(bool islock)
{
    if (MyQueueMutexOK == false)
    {
        return false;
    }

    int retval = 0;
    if (islock == true)
    {
        retval = pthread_mutex_lock(&MyQueueMutex);
    }
    else
    {
        retval = pthread_mutex_unlock(&MyQueueMutex);
    }
    return (retval == 0);
}

/*
 *  功能：
 *      指定条件的任务是否存在于优先级队列
 *  参数：
 *      condition       :   查找条件
 *      bywhat          :   查找方式
 *  返回：
 *      存在返回true，否则返回false
 */
bool TaskQueue::IsInPriorityQueue(string condition, TaskQueue::SearchBy bywhat)
{
    if (Empty() == true)
    {
        return false;
    }

    switch (bywhat)
    {
        case TaskQueue::SEARCH_BY_KEY:
        {
            return (MyQueueMap.find(condition) != MyQueueMap.end());
        }
        case TaskQueue::SEARCH_BY_HCODE:
        {
            typedef map<string, string>::const_iterator CI;
            for (CI p = MyQueueMap.begin(); p != MyQueueMap.end(); ++p)
            {
                if (p->second == condition)
                {
                    return true;
                }
            }
            break;
        }
    }
    return false;
}

/*
 *  功能：
 *      指定条件的任务是否存在于列表队列
 *  参数：
 *      condition       :   查找条件
 *      bywhat          :   查找方式
 *  返回：
 *      存在返回true，否则返回false
 */
bool TaskQueue::IsInListQueue(string condition, TaskQueue::SearchBy bywhat)
{
    if (Empty() == true)
    {
        return false;
    }

    for (list<Task>::iterator iter = MyListQueue.begin(); iter != MyListQueue.end(); iter++)
    {
        switch (bywhat)
        {
            case TaskQueue::SEARCH_BY_KEY:
            {
                if (condition == (*iter).Id())
                {
                    return true;
                }
                break;
            }
            case TaskQueue::SEARCH_BY_HCODE:
            {
                if (condition == (*iter).HCode())
                {
                    return true;
                }
                break;
            }
        }
    }
    return false;
}


