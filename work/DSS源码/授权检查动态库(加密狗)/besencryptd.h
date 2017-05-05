#ifndef _BESENCRYPTD_H
#define	_BESENCRYPTD_H

#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
#include <math.h>
#include "besgeneralheader.h"

class BesEncryptD
{
public:

    BesEncryptD();
    virtual ~BesEncryptD();
public:

    // 授权类型
    enum LicenseType
    {
        LICENSE_TYPE_HOST, // 主机数
        LICENSE_TYPE_SNAPSHOT, // 快照数
        LICENSE_TYPE_DATABASE, // 数据库
        LICENSE_TYPE_NET, // 网启数
        LICENSE_TYPE_VM, // 虚拟机数
        LICENSE_TYPE_FC, // 光纤卡数
        LICENSE_TYPE_CDP, // CDP个数
        LICENSE_TYPE_CDP_KEEPTIME, // CDP连续快照时间
        LICENSE_TYPE_CDP_KEEPUNIT, // CDP连续快照时间单位
        LICENSE_TYPE_EA_NUMBER, // 自动应急个数
        LICENSE_TYPE_EXPIRED_DATE, // 许可到期日
        LICENSE_TYPE_CREATE_TIME, // 许可创建时间
    };

    // 授权模式
    enum LicenseMode
    {
        LICENSE_MODE_HOST, // 基本
        LICENSE_MODE_EMERGENCY, // 应急
        LICENSE_MODE_CDP, // CDP
        LICENSE_MODE_EA, // 自动应急
        LICENSE_MODE_TIME, // 时间
    };

    // 解密类型
    enum BesDecryptType
    {
        BES_DECRYPT_TYPE_LICENSE,
        BES_DECRYPT_TYPE_FILE,
    };

public:

    /*
     *  功能：
     *      解密License
     *  参数：
     *      instr           :   输入
     *      type            :   输入类型
     *      outlic          :   license号
     *      output          :   解密输出
     *      mode            :   授权模式
     *  返回：
     *      解密成功返回true，否则返回false
     */
    static bool Decrypt(string &output, LicenseMode mode);

    /*
     *  功能：
     *      读取授权值
     *  参数
     *      type            :   授权类型
     *  返回：
     *      授权值
     */
    static string GetLicense(BesEncryptD::LicenseType type);
	static bool AddDogTime(string s_createtime);
	static bool DelDogTime(int index);
	static int GetDogIndex(string s_createtime);
	static long ConvertDogCreateTime(string s_createtime);
	static long String5ToLong(string s_createtime);
	static string LongToString5(long l_createtime);
	static bool WriteDogHostCount(int cnt);
	static int ReadDogHostCount();
	static bool CheckDogExist();

	static bool IsLicenseExist();

    /*
     *  功能：
     *      获取License文件名
     *  参数
     *      path            :   License文件目录
     *  返回：
     *      License文件名
     */
    static string GetLicenseFile(string path);

public:
#define DEF_LICENSE_EXTENSION   ".lic"          // 默认License文件扩展名
#define DEF_LICENSE_NAME        "bes.lic"       // 默认License文件名

private:
#define DEF_BUFFER_SIZE         32              // 默认缓冲大小
#define DEF_ENCRYPT_BYTE_SIZE   16              // 默认加密字节长度
#define DEF_ENCRYPT_BMP         "bes.bmp"       // 默认加密位图文件名
private:

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
private:
    
    /*
     *  功能：
     *      解析明文中的授权值
     *  参数：
     *      input           :   输入
     *      type            :   授权类型
     *  返回：
     *      对应的授权值
     */
    static string ParsePlainText(string input, LicenseType type);

    /*
     *  功能：
     *      解析明文中的授权值
     *  参数：
     *      input           :   输入
     *  返回：
     *      授权值列表
     */
    static vector<string> ParsePlainText(string input);

    /*
     *  功能：
     *      根据授权类型获取授权模式
     *  参数：
     *      type            :   授权类型
     *  返回：
     *      授权模式
     */
    static BesEncryptD::LicenseMode GetLicenseModeWithType(LicenseType type);

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
     *      执行系统命令
     *  参数
     *      shellcommand    :   执行命令串
     *      redirect        :   重定向
     *      msgtype         :   信息类型
     *  返回：
     *      成功返回true，否则返回false
     */
    static bool ExecuteSystem(string shellcommand, bool redirect = true);

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
     *      获取当前时间格式
     *  参数：
     *      format          :   时间格式
     *  返回：
     *      时间字符串
     */
    static string GetTimeString(BesEncryptD::TimeFormat format);

    /*
     *  功能：
     *      获取当前时间格式
     *  参数：
     *      time            :   时间
     *      format          :   时间格式
     *  返回：
     *      时间字符串
     */
    static string GetTimeString(time_t time, BesEncryptD::TimeFormat format);

    /*
     *  功能：
     *      获取当前时间格式
     *  参数：
     *      tmptr           :   时间结构指针
     *      format          :   时间格式
     *  返回：
     *      时间字符串
     */
    static string GetTimeString(struct tm *tmptr, BesEncryptD::TimeFormat format);

    /*
     *  功能：
     *      获取日期格式字符串
     *  参数
     *      format          :   日期格式
     *  返回：
     *      日期格式字符串
     */
    static string GetTimeFormatString(BesEncryptD::TimeFormat format);

    /*
     *  功能：
     *      打印调试信息
     *  参数：
     *      msg             :   文件名
     *  返回：
     *      无
     */
    static void DebugPrint(string msg);

};

#endif	/* _BESENCRYPTD_H */
