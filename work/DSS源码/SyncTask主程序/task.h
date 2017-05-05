#ifndef _TASK_H
#define	_TASK_H

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/tree.h>
#include <libxml/xmlsave.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "beserror.h"
#include "besgeneralheader.h"
#include "begenerallib.h"
#include "taskprocdesc.h"
#include "beslog.h"
#include "besencryptf.h"
#include "besencryptd.h"
#include "networkmanager.h"
#include "dbdatamanager.h"
#include "inifile.h"
#include "dralarm.h"

using namespace std;

class Task
{
public:

    // 任务执行步骤

    enum TaskExecStep
    {
        TASK_EXEC_STEP_NONE,
        TASK_EXEC_STEP_VALIDATION_CHECK,
        TASK_EXEC_STEP_PRE_PROCESSING,
        TASK_EXEC_STEP_WAIT_PREPARE_RESULT,
        TASK_EXEC_STEP_EXCUTE,
        TASK_EXEC_STEP_WAIT_EXCUTING_RESULT,
        TASK_EXEC_STEP_POST_PROCESSING,
        TASK_EXEC_STEP_ERROR_PROCESSING,
        TASK_EXEC_STEP_GOTO_FINISH,
        TASK_EXEC_STEP_FINISH,
    };

    // 任务执行结果

    enum TaskExecResult
    {
        TASK_EXEC_RESULT_NONE = -1,
        TASK_EXEC_RESULT_FAIL = 0,
        TASK_EXEC_RESULT_SUCCESS = 1,
    };

    // 任务控制

    enum TaskControl
    {
        TASK_CONTROL_START,
        TASK_CONTROL_PAUSE,
        TASK_CONTROL_RESUME,
        TASK_CONTROL_STOP,
    };

    //    // agent返回结果
    //
    //    enum AgentResult
    //    {
    //        AGENT_RESULT_NONE,
    //        AGENT_RESULT_ERROR,
    //        AGENT_RESULT_FAIL,
    //        AGENT_RESULT_PROCESSING,
    //        AGENT_RESULT_BUSY,
    //        AGENT_RESULT_FINISH,
    //    };

public:

    /*
     *  功能：
     *      构造函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    Task();

    /*
     *  功能：
     *      构造函数
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *      fcmanager       :   FC存储管理器
     *      id              :   任务ID
     *      cmdcode         :   指令编码
     *      hostip          :   主机IP
     *      hostport        :   主机端口
     *      priority        :   优先级
     *      maxcounter      :   最大执行次数
     *  返回：
     *      无
     */
    Task(DbDataManager *dbdatamanager, BeCommand *becommand, string id, string cmdheader, string cmdcode, string parameter, string hostcode, string hostip, string hostport, int priority = 2, int maxcounter = 1);

    /*
     *  功能：
     *      构造函数
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *      fcmanager       :   FC存储管理器
     *      cmdcode         :   指令编码
     *      parameter       :   参数
     *      server          :   服务器IP
     *      serverport      :   服务器端口
     *  返回：
     *      无
     */
    Task(DbDataManager *dbdatamanager, BeCommand *becommand, string cmdheader, string cmdcode, string parameter, string serverip, string serverport);

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~Task();

protected:

    /*
     *  功能：
     *      增加任务
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *      fcmanager       :   FC存储管理器
     *      id              :   任务ID
     *      cmdcode         :   指令编码
     *      hostip          :   主机IP
     *      hostport        :   主机端口
     *      priority        :   优先级
     *      maxcounter      :   最大执行次数
     *  返回：
     *      增加成功返回true，否则返回false
     */
    virtual bool Add(DbDataManager *dbdatamanager, BeCommand *becommand, Task* mastertask, string id, string cmdheader, string cmdcode, string parameter, string hostcode, string hostip, string hostport, string serverip = "", string serverport = "", int priority = 2, int maxcounter = 1);

    /*
     *  功能：
     *      获取任务结果
     *  参数：
     *      无
     *  返回：
     *      任务结果
     */
    virtual Task::TaskExecResult Result();
public:

    /*
     *  功能：
     *      启动任务
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Start();

    /*
     *  功能：
     *      暂停任务
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Pause();

    /*
     *  功能：
     *      重新执行任务
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void Resume();

    /*
     *  功能：
     *      停止任务
     *  参数：
     *      notifyflag      :   是否通知标志
     *  返回：
     *      无
     */
    virtual void Stop(bool notifyflag = false);

    /*
     *  功能：
     *      设置Agent响应信息
     *  参数：
     *      rpstr           :   响应信息
     *  返回：
     *      无
     */
    virtual bool SetResponse(string rpstr);

    /*
     *  功能：
     *      发送处理状态
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void SendProcessingState();

    /*
     *  功能：
     *      检查Agent是否超时
     *  参数
     *      无
     *  返回：
     *      无
     */
    virtual void CheckTimeout();
    /*
     *  功能：
     *      解锁信号量
     *  参数
     *      recvstring      :   执行结果
     *  返回：
     *      无
     */
    virtual void Release(string recvstring);

    /*
     *  功能：
     *      是否为空
     *  参数
     *      无
     *  返回：
     *      如果是返回true，否则返回false
     */
    virtual bool Empty();

    /*
     *  功能：
     *      返回任务ID
     *  参数：
     *      无
     *  返回：
     *      任务ID
     */
    virtual string Id();

    /*
     *  功能：
     *      返回命令头
     *  参数：
     *      无
     *  返回：
     *      命令头
     */
    virtual string CommandHeader();

    /*
     *  功能：
     *      返回命令字
     *  参数：
     *      无
     *  返回：
     *      命令字
     */
    virtual string CommandCode();

    /*
     *  功能：
     *      返回命令参数
     *  参数：
     *      无
     *  返回：
     *      命令参数
     */
    virtual string CommandParameter();

    /*
     *  功能：
     *      返回命令对应的主机编码
     *  参数：
     *      无
     *  返回：
     *      主机编码
     */
    virtual string HCode();

    /*
     *  功能：
     *      重载小于符
     *  参数：
     *      task        ：   任务对象
     *  返回：
     *      如果当前任务的优先级小于比较任务的优先级返回true,否则返回false
     */
    virtual bool operator<(const Task& task)const;

    /*
     *  功能：
     *      最近一次错误
     *  返回：
     *      返回最近一次错误
     */
    virtual BesError* LastError();
private:
    TaskControl MyTaskControl; // 任务控制标志
    TaskExecStep MyTaskExecStep; // 当前执行步骤
    TaskExecStep MyLastTaskExecStep; // 上次执行步骤
    TaskExecResult MyTaskExecResult; // 任务执行结果
    bool MyExecWithErrorFlag; // 执行任务是否有错误标志
    Task *MySubTask; // 子任务
    BesError* MyLastError; // 最近一次错误
    bool MyNotifyStopFlag; // 停止时是否通知

    string MyTaskId; // 任务ID
    string MyCmdCode; // 命令编码
    string MyParameter; // 命令参数
    string MyCmdHeader; // 命令头
    string MyHostCode; // 主机编码
    string MyHostIp; // 主机IP
    string MyHostPort; // 主机端口
    string MyCommandStr; // 命令串
    string MyServerIp;
    string MyServerPort;

    bool MyWaitResultOK;
    bool MyStartCheckTimeoutFlag; // 检查超时标志
    time_t MyStartCheckTime; // 开始检查超时时间
    string MyInternalResult; // 内部结果信息
    bool MyIsUpgradeFlag; // 是否为升级
    string MyAgentResponse; // Agent响应信息
    int MyExecCounter; // 执行次数
    int MyMaxCounter; // 最大执行次数
    DbDataManager * MyDbDataManager; // 数据库管理器
    BeCommand * MyCommandManager;
    time_t MyStartTime; // 任务开始时间
    time_t MyEndTime; // 任务结束时间
    time_t StopEALastPingTime;	//仅供510命令使用
    Task* MyMasterTask;

private:
#define DEFAULT_UPGRADE_BIN_NAME                        "BEUpgrade"                     // 升级执行文件
#define DEFAULT_EXTENSION_LENGTH                        4                               // 扩展名长度(含.)
#define DEFAULT_UPGRADE_EXTENSION                       ".zip"                          // 升级文件扩展名
#define DEFAULT_UPGRADE_TYPE_CORE                       "CORE"                          // 内核文件升级
#define DEFAULT_UPGRADE_TYPE_GUI                        "GUI"                           // GUI管理页面升级
#define DEFAULT_UPGRADE_TYPE_SELF                       "UPGRADE"                       // 升级程序自身升级
#define MIN_AGENT_TIME_OUT                              60                              // agnet最小超时时间
#define MAX_AGENT_TIME_OUT                              86400                           // agent最大超时时间
#define DEFAULT_AGENT_TIME_OUT                          300                             // 默认agent超时时间
#define MIN_SNAPSHOT_SPACE_SIZE                         3000                            // 最小快照区空间（MB）
#define DEFAULT_COPY_TIME_OUT                           1440                            // 默认复制超时时间（单位：分钟）
#define DEFAULT_MAKE_INITRD_IMAGE_BIN_NAME              "modify_initrd.sh"              // 修改initrd的shell文件
#define DEFAULT_MAKE_INITRD_IMAGE_BIN_NAME_WITH_PATH1   "/zhiwang/BES/modify_initrd.sh"
#define DEFAULT_MAKE_INITRD_IMAGE_BIN_NAME_WITH_PATH2   "/home/BES/modify_initrd.sh"

#define MAX_SYNC_RETRY_COUNT                            3                               // 最大同步重试次数
#define MAX_CONNECT_AGENT_COUNT                         3                               // 最大连接agent次数
#define INTERNAL_REFRESH_HOST_COMMAND                   "REFRESHHOST"                   // 内部刷新主机命令
#define INTERNAL_REFRESH_HOST_STATE_COMMAND             "REFRESHHOSTSTATE"              // 内部刷新主机状态命令
#define INTERNAL_REFRESH_HWE_CONFIG_COMMAND             "REFRESHHWE"                    // 内部刷新硬件应急参数
#define MAX_MOUNT_RETRY_COUNT                           3                               // 最大mount重试次数
#define MAX_MOUNT_CHECK_TIMEOUT                         60                              // 最大mount检查超时时间(秒）
#define DEFAULT_HOST_CONFIG_PREFIX_LOCAL                "local"                         // 默认主机配置目录前缀（本地)
#define DEFAULT_HOST_CONFIG_PREFIX_NET                  "net"                           // 默认主机配置目录前缀（网络)
#define DEFAULT_HOST_CONFIG_PREFIX_VM                   "virtual"                       // 默认主机配置目录前缀（虚拟机)
#define DEFAULT_HOST_CONFIG_PREFIX_FCBOOT               "fcboot"                        // 默认主机配置目录前缀（光纤)
#define DEFAULT_SPECIAL_STORAGE_ID                      "-999"
#define DEFAULT_SPECIAL_RACGROUP_ID                     "-999"
#define DEFAULT_FC_CHANNEL_NUMBER                       "0"                             // 默认光纤通道号
#define DEFAULT_HOST_HOT_BOOT_FLAG_FILE                 "shotboot.conf"                 // 主机热启标识文件
#define DEFAULT_HOST_HOT_BOOT_SECTION                   "VIRTUALCOMM"                   // 虚拟化通讯段
#define DEFAULT_HOST_HOT_BOOT_KEY_VIRTUALIP             "VIRTUALIP"                     // 虚拟IP地址
#define DEFAULT_HOST_HOT_BOOT_KEY_VIRTUALMAC            "VIRTUALMAC"                    // 虚拟mac地址
#define DEFAULT_HOST_HOT_BOOT_KEY_VIRTUALSERVERIP       "VIRTUALSERVERIP"               // 虚拟服务器地址
#define DEFAULT_HOST_BOOT_VMNETWORK_SECTION             "VMNETWORK"                     // 虚拟网络段
#define DEFAULT_HOST_BOOT_KEY_VMBOOTMODE                "VMBOOTMODE"                    // 虚拟机启动方式
#define DEFAULT_HOST_BOOT_KEY_NETWORKMODE               "NETWORKMODE"                   // 网络工作方式
#define DEFAULT_HOST_BOOT_KEY_MAINVIRTUALMAC            "LOCALVIRTUALMAC"               // 本地虚拟mac地址
#define DEFAULT_HOST_BOOT_KEY_MAINVIRTUALIP             "LOCALVIRTUALIP"                // 本地虚拟ip地址
#define DEFAULT_MBR_FILE_2003                           "MBR2003"
#define DEFAULT_MBR_FILE_2008                           "MBR2008"
#define MAX_MSDOS_PARTITION_SIZE                        2000                            // GB


#define DEFAULT_VM_VOLUME_CONFIG_FILE                   "volume.conf"                   // 虚拟机卷配置文件
#define DEFAULT_VM_VOLUME_SECION                        "VMVOLUME"                      // 卷配置段
#define DEFAULT_VM_VOLUME_KEY_HCODE                     "HCODE"                         // 主机编码
#define DEFAULT_VM_VOLUME_KEY_COPYID                    "COPYID"                        // 副本编号
#define DEFAULT_VM_VOLUME_KEY_LMOUNTPOINT               "LMOUNTPOINT"                   // 挂载点
private:

    enum ActionProcResult
    {
        ACTION_PROC_RESULT_SUCCESS,
        ACTION_PROC_RESULT_FAIL,
        ACTION_PROC_RESULT_WAIT,
        ACTION_PROC_RESULT_ERROR,
    };


    // 主机检查项目

    enum HostCheckItem
    {
        HOST_CHECK_ITEM_HOST_EXIST = (1 << 0),
        HOST_CHECK_ITEM_LOCAL_EMERGENCY_IS_OPENED = (1 << 1),
        HOST_CHECK_ITEM_HCODE_NOT_EXIST = (1 << 2),
        HOST_CHECK_ITEM_HCODE_IS_VALID = (1 << 3),
        HOST_CHECK_ITEM_HOST_IS_WINDOWS = (1 << 4),
        HOST_CHECK_ITEM_HOST_IS_LINUX = (1 << 5),
        HOST_CHECK_ITEM_HOST_IS_WINDOWS_OR_LINUX = (1 << 6),
        HOST_CHECK_ITEM_HOST_IS_UNIX = (1 << 7),
        HOST_CHECK_ITEM_COPY_USED = (1 << 8),
        HOST_CHECK_ITEM_DATABASE_IS_PROTECTED = (1 << 9),
        HOST_CHECK_ITEM_SYSTEM_VOLUME_IS_PROTECTED = (1 << 10),
        HOST_CHECK_ITEM_NET_EMERGENCY_IS_OPENED = (1 << 11),
        HOST_CHECK_ITEM_VM_EMERGENCY_IS_OPENED = (1 << 12),
        HOST_CHECK_ITEM_FC_EMERGENCY_IS_OPENED = (1 << 13),
        HOST_CHECK_ITEM_EA_IS_NOT_OPENED = (1 << 14),
        HOST_CHECK_ITEM_EA_IS_OPENED = (1 << 15),
        HOST_CHECK_ITEM_HAS_CDP_PROTECTION = (1 << 16),
        HOST_CHECK_ITEM_NO_CDP_PROTECTION = (1 << 17),
        HOST_CHECK_ITEM_TRUSTED_COPY_EXIST = (1 << 18),
        HOST_CHECK_ITEM_VM_RESOURCE = (1 << 19),
		HOST_CHECK_ITEM_HOST_IN_DOG = (1 << 20),
    };

    // 授权检查项目

    enum LicenseCheckItem
    {
        LICENSE_CHECK_ITEM_HOST_NUMBER = (1 << 0),
        LICENSE_CHECK_ITEM_DB_NUMBER = (1 << 1),
        LICENSE_CHECK_ITEM_NET_EMERGENCY_NUMBER = (1 << 2),
        LICENSE_CHECK_ITEM_NET_EMERGENCY_NUMBER_EXCEPT_SELF = (1 << 3),
        LICENSE_CHECK_ITEM_VM_EMERGENCY_NUMBER = (1 << 4),
        LICENSE_CHECK_ITEM_VM_EMERGENCY_NUMBER_EXCEPT_SELF = (1 << 5),
        LICENSE_CHECK_ITEM_FC_EMERGENCY_NUMBER = (1 << 6),
        LICENSE_CHECK_ITEM_FC_EMERGENCY_NUMBER_EXCEPT_SELF = (1 << 7),
        LICENSE_CHECK_ITEM_CDP_NUMBER = (1 << 8),
        LICENSE_CHECK_ITEM_CDP_NUMBER_EXCEPT_SELF = (1 << 9),
        LICENSE_CHECK_ITEM_CDP_KEEPTIME = (1 << 10),
        LICENSE_CHECK_ITEM_CDP_KEEPUNIT = (1 << 11),
        LICENSE_CHECK_ITEM_EA_NUMBER = (1 << 12),
        LICENSE_CHECK_ITEM_EA_NUMBER_EXCEPT_SELF = (1 << 13),
    };

    // 主机状态检查项

    enum HostStateCheckItem
    {
        HOST_STATE_CHECK_ITEM_IS_LOCAL = (1 << 0),
        HOST_STATE_CHECK_ITEM_IS_OFFLINE = (1 << 1),
        HOST_STATE_CHECK_ITEM_NOT_OFFLINE = (1 << 2),
        HOST_STATE_CHECK_ITEM_IS_EMERGENCY = (1 << 3),
        HOST_STATE_CHECK_ITEM_NOT_EMERGENCY = (1 << 4),
        HOST_STATE_CHECK_ITEM_IS_VM_EMERGENCY = (1 << 5),
        HOST_STATE_CHECK_ITEM_IS_NET_EMERGENCY = (1 << 6),
        HOST_STATE_CHECK_ITEM_IS_FC_EMERGENCY = (1 << 7),
    };

    // 副本检查项目

    enum CopyCheckItem
    {
        COPY_CHECK_ITEM_EXIST = (1 << 0),
        COPY_CHECK_ITEM_NOT_EXIST = (1 << 1),
        COPY_CHECK_ITEM_NOT_USED = (1 << 2),
        COPY_CHECK_ITEM_NOT_SNAPSHOT_MODE = (1 << 3),
        COPY_CHECK_ITEM_NOT_EMERGENCY_COPY = (1 << 4),
        COPY_CHECK_ITEM_NOT_TRUSTED_COPY = (1 << 5),
        COPY_CHECK_ITEM_NOT_LAST_NORMAL_COPY = (1 << 6),
        COPY_CHECK_ITEM_HAS_SNAPSHOT = (1 << 7),
        COPY_CHECK_ITEM_IS_EMERGENCY_COPY = (1 << 8),
    };

    // 卷检查项目

    enum VolumeCheckItem
    {
        VOLUME_CHECK_ITEM_EXIST = (1 << 0),
        VOLUME_CHECK_ITEM_HAS_STRATEGY = (1 << 1),
        VOLUME_CHECK_ITEM_NO_COPY_USED = (1 << 2),
        VOLUME_CHECK_ITEM_FILESYSTEM_IS_VALID = (1 << 3),
        VOLUME_CHECK_ITEM_SNAPSHOT_SPACE = (1 << 4),
        VOLUME_CHECK_ITEM_HAS_CDP_PROTECTION = (1 << 5),
        VOLUME_CHECK_ITEM_IS_SYSTEM = (1 << 6),
        VOLUME_CHECK_ITEM_IS_DATA = (1 << 7),
        VOLUME_CHECK_ITEM_IS_RAC = (1 << 8),
        VOLUME_CHECK_ITEM_IS_DB = (1 << 9),
    };

    // 快照检查项目

    enum SnapshotCheckItem
    {
        SNAPSHOT_CHECK_ITEM_EXIST = (1 << 0),
        SNAPSHOT_CHECK_ITEM_HAS_COPY = (1 << 1),
        SNAPSHOT_CHECK_ITEM_NO_COPY = (1 << 2),
        SNAPSHOT_CHECK_ITEM_NOT_FIRST = (1 << 3),
        SNAPSHOT_CHECK_ITEM_NOT_FINAL = (1 << 4),
        SNAPSHOT_CHECK_ITEM_NOT_TRUSTED = (1 << 5),
    };

    // 网卡检查项目

    enum NicCheckItem
    {
        NIC_CHECK_ITEM_EXIST = (1 << 0)
    };

    // 启动协议检查项目

    enum BootProtocolCheckItem
    {
        BOOT_PROTOCOL_CHECK_ITEM_VALID = (1 << 0),
        BOOT_PROTOCOL_CHECK_ITEM_IS_NETBOOT = (1 << 1),
        BOOT_PROTOCOL_CHECK_ITEM_IS_VMBOOT = (1 << 2),
        BOOT_PROTOCOL_CHECK_ITEM_IS_FCBOOT = (1 << 3),
    };

    // 操作路径模式

    enum OpPathMode
    {
        OP_PATH_MODE_COPY,
        OP_PATH_MODE_SNAPSHOT,
    };

    // 操作路径类型

    enum OpPathType
    {
        OP_PATH_TYPE_ROOT,
        OP_PATH_TYPE_HCODE,
        OP_PATH_TYPE_STRATEGY,
        OP_PATH_TYPE_SUB_DIR,
    };

    // 虚拟机动作

    enum VmAction
    {
        VM_ACTION_START,
        VM_ACTION_STOP,
        VM_ACTION_START_ONE_KEY,
        VM_ACTION_STOP_ONE_KEY,
    };

    // 挂载模式

    enum MountMode
    {
        MOUNT_MODE_BY_ISCSI,
        MOUNT_MODE_BY_LOOP_DEVICE,
    };

    // 虚拟机启动模式

    enum VmStartMode
    {
        VM_START_MODE_NORMAL,
        VM_START_MODE_STANDBY,
        VM_START_MODE_ONE_KEY,
    };
private:

    virtual void FreeMem();

    /*
     *  功能：
     *      检查任务有效性
     *  参数
     *      无
     *  返回：
     *      任务有效返回true，否则返回false
     */
    virtual bool CheckTaskValidation();

    /*
     *  功能：
     *      任务预处理
     *  参数：
     *      无
     *  返回：
     *      处理成功返回true，否则返回false
     */
    virtual bool PreProcessing();

    /*
     *  功能：
     *      等待准备结果
     *  参数：
     *      无
     *  返回：
     *      等待成功返回true，否则返回false
     */
    virtual bool WaitPrepareResult();

    /*
     *  功能：
     *      任务执行
     *  参数：
     *      无
     *  返回：
     *      执行成功返回true，否则返回false
     */
    virtual bool Execute();

    /*
     *  功能：
     *      等待执行结果
     *  参数：
     *      无
     *  返回：
     *      处理成功返回true，否则返回false
     */
    virtual ActionProcResult WaitExecutingResult();

    /*
     *  功能：
     *      任务后处理
     *  参数：
     *      无
     *  返回：
     *      处理成功返回true，否则返回false
     */
    virtual bool PostProcessing();

    /*
     *  功能：
     *      错误处理
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void ErrorProcessing();

    /*
     *  功能：
     *      转入结束
     *  参数：
     *      无
     *  返回：
     *      转入结束成功返回true，否则返回false
     */
    virtual bool GotoFinish();

    /*
     *  功能：
     *      更新任务状态描述
     *  参数：
     *      type            :   状态描述类型
     *  返回：
     *      无
     */
    virtual void UpdateTaskStateDesc(TaskProcDesc::TaskProcEnum type, string msg = "");

    /*
     *  功能：
     *      将命令发送到Agent
     *  参数
     *      hostip          :   主机IP
     *      command         :   命令字符串
     *      recvstring      :   接收的字符串
     *  返回：
     *      操作结果
     */
    virtual bool PushToAgent(string hostip, string command, string &recvstring);

    /*
     *  功能：
     *      处理回应信息
     *  参数：
     *      rpstr       :   回应信息串
     *  返回：
     *      返回处理Agent响应结果
     */
    virtual BeCommand::CommandResult ProcResponse(string rpstr);

    /*
     * 功能：
     *      发送命令并解析回应
     * 参数
     *      recvstring      : 接收的字符串
     * 返回：
     *      操作结果
     */
    virtual bool PushAndProcResponse();

    /*
     *  功能：
     *      发送命令并解析回应
     *  参数
     *      command         :   命令字符串
     *      recvstring      :   接收的字符串
     *  返回：
     *      操作结果
     */
    virtual bool PushAndProcResponse(string command);

    /*
     *  功能：
     *      发送命令并解析回应
     *  参数
     *      command         :   命令字符串
     *      recvstring      :   接收的字符串
     *  返回：
     *      操作结果
     */
    virtual bool PushAndProcResponse(string hostip, string command);

    /*
     *  功能：
     *      打印任务执行时间
     *  参数：
     *      start           :   开始时间
     *      end             :   结束时间
     *  返回：
     *      无
     */
    virtual void PrintTaskTime(time_t start, time_t end);

    /*
     *  功能：
     *      执行子任务
     *  参数：
     *      cmdcode         :   命令字
     *      parameter       :   命令参数
     *      hcode           :   主机编码
     *      hostip          :   主机ip
     *      hostport        :   主机端口
     *  返回：
     *      子任务执行结果
     */
    virtual Task::TaskExecResult StartSubTask(string cmdcode, string parameter, string hcode, string hostip, string hostport);

    /*
     *  功能：
     *      执行子任务
     *  参数：
     *      cmdheader       :   命令头
     *      cmdcode         :   命令字
     *      parameter       :   命令参数
     *      hcode           :   主机编码
     *      hostip          :   主机ip
     *      hostport        :   主机端口
     *  返回：
     *      子任务执行结果
     */
    virtual Task::TaskExecResult StartSubTask(string cmdheader, string cmdcode, string parameter, string hcode, string hostip, string hostport);

    /*
     *  功能：
     *      通知agent中断任务
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void NotifyAgentToBreakTask();

     /*
     *  功能：
     *      获取系统状态
     *  参数：
     *      无
     *  返回：
     *      状态信息
     */
    virtual string GetSystemStatus();

    /*
     *  功能：
     *      验证监控用户名和密码
     *  参数：
     *      user            :   用户名
     *      pwd             :   密码
     *  返回：
     *      验证成功返回true，否则返回false
     */
    virtual bool ValidateMonitorUserPassword(string user, string pwd);

    /*
     *  功能：
     *      检查存储空间
     *  参数：
     *      hcode           :   主机编码
     *      strategyid      :   盘符或策略编号
     *      oldvolumesize   :   原有卷空间大小
     *      newvolumesize   :   新的卷空间大小
     *  返回：
     *      检查成功返回true，否则返回false
     */
    virtual bool CheckStorageSpace(string hcode, string volumesymbol, float oldvolumesize, float newvolumesize);

    /*
     *  功能：
     *      检查主机协议是否相等
     *  参数：
     *      oldprotocol     :   旧的协议
     *      newprotocol     :   新的协议
     *  返回：
     *      如果相等则返回true，否则返回false
     */
    virtual bool CheckHostProtocolEqual(string oldprotocol, string newprotocol);

    /*
     *  功能：
     *      检查主机的有效性
     *  参数
     *      hcode           :   主机编码
     *      items           :   检查项
     *      isseterror      :   是否设置错误
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckHostValidation(string hcode, Task::HostCheckItem items, bool isseterror = true);

    /*
     *  功能：
     *      检查主机的有效性
     *  参数
     *      hcode           :   主机编码
     *      items           :   检查项
     *      isseterror      :   是否设置错误
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckHostStateValidation(string hcode, Task::HostStateCheckItem items, bool isseterror = true);

    /*
     *  功能：
     *      检查副本的有效性
     *  参数
     *      hcode           :   主机编码
     *      copyid          :   副本id
     *      items           :   检查项
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckCopyValidation(string hcode, string copyid, Task::CopyCheckItem items);

    /*
     *  功能：
     *      检查卷的有效性
     *  参数
     *      hcode           :   主机编码
     *      volumesymbol    :   卷名或策略id
     *      items           :   检查项
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckVolumeValidation(string hcode, string volumesymbol, Task::VolumeCheckItem items);

    /*
     *  功能：
     *      检查参数有效性
     *  参数
     *      validparanum    :   有效的参数个数
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckParameterValidation(int validparanum);

    /*
     *  功能：
     *      检查快照有效性
     *  参数
     *      hcode           :   主机编码
     *      snapshotid      :   快照id
     *      items           :   检查项
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckSnapshotValidation(string hcode, string snapshotid, Task::SnapshotCheckItem items);

    /*
     *  功能：
     *      检查网卡有效性
     *  参数
     *      nicname         :   网卡名称
     *      items           :   检查项
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckNicValidation(string nicname, Task::NicCheckItem items);

    /*
     *  功能：
     *      检查启动协议有效性
     *  参数
     *      hcode           :   主机编码
     *      protocol        :   启动协议
     *      items           :   检查项
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckBootProtocolValidation(string hcode, string protocol, Task::BootProtocolCheckItem items, bool isseterror = true);

    /*
     *  功能：
     *      检查主机授权的有效性
     *  参数
     *      hcode           :   主机编码
     *      items           :   检查项
     *      isseterror      :   是否设置错误
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckHostLicenseValidation(string hcode, Task::LicenseCheckItem items, bool isseterror = true);

    /*
     *  功能：
     *      检查主机授权的有效性
     *  参数
     *      hcode           :   主机编码
     *      volumesymbol    :   盘符或策略编号
     *      items           :   检查项
     *      isseterror      :   是否设置错误
     *  返回：
     *      如果有效返回true,否则返回false
     */
    virtual bool CheckHostLicenseValidation(string hcode, string volumesymbol, Task::LicenseCheckItem items, bool isseterror = true);

    /*
     *  功能：
     *      删除主机策略
     *  参数
     *      hcode           :   主机编码
     *      strategyid      :   盘符或策略编号
     *      isforce         :   是否强制删除
     *  返回：
     *      如果成功返回true,否则返回false
     */
    virtual bool DeleteStrategyVolume(string hcode, string strategyid, bool isforce = false);

    /*
     *  功能：
     *      查询主机信息
     *  参数
     *      hcode           :   主机编码
     *  返回：
     *      无
     */
    virtual void QueryHostInformation(string hcode);

    virtual void QueryHostStatus(string hcode);

    /*
     *  功能：
     *      查询license信息
     *  参数
     *      无
     *  返回：
     *      无
     */
    virtual void QueryLicense();

    virtual bool QueryHostVolumeType(string hcode, string strategyid, int retry = 1);

	//删除主机策略集
    virtual bool DeleteHostStrategySet(string hcode);

    virtual BaseFileSystem GetHostDefaultFileSystem(string hcode);

    /*
     *  功能：
     *      设置网卡为桥接状态
     *  参数
     *      nicname          :   网卡名
     *  返回：
     *      设置成功返回true，否则返回false
     */
    virtual bool SetNicBridged(string nicname);

    /*
     *  功能：
     *      将CDP时间转换为分钟表示
     *  参数
     *      keeptime            :   连续快照保留时间
     *      keepunit            :   时间单位
     *  返回：
     *      转换后的分钟数
     */
    virtual int ConvertCDPTime(int keeptime, int keepunit);

    /*
     *  功能：
     *      自检
     *  参数：
     *      无
     *  返回：
     *      成功返回true，否则返回false
     */
    virtual bool SelfCheck();

    /*
     *  功能：
     *      检查授权
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void CheckLicense();

    /*
     *  功能：
     *      检查端口
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void CheckPort();

    /*
     *  功能：
     *      检查单一端口
     *  参数：
     *      protocol        :   协议
     *      port            :   端口
     *  返回：
     *      无
     */
    virtual void CheckSinglePort(BeGeneralLib::SocketProtocol protocol, int port);

    /*
     *  功能：
     *      检查服务
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void CheckService();

    /*
     *  功能：
     *      检查单一服务
     *  参数：
     *      name            :   服务名
     *  返回：
     *      无
     */
    virtual void CheckSingleService(string name);

    /*
     *  功能：
     *      检查单一服务
     *  参数：
     *      name            :   服务名
     *      alias           :   服务别名
     *  返回：
     *      无
     */
    virtual void CheckSingleService(string name, string alias);

    /*
     *  功能：
     *      检查程序包
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void CheckPackage();

    /*
     *  功能：
     *      检查单一程序包
     *  参数：
     *      package         :   程序包
     *      alias           :   别名
     *  返回：
     *      无
     */
    virtual void CheckSinglePackage(string package, string alias);

    /*
     *  功能：
     *      检查ping
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void CheckPing();

    /*
     *  功能：
     *      检查数据库
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void CheckDatabase();

    /*
     *  功能：
     *      检查目录权限
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void CheckPermission();

    /*
     *  功能：
     *      检查进程
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void CheckProcess();

    /*
     *  功能：
     *      检查单一进程
     *  参数：
     *      name            ：  进程名
     *  返回：
     *      无
     */
    virtual void CheckSingleProcess(string name);

    /*
     *  功能：
     *      检查进程
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual void WriteSelfCheckLog(BesError::ErrorType type, string parameter, BesError::SelfCheckResult result);

    /*
     *  功能：
     *      发送内部刷新主机状态命令
     *  参数：
     *      hcode           :   主机编码
     *  返回：
     *      无
     */
    virtual void SendInternalRefreshHostStateCommand(string hcode);

    /*
     *  功能：
     *      当前命令字是否为调试打印的命令字
     *  参数：
     *      无
     *  返回：
     *      如果时返回ture，否则返回false
     */
    virtual bool IsDebugCommand();

    /*
     *  功能：
     *      写入卷的本地挂载点（盘符）
     *  参数
     *      device          :   设备
     *      hcode           :   主机编码
     *      copyid          :   副本编号
     *  返回：
     *      如果成功返回true，否则返回false
     */
    virtual bool WriteVolumeLMountPoint(string device, string hcode, string copyid);

    /*
     *  功能：
     *      显示任务阶段信息
     *  参数
     *      无
     *  返回：
     *      无
     */
    virtual void ShowTaskStageInformation();

    /*
     *  功能：
     *      获取任务阶段字符串
     *  参数
     *      无
     *  返回：
     *      无
     */
    virtual string GetTaskStageString();

    /*
     *  功能：
     *      获取可用的网卡名
     *  参数
     *      无
     *  返回：
     *      网卡名
     */
    virtual string GetAvailableNicName();

    virtual void CreateNotCopyDir(string mountpoint);
    virtual bool CreateDiskPartion(string diskname, string size);
    virtual bool CreateFormatDiskScript(string size);
    virtual string SetHostBootProtocol(string bootprotocol, string mountpoint);
};

#endif	/* _TASK_H */

