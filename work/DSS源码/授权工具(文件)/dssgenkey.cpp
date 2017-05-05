#include "gencrypt.h"
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "besencrypt.h"
using namespace std;

//int OffsetF [] = {0x4567, 0x5674, 0x6745, 0x7456,
//    0xcdef, 0xdefc, 0xefcd, 0xfcde,
//    0x10123, 0x11230, 0x12301, 0x3012,
//    0x189ab, 0x19ab8, 0x1ab89, 0x1b89a};

string Input(string prompt, string defaultvalue, int length, bool withendline = false)
{
    string s = "";
    cin.clear();
    cin.sync();

    cout << endl << prompt;
    if (withendline)
    {
        cout << endl;
    }
    getline(cin, s);
    if (!cin) // 当超长时出错处理
    {
        cin.clear();
        while (cin.get() != '\n') continue;	// 清除掉剩余的字符串，以免影响下一个输入
    }

    if (s.length() == 0) s = defaultvalue;

    if (length != 0 && s.length() >= length) s = s.substr(0, length);

	return s;
}

string Input(string prompt)
{
    return Input(prompt, "", 0);
}

void disp(void * pbuf, int size)
{
    int i = 0;
    for (i = 0; i < size; i++) printf("%02x ", *((unsigned char *) pbuf + i));

    putchar('\n');
}

string StringToLower(string str)
{
    if (str.empty()) return "";

    string ResultStr = "";
    const char *StrPointer = str.c_str();
    for (int i = 0; i < str.size(); i++)
    {
        ResultStr += (char) tolower(StrPointer[i]);
    }
    return ResultStr;
}

int main(int argc, char** argv)
{
    while (true)
    {
		string License = "";
        string FileName = Input("请输入文件名:");

		string InputValue = Input("是否使用本机磁盘参数(y/n)--[n]?", "n", 1);
		string DiskSn = BesEncrypt::DiskSerialNo();
		string DiskSize = BesEncrypt::DiskSize();
		if (StringToLower(InputValue) == "n"){
			DiskSn = Input("请输入磁盘序列号：");
			DiskSize = Input("请输入磁盘大小[14488576]：", "14488576", 0);
		}

        string HostNum = "";

//		HostNum = Input("请输入主机数：\t");
		HostNum = Input("请输入策略个数：\t");
//		string SnapshotNum = Input("请输入快照数[64]：", "64", 0);
		string SnapshotNum = Input("请输入源库个数：\t");
//		string DbNum = Input("请输入数据库数[" + HostNum + "]:", HostNum, 0);
		string DbNum = "0";
		License = BesEncrypt::Encrypt(atoi(HostNum.c_str()), atoi(SnapshotNum.c_str()), atoi(DbNum.c_str()), FileName, DiskSn, DiskSize, BesEncrypt::LICENSE_MODE_HOST);
		cout << endl << "许可证号:\t" << License << endl;

		string ExpiredDate = "00000000";
		if (StringToLower(Input("是否创建时间许可(y/n)--[n]?", "n", 1)) == "y") ExpiredDate = Input("请输入许可到期日：\t");

		License = BesEncrypt::Encrypt(ExpiredDate, FileName, DiskSn, DiskSize);
        cout << endl << "许可证号:\t" << License << endl;
		if (StringToLower(Input("是否继续(y/n)--[n]?", "n", 1)) == "n") exit(0);
	}
    return (EXIT_SUCCESS);
}