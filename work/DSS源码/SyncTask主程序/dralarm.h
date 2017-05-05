/* 
 * File:   DrAlarm.h
 * Author: root
 *
 * Created on 2012年12月15日, 上午10:39
 */

#ifndef DRALARM_H
#define	DRALARM_H
#include "begenerallib.h"
#include "dbdatamanager.h"

class DrAlarm
{
public:

    // 报警类型

    enum AlarmType
    {
        ALARM_TYPE_NONE = 0,
        ALARM_TYPE_ADD_TO_MONITOR_QUEUE, // 加入监控队列
        ALARM_TYPE_ADD_TO_MONITOR_QUEUE_FAILED, // 加入监控队列失败
        ALARM_TYPE_ADD_TO_MONITOR_QUEUE_SUCCESS, // 加入监控队列成功
        ALARM_TYPE_REMOVE_FROM_MONITOR_QUEUE, // 从监控队列移除
        ALARM_TYPE_REMOVE_FROM_MONITOR_QUEUE_FAILED, // 从监控队列移除失败
        ALARM_TYPE_REMOVE_FROM_MONITOR_QUEUE_SUCCESS, // 从监控队列移除成功
        ALARM_TYPE_HEARTBEAT_MONITOR_START, // 心跳监控开始
        ALARM_TYPE_HEARTBEAT_MONITOR_FAILED, // 心跳监控失败
        ALARM_TYPE_HEARTBEAT_MONITOR_RETRY, // 重试心跳
        ALARM_TYPE_OTHER_MONITOR_ITEM_START, // 开始检查其他监测项
        ALARM_TYPE_OTHER_MONITOR_ITEM_SUCCESS, // 其他监测项检查成功
        ALARM_TYPE_OTHER_MONITOR_ITEM_FAILED, // 其他监测项检查失败
        ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_START, // 开始自动接管
        ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_STARTVM, // 开始启动虚拟机
        ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_STARTVM_FAILED, // 启动虚拟机失败
        ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_STARTVM_SUCCESS, // 启动虚拟机成功
        ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_STARTVM_RETRY, // 重试启动虚拟机
        ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_FAILED, // 自动接管失败
        ALARM_TYPE_AUTOMATIC_TAKE_CONTROL_SUCCESS, // 自动接管成功
        ALARM_TYPE_CPU_ABNORMAL, // CPU使用率超出指定范围
        ALARM_TYPE_CPU_RESTORE, // CPU使用率回到指定范围
        ALARM_TYPE_MEM_ABNORMAL, // 内存使用率超出指定范围
        ALARM_TYPE_MEM_RESTORE, // 内存使用率回到指定范围
        ALARM_TYPE_DISK_ABNORMAL, // 磁盘使用率超出指定范围
        ALARM_TYPE_DISK_RESTORE, // 磁盘使用率回到指定范围
        ALARM_TYPE_SERVICE_ABNORMAL, // 服务为停止状态
        ALARM_TYPE_SERVICE_RESTORE, // 服务恢复运行状态
		ALARM_TYPE_PROCESS_ABNORMAL,
		ALARM_TYPE_PROCESS_RESTORE,
    };

    // 报警单元
    struct AlarmUnit
    {
        AlarmType Type;
        DbDataManager::AlarmLevel Level;
        string Name;
        string Description;
    };

public:

    /*
     *  功能：
     *      构造函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    DrAlarm();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~DrAlarm();
public:
    //    static AlarmLevel GetAlarmLevel(DrAlarm::AlarmType type);
    //    static string GetAlarmName(DrAlarm::AlarmType type);
    //    static string GetAlarmDescription(DrAlarm::AlarmType type);

    /*
     *  功能：
     *      根据报警类型获取报警单元
     *  参数：
     *      type            :   报警类型
     *  返回：
     *      报警单元
     */
    static AlarmUnit GetAlarmUnit(DrAlarm::AlarmType type);

private:

};

#endif	/* DRALARM_H */