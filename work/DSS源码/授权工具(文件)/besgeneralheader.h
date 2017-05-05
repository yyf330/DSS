/*******************************************************************************
 *
 ********************* 程序文件:  BESGENERALHEADER        ***********************
 ********************* 程序编写:  LATIS                   ***********************
 ********************* 创建时间:  2010-05-24              ***********************
 ********************* 完成时间:  2010-05-28              ***********************
 ********************* 程序版本:  1.0.0                   ***********************
 *
 ******************************************************************************/
/*================================== 修改列表 ==================================//
 *
 *
 *
//================================== 修改结束 ==================================*/

#ifndef _BESGENERALHEADER_H
#define	_BESGENERALHEADER_H
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <exception>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

#define DEFAULT_BUFFER_SIZE                             1024                        // 默认缓冲大小

#define SEPARATOR_CHAR_COMMA                            ','                         // 逗号
#define SEPARATOR_CHAR_SEMICOLON                        ';'                         // 分号
#define SEPARATOR_CHAR_COLON                            ':'                         // 冒号
#define SEPARATOR_CHAR_WELL                             '#'                         // 井号
#define SEPARATOR_CHAR_SPACE                            ' '                         // 空格
#define SEPARATOR_CHAR_UNDERLINE                        '_'                         // 下划线
#define SEPARATOR_CHAR_MINUS_SIGN                       '-'                         // 减号
#define SEPARATOR_CHAR_PLUS_SIGN                        '+'                         // 加号
#define SEPARATOR_CHAR_SLASH                            '/'                         // 斜线
#define SEPARATOR_CHAR_BACKSLASH                        '\\'                        // 反斜线
#define SEPARATOR_CHAR_ASTERISK                         '*'                         // 星号
#define SEPARATOR_CHAR_DOT                              '.'                         // 点
#define SEPARATOR_CHAR_EQUAL_SIGN                       '='                         // 等号

#define LINUX_PARAMETER_SEPCHAR                         (SEPARATOR_CHAR_SPACE)      // Linux参数分割符
#define LINUX_ROOT_CHAR                                 (SEPARATOR_CHAR_SLASH)      // Linux根标识符
#define LINUX_PATH_SEPCHAR                              (SEPARATOR_CHAR_SLASH)      // Linux路径分割符
#define WINDOWS_PATH_SEPCHAR                            (SEPARATOR_CHAR_BACKSLASH)  // windows路径分割符
#define DEFAULT_C_STRING_END_FLAG                       '\0'                        // 默认c字符串结尾标识符

#define DEFAULT_BES_DB_NAME                             "BES"                       // 默认bes数据库名称
#define BES_DATABASE_HOST                               "127.0.0.1"
#define BES_USER_NAME                                   "bes"
#define BES_PASSWORD                                    "sns123"

#define DEFAULT_LICENSE_EXTENSION                       ".lic"                      // 默认License文件扩展名
#define DEFAULT_CONFIG_FILE_NAME                        "bes.conf"                  // 默认配置文件名
#define DEFAULT_CONFIG_ITEM_HOST                        "HOST"                      // 配置项BES的ip
#define DEFAULT_CONFIG_ITEM_USER                        "USER"                      // 配置项数据库用户名
#define DEFAULT_CONFIG_ITEM_PASSWORD                    "PASSWORD"                  // 配置项数据库密码
#define DEFAULT_CONFIG_ITEM_DBNAME                      "DBNAME"                    // 配置项数据库名称
#define DEFAULT_CONFIG_ITEM_FCSERVER                    "FCSERVER"                  // 配置项存储服务器地址
#define DEFAULT_CONFIG_ITEM_LANG                        "LANG"                      // 配置项语言
#define DEFAULT_CONFIG_ITEM_TARGETBASE                  "TARGETBASE"                // 配置项target类型
#define DEFAULT_CONFIG_ITEM_MONITORUSER                 "MONITORUSER"               // 配置项监控的用户名
#define DEFAULT_CONFIG_ITEM_MONITORPWD                  "MONITORPWD"                // 配置项监控的密码
#define DEFAULT_CONFIG_ITEM_DBBACKUPNUM                 "DBBACKUPNUM"               // 配置项数据库备份个数
#define DEFAULT_CONFIG_ITEM_FILESYSTEM                  "FILESYSTEM"                // 配置项文件系统类型
#define DEFAULT_CONFIG_ITEM_AGENTTIMEOUT                "AGENTTIMEOUT"              // 配置项agent超时时间（s）

#define DEFAULT_HOST_CODE_SIZE                          5                           // 默认主机编码长度

#define NETBOOT_PROTOCOL_PXE                            "PXE"                       // 网启PXE协议
#define NETBOOT_PROTOCOL_SNSBOOT                        "SNSBOOT"                   // 其他网启协议
#define NETBOOT_PROTOCOL_HBA                            "HBA"                       // HBA卡网启协议
#define NETBOOT_PROTOCOL_FCBOOT                         "FCBOOT"                    // 光纤卡网启协议
#define NETBOOT_PROTOCOL_VMBOOT                         "VMBOOT"                    // 虚拟机启动协议

#define DEFAULT_OSTYPE_WINDOWS                          "100"                       // WINDOWS操作系统代码
#define DEFAULT_OSTYPE_WIN2K_XP_2K3                     "101"
#define DEFAULT_OSTYPE_WIN2K8                           "102"
#define DEFAULT_OSTYPE_LINUX                            "200"                       // LINUX操作系统代码
#define DEFAULT_OSTYPE_AIX                              "300"                       // AIX操作系统代码
#define DEFAULT_OSTYPE_HPUX                             "400"                       // HPUX操作系统代码

#define BES_AGENT_PORT                                  8585                        // 默认agent端口
#define BES_SERVER_PORT                                 8585                        // 默认server端口
#define BES_INTERNAL_PORT                               8586                        // 内部通讯端口
#define MANAGE_PORT                                     8587                        // 管理端口
#define BES_RDR_PORT                                    9595                        // 默认RDR端口

#define DEFAULT_FC_SERVER_IP                            "10.10.10.253"              // FC存储服务器ip
#define DEFAULT_LOCALHOST_IP                            "127.0.0.1"                 // 默认本地ip

#define DEFAULT_UPGRADE_ZIP_PATH                        "/var/www/html/upload/"     // 默认升级包路径
#define DEFAULT_BACKUP_DIRECTORY                        "/oldversion/"              // 默认备份目录名
#define DEFAULT_BACKUP_FILE_SUFFIX                      "_bak"

#define HOST_ACTION_STARTVM                             "STARTVM"                   // 主机动作启动虚拟机
#define HOST_ACTION_STOPVM                              "STOPVM"                    // 主机动作启动虚拟机

#define DEFAULT_SOCKET_CONNECT_TIMEOUT                  10                          // 默认socket连接超时(s)

#define DEFAULT_DATABASE_BACKUP_NUM                     5                           // 默认数据库备份文件个数
#define DEFAULT_INVALID_RETURN_VALUE                    "?"                         // 默认无效返回值

#define DEFAULT_SERVICE_NAME_DHCP                       "dhcpd"                     // 默认dhcp服务名
#define DEFAULT_SERVICE_NAME_EMBOOT                     "emnbid"                    // 默认emboot服务名
#define DEFAULT_SERVICE_NAME_STGT                       "tgtd"                      // 默认stgt服务名
#define DEFAULT_SERVICE_NAME_SCST                       "iscsi-scst"                // 默认scst服务名
#define DEFAULT_SERVICE_NAME_HTTP                       "httpd"                     // 默认http服务名
#define DEFAULT_SERVICE_NAME_MYSQL                      "mysqld"                    // 默认mysql服务名
#define DEFAULT_SERVICE_NAME_XINETD                     "xinetd"                    // 默认xinetd服务名
#define DEFAULT_SERVICE_NAME_ISCSID                     "iscsid"                    // 默认iscsid服务名

#define DEFAULT_DIR_RIGHT                               (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH) // 默认目录权限

#define DEFAULT_FILESYSTEM_TYPE_BTRFS                   "btrfs"                     // 默认文件系统类型btrfs
#define DEFAULT_FILESYSTEM_TYPE_NILFS2                  "nilfs2"                    // 默认文件系统类型nilfs2
#define DEFAULT_FILESYSTEM_TYPE_FAT32                   "fat32"                     // 默认文件系统类型fat32
#define DEFAULT_FILESYSTEM_TYPE_NTFS                    "ntfs"                      // 默认文件系统类型ntfs
#define DEFAULT_FILESYSTEM_TYPE_NTFS_3G                 "ntfs-3g"                   // 默认文件系统类型ntfs
#define DEFAULT_FILESYSTEM_TYPE_EXT3                    "ext3"                      // 默认文件系统类型ext3
#define DEFAULT_FILESYSTEM_TYPE_EXT4                    "ext4"                      // 默认文件系统类型ext4

enum BaseFileSystem
{
    BASE_FILESYSTEM_UNKNOWN,
    BASE_FILESYSTEM_BTRFS,
    BASE_FILESYSTEM_NILFS2,
    BASE_FILESYSTEM_FAT32,
    BASE_FILESYSTEM_NTFS,
    BASE_FILESYSTEM_EXT3,
    BASE_FILESYSTEM_EXT4,
};

#ifdef _DEBUG_VERSION_
#undef   _DEBUG_VERSION_
#endif

//#ifndef _DEBUG_VERSION_
//#define   _DEBUG_VERSION_
//#endif

#endif	/* _BESGENERALHEADER_H */

