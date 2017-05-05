/* 
 * File:   networkmanager.cpp
 * Author: root
 * 
 * Created on 2010年5月17日, 上午1:13
 */

#include "networkmanager.h"

//------------------------------------ 公有方法 --------------------------------//

/*
 *  功能：
 *      构造函数
 *  参数：
 *      无
 *  返回：
 *      无
 */
NetworkManager::NetworkManager()
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
NetworkManager::~NetworkManager()
{
}

/*
 *  功能：
 *      操作DHCP
 *  参数：
 *      action          :   动作
 *      dbdatamanager   :   数据库管理器
 *  返回：
 *      操作成功返回true，否则返回false
 */
bool NetworkManager::OperateDHCP(DHCPAction action, DbDataManager * dbdatamanager)
{
    switch (action)
    {
        case DHCP_ACTION_START:
        {
            BeGeneralLib::OperateService(BeGeneralLib::SERVICE_ACTION_START, DEFAULT_SERVICE_NAME_DHCP);
            break;
        }
        case DHCP_ACTION_STOP:
        {
            BeGeneralLib::OperateService(BeGeneralLib::SERVICE_ACTION_STOP, DEFAULT_SERVICE_NAME_DHCP);
            break;
        }
        case DHCP_ACTION_RESTART:
        {
            BeGeneralLib::OperateService(BeGeneralLib::SERVICE_ACTION_RESTART, DEFAULT_SERVICE_NAME_DHCP);
            break;
        }
        case DHCP_ACTION_INIT:
        {
            if (ResetDHCPConfigureFile(dbdatamanager))
            {
                // 重启DHCP服务
                BeGeneralLib::OperateService(BeGeneralLib::SERVICE_ACTION_RESTART, DEFAULT_SERVICE_NAME_DHCP);
            }
            else
            {
                return false;
            }
            break;
        }
        case DHCP_ACTION_CHECK:
        {

            break;
        }
    }
    return true;
}

/*
 *  功能：
 *      写网络配置文件
 *  参数：
 *      sqlmanager  :   数据库管理器
 *      nicname     :   网卡名
 *  返回：
 *      获取的IP地址
 */
bool NetworkManager::WriteNetworkConfig(DbDataManager * dbdatamanager, string nicname)
{
    if (nicname == "virbr0")
    {
        return UpdateVirtualNetworkConfig(dbdatamanager, nicname);
    }
    string TempFilePath = DEFAULT_TEMP_PATH;
    string DestFilePath = "";
    string FileName = string(DEFAULT_NETWORK_CONFIG_PREFIX) + nicname;

    TempFilePath = TempFilePath + FileName;
    FILE *fp = fopen(TempFilePath.c_str(), "w+");
    if (fp == NULL)
    {
        BesLog::DebugPrint("Open File Failed!");
        return false;
    }

    // 获取网络参数
    map<string, string> FieldValueList = dbdatamanager->GetNetworkParameter(nicname);
    if (FieldValueList.empty())
    {
        BesLog::DebugPrint("No network parameter!");
        return false;
    }

    string Netmask = FieldValueList["NETMASK"];
    string IpAddr = FieldValueList["IPADDR"];
    string MacAddr = FieldValueList["MACADDR"];
    string GateWay = FieldValueList["GATEWAY"];
    string RouteSubnet = FieldValueList["ROUTESUBNET"];
    string RouteMask = FieldValueList["ROUTENETMASK"];
    string NicType = FieldValueList["NICTYPE"];

    // 设备名
    string DataStr = string("DEVICE=") + nicname + string("\n");
    BeGeneralLib::WriteDataToFile(fp, DataStr);

    // 随系统启动
    BeGeneralLib::WriteDataToFile(fp, "ONBOOT=yes\n");

    // 网络类型
    BeGeneralLib::WriteDataToFile(fp, "TYPE=Ethernet\n");

    // 用户控制
    BeGeneralLib::WriteDataToFile(fp, "USERCTL=no\n");

    // IPv6初始化
    BeGeneralLib::WriteDataToFile(fp, "IPV6INIT=no\n");

    // 对端DNS
    BeGeneralLib::WriteDataToFile(fp, "PEERDNS=yes\n");

    string BridgeName = string(DEFAULT_BRIDGE_NAME_PREFIX) + nicname;
    if (BeGeneralLib::StringToInt(NicType) == 1)
    {
        DataStr = string("BRIDGE=") + BridgeName + string("\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);
        CreateBridge(BridgeName, IpAddr, Netmask);
    }
    else
    {
        // 删除桥接
        DeleteBridge(BridgeName);

        // 子网掩码
        DataStr = string("NETMASK=") + Netmask + string("\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);

        // IP地址
        DataStr = string("IPADDR=") + IpAddr + string("\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);
    }

    fclose(fp);

    // 检查子网
    CheckDhcpSubnetConfig(dbdatamanager, IpAddr, Netmask);

    // 从临时文件生成最终文件
    DestFilePath = NETWORK_SCRIPTS_PATH + FileName;
    BeGeneralLib::CopyFile(TempFilePath, DestFilePath, true, false);

    // 从临时文件生成最终文件
    DestFilePath = NETWORK_DEVICE_PATH + FileName;
    BeGeneralLib::CopyFile(TempFilePath, DestFilePath, true, false);

    // 从临时文件生成最终文件
    DestFilePath = NETWORK_PROFILE_PATH + FileName;
    BeGeneralLib::CopyFile(TempFilePath, DestFilePath, true, false);

    // 激活网卡
    BeGeneralLib::ExecuteSystem("ifdown " + nicname);
    BeGeneralLib::ExecuteSystem("ifup " + nicname);
    return true;
}

/*
 *  功能：
 *      操作路由
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *  返回：
 *      操作成功返回true，否则返回false
 */
bool NetworkManager::OperateRoutes(DbDataManager* dbdatamanager)
{
    // 获取路由信息
    sql::ResultSet * RouteRes = dbdatamanager->GetRouteResultSet();

    // 如果没有路由，则退出
    if (RouteRes == NULL || (RouteRes->rowsCount() == 0))
    {
        // 删除已存在的路由
        if (RouteRes != NULL && RouteRes->rowsCount() == 0)
        {
            // 设置配置文件
            string ConfigFile = string(DEFAULT_STATIC_ROUTES_PATH) + DEFAULT_STATIC_ROUTES_FILE_NAME;

            // 获取配置文件信息
            string ShellCommand = string("grep ^any") + SEPARATOR_CHAR_SPACE + ConfigFile + SEPARATOR_CHAR_SPACE + string(" | sed -e 's/any[ ]*/-/g'");
            vector<string> RouteList = BeGeneralLib::ReadShellReturnValueAll(ShellCommand);

            // 删除路由信息
            if (RouteList.size() != 0)
            {
                for (int i = 0; i < RouteList.size(); i++)
                {
                    ShellCommand = string("route del") + SEPARATOR_CHAR_SPACE + RouteList[i];
                    BeGeneralLib::ExecuteSystem(ShellCommand);
                }
            }

            // 清空路由配置文件信息
            ShellCommand = string(">") + ConfigFile;
            BeGeneralLib::ExecuteSystem(ShellCommand);
        }

        // 销毁结果集
        dbdatamanager->DestroyResultSet(RouteRes);
        return true;
    }

    // 写入静态路由配置信息
    string TempFileName = string(DEFAULT_TEMP_PATH) + DEFAULT_STATIC_ROUTES_FILE_NAME;
    FILE *fp = fopen(TempFileName.c_str(), "w+");
    if (fp == NULL)
    {
        return false;
    }

    // 遍历路由
    while (RouteRes->next())
    {
        string RouteId = RouteRes->getString("ROUTEID");
        string Subnet = RouteRes->getString("SUBNET");
        string Mask = RouteRes ->getString("MASK");
        string Gateway = RouteRes ->getString("GATEWAY");
        string NicDevice = RouteRes->getString("NICDEVICE");
        int IsAllowed = RouteRes ->getInt("ISALLOWED");

        // 检查子网、掩码、网关是否合法
        if (BeGeneralLib::IsValidIp(Subnet) == false // 无效的子网
                || BeGeneralLib::IsValidIp(Mask) == false // 无效的子网掩码
                || BeGeneralLib::IsValidIp(Gateway) == false) // 无效的网关
        {
            continue;
        }

        int Counter = 0;
        string ShellCommand = string("route -e | grep ^") + Subnet + string("| gawk '{print $3}'");
        while (true)
        {
            string OldMask = BeGeneralLib::ReadShellReturnValue(ShellCommand);
            if (BeGeneralLib::IsStringEmpty(OldMask) == true || Counter > MAX_SUBNET_ROUTE_COUNTER)
            {
                break;
            }
            ShellCommand = string("route del -net") + SEPARATOR_CHAR_SPACE + Subnet + SEPARATOR_CHAR_SPACE + string("netmask") + SEPARATOR_CHAR_SPACE + OldMask + SEPARATOR_CHAR_SPACE + string("gw") + SEPARATOR_CHAR_SPACE + Gateway;
            BeGeneralLib::ExecuteSystem(ShellCommand);
            Counter++;
        }

        // 执行添加路由命令
        ShellCommand = string("route add -net") + SEPARATOR_CHAR_SPACE + Subnet + SEPARATOR_CHAR_SPACE + string("netmask") + SEPARATOR_CHAR_SPACE + Mask + SEPARATOR_CHAR_SPACE + string("gw") + SEPARATOR_CHAR_SPACE + Gateway;
        BeGeneralLib::ExecuteSystem(ShellCommand);

        // 写入静态路由配置信息
        if (fp != NULL)
        {
            string DataStr = string("any net") + SEPARATOR_CHAR_SPACE + Subnet + SEPARATOR_CHAR_SPACE + string("netmask") + SEPARATOR_CHAR_SPACE + Mask + SEPARATOR_CHAR_SPACE + string("gw") + SEPARATOR_CHAR_SPACE + Gateway + string("\n");
            BeGeneralLib::WriteDataToFile(fp, DataStr);
        }
    }
    if (fp != NULL)
    {
        fclose(fp);
    }

    // 销毁结果集
    dbdatamanager->DestroyResultSet(RouteRes);

    // 从临时文件生成最终文件
    string DestFilePath = string(DEFAULT_STATIC_ROUTES_PATH) + DEFAULT_STATIC_ROUTES_FILE_NAME;
    BeGeneralLib::CopyFile(TempFileName, DestFilePath, true);

    return true;
}

//------------------------------------ 私有方法 --------------------------------//

/*
 *  功能：
 *      重新设置配置文件
 *  参数：
 *      无
 *  返回：
 *      重置成功返回true，否则返回false
 */
bool NetworkManager::ResetDHCPConfigureFile(DbDataManager *dbdatamanager)
{
    // 获取子网结果集
    sql::ResultSet * SubnetRs = dbdatamanager->GetSubnetResultSet();

    // 如果没有获取到，或者记录为0，则退出
    if (SubnetRs == NULL || SubnetRs->rowsCount() == 0)
    {
        // 销毁结果集
        dbdatamanager->DestroyResultSet(SubnetRs);
        return true;
    }

    // 获取网卡结果集
    sql::ResultSet * NicRs = dbdatamanager->GetNetworkResultSet();
    if (NicRs == NULL || NicRs->rowsCount() == 0)
    {
        // 销毁结果集
        dbdatamanager->DestroyResultSet(NicRs);
        return false;
    }


    // 生成配置文件
    FILE *fp = fopen(TEMP_CONFIG_FILE_NAME, "w+");
    if (fp == NULL)
    {
        BesLog::DebugPrint("Open file failed!");
        return false;
    }

    // 添加dhcp选项
    BeGeneralLib::WriteDataToFile(fp, "option space bes;\n");
    BeGeneralLib::WriteDataToFile(fp, "option bes-encap-opts code 175 = encapsulate bes;\n");
    BeGeneralLib::WriteDataToFile(fp, "option bes.no-pxedhcp code 176 = unsigned integer 8;\n");
    BeGeneralLib::WriteDataToFile(fp, "option bes.bus-id code 177 = string;\n");
    BeGeneralLib::WriteDataToFile(fp, "option bes.bios-drive code 189 = unsigned integer 8;\n");
    BeGeneralLib::WriteDataToFile(fp, "allow booting;\n");
    BeGeneralLib::WriteDataToFile(fp, "allow bootp;\n");
    BeGeneralLib::WriteDataToFile(fp, "ddns-update-style none;\n");
    BeGeneralLib::WriteDataToFile(fp, "ignore client-updates; \n");
    BeGeneralLib::WriteDataToFile(fp, "deny unknown-clients;\n");

    // 写入空白行
    BeGeneralLib::WriteDataToFile(fp, "\n");

    // emboot文件名
    string EmbootFileName = DEFAULT_EMBOOT_CONFIG_FILE_NAME;

    // 建立新的文档
    xmlDocPtr Doc = xmlNewDoc(BAD_CAST "1.0");

    // 设置根节点
    xmlNodePtr RootNode = xmlNewNode(NULL, BAD_CAST "iscsi_boot");
    xmlDocSetRootElement(Doc, RootNode);

    // 新建portals节点，增加到根节点
    xmlNodePtr PortalsNode = xmlNewNode(NULL, BAD_CAST "portals");
    xmlAddChild(RootNode, PortalsNode);

    // 新建clients节点，增加到根节点
    xmlNodePtr ClientsNode = xmlNewNode(NULL, BAD_CAST "clients");
    xmlAddChild(RootNode, ClientsNode);

    // 遍历子网信息
    while (SubnetRs->next())
    {
        // 子网id
        string NetId = SubnetRs->getString("NETID");

        // 子网编码
        string NCode = SubnetRs->getString("NCODE");

        // 子网信息
        string Subnet = SubnetRs->getString("SUBNET");
        string Mask = SubnetRs->getString("MASK");
        string RangeLow = SubnetRs->getString("RANGELOW");
        string RangeHigh = SubnetRs->getString("RANGEHIGH");
        //        string ServerIp = SubnetRes->getString("SERVERIP");

        // 检查dhcp范围
        if ((inet_network(Subnet.c_str()) != (inet_network(RangeLow.c_str()) & inet_network(Mask.c_str())))
                || (inet_network(Subnet.c_str()) != (inet_network(RangeHigh.c_str()) & inet_network(Mask.c_str()))))
        {
            continue;
        }

        string DataStr = string("subnet") + SEPARATOR_CHAR_SPACE + Subnet + SEPARATOR_CHAR_SPACE + string("netmask") + SEPARATOR_CHAR_SPACE + Mask + string("{\n\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);

        // 设置动态地址范围
        DataStr = string("\t range dynamic-bootp \t") + RangeLow + SEPARATOR_CHAR_SPACE + RangeHigh + string(";\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);

        // 默认租赁时间
        DataStr = string("\t default-lease-time \t") + SubnetRs->getString("DEFLEASETIME") + string(";\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);

        // 最大租赁时间
        DataStr = string("\t max-lease-time \t") + SubnetRs->getString("MAXLEASETIME") + string(";\n");
        BeGeneralLib::WriteDataToFile(fp, DataStr);

        // 设置网关
        string Router = SubnetRs->getString("ROUTER");
        if (BeGeneralLib::IsValidIp(Router) == true)
        {
            DataStr = string("\t option routers") + SEPARATOR_CHAR_SPACE + Router + string(";\n");
            BeGeneralLib::WriteDataToFile(fp, DataStr);
        }

        // 设置DNS
        string MainDnsServer = SubnetRs->getString("MAINDNS");
        string SubDnsServer = SubnetRs->getString("SUBDNS");
        string DnsOption = "";
        if (BeGeneralLib::IsValidIp(MainDnsServer) == true)
        {
            DnsOption = MainDnsServer;
            if (BeGeneralLib::IsValidIp(SubDnsServer) == true)
            {
                DnsOption = DnsOption + string(",") + SubDnsServer;
            }
        }
        if (BeGeneralLib::IsStringEmpty(DnsOption) == false)
        {
            DataStr = string("\t option domain-name-servers") + SEPARATOR_CHAR_SPACE + DnsOption + string(";\n");
            BeGeneralLib::WriteDataToFile(fp, DataStr);
        }

        // 检查网卡配置
        NicRs->beforeFirst();
        while (NicRs->next())
        {
            string NicName = NicRs->getString("NICNAME");
            string NicIpAddr = NicRs->getString("IPADDR");
            string NicMask = NicRs->getString("NETMASK");

            // 如果DHCP子网掩码和网卡掩码相同，并且目标子网也相同
            if (NicMask != Mask || (inet_network(Subnet.c_str()) != (inet_network(NicIpAddr.c_str()) & inet_network(Mask.c_str()))))
            {
                continue;
            }

            string ServerIp = NicIpAddr;

            // emboot配置
            // 新建portal节点,增加到portals节点
            xmlNodePtr PortalNode = xmlNewNode(NULL, BAD_CAST "portal");
            xmlAddChild(PortalsNode, PortalNode);

            // 直接在portal上新建元素
            string SanName = DEFAULT_SAN_NAME_PREFIX + NetId;
            xmlNewTextChild(PortalNode, NULL, BAD_CAST "name", BAD_CAST SanName.c_str());
            xmlNewTextChild(PortalNode, NULL, BAD_CAST "description", BAD_CAST DEFAULT_PORTAL_NAME);
            xmlNewTextChild(PortalNode, NULL, BAD_CAST "address", BAD_CAST ServerIp.c_str());
            xmlNewTextChild(PortalNode, NULL, BAD_CAST "port", BAD_CAST "3260");

            // 新建targets节点，增加到portal节点
            xmlNodePtr TargetsNode = xmlNewNode(NULL, BAD_CAST "targets");
            xmlAddChild(PortalNode, TargetsNode);

            sql::ResultSet * HostRs = dbdatamanager->GetHostResultSet(NicName);
            if (HostRs != NULL && HostRs->rowsCount() > 0)
            {
                while (HostRs->next())
                {
                    string HCode = HostRs->getString("HCODE");
                    string HostIP = HostRs->getString("IPADDR");
                    string HostName = HostRs->getString("HOSTNAME");
                    string MacAddr = HostRs->getString("MACADDR");
                    string NetbootProtocol = HostRs->getString("NETBOOTPROTOCOL");
                    string UsedForLE = HostRs->getString("USEDFORLE");

                    // 如果为光纤卡协议或者非本地应急则进行下一个
                    if ((UsedForLE == "0") || NetbootProtocol == DEFAULT_BOOT_PROTOCOL_FCBOOT)
                    {
                        continue;
                    }

                    if (BeGeneralLib::IsStringEmptyOrInvalidValue(HostName) == true
                            || BeGeneralLib::IsValidMacAddress(MacAddr) == false)
                    {
                        continue;
                    }

                    // 如果网启应急和虚拟机应急功能关闭，则继续下一主机
                    if (dbdatamanager->IsHostNetEmergencyOpen(HCode) == false && dbdatamanager->IsHostVmEmergencyOpen(HCode) == false)
                    {
                        continue;
                    }

                    // 如果为网启协议，并且网启应急功能关闭，则继续下一主机
                    if ((NetbootProtocol == DEFAULT_BOOT_PROTOCOL_HBA || NetbootProtocol == DEFAULT_BOOT_PROTOCOL_PXE || NetbootProtocol == DEFAULT_BOOT_PROTOCOL_SNSBOOT)
                            && (dbdatamanager->IsHostNetEmergencyOpen(HCode) == false))
                    {
                        continue;
                    }

                    // 如果为虚拟机协议，并且虚拟机应急功能关闭，则继续下一主机
                    if ((NetbootProtocol == DEFAULT_BOOT_PROTOCOL_VMBOOT && dbdatamanager->IsHostVmEmergencyOpen(HCode) == false))
                    {
                        continue;
                    }

                    // 如果为自动应急，则继续下一主机
                    if (dbdatamanager->IsHostEAOpen(HCode) == true)
                    {
                        continue;
                    }

                    // 主机名
                    DataStr = string("\n\t host") + SEPARATOR_CHAR_SPACE + HCode + SEPARATOR_CHAR_SPACE + string("{\n");
                    BeGeneralLib::WriteDataToFile(fp, DataStr);

                    // MAC地址
                    DataStr = string("\t\t hardware ethernet") + SEPARATOR_CHAR_SPACE + MacAddr + string(";\n");
                    BeGeneralLib::WriteDataToFile(fp, DataStr);

                    // 固定的IP地址
                    DataStr = string("\t\t fixed-address") + SEPARATOR_CHAR_SPACE + HostIP + string(";\n");
                    BeGeneralLib::WriteDataToFile(fp, DataStr);

                    // 如果为虚拟机协议
                    if (NetbootProtocol == DEFAULT_BOOT_PROTOCOL_VMBOOT)
                    {
                        // 插入反花括号
                        BeGeneralLib::WriteDataToFile(fp, string("\t}\n"));
                    }
                    else // 如果为网启协议
                    {
                        // tftp根目录
                        DataStr = string("\t\t option root-path \"") + ServerIp + string(":") + TFTP_BOOT_PATH + string("\";\n");
                        BeGeneralLib::WriteDataToFile(fp, DataStr);

                        // next server
                        DataStr = string("\t\t next-server") + SEPARATOR_CHAR_SPACE + ServerIp + string(";\n\n");
                        BeGeneralLib::WriteDataToFile(fp, DataStr);

                        if (NetbootProtocol == DEFAULT_BOOT_PROTOCOL_SNSBOOT)
                        {
                            DataStr = string("\t\t filename \"bes/iscsinbp.0\";\n");
                            BeGeneralLib::WriteDataToFile(fp, DataStr);
                        }
                        else
                        {
                            DataStr = string("\t\t option bes.no-pxedhcp 1;\n\n");
                            BeGeneralLib::WriteDataToFile(fp, DataStr);

                            DataStr = string("\t\t if not exists bes.bus-id {\n")
                                    + string("\t\t\t filename \"bes/undionly.kpxe\";\n")
                                    + string("\t\t }else{\n")
                                    + string("\t\t\t filename \"bes/") + HCode + string(".gpxe\";\n")
                                    + string("\t\t }\n\n");
                            BeGeneralLib::WriteDataToFile(fp, DataStr);
                        }

                        // 插入反花括号
                        BeGeneralLib::WriteDataToFile(fp, string("\t}\n"));


                        // 根据主机信息生成配置文件
                        string BootProtocol = dbdatamanager->GetHostBootProtocol(HCode);
                        string CopyId = dbdatamanager->GetSystemEmergencyCopyId(HCode);
                        if (dbdatamanager->IsCopyExist(HCode, CopyId) == false)
                        {
                            //                            BesLog::DebugPrint("The system emergency copy of host <" + HCode + "> does not exist!", BesLog::LOG_MESSAGE_TYPE_NORMAL, false);
                            continue;
                        }
                        string Iqn = dbdatamanager->GetCopyIqn(HCode, CopyId);

                        // 获取initiator名称
                        string InitiatorName = dbdatamanager->GetHostInitiatorName(HCode);
                        if (BeGeneralLib::IsStringEmptyOrInvalidValue(InitiatorName) == true)
                        {
                            InitiatorName = HostIP;
                        }

                        // 如果为PXE协议，则创建pxe配置文件
                        if (BootProtocol == DEFAULT_BOOT_PROTOCOL_PXE)
                        {
                            CreateOnePxeFile(ServerIp, HCode, Iqn, InitiatorName);
                        }
                        else if (BootProtocol == DEFAULT_BOOT_PROTOCOL_SNSBOOT) // 如果为emboot，则写入emboot配置文件
                        {
                            // 设置target名字
                            string TargetName = string("target") + SEPARATOR_CHAR_UNDERLINE + HCode + SEPARATOR_CHAR_UNDERLINE + CopyId;

                            // 去除MAC地址中的冒号
                            while (MacAddr.find_first_of(SEPARATOR_CHAR_COLON, 0) != -1)
                            {
                                MacAddr.replace(MacAddr.find(SEPARATOR_CHAR_COLON), 1, "");
                            }

                            // 设置initiator名称
                            //string InitiatorName = DEFAULT_INITIATOR_NAME_PREFIX + MacAddr;
                            //                            string InitiatorName = HostIP;

                            // 新建target节点，增加到targets节点
                            xmlNodePtr TargetNode = xmlNewNode(NULL, BAD_CAST "target");
                            xmlAddChild(TargetsNode, TargetNode);

                            // 直接在target上新建元素
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "name", BAD_CAST TargetName.c_str());
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "description", NULL);
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "target_name", BAD_CAST Iqn.c_str());
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "initiator_name", BAD_CAST InitiatorName.c_str());
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "CHAP", BAD_CAST "No");
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "username", NULL);
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "password", NULL);
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "share_mode", NULL);
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "ram_cache_size", BAD_CAST "128");
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "suppress_hdd_cache_prompt", BAD_CAST "No");
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "discard_hdd_cache", BAD_CAST "No");
                            xmlNewTextChild(TargetNode, NULL, BAD_CAST "commit_hdd_cache", BAD_CAST "No");

                            // 新建client节点，增加到clients节点
                            xmlNodePtr ClientNode = xmlNewNode(NULL, BAD_CAST "client");
                            xmlAddChild(ClientsNode, ClientNode);

                            // 直接在client上新建元素
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "name", BAD_CAST HCode.c_str());
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "MAC", BAD_CAST MacAddr.c_str());
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "description", NULL);

                            // 新建targets节点，增加到client节点
                            xmlNodePtr ClientTargetsNode = xmlNewNode(NULL, BAD_CAST "targets");
                            xmlAddChild(ClientNode, ClientTargetsNode);

                            // 新建pair节点，增加到targets节点
                            xmlNodePtr PairNode = xmlNewNode(NULL, BAD_CAST "pair");
                            xmlAddChild(ClientTargetsNode, PairNode);

                            // 直接在pair上新建元素
                            xmlNewTextChild(PairNode, NULL, BAD_CAST "portal", BAD_CAST SanName.c_str());
                            xmlNewTextChild(PairNode, NULL, BAD_CAST "target", BAD_CAST TargetName.c_str());

                            // 直接在client上新建元素
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "undi_safe_mode", BAD_CAST "No");
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "win2008_gateway_fix", BAD_CAST "No");
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "disabled", BAD_CAST "No");
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "group", NULL);
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "admin", NULL);
                            xmlNewTextChild(ClientNode, NULL, BAD_CAST "boot_menu", BAD_CAST "No");
                        }
                    }
                }
            }

            // 销毁结果集
            dbdatamanager->DestroyResultSet(HostRs);
        }

        BeGeneralLib::WriteDataToFile(fp, "}\n");
    }
    fclose(fp);

    // 销毁结果集
    dbdatamanager->DestroyResultSet(NicRs);

    // 销毁子网结果集
    dbdatamanager->DestroyResultSet(SubnetRs);

    // 保存格式的xml文档
    xmlSaveFormatFile(EmbootFileName.c_str(), Doc, XML_SAVE_FORMAT);

    // 释放文档内节点动态申请的内存
    xmlFreeDoc(Doc);

    // 备份原有配置文件
    BeGeneralLib::CopyFile(DHCP_CONFIG_FILE_NAME, DHCP_CONFIG_BACKUP_NAME, true, false);

    // 替换新的配置文件
    BeGeneralLib::CopyFile(TEMP_CONFIG_FILE_NAME, DHCP_CONFIG_FILE_NAME, true, false);

    // 重启xinetd服务
    BeGeneralLib::OperateService(BeGeneralLib::SERVICE_ACTION_RESTART, DEFAULT_SERVICE_NAME_XINETD);

    // 重启emnbid服务
    BeGeneralLib::OperateService(BeGeneralLib::SERVICE_ACTION_RESTART, DEFAULT_SERVICE_NAME_EMBOOT);

    return true;
}

/*
 *  功能：
 *      获取所有的IP地址
 *  参数：
 *      无
 *  返回：
 *      所有的IP地址的矢量集合
 */
vector<NetworkManager::IpAddr> NetworkManager::GetAllIpAddress()
{
    vector<IpAddr> IpList;
    string ShellCommand = string("ls -l ") + SEPARATOR_CHAR_SPACE + NETWORK_SCRIPTS_PATH + SEPARATOR_CHAR_SPACE + string(" |  grep 'ifcfg-eth' | gawk '{print $8}'");
    FILE *fp;
    if ((fp = popen(ShellCommand.c_str(), "r")) == NULL)
    {
        return IpList;
    }
    while (!feof(fp))
    {
        char Buf[DEFAULT_BUFFER_SIZE + 1];
        bzero(Buf, sizeof (Buf));
        fgets(Buf, sizeof (Buf), fp);
        if (strcmp(Buf, "") != 0)
        {
            Buf[strlen(Buf) - 1] = '\0';
            string FileName = string(NETWORK_SCRIPTS_PATH) + Buf;
            if (BeGeneralLib::IsFileExist(FileName) == true)
            {
                IpAddr TempAddr;
                TempAddr.NicName = Buf;
                TempAddr.Device = BeGeneralLib::GetValueFromFile(FileName, "DEVICE", "=");
                TempAddr.Address = BeGeneralLib::GetValueFromFile(FileName, "IPADDR", "=");
                TempAddr.Mac = BeGeneralLib::GetValueFromFile(FileName, "HWADDR", "=");
                TempAddr.Mask = BeGeneralLib::GetValueFromFile(FileName, "NETMASK", "=");
                TempAddr.Gateway = BeGeneralLib::GetValueFromFile(FileName, "GATEWAY", "=");
                IpList.push_back(TempAddr);
            }
        }
    }
    fclose(fp);

    return IpList;
}

/*
 *  功能：
 *      获取指定接口的IP地址
 *  参数：
 *      interface   :   接口名
 *  返回：
 *      获取的IP地址
 */
string NetworkManager::GetLocalIPAddress(string interface)
{
    int Sock;
    string IpAddr = "";

    struct sockaddr_in * Addr;
    struct ifreq IFIpAddr;

    if ((Sock = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        return "";
    }

    memset(&IFIpAddr, 0, sizeof (IFIpAddr));
    strncpy(IFIpAddr.ifr_name, interface.c_str(), sizeof (IFIpAddr.ifr_name) - 1);
    if (ioctl(Sock, SIOCGIFADDR, &IFIpAddr) < 0)
    {
        return "";
    }
    Addr = (struct sockaddr_in *) & IFIpAddr.ifr_addr;
    IpAddr = string(inet_ntoa(Addr->sin_addr));
    close(Sock);

    return IpAddr;
}

/*
 *  功能：
 *      创建emboot配置文件
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *      serverip        :
 *  返回：
 *      创建成功返回true，否则返回false
 */
bool NetworkManager::CreateEmbootFile(DbDataManager *dbdatamanager, string serverip)
{
    sql::ResultSet *TempRs = dbdatamanager->GetHostEmbootResultSet();
    if (TempRs != NULL && TempRs->rowsCount() > 0)
    {
        string FileName = DEFAULT_EMBOOT_CONFIG_FILE_NAME;

        // 建立新的文档
        xmlDocPtr Doc = xmlNewDoc(BAD_CAST "1.0");

        // 设置根节点
        xmlNodePtr RootNode = xmlNewNode(NULL, BAD_CAST "iscsi_boot");
        xmlDocSetRootElement(Doc, RootNode);

        // 新建portals节点，增加到根节点
        xmlNodePtr PortalsNode = xmlNewNode(NULL, BAD_CAST "portals");
        xmlAddChild(RootNode, PortalsNode);

        // 新建clients节点，增加到根节点
        xmlNodePtr ClientsNode = xmlNewNode(NULL, BAD_CAST "clients");
        xmlAddChild(RootNode, ClientsNode);

        // 新建portal节点,增加到portals节点
        xmlNodePtr PortalNode = xmlNewNode(NULL, BAD_CAST "portal");
        xmlAddChild(PortalsNode, PortalNode);


        // 直接在portal上新建元素
        xmlNewTextChild(PortalNode, NULL, BAD_CAST "name", BAD_CAST "san1");
        xmlNewTextChild(PortalNode, NULL, BAD_CAST "description", BAD_CAST "bes server");
        xmlNewTextChild(PortalNode, NULL, BAD_CAST "address", BAD_CAST serverip.c_str());
        xmlNewTextChild(PortalNode, NULL, BAD_CAST "port", BAD_CAST "3260");

        // 新建targets节点，增加到portal节点
        xmlNodePtr TargetsNode = xmlNewNode(NULL, BAD_CAST "targets");
        xmlAddChild(PortalNode, TargetsNode);


        // 获取结果集
        while (TempRs->next())
        {
            string CopyId = TempRs->getString("SSCOPYID");
            string Iqn = TempRs->getString("IQN");
            string HCode = TempRs->getString("HCODE");
            string MacAddress = TempRs->getString("MACADDR");

            // 设置target名字
            string TargetName = string("target") + SEPARATOR_CHAR_UNDERLINE + HCode + SEPARATOR_CHAR_UNDERLINE + CopyId;

            // 去除MAC地址中的冒号
            while (MacAddress.find_first_of(SEPARATOR_CHAR_COLON, 0) != -1)
            {
                MacAddress.replace(MacAddress.find(SEPARATOR_CHAR_COLON), 1, "");
            }

            string InitiatorName = string("bes.com.sns:") + MacAddress;

            // 新建target节点，增加到targets节点
            xmlNodePtr TargetNode = xmlNewNode(NULL, BAD_CAST "target");
            xmlAddChild(TargetsNode, TargetNode);

            // 直接在target上新建元素
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "name", BAD_CAST TargetName.c_str());
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "description", NULL);
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "target_name", BAD_CAST Iqn.c_str());
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "initiator_name", BAD_CAST InitiatorName.c_str());
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "CHAP", BAD_CAST "No");
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "username", NULL);
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "password", NULL);
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "share_mode", NULL);
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "ram_cache_size", BAD_CAST "128");
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "suppress_hdd_cache_prompt", BAD_CAST "No");
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "discard_hdd_cache", BAD_CAST "No");
            xmlNewTextChild(TargetNode, NULL, BAD_CAST "commit_hdd_cache", BAD_CAST "No");

            // 新建client节点，增加到clients节点
            xmlNodePtr ClientNode = xmlNewNode(NULL, BAD_CAST "client");
            xmlAddChild(ClientsNode, ClientNode);

            // 直接在client上新建元素
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "name", BAD_CAST HCode.c_str());
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "MAC", BAD_CAST MacAddress.c_str());
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "description", NULL);

            // 新建targets节点，增加到client节点
            xmlNodePtr ClientTargetsNode = xmlNewNode(NULL, BAD_CAST "targets");
            xmlAddChild(ClientNode, ClientTargetsNode);

            // 新建pair节点，增加到targets节点
            xmlNodePtr PairNode = xmlNewNode(NULL, BAD_CAST "pair");
            xmlAddChild(ClientTargetsNode, PairNode);

            // 直接在pair上新建元素
            xmlNewTextChild(PairNode, NULL, BAD_CAST "portal", BAD_CAST "san1");
            xmlNewTextChild(PairNode, NULL, BAD_CAST "target", BAD_CAST TargetName.c_str());

            // 直接在client上新建元素
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "undi_safe_mode", BAD_CAST "No");
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "win2008_gateway_fix", BAD_CAST "No");
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "disabled", BAD_CAST "No");
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "group", NULL);
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "admin", NULL);
            xmlNewTextChild(ClientNode, NULL, BAD_CAST "boot_menu", BAD_CAST "No");
        }

        // 保存格式的xml文档
        xmlSaveFormatFile(FileName.c_str(), Doc, XML_SAVE_FORMAT);

        // 释放文档内节点动态申请的内存
        xmlFreeDoc(Doc);
    }
    if (TempRs != NULL)
    {

        TempRs->close();
        delete TempRs;
    }

    // 重启emboot服务
    BeGeneralLib::OperateService(BeGeneralLib::SERVICE_ACTION_RESTART, DEFAULT_SERVICE_NAME_EMBOOT);
}

/*
 *  功能：
 *      创建桥接
 *  参数：
 *      bridagename     :   桥接名
 *      ipaddr          :   ip地址
 *      netmask         :   子网掩码
 *  返回：
 *      创建成功返回true，否则返回false
 */
bool NetworkManager::CreateBridge(string bridagename, string ipaddr, string netmask)
{
    string BridgeName = bridagename;
    string TempFilePath = DEFAULT_TEMP_PATH;
    string DestFilePath = "";
    string FileName = string(DEFAULT_NETWORK_CONFIG_PREFIX) + BridgeName;

    TempFilePath = TempFilePath + FileName;
    FILE *fp = fopen(TempFilePath.c_str(), "w+");
    if (fp == NULL)
    {
        BesLog::DebugPrint("Open File Failed!");
        return false;
    }

    string Netmask = netmask;
    string IpAddr = ipaddr;

    // 设备名
    string DataStr = string("DEVICE=") + BridgeName + string("\n");
    BeGeneralLib::WriteDataToFile(fp, DataStr);

    // 类型
    BeGeneralLib::WriteDataToFile(fp, "TYPE=Bridge\n");

    // 启动协议
    BeGeneralLib::WriteDataToFile(fp, "BOOTPROTO=static\n");

    // 是否自启动
    BeGeneralLib::WriteDataToFile(fp, "ONBOOT=yes\n");

    // IP地址
    DataStr = string("IPADDR=") + IpAddr + string("\n");
    BeGeneralLib::WriteDataToFile(fp, DataStr);

    // 子网掩码
    DataStr = string("NETMASK=") + Netmask + string("\n");
    BeGeneralLib::WriteDataToFile(fp, DataStr);

    // 是否自启动
    BeGeneralLib::WriteDataToFile(fp, "DELAY=0\n");

    // 是否自启动
    BeGeneralLib::WriteDataToFile(fp, "IPV6INIT=no\n");

    // 是否自启动
    BeGeneralLib::WriteDataToFile(fp, "USERCTL=no\n");

    fclose(fp);

    // 从临时文件生成最终文件
    DestFilePath = NETWORK_SCRIPTS_PATH + FileName;
    BeGeneralLib::CopyFile(TempFilePath, DestFilePath, true);
    return true;
}

/*
 *  功能：
 *      删除桥接
 *  参数：
 *      bridagename     :   桥接名
 *  返回：
 *      删除成功返回true，否则返回false
 */
bool NetworkManager::DeleteBridge(string bridgename)
{
    string BridgeName = bridgename;
    string FileName = string(DEFAULT_NETWORK_CONFIG_PREFIX) + BridgeName;
    string DestFilePath = NETWORK_SCRIPTS_PATH + FileName;
    BeGeneralLib::ForceRemoveDirectory(DestFilePath);

    // 删除桥接
    BeGeneralLib::DeleteBridge(bridgename);
    return true;
}

/*
 *  功能：
 *      创建一个pxe文件
 *  参数：
 *      serverip        :   服务器ip
 *      hcode           :   主机编码
 *      iqn             :   iqn
 *      initiatorname   :   initiator名称
 *  返回：
 *      创建成功返回true，否则返回false
 */
bool NetworkManager::CreateOnePxeFile(string serverip, string hcode, string iqn, string initiatorname)
{
    string HCode = hcode;
    string Iqn = iqn;

    // 检查Iqn、服务器ip是否合法
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(Iqn) == true
            || BeGeneralLib::IsValidIp(serverip) == false)
    {
        return false;
    }

    // 如果gpxe目录不存在，则创建gpxe目录
    BeGeneralLib::CheckAndCreateDir(GPXE_BOOT_PATH);

    // 设置gpxe文件名
    string FileName = GPXE_BOOT_PATH + HCode + GPXE_FILE_EXTENSION;

    // 检查文件是否存在，如果存在，则删除
    if (BeGeneralLib::IsFileExist(FileName) == true)
    {
        BeGeneralLib::ForceRemoveDirectory(FileName, false);
    }

    // 打开文件
    FILE *fp = fopen(FileName.c_str(), "w+");

    // 如果打开失败，则退出
    if (fp == NULL)
    {
        return false;
    }

    // 设置标题
    string DataStr = string("#!gpxe chain\n\n");
    BeGeneralLib::WriteDataToFile(fp, DataStr);

    // 设置菜单
    DataStr = string("#menu.c32 menu.cfg\n\n");
    BeGeneralLib::WriteDataToFile(fp, DataStr);

    // 设置initiator名称
    DataStr = string("set initiator-iqn") + SEPARATOR_CHAR_SPACE + initiatorname + string("\n");
    BeGeneralLib::WriteDataToFile(fp, DataStr);

    // 设置启动
    DataStr = string("sanboot iscsi:") + serverip + string(":::1:") + Iqn + string("\n");
    BeGeneralLib::WriteDataToFile(fp, DataStr);

    if (fp != NULL)
    {
        fclose(fp);
    }
    return true;
}

/*
 *  功能：
 *      更新虚拟网卡配置
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *      nicname         :   网卡名
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool NetworkManager::UpdateVirtualNetworkConfig(DbDataManager *dbdatamanager, string nicname)
{
    bool Flag = false;

    // 获取网络参数
    map<string, string> FieldValueList = dbdatamanager->GetNetworkParameter(nicname);
    if (FieldValueList.empty())
    {
        BesLog::DebugPrint("No network parameter!");
        return false;
    }

    // 获取虚拟网卡ip和子网掩码
    string Netmask = FieldValueList["NETMASK"];
    string IpAddr = FieldValueList["IPADDR"];

    // 设置虚拟网卡临时ip地址
    string ShellCommand = string("ifconfig") + SEPARATOR_CHAR_SPACE + nicname + SEPARATOR_CHAR_SPACE + IpAddr + SEPARATOR_CHAR_SPACE + string("netmask") + SEPARATOR_CHAR_SPACE + Netmask;
    BeGeneralLib::ExecuteSystem(ShellCommand);

    // 设置配置文件名
    string FileName = DEFAULT_VIRTUAL_NETWORK_CONFIG;

    // 解析XML文件
    xmlDocPtr XmlDoc = xmlParseFile(FileName.c_str());
    if (XmlDoc == NULL)
    {
        return false;
    }

    xmlNodePtr CurrentNode = xmlDocGetRootElement(XmlDoc);
    CurrentNode = CurrentNode->children;
    while (CurrentNode != NULL)
    {
        if (xmlStrcmp(CurrentNode->name, BAD_CAST "ip") != 0)
        {
            CurrentNode = CurrentNode->next;
            continue;
        }

        // 设置ip

        xmlNodePtr SearchNode = CurrentNode->children;
        while (SearchNode != NULL)
        {
            if (xmlStrcmp(SearchNode->name, BAD_CAST "dhcp") != 0)
            {
                SearchNode = SearchNode->next;
                continue;
            }

            // 删除dhcp节点
            xmlNodePtr TempNode = SearchNode->next;
            xmlUnlinkNode(SearchNode);
            xmlFreeNode(SearchNode);
            SearchNode = TempNode;

            // 保存文件
            xmlSaveFile(FileName.c_str(), XmlDoc);
            xmlFreeDoc(XmlDoc);
            xmlCleanupParser();
            return true;
        }


    }
    xmlFreeDoc(XmlDoc);
    xmlCleanupParser();

    return Flag;
}

/*
 *  功能：
 *      检查子网配置
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *  返回：
 *      更新成功返回true，否则返回false
 */
bool NetworkManager::CheckDhcpSubnetConfig(DbDataManager * dbdatamanager)
{
    // 检查输入有效性
    if (dbdatamanager == NULL)
    {
        return false;
    }

    // 获取网卡结果集
    sql::ResultSet * TempRs = dbdatamanager->GetNetworkResultSet();
    if (TempRs != NULL || TempRs->rowsCount() > 0)
    {
        while (TempRs->next())
        {
            string IpAddr = TempRs->getString("IPADDR");
            string Mask = TempRs->getString("NETMASK");

            // 检查子网配置
            CheckDhcpSubnetConfig(dbdatamanager, IpAddr, Mask);
        }
    }

    // 销毁结果集
    dbdatamanager->DestroyResultSet(TempRs);

    return true;
}

/*
 *  功能：
 *      检查子网配置
 *  参数：
 *      dbdatamanager   :   数据库管理器
 *      ip              :   ip地址
 *      netmask         :   子网掩码
 *  返回：
 *      成功返回true，否则返回false
 */
bool NetworkManager::CheckDhcpSubnetConfig(DbDataManager * dbdatamanager, string ip, string netmask)
{
    string IpAddr = ip;
    string NetMask = netmask;

    // 检查输入有效性
    if (dbdatamanager == NULL
            || BeGeneralLib::IsStringEmptyOrInvalidValue(ip) == true
            || BeGeneralLib::IsStringEmptyOrInvalidValue(netmask) == true)
    {
        return false;
    }

    // 获取子网
    string Subnet = BeGeneralLib::GetSubnet(IpAddr, NetMask);
    if (BeGeneralLib::IsStringEmptyOrInvalidValue(Subnet) == true) // 检查子网是否为无效值
    {
        return false;
    }

    // 检查dhcp
    if (dbdatamanager->IsSubnetExist(Subnet, NetMask) == true)
    {
        return true;
    }

    // 增加dhcp的子网
    string RangeLow = BeGeneralLib::MakeIpAddr(Subnet, DEFAULT_DHCP_RANGLE_LOW_HOST);
    string RangeHigh = BeGeneralLib::MakeIpAddr(Subnet, DEFAULT_DHCP_RANGLE_HIGH_HOST);
    return (dbdatamanager->AddSubnet(Subnet, NetMask, RangeLow, RangeHigh, IpAddr) == true);
}
