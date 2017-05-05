/* 
 * File:   IniFile.h
 * Author: root
 *
 * Created on 2012年9月17日, 下午6:53
 */

#ifndef INIFILE_H
#define	INIFILE_H
#include <cassert>
#include <fstream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include "begenerallib.h"

class IniFile
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
    IniFile();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~IniFile();
public:
    /*
     *  功能：
     *      读取ini文件键值
     *  参数：
     *      filename        :   文件名
     *      section         :   段名
     *      key             :   键名
     *  返回：
     *      如果成功返回键值，否则返回空
     */
    static string ReadProfileString(string filename, string section, string key);

    /*
     *  功能：
     *      读取ini文件键值
     *  参数：
     *      filename        :   文件名
     *      section         :   段名
     *      key             :   键名
     *      defaultvalue    :   如果读取不到键值，默认返回值
     *  返回：
     *      如果成功返回键值，否则返回默认值
     */
    static string ReadProfileString(string filename, string section, string key, string defaultvalue);

    /*
     *  功能：
     *      写入ini文件键值
     *  参数：
     *      filename        :   文件名
     *      section         :   段名
     *      key             :   键名
     *      value           :   键值
     *  返回：
     *      如果成功返回true，否则返回false
     */
    static bool WriteProfileString(string filename, string section, string key, string value);

    /*
     *  功能：
     *      检查ini文件段是否存在
     *  参数：
     *      filename        :   文件名
     *      section         :   段名
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool IsSectionExist(string filename, string section);
private:
#define MAX_INI_FILE_SIZE 1024*16                                   // 最大ini文件大小(16K)

private:
    /*
     *  功能：
     *      加载ini文件
     *  参数：
     *      filename        :   文件名
     *      buf             :   加载成功后的缓冲
     *      filesize        :   文件大小
     *  返回：
     *      如果存在返回true，否则返回false
     */
    static bool LoadIniFile(const string filename, char *buf, int *filesize);

    /*
     *  功能：
     *      解析文件
     *  参数：
     *      section         :   段名
     *      key             :   键名
     *      buf             :   文件数据缓冲
     *      sectionstart    :   段起始字节数
     *      sectionend      :   段结束字节数
     *      keystart        :   键起始字节数
     *      keyend          :   键结束字节数
     *      valuestart      :   键值起始字节数
     *      valueend        :   键值结束字节数
     *  返回：
     *      如果解析成功返回true，否则返回false
     */
    static bool ParseFile(const string section, const string key, const char *buf, int *sectionstart, int *sectionend, int *keystart, int *keyend, int *valuestart, int *valueend);

    /*
     *  功能：
     *      是否为新行
     *  参数：
     *      ch              :   字符
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsNewLine(char ch);

    /*
     *  功能：
     *      是否为字符串结束
     *  参数：
     *      ch              :   字符
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsEndOfString(char ch);

    /*
     *  功能：
     *      是否为左方括号
     *  参数：
     *      ch              :   字符
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsLeftBrace(char ch);

    /*
     *  功能：
     *      是否为右方括号
     *  参数：
     *      ch              :   字符
     *  返回：
     *      如果是返回true，否则返回false
     */
    static bool IsRightBrace(char ch);

    /*
     *  功能：
     *      读取ini文件键值（内部方法）
     *  参数：
     *      filename        :   文件名
     *      section         :   段名
     *      key             :   键名
     *      value           :   键值
     *      defaultvalue    :   默认返回值
     *  返回：
     *      如果成功返回true，否则返回false
     */
    static bool ReadProfileStringInternal(const string filename, const string section, const string key, string &value, const string default_value);

    /*
     *  功能：
     *     写入ini文件键值（内部方法）
     *  参数：
     *      filename        :   文件名
     *      section         :   段名
     *      key             :   键名
     *      value           :   键值
     *  返回：
     *      如果成功返回true，否则返回false
     */
    static bool WriteProfileStringInternal(const string filename, const string section, const string key, const string value);
};

#endif	/* INIFILE_H */

