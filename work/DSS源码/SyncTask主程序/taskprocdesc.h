/*******************************************************************************
 *
 ********************* 程序文件:  TaskProcDesc            ***********************
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

#ifndef _TASKPROCDESC_H
#define	_TASKPROCDESC_H

#include <iostream>
#include <string>
#include "besgeneralheader.h"

class TaskProcDesc
{
public:
    /*
     *  功能：
     *      构造函数
     *  参数
     *      无
     *  返回：
     *      无
     */

    TaskProcDesc();
    /*
     *  功能：
     *      析构函数
     *  参数
     *      无
     *  返回：
     *      无
     */
    virtual ~TaskProcDesc();
public:

    enum TaskProcEnum
    {
        TASK_PROC_PRE_PROCESSING,
        TASK_PROC_CHECK_TASK_VALIDATION,
        TASK_PROC_EXECUTING,
        TASK_PROC_CREATE_MIRROR_TARGET,
        TASK_PROC_DELETE_MIRROR_TARGET,
        TASK_PROC_CREATE_COPY_TARGET,
        TASK_PROC_DELETE_COPY_TARGET,
        TASK_PROC_COPY_CREATING,
        TASK_PROC_SNAPSHOT_DELETING,
        TASK_PROC_SNAPSHOT_SYNCHRONIZING,
        TASK_PROC_CREATE_SHARE,
        TASK_PROC_OPEN_SHARE,
        TASK_PROC_CLOSE_SHARE,
        TASK_PROC_DELETE_SHARE,
        TASK_PROC_PUSH_TO_AGENT,
        TASK_PROC_PROCESS_RESPONSE,
        TASK_PROC_POST_PROCESSING,
        TASK_PROC_COPY_SNAPSHOT,
        TASK_PROC_ERROR_PROCESSING,
        TASK_PROC_COMPLETED,
        TASK_PROC_QUERY_HOST_INFO,
        TASK_PROC_QUERY_HOST_STATUS,
        TASK_PROC_DELETE_HOST_SPACE,
        TASK_PROC_IN_PROCESSING,
    };
public:

    /*
     *  功能：
     *      获取处理过程描述信息
     *  参数
     *      type            :   处理过程类型
     *  返回：
     *      过程描述信息
     */
    static string GetTaskStageDescription(DescriptionLang lang, TaskProcDesc::TaskProcEnum type);
};

#endif	/* _TASKPROCDESC_H */

