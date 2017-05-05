#include "besencryptd.h"
#include "dog_api.h"
#include "dog_vcode.h"

BesEncryptD::BesEncryptD(){}
BesEncryptD::~BesEncryptD(){}

/*
 *  功能：
 *      解密License
 *  参数：
 *      output          :   解密输出
 *      mode            :   授权模式
 */
bool BesEncryptD::Decrypt(string &output, LicenseMode mode)
{
	char c[33] = {0};

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

	switch (mode){
		case BesEncryptD::LICENSE_MODE_HOST:			status = dog_read(handle, 1, 0, 16, &c[0]); break;
		case BesEncryptD::LICENSE_MODE_EMERGENCY:	status = dog_read(handle, 1, 16, 16, &c[0]); break;
		case BesEncryptD::LICENSE_MODE_CDP:			status = dog_read(handle, 1, 32, 16, &c[0]); break;
		case BesEncryptD::LICENSE_MODE_EA:			status = dog_read(handle, 1, 48, 16, &c[0]); break;
		case BesEncryptD::LICENSE_MODE_TIME:			status = dog_read(handle, 1, 64, 32, &c[0]); break;
	}

	switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		printf("handle not active\n"); break;
		case DOG_INV_FILEID:	printf("invalid file id\n"); break;
		case DOG_MEM_RANGE:		printf("exceeds data file range\n"); break;
		case DOG_NOT_FOUND:		printf("key/license container not available\n"); break;
		default:				printf("read failed\n");
	}
    if (status) { dog_logout(handle); return false; }

	switch (mode){
		case BesEncryptD::LICENSE_MODE_TIME:	status = dog_decrypt(handle, c, 32); break;
		default:							status = dog_decrypt(handle, c, 16); break;
	}
	switch (status){
		case DOG_STATUS_OK:	break;
	    case DOG_INV_HND:	printf("handle not active\n"); break;
	    case DOG_TOO_SHORT:	printf("data length too short\n"); break;
	    default:			printf("decryption failed\n");
	}
	dog_logout(handle);
	if (status) return false;

	output = string(c);
	return true;
}

/*
 *  功能：
 *      读取License
 *  参数
 *      type            :   授权类型
 *  返回：
 *      授权值
 */
string BesEncryptD::GetLicense(BesEncryptD::LicenseType type)
{
    string Output = "";
    if (Decrypt(Output, GetLicenseModeWithType(type)) == false) return "";	//会被atoi转为0

    return ParsePlainText(Output, type);
}

/*
 *  功能：
 *      解析明文中的授权值
 *  参数：
 *      input           :   输入
 *      type            :   授权类型
 *  返回：
 *      对应的授权值
 */
string BesEncryptD::ParsePlainText(string input, LicenseType type)
{
    string LicenseValue = "";
    if (input.empty()) return "";
    if (type == LICENSE_TYPE_EXPIRED_DATE || type == LICENSE_TYPE_CREATE_TIME)
    {
        switch (type){
            case LICENSE_TYPE_EXPIRED_DATE:	if (input.size() >= 8) LicenseValue = input.substr(0, 8); break;
            case LICENSE_TYPE_CREATE_TIME:	if (input.size() >= 22) LicenseValue = input.substr(8, 14); break;
        }
    }
    else
    {
        vector<string> LicenseValueList = ParsePlainText(input);
        int ArrayIndex = -1;
        switch (type){
			case LICENSE_TYPE_HOST: // 主机数
	        case LICENSE_TYPE_NET: // 网启数
	        case LICENSE_TYPE_CDP: // cdp个数
	        case LICENSE_TYPE_EA_NUMBER: // 自动应急数
				ArrayIndex = 0;
				break;
	        case LICENSE_TYPE_SNAPSHOT: // 快照数
	        case LICENSE_TYPE_VM: // 虚拟机数
	        case LICENSE_TYPE_CDP_KEEPTIME: // cdp快照保留时间
				ArrayIndex = 1;
				break;
	        case LICENSE_TYPE_DATABASE: // 数据库数
	        case LICENSE_TYPE_FC: // 光纤数
	        case LICENSE_TYPE_CDP_KEEPUNIT: // cdp快照保留时间单位
				ArrayIndex = 2;
				break;
	        default:
				break;
        }
        if (LicenseValueList.size() > ArrayIndex && ArrayIndex != -1)
            LicenseValue = LicenseValueList[ArrayIndex];
    }
    return LicenseValue;
}

/*
 *  功能：
 *      解析明文中的授权值
 *  参数：
 *      input           :   输入
 *  返回：
 *      授权值列表
 */
vector<string> BesEncryptD::ParsePlainText(string input)
{
    vector<string> ValueList;
    if (input.empty()) return ValueList;

    string Value = "";
    int Position = 0;
    while (true){
		//获取值的长度
        if (input.size() < Position + 1) break;		//从Position开始取1字节,即取到Position+1-1,此时size为Position+1
        int Length = StringToInt(input.substr(Position, 1));
        Position++;
		//获取值
        if (input.size() < Position + Length) break;	//从Position开始取Length字节,即取到Position+Length-1,此时size为Position+Length
        Value = input.substr(Position, Length);
        ValueList.push_back(Value);
        Position += Length;
    }
    return ValueList;
}

/*
 *  功能：
 *      根据授权类型获取授权模式
 *  参数：
 *      type            :   授权类型
 *  返回：
 *      授权模式
 */
BesEncryptD::LicenseMode BesEncryptD::GetLicenseModeWithType(LicenseType type)
{
    LicenseMode Mode = BesEncryptD::LICENSE_MODE_HOST;
    switch (type)
    {
        case LICENSE_TYPE_NET: // 网启数
        case LICENSE_TYPE_VM: // 虚拟机数
        case LICENSE_TYPE_FC: // 光纤数
        {
            Mode = BesEncryptD::LICENSE_MODE_EMERGENCY;
            break;
        }
        case LICENSE_TYPE_CDP: // CDP个数
        case LICENSE_TYPE_CDP_KEEPTIME: // CDP快照保留时间
        case LICENSE_TYPE_CDP_KEEPUNIT: // CDP快照保留时间单位
        {
            Mode = BesEncryptD::LICENSE_MODE_CDP;
            break;
        }
        case LICENSE_TYPE_EA_NUMBER: // 自动应急数
        {
            Mode = BesEncryptD::LICENSE_MODE_EA;
            break;
        }
        case LICENSE_TYPE_EXPIRED_DATE: // 许可到期日
        case LICENSE_TYPE_CREATE_TIME: // 许可创建时间
        {
            Mode = BesEncryptD::LICENSE_MODE_TIME;
            break;
        }
    }
    return Mode;
}

/*
 *  功能：
 *      数字转换为字符串
 *  参数：
 *      num             :   数字
 *  返回：
 *      转换后的字符串形式
 */
string BesEncryptD::Number2String(long num)
{
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    bzero(Buf, sizeof (Buf));
    sprintf(Buf, "%ld\0", num);
    return string(Buf);
}

/*
 *  功能：
 *      数字转换为字符串
 *  参数：
 *      num             :   数字
 *      precision       :   精度
 *  返回：
 *      转换后的字符串形式
 */
string BesEncryptD::Number2String(float num, int precision)
{
    std::ostringstream OStringStream;
    if (precision <= 0)
    {
        OStringStream << num;
    }
    else
    {
        OStringStream << setprecision(precision) << num;
    }
    return OStringStream.str();
}

/*
 *  功能：
 *      字符转换为字符串
 *  参数：
 *      c               :   字符
 *  返回：
 *      转换后的字符串
 */
string BesEncryptD::Char2String(char c)
{
    string s = DEFAULT_EMPTY_STRING;
    s.push_back(c);
    return s;
}

/*
 *  功能：
 *      字符串转换为整数
 *  参数：
 *      str             :   字符串
 *  返回：
 *      转换后的整数
 */
int BesEncryptD::StringToInt(string str)
{
    return (atoi(str.c_str()));
}

/*
 *  功能：
 *      字符串转换为浮点数
 *  参数：
 *      str             :   字符串
 *  返回：
 *      转换后的浮点数
 */
float BesEncryptD::StringToFloat(string str)
{
    return (atof(str.c_str()));
}

/*
 *  功能：
 *      字符串转换为bool
 *  参数：
 *      str             :   字符串
 *  返回：
 *      转换后的bool值
 */
bool BesEncryptD::StringToBool(string str)
{
    return (bool)(StringToInt(str));
}

/*
 *  功能：
 *      字符串转为大写
 *  参数
 *      str         :   输入字符串
 *  返回：
 *      命令字符串
 */
string BesEncryptD::StringToUpper(string str)
{
    if (IsStringEmpty(str) == true)
    {
        return DEFAULT_EMPTY_STRING;
    }

    string ResultStr = DEFAULT_EMPTY_STRING;
    const char *StrPointer = str.c_str();
    for (int i = 0; i < str.size(); i++)
    {
        ResultStr += (char) toupper(StrPointer[i]);
    }
    return ResultStr;
}

/*
 *  功能：
 *      检查字段值是否为空或无效值
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BesEncryptD::IsStringEmptyOrInvalidValue(string str)
{
    return (str.empty() == true || str == DEFAULT_INVALID_RETURN_VALUE || str == DEFAULT_INVALID_RETURN_VALUE2);
}

/*
 *  功能：
 *      检查字段值是否为空或无效值
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BesEncryptD::IsStringEmptyOrZero(string str)
{
    return (IsStringEmpty(str) == true || str == "0");
}

/*
 *  功能：
 *      检查字段值是否为空或无效值
 *  参数
 *      str             :   要操作的字符串
 *  返回：
 *      如果是返回true，否则返回false
 */
bool BesEncryptD::IsStringEmpty(string str)
{
    return (str.empty() == true);
}

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
bool BesEncryptD::ExecuteSystem(string shellcommand, bool redirect)
{
    string CommandStr = shellcommand;

    // 重定向
    if (redirect == true) CommandStr = CommandStr + string(" &> /dev/null");

    DebugPrint("ShellCommand:" + CommandStr);

    pid_t Status = system(CommandStr.c_str());

    bool Flag = false;
    if (-1 == Status)
    {
        Flag = false;
    }
    else
    {
        if (WIFEXITED(Status))
        {
            if (0 == WEXITSTATUS(Status))
            {
                Flag = true;
            }
            else
            {
                Flag = false;
            }
        }
        else Flag = false;
    }
    return Flag;
}

/*
 *  功能：
 *      读取shell返回值
 *  参数
 *      shellcommand    :   shell命令
 *  返回：
 *      shell返回值
 */
string BesEncryptD::ReadShellReturnValue(string shellcommand)
{
    return ReadShellReturnValue(shellcommand, true);
}

/*
 *  功能：
 *      读取shell返回值
 *  参数
 *      shellcommand    :   shell命令
 *      tolog           :   输出到日志
 *  返回：
 *      shell返回值
 */
string BesEncryptD::ReadShellReturnValue(string shellcommand, bool tolog)
{
    vector<string> ValueList = ReadShellReturnValueAll(shellcommand, tolog);
    if (ValueList.size() == 0) return "";
    return ValueList[0];
}

/*
 *  功能：
 *      读取shell所有返回值
 *  参数
 *      shellcommand    :   shell命令
 *  返回：
 *      shell返回值
 */
vector<string> BesEncryptD::ReadShellReturnValueAll(string shellcommand)
{
    return ReadShellReturnValueAll(shellcommand, true);
}

/*
 *  功能：
 *      读取shell所有返回值
 *  参数
 *      shellcommand    :   shell命令
 *  返回：
 *      shell返回值
 */
vector<string> BesEncryptD::ReadShellReturnValueAll(string shellcommand, bool tolog)
{
    vector<string> ValueList;
    FILE *fp;
    //    if (tolog == true)
    //    {
    //        BesLog::DebugPrint("ShellCommand:" + shellcommand, BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
    //    }

    if ((fp = popen(shellcommand.c_str(), "r")) == NULL)
    {
        //        if (tolog == true)
        //        {
        //            BesLog::DebugPrint("Open shellcommand failed!\n", BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
        //        }
        return ValueList;
    }
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    while (!feof(fp))
    {
        bzero(Buf, sizeof (Buf));
        fgets(Buf, sizeof (Buf), fp);
        Buf[strlen(Buf) - 1] = DEFAULT_C_STRING_END_FLAG;
        if (IsStringEmpty(string(Buf)) == false)
        {
            ValueList.push_back(string(Buf));
        }
        //        else
        //        {
        //            if (tolog == true)
        //            {
        //                BesLog::DebugPrint("Buffer is empty!\n", BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
        //            }
        //        }
    }
    pclose(fp);

    //    if (tolog == true)
    //    {
    //        BesLog::DebugPrint("ShellValue Size:" + Number2String(ValueList.size()) + string("\n"), BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
    //        for (int i = 0; i < ValueList.size(); i++)
    //        {
    //            BesLog::DebugPrint("ShellValue:" + ValueList[i] + string("\n"), BesLog::LOG_MESSAGE_TYPE_NORMAL, true);
    //        }
    //    }

    return ValueList;
}

/*
 *  功能：
 *      获取执行路径名
 *  参数：
 *      无
 *  返回：
 *      执行路径名
 */
string BesEncryptD::GetExecutePath()
{
    char LinkPath[DEFAULT_BUFFER_SIZE + 1];
    bzero(LinkPath, sizeof (LinkPath));
    string ExecPath = "";
    string FileName = string("/proc/") + Number2String(getpid()) + string("/exe");
    int Res = readlink(FileName.c_str(), LinkPath, sizeof (LinkPath));
    if (Res != -1)
    {
        LinkPath[Res] = DEFAULT_C_STRING_END_FLAG;
        ExecPath = LinkPath;
        ExecPath = ExecPath.substr(0, ExecPath.find_last_of(LINUX_PATH_SEPCHAR) + 1);
    }
    return ExecPath;
}

/*
 *  功能：
 *      十六进制转换为十进制
 *  参数：
 *      str             :   十六进制字符串
 *  返回：
 *      转换后的十进制数
 */
int BesEncryptD::HexToDecimal(string str)
{
    str = StringToUpper(str);
    int Size = str.size();
    int DecValue = 0;
    for (int i = 0; i < Size; i++)
    {
        unsigned char Val = 0;
        char Ch = str.substr(i, 1).c_str()[0];
        switch (Ch)
        {
            case 'F':
            case 'E':
            case 'D':
            case 'C':
            case 'B':
            case 'A':
            {
                Val = Ch - 'A' + 10;
                break;
            }
            default:
            {
                Val = StringToInt(str.substr(i, 1));
                break;
            }
        }
        DecValue = DecValue + (int) Val * (int) pow(16, Size - i - 1);
    }
    return DecValue;
}

/*
 *  功能：
 *      将数组转换为十六进制字符串
 *  参数：
 *      arr             :   数组
 *      len             :   数组长度
 *  返回：
 *      转换后的十六进制字符串
 */
string BesEncryptD::ArrayToHexString(unsigned char *arr, int len)
{
    if (arr == NULL || len <= 0)
    {
        return "";
    }
    //    cout << "len:" << len << endl;
    string HexString = "";
    for (int i = 0; i < len; i++)
    {
        HexString = HexString + ByteToHexString((unsigned char) arr[i], true);
    }
    return HexString;
}

/*
 *  功能：
 *      将字节转换为十六进制字符串
 *  参数：
 *      byte            :   字节
 *      isupper         :   是否大写
 *  返回：
 *      转换后的十六进制字符串
 */
string BesEncryptD::ByteToHexString(unsigned char ch, bool isupper)
{
    unsigned char buf[3];
    bzero(buf, sizeof (buf));
    string format = (isupper == true) ? string("%02X\0") : string("%02x\0");
    sprintf((char*) buf, format.c_str(), ch);
    return string((const char *) buf);
}

/*
 *  功能：
 *      以指定的格式获取当前时间
 *  参数：
 *      format           :   时间格式
 *  返回：
 *      时间字符串
 */
string BesEncryptD::GetTimeString(BesEncryptD::TimeFormat format)
{
    time_t NowTime; time(&NowTime);
    return GetTimeString(NowTime, format);
}

/*
 *  功能：
 *      获取当前时间格式
 *  参数：
 *      time            :   时间
 *      format          :   时间格式
 *  返回：
 *      时间字符串
 */
string BesEncryptD::GetTimeString(time_t time, BesEncryptD::TimeFormat format)
{
    return GetTimeString(localtime(&time), format);
}

/*
 *  功能：
 *      获取当前时间格式
 *  参数：
 *      tmptr           :   时间结构指针
 *      format          :   时间格式
 *  返回：
 *      时间字符串
 */
string BesEncryptD::GetTimeString(struct tm *tmptr, BesEncryptD::TimeFormat format)
{
    char Buf[DEFAULT_BUFFER_SIZE + 1];
    bzero(Buf, sizeof (Buf));

    // 获取本地时间
    struct tm * CurrentTime = tmptr;

    string FormatString = GetTimeFormatString(format);

    // 转换为YYYYMMDDHHMMSS格式
    strftime(Buf, sizeof (Buf) - 1, FormatString.c_str(), CurrentTime);
    string TimeString = string(Buf);
    if (format == BesEncryptD::TIME_FORMAT_COMMAND_HEADER)
    {
        struct timeval NowTime;
        gettimeofday(&NowTime, NULL);
        bzero(Buf, sizeof (Buf));
        sprintf(Buf, "%06ld", NowTime.tv_usec);
        TimeString = TimeString + string(Buf);
    }
    return TimeString;
}

/*
 *  功能：
 *      获取日期格式字符串
 *  参数
 *      format          :   日期格式
 *  返回：
 *      日期格式字符串
 */
string BesEncryptD::GetTimeFormatString(BesEncryptD::TimeFormat format)
{
    string FormatString = "";
    switch (format)
    {
        case BesEncryptD::TIME_FORMAT_LONG:
        {
            FormatString = string("%Y-%m-%d %H:%M:%S");
            break;
        }
        case BesEncryptD::TIME_FORMAT_DATE:
        {
            FormatString = string("%Y-%m-%d");
            break;
        }
        case BesEncryptD::TIME_FORMAT_TIME:
        {
            FormatString = string("%H:%M:%S");
            break;
        }
        case BesEncryptD::TIME_FORMAT_FULL_DATETIME:
        {
            FormatString = string("%Y%m%d%H%M%S");
            break;
        }
        case BesEncryptD::TIME_FORMAT_FULL_DATE:
        {
            FormatString = string("%Y%m%d");
            break;
        }
        case BesEncryptD::TIME_FORMAT_FULL_TIME:
        {
            FormatString = string("%H%M%S");
            break;
        }
        case BesEncryptD::TIME_FORMAT_YEAR:
        {
            FormatString = string("%Y");
            break;
        }
        case BesEncryptD::TIME_FORMAT_MONTH:
        {
            FormatString = string("%m");
            break;
        }
        case BesEncryptD::TIME_FORMAT_DAY:
        {
            FormatString = string("%d");
            break;
        }
        case BesEncryptD::TIME_FORMAT_HOUR:
        {
            FormatString = string("%H");
            break;
        }
        case BesEncryptD::TIME_FORMAT_MINUTE:
        {
            FormatString = string("%M");
            break;
        }
        case BesEncryptD::TIME_FORMAT_SECOND:
        {
            FormatString = string("%S");
            break;
        }
        case BesEncryptD::TIME_FORMAT_COMMAND_HEADER:
        {
            FormatString = string("%d%H%M%S");
            break;
        }
    }
    return FormatString;
}

//打印调试信息
void BesEncryptD::DebugPrint(string msg)
{
    cout << msg << endl;
}

/*
bool BesEncryptD::WriteDogHostCount(int cnt, bool iswrite, string &outlic){
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

	if(iswrite){
		status = dog_write(handle, 2, 0, 16, LicenseString.c_str());
		switch (status){
			case DOG_STATUS_OK:		break;
			case DOG_INV_HND:		printf("handle not active\n"); break;
			case DOG_INV_FILEID:	printf("invalid file id\n"); break;
			case DOG_MEM_RANGE:		printf("exceeds data file range\n"); break;
			case DOG_NOT_FOUND:		printf("key/license container not available\n"); break;
			default:				printf("write failed with status %d\n", status);
	    }
		dog_logout(handle);
		if (status) return false;
	}
	else dog_logout(handle);

	outlic = LicenseString;
	return true;
}*/

//检测加密狗是否连接
bool BesEncryptD::CheckDogExist(){
	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}

	//login failed
	if (status){
		DebugPrint(ErrorInfo);
		return false;
	}

	//login succeed
	dog_logout(handle);
	return true;
}

//返回-1表示出错，>=0表示成功
int BesEncryptD::ReadDogHostCount(){
	char c[17] = {0};

	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;

	//login
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}
	if (status){
		DebugPrint(ErrorInfo);
		return -1;
	}

	//login succeed
	status = dog_read(handle, 2, 0, 16, &c[0]);
	switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
		case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
		case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
		case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
		default:				ErrorInfo = string("read failed");
	}
    if (status){
		DebugPrint(ErrorInfo);
		dog_logout(handle);
		return -1;
	}

	//read succeed
	status = dog_decrypt(handle, c, 16);
	switch (status){
		case DOG_STATUS_OK:	break;
	    case DOG_INV_HND:	ErrorInfo = string("handle not active"); break;
	    case DOG_TOO_SHORT:	ErrorInfo = string("data length too short"); break;
	    default:			ErrorInfo = string("decryption failed");
	}
	dog_logout(handle);
	if (status){
		DebugPrint(ErrorInfo);
		return -1;
	}

	//decrypt succeed
	string output = string(c);
    if (output.empty()) return -1;
	int Length = StringToInt(output.substr(0, 1));
	if (output.size() < 1 + Length) return -1;
	return atoi((output.substr(1, Length)).c_str());
}

bool BesEncryptD::WriteDogHostCount(int cnt){
	if(cnt < 0) return false;
    string HostCount = Number2String(cnt);
    string HostCountSize = Number2String(HostCount.size());
    string Input = HostCountSize + HostCount;

    char c[17] = {0};
	strcpy(c, Input.c_str());

	string LicenseString;

	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;

	//login
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}
	if (status){
		DebugPrint(ErrorInfo);
		return false;
	}

	//login succeed
	status = dog_encrypt(handle, c, 16);
	switch (status){
        case DOG_STATUS_OK:		break;
        case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
        case DOG_TOO_SHORT:		ErrorInfo = string("data length too short"); break;
        case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
        default:				ErrorInfo = string("encryption failed");
	}
	if (status){
		DebugPrint(ErrorInfo);
		dog_logout(handle);
		return false;
	}

	//encrypt succeed
	LicenseString = string(c);
    if (LicenseString.empty()) { dog_logout(handle); return false; }

	status = dog_write(handle, 2, 0, 16, LicenseString.c_str());
	switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
		case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
		case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
		case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
		default:				ErrorInfo = string("write failed");
    }
	dog_logout(handle);
	if (status){
		DebugPrint(ErrorInfo);
		return false;
	}
	return true;
}

//                                  a[7]      a[6]     a[5]     a[4]     a[3]     a[2]     a[1]     a[0]
//l_createtime最大为991231235959，即00000000 00000000 00000000 11100110 11001001 11111100 01010111 01110111
string BesEncryptD::LongToString5(long l_createtime){
	union{
		unsigned long l;		// 64位系统：long占8字节
		char c[8];
	} CreateTime;
	CreateTime.l=l_createtime;
	return string(CreateTime.c);
}

long BesEncryptD::String5ToLong(string s_createtime){
	union{
		unsigned long l;
		char c[8];
	} CreateTime;
	strcpy(CreateTime.c,s_createtime.c_str());
	CreateTime.c[5]=CreateTime.c[6]=CreateTime.c[7]=0;
	return CreateTime.l;
}

//"2016-06-06 16:19:10"(19位)-->"16-06-06 16:19:10"-->"160606161910"(12位)
long BesEncryptD::ConvertDogCreateTime(string s_createtime){
	char from[20];
	char to[13];
	int i=0,j=0;
	strcpy(from, s_createtime.c_str());
	for(i=2; i<=18; i++){
		if((i-4)%3 == 0) i++;
		to[j++]=from[i];
	}
	to[12]=0;
	return atol(to);
}

//取得createtime在明细文件中对应的index，如果不存在则返回-1
int BesEncryptD::GetDogIndex(string s_createtime){
	if(s_createtime.empty()) return -1;
	long l_createtime = ConvertDogCreateTime(s_createtime);
	int count = ReadDogHostCount(); if(count == -1) return -1;
	char c[6] = {0};

	//login
	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}
	if (status){
		DebugPrint(ErrorInfo);
		return -1;
	}

	union{
		unsigned long l;
		char d[8];
	} CreateTime;

	//login succeed
	for(int i=0; i<count; i++){
		//read
		status = dog_read(handle, 3, i*5, 5, &c[0]);
		switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
		case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
		case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
		case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
		default:				ErrorInfo = string("read failed");
		}
		if (status){
			DebugPrint(ErrorInfo);
			break;
		}

		//read succeed
		for(int j = 0; j < 5; j++) CreateTime.d[j]=c[j];
		CreateTime.d[5]=CreateTime.d[6]=CreateTime.d[7]=0;
		if(CreateTime.l == l_createtime){
			//find succeed!
			dog_logout(handle);
			return i;
		}
	}

	//haven't found
	dog_logout(handle);
	return -1;
}

//删除明细文件中index对应的createtime(明细在[index*5,index*5+4]处)
//删除之后不需要修改计数器，由task.cpp负责修改
bool BesEncryptD::DelDogTime(int index){
	if(index<0) return false;
	int count = ReadDogHostCount(); if(count == -1) return false;
	//如果删的不是最后一个，则把最后1个复制到index处，从而实现删除index  即把[(count-1)*5,(count-1)*5+4]复制到[index*5,index*5+4]
	if(index != count-1){
		char c[6] = {0};
		string ErrorInfo = string("");
		dog_status_t status;
		dog_handle_t handle;

		//login
		status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
		switch (status){
		    case DOG_STATUS_OK:			break;
		    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
		    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
		    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
		    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
			default:                    ErrorInfo = string("login failed");
		}
		if (status){
			DebugPrint(ErrorInfo);
			return false;
		}

		//login succeed, begin read
		status = dog_read(handle, 3, (count-1)*5, 5, &c[0]);
		switch (status){
			case DOG_STATUS_OK:		break;
			case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
			case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
			case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
			case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
			default:				ErrorInfo = string("read failed");
		}
		if (status){
			DebugPrint(ErrorInfo);
			dog_logout(handle);
			return false;
		}

		string output = string(c); if(output.empty()) return false;

		//write
		status = dog_write(handle, 3, index*5, 5, &c[0]);
		switch (status){
			case DOG_STATUS_OK:		break;
			case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
			case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
			case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
			case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
			default:				ErrorInfo = string("write failed");
	    }

		dog_logout(handle);
		if (status){
			DebugPrint(ErrorInfo);
			return false;
		}
	}

	return true;
}

//把createtime增加到明细文件中
//增加之后不需要修改计数器，由task.cpp负责修改
bool BesEncryptD::AddDogTime(string s_createtime){
	int count = ReadDogHostCount();
	if(count == -1) return false;

	long l = ConvertDogCreateTime(s_createtime);

	union{
		unsigned long m;				//unsigend __int64
		char c[8];
	} CreateTime;
	CreateTime.m = l;

	//login
	string ErrorInfo = string("");
	dog_status_t status;
	dog_handle_t handle;
	status = dog_login(1, (dog_vendor_code_t *)vendor_code, &handle);
	switch (status){
	    case DOG_STATUS_OK:			break;
	    case DOG_FEATURE_NOT_FOUND:	ErrorInfo = string("dog feature not found"); break;
	    case DOG_NOT_FOUND:         ErrorInfo = string("no SuperDog with vendor code found"); break;
	    case DOG_INV_VCODE:         ErrorInfo = string("invalid vendor code"); break;
	    case DOG_LOCAL_COMM_ERR:	ErrorInfo = string("communication error between API and local SuperDog License Manager"); break;
		default:                    ErrorInfo = string("login failed");
	}
	if (status){
		DebugPrint(ErrorInfo);
		return false;
	}

	//login succeed, begin write
	status = dog_write(handle, 3, count*5, 5, CreateTime.c);	//复制到[count*5,count*5+4]处
	switch (status){
		case DOG_STATUS_OK:		break;
		case DOG_INV_HND:		ErrorInfo = string("handle not active"); break;
		case DOG_INV_FILEID:	ErrorInfo = string("invalid file id"); break;
		case DOG_MEM_RANGE:		ErrorInfo = string("exceeds data file range"); break;
		case DOG_NOT_FOUND:		ErrorInfo = string("key/license container not available"); break;
		default:				ErrorInfo = string("write failed");
    }

	dog_logout(handle);
	if (status){
		DebugPrint(ErrorInfo);
		return false;
	}

	return true;
}

/*
 *  功能：
 *      License文件是否存在
 *  参数
 *      path            :   License文件目录
 */
bool BesEncryptD::IsLicenseExist()
{
//  return (IsFileExist(GetLicenseFile(GetExecutePath())) == true);
	return true;
}

/*
 *  功能：
 *      获取License文件名
 *  参数
 *      path            :   License文件目录
 *  返回：
 *      License文件名
 */
string BesEncryptD::GetLicenseFile(string path)
{
    // 列举License文件
    string ShellCommand = string("ls") + SEPARATOR_CHAR_SPACE + path + SEPARATOR_CHAR_SPACE + string(" | grep ") + DEF_LICENSE_EXTENSION + string("$");

    // 获取License文件列表
    vector<string> LicFileList = ReadShellReturnValueAll(ShellCommand);

    string LicenseFile = "";
    for (int i = 0; i < LicFileList.size(); i++)
    {
        if (LicFileList[i] != DEF_LICENSE_NAME)
        {
            LicenseFile = LicFileList[i];
        }
    }
    if (LicenseFile == "")
    {
        LicenseFile = DEF_LICENSE_NAME;
    }
    LicenseFile = path + LicenseFile;
    return LicenseFile;
}