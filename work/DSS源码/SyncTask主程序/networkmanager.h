/*******************************************************************************
 *
 ********************* 程序文件:  NetworkManager          ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-05-17              ***********************
 ********************* 完成时间:  2010-05-28              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *20100902  LATIS   1)增加了生成路由文件
 *20100904  LATIS   1)在生成路由文件时，修改了路由子网和路由子网掩码字段的取值字段
 *20100913  LATIS   1)优化了生成文件操作，删除了原有的启动服务的方法，写文件的方法
 *20101011  LATIS   1)修改了生成GPXE配置文件和dhcp配置文件的机制
 *20101012  LATIS   1)在CreateEmbootFile方法中，修改了target名称的格式
 *20101105  LATIS   1)在CreateEmbootFile方法中，增加了initiator_name内容
 *20110112  LATIS   1)增加了CreateBridge和DeleteBridge方法
 *20110113  LATIS   1)修改了CreateBridge和DeleteBridge的参数
 *20110113  LATIS   2)在WriteNetworkConfig方法增加了桥接判断
 *20110422  LATIS   1)在WriteNetworkConfig方法增加了桥接为eth0的判断
 *20110816  LATIS   1)增加了CreateOnePxeFile方法
 *20110816  LATIS   2)修改了ResetDHCPConfigureFile方法
 *20110819  LATIS   1)在OperateRoutes方法中增加了删除原有路由的动作
 *20111026  LATIS   1)在ResetDHCPConfigureFile方法中增加了本地应急状态判断
 *20120206  LATIS   1)在CreateOnePxeFile方法中增加了initiator参数
 *20120206  LATIS   1)在ResetDHCPConfigureFile方法中将emboot的initiator名称由原来的mac地址修改为ip地址
 *
//================================== 修改结束 ==================================*/

#ifndef _NETWORKMANAGER_H
#define	_NETWORKMANAGER_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <vector>
#include <sys/stat.h>
#include <wait.h>
#include <iostream>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/tree.h>
#include <libxml/xmlsave.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "begenerallib.h"
#include "beslog.h"
#include "besgeneralheader.h"
#include "dbdatamanager.h"


using namespace std;

class NetworkManager
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
    NetworkManager();

    /*
     *  功能：
     *      析构函数
     *  参数：
     *      无
     *  返回：
     *      无
     */
    virtual ~NetworkManager();
public:
    // DHCP动作

    enum DHCPAction
    {
        DHCP_ACTION_START, // 启动
        DHCP_ACTION_STOP, // 停止
        DHCP_ACTION_RESTART, // 重启
        DHCP_ACTION_INIT, // 初始化
        DHCP_ACTION_CHECK, // 检查
    };
public:

    /*
     *  功能：
     *      操作DHCP
     *  参数：
     *      action          :   动作
     *      dbdatamanager   :   数据库管理器
     *  返回：
     *      操作成功返回true，否则返回false
     */
    static bool OperateDHCP(DHCPAction action, DbDataManager * dbdatamanager);

    /*
     *  功能：
     *      写网络配置文件
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *      nicname         :   网卡名
     *  返回：
     *      写入成功返回true，否则返回false
     */
    static bool WriteNetworkConfig(DbDataManager * dbdatamanager, string nicname);

    /*
     *  功能：
     *      操作路由
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *  返回：
     *      操作成功返回true，否则返回false
     */
    static bool OperateRoutes(DbDataManager * dbdatamanager);

private:
#define DHCP_CONFIG_FILE_NAME           "/etc/dhcp/dhcpd.conf"              // DHCP配置文件名
#define TEMP_CONFIG_FILE_NAME           "/tmp/dhcpd.conf"                   // 临时DHCP文件名
#define DHCP_CONFIG_BACKUP_NAME         "/etc/dhcp/dhcpd.conf.bak"          // DHCP的备份文件名
#define TFTP_BOOT_PATH                  "/var/lib/tftpboot/"                // tftp根目录
#define GPXE_BOOT_PATH                  "/var/lib/tftpboot/bes/"            // gPXE文件目录
#define GPXE_FILE_EXTENSION             ".gpxe"                             // gPXE扩展名
#define NETWORK_SCRIPTS_PATH            "/etc/sysconfig/network-scripts/"
#define NETWORK_DEVICE_PATH             "/etc/sysconfig/networking/devices/"
#define NETWORK_PROFILE_PATH            "/etc/sysconfig/networking/profiles/default/"
#define DEFAULT_BACKUP_FILE_EXTENSION   ".bak"
#define DEFAULT_BRIDGE_NAME_PREFIX      "br_"
#define DEFAULT_EMBOOT_CONFIG_FILE_NAME "/etc/emnbid.conf"                  // 默认emboot配置文件名
#define DEFAULT_SAN_NAME_PREFIX         "san"                               // 默认SAN名前缀
#define DEFAULT_PORTAL_NAME             "bes server"                        // 默认portal名
#define DEFAULT_INITIATOR_NAME_PREFIX   "bes.com.zhiwang:"                  // 默认initiator名前缀
#define DEFAULT_STATIC_ROUTES_PATH      "/etc/sysconfig/"
#define DEFAULT_STATIC_ROUTES_FILE_NAME "static-routes"                     // 默认静态路由配置文件名
#define DEFAULT_TEMP_PATH               "/tmp/"                             // 默认临时文件目录
#define DEFAULT_NETWORK_CONFIG_PREFIX   "ifcfg-"                            // 默认网卡配置文件名前缀
#define MAX_SUBNET_ROUTE_COUNTER        16
#define DEFAULT_VIRTUAL_NETWORK_CONFIG  "/etc/libvirt/qemu/networks/default.xml"
#define DEFAULT_DHCP_RANGLE_LOW_HOST            1
#define DEFAULT_DHCP_RANGLE_HIGH_HOST           254
private:

    struct IpAddr // IP地址结构
    {
        string NicName; // 网卡名
        string Device; // 设备名
        string Address; // 地址
        string Mac; // MAC地址
        string Mask; // 掩码
        string Gateway; // 网关
        string FileName; // 对应的文件名
    };
private:
    // 获取所有的IP地址
    static vector<IpAddr> GetAllIpAddress();

    /*
     *  功能：
     *      重新设置配置文件
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *  返回：
     *      重置成功返回true，否则返回false
     */
    static bool ResetDHCPConfigureFile(DbDataManager *dbdatamanager);

    /*
     *  功能：
     *      获取指定接口的IP地址
     *  参数：
     *      interface       :   接口名
     *  返回：
     *      获取的IP地址
     */
    static string GetLocalIPAddress(string nicname);

    /*
     *  功能：
     *      创建emboot配置文件
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *      serverip        :
     *  返回：
     *      创建成功返回true，否则返回false
     */
    static bool CreateEmbootFile(DbDataManager *dbdatamanager, string serverip);

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
    static bool CreateBridge(string bridagename, string ipaddr, string netmask);

    /*
     *  功能：
     *      删除桥接
     *  参数：
     *      bridagename     :   桥接名
     *  返回：
     *      删除成功返回true，否则返回false
     */
    static bool DeleteBridge(string bridgename);

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
    static bool CreateOnePxeFile(string serverip, string hcode, string iqn, string hostip);

    /*
     *  功能：
     *      更新虚拟网卡配置
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *      nicname         :   网卡名
     *  返回：
     *      更新成功返回true，否则返回false
     */
    static bool UpdateVirtualNetworkConfig(DbDataManager *dbdatamanager, string nicname);

    /*
     *  功能：
     *      检查子网配置
     *  参数：
     *      dbdatamanager   :   数据库管理器
     *  返回：
     *      更新成功返回true，否则返回false
     */
    static bool CheckDhcpSubnetConfig(DbDataManager * dbdatamanager);

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
    static bool CheckDhcpSubnetConfig(DbDataManager * dbdatamanager, string ip, string netmask);
};

#endif	/* _NETWORKMANAGER_H */

