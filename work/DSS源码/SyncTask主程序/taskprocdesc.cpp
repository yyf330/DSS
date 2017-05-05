/* 
 * File:   taskprocdesc.cpp
 * Author: root
 * 
 * Created on 2010年5月26日, 上午10:25
 */

#include "taskprocdesc.h"

/*
 *  功能：
 *      构造函数
 *  参数
 *      无
 *  返回：
 *      无
 */
TaskProcDesc::TaskProcDesc()
{
}

/*
 *  功能：
 *      析构函数
 *  参数
 *      无
 *  返回：
 *      无
 */
TaskProcDesc::~TaskProcDesc()
{
}

/*
 *  功能：
 *      获取处理过程描述信息
 *  参数
 *      type            :   处理过程类型
 *  返回：
 *      过程描述信息
 */
string TaskProcDesc::GetTaskStageDescription(DescriptionLang lang, TaskProcDesc::TaskProcEnum type)
{
    string Description = "";
    switch (type)
    {
        case TASK_PROC_PRE_PROCESSING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("task pre-processing...");
            }
            else
            {
                Description = string("预处理...");
            }
            break;
        }
        case TASK_PROC_CHECK_TASK_VALIDATION:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check task validation...");
            }
            else
            {
                Description = string("检查任务有效性...");
            }
            break;
        }
        case TASK_PROC_EXECUTING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("task executing...");
            }
            else
            {
                Description = string("正在执行...");

            }
            break;
        }
        case TASK_PROC_CREATE_MIRROR_TARGET:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("create mirror target...");
            }
            else
            {
                Description = string("创建镜像target...");
            }
            break;
        }
        case TASK_PROC_DELETE_MIRROR_TARGET:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("delete mirror target...");
            }
            else
            {
                Description = string("删除镜像Target...");
            }
            break;
        }
        case TASK_PROC_CREATE_COPY_TARGET:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("create copy target...");
            }
            else
            {
                Description = string("创建副本Target...");
            }
            break;
        }
        case TASK_PROC_DELETE_COPY_TARGET:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("delete copy target...");
            }
            else
            {
                Description = string("删除副本Target...");
            }
            break;
        }
        case TASK_PROC_COPY_CREATING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("copy creating...");
            }
            else
            {
                Description = string("副本生成中...");
            }
            break;
        }
        case TASK_PROC_SNAPSHOT_DELETING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("snapshot deleting...");
            }
            else
            {
                Description = string("快照删除中...");
            }
            break;
        }
        case TASK_PROC_SNAPSHOT_SYNCHRONIZING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("snapshot synchronizing...");
            }
            else
            {
                Description = string("快照同步中...");
            }
            break;
        }
        case TASK_PROC_CREATE_SHARE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("create share folder...");
            }
            else
            {
                Description = string("创建共享目录...");
            }
            break;
        }
        case TASK_PROC_OPEN_SHARE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("open share folder...");
            }
            else
            {
                Description = string("打开共享目录...");
            }
            break;
        }
        case TASK_PROC_CLOSE_SHARE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("close share folder...");
            }
            else
            {
                Description = string("关闭共享目录...");
            }
            break;
        }
        case TASK_PROC_DELETE_SHARE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("delete share folder...");
            }
            else
            {
                Description = string("删除共享目录...");
            }
            break;
        }
        case TASK_PROC_PUSH_TO_AGENT:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("push to agent...");
            }
            else
            {
                Description = string("转入agent处理...");
            }
            break;
        }
        case TASK_PROC_PROCESS_RESPONSE: // 处理响应
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("deal with response...");
            }
            else
            {
                Description = string("处理回应结果...");
            }
            break;
        }
        case TASK_PROC_POST_PROCESSING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("task post-processing...");
            }
            else
            {
                Description = string("后处理...");
            }
            break;
        }
        case TASK_PROC_COPY_SNAPSHOT:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("copy snapshot files...");
            }
            else
            {
                Description = string("复制快照文件...");
            }
            break;
        }
        case TASK_PROC_ERROR_PROCESSING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("task error processing...");
            }
            else
            {
                Description = string("错误处理中...");
            }
            break;
        }
        case TASK_PROC_COMPLETED:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("task ends! cost...");
            }
            else
            {
                Description = string("用时");
            }
            break;
        }
        case TASK_PROC_QUERY_HOST_INFO:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("query host information...");
            }
            else
            {
                Description = string("查询主机信息中...");
            }
            break;
        }
        case TASK_PROC_DELETE_HOST_SPACE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("delete host space...");
            }
            else
            {
                Description = string("...");
            }
            break;
        }
        case TaskProcDesc::TASK_PROC_IN_PROCESSING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("in processing...");
            }
            else
            {
                Description = string("进行中...");
            }
            break;
        }
        default:
        {

            break;
        }
    }
    return Description;
}

