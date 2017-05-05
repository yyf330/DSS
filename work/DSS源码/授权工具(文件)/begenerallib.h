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
 * 
//================================== 修改结束 ==================================*/

#ifndef _BEGENERALLIB_H
#define	_BEGENERALLIB_H

#include <math.h>
#include <signal.h>
#include <ctype.h>
#include <locale>
#include <unistd.h>
#include <sys/time.h>
#include <bits/stl_algo.h>
#include "besgeneralheader.h"
//#include "beslog.h"
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
        TIME_FORMAT_LONG,
        TIME_FORMAT_DATE,
        TIME_FORMAT_TIME,
        TIME_FORMAT_FULL_DATETIME,
        TIME_FORMAT_FULL_DATE,
        TIME_FORMAT_FULL_TIME,
        TIME_FORMAT_COMMAND_HEADER,
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
public:

    /*
     *  功能：
     *      数字转换为字符串
     *  参数：
     *      num             :   数字
     *  返回：
     *      转换后的字符串形式
     */
    static string Number2String(long num);



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
     *      读取shell返回值
     *  参数
     *      shellcommand    :   shell命令
     *  返回：
     *      shell返回值
     */
    static string ReadShellReturnValue(string shellcommand);

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
     *      redirect        :   重定向
     *      debug           :   调试标志
     *  返回：
     *      无
     */
    static bool ExecuteSystem(string shellcommand, bool redirect = true, bool debug = true);

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
     *  返回：
     *      无
     */
    static void ForceRemoveDirectory(string path);

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
     *      检查字符串是否为有效的IP
     *  参数：
     *      ipstr           :   ip字符串
     *  返回：
     *      如果是则返回true，否则返回false
     */
    static bool IsValidIp(string ipstr);

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
     *      获取空闲的loop设备
     *  参数：
     *      无
     *  返回：
     *      返回空闲的loop设备
     */
    static string GetFreeLoopDevice();

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
     *     获取指定文件大小
     *  参数：
     *      filename        :   文件名
     *  返回：
     *      文件大小
     */
    static long int GetFileSize(string filename);

private:
#define DEFAULT_UPPER_START_CHAR            'A'
#define DEFAULT_LOWER_START_CHAR            'a'
#define DEFAULT_NUMBER_START_CHAR           '0'
#define DEFAULT_ALPHACHAR_COUNT             26
#define DEFAULT_ALPHANUM_COUNT              10
#define MAX_EXEC_PARAMETER_NUMBER           16
#define DEFAULT_IP_SEGMENT_NUM              4
#define MIN_IP_SEGMENT_VALUE                0
#define MAX_IP_SEGMENT_VALUE                255
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


    //    /*
    //     *  功能：
    //     *      打印调试信息
    //     *  参数：
    //     *      msg             :   信息
    //     *      tolog           :   输出到日志
    //     *  返回：
    //     *      无
    //     */
    static void DebugPrint(string msg, bool iserror = false);
};

#endif	/* _BEGENERALLIB_H */

