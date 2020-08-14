#ifndef __SEPCAM_PROTOCAL_H__
#define __SEPCAM_PROTOCAL_H__

#ifdef WIN32
#include <stdint.h>
#ifndef UINT16_t
    typedef uint16_t    UINT16_t;
    typedef uint32_t    UINT32_t;
    typedef uint64_t    UINT64_t;
#endif //UINT16_t
#define INLINE_t inline
//#include <strings.h>
#endif //WIN32

typedef char          T_S8;
typedef int16_t         T_S16;
typedef int32_t         T_S32;
typedef int64_t         T_S64; 

typedef unsigned char        T_U8;
typedef uint16_t       T_U16;
typedef uint32_t       T_U32;
typedef uint64_t       T_U64;

typedef const char      T_CS8;
typedef const unsigned char T_CU8;


#define MAX_BIND_TIME 10
#define BROADCAST_SEND_PORT			8600
#define BROADCAST_RECV_PORT			6801

/************************************************************************/
/*通讯格式：
	STARTCODE+CMD+BCASTPARAM
	定义：
	STARTCODE： short型，字符”HD”,0x4844
	CMD：short型，0x0101 表示获取
	0x0801 表示获取时的返回
	0x0102 表示设置
	0x0802 表示设置时的返回 */
/************************************************************************/

#define STARTCODE  0x4844  //HD
#define CMD_GET  0x0101
#define CMD_GET_RESPONSE  0x0801 
#define CMD_SET  0x0102
#define CMD_SET_RESPONSE  0x0802

#define CMD_GET_FACTORY           			0x0103
#define CMD_GET_FACTORY_RESPONSE          	0x0803

#define CMD_SET_FACTORY_DEFAULT				0x0104
#define CMD_SET_FACTORY_DEFAULT_RESPONSE	0x0804
//record search camera
#define CMD_GET_CAMERA           				0x0105
#define CMD_GET_CAMERA_RESPONSE      			0x0805
//config network to dhcp
#define CMD_CAMERA_DHCP          			0x0106
#define CMD_CAMERA_DHCP_RESPONSE     		0x0806

#define CMD_CAMERA_REBOOT          			0x0107
#define CMD_CAMERA_REBOOT_RESPONSE     		0x0807

#define START_UPDATE_CMD          			0x0110
#define START_UPDATE_CMD_RESP     			0x0810

typedef struct _stBcastParam
{
	char            szIpAddr[16];		//IP地址
	char            szMask[16];		//子网掩码
	char            szGateway[16];		//网关
	char            szDns1[16];		//dns1
	char            szDns2[16];		//dns2
	char            szMacAddr[6];		//设备MAC地址
	unsigned short          nPort;			//设备端口
	char            dwDeviceID[32]; 		//platform deviceid
	char            szDevName[32];		//设备名称
	char            sysver[16];		//固件版本
	char            appver[16];		//软件版本
	char            szUserName[32];		//修改时会对用户认证
	char            szPassword[32];		//修改时会对用户认证
	char            devType;        		//0->baby 1->HDIPCAM
	char					dhcp;
	char            		other[2];       		//other
	unsigned char			WifiMac[6];
	unsigned short			other1;
	char					uuidapilicense[6];
	char					other22[5];			//other2
	char					valid;
	char					fid[32];
}BCASTPARAM, *PBCASTPARAM;

/*typedef struct _MSG_HEAD
{
    T_U16  msg_type;
    T_U16  ext_msg_type;
    T_U32  guid;
    T_U32  result;//0表示成功，其他的错误从1开始
    T_U32  version;//(V 1.0.0)
    
    T_U32  msg_head_len;//sizeof(msg_head) + 扩展长度）;
    T_U32  msg_len;//(msg_head_len + 实际数据长度 )；
    T_U32  timestamp;
    T_U32 sequence;//序列号
    
    T_S8   dev_id[32];

    T_S8  tlv_num;	//如果是TLV结构的，此字段为TLV个数，否则保留不用
    T_S8  res[7];
}MSG_HEAD_t;
*/
//partition type
enum PartitionTypeEnum
{
	PART_UBOOT = 1,
	PART_KERNEL,
	PART_ROOTFS,
	PART_SYSTEM,
	PART_CUSTOM,
	PART_USR,
	PART_VENDOR,
	PART_WEB,
	WIFI_MODULE_FIRMWARE_RTL8710AF
};
#define PART_CUSTOM_NAME	"custom"
#define PART_SYSTEM_NAME	"system"
#define PART_USR_NAME		"usr"
#define PART_VENDOR_NAME	"vendor"
#define PART_WEB_NAME		"web"
#define PART_KERNEL_NAME	"kernel"
#define PART_ROOTFS_NAME	"rootfs"
#define WIFI_MODULE_FIRMWARE_RTL8710AF_NAME "WIFI_MODULE_FIRMWARE_RTL8710AF"

typedef struct{
	int mPartType;
	char mPartName[16];
	int mFileLen;
	char mMD5[32];
}PartUpdateReq_t;

typedef enum
{
    LOGIN_DM_CMD = 0x5910 + 1,
	LOGIN_DM_CMD_RESP,

    WRITE_UUID_DM_CMD,
	WRITE_UUID_DM_CMD_RESP,

    DHCP_DM_CMD,
	DHCP_DM_CMD_RESP,

    UPDATE_DM_CMD,
	UPDATE_DM_CMD_RESP,

    READ_DM_CMD,
	READ_DM_CMD_RESP,

	READ_FINISH_DM_CMD,
	READ_FINISH_DM_CMD_RESP,

	WRITE_DM_CMD,
	WRITE_DM_CMD_RESP,

	WRITE_FINISH_DM_CMD,
	WRITE_FINISH_DM_CMD_RESP,

	FLASH_DM_CMD,
	FLASH_DM_CMD_RESP,

	STOP_UPDATE_DM_CMD,
    STOP_UPDATE_DM_CMD_RESP,
}DM_TASK_CMD_E;

#endif//#ifndef __SEPCAM_PROTOCAL_H__
