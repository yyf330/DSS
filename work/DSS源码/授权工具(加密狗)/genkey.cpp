#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <time.h>
#include "dog_api.h"
using namespace std;

#define DEFAULT_BUFFER_SIZE 1024

unsigned char vendor_code[] =
    "7RA3p7pQzSlv6AzEx9HIcZIb/LJ83DYE1s5T1AjaNc12KkrJdM4sM1dU5+i7FshxVAhgIgSAJv5OMatS6DaVeMIkA9zBn8t7xHSPeSo+e3F2U27pSCXX0cZFLl6ZO4FmOXRyahvl1Vn8qKJEaN9/PNspaBmMD0awwSYFY8a/isvW3LXx6D7NuU4EtIUFGDa9mx3wq8mbVZNEjBlR4Lw+gT9cHsAMBQI5/qpUx/g63oyTtkTqm4K+4f2D989CLiWnkDnXWQhVONDtzD0LyFAz13/mbXt2WrhRhFYSv+eshefBcTTSP+F8k6H+vinmxRT7DAQE3Jv9fpP8OkHf5q7m+jCVGENdn2f23F0qvxlOfWmp/X7AP1wSmik1S2hxrRr+6qw2ZP26Y+kRP/tnCFE6ZPxXed6aFcbhoipwqvPO7veyZQU6axC3PxF4mDae6EYqWbwZoXciH/zZi9hu+tm6EHeqAdmujsyWtQyj7Zt/rzuA1kfjvSEDDrLP7xgLFA6X77qFTrBNDI8S+V/v7mjKwLQ49HdSjiU30lo+65Sc5HhBSTJkiJnI5AiXWHusO3M/NnuEdU+vlbOOKOmYu76OUonNbOaBBJlKKWl3/3llAMCxrpFuA3KaaP/xHTlxG1zCeTfQmjhRmwMr6itwelJsh1WdvdO6/B8EjXScYUd7dbjQwOU0TGVWzNSw8gkskA37LvzF/k038526I1EZK5nqDiId3/x0/5HgwPDnKfAfUMkPO1QNvO5LbfjW6ZKIM+7/HsWIrDU812Bc/9w9SCMSJWtxDJST1F/FZPBZV8+vR98dn2uBkcgBIjhNwkrl5NCGVDUAXY3boenyOLA+oGpfkd+h0tYMO0wfZ8XsmGjigD2xNPkX2zzt1DiKincZfpYFMR4SBvV8mNcVFKWoXXGfx3jxQXDchgniNcqinMHjL3ncCZb1Aa2Fku2fvS1DRlp2lpPTjn7lTCRKscuNQM/k6A==";

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
enum LicenseMode{
	LICENSE_MODE_HOST, // 基本
	LICENSE_MODE_EMERGENCY, // 应急
	LICENSE_MODE_CDP, // CDP
	LICENSE_MODE_EA, // 自动应急
	LICENSE_MODE_TIME, // 时间
};

//根据授权类型获取授权模式
LicenseMode GetLicenseModeWithType(LicenseType type)
{
    LicenseMode Mode = LICENSE_MODE_HOST;
    switch (type)
    {
        case LICENSE_TYPE_NET: // 网启数
        case LICENSE_TYPE_VM: // 虚拟机数
        case LICENSE_TYPE_FC: // 光纤数
        {
            Mode = LICENSE_MODE_EMERGENCY;
            break;
        }
        case LICENSE_TYPE_CDP: // CDP个数
        case LICENSE_TYPE_CDP_KEEPTIME: // CDP快照保留时间
        case LICENSE_TYPE_CDP_KEEPUNIT: // CDP快照保留时间单位
        {
            Mode = LICENSE_MODE_CDP;
            break;
        }
        case LICENSE_TYPE_EA_NUMBER: // 自动应急数
        {
            Mode = LICENSE_MODE_EA;
            break;
        }
        case LICENSE_TYPE_EXPIRED_DATE: // 许可到期日
        case LICENSE_TYPE_CREATE_TIME: // 许可创建时间
        {
            Mode = LICENSE_MODE_TIME;
            break;
        }
    }
    return Mode;
}

//数字转换为字符串
string Number2String(long num)
{
    char Buf[DEFAULT_BUFFER_SIZE + 1];
	memset(Buf, 0, sizeof(Buf));
    sprintf(Buf, "%ld\0", num);
    return string(Buf);
}

/*
 *  功能：
 *      以指定的格式获取当前时间
 *  参数：
 *      format           :   时间格式
 */
string GetTimeString()
{
	char Buf[DEFAULT_BUFFER_SIZE + 1]; memset(Buf, 0, sizeof(Buf));
	time_t NowTime; time(&NowTime);
    // 获取本地时间
    struct tm * CurrentTime = localtime(&NowTime);
    strftime(Buf, sizeof (Buf) - 1, "%Y%m%d%H%M%S", CurrentTime);
//	cout<<"debug1:len"<<strlen(Buf)<<" "<<Buf<<endl;	//14
    return string(Buf);
}

//加密并写入加密狗
bool Encrypt(string input, LicenseMode mode, bool iswrite, string &outlic)
{
	char c[33] = {0}; memset(c, 0, sizeof(c));
	strcpy(c, input.c_str());
//	cout<<"debug3:len:"<<strlen(c)<<" "<<c<<endl;	//22

	string LicenseString;

	dog_status_t status;
	dog_handle_t handle;
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);	//登陆到特征1
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	printf("dog feature not found\n"); break;
	    case DOG_NOT_FOUND:         printf("no SuperDog with vendor code found\n"); break;
	    case DOG_INV_VCODE:         printf("invalid vendor code\n"); break;
	    case DOG_LOCAL_COMM_ERR:	printf("communication error between API and local SuperDog License Manager\n"); break;
		default:                    printf("login to feature failed with status %d\n", status);
	}
	if (status) return false;

	switch (mode){
		case LICENSE_MODE_TIME:		status = dog_encrypt(handle, c, 32); break;
		default:					status = dog_encrypt(handle, c, 16); break;
	}
	switch (status){
        case DOG_STATUS_OK:		break;
        case DOG_INV_HND:		printf("handle not active\n"); break;
        case DOG_TOO_SHORT:		printf("data length too short\n"); break;
        case DOG_NOT_FOUND:		printf("key/license container not available\n"); break;
        default:				printf("encryption failed\n");
	}
	if (status) { dog_logout(handle); return false; }

	LicenseString = string(c);
//	cout<<"debug4:len:"<<LicenseString.size()<<" "<<LicenseString<<endl;
    if (LicenseString.empty()) { dog_logout(handle); return false; }

	dog_logout(handle);

	outlic = LicenseString;
    return true;
}

/*
 *  功能：
 *      生成License
 *  参数：
 *      expireddate     :   到期日
 *  返回：
 *      License
 */
bool Encrypt(string expireddate, bool iswrite, string &outlic)
{
    string Input = expireddate + GetTimeString();
//	cout<<"debug2:len:"<<Input.size()<<" "<<Input<<endl;	//22
    return Encrypt(Input, LICENSE_MODE_TIME, iswrite, outlic);
}

/*
 *  功能：
 *      生成License
 *  参数：
 *      host            :   主机数
 *      snapshot        :   快照数
 *      db              :   数据库数
 *      mode            :   授权模式
 *  返回：
 *      License
 */
bool Encrypt(int host, int snapshot, int db, LicenseMode mode, bool iswrite, string &outlic)
{
    // 主机数
    string HostNum = Number2String(host);
    string HostNumSize = Number2String(HostNum.size());

    // 快照数
    string SnapshotNum = Number2String(snapshot);
    string SnapshotNumSize = Number2String(SnapshotNum.size());

    // 数据库数
    string DbNum = Number2String(db);
    string DbNumSize = Number2String(DbNum.size());

    // 输入值
    string Input = HostNumSize + HostNum + SnapshotNumSize + SnapshotNum + DbNumSize + DbNum;
    return Encrypt(Input, mode, iswrite, outlic);
}

bool WriteDogHostCount(int cnt, bool iswrite, string &outlic){
	if(cnt < 0) return false;
    string HostCount = Number2String(cnt);
    string HostCountSize = Number2String(HostCount.size());
    string Input = HostCountSize + HostCount;

    char c[17] = {0};
	strcpy(c, Input.c_str());

	string LicenseString;

	dog_status_t status;
	dog_handle_t handle;
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
    case DOG_STATUS_OK:			break;
    case DOG_FEATURE_NOT_FOUND:	printf("dog feature not found\n"); break;
    case DOG_NOT_FOUND:         printf("no SuperDog with vendor code found\n"); break;
    case DOG_INV_VCODE:         printf("invalid vendor code\n"); break;
    case DOG_LOCAL_COMM_ERR:	printf("communication error between API and local SuperDog License Manager\n"); break;
	default:                    printf("login failed with status %d\n", status);
	}
	if (status) return false;

	status = dog_encrypt(handle, c, 16);
	switch (status){
        case DOG_STATUS_OK:		break;
        case DOG_INV_HND:		printf("handle not active\n"); break;
        case DOG_TOO_SHORT:		printf("data length too short\n"); break;
        case DOG_NOT_FOUND:		printf("key/license container not available\n"); break;
        default:				printf("encryption failed\n");
	}
	if (status) { dog_logout(handle); return false; }

	LicenseString = string(c);
    if (LicenseString.empty()) { dog_logout(handle); return false; }

	dog_logout(handle);

	outlic = LicenseString;
	return true;
}

//defaultvalue:如果输入为空,采用的默认值  length:最终使用的值的长度
string Input(string prompt, string defaultvalue, unsigned int length, bool withendline = false)
{
    string s = "";
    cin.clear();
    cin.sync();

    cout << endl << prompt;
    if (withendline) cout << endl;

    getline(cin, s);
    if (!cin) // 当超长时出错处理
    {
        cin.clear();
        while (cin.get() != '\n') continue; // 清除掉剩余的字符串，以免影响下一个输入
    }

    if (s.length() == 0) s = defaultvalue;
    if (length != 0 && s.length() >= length) s = s.substr(0, length);
    return s;
}

string Input(string prompt)
{
    return Input(prompt, "", 0);
}

string StringToLower(string str)
{
    if (str.empty()) return "";

    string ResultStr = "";
    const char *StrPointer = str.c_str();
    for (unsigned int i = 0; i < str.size(); i++)
		ResultStr += (char) tolower(StrPointer[i]);
    return ResultStr;
}

int main(int argc, char** argv)
{
	while (true)
    {
        cout << endl << "============================ 菜单 ================================" << endl << endl;
		cout << "                   1:  [生成授权文件]" << endl;
		cout << "                   2:  [生成计数器文件]" << endl;
        cout << "                   q:  [退出]" << endl;
        cout << endl << "============================ 结束 ================================" << endl << endl;
        string Option = Input("请选择菜单项:\t", "0", 1);
        char ch;
        if (Option.length() > 0) ch = Option[0];
        switch (ch)
        {
			case '1':
            {
				string TmpString1="", TmpString2="", TmpString3="", TmpString4="", TmpString5="", LicenseString="";

				string HostNum = "";
//              HostNum = Input("请输入主机数[0]：\t");
				HostNum = Input("请输入策略个数[0]：\t");
//              string SnapshotNum = Input("请输入快照数[64]：", "64", 0);
				string SnapshotNum = Input("请输入源库个数：\t");
//              string DbNum = Input("请输入数据库数[" + HostNum + "]:", HostNum, 0);
				string DbNum = "0";
                if(Encrypt(atoi(HostNum.c_str()), atoi(SnapshotNum.c_str()), atoi(DbNum.c_str()), LICENSE_MODE_HOST, false, TmpString1) == false) exit(-1);

                string ExpiredDate = Input("请输入许可到期日[00000000]：\t", "00000000", 8);	//如果指定0会有问题
				if(Encrypt(ExpiredDate, false, TmpString3) == false) exit(-1);
//				cout<<"debug5:len:"<<TmpString3.size()<<endl;

//				LicenseString = TmpString1 + TmpString2 + TmpString4 + TmpString5 + TmpString3;
				LicenseString = TmpString1 + TmpString1 + TmpString1 + TmpString1 + TmpString3;
//				cout<<"debug6:len:"<<LicenseString.size()<<endl;
//				cout<<"debug7:len:"<<strlen(LicenseString.c_str())<<endl;

//				cout<<"Debug:Len:"<<strlen(TmpString1.c_str())<<" "<<strlen(TmpString2.c_str())<<" "<<strlen(TmpString4.c_str())<<" "<<strlen(TmpString5.c_str())<<" "<<strlen(TmpString3.c_str())<<endl;

				FILE *fp = fopen("basic.lic", "wb+"); if(fp == NULL) exit(-1);		//windows中需要加b，否则会把0a转成0d0a，从而多写入1字节
				fwrite(LicenseString.c_str(), strlen(LicenseString.c_str()), 1, fp);
				fflush(fp);

                if (StringToLower(Input("是否继续(y/n)--[n]?", "n", 1)) == "n") exit(0);
                break;
            }
			case '2':
            {
				string LicenseString;
				string HostCnt = Input("请输入主机计数器[0]：\t", "0", 0);
				if(WriteDogHostCount(atoi(HostCnt.c_str()), false, LicenseString) == false) exit(-1);
				FILE *fp = fopen("counter.lic", "wb+"); if(fp == NULL) exit(-1);
				fwrite(LicenseString.c_str(), strlen(LicenseString.c_str()), 1, fp);
				fflush(fp);

				if (StringToLower(Input("是否继续(y/n)--[n]?", "n", 1)) == "n") exit(0);
                break;
			}
            case 'q':
                exit(0);
                break;
            default:
                break;
        }
    }
    return (EXIT_SUCCESS);
}