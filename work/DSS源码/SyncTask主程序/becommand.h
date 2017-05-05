/* 
 * File:   becommand.h
 * Author: root
 *
 * Created on 2012年5月25日, 下午4:15
 */

/*================================== 修改列表 ==================================//
 * 20121218 latis   1)增加了通知主机删除CDP保护（514）命令字
 * 
//================================== 修改结束 ==================================*/


#ifndef BECOMMAND_H
#define	BECOMMAND_H

#include "commandunit.h"

class BeCommand
{
public:

    enum CommandSet
    {
        COMMAND_SET_UNKNOWN = -1, // 未知
        COMMAND_SET_QUERY = 0, // 查询类
        COMMAND_SET_COPY = 1, // 复制类
        COMMAND_SET_RECOVER = 2, // 恢复类
        COMMAND_SET_DISK_OPERATION = 3, // 磁盘操作类
        COMMAND_SET_HOST_OPERATION = 4, // 主机操作类
        COMMAND_SET_NOTIFY = 5, // 通知类
        COMMAND_SET_INTERACTIVE = 6, // 联动类
        COMMAND_SET_MANAGE = 9, // 管理类
    };

    enum CommandCode
    {
        COMMAND_CODE_UNKNOWN = -1,

        COMMAND_CODE_START_EXTRACTOR = 1, // 启动抓取
        COMMAND_CODE_STOP_EXTRACTOR = 2, // 停止抓取
        COMMAND_CODE_START_LOADER = 3, // 启动装载
        COMMAND_CODE_STOP_LOADER = 4, // 停止装载

        COMMAND_CODE_QUERY_EXTRACTOR_STATE = 5, // 上报挖取进程状态
        COMMAND_CODE_QUERY_LOADER_STATE = 6, // 上报装载进程状态

        COMMAND_CODE_ADD_SOURCEDB = 7, // 增加源库
        COMMAND_CODE_DEL_SOURCEDB = 8, // 删除源库
        COMMAND_CODE_ADD_STRATEGY = 9, // 增加策略
        COMMAND_CODE_DEL_STRATEGY = 10, // 删除策略

        COMMAND_CODE_QUERY_HOST_PROCESS = 11, // 查询主机进程
        COMMAND_CODE_REPORT_CPU_AND_MEMORY_INFO = 31, // 上报CPU和内存信息（参数:主机编码;CPU信息;内存信息;)
        COMMAND_CODE_REPORT_DISK_INFO = 32, // 上报磁盘信息（参数:主机编码;磁盘1信息;磁盘2信息;...)
        COMMAND_CODE_REPORT_SERVICE_INFO = 33, //上报服务信息（参数:主机编码;服务1信息;服务2信息;...)
        COMMAND_CODE_REPORT_PROCESS_INFO = 34, //上报进程信息（参数:主机编码;进程1信息;进程2信息;...)

        COMMAND_CODE_WINDOWS_INITIALIZE_COPY = 102, // windows初始复制
        COMMAND_CODE_WINDOWS_SNAPSHOT_COPY = 104, // windows快照复制
        COMMAND_CODE_WINDOWS_DATABASE_COPY = 107, // windows数据库复制
        COMMAND_CODE_FULL_COPY = 108, // 全量复制
        COMMAND_CODE_START_CDP = 111, // 启动CDP
        COMMAND_CODE_STOP_CDP = 112, // 停止CDP
        COMMAND_CODE_SYNC_CDP = 113, // CDP同步
        COMMAND_CODE_FILES_SNAPSHOT_COPY = 114, // 文件快照复制
        COMMAND_CODE_LINUX_INITIALIZE_COPY = 121, // linux初始复制
        COMMAND_CODE_LINUX_SNAPSHOT_COPY = 122, // linux快照复制
        COMMAND_CODE_LINUX_DATABASE_COPY = 124, // linux数据库复制
        COMMAND_CODE_SCHEDULE_SNAPSHOT_COPY = 123, // 调度快照复制
        COMMAND_CODE_AIX_INITIALIZE_COPY = 131, // aix初始复制
        COMMAND_CODE_AIX_SNAPSHOT_COPY = 132, // aix快照复制
        COMMAND_CODE_AIX_SCHEDULE_SNAPSHOT_COPY = 133, // 调度快照复制
        COMMAND_CODE_AIX_DATABASE_COPY = 134, // aix数据库复制

        COMMAND_CODE_WINDOWS_SYSTEM_RECOVER = 202, // windows系统盘恢复
        COMMAND_CODE_WINDOWS_DATA_RECOVER = 203, // windows数据盘恢复
        COMMAND_CODE_WINDOWS_SEPCIFIC_DATA_RECOVER = 204, // windows指定盘符恢复
        COMMAND_CODE_WINDOWS_MULTIPLE_FILES_RECOVER = 205, // windows多文件恢复
        COMMAND_CODE_WINDOWS_SEPCIFIC_FILE_RECOVER = 206, // windows指定文件恢复
        COMMAND_CODE_LINUX_SYSTEM_RECOVER = 223, // linux系统盘恢复
        COMMAND_CODE_LINUX_DATA_RECOVER = 224, // linux数据盘恢复
        COMMAND_CODE_LINUX_FULL_DISK_RECOVER = 225, // linux全盘恢复
        COMMAND_CODE_AIX_DATA_RECOVER = 231, // AIX数据恢复
        COMMAND_CODE_SYSTEM_RAW_RECOVER = 241, // 系统裸盘恢复

        COMMAND_CODE_MOUNT_NET_DISK = 302, // 挂载网络盘
        COMMAND_CODE_UMOUNT_NET_DISK = 304, // 卸载网络盘

        //
        COMMAND_CODE_REBOOT_HOST = 401, // 重启主机
        COMMAND_CODE_START_HOST_SERVICE = 403, // 启动主机服务
        COMMAND_CODE_STOP_HOST_SERVICE = 404, // 关闭主机服务

        COMMAND_CODE_NOTIFY_HOST_BREAK_TASK = 501, // 通知主机中断任务
        COMMAND_CODE_NOTIFY_HOST_PORT_MODIFICATION = 502, // 通知主机端口修改
        COMMAND_CODE_NOTIFY_HOST_ENTER_BACKUP_MODE = 503, // 通知主机进入备份模式
        COMMAND_CODE_NOTIFY_HOST_EXIT_BACKUP_MODE = 504, // 通知主机退出备份模式
        COMMAND_CODE_NOTIFY_HOST_BACKUP_REGISTER = 505, // 通知主机备份注册表
        COMMAND_CODE_NOTIFY_HOST_SNAPSHOT_PREPARE = 506, // 通知主机快照预处理
        COMMAND_CODE_NOTIFY_HOST_SNAPSHOT_POST = 507, // 通知主机快照后处理
        COMMAND_CODE_NOTIFY_HOST_INITIALIZE_COPY_PREPARE = 508, // 通知主机初始复制预处理
        COMMAND_CODE_NOTIFY_HOST_STARTEA = 509, // 通知主机启动自动应急
        COMMAND_CODE_NOTIFY_HOST_STOPEA = 510, // 通知主机停止自动应急
        COMMAND_CODE_NOTIFY_ADD_HEARTBEAT_HOST = 511, // 通知增加心跳主机
        COMMAND_CODE_NOTIFY_REMOVE_HEAERTBEAT_HOST = 512, // 通知删除心跳主机
        COMMAND_CODE_NOTIFY_HOST_EA_CONFIG_CHANGED = 513, // 通知主机自动应急配置修改
        COMMAND_CODE_NOTIFY_HOST_REMOVE_CDP = 514, // 删除CDP保护
        COMMAND_CODE_NOTIFY_HOST_RAC_INITIALIZE_COPY = 515, // 通知主机rac初始复制（参数：主机编码;策略id;）
        COMMAND_CODE_NOTIFY_HOST_RAC_RECOVER = 516, // 通知主机rac恢复（参数：主机编码;源rac磁盘id;目标rac磁盘id）
        COMMAND_CODE_NOTIFY_HOST_OPEN_MONITOR = 517, // 通知主机打开监控（参数：主机编码）
        COMMAND_CODE_NOTIFY_HOST_CLOSE_MONITOR = 518, // 通知主机关闭监控（参数：主机编码）
        COMMAND_CODE_NOTIFY_HOST_CDP_PORT = 519, // 通知主机cdp端口 （参数：主机编码;策略id;端口号;）
        COMMAND_CODE_NOTIFY_MONITOREA_PARAMETER = 520, // 自动应急参数修改 （参数：主机编码;）
        COMMAND_CODE_NOTIFY_MONITOR_PARAMETER = 521, // 监控参数修改 （参数：主机编码;）
        COMMAND_CODE_ADD_DB_IMAGE_PROTECT = 531,  // 添加数据库镜像保护    （参数:主机编码;镜像服务器IP;镜像数据库IP;镜像类型;)
        COMMAND_CODE_START_DB_IMAGE_PROTECT = 532,  // 通知数据库镜像应急    （参数:主机编码;镜像服务器IP;镜像数据库IP;镜像类型;)
        COMMAND_CODE_STOP_DB_IMAGE_PROTECT = 533,  // 通知取消数据库镜像应急    （参数:主机编码;镜像服务器IP;镜像数据库IP;镜像类型;)
        COMMAND_CODE_NOTIFY_HOST_SHUTDOWN = 544, // 通知主机软关机 （参数：主机编码;）

        COMMAND_CODE_QUERY_BES_STATE = 601, // 查询BES状态
        COMMAND_CODE_OPERATE_BES_HOST = 603, // 操作BES主机
        COMMAND_CODE_OPERATE_VM_HOST = 604, // 操作虚拟主机
        COMMAND_CODE_HEARTBEAT_START_VM_HOST = 605, // 心跳操作虚拟主机
        COMMAND_CODE_SYNC_HOST_INFORMATION = 651, // 同步主机信息（参数：远程服务器id;本地ServerId;主机编码;）
        COMMAND_CODE_SYNC_HOST_COPY = 652, // 同步主机副本（参数：远程服务器id;本地ServerId;主机编码;挂载点;副本类型;本地副本编号;）
        COMMAND_CODE_CREATE_REMOTE_COPY = 653, // 创建远程副本（参数：主机编码;策略id;副本类型;本地副本编号;）
        COMMAND_CODE_CREATE_REMOTE_SNAPSHOT = 654, // 创建远程快照（参数:主机编码;副本id;）
        COMMAND_CODE_CHANGE_REMOTE_SNAPSHOT_NAME = 655, // 修改远程快照名（参数:远程服务器id;主机编码;快照id;）
        COMMAND_CODE_CHANGE_SNAPSHOT_NAME = 656, // 修改快照名（参数:主机编码;快照id;）
        COMMAND_CODE_CREATE_DOCKER_IMAGE = 661, // 创建docker镜像（参数:主机编码;副本编号;镜像名称;)
        COMMAND_CODE_DELETE_DOCKER_IMAGE = 662, // 删除docker镜像（参数:镜像ID;)
        COMMAND_CODE_START_DOCKER_EMERGENCY = 663, // docker应急（参数:主机编码;镜像ID;)
        COMMAND_CODE_STOP_DOCKER_EMERGENCY = 664, // 取消docker应急（参数:主机编码;容器ID;)

        COMMAND_CODE_EXECUTE_TASK = 901, // 执行任务
        COMMAND_CODE_RESCAN_TARGET = 902, // 重新扫描target
        COMMAND_CODE_SET_LICENSE = 903, // 设置主机授权
        COMMAND_CODE_QUERY_LICENSE = 904, // 查询授权信息
        COMMAND_CODE_FORCE_TERMINATE_TASK = 905, // 强制终止任务
        COMMAND_CODE_OPEN_HOST_EA = 906, // 打开主机自动应急
        COMMAND_CODE_CLOSE_HOST_EA = 907, // 关闭主机自动应急
        COMMAND_CODE_CHANGE_HOST_EA_CONFIG = 908, // 修改主机自动应急配置
        COMMAND_CODE_CHANGE_HWEMERGENCY_CONFIG = 909, // 修改硬件应急参数
        COMMAND_CODE_CHANGE_HWEMERGENCY_HOST = 910, // 修改硬件应急主机
        COMMAND_CODE_REBOOT_BES = 911, // 重启BES
        COMMAND_CODE_SHUTDOWN_BES = 912, // 关闭BES
        COMMAND_CODE_UPGRADE_BES = 913, // 升级BES
        COMMAND_CODE_ROLLBACK_BES = 914, // 回滚BES
        COMMAND_CODE_UPDATE_LICENSE = 915, // 更新license
        COMMAND_CODE_SET_SYSTEM_TIME = 916, // 设置系统时间
        COMMAND_CODE_CHANGE_SERVER_ID = 917, // 修改设备编号（参数：设备编号）
        COMMAND_CODE_QUERY_ALL_VERSIONS = 918, // 查询全部版本（参数：无）
        COMMAND_CODE_CHANGE_COPY_MODE = 919, // 修改复制方式（参数:主机编码;复制方式;光纤名;wwn）
        COMMAND_CODE_DO_TIMING_SNAPSHOT = 920, // 处理定时快照（参数:无）
        COMMAND_CODE_SELF_CHECK = 921, // 自检（参数：无）
        COMMAND_CODE_SET_DHCP = 922, // 设置DHCP
        COMMAND_CODE_SET_BUSINESS_NIC_IP = 923, // 设置主机业务网卡IP
        COMMAND_CODE_START_PLAN_EMERGENCY = 924, // 预案应急（参数:主机编码;预案号;）
        COMMAND_CODE_STOP_PLAN_EMERGENCY = 925, // 关闭预案应急（参数:主机编码;预案号;）
        COMMAND_CODE_DELETE_SNAPSHOT = 941, // 删除快照
        COMMAND_CODE_CREATE_NORMAL_COPY = 942, // 创建标准副本
        COMMAND_CODE_DELETE_COPY = 943, // 删除副本
        COMMAND_CODE_SET_EMERGENCY_COPY = 944, // 指定应急副本
        COMMAND_CODE_CREATE_FAST_COPY = 945, // 创建快速副本
        COMMAND_CODE_SET_TRUSTED_SNAPSHOT = 946, // 指定可信快照
        COMMAND_CODE_CANCEL_TRUSTED_SNAPSHOT = 947, // 取消可信快照
        COMMAND_CODE_CREATE_CDP_SNAPSHOT = 948, // 生成CDP快照
        COMMAND_CODE_MODIFY_HOST_VOLUME_SPACE = 952, // 修改主机卷空间
        COMMAND_CODE_DELETE_WINDOWS_VOLUME_SPACE = 953, // 删除windows主机卷空间
        COMMAND_CODE_ADD_AIX_VOLUME_SPACE = 956, // 增加aix主机卷空间
        COMMAND_CODE_DELETE_HOST = 961, // 删除主机
        COMMAND_CODE_RESET_HOST = 962, // 重置主机
        COMMAND_CODE_ADD_HOST = 963, // 增加主机
        COMMAND_CODE_ADD_HOST_DATABASE_PROTECTION = 964, // 增加主机数据库保护
        COMMAND_CODE_DELETE_HOST_DATABASE_PROTECTION = 965, // 删除主机数据库保护
        COMMAND_CODE_CHANGE_BOOT_PROTOCOL = 966,
        COMMAND_CODE_DELETE_LINUX_VOLUME_SPACE = 967, // 删除linux主机卷空间
        COMMAND_CODE_FORCE_DELETE_HOST = 968, // 强制删除主机
        COMMAND_CODE_DELETE_STRATEGY_VOLUME = 969, // 删除策略卷
        COMMAND_CODE_REMOTE_SYNCHRONIZATION_STARTER = 970, // 远程同步启动器
        COMMAND_CODE_ADD_AIX_HOST = 971, // 增加aix主机
        COMMAND_CODE_START_VM = 972, // 启动虚拟机
        COMMAND_CODE_STOP_VM = 973, // 关闭虚拟机
        COMMAND_CODE_CHANGE_HOST_PORT = 974, // 修改主机端口号
        COMMAND_CODE_CANCEL_AIX_EMERGENCY_COPY = 975, // 取消aix应急副本
        COMMAND_CODE_SET_ROUTE = 976, // 设置路由
        COMMAND_CODE_START_VM_ONE_KEY = 977, // 一键虚拟机应急
        COMMAND_CODE_CHANGE_HOST_BUSINESS_NIC = 978, // 修改主机业务网卡名
        COMMAND_CODE_CHANGE_HOST_BRIDGE_NIC = 979, // 修改主机桥接网卡名
        COMMAND_CODE_SET_TRUSTED_COPY = 980, // 指定主机可信副本
        COMMAND_CODE_CANCEL_TRUSTED_COPY = 981, // 取消主机可信副本
        COMMAND_CODE_SWITCH_HOST_LOCAL_EMERGENCY = 982, // 主机本地应急状态切换
        COMMAND_CODE_SWTICH_GROUP_LOCAL_EMERGENCY = 983, // 全组主机本地应急状态切换
        COMMAND_CODE_CHANGE_GROUP_BUSINESS_NIC = 984, // 全组主机修改业务网卡
        COMMAND_CODE_CHANGE_GROUP_BRIDGE_NIC = 985, // 全组主机修改桥接网卡
        COMMAND_CODE_SET_FC_CONFIGURATION = 986, // 修改主机光纤卡配置
        COMMAND_CODE_STOP_VM_ONE_KEY = 987, // 一键虚拟机关闭
        COMMAND_CODE_CHANGE_HOST_MAC_ADDRESS = 988, // 修改主机MAC地址
        COMMAND_CODE_CHANGE_CDP_PROTECTION = 990, // 修改CDP保护
		COMMAND_CODE_CHANGE_HOST_IP_ADDRESS = 991, //修改主机IP地址

    };

    // 命令类型

    enum CommandType
    {
        COMMAND_TYPE_UNKNOWN,
        COMMAND_TYPE_COMMAND, // 命令
        COMMAND_TYPE_RESPONSE, // 回应
    };

    enum CommandResult
    {
        COMMAND_RESULT_UNKNOWN,
        COMMAND_RESULT_FINISH, // 命令完成
        COMMAND_RESULT_ERROR, // 命令失败
        COMMAND_RESULT_PROCESSING, // 命令处理中
        COMMAND_RESULT_BUSY, // Agent繁忙
        COMMAND_RESULT_FAIL, // Agent繁忙
        COMMAND_RESULT_ACK, // 确认
    };

    typedef struct _CommandStruct
    {
        string Header;
        string Tailor;
        CommandType Type;
        string Code;
        CommandResult Result;
        string ParaOrContent;
    } CommandStruct;
public:

    /*
     *  功能：
     *      默认构造函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    BeCommand();

    /*
     *  功能：
     *      默认析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~BeCommand();
public:

    /*
     *  功能：
     *      创建标准格式命令
     *  参数：
     *      type            :   类型
     *      code            :   命令字
     *      parameter       :   参数
     *  返回：
     *      命令内容
     */
    static string CreateStandardCommand(string type, string code, string parameter);

    /*
     *  功能：
     *      创建标准格式命令
     *  参数：
     *      header          :   命令头
     *      type            :   类型
     *      code            :   命令字
     *      parameter       :   参数
     *  返回：
     *      命令内容
     */
    static string CreateStandardCommand(string header, string type, string code, string parameter);

    /*
     *  功能：
     *      创建命令参数
     *  参数：
     *      num             :   参数个数
     *      ...             :   可变参数
     *  返回：
     *      命令参数
     */
    static string CreateCommandParameter(int num, ...);

    /*
     *  功能：
     *      获取命令字所属类别
     *  参数：
     *      cmdcodestring   :   命令字
     *  返回：
     *      命令字所属类别
     */
    static BeCommand::CommandSet GetCommandSet(string cmdcodestring);

    /*
     *  功能：
     *      获取命令字枚举标识返回命令字字符串
     *  参数：
     *      cmdcode         :   命令字枚举量
     *  返回：
     *      命令字字符串
     */
    static string GetCommandCodeString(BeCommand::CommandCode cmdcode);

    /*
     *  功能：
     *      根据命令字字符串返回命令字枚举标识
     *  参数：
     *      cmdcodestring   :   命令字
     *  返回：
     *      命令字枚举标识
     */
    static BeCommand::CommandCode GetCommandCode(string cmdcodestring);

    /*
     *  功能：
     *      解析结果
     *  参数：
     *      cmdstring       :   命令
     *      cmd             :   命令结构
     *  返回：
     *      解析成功返回true，否则返回false
     */
    static bool Parse(string cmdstring, CommandStruct *cmd);

    /*
     *  功能：
     *      初始化命令结构
     *  参数：
     *      cmd             :   命令结构
     *  返回：
     *      无
     */
    static void InitCommandStruct(CommandStruct *cmd);

    /*
     *  功能：
     *      是否为windows或linux复制命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsWindowsOrLinuxCopyCommand(string cmdcode);

    /*
     *  功能：
     *      是否为windows或linux初始复制命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsWindowsOrLinuxInitCopyCommand(string cmdcode);

    /*
     *  功能：
     *      是否为windows或linux快照复制命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsWindowsOrLinuxSnapshotCopyCommand(string cmdcode);

    /*
     *  功能：
     *      是否为windows恢复命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsWindowsRestoreCommand(string cmdcode);

    /*
     *  功能：
     *      是否为linux恢复命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsLinuxRestoreCommand(string cmdcode);

    /*
     *  功能：
     *      是否为windows或linux恢复命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsWindowsOrLinuxRestoreCommand(string cmdcode);

    /*
     *  功能：
     *      是否为unix复制命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsUnixCopyCommand(string cmdcode);

    /*
     *  功能：
     *      是否为远程同步命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsRemoteSyncCommand(string cmdcode);

    /*
     *  功能：
     *      是否为监控上报命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsMonitorReportCommand(string cmdcode);

    /*
     *  功能：
     *      是否为应急命令
     *  参数：
     *      cmdcode         :   命令字
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsEmergencyCommand(string cmdcode);
public:

    /*
     *  功能：
     *      获取指定命令字的参数个数
     *  参数：
     *      cmdcode             :   命令字
     *  返回：
     *      如果成功返回参数个数，否则返回-1
     */
    virtual int GetParameterCount(string cmdcode);

    /*
     *  功能：
     *      检查命令字是否有效
     *  参数：
     *      cmdcode             :   命令字
     *  返回：
     *      如果有效返回true，否则返回false
     */
    virtual bool IsCommandCodeValid(string cmdcode);

    /*
     *  功能：
     *      获取命令对象
     *  参数：
     *      cmdcode             :   命令字
     *  返回：
     *      成功返回命令对象，否则返回NULL
     */
    virtual CommandUnit * GetCommandObject(string cmdcode);

private:
    map<string, CommandUnit*> MyCommandCollection;
private:
#define DEFAULT_QUERY_COMMAND_PREFIX                            "0"                 // 查询类命令字前缀

#define DEFAULT_COMMAND_START_EXTRACTOR                         "001"               // 启动抓取                 （参数：源库id;）
#define DEFAULT_COMMAND_STOP_EXTRACTOR                          "002"               // 停止抓取                 （参数：源库id;）
#define DEFAULT_COMMAND_START_LOADER                            "003"               // 启动装载                 （参数：策略id;）
#define DEFAULT_COMMAND_STOP_LOADER                             "004"               // 停止装载                 （参数：策略id;）

#define DEFAULT_COMMAND_QUERY_EXTRACTOR_STATE                   "005"               // 上报挖取进程状态         （参数：源库号;状态;）
#define DEFAULT_COMMAND_QUERY_LOADER_STATE                      "006"               // 上报装载进程状态         （参数：策略号;状态;）

#define DEFAULT_COMMAND_ADD_SOURCEDB                            "007"               // 增加源库                 （参数：DBTYPE;SIP;INSTANCENAME;PORT;USERNAME;PASSWORD;TSNAME;TRUNK_SCN;BRANCH_SCN;INTERVAL;）
#define DEFAULT_COMMAND_DEL_SOURCEDB                            "008"               // 删除源库                 （参数：源库id;)
#define DEFAULT_COMMAND_ADD_STRATEGY                            "009"               // 增加策略                 （参数：STRATEGY_NAME;SID;DID;SCN;SYNC_USERS;INTERVAL;ERRORSKIP;AUTOSTART;）
#define DEFAULT_COMMAND_DEL_STRATEGY                            "010"               // 删除策略                 （参数：策略id;）

#define DEFAULT_COMMAND_QUERY_HOST_PROCESS                      "011"               // 查询主机进程             （参数：主机编码;）
#define DEFAULT_COMMAND_REPORT_CPU_AND_MEMORY_INFO              "031"               // 上报CPU和内存信息        （参数:主机编码;CPU信息;内存信息;)
#define DEFAULT_COMMAND_REPORT_DISK_INFO                        "032"               // 上报磁盘信息             （参数:主机编码;磁盘1信息;磁盘2信息;...)
#define DEFAULT_COMMAND_REPORT_SERVICE_INFO						"033"				// 上报服务信息             （参数:主机编码;服务1信息;服务2信息;...)
#define DEFAULT_COMMAND_REPORT_PROCESS_INFO						"034"				// 上报进程信息             （参数:主机编码;进程1信息;进程2信息;...)

#define DEFAULT_COPY_COMMAND_PREFIX                             "1"                 // 复制类命令字前缀
#define DEFAULT_COMMAND_WINDOWS_INITIALIZE_COPY                 "102"               // windows初始复制          （参数：主机编码;盘符;）
#define DEFAULT_COMMAND_WINDOWS_SNAPSHOT_COPY                   "104"               // windows快照复制          （参数：主机编码;盘符;）
#define DEFAULT_COMMAND_WINDOWS_DATABASE_COPY                   "107"               // windows数据库复制        （参数：主机编码;盘符;）
#define DEFAULT_COMMAND_FULL_COPY                               "108"               // 全量复制                 （参数：主机编码;策略id;是否创建新副本）
#define DEFAULT_COMMAND_START_CDP                               "111"               // 启动CDP                  （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_STOP_CDP                                "112"               // 停止CDP                  （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_SYNC_CDP                                "113"               // CDP同步                  （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_FILES_SNAPSHOT_COPY                     "114"               // 文件快照复制             （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_LINUX_INITIALIZE_COPY                   "121"               // linux初始复制            （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_LINUX_SNAPSHOT_COPY                     "122"               // linux快照复制            （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_LINUX_DATABASE_COPY                     "124"               // linux数据库复制          （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_SCHEDULE_SNAPSHOT_COPY                  "123"               // 调度快照复制             （参数：主机编码;策略id;调度计划id;）
#define DEFAULT_COMMAND_AIX_INITIALIZE_COPY                     "131"               // aix初始复制              （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_AIX_SNAPSHOT_COPY                       "132"               // aix快照复制              （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_AIX_DATABASE_COPY                       "134"               // aix数据库复制            （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_AIX_SCHEDULE_SNAPSHOT_COPY              "133"               // 调度快照复制             （参数：主机编码;策略id;调度计划id;）

#define DEFAULT_RECOVER_COMMAND_PREFIX                          "2"                 // 恢复类命令字
#define DEFAULT_COMMAND_WINDOWS_SYSTEM_RECOVER                  "202"               // windows系统盘恢复        （参数：主机编码;盘符;）
#define DEFAULT_COMMAND_WINDOWS_DATA_RECOVER                    "203"               // windows数据盘恢复        （参数：主机编码;网络盘符;主机盘符;）
#define DEFAULT_COMMAND_WINDOWS_SPECIFIC_DATA_RECOVER           "204"               // windows指定盘符恢复      （参数：主机编码;源盘符;目标盘符;）
#define DEFAULT_COMMAND_WINDOWS_MULTIPLE_FILES_RECOVER          "205"               // windows多文件恢复        （参数：主机编码;策略id;源盘符;目标盘符;）
#define DEFAULT_COMMAND_WINDOWS_SPECIFIC_FILE_RECOVER           "206"               // windows指定文件恢复      （参数：主机编码;源目录;源文件;目标目录;）
#define DEFAULT_COMMAND_LINUX_SYSTEM_RECOVER                    "223"               // linux系统盘恢复          （参数：主机编码;硬盘;）
#define DEFAULT_COMMAND_LINUX_DATA_RECOVER                      "224"               // linux数据盘恢复          （参数：主机编码;策略id;路径;全路径标志;）
#define DEFAULT_COMMAND_LINUX_FULL_DISK_RECOVER                 "225"               // linux全盘恢复            （参数：主机编码;硬盘;）
#define DEFAULT_COMMAND_AIX_DATA_RECOVER                        "231"               // AIX数据恢复              （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_SYSTEM_RAW_RECOVER                      "241"               // 系统裸盘恢复             （参数：主机编码;目标磁盘;）
#define DEFAULT_COMMAND_RAC_ANY_DISK_RECOVER                    "243"               // rac任意盘恢复（参数：主机编码；源rac磁盘id;目标rac磁盘id）

#define DEFAULT_DISK_OPERATION_COMMAND_PREFIX                   "3"                 // 盘符操作类命令字前缀
#define DEFAULT_COMMAND_MOUNT_NET_DISK                          "302"               // 挂载网络盘               （参数：主机编码;iqn;）
#define DEFAULT_COMMAND_UMOUNT_NET_DISK                         "304"               // 卸载网络盘               （参数：主机编码;iqn;）

#define DEFAULT_HOST_CONTROL_COMMAND_PREFIX                     "4"                 // 主机控制类命令字前缀
#define DEFAULT_COMMAND_REBOOT_HOST                             "401"               // 重启主机                 （参数：主机编码;）
#define DEFAULT_COMMAND_START_HOST_SERVICE                      "403"               // 启动主机服务             （参数：主机编码;）
#define DEFAULT_COMMAND_STOP_HOST_SERVICE                       "404"               // 关闭主机服务             （参数：主机编码;）

#define DEFAULT_NOTIFY_COMMAND_PREFIX                           "5"                 // 通知类命令字前缀
#define DEFAULT_COMMAND_NOTIFY_HOST_BREAK_TASK                  "501"               // 通知主机中断任务         （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_HOST_PORT_MODIFICATION           "502"               // 通知主机端口修改         （参数：主机编码;端口）
#define DEFAULT_COMMAND_NOTIFY_HOST_ENTER_BACKUP_MODE           "503"               // 通知主机进入备份模式     （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_HOST_EXIT_BACKUP_MODE            "504"               // 通知主机退出备份模式     （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_HOST_BACKUP_REGISTER             "505"               // 通知主机备份注册表       （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_HOST_SNAPSHOT_PREPARE            "506"               // 通知主机快照预处理       （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_NOTIFY_HOST_SNAPSHOT_POST               "507"               // 通知主机快照后处理       （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_NOTIFY_HOST_INITIALIZE_COPY_PREPARE     "508"               // 通知主机初始复制预处理   （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_NOTIFY_HOST_STARTEA                     "509"               // 通知主机启动自动应急     （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_HOST_STOPEA                      "510"               // 通知主机停止自动应急     （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_ADD_HEARTBEAT_HOST               "511"               // 通知增加心跳主机         （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_REMOVE_HEAERTBEAT_HOST           "512"               // 通知删除心跳主机         （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_HOST_EA_CONFIG_CHANGED           "513"               // 通知主机自动应急配置修改  （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_HOST_REMOVE_CDP                  "514"               // 通知主机删除CDP保护      （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_NOTIFY_HOST_RAC_INITIALIZE_COPY         "515"               // 通知主机rac初始复制      （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_NOTIFY_HOST_RAC_RECOVER                 "516"               // 通知主机rac恢复          （参数：主机编码；源rac磁盘id;目标rac磁盘id）
#define DEFAULT_COMMAND_NOTIFY_HOST_OPEN_MONITOR                "517"               // 通知主机打开监控         （参数：主机编码）
#define DEFAULT_COMMAND_NOTIFY_HOST_CLOSE_MONITOR               "518"               // 通知主机关闭监控         （参数：主机编码）
#define DEFAULT_COMMAND_NOTIFY_HOST_CDP_PORT                    "519"               // 通知主机cdp端口          （参数：主机编码;策略id;端口号;）
#define DEFAULT_COMMAND_NOTIFY_MONITOREA_PARAMETER				"520"				// 自动应急参数修改      （参数：主机编码;）
#define DEFAULT_COMMAND_NOTIFY_MONITOR_PARAMETER				"521"				// 监控参数修改      （参数：主机编码;）
#define DEFAULT_COMMAND_ADD_DB_IMAGE_PROTECT					"531"				// 添加数据库镜像保护    （参数:主机编码;镜像服务器IP;镜像数据库IP;镜像类型;)
#define DEFAULT_COMMAND_START_DB_IMAGE_PROTECT					"532"				// 通知数据库镜像应急    （参数:主机编码;镜像服务器IP;镜像数据库IP;镜像类型;)
#define DEFAULT_COMMAND_STOP_DB_IMAGE_PROTECT					"533"				// 通知取消数据库镜像应急    （参数:主机编码;镜像服务器IP;镜像数据库IP;镜像类型;)
#define DEFAULT_COMMAND_NOTIFY_HOST_SHUTDOWN					"544"				// 通知主机软关机          （参数：主机编码;）

#define DEFAULT_INTERACT_COMMAND_PREFIX                         "6"                 // 联动命令字前缀
#define DEFAULT_COMMAND_QUERY_BES_STATE                         "601"               // 查询BES状态              （参数：用户名;密码）
#define DEFAULT_COMMAND_OPERATE_BES_HOST                        "603"               // 操作BES主机              （参数：用户名;密码;主机编码;动作;）
#define DEFAULT_COMMAND_OPERATE_VM_HOST                         "604"               // 操作虚拟主机             （参数：用户名;密码;主机编码;动作;）
#define DEFAULT_COMMAND_HEARTBEAT_START_VM_HOST                 "605"               // 心跳操作虚拟主机         （参数：主机编码;动作;）
#define DEFAULT_COMMAND_SYNC_HOST_INFORMATION                   "651"               // 同步主机信息             （参数：远程服务器id;本地ServerId;主机编码;）
#define DEFAULT_COMMAND_SYNC_HOST_COPY                          "652"               // 同步主机副本             （参数：远程服务器id;本地ServerId;主机编码;挂载点;副本类型;本地副本编号;）
#define DEFAULT_COMMAND_CREATE_REMOTE_COPY                      "653"               // 创建远程副本             （参数：主机编码;策略id;副本类型;本地副本编号;）
#define DEFAULT_COMMAND_CREATE_REMOTE_SNAPSHOT                  "654"               // 创建远程快照             （参数：主机编码;副本id;）
#define DEFAULT_COMMAND_CHANGE_REMOTE_SNAPSHOT_NAME             "655"               // 修改远程快照名           （参数:主机编码;快照id;发送端快照id;）
#define DEFAULT_COMMAND_CHANGE_SNAPSHOT_NAME                    "656"               // 修改快照名               （参数:主机编码;快照id;）
#define DEFAULT_COMMAND_CREATE_DOCKER_IMAGE                     "661" 				// 创建docker镜像      （参数:主机编码;副本编号;镜像名称;)
#define DEFAULT_COMMAND_DELETE_DOCKER_IMAGE                     "662"			 	// 删除docker镜像      （参数:镜像ID;)
#define DEFAULT_COMMAND_START_DOCKER_EMERGENCY                  "663" 				// docker应急             （参数:主机编码;镜像ID;)
#define DEFAULT_COMMAND_STOP_DOCKER_EMERGENCY                   "664" 				// 取消docker应急              （参数:主机编码;容器ID;)

#define DEFAULT_MANAGE_COMMAND_PREFIX                           "9"                 // 管理命令字前缀
#define DEFAULT_COMMAND_EXECUTE_TASK                            "901"               // 执行任务                 （参数：无）
#define DEFAULT_COMMAND_RESCAN_TARGET                           "902"               // 重新扫描target           （参数：无）
#define DEFAULT_COMMAND_SET_LICENSE                             "903"               // 设置主机授权             （参数：无）
#define DEFAULT_COMMAND_QUERY_LICENSE                           "904"               // 查询授权信息             （参数：无）
#define DEFAULT_COMMAND_FORCE_TERMINATE_TASK                    "905"               // 强制终止任务             （参数：任务编号）
#define DEFAULT_COMMAND_OPEN_HOST_EA                            "906"               // 打开主机自动应急         （参数：主机编码;）
#define DEFAULT_COMMAND_CLOSE_HOST_EA                           "907"               // 关闭主机自动应急         （参数：主机编码;）
#define DEFAULT_COMMAND_CHANGE_HOST_EA_CONFIG                   "908"               // 修改主机自动应急配置     （参数：主机编码;）
#define DEFAULT_COMMAND_CHANGE_HWEMERGENCY_CONFIG               "909"               // 修改硬件应急参数         （参数：无）
#define DEFAULT_COMMAND_CHANGE_HWEMERGENCY_HOST                 "910"               // 修改硬件应急主机         （参数：无）
#define DEFAULT_COMMAND_REBOOT_BES                              "911"               // 重启BES                  （参数：无）
#define DEFAULT_COMMAND_SHUTDOWN_BES                            "912"               // 关闭BES                  （参数：无）
#define DEFAULT_COMMAND_UPGRADE_BES                             "913"               // 升级BES                  （参数：升级程序文件名）
#define DEFAULT_COMMAND_ROLLBACK_BES                            "914"               // 回滚BES                  （参数：回滚程序名）
#define DEFAULT_COMMAND_UPDATE_LICENSE                          "915"               // 更新license              （参数：License文件名）
#define DEFAULT_COMMAND_SET_SYSTEM_TIME                         "916"               // 设置系统时间             （参数：系统时间）
#define DEFAULT_COMMAND_CHANGE_SERVER_ID                        "917"               // 修改设备编号             （参数：设备编号）
#define DEFAULT_COMMAND_QUERY_ALL_VERSIONS                      "918"               // 查询全部版本（参数：无）
#define DEFAULT_COMMAND_CHANGE_COPY_MODE                        "919"               // 修改复制方式（参数:主机编码;复制方式;光纤名;wwn）
#define DEFAULT_COMMAND_DO_TIMING_SNAPSHOT                      "920"               // 处理定时快照             （参数:无）
#define DEFAULT_COMMAND_SELF_CHECK                              "921"               // 自检                     （参数：无）
#define DEFAULT_COMMAND_SET_DHCP                                "922"               // 设置DHCP                 （参数:无）
#define DEFAULT_COMMAND_SET_BUSINESS_NIC_IP                     "923"               // 设置主机业务网卡IP       （参数：网卡名称）
#define DEFAULT_COMMAND_START_PLAN_EMERGENCY                    "924"               // 预案应急                 （参数:主机编码;预案号;）
#define DEFAULT_COMMAND_STOP_PLAN_EMERGENCY                     "925"               // 关闭预案应急             （参数:主机编码;预案号;）
#define DEFAULT_COMMAND_CHANGE_RAC_MASTER_HOST                  "934"               // 修改rac组的主控主机 (参数:RAC组ID;新MasterHost;)
#define DEFAULT_COMMAND_DELETE_SNAPSHOT                         "941"               // 删除快照                 （参数：主机编码;快照Id;）
#define DEFAULT_COMMAND_CREATE_NORMAL_COPY                      "942"               // 创建标准副本             （参数：主机编码;快照Id;）
#define DEFAULT_COMMAND_DELETE_COPY                             "943"               // 删除副本                 （参数：主机编码;副本Id;）
#define DEFAULT_COMMAND_SET_EMERGENCY_COPY                      "944"               // 指定应急副本             （参数：主机编码;快照Id;）
#define DEFAULT_COMMAND_CREATE_FAST_COPY                        "945"               // 创建快速副本             （参数：主机编码;快照Id;）
#define DEFAULT_COMMAND_SET_TRUSTED_SNAPSHOT                    "946"               // 指定可信快照             （参数：主机编码;快照Id;）
#define DEFAULT_COMMAND_CANCEL_TRUSTED_SNAPSHOT                 "947"               // 取消可信快照             （参数：主机编码;快照Id;）
#define DEFAULT_COMMAND_CREATE_CDP_SNAPSHOT                     "948"               // 生成CDP快照              （参数：主机编码;策略id;时间）
#define DEFAULT_COMMAND_MODIFY_HOST_VOLUME_SPACE                "952"               // 修改主机卷空间           （参数：主机编码;策略Id;新的磁盘空间大小;是否进行全量复制;）
#define DEFAULT_COMMAND_DELETE_WINDOWS_VOLUME_SPACE             "953"               // 删除windows主机卷空间    （参数：主机编码;盘符;）
#define DEFAULT_COMMAND_ADD_AIX_VOLUME_SPACE                    "956"               // 增加aix主机卷空间        （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_DELETE_HOST                             "961"               // 删除主机                 （参数：主机编码;主机ip;）
#define DEFAULT_COMMAND_RESET_HOST                              "962"               // 重置主机                 （参数：主机编码;）
#define DEFAULT_COMMAND_ADD_HOST                                "963"               // 增加主机                 （参数：主机编码;主机名;主机IP;操作系统;网启协议;业务网卡;桥接网卡;复制方式;rac组id;业务光纤卡;WWN;是否仅备份;）
#define DEFAULT_COMMAND_ADD_HOST_DATABASE_PROTECTION            "964"               // 增加主机数据库保护       （参数：主机编码;）
#define DEFAULT_COMMAND_DELETE_HOST_DATABASE_PROTECTION         "965"               // 删除主机数据库保护       （参数：主机编码;）
#define DEFAULT_COMMAND_CHANGE_BOOT_PROTOCOL                    "966"               // 修改启动协议            （参数：主机编码;网启协议;备机ip;）
#define DEFAULT_COMMAND_DELETE_LINUX_VOLUME_SPACE               "967"               // 删除linux主机卷空间      （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_FORCE_DELETE_HOST                       "968"               // 强制删除主机             （参数：主机编码;主机ip;）
#define DEFAULT_COMMAND_DELETE_STRATEGY_VOLUME                  "969"               // 删除策略卷               （参数：主机编码;策略id;）
#define DEFAULT_COMMAND_REMOTE_SYNCHRONIZATION_STARTER          "970"               // 远程同步启动器           （参数：远程服务器ip）
#define DEFAULT_COMMAND_ADD_AIX_HOST                            "971"               // 增加aix主机              （参数：主机编码;主机名;主机IP;操作系统;网启协议;WWN;端口）
#define DEFAULT_COMMAND_START_VM                                "972"               // 启动虚拟机               （参数：主机编码;）
#define DEFAULT_COMMAND_STOP_VM                                 "973"               // 关闭虚拟机               （参数：主机编码;）
#define DEFAULT_COMMAND_CHANGE_HOST_PORT                        "974"               // 修改主机端口号           （参数：主机编码;端口）
#define DEFAULT_COMMAND_CANCEL_AIX_EMERGENCY_COPY               "975"               // 取消aix应急副本          （参数：主机编码;策略id）
#define DEFAULT_COMMAND_SET_ROUTE                               "976"               // 设置路由                 （参数：无）
#define DEFAULT_COMMAND_START_VM_ONE_KEY                        "977"               // 一键虚拟机应急           （参数：主机编码;）
#define DEFAULT_COMMAND_CHANGE_HOST_BUSINESS_NIC                "978"               // 修改主机业务网卡名       （参数：主机编码;网卡名;）
#define DEFAULT_COMMAND_CHANGE_HOST_BRIDGE_NIC                  "979"               // 修改主机桥接网卡名       （参数：主机编码;网卡名;）
#define DEFAULT_COMMAND_SET_TRUSTED_COPY                        "980"               // 指定主机可信副本         （参数：主机编码;副本id;）
#define DEFAULT_COMMAND_CANCEL_TRUSTED_COPY                     "981"               // 取消主机可信副本         （参数：主机编码;副本id;）
#define DEFAULT_COMMAND_SWITCH_HOST_LOCAL_EMERGENCY             "982"               // 主机本地应急状态切换      （参数：主机编码;本地应急状态;）
#define DEFAULT_COMMAND_SWTICH_GROUP_LOCAL_EMERGENCY            "983"               // 全组主机本地应急状态切换 （参数：原始服务器id;本地应急状态;）
#define DEFAULT_COMMAND_CHANGE_GROUP_BUSINESS_NIC               "984"               // 全组主机修改业务网卡     （参数：原始服务器id;网卡名;）
#define DEFAULT_COMMAND_CHANGE_GROUP_BRIDGE_NIC                 "985"               // 全组主机修改桥接网卡     （参数：原始服务器id;网卡名;）
#define DEFAULT_COMMAND_SET_FC_CONFIGURATION                    "986"               // 修改主机光纤卡配置       （参数：光纤卡名）
#define DEFAULT_COMMAND_STOP_VM_ONE_KEY                         "987"               // 一键虚拟机关闭           （参数：主机编码）
#define DEFAULT_COMMAND_CHANGE_HOST_MAC_ADDRESS                 "988"               // 修改主机MAC地址          （参数：主机编码;mac地址）
#define DEFAULT_COMMAND_CHANGE_CDP_PROTECTION                   "990"               // 修改CDP保护状态          （参数：主机编码;策略id;保护状态;）
#define DEFAULT_COMMAND_CHANGE_HOST_IP_ADDRESS                  "991"               // 修改主机IP地址          （参数：主机编码;IP地址）



#define COMMAND_SEGMENT_SEPCHAR                                 (SEPARATOR_CHAR_WELL)       // 命令段分割符
#define COMMAND_TAILOR                                          "\r\n"                      // 命令尾
#define COMMAND_CODE_ID                                         "C"                         // 命令标识
#define COMMAND_RESPONSE_ID                                     "R"                         // 回应标识
#define COMMAND_SEGMENT_NUM                                     5                           // 命令段数
#define COMMAND_CODE_SIZE                                       3                           // 命令字长度
#define COMMAND_HEADER_SIZE                                     14                          // 命令头长度
#define DEFAULT_COMMAND_RESULT_FINISH                           "FINISH"                    // 命令完成
#define DEFAULT_COMMAND_RESULT_ERROR                            "ERROR"                     // 命令失败
#define DEFAULT_COMMAND_RESULT_PROCESSING                       "PROCESSING"                // 命令处理中
#define DEFAULT_COMMAND_RESULT_BUSY                             "BUSY"                      // 忙碌
#define DEFAULT_COMMAND_RESULT_FAIL                             "FAIL"                      // 失败
#define DEFAULT_COMMAND_RESULT_ACK                              "ACK"                       // 确认
private:

    enum CommandSegment
    {
        COMMAND_SEGMENT_HEADER,
        COMMAND_SEGMENT_TYPE,
        COMMAND_SEGMENT_CODE_OR_RESULT,
        COMMAND_SEGMENT_PARAMETER_OR_CONTENT,
        COMMAND_SEGMENT_TAILOR,
    };

private:
    //
    //    /*
    //     *  功能：
    //     *      解析结果
    //     *  参数：
    //     *      cmdstring       :   命令
    //     *      segment         :   段名
    //     *      outvalue        :   输出值
    //     *  返回：
    //     *      解析成功返回true，否则返回false
    //     */
    //    static bool Parse(string cmdstring, BeCommand::CommandSegment segment, string & outvalue);


    /*
     *  功能：
     *      将字符串转换为命令类型
     *  参数：
     *      type            :   命令类型
     *  返回：
     *      命令类型
     */
    static BeCommand::CommandType ConvertToCommandType(string type);

    /*
     *  功能：
     *      将字符串转换为命令结果
     *  参数：
     *      result          :   命令结果
     *  返回：
     *      命令结果
     */
    static BeCommand::CommandResult ConvertToCommandResult(string result);

private:
    /*
     *  功能：
     *      初始化命令集合
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void InitCommandCollection();

    /*
     *  功能：
     *      将命令单元增加到集合
     *  参数：
     *      cmd             :   命令字
     *      paracount       :   参数个数
     *      chnname         :   中文名称
     *      engname         :   英文名称
     *  返回：
     *      成功返回true，否则返回false
     */
    virtual bool AddCommandUnitToCollection(string code, string paradesc, string chnname, string engname);

    /*
     *  功能：
     *      销毁命令集和
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void DestroyCommandCollection();
};

#endif	/* BECOMMAND_H */

