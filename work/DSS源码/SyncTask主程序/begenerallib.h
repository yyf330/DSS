/*******************************************************************************
 *
 ********************* 程序文件:  BeGeneralLib            ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-06-28              ***********************
 ********************* 完成时间:  2010-08-08              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *……
 *20100820  LATIS   1)增加了字符串修改函数
 *20100824  LATIS   1)增加了读取shell所有返回值函数
 *20100827  LATIS   1)将数字转换为字符串的输入数值修改为长整型
 *20100827  LATIS   1)增加了启动进程的函数
 *20100901  LATIS   1)修改了读取命令返回单一值的处理方法
 *20100901  LATIS   2)增加了CheckAndCreateDir方法
 *20100902  LATIS   1)增加了ForceRemoveDirectory方法
 *20100904  LATIS   1)增加了GetDirectoryName方法
 *20100910  LATIS   1)增加了GetTimeString方法
 *20110112  LATIS   1)增加了StringReplace方法
 *20110522  LATIS   1)增加了GetTimeString的重载方法
 *20110616  LATIS   1)增加了CreateStandardCommand的重载方法
 *20110616  LATIS   2)增加了SumStringChars方法
 *20110616  LATIS   3)增加了StringReverse方法
 *20110616  LATIS   4)增加了Cipher方法
 *20120314  LATIS   1)增加了GetConfigItemValue方法
 *20120405  LATIS   1)增加了SetSystemTime方法
 *20120418  LATIS   1)增加了MoveFile方法
 *20120420  LATIS   1)增加了GetDeviceFileSystem方法
 *20120710  LATIS   1)增加了GetDiskUsedPercentage方法
 *                  2)增加了StringToInteger方法
 *20120717  LATIS   1)增加了EscapeString方法
 *20121110  latis   1)增加了IsFileContainConfigValue、GetBootDevice、RefreshFCScsi、GetFCHostWWN、DeleteBlockDevice和GetDriverFullPath方法
 *20121112  latis   1)增加了MountDevice重载方法
 *20121213  latis   1)增加了GenerateKvmMac方法
 *20121214  latis   1)增加了IsTcpPortOpen方法
 *20121217  latis   1)增加了IsValidMacAddress方法
 *                  2)增加了IsAllHexDigits方法
 *20130221  latis   1）增加了Utf8ToUnicode、UnicodeToUtf8和CharsetConvert方法
 * 
//================================== 修改结束 ==================================*/

#ifndef _BEGENERALLIB_H
#define	_BEGENERALLIB_H

#include "besgeneralheader.h"
#include "beslog.h"
//#include "becommand.h"

class BeGeneralLib
{
public:

    /*
     *  功能：
     *      构造函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    BeGeneralLib();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~BeGeneralLib();
public:

    enum SplitOptions
    {
        WITH_EMPTY_ITEM,
        REMOVE_EMPTY_ITEM,
    };

    enum TimeFormat
    {
        TIME_FORMAT_LONG, // %Y-%m-%d %H:%M:%S
        TIME_FORMAT_DATE, // %Y-%m-%d
        TIME_FORMAT_TIME, // %H:%M:%S
        TIME_FORMAT_FULL_DATETIME, // %Y%m%d%H%M%S
        TIME_FORMAT_FULL_DATE, // %Y%m%d
        TIME_FORMAT_FULL_TIME, // %H%M%S
        TIME_FORMAT_YEAR, //
        TIME_FORMAT_MONTH, //
        TIME_FORMAT_DAY,
        TIME_FORMAT_HOUR,
        TIME_FORMAT_MINUTE,
        TIME_FORMAT_SECOND,
        TIME_FORMAT_COMMAND_HEADER, // %d%H%M%S
    };

    enum ServiceAction
    {
        SERVICE_ACTION_START,
        SERVICE_ACTION_STOP,
        SERVICE_ACTION_RESTART,
    };

    enum MountedCheckItem
    {
        MOUNTED_CHECK_ITEM_DEVICE,
        MOUNTED_CHECK_ITEM_MOUNTPOINT,
    };

    enum LinuxKernelVersion
    {
        LINUX_KERNEL_VERSION_RHEL3_8,
        LINUX_KERNEL_VERSION_RHEL4_0,
        LINUX_KERNEL_VERSION_RHEL4_1,
        LINUX_KERNEL_VERSION_RHEL4_2,
        LINUX_KERNEL_VERSION_RHEL4_3,
        LINUX_KERNEL_VERSION_RHEL4_4,
        LINUX_KERNEL_VERSION_RHEL4_5,
        LINUX_KERNEL_VERSION_RHEL4_6,
        LINUX_KERNEL_VERSION_RHEL4_7,
        LINUX_KERNEL_VERSION_RHEL4_8,
        LINUX_KERNEL_VERSION_RHEL4_9,
        LINUX_KERNEL_VERSION_RHEL5_0,
        LINUX_KERNEL_VERSION_RHEL5_1,
        LINUX_KERNEL_VERSION_RHEL5_2,
        LINUX_KERNEL_VERSION_RHEL5_3,
        LINUX_KERNEL_VERSION_RHEL5_4,
        LINUX_KERNEL_VERSION_RHEL5_5,
        LINUX_KERNEL_VERSION_RHEL5_6,
        LINUX_KERNEL_VERSION_RHEL5_7,
        LINUX_KERNEL_VERSION_RHEL6_0,
    };

    enum LinuxOSType
    {
        LINUX_OS_TYPE_RHEL,
        LINUX_OS_TYPE_SUSE,
    };

    // 时间周期

    enum TimeCycle
    {
        TIME_CYCLE_HOUR = 1,
        TIME_CYCLE_DAY = 2,
        TIME_CYCLE_WEEK = 3,
        TIME_CYCLE_MONTH = 4,
        TIME_CYCLE_MINUTE = 5,
    };

    // 时间单位

    enum TimeUnit
    {
        TIME_UNIT_SECOND,
        TIME_UNIT_MINUTE,
        TIME_UNIT_HOUR,
        TIME_UNIT_DAY,
        TIME_UNIT_MONTH,
        TIME_UNIT_YEAR,
        TIME_UNIT_WEEK,
    };

    // 磁盘大小单位

    enum DiskSizeUnit
    {
        DISK_SIZE_UNIT_B,
        DISK_SIZE_UNIT_K,
        DISK_SIZE_UNIT_M,
        DISK_SIZE_UNIT_G,
        DISK_SIZE_UNIT_T,
        DISK_SIZE_UNIT_P,
        DISK_SIZE_UNIT_E,
        DISK_SIZE_UNIT_Z,
        DISK_SIZE_UNIT_Y,
    };

    // 文件类型

    enum FileType
    {
        FILE_TYPE_UNKNOWN, // 未知
        FILE_TYPE_REGULAR, // 普通文件
        FILE_TYPE_DIRECTORY, // 目录
        FILE_TYPE_CHARDEVICE, // 字符设备
        FILE_TYPE_BLOCK, // 块设备
        FILE_TYPE_FIFO, // FIFO或管道
        FILE_TYPE_SYMLINK, // 符号链接
        FILE_TYPE_SOCKET, // 套接字
    };

    // 进制

    enum NumberBase
    {
        NUMBER_BASE_2,
        NUMBER_BASE_8,
        NUMBER_BASE_10,
        NUMBER_BASE_16,
    };

    // 写文件方式

    enum WriteFileMode
    {
        WRITE_FILE_MODE_APPEND,
        WRITE_FILE_MODE_CREATE,
        WRITE_FILE_MODE_CLEAR,
    };

    // 套接字协议

    enum SocketProtocol
    {
        SOCKET_PROTOCOL_TCP,
        SOCKET_PROTOCOL_UDP,
    };
public:
	/*
	static bool CheckDogExist();
	static int ReadDogHostCount();
	static bool WriteDogHostCount(int cnt);
	static string LongToString5(long l_createtime);
	static long String5ToLong(string s_createtime);
	static long ConvertDogCreateTime(string s_createtime);
	static int GetDogIndex(string s_createtime);
	static bool DelDogTime(int index);
	static bool AddDogTime(string s_createtime);
	*/

    /*
     *  功能：
     *      数字转换为字符串
     *  参数：
     *      num             :   数字
     *  返回：
     *      转换后的字符串形式
     */
    static string Number2String(long num);

    //    /*
    //     *  功能：
    //     *      数字转换为字符串
    //     *  参数：
    //     *      num             :   数字
    //     *  返回：
    //     *      转换后的字符串形式
    //     */
    //    static string Number2String(float num);

    /*
     *  功能：
     *      数字转换为字符串
     *  参数：
     *      num             :   数字
     *      precision       :   精度
     *  返回：
     *      转换后的字符串形式
     */
    static string Number2String(float num, int precision);

    /*
     *  功能：
     *      字符转换为字符串
     *  参数：
     *      c               :   字符
     *  返回：
     *      转换后的字符串
     */
    static string Char2String(char c);

    /*
     *  功能：
     *      字符串转换为整数
     *  参数：
     *      str             :   字符串
     *  返回：
     *      转换后的整数
     */
    static int StringToInt(string str);

    /*
     *  功能：
     *      字符串转换为浮点数
     *  参数：
     *      str             :   字符串
     *  返回：
     *      转换后的浮点数
     */
    static float StringToFloat(string str);

    /*
     *  功能：
     *      字符串转换为bool
     *  参数：
     *      str             :   字符串
     *  返回：
     *      转换后的bool值
     */
    static bool StringToBool(string str);


    /*
     *  功能：
     *      字符串转为大写
     *  参数
     *      str             :   输入字符串
     *  返回：
     *      命令字符串
     */
    static string StringToUpper(string str);

    /*
     *  功能：
     *      字符串转为大写
     *  参数
     *      str             :   输入字符串
     *  返回：
     *      命令字符串
     */
    static string StringToLower(string str);

    /*
     *  功能：
     *      按指定的字符对字符串进行分割
     *  参数
     *      str             :   要操作的字符串
     *      ch              :   要分割的字符
     *  返回：
     *      分割后的矢量列表
     */
    static vector<string> StringSplit(string str, char ch, SplitOptions option);

    /*
     *  功能：
     *      删除字符串两端空格
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      删除后的字符串
     */
    static string StringTrim(string str);

    /*
     *  功能：
     *      删除字符串两端指定字符
     *  参数
     *      str             :   要操作的字符串
     *      ch              :   要删除字符
     *  返回：
     *      删除后的字符串
     */
    static string StringTrim(string str, char ch);

    /*
     *  功能：
     *      删除字符串左端指定空格
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      删除后的字符串
     */
    static string StringLTrim(string str);

    /*
     *  功能：
     *      删除字符串左端指定字符
     *  参数
     *      str             :   要操作的字符串
     *      ch              :   要删除字符
     *  返回：
     *      删除后的字符串
     */
    static string StringLTrim(string str, char ch);

    /*
     *  功能：
     *      删除字符串右端指定空格
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      删除后的字符串
     */
    static string StringRTrim(string str);

    /*
     *  功能：
     *      删除字符串右端指定字符
     *  参数
     *      str             :   要操作的字符串
     *      ch              :   要删除字符
     *  返回：
     *      删除后的字符串
     */
    static string StringRTrim(string str, char ch);

    /*
     *  功能：
     *      将str中所有的src替换为dest
     *  参数
     *      str             :   要操作的字符串
     *      src             :   源子串
     *      dest            :   替换为的子串
     *  返回：
     *      替换后的字符串
     */
    static string StringReplace(string str, string src, string dest);

    /*
     *  功能：
     *      将str进行反转
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      反转后的字符串
     */
    static string StringReverse(string str);

    /*
     *  功能：
     *      检查字段值是否为空或无效值
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsStringEmptyOrInvalidValue(string str);

    /*  功能：
     *      检查字段值是否为空或无效值
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsStringEmptyOrZero(string str);

    /*
     *  功能：
     *      检查字段值是否为空
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsStringEmpty(string str);

    /*
     *  功能：
     *      替换无效值
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      如果str是无效值，则替换为另一个无效值，否则返回源值
     */
    static string ReplaceInvalidValue(string str);

    /*
     *  功能：
     *      替换无效值
     *  参数
     *      str             :   要操作的字符串
     *     newvalue        :   新值
     *  返回：
     *      如果str是无效值，则替换为newvalue，否则返回源值
     */
    static string ReplaceInvalidValue(string str, string newvalue);

    /*
     *  功能：
     *      替换str中的转义字符
     *  参数
     *      str             :   要操作的字符串
     *  返回：
     *      转义后的字符串
     */
    static string EscapeString(string str);

    /*
     *  功能：
     *      读取shell返回值
     *  参数
     *      shellcommand    :   shell命令
     *  返回：
     *      shell返回值
     */
    static string ReadShellReturnValue(string shellcommand);

    /*
     *  功能：
     *      读取shell返回值
     *  参数
     *      shellcommand    :   shell命令
     *      tolog           :   输出到日志
     *  返回：
     *      shell返回值
     */
    static string ReadShellReturnValue(string shellcommand, bool tolog);

    /*
     *  功能：
     *      读取shell所有返回值
     *  参数
     *      shellcommand    :   shell命令
     *  返回：
     *      shell返回值
     */
    static vector<string> ReadShellReturnValueAll(string shellcommand);

    /*
     *  功能：
     *      读取shell所有返回值
     *  参数
     *      shellcommand    :   shell命令
     *      tolog           :   输出到日志
     *  返回：
     *      shell返回值
     */
    static vector<string> ReadShellReturnValueAll(string shellcommand, bool tolog);

    /*
     *  功能：
     *      获取执行路径名
     *  参数：
     *      无
     *  返回：
     *      执行路径名
     */
    static string GetExecutePath();

    /*
     *  功能：
     *      获取目录使用空间
     *  参数：
     *      folder              :   目录（绝对路径）
     *  返回：
     *      使用空间（MB）
     */
    static int GetFolderSpace(string folder);

    /*
     *  功能：
     *      从文件中替换指定键的键值
     *  参数
     *      filename        :   文件名
     *      key             :   键名
     *      separator       :   键名和键值分割符
     *      新的键值
     *  返回：
     *      无
     */
    static bool IsFileContainConfigValue(string filename, string key, string separator, string value);

    /*
     *  功能：
     *      从文件中获取指定键值
     *  参数
     *      filename        :   文件名
     *      key             :   键名
     *      separator       :   键名和键值分割符
     *  返回：
     *      键值
     */
    static string GetValueFromFile(string filename, string key, string separator);

    /*
     *  功能：
     *      从文件中替换指定键的键值
     *  参数
     *      filename        :   文件名
     *      key             :   键名
     *      separator       :   键名和键值分割符
     *      新的键值
     *  返回：
     *      无
     */
    static void ReplaceValueFromFile(string filename, string key, string separator, string value);

    /*
     *  功能：
     *      检测实例是否存在
     *  参数
     *      无
     *  返回：
     *      存在返回true，否则返回false
     */
    static bool IsInstanceExist();

    /*
     *  功能：
     *      获取指定进行名的执行路径
     *  参数：
     *      processname     :   进程名
     *  返回：
     *      执行路径名
     */
    static string GetProcessExecutePath(string processname);

    /*
     *  功能：
     *      将字符串类型数据写入文件中
     *  参数：
     *      fp          :   文件指针
     *      datastr     :   字符串数据
     *  返回：
     *      无
     */
    static void WriteDataToFile(FILE *fp, string datastr);

    /*
     *  功能：
     *      执行系统命令
     *  参数
     *      shellcommand    :   执行命令串
     *  返回：
     *      无
     */
    static bool ExecuteSystem(string shellcommand);

    /*
     *  功能：
     *      执行系统命令
     *  参数
     *      shellcommand    :   执行命令串
     *      redirect        :   重定向
     *  返回：
     *      无
     */
    static bool ExecuteSystem(string shellcommand, bool redirect);

    /*
     *  功能：
     *      执行系统命令
     *  参数
     *      shellcommand    :   执行命令串
     *      redirect        :   重定向
     *      debug           :   打印调试信息
     *  返回：
     *      无
     */
    static bool ExecuteSystem(string shellcommand, bool redirect, bool debug);

    /*
     *  功能：
     *      执行系统命令
     *  参数
     *      shellcommand    :   执行命令串
     *      redirect        :   重定向
     *      msgtype         :   信息类型
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool ExecuteSystem(string shellcommand, bool redirect, BesLog::LogMessageType msgtype);

    /*
     *  功能：
     *      关闭套接字
     *  参数
     *      sock            :   执行命令串
     *  返回：
     *      无
     */
    static void CloseSock(int sock);

    /*
     *  功能：
     *      检查服务是否存在
     *  参数
     *      servicename     :   服务名
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool IsServiceExist(string servicename);

    /*
     *  功能：
     *      检查服务是否正在运行
     *  参数
     *      servicename     :   服务名
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsServiceRunning(string servicename);

    /*
     *  功能：
     *      操作服务
     *  参数
     *      action          :   动作
     *      servicename     :   服务名
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool OperateService(BeGeneralLib::ServiceAction action, string servicename);

    /*
     *  功能：
     *      检查服务是否启动，如果没有根据启动标志启动
     *  参数：
     *      servicename     :   服务名称
     *      startflag       :   是否启动标志
     *  返回：
     *      如果启动返回true，否则返回false
     */
    static bool CheckService(string servicename, bool startflag);

    /*
     *  功能：
     *      加密
     *  参数
     *      input           :   输入
     *      enflag          :   加密标志
     *  返回：
     *      加密后的字符串
     */
    static string Cipher(string input, bool enflag = true);

    /*
     *  功能：
     *      加密
     *  参数
     *      input           :   输入
     *      factor          :   加密因子
     *      enflag          :   加密标志
     *  返回：
     *      加密后的字符串
     */
    static string Cipher(string input, int factor, bool enflag = true);

    /*
     *  功能：
     *      执行进程
     *  参数
     *      procstring      :   执行命令串
     *  返回：
     *      返回执行结果
     */
    static bool StartupProcess(string procstring);

    /*
     *  功能：
     *      执行进程
     *  参数
     *      procstring      :   执行命令串
     *      isbackground    :   是否后台运行
     *  返回：
     *      执行成功返回true，否则返回false
     */
    static bool StartupProcess(string procstring, bool isbackground);

    /*
     *  功能：
     *      检查并创建目录
     *  参数
     *      path            :   路径名
     *  返回：
     *      无
     */
    static void CheckAndCreateDir(string path);

    /*
     *  功能：
     *      强制删除目录
     *  参数
     *      path            :   路径名
     *      todebug         :   是否输出
     *  返回：
     *      无
     */
    static void ForceRemoveDirectory(string path, bool todebug = true);

    /*
     *  功能：
     *      检测目录下文件个数
     *  参数：
     *      folder          :   目录
     *  返回：
     *      文件个数(含目录)
     */
    static int CheckFileNumberInFolder(string folder);

    /*
     *  功能：
     *      根据文件名获取上级目录名
     *  参数：
     *      file            :   文件名
     *  返回：
     *      目录名
     */
    static string GetDirectoryName(string file);

    /*
     *  功能：
     *      根据文件全路径获取文件名
     *  参数：
     *      path            :   文件
     *  返回：
     *      文件名
     */
    static string GetFileName(string path);

    /*
     *  功能：
     *      获取文件类型
     *  参数：
     *      path            :   文件
     *  返回：
     *      文件类型
     */
    static BeGeneralLib::FileType GetFileType(string path);

    /*
     *  功能：
     *     获取指定文件大小
     *  参数：
     *      path            :   文件
     *  返回：
     *      文件大小
     */
    static long int GetFileSize(string path);

    /*
     *  功能：
     *      获取文件上次访问时间
     *  参数：
     *      path            :   文件
     *  返回：
     *      成功返回上次访问时间，否则返回-1
     */
    static time_t GetFileAccessTime(string path);

    /*
     *  功能：
     *      获取文件上次修改时间
     *  参数：
     *      path            :   文件
     *  返回：
     *      成功返回上次修改时间，否则返回-1
     */
    static time_t GetFileModificationTime(string path);

    /*
     *  功能：
     *      获取文件上次状态修改时间
     *  参数：
     *      path            :   文件
     *  返回：
     *      成功返回上次状态修改时间，否则返回-1
     */
    static time_t GetFileStatusChangeTime(string path);

    /*
     *  功能：
     *      移动文件
     *  参数：
     *      source          :   源文件名
     *      dest            :   目标文件名
     *      overwrite       :   是否覆盖
     *  返回：
     *      操作成功返回true,否则返回false
     */
    static bool MoveFile(string source, string dest, bool overwrite, bool debug = true);

    /*
     *  功能：
     *      移动文件
     *  参数：
     *      source          :   源文件名
     *      dest            :   目标文件名
     *      overwrite       :   是否覆盖
     *  返回：
     *      操作成功返回true,否则返回false
     */
    static bool CopyFile(string source, string dest, bool overwrite, bool debug = true);

    /*
     *  功能：
     *      修改文件时间戳
     *  参数：
     *      path            :   文件名
     *  返回：
     *      无
     */
    static void ChangeFileTimestamp(string path);

    /*
     *  功能：
     *      获取当前系统时间
     *  参数：
     *      无
     *  返回：
     *      系统时间值
     */
    static time_t GetSystemTime();

    /*
     *  功能：
     *      获取当前时间格式
     *  参数：
     *      format          :   时间格式
     *  返回：
     *      时间字符串
     */
    static string GetTimeString(BeGeneralLib::TimeFormat format);

    /*
     *  功能：
     *      获取当前时间格式
     *  参数：
     *      time            :   时间
     *      format          :   时间格式
     *  返回：
     *      时间字符串
     */
    static string GetTimeString(time_t time, BeGeneralLib::TimeFormat format);

    /*
     *  功能：
     *      获取当前时间格式
     *  参数：
     *      tmptr           :   时间结构指针
     *      format          :   时间格式
     *  返回：
     *      时间字符串
     */
    static string GetTimeString(struct tm *tmptr, BeGeneralLib::TimeFormat format);

    /*
     *  功能：
     *      获取当前时间格式
     *  参数：
     *      tmptr           :   时间结构指针
     *      format          :   时间格式
     *  返回：
     *      时间字符串
     */
    static bool SetSystemTime(string timestring);

    /*
     *  功能：
     *      将时间字符串转换为时间类型
     *  参数：
     *      timestring      :   时间结构指针
     *      format          :   时间格式
     *  返回：
     *      时间类型
     */
    static time_t ConvertStringToTime(string timestring, BeGeneralLib::TimeFormat format);

    /*
     *  功能：
     *      计算周期时间
     *  参数：
     *      starttime       :   开始时间
     *      currenttime     :   当前时间
     *      cycle           :   周期
     *      interval        :   间隔
     *  返回：
     *      下次运行时间
     */
    static string CalcCycleTime(string starttime, string currenttime, BeGeneralLib::TimeCycle cycle, int interval);

    /*
     *  功能：
     *      日期时间加减
     *  参数：
     *      time            :   时间
     *      unit            :   时间单位
     *      interval        :   间隔（可以为负值）
     *  返回：
     *      运算后的时间
     */
    static time_t DatetimeAdd(time_t time, BeGeneralLib::TimeUnit unit, int interval);

    /*
     *  功能：
     *      日期时间加减
     *  参数：
     *      time            :   时间
     *      unit            :   时间单位
     *      interval        :   间隔（可以为负值）
     *  返回：
     *      运算后的时间
     */
    static time_t DatetimeAdd(string time, BeGeneralLib::TimeUnit unit, int interval);

    /*
     *  功能：
     *      返回日期间隔的小时数（time1 - time2）
     *  参数：
     *      time1           :   时间1
     *      time2           :   时间2
     *  返回：
     *      间隔的小时数
     */
    static double DiffTimeToHours(time_t time1, time_t time2);

    /*
     *  功能：
     *      返回日期间隔的天数（time1 - time2）
     *  参数：
     *      time1           :   时间1
     *      time2           :   时间2
     *  返回：
     *      间隔的天数
     */
    static double DiffTimeToDays(time_t time1, time_t time2);

    /*
     *  功能：
     *      获取转换后的时间星期几索引(星期一-1，星期二-2...星期日-7)
     *  参数：
     *      time            :   时间
     *  返回：
     *      星期几索引
     */
    static int GetWeekday(time_t time);

    /*
     *  功能：
     *      获取转换后的时间星期几索引(星期一-1，星期二-2...星期日-7)
     *  参数：
     *      tmptr           :   时间结构
     *  返回：
     *      星期几索引
     */
    static int GetWeekday(struct tm *tmptr);

    /*
     *  功能：
     *      获取指定日期中月的第几天
     *  参数：
     *      time            :   时间
     *  返回：
     *      月的第几天
     */
    static int GetMonthDay(time_t time);

    /*
     *  功能：
     *      获取指定日期中月的第几天
     *  参数：
     *      tmptr           :   时间结构
     *  返回：
     *      月的第几天
     */
    static int GetMonthDay(struct tm* tmptr);

    /*
     *  功能：
     *      比较两个给定时间的时间部分大小
     *  参数：
     *      time1           :   时间1
     *      time2           :   时间2
     *  返回：
     *      如果返回大于0，则time1 > time2
     *      如果等于0，则time1 = time2
     *      如果小于0，则time1 < time2
     */
    static int CompareTime(time_t time1, time_t time2);

    /*
     *  功能：
     *      计算给定时间和当前时间的时间绝对值（单位秒）
     *  参数：
     *      time1           :   时间1
     *  返回：
     *      时间差
     */
    static double DiffTime(time_t time1);

    /*
     *  功能：
     *      计算两个给定时间的时间差（单位秒）
     *  参数：
     *      time1           :   时间1
     *      time2           :   时间2
     *      isabs           :   是否为绝对值
     *  返回：
     *      时间差
     */
    static double DiffTime(time_t time1, time_t time2, bool isabs = true);

    /*
     *  功能：
     *      文件是否存在
     *  参数：
     *      file            :   文件名
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool IsFileExist(string file);

    /*
     *  功能：
     *      获取应用程序名
     *  参数：
     *      无
     *  返回：
     *      应用程序名
     */
    static string GetApplicationName();

    /*
     *  功能：
     *      对字符串的每一位字符进行求和
     *  参数：
     *      str             :   输入字符串
     *  返回：
     *      返回求和值
     */
    static int SumStringChars(string str);

    /*
     *  功能：
     *      检查字符串是否全部为数字
     *  参数：
     *      str             :   待检查的字符串
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsAllDigits(string str);

    /*
     *  功能：
     *      检查字符串是否全部为十六进制数
     *  参数：
     *      str             :   待检查的字符串
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsAllHexDigits(string str);

    /*
     *  功能：
     *      检查字符串是否为有效的IP
     *  参数：
     *      ipstr           :   ip字符串
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsValidIp(string ipstr);

    /*
     *  功能：
     *      检查字符串是否为有效的mac地址
     *  参数：
     *      macstr           :   mac字符串
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsValidMacAddress(string macstr);

    /*
     *  功能：
     *      生成kvm虚拟机的MAC地址
     *  参数
     *      无
     *  返回：
     *      成功返回MAC地址，否则返回空
     */
    static string GenerateKvmMac();

    /*
     *  功能：
     *      检查值是否存在于列表中
     *  参数：
     *      list            :   列表
     *      value           :   值
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool IsValueExist(vector<string> list, string value);

    /*
     *  功能：
     *      获取配置项值
     *  参数：
     *      item            :   项名称
     *  返回：
     *      配置项值
     */
    static string GetConfigItemValue(string item);

    /*
     *  功能：
     *      获取配置项值
     *  参数：
     *      configfile      :   配置文件名
     *      item            :   项名称
     *  返回：
     *      配置项值
     */
    static string GetConfigItemValue(string configfile, string item);

    /*
     *  功能：
     *      获取配置项值
     *  参数：
     *      configfile      :   配置文件名
     *      item            :   项名称
     *      separator       :   分割符
     *  返回：
     *      配置项值
     */
    static string GetConfigItemValue(string configfile, string item, string separator);

    /*
     *  功能：
     *     检查设备是否挂载
     *  参数：
     *      device          :   设备
     *  返回：
     *      如果挂载返回true，否则返回false
     */
    static bool IsDeviceMounted(string device);

    /*
     *  功能：
     *     检查设备是否挂载
     *  参数：
     *      device          :   设备
     *      mountpoint      :   挂载点
     *  返回：
     *      如果挂载返回true，否则返回false
     */
    static bool IsDeviceMounted(string device, string mountpoint);

    /*
     *  功能：
     *     检查设备是否挂载
     *  参数：
     *      mountedvalue    :   挂载值
     *      item            :   检查项
     *  返回：
     *      如果挂载返回true，否则返回false
     */
    static bool IsDeviceMounted(string mountedvalue, BeGeneralLib::MountedCheckItem item);

    /*
     *  功能：
     *      挂载设备
     *  参数：
     *      device          :   设备
     *      mountpoint      :   挂载点
     *      filesystem      :   文件系统
     *      cratemountpoint :   是否创建挂载点
     *  返回：
     *      如果挂载成功返回true，否则返回false
     */
    static bool MountDevice(string device, string mountpoint, BaseFileSystem filesystem, bool createmountpoint = true);

    /*
     *  功能：
     *      挂载设备
     *  参数：
     *      device          :   设备
     *      mountpoint      :   挂载点
     *      filesystem      :   文件系统
     *      cratemountpoint :   是否创建挂载点
     *  返回：
     *      如果挂载成功返回true，否则返回false
     */
    static bool MountDevice(string device, string mountpoint, string filesystem, bool createmountpoint = true);

    /*
     *  功能：
     *      根据挂载点获取挂载设备
     *  参数：
     *      mountpoint      :   挂载点
     *  返回：
     *      挂载的设备名
     */
    static string GetMountedDevice(string mountpoint);

    /*
     *  功能：
     *      卸载设备
     *  参数：
     *      device          :   设备
     *      mountpoint      :   挂载点
     *      removemountpoint    :   是否删除挂载点
     *  返回：
     *      如果卸载成功返回true，否则返回false
     */
    static bool UmountDevice(string device, string mountpoint, bool removemountpoint = false);

    /*
     *  功能：
     *      获取指定设备的文件系统
     *  参数：
     *      device          :   设备
     *  返回：
     *      如果成功返回文件系统，否则返回空
     */
    static string GetDeviceFileSystem(string device);

    /*
     *  功能：
     *      获取文件系统类型名称
     *  参数：
     *      filesystem       :   基本文件系统类型
     *  返回：
     *      如果成功返回文件系统，否则返回空
     */
    static string GetFileSystemName(BaseFileSystem filesystem);

    /*
     *  功能：
     *      根据文件系统名获取基本文件系统类型
     *  参数：
     *      filesystemname   :   文件系统名
     *  返回：
     *      基本文件系统类型
     */
    static BaseFileSystem GetBaseFileSystem(string filesystemname);

    /*
     *  功能：
     *      检查设备是否存在
     *  参数：
     *      device          :   设备
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool IsDeviceExist(string device);

    /*
     *  功能：
     *      根据设备链接获取设备名
     *  参数：
     *      devlink         :   设备链接
     *  返回：
     *      成功返回设备名，否则返回空
     */
    static string GetDeviceName(string devlink);

    /*
     *  功能：
     *      根据设备名获取设备链接
     *  参数：
     *      devname         :   设备名
     *  返回：
     *      设备链接
     */
    static vector<string> GetDeviceLinks(string devname);

    /*
     *  功能：
     *      获取空闲的loop设备
     *  参数：
     *      无
     *  返回：
     *      返回空闲的loop设备
     */
    static string GetFreeLoopDevice();

    /*
     *  功能：
     *      设置image对应的loop设备
     *  参数：
     *      imagefile       :   image文件
     *  返回：
     *      返回关联的loop设备
     */
    static string SetLoopDevice(string imagefile);

    /*
     *  功能：
     *      设置image文件关联的loop设备
     *  参数：
     *      imagefile       :   image文件
     *      offset          :   偏移量
     *  返回：
     *      返回关联的loop设备
     */
    static string SetLoopDevice(string imagefile, string offset);

    /*
     *  功能：
     *      释放指定的loop设备
     *  参数：
     *      loopdevice      :   loop设备名
     *  返回：
     *      无
     */
    static void FreeLoopDevice(string loopdevice);

    /*
     *  功能：
     *      释放指定的loop设备
     *  参数：
     *      loopdevice      :   loop设备名
     *      isdelpartition  :   是否删除分区
     *  返回：
     *      无
     */
    static void FreeLoopDevice(string loopdevice, bool isdelpartition);

    /*
     *  功能：
     *      释放指定的loop设备
     *  参数：
     *      imagefile       :   image文件
     *  返回：
     *      无
     */
    static void FreeFileLoopDevice(string imagefile);

    /*
     *  功能：
     *      释放指定的loop设备
     *  参数：
     *      imagefile       :   image文件
     *      isdelpartition  :   是否删除分区
     *  返回：
     *      无
     */
    static void FreeFileLoopDevice(string imagefile, bool isdelpartition);

    /*
     *  功能：
     *      获取映像文件对应的loop设备
     *  参数：
     *      imagefile       :   image文件
     *  返回：
     *      loop设备列表
     */
    static vector<string> GetFileLoopDevices(string imagefile);

    /*
     *  功能：
     *      获取loop设备对应的映像文件
     *  参数：
     *      loopdevice      :   loop设备
     *  返回：
     *      映像文件
     */
    static string GetLoopDeviceFile(string loopdevice);

    /*
     *  功能：
     *      检查指定的设备是否为loop设备
     *  参数：
     *      device          :   设备名
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsLoopDevice(string device);

    /*
     *  功能：
     *      检查loop设备是否绑定
     *  参数：
     *      loopdevice      :   loop设备
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsLoopDeviceBind(string loopdevice);

    /*
     *  功能：
     *      是否有未用的循环设备
     *  参数：
     *      无
     *  返回：
     *      如果有返回true，否则返回false
     */
    static bool HasUnusedLoopDevice();

    /*
     *  功能：
     *      绑定loop设备
     *  参数：
     *      imagefile       :   image文件
     *      isexist         :   设备是否存在的标志
     *  返回：
     *      如果绑定成功返回设备名，否则返回空
     */
    static string BindLoopDevice(string imagefile, bool &isexist);

    /*
     *  功能：
     *      删除loop设备分区
     *  参数：
     *      loopdevice      :   loop设备
     *  返回：
     *      无
     */
    static void DeleteLoopDevicePartition(string loopdevice);

    /*
     *  功能：
     *      创建loop设备节点
     *  参数：
     *      无
     *  返回：
     *      无
     */
    static void CreateLoopDeviceNode();

    /*
     *  功能：
     *      将loop设备挂载到指定的挂载点
     *  参数：
     *      device          :   loop设备
     *      mountpoint      :   挂载点
     *      filesystem      :   文件系统
     *      partitionnum    :   分区号
     *  返回：
     *      挂载成功返回true，否则返回false
     */
    static bool MountLoopDevice(string device, string mountpoint, BaseFileSystem filesystem, int partitionnum);

    /*
     *  功能：
     *      增加虚拟磁盘分区
     *  参数：
     *      device          :   设备名
     *  返回：
     *      无
     */
    static void AddVirtualDevicePartition(string device);

    /*
     *  功能：
     *      增加虚拟磁盘分区
     *  参数：
     *      device          :   设备名
     *      partlist        :   磁盘分区列表
     *  返回：
     *      无
     */
    static void AddVirtualDevicePartition(string device, vector<string>& partlist);

    /*
     *  功能：
     *      删除虚拟磁盘分区
     *  参数：
     *      device          :   设备名
     *  返回：
     *      无
     */
    static void DeleteVirtualDevicePartition(string device);

    /*
     *  功能：
     *      获取虚拟磁盘分区
     *  参数：
     *      device          :   设备名
     *      withvirtualpath :   是否含有虚拟路径
     *  返回：
     *      虚拟磁盘分区列表
     */
    static vector<string> GetVirtualDevicePartition(string device, bool withvirtualpath = true);

    /*
     *  功能：
     *      删除虚拟设备映射
     *  参数：
     *      device          :   设备名
     *  返回：
     *      无
     */
    static void DeleteVirtualDeviceMapper(string device);

    /*
     *  功能：
     *      获取指定设备的虚拟设备映射
     *  参数：
     *      device          :   设备名
     *      withvirtualpath :   是否含有虚拟路径
     *  返回：
     *      虚拟设备映射列表
     */
    static vector<string> GetVirtualDeviceMapper(string device, bool withvirtualpath = true);

    /*
     *  功能：
     *     在给定的路径后增加路径分割符
     *  参数：
     *      path            :   路径
     *  返回：
     *      返回处理后的路径
     */
    static string AddPathSeparator(string path);

    /*
     *  功能：
     *     在给定的路径后删除路径分割符
     *  参数：
     *      path            :   路径
     *  返回：
     *      返回处理后的路径
     */
    static string RemovePathSeparator(string path);

    /*
     *  功能：
     *     获取磁盘使用百分比
     *  参数：
     *      disk             :   磁盘名称
     *  返回：
     *      如果磁盘存在返回实际使用百分比，否则返回-1
     */
    static int GetDiskUsedPercentage(string disk);

    /*
     *  功能：
     *     按照指定的单位获取磁盘可用空间
     *  参数：
     *      disk            :   磁盘名称
     *      unit            :   单位
     *  返回：
     *      磁盘可用空间
     */
    static int GetDiskAvailableSize(string disk, BeGeneralLib::DiskSizeUnit unit);

    /*
     *  功能：
     *     获取系统盘名称
     *  参数：
     *      无
     *  返回：
     *      系统盘名称
     */
    static string GetSystemDiskName();

    /*
     *  功能：
     *     获取系内核版本
     *  参数：
     *      无
     *  返回：
     *      内核版本
     */
    static string GetKernelVersion();

    /*
     *  功能：
     *     获取系内核版本
     *  参数：
     *      linuxos         :   操作系统类型
     *      withrebuild     :   是否将内核版本重构，以点（.）为分割符
     *  返回：
     *      内核版本
     */
    static string GetKernelVersion(BeGeneralLib::LinuxOSType linuxos, bool withrebuild);

    /*
     *  功能：
     *     获取系内核版本主号
     *  参数：
     *      无
     *  返回：
     *      内核版本主号
     */
    static string GetKernelVersionMajor();

    /*
     *  功能：
     *     获取系内核版本次号
     *  参数：
     *      无
     *  返回：
     *      内核版本次号
     */
    static string GetKernelVersionMinor();

    /*
     *  功能：
     *     获取系内核版本补丁版本
     *  参数：
     *      无
     *  返回：
     *      内核版本补丁版本
     */
    static string GetKernelVersionPatch();

    /*
     *  功能：
     *     获取系内核版本打包信息
     *  参数：
     *      无
     *  返回：
     *      内核版本打包信息
     */
    static string GetKernelVersionBuild();

    /*
     *  功能：
     *     获取系内核版本描述
     *  参数：
     *      无
     *  返回：
     *      内核版本主号
     */
    static string GetKernelVersionDescription();

    /*
     *  功能：
     *     获取系内核版本归档信息
     *  参数：
     *      无
     *  返回：
     *      内核版本归档信息
     */
    static string GetKernelVersionArchive();

    /*
     *  功能：
     *     判断被保护主机内核是否是xxxx-xen内核
     *  参数：
     *      xenversion      :   返回xen的版本
     *  返回：
     *      如果是，则返回true，并设置xenversion，否则返回false
     */
    static bool IsXenKernel(string &xenversion);

    /*
     *  功能：
     *      根据ip地址获取对应网卡名称
     *  参数：
     *      ipaddr          :   ip地址
     *  返回：
     *      网卡名称
     */
    static string GetNicName(string ipaddr);

    /*
     *  功能：
     *      根据ip地址获取对应MAC地址
     *  参数：
     *      ipaddr          :   ip地址
     *  返回：
     *      MAC地址
     */
    static string GetMacAddress(string ipaddr);

    /*
     *  功能：
     *     获取initiator名称
     *  参数：
     *      无
     *  返回：
     *      initiator名称
     */
    static string GetInitiatorName();

    //    static bool GreaterThanKernelVersion()

    /*
     *  功能：
     *     比较内核版本大小
     *  参数：
     *      kernelver1      :   内核版本1
     *      kernelver2      :   内核版本2
     *  返回：
     *      如果kernelver1 > kernelver2返回true，否则返回false
     */
    static bool GreaterThanKernelVersion(string kernelver1, string kernelver2);

    /*
     *  功能：
     *      格式化磁盘
     *  参数：
     *      device          :   磁盘设备
     *      filesystem      :   文件系统
     *  返回：
     *      格式化成功返回true，否则返回false
     */
    static bool FormatDisk(string device, string filesystem);

    /*
     *  功能：
     *      格式化磁盘
     *  参数：
     *      device          :   磁盘设备
     *      filesystem      :   文件系统
     *      label           :   卷标
     *  返回：
     *      格式化成功返回true，否则返回false
     */
    static bool FormatDisk(string device, string filesystem, string label);

    /*
     *  功能：
     *      格式化磁盘
     *  参数：
     *      device          :   磁盘设备
     *      filesystem      :   文件系统
     *      label           :   卷标
     *      isforce         :   是否强制格式化
     *  返回：
     *      格式化成功返回true，否则返回false
     */
    static bool FormatDisk(string device, string filesystem, string label, bool isforce);

    /*
     *  功能：
     *      将设备格式化为reiserfs
     *  参数
     *      device          :   设备
     *  返回：
     *      如果成功返回true，否则返回false
     */
    static bool FormatReiserfs(string device);

    /*
     *  功能：
     *      清零磁盘头
     *  参数：
     *      disk            :   磁盘设备
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool ZeroDiskHeader(string disk);

    /*
     *  功能：
     *      清零磁盘头
     *  参数：
     *      disk            :   磁盘设备
     *      sectors         :   扇区数
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool ZeroDiskHeader(string disk, long sectors);

    /*
     *  功能：
     *      获取数据卷卷标
     *  参数
     *      无
     *  返回：
     *      数据卷卷标
     */
    static string GetDataVolumeLabel();

    /*
     *  功能：
     *      创建空文件
     *  参数：
     *      filename        :   文件名
     *  返回：
     *      创建成功返回true，否则返回false
     */
    static bool CreateEmptyFile(string filename);

    /*
     *  功能：
     *      创建空文件
     *  参数：
     *      filename        :   文件名
     *      isforce         :   是否强制创建
     *  返回：
     *      创建成功返回true，否则返回false
     */
    static bool CreateEmptyFile(string filename, bool isforce);

    /*
     *  功能：
     *      追加数据到现有文件
     *  参数：
     *      filename        :   文件名
     *      data            :   数据内容
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool AppendToFile(string filename, string data);

    /*
     *  功能：
     *      将数据写入文件
     *  参数：
     *      filename        :   文件名
     *      data            :   数据内容
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool WriteFile(string filename, string data, BeGeneralLib::WriteFileMode mode);

    /*
     *  功能：
     *      从文件中读取数据
     *  参数：
     *      filename        :   文件名
     *      data            :   数据内容
     *  返回：
     *      成功返回true，否则返回false
     */
    static string ReadFile(string filename);

    /*
     *  功能：
     *      获取启动设备名
     *  参数：
     *      withprefix      :  是否含有前缀
     *  返回：
     *      如果成功返回设备名，否则返回空
     */
    static string GetBootDevice(bool withprefix = false);

    /*
     *  功能：
     *      刷新光纤scsi
     *  参数：
     *      wwn             :   如果为空，则刷新所有，否则刷新指定wwn的光纤scsi
     *  返回：
     *      如果刷新成功返回true，否则返回false
     */
    static bool RefreshFCScsi(string wwn);

    /*
     *  功能：
     *      获取指定编号的光纤的wwn
     *  参数：
     *      hostnum         :   编号
     *      withsepchar     :   是否带分割符
     *  返回：
     *      如果成功返回wwn，否则返回空
     */
    static string GetFCHostWWN(string hostnum, bool withsepchar = true);

    /*
     *  功能：
     *      列举光纤卡wwn
     *  参数：
     *      无
     *  返回：
     *      光纤卡wwn列表
     */
    static vector<string> ListFCWWN();

    /*
     *  功能：
     *      删除块设备
     *  参数：
     *      devicename      :   块设备名
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool DeleteBlockDevice(string devicename);

    /*
     *  功能：
     *      加载驱动
     *  参数：
     *      driverpath      :   驱动路径
     *      dirvername      :   驱动名称
     *      modulename      :   加载后的模块名称
     *  返回：
     *      如果加载返回true，否则返回false
     */
    static bool LoadOneDriver(string driverpath, string drivername, string modulename);

    /*
     *  功能：
     *      删除驱动
     *  参数：
     *      modulename      :   模块名称
     *  返回：
     *      如果删除返回true，否则返回false
     */
    static bool RemoveDriver(string modulename);

    /*
     *  功能：
     *      根据驱动名获取驱动全路径
     *  参数：
     *      drivername      :   驱动名
     *  返回：
     *      成功返回驱动全路径，否则返回空
     */
    static string GetDriverFullPath(string drivername);

    /*
     *  功能：
     *      根据驱动名在指定的内核下获取驱动全路径
     *  参数：
     *      drivername      :   驱动名
     *      kernelversion   :   内核版本
     *  返回：
     *      成功返回驱动全路径，否则返回空
     */
    static string GetDriverFullPath(string drivername, string kernelversion);

    /*
     *  功能：
     *      检查驱动是否加载
     *  参数：
     *      drivername      :   驱动名称
     *  返回：
     *      如果加载返回true，否则返回false
     */
    static bool IsDriverLoaded(string drivername);

    /*
     *  功能：
     *      发送udp数据
     *  参数
     *      ip              :   ip地址
     *      port            :   端口
     *      data            :   数据
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool SendUdpData(string ip, string port, string data);

    /*
     *  功能：
     *      发送udp数据
     *  参数
     *      ip              :   ip地址
     *      port            :   端口
     *      data            :   数据
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool SendUdpData(string ip, int port, string data);

    /*
     *  功能：
     *      发送tcp数据
     *  参数
     *      ip              :   ip地址
     *      port            :   端口
     *      data            :   数据
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool SendTcpData(string ip, string port, string data);

    /*
     *  功能：
     *      发送tcp数据
     *  参数
     *      ip              :   ip地址
     *      port            :   端口
     *      data            :   数据
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool SendTcpData(string ip, int port, string data);

    /*
     *  功能：
     *      检查TCP端口是否打开
     *  参数
     *      ip              :   ip地址
     *      port            :   端口
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool IsTcpPortOpen(string ip, int port);
    static bool IsTcpPortOpenNonBlock(string ip, int port);

    /*
     *  功能：
     *      获取监听端口进程id
     *  参数
     *      protocol        :   协议
     *      port            :   端口
     *  返回：
     *      进程id
     */
    static string GetListenPortProcessId(BeGeneralLib::SocketProtocol protocol, int port);

    /*
     *  功能：
     *      获取套接字协议名
     *  参数
     *      protocol        :   协议
     *  返回：
     *      协议名
     */
    static string GetSocketProtocolName(BeGeneralLib::SocketProtocol protocol);

    /*
     *  功能：
     *      检查端口是否被占用
     *  参数
     *      protocol        :   协议
     *      port            :   端口
     *  返回：
     *      如果时返回true，否则返回false
     */
    static bool IsPortUsed(BeGeneralLib::SocketProtocol protocol, string port);

    /*
     *  功能：
     *      检查端口是否被占用
     *  参数
     *      protocol        :   协议
     *      port            :   端口
     *  返回：
     *      如果时返回true，否则返回false
     */
    static bool IsPortUsed(BeGeneralLib::SocketProtocol protocol, int port);

    /*
     *  功能：
     *      检查ping是否OK
     *  参数
     *      ip              :   ip地址
     *      count           :   ping包个数
     *      interval        :   间隔
     *  返回：
     *      如果ping成功返回true，否则返回false
     */
    static bool IsPingOK(string ip, int count, float interval);


    /*
     *  功能：
     *      将ip字符串转换为ip结构
     *  参数
     *      ip              :   ip地址
     *  返回：
     *      ip结构
     */
    static struct in_addr ConvertIpStringToStruct(char * ip);

    /*
     *  功能：
     *      将ip结构转换为ip字符串
     *  参数
     *      addr            :   ip结构
     *  返回：
     *      ip结构
     */
    static string ConvertStructToIpString(struct in_addr addr);

    /*
     *  功能：
     *      获取子网
     *  参数
     *      ip              :   ip地址
     *      mask            :   子网掩码
     *  返回：
     *      获取子网
     */
    static string GetSubnet(string ip, string mask);

    /*
     *  功能：
     *      生成ip地址
     *  参数
     *      subnet          :   子网
     *      host            :   主机
     *  返回：
     *      ip地址
     */
    static string MakeIpAddr(string subnet, unsigned int host);

    /*
     *  功能：
     *      删除桥接
     *  参数
     *      bridge          :   桥接名
     *  返回：
     *      无
     */
    static void DeleteBridge(string bridge);

    /*
     *  功能：
     *      激活网卡
     *  参数
     *      interface       :   网卡
     *  返回：
     *      无
     */
    static void ActivateNetworkInterface(string interface);

    /*
     *  功能：
     *      停用网卡
     *  参数
     *      interface       :   网卡
     *  返回：
     *      无
     */
    static void DeactivateNetworkInterface(string interface);

    /*
     *  功能：
     *      桥接网卡是否存在
     *  参数
     *      interface       :   网卡
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool IsBridgeNicExist(string interface);

    /*
     *  功能：
     *      物理网卡是否存在
     *  参数
     *      interface       :   网卡
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool IsPhysicalNicExist(string interface);

    /*
     *  功能：
     *      将utf-8转换为unicode编码
     *  参数
     *      instring        :   输入的字符串
     *      outbuffer       :   输出缓冲
     *      outbuffersize   :   输出缓冲大小
     *  返回：
     *      转换的字节数
     */
    static int Utf8ToUnicode(string instring, char * outbuffer, size_t outbuffersize);

    /*
     *  功能：
     *      将unicode转换为utf-8编码
     *  参数
     *      inbuffer        :   输入缓冲
     *      inbuffersize    :   输入缓冲大小
     *      outstring       :   输出字符串
     *  返回：
     *      转换的字节数
     */
    static int UnicodeToUtf8(char * inbuffer, size_t inbuffersize, string &outstring);

    /*
     *  功能：
     *      更新image文件扇区数
     *  参数
     *      imagefile       :   镜像文件名
     *      size            :   磁盘大小
     *  返回：
     *      如果更新成功返回true，否则返回false
     */
    static bool UpdateImageFileSectors(string imagefile, int size);


    /*
     *  功能：
     *      更新image文件磁盘签名
     *  参数
     *      imagefile       :   镜像文件名
     *  返回：
     *      如果更新成功返回true，否则返回false
     */
    static bool UpdateImageFileSignature(string imagefile);

    /*
     *  功能：
     *      更新image文件文件系统
     *  参数
     *      imagefile       :   镜像文件名
     *      fs              :   文件系统
     *  返回：
     *      如果更新成功返回true，否则返回false
     */
    static bool UpdateImageFileSystem(string imagefile, BaseFileSystem fs);

    /*
     *  功能：
     *      更新image文件文件系统
     *  参数
     *      imagefile       :   镜像文件名
     *      partition       :   分区
     *      fs              :   文件系统
     *  返回：
     *      如果更新成功返回true，否则返回false
     */
    static bool UpdateImageFileSystem(string imagefile, int partition, BaseFileSystem fs);

    /*
     *  功能：
     *      导入镜像文件头
     *  参数：
     *      imagefile       :   镜像文件
     *      headerfile      :   头文件
     *  返回：
     *      导入成功返回true，否则返回false
     */
    static bool ImportImageHeader(string imagefile, string headerfile);

    /*
     *  功能：
     *      创建镜像文件
     *  参数
     *      filename        :   文件名
     *      capacity        :   容量
     *  返回：
     *      创建成功返回true，否则返回false
     */
    static bool CreateImageFile(string filename, int capacity);

    /*
     *  功能：
     *      创建iqn
     *  参数
     *      hcode           :   主机编码
     *      copyfile        :   副本文件名
     *  返回：
     *      无
     */
    static string GenerateIqn(string hcode, string copyfile);

    /*
     *  功能：
     *      创建副本挂载点
     *  参数
     *      path            :  路径
     *      copyid          :   副本id
     *      copyfile        :   副本文件名
     *  返回：
     *      无
     */
    static string GenerateCopyMountPoint(string path, string copyid, string copyfile);

    /*
     *  功能：
     *      获取主机名
     *  参数
     *      无
     *  返回：
     *      主机名
     */
    static string GetSystemHostName();

    /*
     *  功能：
     *      重置终端颜色
     *  参数
     *      无
     *  返回：
     *      无
     */
    static void ResetConsoleColor();

    /*
     *  功能：
     *      删除系统日志
     *  参数
     *      无
     *  返回：
     *      无
     */
    static void DeleteSystemLog();

    /*
     *  功能：
     *      获取符号链接文件对应文件的绝对路径
     *  参数
     *      linkfile        :   符号链接文件
     *  返回：
     *      符号链接文件对应文件的绝对路径
     */
    static string GetLinkFileFullPath(string linkfile);

    /*
     *  功能：
     *      配置动态链接
     *  参数
     *      无
     *  返回：
     *      无
     */
    static void ConfigureDynamicLinker();

    /*
     *  功能：
     *      创建符号链接
     *  参数
     *      target              :   目标文件
     *      linker              :   链接名
     *  返回：
     *      创建成功返回true，否则返回false
     */
    static bool CreateSymbolLinks(string target, string linker);

    /*
     *  功能：
     *      生成动态链接库文件名
     *  参数
     *      dynamiclinkname     :   动态链接库名
     *  返回：
     *      成功返回文件名，否则空
     */
    static string GenerateDynamicLinkFileName(string dynamiclinkname);

    /*
     *  功能：
     *      检查包是否安装
     *  参数
     *      无
     *  返回：
     *      如果已安装则返回true，否则返回false
     */
    static bool IsPackageInstalled(string name);

    /*
     *  功能：
     *      检查安装包
     *  参数
     *      无
     *  返回：
     *      无
     */
    static void CheckPackage();



    /*
     *  功能：
     *      检查端口是否使用，如果使用，则将使用的进程杀死
     *  参数
     *      port            :   端口号
     *  返回：
     *      无
     */
    static void CheckPort(int port);

    /*
     *  功能：
     *      杀死进程
     *  参数
     *      name            :   进程名
     *      isforce         :   是否强制杀死
     *  返回：
     *      无
     */
    static void KillProcess(string name, bool isforce);

    /*
     *  功能：
     *      杀死进程
     *  参数
     *      pid             :   进程id
     *      isforce         :   是否强制杀死
     *  返回：
     *      无
     */
    static void KillProcessById(string pid, bool isforce);

    /*
     *  功能：
     *      检查环境变量
     *  参数
     *      无
     *  返回：
     *      无
     */
    static void CheckEnvVariables();

    /*
     *  功能：
     *      检查进程是否运行
     *  参数
     *      processname     :   进程名
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool IsProcessRunning(string processname);

    /*
     *  功能：
     *      检查进程是否运行
     *  参数
     *      processname     :   进程名
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static vector<string> GetProcessId(string processname);

    /*
     *  功能：
     *      检查进程
     *  参数：
     *      processname     :   进程名
     *  返回：
     *      检查成功返回true，否则返回false
     */
    static bool CheckProcess(string processname);

    /*
     *  功能：
     *      检查文件权限
     *  参数：
     *      path            :   路径名
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool CheckFilePermission(string path);

    /*
     *  功能：
     *      检查文件权限
     *  参数：
     *      path            :   路径名
     *      mode            :   文件模式
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool CheckFilePermission(string path, int mode);

    /*
     *  功能：
     *      修改文件权限
     *  参数：
     *      path            :   路径名
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool ChangeFilePermission(string path);

    /*
     *  功能：
     *      修改文件权限
     *  参数：
     *      path            :   路径名
     *      mode            :   文件模式
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool ChangeFilePermission(string path, int mode);

    /*
     *  功能：
     *      修改文件权限
     *  参数：
     *      path            :   路径名
     *      mode            :   文件模式
     *      recursive       :   是否递归
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool ChangeFilePermission(string path, int mode, bool recursive);

    /*
     *  功能：
     *      检查并修改文件权限
     *  参数：
     *      path            :   路径名
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool CheckAndChangeFilePermission(string path);

    /*
     *  功能：
     *      检查并修改文件权限
     *  参数：
     *      path            :   路径名
     *      mode            :   文件模式
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool CheckAndChangeFilePermission(string path, int mode);

    /*
     *  功能：
     *      检查并修改文件权限
     *  参数：
     *      path            :   路径名
     *      mode            :   文件模式
     *      recursive       :   是否递归
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool CheckAndChangeFilePermission(string path, int mode, bool recursive);

    /*
     *  功能：
     *      检查并修改可执行文件权限
     *  参数：
     *      file            :   文件名
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool CheckAndChangeBinFilePermission(string file);

    /*
     *  功能：
     *      设置文件为锁定状态
     *  参数：
     *      path            :   文件名
     *      islock          :   是否锁定
     *      recursive       :   是否递归
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool SetFileLocked(string path, bool islock);

    /*
     *  功能：
     *      设置文件为锁定状态
     *  参数：
     *      path            :   文件名
     *      islock          :   是否锁定
     *      recursive       :   是否递归
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool SetFileLocked(string path, bool islock, bool recursive);

    /*
     *  功能：
     *      根据提示获取控制台输入
     *  参数：
     *      prompt          :   提示信息
     *  返回：
     *      控制台输入
     */
    static string ConsoleInput(string prompt);

    /*
     *  功能：
     *      根据提示获取控制台输入
     *  参数：
     *      prompt          :   提示信息
     *      defaultvalue    :   默认值
     *      length          :   输入长度
     *      withendline     :   是否有结尾符
     *  返回：
     *      控制台输入
     */
    static string ConsoleInput(string prompt, string defaultvalue, int length, bool withendline = false);

    /*
     *  功能：
     *      转换进制
     *  参数：
     *      input           :   输入
     *      obase           :   输出的进制
     *  返回：
     *      成功返回转换后的进制，否则为空
     */
    static string ConvertNumberBase(string input, BeGeneralLib::NumberBase obase);

    /*
     *  功能：
     *      转换进制
     *  参数：
     *      input           :   输入
     *      ibase           :   输入的进制
     *      obase           :   输出的进制
     *  返回：
     *      成功返回转换后的进制，否则为空
     */
    static string ConvertNumberBase(string input, BeGeneralLib::NumberBase ibase, BeGeneralLib::NumberBase obase);

    /*
     *  功能：
     *      十六进制转换为十进制
     *  参数：
     *      str             :   十六进制字符串
     *  返回：
     *      转换后的十进制数
     */
    static int HexToDecimal(string str);

    /*
     *  功能：
     *      将数组转换为十六进制字符串
     *  参数：
     *      arr             :   数组
     *      len             :   数组长度
     *  返回：
     *      转换后的十六进制字符串
     */
    static string ArrayToHexString(unsigned char *arr, int len);

    /*
     *  功能：
     *      将字节转换为十六进制字符串
     *  参数：
     *      byte            :   字节
     *      isupper         :   是否大写
     *  返回：
     *      转换后的十六进制字符串
     */
    static string ByteToHexString(unsigned char ch, bool isupper);

    /*
     *  功能：
     *      获取最后一个分割符后的字符串
     *  参数：
     *      str             :   字符串
     *      seperator       :   分隔符
     *  返回：
     *      成功返回转换后的进制，否则为空
     */
    static string GetStringAfterFinalSeperator(string str, char seperator);

    /*
     *  功能：
     *      获取描述语言
     *  参数：
     *      无
     *  返回：
     *      描述语言
     */
    static DescriptionLang GetDescriptionLang();

    /*
     *  功能：
     *      获取自检日志文件名
     *  参数：
     *      无
     *  返回：
     *      target基础
     */
    static string GetSelfCheckLogFileName();

    /*
     *  功能：
     *      获取字符串中ascii字符个数
     *  参数：
     *      str
     *  返回：
     *      ascii字符个数
     */
    static int AsciiCharCount(string str);

    /*
     *  功能：
     *       将UInt32转换为字符串
     *  参数
     *      str        	:	转换后的字符串
     *      value	:	数值
     *  返回：
     *      转换成功返回true，否则返回false
     */
    static bool UInt32ToString(char *str, int value);

private:
#define DEFAULT_UPPER_START_CHAR                    'A'
#define DEFAULT_LOWER_START_CHAR                    'a'
#define DEFAULT_NUMBER_START_CHAR                   '0'
#define DEFAULT_ALPHACHAR_COUNT                     26
#define DEFAULT_ALPHANUM_COUNT                      10
#define MAX_EXEC_PARAMETER_NUMBER                   16
#define DEFAULT_IP_SEGMENT_NUM                      4
#define MIN_IP_SEGMENT_VALUE                        0
#define MAX_IP_SEGMENT_VALUE                        255
#define DEFAULT_MAC_ADDRESS_LENGTH                  17                      // 默认mac地址长度
#define DEFAULT_MAC_SEGMENT_NUM                     6                       // 默认mac地址段数
#define DEFAULT_MAC_SINGLE_SEGMENT_LENGTH           2                       // 默认mac地址单段长度
#define DEFAULT_INITIATOR_CONF                      "/etc/iscsi/initiatorname.iscsi"        // initiator配置文件
#define DEFAULT_INITIATOR_CONF2                     "/etc/initiatorname.iscsi"              // initiator配置文件
#define DEFAULT_INITIATOR_NAME                      "InitiatorName"                         // initiator name字段
#define DEFAULT_FC_HOST_PATH                        "/sys/class/fc_host/"                   // fc_host的路径
#define DEFAULT_RHEL3_8_KERNEL_VERSION              "2.4.21.47"
#define DEFAULT_RHEL4_0_KERNEL_VERSION              "2.6.9.5"
#define DEFAULT_RHEL4_1_KERNEL_VERSION              "2.6.9.11"
#define DEFAULT_RHEL4_2_KERNEL_VERSION              "2.6.9.22"
#define DEFAULT_RHEL4_3_KERNEL_VERSION              "2.6.9.34"
#define DEFAULT_RHEL4_4_KERNEL_VERSION              "2.6.9.42"
#define DEFAULT_RHEL4_5_KERNEL_VERSION              "2.6.9.55"
#define DEFAULT_RHEL4_6_KERNEL_VERSION              "2.6.9.67"
#define DEFAULT_RHEL4_7_KERNEL_VERSION              "2.6.9.78"
#define DEFAULT_RHEL4_8_KERNEL_VERSION              "2.6.9.89"
#define DEFAULT_RHEL4_9_KERNEL_VERSION              "2.6.9.100"
#define DEFAULT_RHEL5_0_KERNEL_VERSION              "2.6.18.8"
#define DEFAULT_RHEL5_1_KERNEL_VERSION              "2.6.18.53"
#define DEFAULT_RHEL5_2_KERNEL_VERSION              "2.6.18.92"
#define DEFAULT_RHEL5_3_KERNEL_VERSION              "2.6.18.128"
#define DEFAULT_RHEL5_4_KERNEL_VERSION              "2.6.18.164"
#define DEFAULT_RHEL5_5_KERNEL_VERSION              "2.6.18.194"
#define DEFAULT_RHEL5_6_KERNEL_VERSION              "2.6.18.238"
#define DEFAULT_RHEL5_7_KERNEL_VERSION              "2.6.18.274"
#define DEFAULT_RHEL6_0_KERNEL_VERSION              "2.6.32.71"
#define DEFAULT_DRIVER_EXTENSION                    ".ko"                                   // 默认驱动扩展名
#define DEFAULT_MBR_SIGNATURE_OFFSET                440                                     // 默认MBR扇区偏移量
#define DEFAULT_HOST_NAME_CONFIG                    "/etc/sysconfig/network"                // 默认主机名配置文件
#define DEFAULT_VIRTUAL_MAPPER_PATH                 "/dev/mapper/"
#define DEFAULT_LOOP_DEVICE_NAME                    "/dev/loop"
#define DEFAULT_FREE_LOOP_DEVICE_RETRY_NUM          3                                       // 默认释放loop设备重试次数

#define DEFAULT_BASE_2_STRING                       "2"                                     // 二进制
#define DEFAULT_BASE_8_STRING                       "8"                                     // 八进制
#define DEFAULT_BASE_10_STRING                      "10"                                    // 十进制
#define DEFAULT_BASE_16_STRING                      "16"                                    // 十六进制

#define DEFAULT_DRIVER_EXTENSION                    ".ko"                                   // 默认驱动扩展名
#define MAX_LOAD_DRIVER_RETRY_NUM                   3                                       // 默认加载驱动重试次数

#define DEFAULT_MKFS_REISERFS_SCRIPT                "formatreiserfs.sh"                     // 格式化reiserfs脚本
private:

    /*
     *  功能：
     *      字符转换
     *  参数
     *      char            :   输入字符
     *      len             :   字符串长度
     *      isleft          :   转换方向
     *  返回：
     *      转换后的字符
     */
    static char TransferTo(char ch, int len, bool isleft);

    /*
     *  功能：
     *      获取日期格式字符串
     *  参数
     *      format          :   日期格式
     *  返回：
     *      日期格式字符串
     */
    static string GetTimeFormatString(BeGeneralLib::TimeFormat format);

    /*
     *  功能：
     *      获取时间单位字符串
     *  参数
     *      unit            :   日期单位
     *  返回：
     *      时间字符串
     */
    static string GetTimeUnitString(BeGeneralLib::TimeUnit unit);

    /*
     *  功能：
     *      字符集转换
     *  参数
     *      fromcharset     :   源字符集
     *      tocharset       :   目标字符集
     *      intputstring    :   输入字符串
     *      inputlength     :   输入长度
     *      outbuffer       :   输出缓冲
     *      outbufferlen    :   输出缓存长度
     *  返回：
     *      转换字节数
     */
    static int CharsetConvert(string fromcharset, string tocharset, char* inputstring, size_t inputlength, char *outbuffer, size_t outbufferlen);
    //    static int CharsetConvert(char* inputstring, size_t inputlength, char *outbuffer, size_t outbufferlen);
    //    static int CharsetConvert(string inputstring, char *outbuffer, size_t outbufferlen);

    /*
     *  功能：
     *      将进制转换为字符串
     *  参数
     *      base            :   进制
     *  返回：
     *      字符串表示的进制
     */
    static string Base2String(BeGeneralLib::NumberBase base);

};

#endif	/* _BEGENERALLIB_H */

