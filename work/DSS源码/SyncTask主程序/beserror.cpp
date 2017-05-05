/* 
 * File:   beserror.cpp
 * Author: root
 * 
 * Created on 2010年5月18日, 下午4:09
 */

#include "beserror.h"
#include "besencryptf.h"
#include "besencryptd.h"

extern int ZwgDebugLevel;

/*
 *  功能：
 *      构造函数
 *  参数
 *      无
 *  返回：
 *      无
 */
BesError::BesError()
{
    Clear();
}

/*
 *  功能：
 *      析构函数
 *  参数
 *      无
 *  返回：
 *      无
 */
BesError::~BesError()
{
}

/*
 *  功能：
 *      设置错误
 *  参数
 *      type        :   错误类型
 *  返回：
 *      无
 */
void BesError::SetError(BesError::ErrorType type)
{
    SetError(type, "");
}

/*
 *  功能：
 *      设置错误
 *  参数
 *      type        :   错误类型
 *      parameter   :   参数
 *  返回：
 *      无
 */
void BesError::SetError(BesError::ErrorType type, string parameter)
{
    SetError(type, parameter, "", true);
}

/*
 *  功能：
 *      设置错误
 *  参数
 *      type        :   错误类型
 *      parameter   :   参数
 *      debuginfo   :   调试信息
 *      overwrite   :   是否覆盖
 *  返回：
 *      无
 */
void BesError::SetError(BesError::ErrorType type, string parameter, string debuginfo, bool overwrite)
{
    if (overwrite == false && HasError() == true)
    {
        return;
    }

    MyType = type;
    MyParameter = parameter;
    MyDebugInfo = debuginfo;
}

/*
 *  功能：
 *      设置错误
 *  参数
 *      errobj      :   错误类型
 *  返回：
 *      无
 */
void BesError::SetError(BesError *errobj)
{
    if (errobj == NULL)
    {
        return;
    }

    SetError(errobj->GetErrorType(), errobj->GetErrorParameter());
}

/*
 *  功能：
 *      获取错误类型
 *  参数
 *      无
 *  返回：
 *      错误类型
 */
BesError::ErrorType BesError::GetErrorType()
{
    return MyType;
}

/*
 *  功能：
 *      获取错误参数
 *  参数
 *      无
 *  返回：
 *      错误参数
 */
string BesError::GetErrorParameter()
{
    return MyParameter;
}

/*
 *  功能：
 *      获取错误描述
 *  参数
 *      无
 *  返回：
 *      错误信息描述
 */
string BesError::GetErrorDescription()
{
    return GetErrorDescription(BeGeneralLib::GetDescriptionLang());
}

/*
 *  功能：
 *      获取错误描述
 *  参数
 *      lang        :   错误描述语言
 *  返回：
 *      错误描述
 */
string BesError::GetErrorDescription(DescriptionLang lang)
{
    string ErrorDescription = GetErrorDescription(lang, MyType, MyParameter);
    if ((BesLog::LogMessageType)ZwgDebugLevel <= BesLog::LOG_MESSAGE_TYPE_VERBOSE)
    {
        ErrorDescription = MyDebugInfo + SEPARATOR_CHAR_SPACE + ErrorDescription;
    }
    return ErrorDescription;
}

/*
 *  功能：
 *      获取错误描述
 *  参数
 *      type        :   错误类型
 *  返回：
 *      错误信息描述
 */
string BesError::GetErrorDescription(BesError::ErrorType type)
{
    return GetErrorDescription(type, "");
}

/*
 *  功能：
 *      获取错误描述
 *  参数
 *      type        :   错误类型
 *      parameter   :   参数
 *  返回：
 *      错误信息描述
 */
string BesError::GetErrorDescription(BesError::ErrorType type, string parameter)
{
    return GetErrorDescription(BeGeneralLib::GetDescriptionLang(), type, parameter);
}

/*
 *  功能：
 *      获取错误描述
 *  参数
 *      lang        :   错误描述语言
 *      type        :   错误类型
 *      parameter   :   参数
 *  返回：
 *      错误信息描述
 */
string BesError::GetErrorDescription(DescriptionLang lang, BesError::ErrorType type, string parameter)
{
    // 获取内部错误描述
    string Description = GetInteralErrorDescription(lang, type);

    // 如果参数不为空，则为参数增加修饰
    if (BeGeneralLib::IsStringEmpty(parameter) == false)
    {
        parameter = string("[") + parameter + string("]");
    }

    // 替换错误描述中的指代符
    Description = BeGeneralLib::StringReplace(Description, "%s", parameter);
    return Description;
}

/*
 *  功能：
 *      获取错误描述
 *  参数
 *      item        :   自检项
 *      parameter   :   参数
 *      result      :   自检结果
 *  返回：
 *      错误信息描述
 */
string BesError::GetErrorDescription(BesError::ErrorType type, string parameter, BesError::SelfCheckResult result)
{
    DescriptionLang Lang = BeGeneralLib::GetDescriptionLang();
    string Description = GetInteralErrorDescription(Lang, type);

    // 如果参数不为空，则为参数增加修饰
    if (BeGeneralLib::IsStringEmpty(parameter) == false)
    {
        parameter = string("[") + parameter + string("]");
    }

    // 替换错误描述中的指代符
    Description = BeGeneralLib::StringReplace(Description, "%s", parameter);

    string ResultDescription = GetSelfCheckResultDescription(Lang, result);
    if (result != SELF_CHECK_RESULT_NONE)
    {
        int StringSize = Description.size();
        if (Lang == DESCRIPTION_LANG_CN)
        {
            int AsciiCounter = BeGeneralLib::AsciiCharCount(Description);
            StringSize = (Description.size() - AsciiCounter) *2 / 3 + AsciiCounter;
        }
        Description = Description + string(DEFAULT_SELF_CHECK_MESSAGE_LENGTH - StringSize, SEPARATOR_CHAR_DOT);
        ResultDescription = string("[") + ResultDescription + string("]");
    }

    Description = Description + ResultDescription;
    return Description;
}

/*
 *  功能：
 *      获取授权项名称
 *  参数
 *      item        :   授权项
 *  返回：
 *      授权项名称
 */
string BesError::GetLicenseItemName(LicenseItem item)
{
    // 判断语言项
    DescriptionLang Lang = BeGeneralLib::GetDescriptionLang();

    string ItemName = DEFAULT_EMPTY_STRING;
    switch (item)
    {
        case BesError::LICENSE_ITEM_HOST:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("host");
            }
            else
            {
                ItemName = string("主机");
            }
            break;
        }
        case BesError::LICENSE_ITEM_SNAPSHOT:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("snapshot");
            }
            else
            {
                ItemName = string("快照");
            }
            break;
        }
        case BesError::LICENSE_ITEM_DATABASE:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("database");
            }
            else
            {
                ItemName = string("数据库");
            }
            break;
        }
        case BesError::LICENSE_ITEM_NET_EMERGENCY:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("net emergency");
            }
            else
            {
                ItemName = string("网启应急");
            }
            break;
        }
        case BesError::LICENSE_ITEM_VM_EMERGENCY:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("vm emergency");
            }
            else
            {
                ItemName = string("虚拟机应急");
            }
            break;
        }
        case BesError::LICENSE_ITEM_FC_EMERGENCY:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("fc emergency");
            }
            else
            {
                ItemName = string("光纤应急");
            }
            break;
        }
        case BesError::LICENSE_ITEM_CDP:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("CDP");
            }
            else
            {
                ItemName = string("CDP");
            }
            break;
        }
        case BesError::LICENSE_ITEM_CDP_SNAPSHOT_KEEPTIME:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("CDP snapshot keeptime");
            }
            else
            {
                ItemName = string("CDP快照保留时间");
            }
            break;
        }
        case BesError::LICENSE_ITEM_EA:
        {
            if (Lang == DESCRIPTION_LANG_EN)
            {
                ItemName = string("EA");
            }
            else
            {
                ItemName = string("自动应急");
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return ItemName;
}

/*
 *  功能：
 *      清除错误
 *  参数
 *      无
 *  返回：
 *      无
 */
void BesError::Clear()
{
    MyType = BES_ERR_NONE;
    MyParameter = "";
}

/*
 *  功能：
 *      是否有错误
 *  参数
 *      无
 *  返回：
 *      如果有返回true，否则返回false
 */
bool BesError::HasError()
{
    return (MyType != BesError::BES_ERR_NONE);
}

/*
 *  功能：
 *      获取错误描述
 *  参数
 *      lang        :   语言
 *      type        :   错误类型
 *  返回：
 *      错误描述
 */
string BesError::GetInteralErrorDescription(DescriptionLang lang, ErrorType type)
{
    string Description = "";
    switch (type)
    {
            // License错误
        case BesError::BES_ERR_LICENSE_NOT_EXIST: // License文件不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The license file does not exist!");
            }
            else
            {
                Description = string("授权文件不存在！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_INVALID: // 无效的license
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The license is invalid!");
            }
            else
            {
                Description = string("无效的授权！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_EXPIRED: // License已经过期
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The license is expired!");
            }
            else
            {
                Description = string("授权已经过期！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_EXPIRED_DATE_INVALID: // 无效的授权到期日
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The license expired date is invalid!");
            }
            else
            {
                Description = string("无效的授权到期日！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_HOST_NUMBER: // 主机数超出License许可
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host number exceeds the license!");
            }
            else
            {
                Description = string("主机数超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_SNAPSHOT_NUMBER: // 快照数超出License许可
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The snapshot number exceeds the license!");
            }
            else
            {
                Description = string("快照数超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_DATABASE_NUMBER: // 数据库数超出License许可
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The database number exceeds the license!");
            }
            else
            {
                Description = string("数据库数超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_NET_EMERGENCY_NUMBER:// 超过license网启应急数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The NET emergency number exceeds the license!");
            }
            else
            {
                Description = string("网启应急主机数超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_VM_EMERGENCY_NUMBER:// 超过license虚拟机应急数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The VM emergency number exceeds the license!");
            }
            else
            {
                Description = string("虚拟机应急主机数超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_FC_EMERGENCY_NUMBER: // 超过license光纤卡应急数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The FC emergency number exceeds the license!");
            }
            else
            {
                Description = string("光纤卡应急主机数超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_CDP_NUMBER: // 超过授权的CDP个数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The CDP number exceeds the license!");
            }
            else
            {
                Description = string("CDP保护个数超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_CDP_KEEP_SNAPSHOT_TIME: // 超过license自动应急数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The CDP snapshot keep time exceeds the license!");
            }
            else
            {
                Description = string("CDP连续快照时间超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_OVER_EA_NUMBER: // 超过license自动应急数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The EA number exceeds the license!");
            }
            else
            {
                Description = string("自动应急主机数超出授权许可！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_SYSTEM_TIME_EARLIER_THAN_LICENSE_DATE: // 系统时间早于授权创建时间
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The system time is earlier than license time!");
            }
            else
            {
                Description = string("系统时间早于授权创建时间！");
            }
            break;
        }
        case BesError::BES_ERR_LICENSE_CURRENT_EARLIER_THAN_LAST_LICENSE_DATE: // 当前授权文件早于上个授权文件创建时间
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The license time of current is earlier than last one!");
            }
            else
            {
                Description = string("当前授权文件早于上个授权文件的创建时间！");
            }
            break;
        }

        case BesError::BES_ERR_SYSTEM_TIME_CHANGED: // 系统时间已修改
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The system time has been changed!");
            }
            else
            {
                Description = string("系统时间已修改！");
            }
            break;
        }
            // 数据库错误
        case BesError::BES_ERR_DATABASE_OFFLINE: // 数据库处于离线状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The database is offline!");
            }
            else
            {
                Description = string("数据库处于离线状态！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_CONNECTED_ERROR: // 数据库连接失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Connect database server failed!");
            }
            else
            {
                Description = string("连接数据库失败！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_INVALID_HOST: // 无效的数据库服务器
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid database server %s!");
            }
            else
            {
                Description = string("无效的数据库服务器%s！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_INVALID_USER: // 无效的数据库用户名
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid database user name %s!");
            }
            else
            {
                Description = string("无效的数据库用户名%s！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_INVALID_PASSWORD:// 无效的数据库密码
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid database password %s!");
            }
            else
            {
                Description = string("无效的数据库密码%s！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_INVALID_DBNAME: // 无效的数据库名称
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid database name %s!");
            }
            else
            {
                Description = string("无效的数据库名称%s！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_INVALID_TABLE: // 无效的数据表
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid table %s!");
            }
            else
            {
                Description = string("无效的数据表%s！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_INVALID_FILED: // 无效的字段
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid filed name %s!");
            }
            else
            {
                Description = string("无效的字段%s！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_INVALID_FILED_VALUE: // 无效的字段值
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid filed value %s!");
            }
            else
            {
                Description = string("无效的字段值%s！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_EXEC_QUERY_FAILED: // 执行SQL查询失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Execute query sql %s failed!");
            }
            else
            {
                Description = string("执行SQL查询%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_EXEC_INSERT_FAILED: // 执行SQL插入失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Execute insert sql %s failed!");
            }
            else
            {
                Description = string("执行SQL插入%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_EXEC_UPDATE_FAILED: // 执行SQL更新失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Execute update sql %s failed!");
            }
            else
            {
                Description = string("执行SQL更新%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DATABASE_EXEC_DELETE_FAILED: // 执行SQL删除失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Execute delete sql %s failed!");
            }
            else
            {
                Description = string("执行SQL删除%s失败！");
            }
            break;
        }
            // Agent错误
        case BesError::BES_ERR_AGENT_CONNECT_FAILED: // 连接Agent失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Connect agent %s failed!");
            }
            else
            {
                Description = string("连接Agent%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_NO_RESPONSE: // Agent没有回应
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The agent is timeout!");
            }
            else
            {
                Description = string("Agent没有回应！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_RECEIVE_TIMEOUT: // 接收Agent回应超时
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Receiving data is timeout for agent %s!");
            }
            else
            {
                Description = string("接收Agent%s回应超时！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_IS_BUSY: // Agent处于忙碌状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The agent is busy %s!");
            }
            else
            {
                Description = string("Agent处于忙碌状态%s！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_TASK_ERROR: // Agent处理任务错误
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The agent proceeds task error %s!");
            }
            else
            {
                Description = string("Agent处理任务错误%s！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_TASK_FAILED: // 处理任务失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The agent proceeds task failed %s!");
            }
            else
            {
                Description = string("Agent处理任务失败%s！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_INIT_COPY_FAILED: // 初始复制失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The agent %s initializing copy failed!");
            }
            else
            {
                Description = string("Agent%s初始复制失败！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_SNAPSHOT_PREPROCESSING_FAILED: // 快照预处理失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The agent %s snapshot-preprocessing failed!");
            }
            else
            {
                Description = string("Agent%s快照预处理失败！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_SNAPSHOT_POSTPROCESSING_FAILED: // 快照后处理失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The agent %s snapshot-postprocessing failed!");
            }
            else
            {
                Description = string("Agent%s快照后处理失败！");
            }
            break;
        }
        case BesError::BES_ERR_AGENT_QUERY_VOLUME_TYPE_FAILED: // 查询卷类型失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The agent %s query volume type failed!");
            }
            else
            {
                Description = string("Agent%s查询卷类型失败！");
            }
            break;
        }

            // 任务错误
        case BesError::BES_ERR_TASK_TIMEOUT: // 任务超时
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is timeout!");
            }
            else
            {
                Description = string("任务%s超时！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_OVER_COUNTER: // 任务已经超出最大重试次数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task is over counter!");
            }
            else
            {
                Description = string("任务已经超出最大重试次数！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_INVALID_TASK_TYPE: // 无效的任务类型
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The type of task %s is invalid !");

            }
            else
            {
                Description = string("无效的任务类型%s！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_INVALID_TASK_STATE: // 无效的任务状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The state of task %s is invalid!");
            }
            else
            {
                Description = string("无效的任务状态%s！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_EXECUTE_TIMEOUT: // 执行任务超时
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Excuting task %s is timeout!");
            }
            else
            {
                Description = string("执行任务%s超时！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_TERMINATED: // 任务被终止
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is terminated!");
            }
            else
            {
                Description = string("任务%s被终止！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_TERMINATING: // 任务终止中
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is being terminated!");
            }
            else
            {
                Description = string("任务%s正在被强制终止...！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_TERMINATED_BY_EMERGENCY: // 任务被应急操作终止
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is terminated by emergency operation!");
            }
            else
            {
                Description = string("任务%s被应急操作终止！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_FAILED:// 任务失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s failed!");
            }
            else
            {
                Description = string("%s任务执行失败！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_SUCCESS: // 任务成功
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is finished successfully!");
            }
            else
            {
                Description = string("%s任务执行成功！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_ERROR: // 任务错误
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is finished with error!");
            }
            else
            {
                Description = string("%s任务出现错误！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_BUSY:// 任务繁忙
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is busy!");
            }
            else
            {
                Description = string("%s任务繁忙！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_IS_EXECUTING: // 任务正在执行
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is executing...");
            }
            else
            {
                Description = string("%s任务正在执行...");
            }
            break;
        }
        case BesError::BES_ERR_TASK_IS_WAITING: // 任务正在等待
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s is waiting...");
            }
            else
            {
                Description = string("%s任务正在等待...");
            }
            break;
        }
        case BesError::BES_ERR_TASK_FINISH: // 任务完成
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s has finished!");
            }
            else
            {
                Description = string("%s任务已完成！");
            }
            break;
        }
        case BesError::BES_ERR_TASK_EXIST:// 任务已存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The task %s already exists!");
            }
            else
            {
                Description = string("%s任务已存在！");
            }
            break;
        }

            // 盘符错误
        case BesError::BES_ERR_VOLUME_SYMBOL_NOT_EXIST: // 盘符不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The volume symbol %s does not exist!");
            }
            else
            {
                Description = string("盘符%s不存在！");
            }
            break;
        }

            // 套接字错误
        case BesError::BES_ERR_SOCKET_OPEN_FAILED: // 打开套接字失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Open socket %s failed!");
            }
            else
            {
                Description = string("打开套接字%s失败！");
            }
            break;
        }

            // 主机检查错误

        case BesError::BES_ERR_INVALID_HOST_CODE: // 无效的主机编码
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid host code %s!");
            }
            else
            {
                Description = string("无效的主机编码%s！");
            }
            break;
        }
        case BesError::BES_ERR_INVALID_HOST_NAME: // 无效的主机名
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid host name %s!");
            }
            else
            {
                Description = string("无效的主机名%s！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_NOT_EXIST: // 主机不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s does not exist!");
            }
            else
            {
                Description = string("主机%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_CODE_EXIST: // 主机编码已存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host code %s already exists!");
            }
            else
            {
                Description = string("主机编码%s已存在！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_NAME_EXIST: // 主机名称已存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host name %s already exists!");
            }
            else
            {
                Description = string("主机名称%s已存在！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_IP_EXIST: // ip地址已存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The ip address %s already exists!");
            }
            else
            {
                Description = string("ip地址%s已存在！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_IP_NOT_EXIST: // 主机ip不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
                Description = string("The ip address %s does not exist!");
            else
                Description = string("ip地址%s不存在！");
            break;
        }
		case BesError::BES_ERR_HOST_NOT_IN_DOG:
        {
            if (lang == DESCRIPTION_LANG_EN)
                Description = string("The host is invalid!");
            else
                Description = string("被保护主机非法！");
            break;
        }

		// 文件错误
        case BesError::BES_ERR_FILE_OPEN_FAILED: // 打开文件失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Open file %s failed!");
            }
            else
            {
                Description = string("打开文件%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_FILE_NOT_EXIST: // 文件不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The file %s does not exist!");
            }
            else
            {
                Description = string("文件%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_FILE_NO_PRIVILLAGE: // 访问文件权限不够
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Without enough privillage to access file %s!");
            }
            else
            {
                Description = string("访问文件%s权限不够！");
            }
            break;
        }
        case BesError::BES_ERR_FILE_READ_FAILED: // 读取文件失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Read file %s failed!");
            }
            else
            {
                Description = string("读取文件%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_FILE_WRITE_FAILED: // 写入文件失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Write file %s failed!");
            }
            else
            {
                Description = string("写入文件%s失败！");
            }
            break;
        }

            // 磁盘错误
        case BesError::BES_ERR_DISK_NO_SPACE: // 磁盘空间不足
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The disk %s has no space!");
            }
            else
            {
                Description = string("磁盘%s空间不足！");
            }
            break;
        }
        case BesError::BES_ERR_DISK_NOT_EXIST: // 磁盘不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The disk %s does not exist!");
            }
            else
            {
                Description = string("磁盘%s不存在！");
            }
            break;
        }
            // 命令错误
        case BesError::BES_ERR_COMMAND_INVALID_SEGMENT_COUNT: // 无效的命令段数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid command segment count!");
            }
            else
            {
                Description = string("无效的命令段数%s！");
            }
            break;
        }
        case BesError::BES_ERR_COMMAND_INVALID_HEADER: // 无效的命令头
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid command header %s!");
            }
            else
            {
                Description = string("无效的命令头！");
            }
            break;
        }
        case BesError::BES_ERR_COMMAND_INVALID_TAILOR: // 无效的命令尾
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid command tailor %s!");
            }
            else
            {
                Description = string("无效的命令尾！");
            }
            break;
        }
        case BesError::BES_ERR_COMMAND_INVALID_CODE: // 无效的命令字
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid command code %s!");
            }
            else
            {
                Description = string("无效的命令字%s！");
            }
            break;
        }
        case BesError::BES_ERR_COMMAND_INVALID_PARAMETER: // 无效的指令参数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid command parameter %s!");
            }
            else
            {
                Description = string("无效的指令参数%s！");
            }
            break;
        }
        case BesError::BES_ERR_COMMAND_INVALID_PARAMETER_COUNT: // 无效的指令参数个数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid command parameter count %s!");
            }
            else
            {
                Description = string("无效的指令参数个数%s！");
            }
            break;
        }
        case BesError::BES_ERR_COMMAND_INVALID_RESPONSE: // 无效的指令应答
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid command response %s!");
            }
            else
            {
                Description = string("无效的指令应答%s！");
            }
            break;
        }

            // Target错误
        case BesError::BES_ERR_TARGET_CREATE_FAILED: // 创建Target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create target %s failed!");
            }
            else
            {
                Description = string("创建target%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_ADJUST_FAILED: // 调整target空间失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Adjust target %s space failed!");
            }
            else
            {
                Description = string("调整target%s空间失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_DELETE_FAILED: // 删除target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete target %s failed!");
            }
            else
            {
                Description = string("删除target%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_MOUNT_FAILED: // 挂载target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Mount target %s failed!");
            }
            else
            {
                Description = string("挂载target%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_UNMOUNT_FAILED: // 卸载Target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Unmount target %s failed!");
            }
            else
            {
                Description = string("卸载target%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_LOGON_FAILED: // 登录target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Logon target %s failed!");
            }
            else
            {
                Description = string("登录target%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_LOGOFF_FAILED: // 注销target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Logoff target %s failed!");
            }
            else
            {
                Description = string("注销target%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_DISCOVER_FAILED: // 扫描target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Discover target %s failed!");
            }
            else
            {
                Description = string("扫描target%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_ENABLE_FAILED: // 启用target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Enable target %s failed!");
            }
            else
            {
                Description = string("启用target%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_BIND_FAILED: // target绑定initiator失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Bind target %s initiators failed!");
            }
            else
            {
                Description = string("target%s绑定initiator失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_UNBIND_FAILED: // target解除initiator绑定失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Unbind target %s initiators failed!");
            }
            else
            {
                Description = string("target%s解除initiator绑定失败！");
            }
            break;
        }
        case BesError::BES_ERR_TARGET_SESSION_CHECK_FAILED: // target会话检查失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Check target %s session failed!");
            }
            else
            {
                Description = string("检查target%s会话失败！");
            }
            break;
        }
            // 共享目录错误
        case BesError::BES_ERR_SHARE_OPEN_FAILED: // 打开共享目录失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Open share folder %s failed!");
            }
            else
            {
                Description = string("打开共享目录%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_SHARE_CLOSE_FAILED: // 关闭共享目录失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Close share folder %s failed!");
            }
            else
            {
                Description = string("关闭共享目录%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_SHARE_DELETE_FAILED: // 删除共享目录失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete share folder %s failed!");
            }
            else
            {
                Description = string("删除共享目录%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_SHARE_FOLDER_IS_EMPTY: // 共享目录为空
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Share folder %s is empty!");
            }
            else
            {
                Description = string("共享目录%s为空！");
            }
            break;
        }

            // 副本错误
        case BesError::BES_ERR_COPY_IS_USED: // 副本正在使用中
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy %s is used!");
            }
            else
            {
                Description = string("副本%s正在使用中！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_BOOT_NOT_ALLOWED_DELETE: // 应急副本不允许删除
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The emergency copy %s can not be deleted!");
            }
            else
            {
                Description = string("应急副本%s不允许删除！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_IS_NOT_USED: // 副本未使用
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy %s is not used!");
            }
            else
            {
                Description = string("副本%s未使用！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_NO_SPACE: // 副本空间不足
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("There is no more space in the copy %s!");
            }
            else
            {
                Description = string("副本%s空间不足！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_FOLDER_IS_EMPTY: // 副本目录为空
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Copy folder %s is empty!");
            }
            else
            {
                Description = string("%s副本目录为空！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_NOT_EXIST: // 副本不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy %s does not exist!");
            }
            else
            {
                Description = string("副本%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_NEW_NOT_ALLOWED_DELETE: // 最新副本不允许删除
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The newest copy %s can not be deleted!");
            }
            else
            {
                Description = string("最新副本%s不允许删除！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_STATE_ERROR: // 副本状态错误
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The state of copy %s is error!");
            }
            else
            {
                Description = string("副本%s状态错误！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_NOT_BOOT_COPY: // 该副本不是网启副本
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy %s is not the emergency copy!");
            }
            else
            {
                Description = string("副本%s不是应急副本！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_TYPE_NOT_ALLOWED_SNAPSHOT:// 该副本类型不允许进行快照
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy %s is not  allowed to create snapshot!");
            }
            else
            {
                Description = string("该副本%s类型不允许创建快照！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_TYPE_IS_SNAPSHOT_COPY:// 副本为快速型副本
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The type of copy %s is fast copy!");
            }
            else
            {
                Description = string("副本%s为快速副本！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_EXIST: // 副本已存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy %s already exists!");
            }
            else
            {
                Description = string("副本%s已存在！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_FINAL_NORMAL_NOT_ALLOWED_DELETE: // 最后一个标准副本不允许删除
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The final standard copy %s is not allowed to be deleted!");
            }
            else
            {
                Description = string("最后一个标准副本%s不允许删除！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_TRUSTED_NOT_EXIST: // 可信副本不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The trusted copy does not exist!");
            }
            else
            {
                Description = string("可信副本%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_EMERGENCY_NOT_EXIST: // 应急副本不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The emergency copy %s does not exist!");
            }
            else
            {
                Description = string("应急副本%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_TRUSTED_NOT_ALLOWED_DELETE: // 可信副本不允许删除
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The trusted copy %s is not allowed to be deleted!");
            }
            else
            {
                Description = string("可信副本%s不允许删除！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_VOLUME_NOT_SYSTEM_VOLUME: // 该副本不是系统盘的副本
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy %s is not system volume copy!");
            }
            else
            {
                Description = string("该副本%s不是系统盘的副本！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_SIZE_NOT_MATCH_ORIGINAL: // 副本大小和原始大小不一致
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The size of copy %s is not match with original!");
            }
            else
            {
                Description = string("副本%s大小和原始大小不一致！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_CREATE_FAILED: // 创建副本失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create copy %s failed!");
            }
            else
            {
                Description = string("创建副本%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_DELETE_FAILED: // 删除副本失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete copy %s failed!");
            }
            else
            {
                Description = string("删除副本%s失败！");
            }
            break;
        }

        // 磁盘C
        case BesError::BES_ERR_VOLUME_C_NOT_ALLOWED_SWITCH: // C盘不允许做网盘切换
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The volume C %s is not allowed to switch!");
            }
            else
            {
                Description = string("%sC盘不允许做网盘切换！");
            }
            break;
        }
        case BesError::BES_ERR_VOLUME_C_NOT_ALLOWED_RESTORE: // C盘不允许做网盘还原
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The volume C %s is not allowed to restore!");
            }
            else
            {
                Description = string("%sC盘不允许做网盘还原！");
            }
            break;
        }

        // 快照
        case BesError::BES_ERR_SNAPSHOT_ZERO_NOT_ALLOWED_DELETE: // 快照零不允许删除
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The zero snapshot %s is not allowed to be deleted!");
            }
            else
            {
                Description = string("快照零%s不允许删除！");
            }
            break;
        }
        case BesError::BES_ERR_SNAPSHOT_NEW_NOT_ALLOWED_DELETE: // 最新快照不允许删除
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The newest snapshot %s is not allowed to be deleted!");
            }
            else
            {
                Description = string("最新快照%s不允许删除！");
            }
            break;
        }
        case BesError::BES_ERR_SNAPSHOT_IS_TRUSTED: // 该快照为可信快照
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The snapshot %s is trusted!");
            }
            else
            {
                Description = string("该快照%s为可信快照！");
            }
            break;
        }
        case BesError::BES_ERR_SNAPSHOT_CREATE_FAILED: // 生成快照失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create snapshot %s failed!");
            }
            else
            {
                Description = string("生成快照%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_SNAPSHOT_NOT_EXIST: // 快照不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The snapshot %s does not exist!");
            }
            else
            {
                Description = string("快照%s不存在！");
            }
            break;
        }

        // 管理网卡
        case BesError::BES_ERR_EHTO_NOT_ALLOWED_CONFIG: // 管理网卡不允许设置
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Manage network interface %s is not allowed to be configured!");
            }
            else
            {
                Description = string("管理网卡%s不允许设置！");
            }
            break;
        }

        case BesError::BES_ERR_HOST_IS_NET_BOOT: // 主机处于网启中
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s is netboot!");
            }
            else
            {
                Description = string("主机%s处于网启中！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_IS_NET_BOOT_OR_COPY_USED: // 主机处于网启或副本使用中
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s is netboot or copy used!");
            }
            else
            {
                Description = string("主机%s处于网启或副本使用中！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_NOT_PROTECT_DATABASE: // 主机没有设置进行数据库保护
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s is not set to protect database!");
            }
            else
            {
                Description = string("主机%s没有设置进行数据库保护！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_IS_NOT_NET_BOOT: // 主机不是网启状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s is not in NET emergency mode");
            }
            else
            {
                Description = string("主机%s不是网启状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_IS_IN_EMERGENCY_MODE: // 主机处于应急状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s is in emergency mode");
            }
            else
            {
                Description = string("主机%s处于应急状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_IS_NOT_IN_EMERGENCY_MODE: // 主机不是处于应急状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s is not in emergency mode");
            }
            else
            {
                Description = string("主机%s不是处于应急状态！");
            }
            break;
        }
        case BesError::BES_ERR_NIC_NOT_EXIST: // 网卡不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The network adapter %s does not exist!");
            }
            else
            {
                Description = string("网卡%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_SERVER_IP_NOT_EXIST: // 服务器ip不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The server ip %s does not exist!");
            }
            else
            {
                Description = string("服务器ip%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_STRATEGY_NOT_EXIST: // 策略不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The strategy %s does not exist!");
            }
            else
            {
                Description = string("策略%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_PATH_NOT_EXIST: // 路径不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The path %s does not exist!");
            }
            else
            {
                Description = string("路径%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_INVALID_OSTYPE: // 无效的操作系统类型
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid host OS type %s!");
            }
            else
            {
                Description = string("无效的操作系统类型%s！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_INVALID_NETBOOT_PROTOCOL: // 无效的启动协议
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The protocol %s is invalid!");
            }
            else
            {
                Description = string("无效的启动协议%s！");
            }
            break;
        }
        case BesError::BES_ERR_BOOT_PROTOCOL_IS_NOT_NETBOOT: // 启动协议不是网启协议
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The protocol %s is not for NET emergency!");
            }
            else
            {
                Description = string("启动协议%s不是网启协议！");
            }
            break;
        }
        case BesError::BES_ERR_BOOT_PROTOCOL_IS_NOT_VMBOOT: // 启动协议不是虚拟机协议
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The protocol %s is not for VM emergency!");
            }
            else
            {
                Description = string("启动协议%s不是虚拟机协议！");
            }
            break;
        }
        case BesError::BES_ERR_BOOT_PROTOCOL_IS_NOT_FCBOOT: // 启动协议不是光纤协议
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The protocol %s is not for FC emergency!");
            }
            else
            {
                Description = string("启动协议%s不是光纤协议！");
            }
            break;
        }
        case BesError::BES_ERR_SCHEDULE_NOT_EXIST: // 调度计划不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The schedule %s does not exist!");
            }
            else
            {
                Description = string("调度计划%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_OSTYPE_NOT_MATCH_COMMAND_CODE: // 主机操作系统类型和命令字%s不相符
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host OS type  does not match the command code %s!");
            }
            else
            {
                Description = string("主机操作系统类型和命令字%s不相符！");
            }
            break;
        }
        case BesError::BES_ERR_INVALID_MOUNT_POINT: // 无效的主机挂载点
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid mount point %s!");
            }
            else
            {
                Description = string("无效的主机挂载点%s！");
            }
            break;
        }
        case BesError::BES_ERR_SYNC_SNAPSHOT_FAILED: // 同步快照失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Synchronizing snapshot %s failed!");
            }
            else
            {
                Description = string("同步快照%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DELETE_HOST_SPACE_FAILED: // 删除主机卷空间失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete host space %s failed!");
            }
            else
            {
                Description = string("删除主机卷空间%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_FC_CREATE_PARTITION_FAILED: // 创建FC分区失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create host %s FC partition failed!");
            }
            else
            {
                Description = string("创建主机%sFC分区失败！");
            }
            break;
        }
        case BesError::BES_ERR_FC_CREATE_SNAPSHOT_IMAGE_FAILED:// 创建FC快照失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create host %s FC snapshot image failed!");
            }
            else
            {
                Description = string("创建主机%sFC快照失败！");
            }
            break;
        }
        case BesError::BES_ERR_FC_CREATE_VOLUME_PAIR_FAILED: // 创建FC卷对失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create host %s FC volume pair failed!");
            }
            else
            {
                Description = string("创建主机%sFC卷对失败！");
            }
            break;
        }
        case BesError::BES_ERR_FC_CREATE_LUN_MAP_FAILED: // 创建FC主机映射失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create host %s FC lun map failed!");
            }
            else
            {
                Description = string("创建主机%sFC映射失败！");
            }
            break;
        }
        case BesError::BES_ERR_FC_ADD_TARGET_FAILED: // 增加主机FC target失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Add host %s FC target failed!");
            }
            else
            {
                Description = string("增加主机%sFC target失败！");
            }
            break;
        }
        case BesError::BES_ERR_FC_WWN_IS_INVALID: // 无效的wwn
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The WWN %s is invalid!");
            }
            else
            {
                Description = string("无效的光纤卡WWN%s！");
            }
            break;
        }

        // 虚拟机
        case BesError::BES_ERR_VM_NO_MEMORY: // VM没有内存
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Virutal machine %s does not have enough memory!");
            }
            else
            {
                Description = string("虚拟机%s内存空间不足！");
            }
            break;
        }
        case BesError::BES_ERR_VM_NO_CPU: // VM没有CPU
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Virutal machine %s does not have enough CPU!");
            }
            else
            {
                Description = string("虚拟机%sCPU个数不足！");
            }
            break;
        }
        case BesError::BES_ERR_VM_CREATE_FAILED: // 创建虚拟机失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create virutal machine %s failed!");
            }
            else
            {
                Description = string("创建虚拟机%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_VM_START_FAILED: // 启动虚拟机失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Start virutal machine %s failed!");
            }
            else
            {
                Description = string("启动虚拟机%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_VM_SHUTDOWN_FAILED: // 关闭虚拟机失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Shutdown virutal machine %s failed!");
            }
            else
            {
                Description = string("关闭虚拟机%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_VM_DELETE_FAILED: // 删除虚拟机失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete virutal machine %s failed!");
            }
            else
            {
                Description = string("删除虚拟机%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_VM_STAGE_CHECK_COPY: // 检查副本
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Check virtual machine copy %s...");
            }
            else
            {
                Description = string("检查虚拟机副本%s...");
            }
            break;
        }
        case BesError::BES_ERR_VM_STAGE_CHANGE_EMERGENCY_PROTOCOL: // 切换启动协议
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Change boot protocol %s...");
            }
            else
            {
                Description = string("切换启动协议%s...");
            }
            break;
        }
        case BesError::BES_ERR_VM_STAGE_APPLY_VM_CONFIG:// 应用虚拟机配置
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Apply virtual machine configuration %s...");
            }
            else
            {
                Description = string("应用虚拟机配置%s...");
            }
            break;
        }
        case BesError::BES_ERR_VM_STAGE_CREATEVM: // 创建虚拟机
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create virtual machine %s...");
            }
            else
            {
                Description = string("创建虚拟机%s...");
            }
            break;
        }
        case BesError::BES_ERR_VM_STAGE_STARTVM: // 启动虚拟机
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Start virtual machine %s...");
            }
            else
            {
                Description = string("启动虚拟机%s...");
            }
            break;
        }
        case BesError::BES_ERR_VM_STAGE_STARTVM_FINISH: // 启动虚拟机完成
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Start virtual machine finished %s.");
            }
            else
            {
                Description = string("启动虚拟机完成%s。");
            }
            break;
        }
        case BesError::BES_ERR_INVALID_HOST_ACTION: // 无效的主机动作
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid host action %s!");
            }
            else
            {
                Description = string("无效的主机动作%s！");
            }
            break;
        }
        case BesError::BES_ERR_INVALID_HOST_STATUS: // 无效的主机状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is invalid!");
            }
            else
            {
                Description = string("无效的主机状态%s！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_STATUS_IS_NOT_LOCAL: // 主机不是本地状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is not local!");
            }
            else
            {
                Description = string("主机%s不是本地状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_STATUS_IS_NOT_EMERGENCY: // 主机不是应急状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is not emergency!");
            }
            else
            {
                Description = string("主机%s不是应急状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_STATUS_IS_EMERGENCY: // 主机处于应急状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is emergency!");
            }
            else
            {
                Description = string("主机%s处于应急状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_STATUS_IS_NOT_NET: // 主机不是网启状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is not NET boot!");
            }
            else
            {
                Description = string("主机%s不是网启应急状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_STATUS_IS_NOT_VM: // 主机不是虚拟机状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is not VM boot!");
            }
            else
            {
                Description = string("主机%s不是虚拟应急状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_STATUS_IS_NOT_FC: // 主机不是光纤启动状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is not FC boot!");
            }
            else
            {
                Description = string("主机%s不是光纤应急状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_STATUS_IS_OFFLINE: // 主机处于离线状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is  offline!");
            }
            else
            {
                Description = string("主机%s处于离线状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_STATUS_IS_NOT_OFFLINE: // 主机不是离线状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The status of host %s is not offline!");
            }
            else
            {
                Description = string("主机%s不是离线状态！");
            }
            break;
        }
        case BesError::BES_ERR_INVALID_USER_AND_PASSWORD: // 无效的用户名和密码
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid user and password %s!");
            }
            else
            {
                Description = string("无效的用户名和密码%s！");
            }
            break;
        }
        case BesError::BES_ERR_CHANGE_BOOT_PROTOCOL_FAILED: // 修改启动协议失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Change boot protocol %s failed!");
            }
            else
            {
                Description = string("修改启动协议%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_NIC_IS_NOT_BRIDGED: // 网卡没有设置桥接
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The network adapter %s is not bridged!");
            }
            else
            {
                Description = string("网卡%s没有设置桥接！");
            }
            break;
        }
        case BesError::BES_ERR_SERVER_ID_NOT_MATCH_LOCAL:// 服务器id和本地id不一致
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The server id %s does not match the local server id!");
            }
            else
            {
                Description = string("服务器id%s和本地id不一致！");
            }
            break;
        }
        case BesError::BES_ERR_SERVER_ID_EXIST:// 服务器编号已经存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The server id %s already exists!");
            }
            else
            {
                Description = string("服务器编号%s已经存在！");
            }
            break;
        }
        case BesError::BES_ERR_SERVER_ID_IS_INVALID:// 服务器编号是无效的
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The server id %s is invalid!");
            }
            else
            {
                Description = string("无效的服务器编号%s！");
            }
            break;
        }
        case BesError::BES_ERR_REMOTE_SERVER_NOT_EXIST: // 远程服务器不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The remote server %s does not exist!");
            }
            else
            {
                Description = string("远程服务器%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_REMOTE_SERVER_TYPE_IS_INVALID: // 远程服务器类型无效
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The type of remote server %s is invalid!");
            }
            else
            {
                Description = string("远程服务器%s类型无效！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_LOCAL_EMERGENCY_FUNCTION_IS_OPENED: // 主机本地应急功能处于打开状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The local emergency function of host %s is opened!");
            }
            else
            {
                Description = string("主机%s本地应急功能处于打开状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_LOCAL_EMERGENCY_FUNCTION_IS_CLOSED: // 主机本地应急功能处于关闭状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The local emergency function of host %s is closed!");
            }
            else
            {
                Description = string("主机%s本地应急功能处于关闭状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_NET_EMERGENCY_FUNCTION_IS_OPENED: // 主机网启应急功能处于打开状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The NET emergency function of host %s is opened!");
            }
            else
            {
                Description = string("主机%s网启应急功能处于打开状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_NET_EMERGENCY_FUNCTION_IS_CLOSED: // 主机网启应急功能处于关闭状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The NET emergency function of host %s is closed!");
            }
            else
            {
                Description = string("主机%s网启应急功能处于关闭状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_VM_EMERGENCY_FUNCTION_IS_OPENED: // 主机虚拟机应急功能处于打开状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The VM emergency function of host %s is opened!");
            }
            else
            {
                Description = string("主机%s虚拟机应急功能处于打开状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_VM_EMERGENCY_FUNCTION_IS_CLOSED: // 主机虚拟机应急功能处于关闭状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The VM emergency function of host %s is closed!");
            }
            else
            {
                Description = string("主机%s虚拟机应急功能处于关闭状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_FC_EMERGENCY_FUNCTION_IS_OPENED: // 主机光纤卡应急功能处于打开状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The FC emergency function of host %s is opened!");
            }
            else
            {
                Description = string("主机%s光纤卡应急功能处于打开状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_FC_EMERGENCY_FUNCTION_IS_CLOSED: // 主机光纤应急功能处于关闭状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The FC emergency function of host %s is closed!");
            }
            else
            {
                Description = string("主机%s光纤应急功能处于关闭状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_REMOTE_DR_OPENED: // 主机远程容灾是打开的
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The RDR function of host %s is opened!");
            }
            else
            {
                Description = string("主机%s远程容灾处于打开状态！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_REMOTE_DR_CLOSED: // 主机远程容灾是关闭的
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The RDR function of host %s is closed!");
            }
            else
            {
                Description = string("主机%s远程容灾处于关闭状态！");
            }
            break;
        }
        // 授权和复制方式
        case BesError::BES_ERR_HOST_NO_NET_LICENSE_WITH_ISCSI_COPY: // 主机没有网启应急授权,不能使用iscsi复制
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host has no NET emergency license, then can not copy with iscsi!");
            }
            else
            {
                Description = string("主机%s没有网启应急授权，不能使用iscsi复制！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_NO_VM_LICENSE_WITH_ISCSI_COPY: // 主机没有虚拟机应急授权，不能使用iscsi复制
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host has no VM emergency license, then can not copy with iscsi!");
            }
            else
            {
                Description = string("主机%s没有虚拟机应急授权，不能使用iscsi复制！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_NO_FC_LICENSE_WITH_FC_COPY: // 主机没有光纤应急授权，不能使用光纤复制
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host has no FC emergency license, then can not copy with FC!");
            }
            else
            {
                Description = string("主机%s没有光纤应急授权，不能使用光纤复制！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_INVALID_COPY_MODE: // 无效的主机复制方式
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy mode of host %s is invalid!");
            }
            else
            {
                Description = string("主机%s复制方式无效！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_INVALID_STORAGE_SPACE: // 无效的存储空间
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The storage space of host %s is invalid!");
            }
            else
            {
                Description = string("主机%s存储空间无效！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_HAS_INITIALIZE_COPYED: // 主机已经做过初始复制
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s has done initializing copy!");
            }
            else
            {
                Description = string("主机%s已经做过初始复制！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_HAS_CDP_PROTECTION: // 主机有CDP保护策略
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s has cdp protection!");
            }
            else
            {
                Description = string("主机%s有CDP保护策略！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_NO_CDP_PROTECTION: // 主机无CDP保护策略
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s has no cdp protection!");
            }
            else
            {
                Description = string("主机%s无CDP保护策略！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_IP_IS_SERVER_IP: // 主机ip为服务器地址
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host ip %s is server ip!");
            }
            else
            {
                Description = string("主机ip%s是服务器的ip地址！");
            }
            break;
        }
        case BesError::BES_ERR_STORAGE_NOT_EXIST: // 存储空间不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The storage space %s does not exist!");
            }
            else
            {
                Description = string("存储空间%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_STORAGE_NO_SPACE: // 存储空间不足
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The storage space %s has no space!");
            }
            else
            {
                Description = string("存储空间%s不足！");
            }
            break;
        }
        case BesError::BES_ERR_REMOTE_CONNECT_FAILED: // 远端服务器连接失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Connect remote server %s failed!");
            }
            else
            {
                Description = string("连接远端服务器%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_REMOTE_NO_RESPONSE: // 远端服务器无响应
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("No response from remote server %s!");
            }
            else
            {
                Description = string("远端服务器%s没有回应！");
            }
            break;
        }
        case BesError::BES_ERR_REMOTE_RECEIVE_TIMEOUT: // 远端服务器接收超时
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Receiving data is timeout for remote server %s!");
            }
            else
            {
                Description = string("远端服务器%s接收回应超时！");
            }
            break;
        }
        case BesError::BES_ERR_REMOTE_IS_BUSY: // 远端服务器繁忙
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The remote server %s is busy!");
            }
            else
            {
                Description = string("远端服务器%s处于忙碌状态！");
            }
            break;
        }
        case BesError::BES_ERR_REMOTE_TASK_ERROR: // 远端服务器任务错误
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The remote server %s proceeds task error!");
            }
            else
            {
                Description = string("远端服务器处理任务错误%s！");
            }
            break;
        }
        case BesError::BES_ERR_REMOTE_TASK_FAILED:// 远端服务器任务失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The remote server %s proceeds task failed!");
            }
            else
            {
                Description = string("远端服务器处理任务失败%s！");
            }
            break;
        }
        case BesError::BES_ERR_SYNC_COPY_FAILED: // 同步副本失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Syncronize copy %s failed!");
            }
            else
            {
                Description = string("同步副本%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_SOURCE_PATH_IS_EMPTY_OR_NOT_EXIST: // 源路径为空或不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The source path %s is empty or does not exist!");
            }
            else
            {
                Description = string("源路径%s为空或不存在！");
            }
            break;
        }
        case BesError::BES_ERR_DEST_PATH_IS_EMPTY_OR_NOT_EXIST: // 目标路径为空或不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The destination path  %s is empty or does not exist!");
            }
            else
            {
                Description = string("目标路径%s为空或不存在！");
            }
            break;
        }
        case BesError::BES_ERR_INVALID_IP_ADDRESS: // 无效的ip地址
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid ip address %s!");
            }
            else
            {
                Description = string("无效的ip地址%s！");
            }
            break;
        }
        case BesError::BES_ERR_FC_NAME_NOT_EXIST: // 光纤卡不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The fibre channel %s does not exist!");
            }
            else
            {
                Description = string("光纤卡%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_COPY_MODE_NOT_SUPPORTED: // 不支持该复制方式
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The copy mode %s is not supported!");
            }
            else
            {
                Description = string("该复制方式%s不支持！");
            }
            break;
        }

        case BesError::BES_ERR_NIC_FOR_PROTECTED_NOT_SET_FOR_VM: // 用于保护的网卡没有用于虚拟机启动
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The protected network adapter %s is not set for VM!");
            }
            else
            {
                Description = string("用于保护的网卡%s没有设置用于虚拟机启动！");
            }
            break;
        }
        case BES_ERR_FILESYSTEM_NOT_SUPPORTED_OPERATION: // 该文件系统不支持此操作
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The file system %s does not support the operation!");
            }
            else
            {
                Description = string("该文件系统%s不支持此操作！");
            }
            break;
        }
        case BES_ERR_FILESYSTEM_IS_INVALID: // 无效的文件系统
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid file system %s!");
            }
            else
            {
                Description = string("无效的文件系统%s！");
            }
            break;
        }
        case BesError::BES_ERR_FILESYSTEM_NOT_MATCH_DEVICE: // 设备和文件系统不匹配
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The device %s does not match filesystem!");
            }
            else
            {
                Description = string("设备%s和文件系统不匹配！");
            }
            break;
        }
        case BesError::BES_ERR_DEVICE_NOT_EXIST: // 设备不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The device %s does not exist!");
            }
            else
            {
                Description = string("设备%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_DEVICE_NOT_MOUNTED: // 设备未挂载
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The device %s is not mounted!");
            }
            else
            {
                Description = string("设备%s未挂载！");
            }
            break;
        }
        case BesError::BES_ERR_DEVICE_HAS_MOUNTED: // 设备已经挂载
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The device %s has been mounted!");
            }
            else
            {
                Description = string("设备%s已经挂载！");
            }
            break;
        }
        case BesError::BES_ERR_DEVICE_ADD_FAILED: // 设备增加失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Add device %s failed！");
            }
            else
            {
                Description = string("增加设备%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DEVICE_DELETE_FAILED:// 设备删除失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete device %s failed！");
            }
            else
            {
                Description = string("删除设备%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DEVICE_MOUNT_FAILED:// 设备挂载失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Mount device %s failed!");
            }
            else
            {
                Description = string("挂载设备%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DEVICE_UNMOUNT_FAILED:// 设备卸载失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Unmount device %s failed!");
            }
            else
            {
                Description = string("卸载设备%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_GROUP_NOT_EXIST: // 组不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The group %s does not exist!");
            }
            else
            {
                Description = string("组%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_GROUP_ADD_FAILED: // 添加组失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Add group %s failed!");
            }
            else
            {
                Description = string("增加组%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_GROUP_DELETE_FAILED:// 删除组失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete group %s failed!");
            }
            else
            {
                Description = string("删除组%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_LUN_NOT_EXIST: // lun不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The lun %s does not exist!");
            }
            else
            {
                Description = string("Lun%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_LUN_ADD_FAILED: // 增加lun失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Add lun %s failed!");
            }
            else
            {
                Description = string("增加lun%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_LUN_DELETE_FAILED: // 删除lun失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete lun %s failed!");
            }
            else
            {
                Description = string("删除lun%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_INITIATOR_NOT_EXIST: // initiator不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The initiator %s does not exist!");
            }
            else
            {
                Description = string("Initiator%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_INITIATOR_ADD_FAILED: // 增加initiator失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Add initiator %s failed!");
            }
            else
            {
                Description = string("增加initiator%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_INITIATOR_DELETE_FAILED: // 删除initiator失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete initiator %s failed!");
            }
            else
            {
                Description = string("删除initiator%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_CHECKPOINT_NOT_EXIST: // checkpoint不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The checkpoint %s does not exist!");
            }
            else
            {
                Description = string("Checkpoint%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_CHECKPOINT_TYPE_IS_INVALID: // checkpoint类型错误
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The type of checkpoint %s is invalid!");
            }
            else
            {
                Description = string("Checkpoint%s类型错误！");
            }
            break;
        }
        case BesError::BES_ERR_DHCP_INVALID_SUBNET_RANGE: // 无效的dhcp子网范围
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid dhcp subnet %s range!");
            }
            else
            {
                Description = string("无效的dhcp子网范围%s！");
            }
            break;
        }
        case BesError::BES_ERR_STRATEGY_NO_CDP_PROTECTION: // 策略没有进行CDP保护
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The strategy %s does not have CDP protection!");
            }
            else
            {
                Description = string("策略%s没有CDP保护！");
            }
            break;
        }
        case BesError::BES_ERR_STRATEGY_MAX_SNAPSHOT_IS_INVALID: // 无效的策略快照数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The snapshot number of strategy %s is invalid!");
            }
            else
            {
                Description = string("策略%s的快照数无效！");
            }
            break;
        }
        case BesError::BES_ERR_STRATEGY_NOT_SYSTEM_TYPE: // 策略不是系统复制类型
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The strategy %s is not for system copy!");
            }
            else
            {
                Description = string("策略%s不是系统复制类型！");
            }
            break;
        }
        case BesError::BES_ERR_STRATEGY_NOT_DATA_TYPE: // 策略不是数据复制类型
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The strategy %s is not for data copy!");
            }
            else
            {
                Description = string("策略%s不是数据复制类型！");
            }
            break;
        }
        case BesError::BES_ERR_STRATEGY_NOT_RAC_TYPE: // 策略不是rac类型
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The type of strategy %s is not for RAC!");
            }
            else
            {
                Description = string("策略%s不是RAC类型！");
            }
            break;
        }
        case BesError::BES_ERR_STRATEGY_NOT_DB_TYPE: // 策略不是数据库备份类型
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The strategy %s is not of the type of database backup!");
            }
            else
            {
                Description = string("策略%s不是数据库备份类型！");
            }
            break;
        }
        case BesError::BES_ERR_NOTIFY_HOST_INITIALIZE_COPY_PREPARE_FAILED: // 通知主机初始复制预处理失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Notify host %s to prepare initilizing copy failed!");
            }
            else
            {
                Description = string("通知主机%s初始复制预处理失败！");
            }
            break;
        }
        case BesError::BES_ERR_NOTIFY_HOST_REMOVE_CDP_FAILED: // 通知主机删除cdp保护失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Notify host %s to remove cdp failed!");
            }
            else
            {
                Description = string("通知主机%s删除CDP保护失败！");
            }
            break;
        }
        case BesError::BES_ERR_NOTIFY_HOST_RAC_INITIALIZE_COPY_FAILED: // 通知主机RAC初始复制失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Notify host %s to do RAC initilizing copy failed!");
            }
            else
            {
                Description = string("通知主机%sRAC初始复制失败！");
            }
            break;
        }
        case BesError::BES_ERR_NOTIFY_HOST_RAC_RECOVER_FAILED: // 通知主机RAC恢复失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Notify host %s to do RAC recovery failed!");
            }
            else
            {
                Description = string("通知主机%sRAC恢复失败！");
            }
            break;
        }
        case BesError::BES_ERR_RAC_GROUP_NOT_EXIST: // RAC组不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The RAC group %s does not exist!");
            }
            else
            {
                Description = string("RAC组%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_RAC_DISK_NOT_EXIST: // RAC磁盘不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The RAC disk %s does not exist!");
            }
            else
            {
                Description = string("RAC磁盘%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_RAC_GROUP_NOT_MATCH_HOST: // RAC组编号和主机的rac组编号不一致
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Rac group id does not match with host rac group id %s!");
            }
            else
            {
                Description = string("RAC组编号和主机的rac组编号%s不一致！");
            }
            break;
        }
        case BesError::BES_ERR_CDP_OVER_CONFIG_NUMBER: // 超出CDP配置个数
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The current CDP number is over the config number %s!");
            }
            else
            {
                Description = string("现有的CDP个数已超出配置个数%s！");
            }
            break;
        }
        case BesError::BES_ERR_CDP_OVER_CONFIG_KEEPTIME: // 超出CDP配置的连续快照时间
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The current CDP keep time is over the config keep time %s!");
            }
            else
            {
                Description = string("现有的CDP连续快照时间已超出配置的连续快照时间%s！");
            }
            break;
        }
        case BesError::BES_ERR_CDP_NOT_FOUND_RECOVERY_POINT: // 找不到CDP快照恢复点
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Not found CDP recovery point %s!");
            }
            else
            {
                Description = string("找不到CDP快照恢复点%s！");
            }
            break;
        }
        case BesError::BES_ERR_CDP_NOT_SUPPORT_SET_EMERGENCY_COPY: // CDP不支持指定应急副本
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The CDP does not support the operation to set emgency copy %s!");
            }
            else
            {
                Description = string("CDP不支持指定应急副本操作%s！");
            }
            break;
        }
        case BesError::BES_ERR_CDP_PORT_NOT_FOUND: // 未找到可用的CDP端口
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Not found available CDP port %s!");
            }
            else
            {
                Description = string("未找到可用的CDP端口%s！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_VOLUME_SPACE_NO_CHANGE: // 磁盘空间大小未改变
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The volume space has no change %s!");
            }
            else
            {
                Description = string("磁盘空间大小未改变%s！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_VOLUME_SPACE_LESS_THAN_USED_SPACE: // 磁盘空间小于已用空间
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The new volume space is less than the used space %s!");
            }
            else
            {
                Description = string("磁盘空间小于磁盘已用空间%s！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_EA_OPENED: // 主机自动应急已打开
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host %s has opened automatically emergency!");
            }
            else
            {
                Description = string("主机%s自动应急已打开！");
            }
            break;
        }
        case BesError::BES_ERR_HOST_EA_NOT_OPENED: // 主机自动应急未打开
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The host does not open automatically emergency!");
            }
            else
            {
                Description = string("主机%s自动应急未打开！");
            }
            break;
        }
        case BesError::BES_ERR_HEARTBEAT_NOTIFY_FAILED: // 心跳通知失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The heartbeat notify %s failed!");
            }
            else
            {
                Description = string("心跳通知%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_LOCAL_BACKUP_NOT_EXIST:// 本机备份记录不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The local backup %s does not exist!");
            }
            else
            {
                Description = string("本机备份记录%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_LOCAL_BACKUP_STATE_IS_INVALID:// 无效的备份状态
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid backup state %s！");
            }
            else
            {
                Description = string("无效的备份状态%s！");
            }
            break;
        }
        case BesError::BES_ERR_MAC_ADDRESS_IS_INVALID: // 无效的mac地址
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Invalid mac address %s!");
            }
            else
            {
                Description = string("无效的mac地址%s！");
            }
            break;
        }
        case BesError::BES_ERR_BEDEAMON_NOT_RUNNING: // 守护进程没有运行
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The daemon %s is not running!");
            }
            else
            {
                Description = string("守护进程%s没有运行！");
            }
            break;
        }
        case BesError::BES_ERR_DRHDEAMON_NOT_RUNNING: // 心跳守护进程没有运行
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The heatbeat daemon %s is not running!");
            }
            else
            {
                Description = string("心跳守护进程%s没有运行！");
            }
            break;
        }
        case BesError::BES_ERR_RDRTASKEXEC_NOT_RUNNING: // 异地容灾任务执行器没有运行
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The RdrTaskExec %s is not running!");
            }
            else
            {
                Description = string("异地容灾任务执行器%s没有运行！");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_BEGIN: // 开始自检
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("******************** BEGIN CHECK ********************");
            }
            else
            {
                Description = string("******************** 开 始 自 检 ********************");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_END:// 结束自检
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("******************** END CHECK ********************");
            }
            else
            {
                Description = string("******************** 自 检 结 束 ********************");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_LICENSE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s license");
            }
            else
            {
                Description = string("检查%s授权");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_LOG:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s log file");
            }
            else
            {
                Description = string("检查%s日志");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_STORAGE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s storage");
            }
            else
            {
                Description = string("检查%s存储");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_PING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check ping");
            }
            else
            {
                Description = string("检查%sping");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_DATABASE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s database");
            }
            else
            {
                Description = string("检查%s数据库");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_PERMISSION:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s permission");
            }
            else
            {
                Description = string("检查%s权限");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_TARGET:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s target");
            }
            else
            {
                Description = string("检查%starget");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_PACKAGE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s package");
            }
            else
            {
                Description = string("检查%s安装包");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_PORT:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s port");
            }
            else
            {
                Description = string("检查%s端口");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_SERVICE:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s service");
            }
            else
            {
                Description = string("检查%s服务");
            }
            break;
        }
        case BesError::BES_ERR_SELF_CHECK_ITEM_PROCESS:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("check %s process");
            }
            else
            {
                Description = string("检查%s进程");
            }
            break;
        }
        case BesError::BES_ERR_PROCESS_NOT_EXIST:// 进程不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The process %s does not exist!");
            }
            else
            {
                Description = string("进程%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_PROCESS_NOT_RUNNING: // 进程没有运行
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The process %s is not running!");
            }
            else
            {
                Description = string("进程%s没有运行！");
            }
            break;
        }
        case BesError::BES_ERR_PROCESS_IS_RUNNING: // 进程正在运行
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The process %s is running!");
            }
            else
            {
                Description = string("进程%s正在运行！");
            }
            break;
        }
        case BesError::BES_ERR_PROCESS_START_FAILED:// 启动进程失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Start process %s failed!");
            }
            else
            {
                Description = string("启动进程%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_PROCESS_STOP_FAILED: // 停止进程失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Stop process %s failed!");
            }
            else
            {
                Description = string("停止进程%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_SERVICE_NOT_EXIST: // 服务不存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The service %s does not exist!");
            }
            else
            {
                Description = string("服务%s不存在！");
            }
            break;
        }
        case BesError::BES_ERR_SERVICE_ALREADY_EXIST: // 服务已存在
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The service %s already exists!");
            }
            else
            {
                Description = string("服务%s已存在！");
            }
            break;
        }
        case BesError::BES_ERR_SERVICE_NOT_RUNNING: // 服务没有运行
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The service %s is not running!");
            }
            else
            {
                Description = string("服务%s没有运行！");
            }
            break;
        }
        case BesError::BES_ERR_SERVICE_IS_RUNNING: // 服务正在运行
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The service %s is running!");
            }
            else
            {
                Description = string("服务%s正在运行！");
            }
            break;
        }
        case BesError::BES_ERR_SERVICE_START_FAILED: // 启动服务失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Start service %s failed!");
            }
            else
            {
                Description = string("启动服务%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_SERVICE_STOP_FAILED: // 停止服务失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Stop service %s failed!");
            }
            else
            {
                Description = string("停止服务%s失败！");
            }
            break;
        }

        case BesError::BES_ERR_PACKAGE_IS_INSTALLED: // 程序包已经安装
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The package %s is already installed!");
            }
            else
            {
                Description = string("程序包%s已安装！");
            }
            break;
        }
        case BesError::BES_ERR_PACKAGE_NOT_INSTALLED: // 程序包没有安装
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The package %s is not installed!");
            }
            else
            {
                Description = string("程序包%s没有安装！");
            }
            break;
        }
        case BesError::BES_ERR_PACKAGE_NOT_FOUND: // 程序包没有找到
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("The package %s is not found!");
            }
            else
            {
                Description = string("程序包%s没有找到！");
            }
            break;
        }
        case BesError::BES_ERR_PACKAGE_INSTALL_FAILED: // 安装程序包失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Install package %s failed!");
            }
            else
            {
                Description = string("安装程序包%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_PACKAGE_UNINSTALL_FAILED: // 卸载程序包失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Uninstall package %s failed!");
            }
            else
            {
                Description = string("卸载程序包%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_PORT_IS_OPENED: // 端口已打开
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Port %s is opened!");
            }
            else
            {
                Description = string("端口%s已打开！");
            }
            break;
        }
        case BesError::BES_ERR_PORT_NOT_OPENED:// 端口未打开
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Port %s is not opened!");
            }
            else
            {
                Description = string("端口%s未打开！");
            }
            break;
        }
        case BesError::BES_ERR_SUBVOLUME_CREATE_FAILED: // 创建子卷失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create subvolume %s failed!");
            }
            else
            {
                Description = string("创建子卷%失败！");
            }
            break;
        }
        case BesError::BES_ERR_SUBVOLUME_DELETE_FAILED: // 删除子卷失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Delete subvolume %s failed!");
            }
            else
            {
                Description = string("删除子卷%s失败！");
            }
            break;
        }

        case BesError::BES_ERR_LOOP_DEVICE_BIND_FAILED: // 绑定loop设备失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Bind loop device %s failed!");
            }
            else
            {
                Description = string("绑定loop设备%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_LOOP_DEVICE_UNBIND_FAILED: // 解除loop设备绑定失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Unbind loop device %s failed!");
            }
            else
            {
                Description = string("解除loop设备绑定%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DRBD_CREATE_RESOURCE_FAILED: // 创建资源失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create resource %s failed!");
            }
            else
            {
                Description = string("创建资源%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DRBD_CREATE_METADATA_FAILED: // 创建元数据失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Create meta-data %s failed!");
            }
            else
            {
                Description = string("创建元数据%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DRBD_ENABLE_RESOURCE_FAILED: // 启用资源失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Enable resource %s failed!");
            }
            else
            {
                Description = string("启用资源%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DRBD_DISABEL_RESOURCE_FAILED: // 停止资源失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Disable resource %s failed!");
            }
            else
            {
                Description = string("停止资源%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_PLAN_EMERGENCY_FAILED: // 预案应急失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Plan emergency %s failed!");
            }
            else
            {
                Description = string("预案应急%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_PLAN_EMERGENCY_STOP_FAILED: // 关闭预案应急失败
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Stop plan emergency %s failed!");
            }
            else
            {
                Description = string("关闭预案应急%s失败！");
            }
            break;
        }
        case BesError::BES_ERR_DOCKER_CREATE_IMAGE_FAILED:
        {
        	if (lang == DESCRIPTION_LANG_EN) Description = string("create docker image failed");
        	else Description = string("创建DOCKER镜像失败");
        	break;
        }
        case BesError::BES_ERR_DOCKER_EMERGENCY_FAILED:
        {
        	if (lang == DESCRIPTION_LANG_EN) Description = string("docker emergency failed");
        	else Description = string("DOCKER应急失败");
        	break;
        }
        case BesError::BES_ERR_DOCKER_IMAGE_NOT_EXIST:
        {
        	if (lang == DESCRIPTION_LANG_EN) Description = string("docker image not exist");
        	else Description = string("DOCKER镜像不存在");
        	break;
        }
        case BesError::BES_ERR_DOCKER_HAS_CONTAINER:
        {
        	if (lang == DESCRIPTION_LANG_EN) Description = string("docker image has container");
        	else Description = string("DOCKER镜像存在正在运行的容器");
        	break;
        }
        case BesError::BES_ERR_DOCKER_CONTAINER_NOT_EXIST:
        {
        	if (lang == DESCRIPTION_LANG_EN) Description = string("docker container not exist");
        	else Description = string("DOCKER容器不存在");
        	break;
        }
		case BesError::BES_ERR_DOG_CHECK_FAILED:
        {
        	if (lang == DESCRIPTION_LANG_EN) Description = string("dog check failed");
        	else Description = string("加密狗检测失败");
        	break;
        }
		case BesError::BES_ERR_CHANGE_HOST_IP_ADDRESS_FAILED:
        {
        	if (lang == DESCRIPTION_LANG_EN) Description = string("change host ip address failed");
        	else Description = string("更改主机IP地址失败");
        	break;
        }
    }
    return Description;
}

/*
 *  功能：
 *      获取自检结果描述
 *  参数
 *      lang        :   语言
 *      result      :   自检结果
 *  返回：
 *      自检结果描述
 */
string BesError::GetSelfCheckResultDescription(DescriptionLang lang, SelfCheckResult result)
{
    string Description = DEFAULT_EMPTY_STRING;
    switch (result)
    {
        case BesError::SELF_CHECK_RESULT_NONE:
        {
            Description = DEFAULT_EMPTY_STRING;
            break;
        }
        case BesError::SELF_CHECK_RESULT_SUCCESS:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("OK");
            }
            else
            {
                Description = string("成功");
            }
            break;
        }
        case BesError::SELF_CHECK_RESULT_FAILED:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Failed");
            }
            else
            {
                Description = string("失败");
            }
            break;
        }
        case BesError::SELF_CHECK_RESULT_NOT_RUNNING:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Not running");
            }
            else
            {
                Description = string("没有运行");
            }
            break;
        }
        case BesError::SELF_CHECK_RESULT_FIXED:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Fixed");
            }
            else
            {
                Description = string("已修复");
            }
            break;
        }
        case BesError::SELF_CHECK_RESULT_NOT_EXIST:
        {
            if (lang == DESCRIPTION_LANG_EN)
            {
                Description = string("Not exist");
            }
            else
            {
                Description = string("不存在");
            }
            break;
        }
    }
    return Description;
}

