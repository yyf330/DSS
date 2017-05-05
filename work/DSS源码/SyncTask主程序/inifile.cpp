/* 
 * File:   IniFile.cpp
 * Author: root
 * 
 * Created on 2012年9月17日, 下午6:53
 */

#include "inifile.h"

/*
 *  功能：
 *      构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
IniFile::IniFile()
{
}

/*
 *  功能：
 *      析构函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
IniFile::~IniFile()
{
}

//---------------------------------- 公有方法 ----------------------------------//
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
string IniFile::ReadProfileString(string filename, string section, string key)
{
    return ReadProfileString(filename, section, key, "");
}

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
string IniFile::ReadProfileString(string filename, string section, string key, string defaultvalue)
{
    string KeyValue = "";
    ReadProfileStringInternal(filename, section, key, KeyValue, defaultvalue);
    return KeyValue;
}

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
bool IniFile::WriteProfileString(string filename, string section, string key, string value)
{
    return WriteProfileStringInternal(filename, section, key, value);
}

/*
 *  功能：
 *      检查ini文件段是否存在
 *  参数：
 *      filename        :   文件名
 *      section         :   段名
 *  返回：
 *      如果存在返回true，否则返回false
 */
bool IniFile::IsSectionExist(string filename, string section)
{
    char buf[MAX_INI_FILE_SIZE] = {0};
    int SectionStartPosition = -1;
    int SectionEndPosition = -1;
    int KeyStartPosition = -1;
    int KeyEndPosition = -1;
    int ValueStartPosition = -1;
    int ValueEndPosition = -1;
    int FileSize = 0;

    //check parameters
    //    assert(section != NULL && strlen(section));
    //    assert(key != NULL && strlen(key));
    //    assert(value != NULL);
    //    assert(file != NULL && strlen(key));

    if (!LoadIniFile(filename, buf, &FileSize))
    {
        SectionStartPosition = -1;
    }
    else
    {
        ParseFile(section, "", buf, &SectionStartPosition, &SectionEndPosition, &KeyStartPosition, &KeyEndPosition, &ValueStartPosition, &ValueEndPosition);
    }

    return (SectionStartPosition != -1);
}

//---------------------------------- 私有方法 ----------------------------------//
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
bool IniFile::LoadIniFile(const string filename, char *buf, int *file_size)
{
    FILE *fp = NULL;
    int i = 0;
    *file_size = 0;

    //assert(file != NULL);
    assert(buf != NULL);

    fp = fopen(filename.c_str(), "r");
    if (NULL == fp)
    {
        return false;
    }

    buf[i] = fgetc(fp);

    //load initialization file
    while (buf[i] != (char) EOF)
    {
        i++;
        assert(i < MAX_INI_FILE_SIZE); //file too big, you can redefine MAX_INI_FILE_SIZE to fit the big file
        buf[i] = fgetc(fp);
    }

    buf[i] = '\0';
    *file_size = i;

    fclose(fp);
    return true;
}

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
bool IniFile::ParseFile(const string section, const string key, const char *buf, int *sec_s, int *sec_e, int *key_s, int *key_e, int *value_s, int *value_e)
{
    const char *p = buf;
    int i = 0;
    char TempBuf [DEFAULT_BUFFER_SIZE];

    assert(buf != NULL);
    *sec_e = *sec_s = *key_e = *key_s = *value_s = *value_e = -1;

    while (!IsEndOfString(p[i]))
    {
        // 查找段
        if ((0 == i || IsNewLine(p[i - 1])) && IsLeftBrace(p[i]))
        {
            int SectionStartPosition = i + 1;

            //find the ']'
            do
            {
                i++;
            }
            while (!IsRightBrace(p[i]) && !IsEndOfString(p[i]));

            bzero(TempBuf, sizeof (TempBuf));
            strncpy(TempBuf, p + SectionStartPosition, i - SectionStartPosition);
            TempBuf[i - SectionStartPosition] = '\0';
            if (strcmp(section.c_str(), TempBuf) == 0)
            {
                //                cout << "i=" << i << endl;
                int newline_start = 0;

                i++;

                //Skip over space char after ']'
                while (isspace(p[i]))
                {
                    i++;
                }

                //find the section
                *sec_s = SectionStartPosition;
                // *sec_e = i;

                int RealSectionEnd = i;
                do
                {
                    RealSectionEnd++;
                }
                while (!IsLeftBrace(p[RealSectionEnd]) && !IsEndOfString(p[RealSectionEnd]));
                *sec_e = RealSectionEnd;

                while (!(IsNewLine(p[i - 1]) && IsLeftBrace(p[i]))
                        && !IsEndOfString(p[i]))
                {
                    int j = 0;
                    //get a new line
                    newline_start = i;

                    while (!IsNewLine(p[i]) && !IsEndOfString(p[i]))
                    {
                        i++;
                    }

                    //now i  is equal to end of the line
                    j = newline_start;

                    if ('#' != p[j]) //skip over comment
                    {
                        while (j < i && p[j] != '=')
                        {
                            j++;
                            if ('=' == p[j])
                            {
                                bzero(TempBuf, sizeof (TempBuf));
                                strncpy(TempBuf, p + newline_start, j - newline_start);
                                TempBuf[j - newline_start] = '\0';
                                //if (strncmp(key.c_str(), p + newline_start, j - newline_start) == 0)
                                if (strcmp(key.c_str(), TempBuf) == 0)
                                {
                                    //find the key ok
                                    *key_s = newline_start;
                                    *key_e = j - 1;

                                    *value_s = j + 1;
                                    *value_e = i;

                                    return true;
                                }
                            }
                        }
                    }

                    i++;
                }
            }
        }
        else
        {

            i++;
        }
    }
    return false;
}

/*
 *  功能：
 *      是否为新行
 *  参数：
 *      ch              :   字符
 *  返回：
 *      如果是返回true，否则返回false
 */
bool IniFile::IsNewLine(char ch)
{
    return ('\n' == ch || '\r' == ch) ? 1 : 0;
}

/*
 *  功能：
 *      是否为字符串结束
 *  参数：
 *      ch              :   字符
 *  返回：
 *      如果是返回true，否则返回false
 */
bool IniFile::IsEndOfString(char ch)
{
    return '\0' == ch ? 1 : 0;
}

/*
 *  功能：
 *      是否为左方括号
 *  参数：
 *      ch              :   字符
 *  返回：
 *      如果是返回true，否则返回false
 */
bool IniFile::IsLeftBrace(char ch)
{
    return '[' == ch ? 1 : 0;
}

/*
 *  功能：
 *      是否为右方括号
 *  参数：
 *      ch              :   字符
 *  返回：
 *      如果是返回true，否则返回false
 */
bool IniFile::IsRightBrace(char ch)
{
    return ']' == ch ? 1 : 0;
}

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
bool IniFile::ReadProfileStringInternal(const string filename, const string section, const string key, string &value, const string defaultvalue)
{
    char buf[MAX_INI_FILE_SIZE] = {0};
    int FileSize = 0;
    int SectionStartPosition = -1;
    int SectionEndPosition = -1;
    int KeyStartPosition = -1;
    int KeyEndPosition = -1;
    int ValueStartPosition = -1;
    int ValueEndPosition = -1;
    char KeyValueBuffer[DEFAULT_BUFFER_SIZE];
    if (LoadIniFile(filename, buf, &FileSize) == false)
    {
        value = defaultvalue;
        return false;
    }

    if (!ParseFile(section, key, buf, &SectionStartPosition, &SectionEndPosition, &KeyStartPosition, &KeyEndPosition, &ValueStartPosition, &ValueEndPosition))
    {
        value = defaultvalue;
        return false; //not find the key
    }

    //    cout << "SectiontStart:" << SectionStartPosition << endl;
    //    cout << "SectionEndPosition:" << SectionEndPosition << endl;
    //    cout << "KeyStartPosition:" << KeyStartPosition << endl;
    //    cout << "KeyEndPosition:" << KeyEndPosition << endl;
    //    cout << "ValueStartPosition:" << ValueStartPosition << endl;
    //    cout << "ValueEndPosition:" << ValueEndPosition << endl;

    int Length = ValueEndPosition - ValueStartPosition;
    int size = sizeof (KeyValueBuffer);
    if (size - 1 < Length)
    {
        Length = size - 1;
    }

    memset(KeyValueBuffer, 0, size);
    memcpy(KeyValueBuffer, buf + ValueStartPosition, Length);
    KeyValueBuffer[Length] = '\0';
    value = KeyValueBuffer;

    return true;
}

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
bool IniFile::WriteProfileStringInternal(const string filename, const string section, const string key, const string value)
{
    char buf[MAX_INI_FILE_SIZE] = {0};
    char WriteBuffer[MAX_INI_FILE_SIZE] = {0};
    int SectionStartPosition = -1;
    int SectionEndPosition = -1;
    int KeyStartPosition = -1;
    int KeyEndPosition = -1;
    int ValueStartPosition = -1;
    int ValueEndPosition = -1;
    int ValueLength = value.size();
    int FileSize = 0;

    //check parameters
    //    assert(section != NULL && strlen(section));
    //    assert(key != NULL && strlen(key));
    //    assert(value != NULL);
    //    assert(file != NULL && strlen(key));

    if (!LoadIniFile(filename, buf, &FileSize))
    {
        SectionStartPosition = -1;
    }
    else
    {
        ParseFile(section, key, buf, &SectionStartPosition, &SectionEndPosition, &KeyStartPosition, &KeyEndPosition, &ValueStartPosition, &ValueEndPosition);
    }

    if (SectionStartPosition == -1)
    {
        if (FileSize > 0)
        {
            //not find the section, then add the new section at end of the file
            memcpy(WriteBuffer, buf, FileSize);
            if ((FileSize == 1 && IsNewLine(buf[0]) == false) || IsNewLine(buf[FileSize - 1]) == false)
            {
                sprintf(WriteBuffer + FileSize, "\r\n");
                FileSize = FileSize + 2;
            }
        }
        sprintf(WriteBuffer + FileSize, "[%s]\r\n%s=%s\r\n", section.c_str(), key.c_str(), value.c_str());
    }
    else if (KeyStartPosition == -1)
    {
        //not find the key, then add the new key=value at end of the section
        //        cout << "SectionEndPosition:" << SectionEndPosition << endl;
        int Index = SectionEndPosition;
        //        cout << "Buf[index]=" << (int) buf[Index] << endl;
        while ((IsLeftBrace(buf[Index]) == true && IsNewLine(buf[Index - 1]) == true)
                || (IsEndOfString(buf[Index]) == true && IsNewLine(buf[Index - 1]) == true))
        {
            Index--;
        }
        if (IsEndOfString(buf[Index]) == false)
        {
            Index++;
        }
        //        cout << "Index:" << Index << endl;
        memcpy(WriteBuffer, buf, Index);
        sprintf(WriteBuffer + Index, "%s=%s\r\n", key.c_str(), value.c_str());
        sprintf(WriteBuffer + Index + key.size() + ValueLength + 3, buf + Index, FileSize - Index);
    }
    else
    {
        //update value with new value
        memcpy(WriteBuffer, buf, ValueStartPosition);
        memcpy(WriteBuffer + ValueStartPosition, value.c_str(), ValueLength);
        memcpy(WriteBuffer + ValueStartPosition + ValueLength, buf + ValueEndPosition, FileSize - ValueEndPosition);
    }

    FILE * fp = fopen(filename.c_str(), "w");
    if (NULL == fp)
    {
        return 0;
    }

    if (fputs(WriteBuffer, fp) == -1)
    {
        fclose(fp);
        return false;
    }

    fclose(fp);
    return true;
}

