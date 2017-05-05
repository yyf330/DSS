/* 
 * File:   DrAlarm.cpp
 * Author: root
 * 
 * Created on 2012年12月15日, 上午10:39
 */

#include "dralarm.h"

DrAlarm::DrAlarm()
{

}

DrAlarm::~DrAlarm()
{
}

//DrAlarm::AlarmLevel DrAlarm::GetAlarmLevel(DrAlarm::AlarmType type)
//{
//
//}
//
//string DrAlarm::GetAlarmName(DrAlarm::AlarmType type)
//{
//
//}
//
//string DrAlarm::GetAlarmDescription(DrAlarm::AlarmType type)
//{
//
//}

DrAlarm::AlarmUnit DrAlarm::GetAlarmUnit(DrAlarm::AlarmType type)
{
    AlarmUnit TempAlarm;
    TempAlarm.Type = type;
    switch (type)
    {
        case DrAlarm::ALARM_TYPE_ADD_TO_MONITOR_QUEUE:// 加入监控队列
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("监控队列");
            TempAlarm.Description = string("加入监控队列...");
            break;
        }
        case DrAlarm::ALARM_TYPE_ADD_TO_MONITOR_QUEUE_FAILED: // 加入监控队列失败
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_ERROR;
            TempAlarm.Name = string("监控队列");
            TempAlarm.Description = string("加入监控队列失败");
            break;
        }
        case DrAlarm::ALARM_TYPE_ADD_TO_MONITOR_QUEUE_SUCCESS: // 加入监控队列成功
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("监控队列");
            TempAlarm.Description = string("加入监控队列成功");
            break;
        }
        case DrAlarm::ALARM_TYPE_REMOVE_FROM_MONITOR_QUEUE: // 从监控队列移除
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("监控队列");
            TempAlarm.Description = string("从监控队列移除...");
            break;
        }
        case DrAlarm::ALARM_TYPE_REMOVE_FROM_MONITOR_QUEUE_FAILED: // 从监控队列移除失败
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_ERROR;
            TempAlarm.Name = string("监控队列");
            TempAlarm.Description = string("从监控队列移除失败");
            break;
        }
        case DrAlarm::ALARM_TYPE_REMOVE_FROM_MONITOR_QUEUE_SUCCESS: // 从监控队列移除成功
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("监控队列");
            TempAlarm.Description = string("从监控队列移除成功");
            break;
        }
        case DrAlarm::ALARM_TYPE_HEARTBEAT_MONITOR_START: // 心跳监控开始
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("心跳监测");
            TempAlarm.Description = string("心跳监测开始...");
            break;
        }
        case DrAlarm::ALARM_TYPE_HEARTBEAT_MONITOR_FAILED: // 心跳监控失败
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_ERROR;
            TempAlarm.Name = string("心跳监测");
            TempAlarm.Description = string("心跳监测失败");
            break;
        }
        case DrAlarm::ALARM_TYPE_HEARTBEAT_MONITOR_RETRY: // 重试心跳
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("心跳监测");
            TempAlarm.Description = string("重试心跳...");
            break;
        }
        case DrAlarm::ALARM_TYPE_OTHER_MONITOR_ITEM_START: // 开始检查其他监测项
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("检查其他监测项");
            TempAlarm.Description = string("其他监测项检查开始...");
            break;
        }
        case DrAlarm::ALARM_TYPE_OTHER_MONITOR_ITEM_SUCCESS: // 其他监测项检查成功
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("检查其他监测项");
            TempAlarm.Description = string("其他监测项检查成功");
            break;
        }
        case DrAlarm::ALARM_TYPE_OTHER_MONITOR_ITEM_FAILED: // 其他监测项检查失败
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
            TempAlarm.Name = string("检查其他监测项");
            TempAlarm.Description = string("其他监测项检查失败");
            break;
        }
        case DrAlarm::ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_START: // 开始自动接管
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_ATTENTION;
            TempAlarm.Name = string("自动接管");
            TempAlarm.Description = string("开始自动接管...");
            break;
        }
        case DrAlarm::ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_STARTVM: // 开始启动虚拟机
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("自动接管");
            TempAlarm.Description = string("开始启动虚拟机...");
            break;
        }
        case DrAlarm::ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_STARTVM_FAILED: // 启动虚拟机失败
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_ERROR;
            TempAlarm.Name = string("自动接管");
            TempAlarm.Description = string("启动虚拟机失败");
            break;
        }
        case DrAlarm::ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_STARTVM_SUCCESS: // 启动虚拟机成功
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_INFORMATION;
            TempAlarm.Name = string("自动接管");
            TempAlarm.Description = string("启动虚拟机成功");
            break;
        }
        case DrAlarm::ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_STARTVM_RETRY: // 重试启动虚拟机
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
            TempAlarm.Name = string("自动接管");
            TempAlarm.Description = string("重试启动虚拟机...");
            break;
        }
        case DrAlarm::ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_FAILED: // 自动接管失败
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_ERROR;
            TempAlarm.Name = string("自动接管");
            TempAlarm.Description = string("虚拟机应急失败");
            break;
        }
        case DrAlarm::ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_SUCCESS: // 自动接管成功
        {
            TempAlarm.Level = DbDataManager::ALARM_LEVEL_ATTENTION;
            TempAlarm.Name = string("自动接管");
            TempAlarm.Description = string("虚拟机应急成功");
            break;
        }
        case DrAlarm::ALARM_TYPE_CPU_ABNORMAL:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("CPU监测");
        	TempAlarm.Description = string("使用率异常:");
        	break;
        }
        case DrAlarm::ALARM_TYPE_CPU_RESTORE:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("CPU监测");
        	TempAlarm.Description = string("使用率恢复正常:");
        	break;
        }
        case DrAlarm::ALARM_TYPE_MEM_ABNORMAL:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("内存监测");
        	TempAlarm.Description = string("使用率异常:");
        	break;
        }
        case DrAlarm::ALARM_TYPE_MEM_RESTORE:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("内存监测");
        	TempAlarm.Description = string("使用率恢复正常:");
        	break;
        }
        case DrAlarm::ALARM_TYPE_DISK_ABNORMAL:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("磁盘监测");
        	TempAlarm.Description = string("使用率异常:");
        	break;
        }
        case DrAlarm::ALARM_TYPE_DISK_RESTORE:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("磁盘监测");
        	TempAlarm.Description = string("使用率恢复正常:");
        	break;
        }
        case DrAlarm::ALARM_TYPE_SERVICE_ABNORMAL:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("服务监测");
        	TempAlarm.Description = string("服务停止:");
        	break;
        }
        case DrAlarm::ALARM_TYPE_SERVICE_RESTORE:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("服务监测");
        	TempAlarm.Description = string("服务恢复运行:");
        	break;
        }
		case DrAlarm::ALARM_TYPE_PROCESS_ABNORMAL:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("进程监测");
        	TempAlarm.Description = string("进程停止:");
        	break;
        }
        case DrAlarm::ALARM_TYPE_PROCESS_RESTORE:
        {
        	TempAlarm.Level = DbDataManager::ALARM_LEVEL_WARNING;
        	TempAlarm.Name = string("进程监测");
        	TempAlarm.Description = string("进程恢复运行:");
        	break;
        }
        default:
        {
            break;
        }
    }

    return TempAlarm;
}

