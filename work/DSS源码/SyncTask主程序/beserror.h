/*******************************************************************************
 *
 ********************* 程序文件:  BESError                ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-05-18              ***********************
 ********************* 完成时间:  2010-05-28              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *20100613  LATIS   1)修改了原来错误描述信息中doesn't和isn't描述，引起的SQL执行错误
 *20100623  LATIS   1)增加了副本空间不足的错误
 *20100623  LATIS   2)增加了Clear()和HasError()
 *20100630  LATIS   1)增加了副本不存在错误
 *20100712  LATIS   1)增加了策略不存在错误
 *20100724  LATIS   1)增加了主机无效操作系统错误
 *20100724  LATIS   2)增加了主机无效网启协议错误
 *20100726  LATIS   1)增加了调度计划不存在错误
 *20100726  LATIS   2)增加了回应失败错误
 *20100727  LATIS   1)增加了操作系统和命令字不一致错误
 *20100728  LATIS   1)增加了初始复制失败错误
 *20100728  LATIS   2)增加了服务不存在错误
 *20100728  LATIS   3)增加了服务启动失败错误
 *20100728  LATIS   4)增加了服务停止失败错误
 *20100728  LATIS   5)增加了扫描target失败错误
 *20100820  LATIS   1)增加了无效的主机挂载点
 *20100906  LATIS   1)增加了主机为网启状态错误
 *20100913  LATIS   1)增加了删除主机卷空间错误
 *20101223  LATIS   1)创建FC分区失败
 *20101223  LATIS   2)创建FC快照失败
 *20101223  LATIS   3)创建FC卷对失败
 *20101223  LATIS   4)创建FC主机映射失败
 *20110112  LATIS   1)VM没有内存和VM没有CPU
 *20110325  LATIS   1)增加了副本已经存在的错误
 *20110401  LATIS   1)增加了最后一个标准副本不允许删除的错误
 *20110519  LATIS   1)增加了无效的主机动作错误
 *20110520  LATIS   1)增加了无效的主机状态错误
 *20110520  LATIS   2)增加了创建虚拟机失败错误
 *20110817  LATIS   1)修改启动协议失败
 *20111009  LATIS   1)增加了GetErrorDescription的重载方法
 *20111027  LATIS   1)增加了主机本地应急状态处于打开或关闭状态的错误
 *20111117  LATIS   1)增加了无效的主机复制方式错误
 *20111124  LATIS   1)增加了可信副本不存在和应急副本不存在的错误
 *20111125  LATIS   1)增加了可信副本不允许删除的错误
 *20111205  LATIS   1)增加了该副本不是系统盘的副本，无法指定为可信副本
 *20111206  LATIS   1)增加了主机处于应急状态和主机不是处于应急状态的错误
 *20111220  LATIS   1)增加了远端服务器连接失败、远端服务器无响应、远端服务器接收超时、远端服务器繁忙错误、远端服务器处理任务错误和远端服务器处理任务失败的错误
 *20111220  LATIS   2)增加了同步副本错误
 *20111220  LATIS   3)增加了源路径为空或不存在的错误
 *20111220  LATIS   4)增加了目标路径为空或不存在的错误
 *20111230  LATIS   1)增加了无效的ip地址的错误
 *20120220  LATIS   1)增加了光纤卡不存在的错误
 *20120312  LATIS   1)增加了复制方式不支持的错误
 *20120315  LATIS   1)增加了用于保护的网卡没有用于虚拟机启动的错误
 *20120330  LATIS   1)增加了该文件系统不支持此操作错误
 *20120330  LATIS   2)增加了无效的文件系统错误
 *20120420  LATIS   1)增加了设备和文件系统不匹配错误
 *20120502  LATIS   1)增加了设备不存在错误
 *20120503  LATIS   1)增加了checkpoint不存在错误
 *                  2)增加了checkpoint类型错误
 *20120514  LATIS   1)增加了无效的dhcp子网范围错误
 *20120522  LATIS   1)增加了设备增加失败错误
 *                  2)增加了设备删除失败错误
 *                  3)增加了组不存在错误
 *                  4)增加了组增加失败错误
 *                  5)增加了组删除失败错误
 *                  6)增加了lun不存在错误
 *                  7)增加了lun增加失败错误
 *                  8)增加了lun删除失败错误
 *                  9)增加了initiator不存在错误
 *                  10)增加了initiator增加失败错误
 *                  11)增加了initiator删除失败错误
 *20120702  LATIS   1)增加了网启应急打开和关闭的错误
 *                  2)增加了虚拟机应急功能打开和关闭的错误
 *                  3）增加了光纤卡应急功能打开和关闭的错误
 *20120703  LATIS   1）增加了网启应急主机数超过license许可错误
 *                  2）增加了虚拟机应急主机数超过license许可错误
 *                  3）增加了光纤卡应急主机数超过license许可错误
 *20120717  LATIS   1)增加了启动协议不是网启协议错误
 *                  2)增加了启动协议不是虚拟机协议错误
 *                  3)增加了启动协议不是光纤协议错误
 *                  4)增加了主机不是本地状态错误
 *                  5)增加了主机不是应急状态错误
 *                  6)增加了主机处于应急状态错误
 *                  7)增加了主机不是网启状态错误
 *                  8)增加了主机不是虚拟机状态错误
 *                  9)增加了主机不是光纤启动状态错误
 *                  10)增加了主机不是离线状态错误
 *                  11)增加了主机处于离线状态错误
 *20120725  LATIS   1）无效的wwn
 *20121010  LATIS   1)无效的策略快照数
 *20121021  latis   1）增加了无效的任务类型、无效的任务状态和执行任务超时的错误
 *20121022  latis   1)增加了CDP不支持指定应急副本、找不到CDP快照恢复点的错误
 *20121106  latis   1)增加了磁盘空间大小未改变的错误
 *                  2)增加了磁盘空间小于已用的磁盘空间的错误
 *20121204  latis   1)增加了主机自动应急已打开和主机自动应急未打开的错误
 *20121213  latis   1)增加了设备挂载失败和设备卸载失败错误
 * 
//================================== 修改结束 ==================================*/


#ifndef _BESERROR_H
#define	_BESERROR_H
#include "begenerallib.h"
#include "beslog.h"

using namespace std;

class BesError
{
public:

    // 错误类型

    enum ErrorType
    {
        BES_ERR_NONE = -1,

        BES_ERR_LICENSE_NOT_EXIST, // LICENSE不存在
        BES_ERR_LICENSE_INVALID, // 无效的License
        BES_ERR_LICENSE_EXPIRED, // License已经过期
        BES_ERR_LICENSE_EXPIRED_DATE_INVALID, // 无效的授权到期日
        BES_ERR_LICENSE_OVER_HOST_NUMBER, // license超过主机数
        BES_ERR_LICENSE_OVER_SNAPSHOT_NUMBER, // 超过license快照数
        BES_ERR_LICENSE_OVER_DATABASE_NUMBER, // 超过license数据库保护数
        BES_ERR_LICENSE_OVER_NET_EMERGENCY_NUMBER, // 超过license网启应急数
        BES_ERR_LICENSE_OVER_VM_EMERGENCY_NUMBER, // 超过license虚拟机应急数
        BES_ERR_LICENSE_OVER_FC_EMERGENCY_NUMBER, // 超过license光纤卡应急数
        BES_ERR_LICENSE_OVER_CDP_NUMBER, // 超过授权的CDP个数
        BES_ERR_LICENSE_OVER_CDP_KEEP_SNAPSHOT_TIME, // 超过cdp快照保留时间
        BES_ERR_LICENSE_OVER_EA_NUMBER, // 超过license自动应急数
        BES_ERR_LICENSE_SYSTEM_TIME_EARLIER_THAN_LICENSE_DATE, // 系统时间早于授权创建时间
        BES_ERR_LICENSE_CURRENT_EARLIER_THAN_LAST_LICENSE_DATE, // 当前授权文件早于上个授权文件创建时间

        BES_ERR_SYSTEM_TIME_CHANGED, // 系统时间已修改

        BES_ERR_DATABASE_OFFLINE, // 数据库不在线
        BES_ERR_DATABASE_CONNECTED_ERROR, // 数据库连接错误
        BES_ERR_DATABASE_INVALID_HOST, // 无效的数据库服务器
        BES_ERR_DATABASE_INVALID_USER, // 无效的数据库用户
        BES_ERR_DATABASE_INVALID_PASSWORD, // 无效的数据库密码
        BES_ERR_DATABASE_INVALID_DBNAME, // 无效的数据库名称
        BES_ERR_DATABASE_INVALID_TABLE, // 无效的表
        BES_ERR_DATABASE_INVALID_FILED, // 无效的字段
        BES_ERR_DATABASE_INVALID_FILED_VALUE, // 无效的字段值
        BES_ERR_DATABASE_EXEC_QUERY_FAILED, // 查询数据库失败
        BES_ERR_DATABASE_EXEC_INSERT_FAILED, // 插入记录失败
        BES_ERR_DATABASE_EXEC_UPDATE_FAILED, // 更新记录失败
        BES_ERR_DATABASE_EXEC_DELETE_FAILED, // 删除记录失败

        BES_ERR_AGENT_CONNECT_FAILED, // Agent连接失败
        BES_ERR_AGENT_NO_RESPONSE, // Agent无响应
        BES_ERR_AGENT_RECEIVE_TIMEOUT, // Agent接收超时
        BES_ERR_AGENT_IS_BUSY, // Agent繁忙
        BES_ERR_AGENT_TASK_ERROR, // Agent任务错误
        BES_ERR_AGENT_TASK_FAILED, // Agent任务失败
        BES_ERR_AGENT_INIT_COPY_FAILED, // Agent初始复制失败
        BES_ERR_AGENT_SNAPSHOT_PREPROCESSING_FAILED, // agent快照预处理失败
        BES_ERR_AGENT_SNAPSHOT_POSTPROCESSING_FAILED, // agent快照后处理失败
        BES_ERR_AGENT_QUERY_VOLUME_TYPE_FAILED, // agent查询卷类型失败

        BES_ERR_TASK_TIMEOUT, // 任务超时
        BES_ERR_TASK_OVER_COUNTER, // 任务超过最大重试次数
        BES_ERR_TASK_INVALID_TASK_TYPE, // 无效的任务类型
        BES_ERR_TASK_INVALID_TASK_STATE, // 无效的任务状态
        BES_ERR_TASK_EXECUTE_TIMEOUT, // 执行任务超时
        BES_ERR_TASK_TERMINATED, // 任务被终止
        BES_ERR_TASK_TERMINATING, // 任务终止中
        BES_ERR_TASK_TERMINATED_BY_EMERGENCY, // 任务被应急操作终止
        BES_ERR_TASK_FAILED, // 任务失败
        BES_ERR_TASK_SUCCESS, // 任务成功
        BES_ERR_TASK_ERROR, // 任务错误
        BES_ERR_TASK_BUSY, // 任务繁忙
        BES_ERR_TASK_IS_EXECUTING, // 任务正在执行
        BES_ERR_TASK_IS_WAITING, // 任务正在等待
        BES_ERR_TASK_FINISH, // 任务完成
        BES_ERR_TASK_EXIST, // 任务已存在

        BES_ERR_VOLUME_SYMBOL_NOT_EXIST, // 盘符不存在

        BES_ERR_SOCKET_OPEN_FAILED, // 打开套接字失败

        BES_ERR_INVALID_HOST_CODE, // 无效的主机编码
        BES_ERR_INVALID_HOST_NAME, // 无效的主机名
        BES_ERR_HOST_NOT_EXIST, // 主机不存在
        BES_ERR_HOST_CODE_EXIST, // 主机编码已存在
        BES_ERR_HOST_NAME_EXIST, // 主机名已存在
        BES_ERR_HOST_IP_EXIST, // 主机ip已存在
        BES_ERR_HOST_IP_NOT_EXIST, // 主机ip不存在
		BES_ERR_HOST_NOT_IN_DOG,	//加密狗中检索不到该主机


        BES_ERR_FILE_OPEN_FAILED, // 打开文件失败
        BES_ERR_FILE_NOT_EXIST, // 文件不存在
        BES_ERR_FILE_NO_PRIVILLAGE, // 文件权限不足
        BES_ERR_FILE_READ_FAILED, // 读取文件失败
        BES_ERR_FILE_WRITE_FAILED, // 写入文件失败

        BES_ERR_DISK_NO_SPACE, // 磁盘空间不足
        BES_ERR_DISK_NOT_EXIST, // 磁盘不存在

        BES_ERR_COMMAND_INVALID_SEGMENT_COUNT, // 无效的命令段数
        BES_ERR_COMMAND_INVALID_HEADER, // 无效的命令头
        BES_ERR_COMMAND_INVALID_TAILOR, // 无效的命令尾
        BES_ERR_COMMAND_INVALID_CODE, // 无效的命令字
        BES_ERR_COMMAND_INVALID_PARAMETER, // 无效的参数
        BES_ERR_COMMAND_INVALID_PARAMETER_COUNT, // 无效的参数个数
        BES_ERR_COMMAND_INVALID_RESPONSE, // 无效的响应

        BES_ERR_TARGET_CREATE_FAILED, // 创建target失败
        BES_ERR_TARGET_ADJUST_FAILED, // 调整target失败
        BES_ERR_TARGET_DELETE_FAILED, // 删除target失败
        BES_ERR_TARGET_MOUNT_FAILED, // 挂载target失败
        BES_ERR_TARGET_UNMOUNT_FAILED, // 卸载target失败
        BES_ERR_TARGET_LOGON_FAILED, // 登录target失败
        BES_ERR_TARGET_LOGOFF_FAILED, // 注销target失败
        BES_ERR_TARGET_DISCOVER_FAILED, // 扫描target失败
        BES_ERR_TARGET_ENABLE_FAILED, // 启用target失败
        BES_ERR_TARGET_BIND_FAILED, // target绑定initiator失败
        BES_ERR_TARGET_UNBIND_FAILED, // target解除initiator绑定失败
        BES_ERR_TARGET_SESSION_CHECK_FAILED, // target会话检查失败

        BES_ERR_SHARE_OPEN_FAILED, // 打开共享目录失败
        BES_ERR_SHARE_CLOSE_FAILED, // 关闭共享目录失败
        BES_ERR_SHARE_DELETE_FAILED, // 删除共享目录失败
        BES_ERR_SHARE_FOLDER_IS_EMPTY, // 共享目录为空

        BES_ERR_COPY_IS_USED, // 副本正在使用中
        BES_ERR_COPY_BOOT_NOT_ALLOWED_DELETE, // 网启副本不允许删除
        BES_ERR_COPY_IS_NOT_USED, // 副本未使用
        BES_ERR_COPY_NO_SPACE, // 副本空间不足
        BES_ERR_COPY_FOLDER_IS_EMPTY, // 副本文件目录为空
        BES_ERR_COPY_NOT_EXIST, // 副本不存在
        BES_ERR_COPY_NEW_NOT_ALLOWED_DELETE, // 最新副本不允许删除
        BES_ERR_COPY_STATE_ERROR, // 副本状态错误
        BES_ERR_COPY_NOT_BOOT_COPY, // 副本不是网启副本
        BES_ERR_COPY_TYPE_NOT_ALLOWED_SNAPSHOT, // 该副本类型不允许进行快照
        BES_ERR_COPY_TYPE_IS_SNAPSHOT_COPY, // 副本为快速型副本
        BES_ERR_COPY_EXIST, // 副本已存在
        BES_ERR_COPY_FINAL_NORMAL_NOT_ALLOWED_DELETE, // 最后一个标准副本不允许删除
        BES_ERR_COPY_TRUSTED_NOT_EXIST, // 可信副本不存在
        BES_ERR_COPY_EMERGENCY_NOT_EXIST, // 应急副本不存在
        BES_ERR_COPY_TRUSTED_NOT_ALLOWED_DELETE, // 可信副本不允许删除
        BES_ERR_COPY_VOLUME_NOT_SYSTEM_VOLUME, // 该副本不是系统盘的副本，无法指定为可信副本
        BES_ERR_COPY_SIZE_NOT_MATCH_ORIGINAL, // 副本大小和原始大小不一致
        BES_ERR_COPY_CREATE_FAILED, // 创建副本失败
        BES_ERR_COPY_DELETE_FAILED, // 删除副本失败

        BES_ERR_VOLUME_C_NOT_ALLOWED_SWITCH, // C盘不允许进行切换
        BES_ERR_VOLUME_C_NOT_ALLOWED_RESTORE, // C盘不允许进行还原

        BES_ERR_SNAPSHOT_ZERO_NOT_ALLOWED_DELETE, // 快照零不允许删除
        BES_ERR_SNAPSHOT_NEW_NOT_ALLOWED_DELETE, // 最新快照不允许删除
        BES_ERR_SNAPSHOT_IS_TRUSTED, // 该快照为可信快照
        BES_ERR_SNAPSHOT_CREATE_FAILED, // 生成快照失败
        BES_ERR_SNAPSHOT_NOT_EXIST, // 快照不存在

        BES_ERR_EHTO_NOT_ALLOWED_CONFIG, // eth0不允许进行设置

        BES_ERR_HOST_IS_NET_BOOT, // 主机处于网启中
        BES_ERR_HOST_IS_NET_BOOT_OR_COPY_USED, // 主机处于网启或副本使用中
        BES_ERR_HOST_NOT_PROTECT_DATABASE, // 该主机未保护数据库
        BES_ERR_HOST_IS_NOT_NET_BOOT, // 主机不是网启状态
        BES_ERR_HOST_IS_IN_EMERGENCY_MODE, // 主机处于应急状态
        BES_ERR_HOST_IS_NOT_IN_EMERGENCY_MODE, // 主机不是应急状态
        BES_ERR_NIC_NOT_EXIST, // 网卡不存在
        BES_ERR_SERVER_IP_NOT_EXIST, // 服务器ip不存在
        BES_ERR_STRATEGY_NOT_EXIST, // 策略不存在
        BES_ERR_PATH_NOT_EXIST, // 路径不存在
        BES_ERR_HOST_INVALID_OSTYPE, // 无效的操作系统类型
        BES_ERR_HOST_INVALID_NETBOOT_PROTOCOL, // 无效的网启协议类型
        BES_ERR_BOOT_PROTOCOL_IS_NOT_NETBOOT, // 启动协议不是网启协议
        BES_ERR_BOOT_PROTOCOL_IS_NOT_VMBOOT, // 启动协议不是虚拟机协议
        BES_ERR_BOOT_PROTOCOL_IS_NOT_FCBOOT, // 启动协议不是光纤协议
        BES_ERR_SCHEDULE_NOT_EXIST, // 调度计划不存在
        BES_ERR_HOST_OSTYPE_NOT_MATCH_COMMAND_CODE, // 主机操作系统类型和命令字不相符

        BES_ERR_INVALID_MOUNT_POINT, // 无效的主机挂载点
        BES_ERR_SYNC_SNAPSHOT_FAILED, // 同步快照失败
        BES_ERR_DELETE_HOST_SPACE_FAILED, // 删除主机卷空间失败
        BES_ERR_FC_CREATE_PARTITION_FAILED, // 创建FC分区失败
        BES_ERR_FC_CREATE_SNAPSHOT_IMAGE_FAILED, // 创建FC快照失败
        BES_ERR_FC_CREATE_VOLUME_PAIR_FAILED, // 创建FC卷对失败
        BES_ERR_FC_CREATE_LUN_MAP_FAILED, // 创建FC主机映射失败
        BES_ERR_FC_ADD_TARGET_FAILED, // 增加FCtarget失败
        BES_ERR_FC_NAME_NOT_EXIST, // 光纤卡不存在
        BES_ERR_FC_WWN_IS_INVALID, // 无效wwn

        BES_ERR_VM_NO_MEMORY, // VM没有内存空间
        BES_ERR_VM_NO_CPU, // VM没有CPU
        BES_ERR_VM_CREATE_FAILED, //   创建虚拟机失败
        BES_ERR_VM_START_FAILED, //   启动虚拟机失败
        BES_ERR_VM_SHUTDOWN_FAILED, // 关闭虚拟机失败
        BES_ERR_VM_DELETE_FAILED, // 删除虚拟机失败
        BES_ERR_VM_STAGE_CHECK_COPY, // 检查副本
        BES_ERR_VM_STAGE_CHANGE_EMERGENCY_PROTOCOL, // 切换启动协议
        BES_ERR_VM_STAGE_APPLY_VM_CONFIG, // 应用虚拟机配置
        BES_ERR_VM_STAGE_CREATEVM, // 创建虚拟机
        BES_ERR_VM_STAGE_STARTVM, // 启动虚拟机
        BES_ERR_VM_STAGE_STARTVM_FINISH, // 启动虚拟机完成

        BES_ERR_INVALID_HOST_ACTION, // 无效的主机动作
        BES_ERR_INVALID_HOST_STATUS, // 无效的主机状态
        BES_ERR_HOST_STATUS_IS_NOT_LOCAL, // 主机不是本地状态
        BES_ERR_HOST_STATUS_IS_NOT_EMERGENCY, // 主机不是应急状态
        BES_ERR_HOST_STATUS_IS_EMERGENCY, // 主机处于应急状态
        BES_ERR_HOST_STATUS_IS_NOT_NET, // 主机不是网启应急状态
        BES_ERR_HOST_STATUS_IS_NOT_VM, // 主机不是虚拟应急状态
        BES_ERR_HOST_STATUS_IS_NOT_FC, // 主机不是光纤应急状态
        BES_ERR_HOST_STATUS_IS_OFFLINE, // 主机处于离线状态
        BES_ERR_HOST_STATUS_IS_NOT_OFFLINE, // 主机处于离线状态

        BES_ERR_INVALID_USER_AND_PASSWORD, // 无效的用户名和密码

        BES_ERR_CHANGE_BOOT_PROTOCOL_FAILED, // 修改启动协议失败
        BES_ERR_NIC_IS_NOT_BRIDGED, // 网卡没有桥接

        BES_ERR_SERVER_ID_NOT_MATCH_LOCAL, // 服务器id和本地id不一致
        BES_ERR_SERVER_ID_EXIST, // 服务器编号已经存在
        BES_ERR_SERVER_ID_IS_INVALID, // 服务器编号是无效的
        BES_ERR_REMOTE_SERVER_NOT_EXIST, // 远程服务器不存在
        BES_ERR_REMOTE_SERVER_TYPE_IS_INVALID, // 远程服务器类型无效
        BES_ERR_HOST_LOCAL_EMERGENCY_FUNCTION_IS_OPENED, // 主机本地应急功能是打开的
        BES_ERR_HOST_LOCAL_EMERGENCY_FUNCTION_IS_CLOSED, // 主机本地应急功能是关闭的
        BES_ERR_HOST_NET_EMERGENCY_FUNCTION_IS_OPENED, // 主机网启应急功能是打开的
        BES_ERR_HOST_NET_EMERGENCY_FUNCTION_IS_CLOSED, // 主机网启应急功能是关闭的
        BES_ERR_HOST_VM_EMERGENCY_FUNCTION_IS_OPENED, // 主机虚拟机应急功能是打开的
        BES_ERR_HOST_VM_EMERGENCY_FUNCTION_IS_CLOSED, // 主机虚拟机应急功能是关闭的
        BES_ERR_HOST_FC_EMERGENCY_FUNCTION_IS_OPENED, // 主机光纤卡应急功能是打开的
        BES_ERR_HOST_FC_EMERGENCY_FUNCTION_IS_CLOSED, // 主机光纤卡应急功能是关闭的
        BES_ERR_HOST_REMOTE_DR_OPENED, // 主机远程容灾是打开的
        BES_ERR_HOST_REMOTE_DR_CLOSED, // 主机远程容灾是关闭的

        BES_ERR_HOST_NO_NET_LICENSE_WITH_ISCSI_COPY, // 主机无网启应急授权,无法通过iscsi复制
        BES_ERR_HOST_NO_VM_LICENSE_WITH_ISCSI_COPY, // 主机无虚拟机应急授权，无法通过iscsi复制
        BES_ERR_HOST_NO_FC_LICENSE_WITH_FC_COPY, // 主机无光纤应急授权，无法通过光纤复制

        BES_ERR_HOST_INVALID_COPY_MODE, // 无效的主机复制方式
        BES_ERR_HOST_INVALID_STORAGE_SPACE, // 无效的存储空间
        BES_ERR_HOST_HAS_INITIALIZE_COPYED, // 主机已经做过初始复制
        BES_ERR_HOST_HAS_CDP_PROTECTION, // 主机有CDP保护策略
        BES_ERR_HOST_NO_CDP_PROTECTION, // 主机无CDP保护策略
        BES_ERR_HOST_IP_IS_SERVER_IP, // 主机ip为服务器地址

        BES_ERR_STORAGE_NOT_EXIST, // 存储空间不存在
        BES_ERR_STORAGE_NO_SPACE, // 存储空间不足

        BES_ERR_REMOTE_CONNECT_FAILED, // 远端服务器连接失败
        BES_ERR_REMOTE_NO_RESPONSE, // 远端服务器无响应
        BES_ERR_REMOTE_RECEIVE_TIMEOUT, // 远端服务器接收超时
        BES_ERR_REMOTE_IS_BUSY, // 远端服务器繁忙
        BES_ERR_REMOTE_TASK_ERROR, // 远端服务器任务错误
        BES_ERR_REMOTE_TASK_FAILED, // 远端服务器任务失败
        BES_ERR_SYNC_COPY_FAILED, // 同步副本失败
        BES_ERR_SOURCE_PATH_IS_EMPTY_OR_NOT_EXIST, // 源路径为空或不存在
        BES_ERR_DEST_PATH_IS_EMPTY_OR_NOT_EXIST, // 目标路径为空或不存在

        BES_ERR_INVALID_IP_ADDRESS, // 无效的ip地址
        BES_ERR_COPY_MODE_NOT_SUPPORTED, // 不支持该复制方式

        BES_ERR_NIC_FOR_PROTECTED_NOT_SET_FOR_VM, // 用于保护的网卡没有用于虚拟机启动

        BES_ERR_FILESYSTEM_NOT_SUPPORTED_OPERATION, // 该文件系统不支持此操作
        BES_ERR_FILESYSTEM_IS_INVALID, // 无效的文件系统

        BES_ERR_FILESYSTEM_NOT_MATCH_DEVICE, // 文件系统和设备不匹配

        BES_ERR_DEVICE_NOT_EXIST, // 设备不存在
        BES_ERR_DEVICE_NOT_MOUNTED, // 设备未挂载
        BES_ERR_DEVICE_HAS_MOUNTED, // 设备已经挂载
        BES_ERR_DEVICE_ADD_FAILED, // 设备增加失败
        BES_ERR_DEVICE_DELETE_FAILED, // 设备删除失败
        BES_ERR_DEVICE_MOUNT_FAILED, // 设备挂载失败
        BES_ERR_DEVICE_UNMOUNT_FAILED, // 设备卸载失败

        BES_ERR_GROUP_NOT_EXIST, // 组不存在
        BES_ERR_GROUP_ADD_FAILED, // 添加组失败
        BES_ERR_GROUP_DELETE_FAILED, // 删除组失败

        BES_ERR_LUN_NOT_EXIST, // lun不存在
        BES_ERR_LUN_ADD_FAILED, // 增加lun失败
        BES_ERR_LUN_DELETE_FAILED, // 删除lun失败

        BES_ERR_INITIATOR_NOT_EXIST, // initiator不存在
        BES_ERR_INITIATOR_ADD_FAILED, // 增加initiator失败
        BES_ERR_INITIATOR_DELETE_FAILED, // 删除initiator失败

        BES_ERR_CHECKPOINT_NOT_EXIST, // checkpoint不存在
        BES_ERR_CHECKPOINT_TYPE_IS_INVALID, // checkpoint类型错误
        BES_ERR_DHCP_INVALID_SUBNET_RANGE, // 无效的dhcp子网范围

        BES_ERR_STRATEGY_NO_CDP_PROTECTION, // 策略没有进行CDP保护
        BES_ERR_STRATEGY_MAX_SNAPSHOT_IS_INVALID, // 无效的策略快照数
        BES_ERR_STRATEGY_NOT_SYSTEM_TYPE, // 该策略不是系统复制类型
        BES_ERR_STRATEGY_NOT_DATA_TYPE, // 该策略不是数据复制类型
        BES_ERR_STRATEGY_NOT_RAC_TYPE, // 该策略不是rac类型
        BES_ERR_STRATEGY_NOT_DB_TYPE, // 该策略不是数据库类型

        BES_ERR_NOTIFY_HOST_INITIALIZE_COPY_PREPARE_FAILED, // 通知主机初始复制预处理失败
        BES_ERR_NOTIFY_HOST_REMOVE_CDP_FAILED, // 通知主机删除CDP失败
        BES_ERR_NOTIFY_HOST_RAC_INITIALIZE_COPY_FAILED, // 通知主机rac初始复制失败
        BES_ERR_NOTIFY_HOST_RAC_RECOVER_FAILED, // 通知主机rac恢复失败

        BES_ERR_RAC_GROUP_NOT_EXIST, //  RAC组不存在
        BES_ERR_RAC_DISK_NOT_EXIST, // RAC磁盘不存在
        BES_ERR_RAC_GROUP_NOT_MATCH_HOST, // RAC组编号和主机的rac组编号不一致

        BES_ERR_CDP_NUMBER_IS_INVALID, // 无效的CDP个数
        BES_ERR_CDP_OVER_CONFIG_NUMBER, // 超出CDP配置个数
        BES_ERR_CDP_OVER_CONFIG_KEEPTIME, // 超出CDP配置的连续快照时间
        BES_ERR_CDP_NOT_FOUND_RECOVERY_POINT, // 找不到CDP快照恢复点
        BES_ERR_CDP_NOT_SUPPORT_SET_EMERGENCY_COPY, // CDP不支持指定应急副本
        BES_ERR_CDP_PORT_NOT_FOUND, // 未找到可用的CDP端口

        BES_ERR_HOST_VOLUME_SPACE_NO_CHANGE, // 磁盘空间大小未改变
        BES_ERR_HOST_VOLUME_SPACE_LESS_THAN_USED_SPACE, // 磁盘空间小于已用的磁盘空间

        BES_ERR_HOST_EA_OPENED, // 主机自动应急已打开
        BES_ERR_HOST_EA_NOT_OPENED, // 主机自动应急未打开

        BES_ERR_HEARTBEAT_NOTIFY_FAILED, // 心跳通知失败

        BES_ERR_LOCAL_BACKUP_NOT_EXIST, // 本机备份记录不存在
        BES_ERR_LOCAL_BACKUP_STATE_IS_INVALID, // 无效的备份状态

        BES_ERR_MAC_ADDRESS_IS_INVALID, // 无效的mac地址

        BES_ERR_BEDEAMON_NOT_RUNNING, // 守护进程没有运行
        BES_ERR_DRHDEAMON_NOT_RUNNING, // 心跳守护进程没有运行
        BES_ERR_RDRTASKEXEC_NOT_RUNNING, // 异地容灾任务执行器没有运行

        BES_ERR_SELF_CHECK_BEGIN, // 开始自检
        BES_ERR_SELF_CHECK_END, // 结束自检
        BES_ERR_SELF_CHECK_ITEM_LICENSE, // 授权自检
        BES_ERR_SELF_CHECK_ITEM_LOG, // 日志自检
        BES_ERR_SELF_CHECK_ITEM_STORAGE, // 存储自检
        BES_ERR_SELF_CHECK_ITEM_PING, // ping自检
        BES_ERR_SELF_CHECK_ITEM_DATABASE, // 数据库自检
        BES_ERR_SELF_CHECK_ITEM_PERMISSION, // 权限自检
        BES_ERR_SELF_CHECK_ITEM_TARGET, // target自检
        BES_ERR_SELF_CHECK_ITEM_PACKAGE, // 程序包自检
        BES_ERR_SELF_CHECK_ITEM_PORT, // 端口自检
        BES_ERR_SELF_CHECK_ITEM_SERVICE, // 服务自检
        BES_ERR_SELF_CHECK_ITEM_PROCESS, // 进程自检

        BES_ERR_PROCESS_NOT_EXIST, // 进程不存在
        BES_ERR_PROCESS_NOT_RUNNING, // 进程没有运行
        BES_ERR_PROCESS_IS_RUNNING, // 进程正在运行
        BES_ERR_PROCESS_START_FAILED, // 启动进程失败
        BES_ERR_PROCESS_STOP_FAILED, // 停止进程失败

        BES_ERR_SERVICE_NOT_EXIST, // 服务不存在
        BES_ERR_SERVICE_ALREADY_EXIST, // 服务已存在
        BES_ERR_SERVICE_NOT_RUNNING, // 服务没有运行
        BES_ERR_SERVICE_IS_RUNNING, // 服务正在运行
        BES_ERR_SERVICE_START_FAILED, // 启动服务失败
        BES_ERR_SERVICE_STOP_FAILED, // 停止服务失败

        BES_ERR_PACKAGE_IS_INSTALLED, // 程序包已经安装
        BES_ERR_PACKAGE_NOT_INSTALLED, // 程序包没有安装
        BES_ERR_PACKAGE_NOT_FOUND, // 程序包没有找到
        BES_ERR_PACKAGE_INSTALL_FAILED, // 安装程序包失败
        BES_ERR_PACKAGE_UNINSTALL_FAILED, // 卸载程序包失败

        BES_ERR_PORT_NOT_OPENED, // 端口未打开
        BES_ERR_PORT_IS_OPENED, // 端口已打开

        BES_ERR_SUBVOLUME_CREATE_FAILED, // 创建子卷失败
        BES_ERR_SUBVOLUME_DELETE_FAILED, // 删除子卷失败

        BES_ERR_LOOP_DEVICE_BIND_FAILED, // 绑定loop设备失败
        BES_ERR_LOOP_DEVICE_UNBIND_FAILED, // 解除loop设备绑定失败

        BES_ERR_DRBD_CREATE_RESOURCE_FAILED, // 创建资源失败
        BES_ERR_DRBD_CREATE_METADATA_FAILED, // 创建源数据失败
        BES_ERR_DRBD_ENABLE_RESOURCE_FAILED, // 启用资源失败
        BES_ERR_DRBD_DISABEL_RESOURCE_FAILED, // 停止资源失败

        BES_ERR_PLAN_EMERGENCY_FAILED, // 预案应急失败
        BES_ERR_PLAN_EMERGENCY_STOP_FAILED, // 关闭预案应急失败

        BES_ERR_DOCKER_CREATE_IMAGE_FAILED,	//创建docker镜像失败
        BES_ERR_DOCKER_EMERGENCY_FAILED,	//DOCKER应急失败
        BES_ERR_DOCKER_IMAGE_NOT_EXIST,		//DOCKER镜像不存在
        BES_ERR_DOCKER_HAS_CONTAINER,		//DOCKER镜像存在正在运行的容器
        BES_ERR_DOCKER_CONTAINER_NOT_EXIST,	//DOCKER容器不存在

		BES_ERR_DOG_CHECK_FAILED,			//加密狗检测失败
		BES_ERR_CHANGE_HOST_IP_ADDRESS_FAILED,	//改主机IP失败
    };

    // 授权项

    enum LicenseItem
    {
        LICENSE_ITEM_HOST, // 主机
        LICENSE_ITEM_SNAPSHOT, // 快照数
        LICENSE_ITEM_DATABASE, // 数据库
        LICENSE_ITEM_NET_EMERGENCY, // 网启应急
        LICENSE_ITEM_VM_EMERGENCY, // 虚拟机应急
        LICENSE_ITEM_FC_EMERGENCY, // 光纤应急
        LICENSE_ITEM_CDP, // CDP
        LICENSE_ITEM_CDP_SNAPSHOT_KEEPTIME, // CDP快照保留时间
        LICENSE_ITEM_EA, // 自动应急
    };

    // 自检结果

    enum SelfCheckResult
    {
        SELF_CHECK_RESULT_NONE,
        SELF_CHECK_RESULT_SUCCESS,
        SELF_CHECK_RESULT_FAILED,
        SELF_CHECK_RESULT_NOT_RUNNING,
        SELF_CHECK_RESULT_FIXED,
        SELF_CHECK_RESULT_NOT_EXIST,
    };

public:
    /*
     *  功能：
     *      构造函数
     *  参数
     *      无
     *  返回：
     *      无
     */
    BesError();

    /*
     *  功能：
     *      构造函数
     *  参数
     *      无
     *  返回：
     *      无
     */
    virtual ~BesError();
public:

    /*
     *  功能：
     *      设置错误
     *  参数
     *      type        :   错误类型
     *  返回：
     *      无
     */
    virtual void SetError(BesError::ErrorType type);

    /*
     *  功能：
     *      设置错误
     *  参数
     *      type        :   错误类型
     *      parameter   :   参数
     *  返回：
     *      无
     */
    virtual void SetError(BesError::ErrorType type, string parameter);

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
    virtual void SetError(BesError::ErrorType type, string parameter, string debuginfo, bool overwrite);

    /*
     *  功能：
     *      设置错误
     *  参数
     *      errobj      :   错误类型
     *  返回：
     *      无
     */
    virtual void SetError(BesError *errobj);

    /*
     *  功能：
     *      获取错误类型
     *  参数
     *      无
     *  返回：
     *      返回错误类型
     */
    virtual BesError::ErrorType GetErrorType();

    /*
     *  功能：
     *      获取错误参数
     *  参数
     *      无
     *  返回：
     *      错误参数
     */
    virtual string GetErrorParameter();

    /*
     *  功能：
     *      获取错误描述
     *  参数
     *      无
     *  返回：
     *      错误信息描述
     */
    virtual string GetErrorDescription();

    /*
     *  功能：
     *      获取错误描述
     *  参数
     *      lang        :   错误描述语言
     *  返回：
     *      错误信息描述
     */
    virtual string GetErrorDescription(DescriptionLang lang);

    /*
     *  功能：
     *      获取错误描述
     *  参数
     *      type        :   错误类型
     *  返回：
     *      错误信息描述
     */
    static string GetErrorDescription(BesError::ErrorType type);

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
    static string GetErrorDescription(BesError::ErrorType type, string parameter);

    /*
     *  功能：
     *      获取错误描述
     *  参数
     *      type        :   错误类型
     *      parameter   :   参数
     *  返回：
     *      错误信息描述
     */
    static string GetErrorDescription(DescriptionLang lang, BesError::ErrorType type, string parameter);

    /*
     *  功能：
     *      获取错误描述
     *  参数
     *      type        :   错误类型
     *      parameter   :   参数
     *      result      :   自检结果
     *  返回：
     *      错误信息描述
     */
    static string GetErrorDescription(BesError::ErrorType type, string parameter, BesError::SelfCheckResult result);

    /*
     *  功能：
     *      获取授权项名称
     *  参数
     *      item        :   授权项
     *  返回：
     *      授权项名称
     */
    static string GetLicenseItemName(BesError::LicenseItem item);

    /*
     *  功能：
     *      清除错误
     *  参数
     *      无
     *  返回：
     *      无
     */
    virtual void Clear();

    /*
     *  功能：
     *      是否有错误
     *  参数
     *      无
     *  返回：
     *      如果有返回true，否则返回false
     */
    virtual bool HasError();

private:
    ErrorType MyType; // 错误类型
    string MyDescription; // 错误描述
    string MyParameter; // 参数
    string MyDebugInfo;
private:
#define DEFAULT_SELF_CHECK_MESSAGE_LENGTH           48
private:
    /*
     *  功能：
     *      获取错误描述
     *  参数
     *      lang        :   语言
     *      type        :   错误类型
     *  返回：
     *      错误描述
     */
    static string GetInteralErrorDescription(DescriptionLang lang, ErrorType type);

    /*
     *  功能：
     *      获取自检结果描述
     *  参数
     *      lang        :   语言
     *      result      :   自检结果
     *  返回：
     *      自检结果描述
     */
    static string GetSelfCheckResultDescription(DescriptionLang lang, SelfCheckResult result);
};

#endif	/* _BESERROR_H */

