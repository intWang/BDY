/*
*filename:JSONStructProtocal.h
*author:hujian
*date:2017/12/03 11:03
*/
#ifndef __JSON_STRUCT_PROTOCAL_H__
#define __JSON_STRUCT_PROTOCAL_H__
#include "JSONObject.hpp"

#if _WIN32
#pragma comment(lib, "ws2_32.lib")
#endif

typedef char          T_S8;
typedef short int         T_S16;
typedef int         T_S32;
typedef long long         T_S64; 

typedef unsigned char        T_U8;
typedef unsigned short int       T_U16;
typedef unsigned int       T_U32;
typedef unsigned long long        T_U64;
#ifndef __APPLE__
typedef int BOOL;
#endif

typedef enum{
	IPCNET_LOGIN_REQ = 1000,
	IPCNET_LOGIN_RESP,

	IPCNET_KEEPALIVE_REQ,
	IPCNET_KEEPALIVE_RESP,

	IPCNET_STREAM_REQ,
	IPCNET_STREAM_RESP,

	IPCNET_TALK_REQ,
	IPCNET_TALK_RESP,

	IPCNET_FORCELOGOUT_REQ,
	IPCNET_FORCELOGOUT_RESP,

    IPCNET_STREAM_EX_REQ,
    IPCNET_STREAM_EX_RESP,
    
	IPCNET_NET_SYSINFO_REQ = 1020,
	IPCNET_NET_SYSINFO_RESP,

	IPCNET_USER_SET_REQ,
	IPCNET_USER_SET_RESP,

	IPCNET_MCU_INFO_REQ,
	IPCNET_MCU_INFO_RESP,
	
	IPCNET_ETH_SET_REQ = 1040,  //IPCNetEthConfig_st
	IPCNET_ETH_SET_RESP,
	
    IPCNET_ETH_GET_REQ = 1042,
    IPCNET_ETH_GET_RESP,    //IPCNetEthConfig_st
    
    IPCNET_NETWORK_MOBILE_SET_REQ = 1044, //Ipcnet3GInfo_st
    IPCNET_NETWORK_MOBILE_SET_RESP,

    IPCNET_NETWORK_MOBILE_GET_REQ = 1046,
    IPCNET_NETWORK_MOBILE_GET_RESP, //Ipcnet3GInfo_st

    IPCNET_NETWORK_WIFI_SET_REQ = 1048, //IPCNetWirelessConfig_st
    IPCNET_NETWORK_WIFI_SET_RESP,
    
    IPCNET_NETWORK_WIFI_GET_REQ = 1050,
    IPCNET_NETWORK_WIFI_GET_RESP,   //IPCNetWirelessConfig_st
    
    IPCNET_NETWORK_WIFI_SEARCH_GET_REQ = 1052,
    IPCNET_NETWORK_WIFI_SEARCH_GET_RESP,  //IpcnetNetworkWirelessSearch_st

    IPCNET_VIDEO_ENC_SET_REQ = 1054,    //IPCNetVideoEncodeCfg_st
    IPCNET_VIDEO_ENC_SET_RESP,

    IPCNET_VIDEO_ENC_GET_REQ = 1056,
    IPCNET_VIDEO_ENC_GET_RESP,  //IPCNetVideoEncodeCfg_st
    
    IPCNET_AUDIO_ENC_SET_REQ = 1058,   //IpcnetAudioEncodeCfg_st
    IPCNET_AUDIO_ENC_SET_RESP,

    IPCNET_AUDIO_ENC_GET_REQ = 1060,
    IPCNET_AUDIO_ENC_GET_RESP,  //IpcnetAudioEncodeCfg_st

    IPCNET_PTZ_GET_REQ = 1062,  //未用
    IPCNET_PTZ_GET_RESP,    //未用

    IPCNET_PTZ_SET_REQ = 1064,     //IPCPtzCtrlMsg_st
    IPCNET_PTZ_SET_RESP,
    
    IPCNET_MOVE_ALARM_SET_REQ = 1066, //IPCNETMoveCellAlarmCfg_st
    IPCNET_MOVE_ALARM_SET_RESP,

    IPCNET_MOVE_ALARM_GET_REQ = 1068,
    IPCNET_MOVE_ALARM_GET_RESP,

	IPCNET_AV_RECO_CONF_SET_REQ = 1070,
    IPCNET_AV_RECO_CONF_SET_RESP,

    IPCNET_AV_RECO_CONF_GET_REQ = 1072,
    IPCNET_AV_RECO_CONF_GET_RESP,

	IPCNET_AV_RECO_LIST_GET_REQ = 1074,
    IPCNET_AV_RECO_LIST_GET_RESP,

	IPCNET_AV_RECO_LIST_PAGE_GET_REQ = 1076,
    IPCNET_AV_RECO_LIST_PAGE_GET_RESP,

	IPCNET_AV_RECO_OP_REQ = 1078,
    IPCNET_AV_RECO_OP_RESP,     // 1079

	IPCNET_UPGRADE_REQ = 1080,
	IPCNET_UPGRADE_RESP,

	IPCNET_GET_GPIO_REQ = 1082,
	IPCNET_GET_GPIO_RESP,
	
	IPCNET_SET_GPIO_REQ = 1084,
	IPCNET_SET_GPIO_RESP,

	IPCNET_GET_SUBDEV_REQ = 1086,
	IPCNET_GET_SUBDEV_RESP,
	
	IPCNET_SET_SUBDEV_REQ = 1088,
	IPCNET_SET_SUBDEV_RESP,

	IPCNET_GET_BUS_REQ = 1090,
	IPCNET_GET_BUS_RESP,
	
	IPCNET_SET_BUS_REQ = 1092, //串口透传
	IPCNET_SET_BUS_RESP,    // 1093

	IPCNET_GET_DEV_INFO_REQ = 1094,
	IPCNET_GET_DEV_INFO_RESP,

	IPCNET_SET_DEV_INFO_REQ = 1096,
	IPCNET_SET_DEV_INFO_RESP,
	
	IPCNET_UPGRADE_CFG_REQ = 1098,
	IPCNET_UPGRADE_CFG_RESP,
	
	IPCNET_UPGRADE_AUTO_SET_REQ = 1100,
	IPCNET_UPGRADE_AUTO_SET_RESP,
	
	IPCNET_SET_DEFAULT_REQ = 1102,
    IPCNET_SET_DEFAULT_RESP,

	IPCNET_SET_REBOOT_REQ = 1104,
    IPCNET_SET_REBOOT_RESP,
    
    IPCNET_GET_NET_PORT_REQ = 1106,
    IPCNET_GET_NET_PORT_RESP,
    
    IPCNET_SET_NET_PORT_REQ = 1108,
    IPCNET_SET_NET_PORT_RESP,

	IPCNET_GET_GPIO_ARRAY_REQ = 1110,
	IPCNET_GET_GPIO_ARRAY_RESP,
	
	IPCNET_SET_GPIO_ARRAY_REQ = 1112,
	IPCNET_SET_GPIO_ARRAY_RESP,

    IPCNET_GET_MOTION_DETECT_REQ = 1114,
    IPCNET_GET_MOTION_DETECT_RESP,
    
    IPCNET_SET_MOTION_DETECT_REQ = 1116,
    IPCNET_SET_MOTION_DETECT_RESP,

	IPCNET_WIFI_STATUS_REPORT_RESP = 1201,       // 1201

    IPCNET_VIDEO_IFRAME_REQ = 1202,   //IPCNetVideoIFrame_st
    IPCNET_VIDEO_IFRAME_RESP,

    IPCNET_GET_VIDEO_QULITY_LEVEL_REQ = 1204,
    IPCNET_GET_VIDEO_QULITY_LEVEL_RESP,
    
    IPCNET_SET_VIDEO_QULITY_LEVEL_REQ = 1206,
    IPCNET_SET_VIDEO_QULITY_LEVEL_RESP,
    
    IPCNET_GET_NETWORKS_STATUS_REQ = 1208,
    IPCNET_GET_NETWORKS_STATUS_RESP,
	
	IPCNET_NET_CFG_PTZ_REQ = 1400,
	IPCNET_NET_CFG_PTZ_RESP,

	IPCNET_GET_TIME_REQ = 1402,
	IPCNET_GET_TIME_RESP,
	
	IPCNET_SET_TIME_REQ = 1404, //IPCNetTimeCfg_st
	IPCNET_SET_TIME_RESP,   //1405

    IPCNET_NET_PRESET_SET_PTZ_REQ = 1406,
	IPCNET_NET_PRESET_SET_PTZ_RESP, //IPCNetTimeCfg_st

    IPCNET_NET_PRESET_GET_PTZ_REQ = 1408,      
	IPCNET_NET_PRESET_GET_PTZ_RESP,

	//---控制PC灯光(普通照明) 
	IPCNET_GET_LAMP_REQ = 1410,
	IPCNET_GET_LAMP_RESP,
	
	IPCNET_SET_LAMP_REQ = 1412,
	IPCNET_SET_LAMP_RESP,

	//----图像翻转
	IPCNET_GET_OVERTURN_REQ = 1414,
	IPCNET_GET_OVERTURN_RESP,       //1415
	
	IPCNET_SET_OVERTURN_REQ = 1416,	
	IPCNET_SET_OVERTURN_RESQ,	
	
	/**设置曝光类型**/
	IPCNET_GET_EXPOSURE_TYPE_REQ = 1418,
	IPCNET_GET_EXPOSURE_TYPE_RESP,
	
	IPCNET_SET_EXPOSURE_TYPE_REQ = 1420,
	IPCNET_SET_EXPOSURE_TYPE_RESP,

	/**自动曝光**/
	IPCNET_GET_AUTO_EXPOSURE_REQ = 1422,
	IPCNET_GET_AUTO_EXPOSURE_RESP,

	IPCNET_SET_AUTO_EXPOSURE_REQ = 1424,
	IPCNET_SET_AUTO_EXPOSURE_RESP,      //1425

	/**手动曝光**/
	IPCNET_GET_MANUAL_EXPOSURE_REQ = 1426,
	IPCNET_GET_MANUAL_EXPOSURE_RESP,

	IPCNET_SET_MANUAL_EXPOSURE_REQ = 1428,
	IPCNET_SET_MANUAL_EXPOSURE_RESP,

	//----彩转黑方式
	IPCNET_GET_PICOLOR_REQ = 1430,	 /*获取彩转黑*/         //1430
	IPCNET_GET_PICOLOR_RESP,
	
	IPCNET_SET_PICOLOR_REQ = 1432, /*设置彩转黑*/
	IPCNET_SET_PICOLOR_RESP, 
	
	//----环境设置
	IPCNET_GET_ENVIRONMENT_REQ = 1434,		/*获取环境*///1--outdoor模式 0--indoor模式
	IPCNET_GET_ENVIRONMENT_RESP,
	
	IPCNET_SET_ENVIRONMENT_REQ = 1436,		/*设置环境*///1--outdoor模式 0--indoor模式
	IPCNET_SET_ENVIRONMENT_RESP,

	//---2D/3D降噪
	IPCNET_GET_DENOISE_REQ = 1438,		/*1438 获取2D/3D 降噪*/	
	IPCNET_GET_DENOISE_RESP,
	
	IPCNET_SET_DENOISE_REQ = 1440,	/*设置2D/3D 降噪*/          //1440
	IPCNET_SET_DENOISE_RESP,        

	//v1---宽动态设置
	IPCNET_GET_WDR_REQ = 1442,		/* 1442 获取WDR*/
	IPCNET_GET_WDR_RESP,
	
	IPCNET_SET_WDR_REQ = 1444,		/*设置WDR*/
	IPCNET_SET_WDR_RESP,		//1445

	//--白平衡
	IPCNET_GET_WH_BLANCE_REQ = 1446,
	IPCNET_GET_WH_BLANCE_RESP,
	
	IPCNET_SET_WH_BLANCE_REQ = 1448,
	IPCNET_SET_WH_BLANCE_RESP,

	//--降帧
	IPCNET_GET_SLOW_FRAME_RATE_REQ = 1450,
	IPCNET_GET_SLOW_FRAME_RATE_RESP,     
	
	IPCNET_SET_SLOW_FRAME_RATE_REQ = 1452,
	IPCNET_SET_SLOW_FRAME_RATE_RESP,
	
	//--设置默认值
	IPCNET_GET_EXPOSURE_DEFAULT_REQ = 1454,		//未用到
	IPCNET_GET_EXPOSURE_DEFAULT_RESP,       //1455
	
	IPCNET_SET_EXPOSURE_DEFAULT_REQ = 1456,      
	IPCNET_SET_EXPOSURE_DEFAULT_RESP,

	//--坏点校正
	IPCNET_GET_BADPIXEL_DETECT_REQ = 1458, 	//未用到
	IPCNET_GET_BADPIXEL_DETECT_RESP,
	
	IPCNET_SET_BADPIXEL_DETECT_REQ = 1460,
	IPCNET_SET_BADPIXEL_DETECT_RESP,       
	
	IPCNET_GET_ANTIFLICKER_REQ = 1462,	 	/*1462 获取抗闪炼*/	
	IPCNET_GET_ANTIFLICKER_RESP,
	
	IPCNET_SET_ANTIFLICKER_REQ = 1464,		 /*设置抗闪炼*/
	IPCNET_SET_ANTIFLICKER_RESP,        //1465
	
	//wdr v2---多余的命令，保留不能删除，否则影响后面的命令
	IPCNET_GET_RESERVE_REQ = 1466,		
	IPCNET_GET_RESERVE_RESP,
	
	IPCNET_SET_RESERVE_REQ = 1468,		
	IPCNET_SET_RESERVE_RESP,

	//----自动彩转黑手动设置光量值
	IPCNET_GET_PICOLOR4MANUlUAM_REQ = 1470,
	IPCNET_GET_PICOLOR4MANUlUAM_RESP,   
	
	IPCNET_SET_PICOLOR4MANUlUAM_REQ = 1472, 
	IPCNET_SET_PICOLOR4MANUlUAM_RESP,

    IPCNET_SNAP_SHOOT_REQ = 1474,
	IPCNET_SNAP_SHOOT_RESP,     //1475

    //预置点
	IPCNET_GET_PREPOINT_REQ = 1476,
	IPCNET_GET_PREPOINT_RESP,       //1477    

	IPCNET_SET_PREPOINT_REQ = 1478,
	IPCNET_SET_PREPOINT_RESP,       //1479
    
	IPCNET_OPERATE_PREPOINT_REQ = 1480,
	IPCNET_OPERATE_PREPOINT_RESP,       //1481

    //ftp参数设置
	IPCNET_SET_FTP_CFG_REQ = 1482,
	IPCNET_SET_FTP_CFG_RESP,

	IPCNET_GET_FTP_CFG_REQ = 1484,
	IPCNET_GET_FTP_CFG_RESP,        //1485

    /***Email cfg***/
	IPCNET_SET_EMAIL_CFG_REQ = 1486,
	IPCNET_SET_EMAIL_CFG_RESP,

	IPCNET_GET_EMAIL_CFG_REQ = 1488,
	IPCNET_GET_EMAIL_CFG_RESP,

    /***Ddns cfg***/
	IPCNET_SET_DDNS_CFG_REQ = 1490,
	IPCNET_SET_DDNS_CFG_RESP,

	IPCNET_GET_DDNS_CFG_REQ,
	IPCNET_GET_DDNS_CFG_RESP,

    /***设置亮度，对比度，饱和度***/
	IPCNET_SET_CAM_PIC_CFG_REQ = 1494,
	IPCNET_SET_CAM_PIC_CFG_RESP,        //1495

	IPCNET_GET_CAM_PIC_CFG_REQ = 1496,
	IPCNET_GET_CAM_PIC_CFG_RESP,

	/****IRCUT切换****/
	IPCNET_SET_IRCUT_REQ = 1498,
	IPCNET_SET_IRCUT_RESP,

	IPCNET_GET_IRCUT_REQ = 1500,
	IPCNET_GET_IRCUT_RESP,

	IPCNET_ALARM_REPORT_RESP = 1502,       // 1502主动上传报警信息

    /****OSD cfg****/
	IPCNET_SET_OSD_REQ = 1503,
	IPCNET_SET_OSD_RESP = 1504,
    
	IPCNET_GET_OSD_REQ = 1505,
	IPCNET_GET_OSD_RESP = 1506,

	IPCNET_FORMAT_EXFAT_REQ = 1507,
	IPCNET_FORMAT_EXFAT_RESP,

	IPCNET_BINARY_DATA_MSG_REQ =   1509, //二进制数据消息
	IPCNET_BINARY_DATA_MSG_RESP,

	IPCNET_START_REMOTE_DEBUG_MSG_REQ = 1511, //开启远程调试
	IPCNET_START_REMOTE_DEBUG_MSG_RESP,

	IPCNET_END_REMOTE_DEBUG_MSG_REQ = 1513, //关闭远程调试
	IPCNET_END_REMOTE_DEBUG_MSG_RESP,

	IPCNET_GET_FUNC_CFG_REQ = 1515, //功能参数获取
	IPCNET_GET_FUNC_CFG_RESP,    

    IPCNET_SET_FUNC_CFG_REQ = 1517, //功能参数设置
	IPCNET_SET_FUNC_CFG_RESP,    

	IPCNET_GET_STA_LED_CFG_REQ = 1519, //获取灯的使用状态
	IPCNET_GET_STA_LED_CFG_RESP,

    IPCNET_SET_STA_LED_CFG_REQ = 1521, //设置灯的使能
	IPCNET_SET_STA_LED_CFG_RESP,

    IPCNET_GET_DET_LUM_AREA_CFG_REQ = 1523, //获取区域亮度检测参数
    IPCNET_GET_DET_LUM_AREA_CFG_RESP,       

    IPCNET_SET_DET_LUM_AREA_CFG_REQ = 1525, //设置区域亮度检测参数
    IPCNET_SET_DET_LUM_AREA_CFG_RESP,  

    IPCNET_DISK_TEST_REQ = 1527, //disk测试
    IPCNET_DISK_TEST_RESP,

    IPCNET_SET_P2P_SER_CODE_REQ = 1529, //更改p2p服务器串码
    IPCNET_SET_P2P_SER_CODE_RESP,

    IPCNET_GET_P2P_SER_CODE_REQ = 1531,        // 1531 获取p2p服务器串码
    IPCNET_GET_P2P_SER_CODE_RESP,

#if 0
    IPCNET_GET_NET_MAN_CFG_REQ,         // 1533 --网络选择信息
    IPCNET_GET_NET_MAN_CFG_RESQ,
    
    IPCNET_SET_NET_MAN_CFG_REQ,         // 1535 --设置使用哪个网络
    IPCNET_SET_NET_MAN_CFG_RESQ,
#endif  

    IPCNET_GET_TIMER_CFG_REQ = 1537,       //获取定时器参数
    IPCNET_GET_TIMER_CFG_RESP,

    IPCNET_SET_TIMER_CFG_REQ = 1539,       //获取定时器参数
    IPCNET_SET_TIMER_CFG_RESP,

    IPCNET_GET_DISK_CFG_REQ = 1541,        //获取磁盘参数
    IPCNET_GET_DISK_CFG_RESP,

    IPCNET_GET_TIMEZONE_CFG_REQ = 1543, //时区、夏令时 IPCNetTimeZoneCfg_st
    IPCNET_GET_TIMEZONE_CFG_RESP,

    IPCNET_SET_TIMEZONE_CFG_REQ = 1545, //时区、夏令时 IPCNetTimeZoneCfg_st
    IPCNET_SET_TIMEZONE_CFG_RESP,

    
    
    //---------------这里以下主要是sdk协议的扩展，不作网络协议----------
    IPCNET_INTERNEL_MSG_TYPE = 3000,

	REMOTE_MSG_LOGIN=3001,

    IPCNET_GET_USER_ALL_INFO_REQ = 3002,
    IPCNET_GET_USER_ALL_INFO_RESP,  //IPCNetGetUserGroupCfg_st

    IPCNET_GET_USER_INFO_REQ = 3004,    //IPCNetUserInfoReq_st
    IPCNET_GET_USER_INFO_RESP,  //IPCNetUserInfo_st
    
	IPCNET_GET_STREAM_REQ = 3006,

    IPCNET_SET_USER_INFO_REQ = 3006,    //IPCNetUserInfo_st
    IPCNET_SET_USER_INFO_RESP,

    IPCNET_GET_SYS_INFO = 3008,      //IPCNetCamInfo_st    
    IPCNET_GET_SYS_INFO_RESP,
    
    IPCNET_MSG_TYPE_BUTT = 4000,
	REMOTE_MSG_RESP_LOGIN = 4003,
}JIANLE_NET_MSG_TYPE_e;
typedef enum
{
    IPCNET_RET_OK,      //0成功
    
	_IPCNET_RET_OK_ = 100,     //这个是不用的
	IPCNET_NET_RET_UNKNOWN,          //其它错误码从101开始
	IPCNET_NET_RET_VERSION_NOT_SUPPORT,
	IPCNET_RET_REQ_ILLEGAL,
	IPCNET_NET_RET_LOGIN_EXSIST,

	IPCNET_RET_UNLOGIN = 105, //5
	IPCNET_RET_PASSWARD_ERR,
	IPCNET_RET_UNAUTHORISED,
	IPCNET_RET_TIMEOUT,
	IPCNET_RET_NOT_FOUND,

    IPCNET_RET_USER_NOT_FOUND = 110,
	IPCNET_RET_PASSWARD_NULL,
	IPCNET_NET_RET_FIND_AND_SEND_OVER = 112,
	IPCNET_NET_RET_FIND_AND_SEND_APART,
	IPCNET_RET_SENDBUF_MEM_NOTENOUGH = 114,

	IPCNET_NET_RET_NEED_RESTART_APP = 602,
	IPCNET_NET_RET_NEED_RESTART_SYS,
	IPCNET_NET_RET_WRITE_FILE_ERR,
	
	IPCNET_NET_RET_REQ_NOT_SUPPORT = 605,
	IPCNET_NET_RET_NEED_VERIFY_ERR,
	IPCNET_NET_RET_NEED_CONFIG_UNEXIST,
	IPCNET_NET_RET_NEED_PASER_ERR,
	IPCNET_RET_NO_MEM,

	IPC_NET_RET_UPDATING = 610,
	IPC_NET_RET_OPERATE_BUSYING,
	IPCNET_RET_USER_NAME_TOO_LONG,
	IPCNET_RET_PASSWD_TOO_LONG,
	
	IPCNET_NET_RET_REQ_ID_UNKNOWN = 1001,
	IPCNET_NET_RET_REQ_UNKNOWN,

	IPCNET_RET_FORMAT_FAIL =1050,
	
	IPCNET_NET_RET_NULL = 0xFFFFFF, //这个值不属于协议范围，是为了让处理函数on_ipcnet_msg_process()知道这个请求是异步的，不是立即回复的
}IPCNET_NET_RETURN_CODE_e;

#define STREAM_TYPE_VIDEO 0x01      //开启视频
#define STREAM_TYPE_AUDIO 0x02      //开启音频
#define STREAM_TYPE_COMP 0x03       //开启复合流
#define DECODE_TYPE_VIDEO 0x04       //解码视频
#define DECODE_TYPE_AUDIO 0x08       //解码音频
#define DECODEM_TYPE_COMP 0x0c       //开启复合流

typedef enum
{
	AUDIO_ADPCM = 20,
	AUDIO_G711A,
	AUDIO_G711U,
	AUDIO_G726,
	AUDIO_AMR,	
	AUDIO_ACC,
}AUDIO_TYPE;

#define CMD_TYPE_START  0x01        //开始发送码流
#define CMD_TYPE_STOP   0x02        //结束发送码流
#define CMD_TYPE_START_EX  0x03        //开始发送码流

//数据包头
typedef struct PKT_HEAD
{
    T_U32 frame_no;//整个帧序号
    T_U32  frame_size; //整个帧长度
    T_U8  frame_type;//帧类型
    T_U8  pkt_no;//小帧序号
    T_U16 pkt_size;//小帧长度
    //以下有HI3512取帧时时间戳 U64 分解成
    T_U32 sec;
    T_U32 usec;
}PKT_HEAD_t;

typedef struct P2P_PKT_HEAD
{
    unsigned signature;
    PKT_HEAD_t pkt_hd;
}P2P_PKT_HEAD_t;

struct VIDEO_BUF_HEAD
{
    unsigned int head; /* 头，必须等于0xFF00FF */
    unsigned int timestamp; //时间戳，如果是录像，则填录像的时间戳，如果是实时视频，则为0
    unsigned int len;    /*长度*/
    unsigned char frametype;
	unsigned char streamid;
	unsigned char sessid;
	unsigned char version;
	unsigned int frameno; /*帧数*/
	unsigned short milistamp;/*2014.4.24新增，实时视频时间戳，单位毫秒*/	
};

#define SEPCAM_MOST_STR_VAR_LEN 32
#define MSG_HARD_BUF_LEN (8*1024)
#define MSG_START_SIGNATURE_LEN 4
#define MSG_START_SIGNATURE 0x55aabbcc
#define MAX_FRAME_LENGTH 2097152

//设备端和PC端通讯协议<H>//protocol:协议
typedef struct MSG_HEAD
{
	T_U32 msg_signature;
    T_U16  msg_type;
    T_U16  ext_msg_type;
    T_U32  guid;
    T_U32  result;//0表示成功，其他的错误从1开始
    T_U32  version;//(V 1.0.0)
    
    T_U32  msg_head_len;//sizeof(msg_head) + 扩展长度）;
    T_U32  msg_len;//(msg_head_len + 实际数据长度 )；
    T_U32  timestamp;
    T_U32 sequence;//序列号
    
    T_S8   dev_id[SEPCAM_MOST_STR_VAR_LEN];

    T_S8  tlv_num;	//如果是TLV结构的，此字段为TLV个数，否则保留不用
    T_S8  res[7];
}MSG_HEAD_t;

void SEPCAM_BuildMsgHdReq(void *buf, unsigned CmdType, unsigned guid, unsigned BodyLen, unsigned Sequence, char *DevID);
int BuildLoginReqMsg(char *pMsg, char *sUserName, char *sPassword);

class MSG_LOGIN_t
{
public:
	String user;//用户名称
	String passwd;//md5加密后的密码
	int interval;//心跳间隔时间
	int parseJSON(JSONObject &jsdata){
	    int ret=1;
		JSONObject *jsroot= jsdata.getJSONObject("MSG_LOGIN_t");
		ret &= jsroot->getString("user",user);
		ret &= jsroot->getString("passwd",passwd);
		ret &= jsroot->getInt("interval",interval);
		delete jsroot;
	    return ret;
	}
	int toJSONString(String&str){
		JSONObject jsroot;// = new JSONObject();

		JSONObject jsethnetwork;// = new JSONObject();
		jsethnetwork.put("user", user);
		jsethnetwork.put("passwd", passwd);
		jsethnetwork.put("interval", interval);
		jsroot.put("MSG_LOGIN_t", jsethnetwork);
		jsroot.toString(str);
		return str.length();
	}
};

typedef struct REQUEST_STREAM_CMD
{
	T_U8   channel;        //请求的通道0
	T_U8	 flag;			 //0：主码流，1：副码流, 2: 只做音频
	T_U8   stream_type;    //帧类型：音频流.视频流，复合码流1
	T_U8   cmd_type;       //命令类型：开始发送,结束发送1
	T_U32    reserve;        //保留字段
	boolean parseJSON(JSONObject jsdata){
	    int ret=1;
		int value;
		JSONObject *jsroot= jsdata.getJSONObject("REQUEST_STREAM_CMD_t");
		ret &= jsroot->getInt("channel",value);channel=value;
		ret &= jsroot->getInt("flag",value);flag=value;
		ret &= jsroot->getInt("stream_type",value);stream_type=value;
		ret &= jsroot->getInt("cmd_type",value);cmd_type=value;
		ret &= jsroot->getInt("reserve",value);reserve=value;
		
		delete jsroot;
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;// = new JSONObject();

		JSONObject jsrsc;
		jsrsc.put("channel", (int)channel);
		jsrsc.put("flag", (int)flag);
		jsrsc.put("stream_type", (int)stream_type);
		jsrsc.put("cmd_type", (int)cmd_type);
		jsrsc.put("reserve", (int)reserve);
		jsroot.put("REQUEST_STREAM_CMD_t", jsrsc);
		jsroot.toString(str);
		return str.length();
	}
}REQUEST_STREAM_CMD_t;

typedef struct IPCNetCamColorCfg{
	boolean SetDefault;    //是否设置默认值
	int ViCh;//max=3
	int Brightness;//max=255 def:0
	int Chroma;//max=255 def:0
	int Contrast;//max=255 def:0x80
	    
	int Saturtion;//max=255 def:0
	int Acutance;//max=255 def:0
	IPCNetCamColorCfg(){
		SetDefault = false;
		ViCh=0;//max=3
		Brightness=-1;//max=255 def:0
		Chroma=-1;//max=255 def:0
		Contrast=-1;//max=255 def:0x80
	    
		Saturtion=-1;//max=255 def:0
		Acutance=-1;//max=255 def:0
	}
	boolean parseJSON(JSONObject &jsdata){
		JSONObject *jsroot= jsdata.getJSONObject("CamCfg.info");
		jsroot->getInt("ViCh",ViCh);
		jsroot->getInt("Brightness",Brightness);
		jsroot->getInt("Chroma",Chroma);
		jsroot->getInt("Contrast",Contrast);
		jsroot->getInt("Saturtion",Saturtion);
		jsroot->getInt("Acutance",Acutance);
		
		delete jsroot;
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;
		
		JSONObject jsresult;
		jsresult.putBoolean("SetDefault",SetDefault);
		jsresult.put("ViCh",ViCh);
		jsresult.put("Brightness",Brightness);
		jsresult.put("Chroma",Chroma);
		jsresult.put("Contrast",Contrast);
		jsresult.put("Saturtion",Saturtion);
		jsresult.put("Acutance",Acutance);

		jsroot.put("CamCfg.info", jsresult);
		
		jsroot.toString(str);
			
		return str.length();
	}

	bool isValueValid(){
		return Acutance>=0 && Brightness>=0 && Chroma>=0 && 
			Contrast>=0 && Saturtion>=0 && Acutance>=0;
	}
}IPCNetCamColorCfg_st;

typedef struct IPCNetPicOverTurn{
	int ViCh;//max=3
	int Mirror;
	int Flip;

	IPCNetPicOverTurn(){
		ViCh=0;//max=3
	}
	boolean parseJSON(JSONObject &jsdata){
		JSONObject *jsroot= jsdata.getJSONObject("IspOverTurn.info");

		if(jsroot){
		jsroot->getInt("ViCh",ViCh);
		jsroot->getInt("Mirror",Mirror);
		jsroot->getInt("Flip",Flip);
		
		delete jsroot;
		}
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;
		
		JSONObject jsresult;
		jsresult.put("ViCh",ViCh);
		jsresult.put("Flip",Flip);
		jsresult.put("Mirror",Mirror);

		jsroot.put("IspOverTurn.info", jsresult);
		
		jsroot.toString(str);
			
		return str.length();
	}
}IPCNetPicOverTurn_st;
typedef struct IPCNetTime_st{
	int Hour;
	int Min;
	int Sec;
}IPCNetTime_st;
typedef struct IPCNetDate_st{
	int Year;
	int Mon;
	int Day;
}IPCNetDate_st;
typedef struct IPCNetTimeCfg{
	IPCNetDate_st *Date;
	IPCNetTime_st *Time;
	boolean NtpEnable;
	String NtpServ;
	int TimeZone;
	IPCNetTimeCfg(){
		Date = new IPCNetDate_st();
		Time = new IPCNetTime_st();
	}
	~IPCNetTimeCfg(){
		delete Date;
		delete Time;
	}
	boolean parseJSON(JSONObject &jsdata){
		int value;
		JSONObject *jsroot= jsdata.getJSONObject("Time.Conf");
		if(jsroot){
			bool bv;
			jsroot->getBoolean("NtpEnable",bv);
			NtpEnable = bv;
			jsroot->getString("NtpServ",NtpServ);
			jsroot->getInt("TimeZone",value);
			TimeZone = value;
					
			JSONObject *jsTime = jsroot->getJSONObject("Time");
			jsTime->getInt("Hour",value);
			Time->Hour = value;
			jsTime->getInt("Min",value);
			Time->Min = value;
			jsTime->getInt("Sec",value);
			Time->Sec = value;
					
			JSONObject *info = jsroot->getJSONObject("Date");
			info->getInt("Day",value);
			Date->Day = value;
			info->getInt("Mon",value);
			Date->Mon = value;
			info->getInt("Year",value);
			Date->Year = value;

			delete jsroot;
		}
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;

		JSONObject jsresult;
				
		jsresult.putBoolean("NtpEnable", NtpEnable);
		jsresult.put("NtpServ", NtpServ);
		jsresult.put("TimeZone",TimeZone);
				
		JSONObject jsTime;
		jsTime.put("Hour", Time->Hour);
		jsTime.put("Min", Time->Min);
		jsTime.put("Sec", Time->Sec);
		jsresult.put("Time", jsTime);
				
		JSONObject info;
		info.put("Day",Date->Day);
		info.put("Mon",Date->Mon);
		info.put("Year",Date->Year);
		jsresult.put("Date", info);
				
		jsroot.put("Time.Conf", jsresult);
		
		jsroot.toString(str);
		return str.length();
	}
}IPCNetTimeCfg_st;
typedef struct IPCNetRecordGetCfg{
	int ViCh;//sensor index.
	String Path;
	boolean parseJSON(JSONObject &jsdata){
	    JSONObject *jsroot= jsdata.getJSONObject("Rec.Conf");
		if(jsroot!=0){
			jsroot->getInt("ViCh",ViCh);
			jsroot->getString("Path",Path);

			delete jsroot;
		}
		
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;// = new JSONObject();

		JSONObject jresult;// = new JSONObject();
		jresult.put("ViCh", ViCh);
		jresult.put("Path", Path);
				
		jsroot.put("Rec.Conf", jresult);
		
		jsroot.toString(str);
		return str.length();
	}
}IPCNetRecordGetCfg_st;
typedef struct DiskInfo{
	unsigned int Total;
	unsigned int Free;
	String Path;
	bool isValid;
	int Type;
}DiskInfo_st;
typedef struct RecTime{
	RecTime(){
		En=0;
		St1="00:00:00";
		Ed1="23:59:59";
		St2="00:00:00";
		Ed2="23:59:59";
	}
	//startx->"00:00:00",endx->"23:59:59"
	//You can set any piece of time into this field, such as startx->"09:00:00",endx->"21:00:00"
	//startx is lower then endx
	int En;//enable
	String St1;//start 1
	String Ed1;//end 1
	String St2;
	String Ed2;
}RecTime_st;

//VeCh: record main(0) stream or sub(1) stream
//RecMins: record duration, please pick up one of the time from the RecMinsOption in the json struct feedback from OnCmdResult_t which can trigger by IPCNetGetRecordR
//ReserveSize:how many disk size would you want to reserve
typedef struct IPCNetRecordCfg{
	int ViCh;//sensor index.
	DiskInfo_st *DiskInfo;
	int RecMinsOptionNum;
	int RecMinsOption[32];
	int RecMins;//set with index of RecMinsOption
	bool AutoDel;
	int VeCh;
	RecTime_st *RecTime[8];
	int PackageType;
	int ReserveSize;
	IPCNetRecordCfg(){
		int i;
		DiskInfo=new DiskInfo_st;
		for(i=0;i<8;i++){
			RecTime[i]=new RecTime_st;
		}
	}
	~IPCNetRecordCfg(){
		int i;
		delete DiskInfo;
		for(i=0;i<8;i++){
			delete RecTime[i];
		}
	}
	boolean parseJSON(JSONObject &jsdata){
	    JSONObject *jsroot= jsdata.getJSONObject("Rec.Conf");
		if(jsroot!=0){
			int value;
			jsroot->getInt("ViCh",ViCh);
			JSONObject *jsDiskInfo=jsroot->getJSONObject("DiskInfo");
			if(jsDiskInfo){
				jsDiskInfo->getInt("Free",value);
				DiskInfo->Free=value;
				jsDiskInfo->getInt("Total",value);
				DiskInfo->Total=value;
				jsDiskInfo->getString("Path",DiskInfo->Path);

				jsDiskInfo->getBoolean("isValid",DiskInfo->isValid);

				jsDiskInfo->getInt("Type",DiskInfo->Type);

				delete jsDiskInfo;
			}
			jsroot->getInt("RecMins",value);
			RecMins=value;
			JSONArray*jsaRecMinsOption=jsroot->getJSONArray("RecMinsOption");
			if(jsaRecMinsOption){
				int rl=jsaRecMinsOption->getLength();
				RecMinsOptionNum=rl;
				rl=rl>32?32:rl;
				for(int i=0;i<rl;i++){
					jsaRecMinsOption->getInt(i,value);
					RecMinsOption[i]=value;
				}

				delete jsaRecMinsOption;
			}
			jsroot->getInt("VeCh",VeCh);
			jsroot->getBoolean("AutoDel",AutoDel);
			jsroot->getInt("PackageType",PackageType);
			jsroot->getInt("ReserveSize",ReserveSize);

			JSONArray*jsaRecTime=jsroot->getJSONArray("RecTime");
			if(jsaRecTime){
				int i;
				for(i=0;i<jsaRecTime->getLength();i++){
					JSONObject*jsRt=jsaRecTime->getJSONObject(i);
					if(jsRt){
						jsRt->getInt("En",RecTime[i]->En);
						jsRt->getString("St1",RecTime[i]->St1);
						jsRt->getString("Ed1",RecTime[i]->Ed1);
						jsRt->getString("St2",RecTime[i]->St2);
						jsRt->getString("Ed2",RecTime[i]->Ed2);
						delete jsRt;
					}
				}
				delete jsaRecTime;
			}

			delete jsroot;
		}
		
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;

		JSONObject jresult;
		jresult.put("ViCh", ViCh);
		jresult.putBoolean("AutoDel", AutoDel);
		jresult.put("PackageType", PackageType);
		jresult.put("ReserveSize", ReserveSize);
		jresult.put("VeCh", VeCh);
		jresult.put("RecMins", RecMins);
		JSONObject jsDiskInfo;
		//no need
		jresult.put("DiskInfo",jsDiskInfo);
		JSONArray jsaRecMinsOption;
		//no need
		jresult.put("RecMinsOption",jsaRecMinsOption);

		JSONArray jsaRecTime;
		int i;
		for(i=0;i<8;i++){
			JSONObject jsRt;
			jsRt.put("En",RecTime[i]->En);
			jsRt.put("St1",RecTime[i]->St1);
			jsRt.put("Ed1",RecTime[i]->Ed1);
			jsRt.put("St2",RecTime[i]->St2);
			jsRt.put("Ed2",RecTime[i]->Ed2);
			jsaRecTime.put(i,jsRt);
		}
		jresult.put("RecTime",jsaRecTime);
				
		jsroot.put("Rec.Conf", jresult);
		
		jsroot.toString(str);
		return str.length();
	}
}IPCNetRecordCfg_st;

typedef struct IPCNetEthConfig{
	boolean DhcpEnble;
	String IP;
	String Netmask;
	String Getway;
	String DNS1;
	String DNS2;
	boolean parseJSON(JSONObject &jsdata){

		JSONObject *jsroot = jsdata.getJSONObject("NetWork.Eth");
		if(jsroot){
			bool dhcpenale;
			jsroot->getBoolean("DhcpEnble",dhcpenale);
			DhcpEnble=dhcpenale;
			jsroot->getString("IP",IP);
			jsroot->getString("Netmask",Netmask);
			jsroot->getString("Getway",Getway);
			jsroot->getString("DNS1",DNS1);
			jsroot->getString("DNS2",DNS2);

			delete jsroot;
		}else{
			return false;
		}
		
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;// = new JSONObject();
		
		JSONObject jsethnetwork;// = new JSONObject();
		jsethnetwork.putBoolean("DhcpEnble", DhcpEnble);
		jsethnetwork.put("IP", IP);
		jsethnetwork.put("Netmask", Netmask);
		jsethnetwork.put("Getway", Getway);
		jsethnetwork.put("DNS1", DNS1);
		jsethnetwork.put("DNS2", DNS2);
		jsroot.put("NetWork.Eth", jsethnetwork);
		
		jsroot.toString(str);
		return str.length();
	}
}IPCNetEthConfig_st;
typedef struct IPCNetWirelessConfig{
	boolean WirelessEnable;
	int WirelessStatus;
	String SsidSetMode;
	String EncType;
	String SSID;
	String Password;
	boolean DhcpEnble;
	String IP;
	String Netmask;
	String Getway;
	boolean parseJSON(JSONObject &jsdata){
	   
		JSONObject *jsroot= jsdata.getJSONObject("NetWork.Wireless");
		if(jsroot){
			bool wirelessenale;
			jsroot->getBoolean("WirelessEnable",wirelessenale);
			WirelessEnable=wirelessenale;
			jsroot->getInt("WirelessStatus",WirelessStatus);
			jsroot->getString("EncType",EncType);
			jsroot->getString("SSID",SSID);
			jsroot->getString("Password",Password);

			bool dhcpenale;
			jsroot->getBoolean("DhcpEnble",dhcpenale);
			DhcpEnble=dhcpenale;
			jsroot->getString("IP",IP);
			jsroot->getString("Netmask",Netmask);
			jsroot->getString("Getway",Getway);

			jsroot->getString("SsidSetMode",SsidSetMode);

			delete jsroot;
		}else{
			return false;
		}
		
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;// = new JSONObject();
		
		JSONObject jsresult;// = new JSONObject();
		jsresult.put("WirelessEnable", WirelessEnable);
		jsresult.put("WirelessStatus", WirelessStatus);
		jsresult.put("SsidSetMode", SsidSetMode);
		jsresult.put("EncType", EncType);
		jsresult.put("SSID", SSID);
		jsresult.put("Password", Password);
		jsresult.put("DhcpEnble", DhcpEnble);
		jsresult.put("IP", IP);
		jsresult.put("Netmask", Netmask);
		jsresult.put("Getway", Getway);
		jsroot.put("NetWork.Wireless", jsresult);
		
		jsroot.toString(str);
		return str.length();
	}
}IPCNetWirelessConfig_st;
typedef struct IPCNetMobileNetworkInfo{
	boolean enable;	//使能3g模块，0-不生效，1-生效
	int type;		// 3G模块类型，0-电信evdo,1-联通wcdma
	String ip;	// 3G模块获取的IP地址
	String vpn;
	String name;
	boolean parseJSON(JSONObject &jsdata){
	    
		JSONObject *jsroot= jsdata.getJSONObject("Net.Mobile");
		if(jsroot){
			jsroot->getInt("type",type);
			bool bv;
			jsroot->getBoolean("enable",bv);
			enable = bv;
			jsroot->getString("ip",ip);
			jsroot->getString("vpn",vpn);
			jsroot->getString("name",name);
			delete jsroot;
		}
		
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;// = new JSONObject();
		
			JSONObject jsresult;// = new JSONObject();
			jsresult.put("type", type);
			jsresult.put("enable", enable);
			jsresult.put("ip", ip);
			jsresult.put("name", name);
			jsresult.put("vpn", vpn);
			jsroot.put("Net.Mobile", jsresult);
		
		jsroot.toString(str);
		return str.length();
	}
}IPCNetMobileNetworkInfo_st;
typedef struct IPCNetWifiApItem{
	/*IPCNetWifiApItem(){
		SSID=0;
		EncType=0;
	}
	~IPCNetWifiApItem(){
		if(SSID){
			delete SSID;
		}
		if(EncType){
			delete EncType;
		}
	}
	char* SSID;
	char* EncType;*/
	String SSID;
	String EncType;
	int RSSI;
}IPCNetWifiApItem_st;
typedef struct IPCNetWifiAplist{
	IPCNetWifiApItem_st **ApItem;// = new IPCNetWifiApItem[1];
	int num;
	IPCNetWifiAplist(){
		ApItem=0;
		num=0;
	}
	~IPCNetWifiAplist(){
		if(ApItem){
			for(int i=0;i<num;i++){
				delete ApItem[i];
			}
			delete[] ApItem;
		}
	}
	boolean parseJSON(JSONObject &jsdata){
	    
			JSONObject *jsroot= jsdata.getJSONObject("NetWork.WirelessSearch");
			if(jsroot){
				JSONArray *Aplist = jsroot->getJSONArray("Aplist");
				if(Aplist){
					int apcount = Aplist->getLength();
					num=apcount;
					//Log.d(TAG,"ap num:" + apcount);
					ApItem = new IPCNetWifiApItem_st*[apcount];

					for(int i=0;i<apcount;i++){
						ApItem[i] = new IPCNetWifiApItem_st;
						JSONObject *ap = Aplist->getJSONObject(i);
						if(ap){
							//IPCNetWifiApItem_st *apitem = new IPCNetWifiApItem();
							ap->getString("SSID",ApItem[i]->SSID);
							ap->getString("EncType",ApItem[i]->EncType);
							ap->getInt("RSSI",ApItem[i]->RSSI);
							//ApItem[i] = apitem;
							delete ap;
						}
					}
					delete Aplist;
				}
				delete jsroot;
			}
		
	    return true;
	}
	int toJSONString(String&str){
		str="";
		return 0;
	}
}IPCNetWifiAplist_st;

typedef struct IPCNetOsdConf_st{
	boolean Enable;       //是否启用
	int DisplayMode;  //字体与背景是否反色，0-不反色，1-反色
	int Xcord;        //x坐标0-704
	int Ycord;        //y坐标0-576
}IPCNetOsdConf_st;

typedef struct IPCNetOsdName_st{
	String NameText;
	IPCNetOsdConf_st *NameConf;
	IPCNetOsdName_st(){
		NameText="";
		NameConf = new IPCNetOsdConf_st();
	}
	~IPCNetOsdName_st(){
		delete NameConf;
	}
}IPCNetOsdName_st;

typedef struct IPCNetOsdCfg_st{
	boolean SetDefault;
	int Vich;
	IPCNetOsdName_st *OsdNameInfo;
	IPCNetOsdConf_st *OsdDateInfo;
	IPCNetOsdConf_st *OsdRateInfo;
	IPCNetOsdCfg_st(){
		OsdNameInfo = new IPCNetOsdName_st();
		OsdDateInfo = new IPCNetOsdConf_st();
		OsdRateInfo = new IPCNetOsdConf_st();
	}
	~IPCNetOsdCfg_st(){
		delete OsdNameInfo;
		delete OsdDateInfo;
		delete OsdRateInfo;
	}
	boolean parseJSON(JSONObject &jsdata){
		JSONObject *jsroot = jsdata.getJSONObject("OsdCfg.info");
		if(jsroot){
			bool sd;
			jsroot->getBoolean("SetDefault",sd);
			SetDefault=sd;
			jsroot->getInt("Vich",Vich);
					
			JSONObject *jsOsdNameInfo = jsroot->getJSONObject("OsdNameInfo");
			jsOsdNameInfo->getString("NameText",OsdNameInfo->NameText);

			JSONObject *jsOsdNameInfo_NameConf = jsOsdNameInfo->getJSONObject("NameConf");
			jsOsdNameInfo_NameConf->getBoolean("Enable",sd);       //是否启用
			OsdNameInfo->NameConf->Enable=sd;
			jsOsdNameInfo_NameConf->getInt("DisplayMode",OsdNameInfo->NameConf->DisplayMode);  //字体与背景是否反色，0-不反钯，1-反色
			jsOsdNameInfo_NameConf->getInt("Xcord",OsdNameInfo->NameConf->Xcord);        //x坐标0-704
			jsOsdNameInfo_NameConf->getInt("Ycord",OsdNameInfo->NameConf->Ycord);
				    
			JSONObject *jsOsdDateInfo = jsroot->getJSONObject("OsdDateInfo");
			jsOsdDateInfo->getBoolean("Enable",sd);       //是否启用
				OsdDateInfo->Enable=sd;
			jsOsdDateInfo->getInt("DisplayMode",OsdDateInfo->DisplayMode);  //字体与背景是否反色，0-不反钯，1-反色
			jsOsdDateInfo->getInt("Xcord",OsdDateInfo->Xcord);        //x坐标0-704
			jsOsdDateInfo->getInt("Ycord",OsdDateInfo->Ycord);
					
			JSONObject *jsOsdRateInfo = jsroot->getJSONObject("OsdRateInfo");
			jsOsdRateInfo->getBoolean("Enable",sd);       //是否启用
			OsdRateInfo->Enable = sd;
			jsOsdRateInfo->getInt("DisplayMode",OsdRateInfo->DisplayMode);  //字体与背景是否反色，0-不反钯，1-反色
			jsOsdRateInfo->getInt("Xcord",OsdRateInfo->Xcord);        //x坐标0-704
			jsOsdRateInfo->getInt("Ycord",OsdRateInfo->Ycord);
		}else{
			return false;
		}

	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;// = new JSONObject();
		
		JSONObject jsresult;// = new JSONObject();
		jsresult.put("Vich", Vich);
		jsresult.putBoolean("SetDefault",SetDefault);
				
		JSONObject jsOsdNameInfo;// = new JSONObject();

			jsOsdNameInfo.put("NameText",OsdNameInfo->NameText);
			
		JSONObject jsOsdNameInfo_NameConf;// = new JSONObject();
		jsOsdNameInfo_NameConf.putBoolean("Enable",OsdNameInfo->NameConf->Enable);       //是否启用
		jsOsdNameInfo_NameConf.put("DisplayMode",OsdNameInfo->NameConf->DisplayMode);  //字体与背景是否反色，0-不反钯，1-反色
		jsOsdNameInfo_NameConf.put("Xcord",OsdNameInfo->NameConf->Xcord);        //x坐标0-704
		jsOsdNameInfo_NameConf.put("Ycord",OsdNameInfo->NameConf->Ycord);
		jsOsdNameInfo.put("NameConf",jsOsdNameInfo_NameConf);
				
		jsresult.put("OsdNameInfo",jsOsdNameInfo);
				
		JSONObject jsOsdDateInfo;// = new JSONObject();
		jsOsdDateInfo.putBoolean("Enable",OsdDateInfo->Enable);       //是否启用
		jsOsdDateInfo.put("DisplayMode",OsdDateInfo->DisplayMode);  //字体与背景是否反色，0-不反钯，1-反色
		jsOsdDateInfo.put("Xcord",OsdDateInfo->Xcord);        //x坐标0-704
		jsOsdDateInfo.put("Ycord",OsdDateInfo->Ycord);
		jsresult.put("OsdDateInfo",jsOsdDateInfo);
				
		JSONObject jsOsdRateInfo;// = new JSONObject();
		jsOsdRateInfo.putBoolean("Enable",OsdRateInfo->Enable);       //是否启用
		jsOsdRateInfo.put("DisplayMode",OsdRateInfo->DisplayMode);  //字体与背景是否反色，0-不反钯，1-反色
		jsOsdRateInfo.put("Xcord",OsdRateInfo->Xcord);        //x坐标0-704
		jsOsdRateInfo.put("Ycord",OsdRateInfo->Ycord);
		jsresult.put("OsdRateInfo",jsOsdRateInfo);
				
		jsroot.put("OsdCfg.info", jsresult);

		jsroot.toString(str);
		return str.length();
	}
}IPCNetOsdCfg_st;

typedef struct
{
	int Vich;
	int toJSONString(String&str){
		JSONObject jsroot;// = new JSONObject();
		
		JSONObject jsresult;
		jsresult.put("Vich",Vich);

		jsroot.put("OsdGetCfg.info", jsresult);

		jsroot.toString(str);
		return str.length();
	}
}IPCNetGetOsdCfg_st;

typedef struct
{
	int Witdh;
	int Height;
}IPCNetVideoResolutionOpt_st;
typedef struct IPCNetVideoEncode{
	IPCNetVideoEncode(){
		VideoResolutionOpt=0;
		OpionEncode=0;
		ProfileTypeOption=0;
		VideoResolutionOptNum=0;
		OpionEncodeNum=0;
	}
	~IPCNetVideoEncode(){
		if(VideoResolutionOpt){
			for(int i=0;i<VideoResolutionOptNum;i++){
				delete VideoResolutionOpt[i];
			}
			delete[] VideoResolutionOpt;
		}
		if(OpionEncode){
			for(int i=0;i<OpionEncodeNum;i++){
				delete OpionEncode[i];
			}
			delete[] OpionEncode;
		}
		if(ProfileTypeOption){
			for(int i=0;i<ProfileTypeOptionNum;i++){
				delete ProfileTypeOption[i];
			}
			delete[] ProfileTypeOption;
		}
	}
	int EncCh;
	String Encode;
	String **OpionEncode;
	int OpionEncodeNum;
	
	int Witdh;
	int Height;
	IPCNetVideoResolutionOpt_st **VideoResolutionOpt;
	int VideoResolutionOptNum;
	
	int IFrame;
	int MaxIFrame;
	int MinIFrame;
	
	int FrameRate;
	int MaxFrameRate;
	int MinFrameRate;

    int RateCtrl; // CBR:1不可变码流,0:VBR可变码流
    int Bitrate;  //码率 16K-20M BIT/S为单位

	String ProfileType;
	String **ProfileTypeOption;
	int ProfileTypeOptionNum;
}IPCNetVideoEncode_st;
typedef struct IPCNetVideoEncodeCfg{
	int ViCh;
	int VideoEncodeNum;
	IPCNetVideoEncode_st **VideoEncode;
	IPCNetVideoEncodeCfg(){
		VideoEncode=0;
		VideoEncodeNum=0;
	}
	~IPCNetVideoEncodeCfg(){
		if(VideoEncode){
			for(int i=0;i<VideoEncodeNum;i++){
				delete VideoEncode[i];
			}
			delete[] VideoEncode;
			VideoEncode=0;
		}
	}
	boolean parseJSON(JSONObject &jsdata){
	    
		JSONObject *jsroot= jsdata.getJSONObject("AVEnc.VideoEncode");
		if(jsroot){
			jsroot->getInt("ViCh",ViCh);
			jsroot->getInt("VideoEncodeNum",VideoEncodeNum);
			JSONArray*jaVideoEncode=jsroot->getJSONArray("VideoEncode");
			if(jaVideoEncode){
				VideoEncodeNum=jaVideoEncode->getLength();
				VideoEncode=new IPCNetVideoEncode_st*[VideoEncodeNum];
				for(int i=0;i<VideoEncodeNum;i++){
					VideoEncode[i]=new IPCNetVideoEncode_st();
					JSONObject *jsve=jaVideoEncode->getJSONObject(i);
					if(jsve){
						jsve->getInt("EncCh",VideoEncode[i]->EncCh);
						jsve->getString("Encode",VideoEncode[i]->Encode);
						//Log("%s:%d\n",__FUNCTION__,__LINE__);
						JSONArray*jaoe=jsve->getJSONArray("OpionEncode");
						//Log("%s:%d\n",__FUNCTION__,__LINE__);
						if(jaoe){
							VideoEncode[i]->OpionEncodeNum=jaoe->getLength();
							VideoEncode[i]->OpionEncode=new String*[VideoEncode[i]->OpionEncodeNum];
							//Log("%s:%d\n",__FUNCTION__,__LINE__);
							for(int j=0;j<VideoEncode[i]->OpionEncodeNum;j++){
								VideoEncode[i]->OpionEncode[j]=new String;
								jaoe->getString(j,*VideoEncode[i]->OpionEncode[j]);
							}
							delete jaoe;
						}
						jsve->getInt("Witdh",VideoEncode[i]->Witdh);
						jsve->getInt("Height",VideoEncode[i]->Height);
						JSONArray*javro=jsve->getJSONArray("VideoResolutionOpt");
						if(javro){
							VideoEncode[i]->VideoResolutionOptNum=javro->getLength();
							VideoEncode[i]->VideoResolutionOpt=new IPCNetVideoResolutionOpt_st*[VideoEncode[i]->VideoResolutionOptNum];
							for(int j=0;j<VideoEncode[i]->VideoResolutionOptNum;j++){
								VideoEncode[i]->VideoResolutionOpt[j]=new IPCNetVideoResolutionOpt_st;
								JSONObject *jsvro=javro->getJSONObject(j);
								if(jsvro){
									jsvro->getInt("Witdh",VideoEncode[i]->VideoResolutionOpt[j]->Witdh);
									jsvro->getInt("Height",VideoEncode[i]->VideoResolutionOpt[j]->Height);
								}
							}
							delete javro;
						}
						jsve->getInt("IFrame",VideoEncode[i]->IFrame);
						jsve->getInt("MaxIFrame",VideoEncode[i]->MaxIFrame);
						jsve->getInt("MinIFrame",VideoEncode[i]->MinIFrame);
						jsve->getInt("FrameRate",VideoEncode[i]->FrameRate);
						jsve->getInt("MaxFrameRate",VideoEncode[i]->MaxFrameRate);
						jsve->getInt("MinFrameRate",VideoEncode[i]->MinFrameRate);
						jsve->getInt("RateCtrl",VideoEncode[i]->RateCtrl);
						jsve->getInt("Bitrate",VideoEncode[i]->Bitrate);

						delete jsve;
					}
				}
				delete jaVideoEncode;
			}

			delete jsroot;
		}
		
	    return true;
	}
	int toJSONString(String&str){
		JSONObject jsroot;
		
			JSONObject jsresult;
			jsresult.put("ViCh", ViCh);
			jsresult.put("VideoEncodeNum", VideoEncodeNum);
			if(VideoEncode==0){
				VideoEncode=new IPCNetVideoEncode_st*[VideoEncodeNum];
				for(int i=0;i<VideoEncodeNum;i++){
					VideoEncode[i]=new IPCNetVideoEncode_st;
				}
			}
			JSONArray jaVideoEncode;
			for(int i=0;i<VideoEncodeNum;i++){
				JSONObject jsVideoEncode;
				jsVideoEncode.put("EncCh",VideoEncode[i]->EncCh);
				jsVideoEncode.put("Encode",VideoEncode[i]->Encode);
				if(VideoEncode[i]->OpionEncode==0){
					VideoEncode[i]->OpionEncode=new String*[VideoEncode[i]->OpionEncodeNum];
					for(int k=0;k<VideoEncode[i]->OpionEncodeNum;k++){
						VideoEncode[i]->OpionEncode[k]=new String;
					}
				}
				JSONArray jaEncCh;
				for(int j=0;j<VideoEncode[i]->OpionEncodeNum;j++){
					jaEncCh.put(j,*VideoEncode[i]->OpionEncode[j]);
				}
				jsVideoEncode.put("OpionEncode",jaEncCh);
				jsVideoEncode.put("Witdh",VideoEncode[i]->Witdh);
				jsVideoEncode.put("Height",VideoEncode[i]->Height);
				if(VideoEncode[i]->VideoResolutionOpt==0){
					VideoEncode[i]->VideoResolutionOpt=new IPCNetVideoResolutionOpt_st*[VideoEncode[i]->VideoResolutionOptNum];
					for(int k=0;k<VideoEncode[i]->VideoResolutionOptNum;k++){
						VideoEncode[i]->VideoResolutionOpt[k]=new IPCNetVideoResolutionOpt_st;
					}
				}
				JSONArray jaVideoResolutionOpt;
				for(int j=0;j<VideoEncode[i]->VideoResolutionOptNum;j++){
					JSONObject jsvro;
					jsvro.put("Witdh",VideoEncode[i]->VideoResolutionOpt[j]->Witdh);
					jsvro.put("Height",VideoEncode[i]->VideoResolutionOpt[j]->Height);
					jaVideoResolutionOpt.put(j,jsvro);
				}
				jsVideoEncode.put("VideoResolutionOpt",jaVideoResolutionOpt);
				jsVideoEncode.put("IFrame",VideoEncode[i]->IFrame);
				jsVideoEncode.put("MaxIFrame",VideoEncode[i]->MaxIFrame);
				jsVideoEncode.put("MinIFrame",VideoEncode[i]->MinIFrame);
				jsVideoEncode.put("FrameRate",VideoEncode[i]->FrameRate);
				jsVideoEncode.put("MaxFrameRate",VideoEncode[i]->MaxFrameRate);
				jsVideoEncode.put("MinFrameRate",VideoEncode[i]->MinFrameRate);
				jsVideoEncode.put("RateCtrl",VideoEncode[i]->RateCtrl);
				jsVideoEncode.put("Bitrate",VideoEncode[i]->Bitrate);

				jaVideoEncode.put(i,jsVideoEncode);
			}
			jsresult.put("VideoEncode", jaVideoEncode);
		jsroot.put("AVEnc.VideoEncode", jsresult);
		
		jsroot.toString(str);
		return str.length();
	}
}IPCNetVideoEncodeCfg_st;


//{"IspPicColor.info":{"Type":2,"SwitchLevel":0,"SwitchTime":1,"TimerOpera":0,"Start":{"Hour":0,"Min":0,"Sec":0},"End":{"Hour":0,"Min":0,"Sec":0}},"ret":0}
typedef struct
{
	int Type;	//彩转黑类型

	//自动黑白-彩色转换有效
	int SwitchLevel;  //切换灵敏度，0-低，1-中，2-高
	int SwitchTime;	//切换时间，单位s

	//以下参数仅在定时彩转黑有效
	int TimerOpera;	//设定时间内的操作。0-彩色模式，1-黑白模式
	IPCNetTime_st Start;
	IPCNetTime_st End;

	boolean parseJSON(JSONObject &jsdata){
	    
		JSONObject *jsroot= jsdata.getJSONObject("IspPicColor.info");
		if(jsroot){
			jsroot->getInt("Type",Type);
			jsroot->getInt("SwitchLevel",SwitchLevel);
			jsroot->getInt("SwitchTime",SwitchTime);

			jsroot->getInt("TimerOpera",TimerOpera);

			JSONObject *jsstart=jsroot->getJSONObject("Start");
			if(jsstart){
				jsstart->getInt("Hour",Start.Hour);
				jsstart->getInt("Min",Start.Min);
				jsstart->getInt("Sec",Start.Sec);
				delete jsstart;
			}

			JSONObject *jsend=jsroot->getJSONObject("End");
			if(jsend){
				jsend->getInt("Hour",Start.Hour);
				jsend->getInt("Min",Start.Min);
				jsend->getInt("Sec",Start.Sec);
				delete jsend;
			}

			delete jsroot;
		}

		return true;
	}

	int toJSONString(String&str){
		JSONObject jsroot;
		JSONObject jsresult;
		jsresult.put("Type",Type);
		jsresult.put("SwitchLevel",SwitchLevel);
		jsresult.put("SwitchTime",SwitchTime);
		jsresult.put("TimerOpera",TimerOpera);

		JSONObject jsstart;
		jsstart.put("Hour",Start.Hour);
		jsstart.put("Min",Start.Min);
		jsstart.put("Sec",Start.Sec);
		jsresult.put("Start",jsstart);

		JSONObject jsend;
		jsend.put("Hour",Start.Hour);
		jsend.put("Min",Start.Min);
		jsend.put("Sec",Start.Sec);
		jsresult.put("End",jsend);

		jsroot.put("IspPicColor.info", jsresult);

		jsroot.toString(str);
		return str.length();
	}
}IPCNetPicColorInfo_st;

typedef struct IPCNetTimePeriod{
	IPCNetTime_st Start;
	IPCNetTime_st End;
}IPCNetTimePeriod_st;
typedef struct//布防时间段
IPCNetDefendPeriodInfo{
	IPCNetDefendPeriodInfo(){
		memset(TimePeriod,0,sizeof(TimePeriod));
	}
	~IPCNetDefendPeriodInfo(){
	}
	int  Flag;
	IPCNetTimePeriod_st *TimePeriod[2];
//		public int TimePeriodCount;
}IPCNetDefendPeriodInfo_st;
typedef struct IPCNetVideoMoveInfo{
	IPCNetVideoMoveInfo()
	{
	}
	~IPCNetVideoMoveInfo(){
	}
	int  MdEnable;
	int  Sensitive;  
//		public int  Columns;
//		public int  Rows;
	int  Area[256];//16x12, Z scan
	int  AreaCount;
}IPCNetVideoMoveInfo_st;
typedef struct{
	int   Delay;
	int   EmailEnable;
}JianleAlarmIoOutPutInfo_st;
typedef struct{
	int RecordTime;
	int FtpEnable;
	int EmailEnable;
}JianleAlarmRecInfo_st;
typedef struct{
	int PushEnable;
	int PictureNum;
	int Sec;
	int Msec;
	int FtpEnable;
	int EmailEnable;
	int CloudEnable;
	boolean parseJSON(JSONObject &jsdata){
		jsdata.getInt("PushEnable",PushEnable);
		jsdata.getInt("PictureNum",PictureNum);
		jsdata.getInt("Sec",Sec);
		jsdata.getInt("Msec",Msec);
		jsdata.getInt("FtpEnable",FtpEnable);
	    jsdata.getInt("EmailEnable",EmailEnable);
	    jsdata.getInt("CloudEnable",CloudEnable);
	    return true;
	}
}IPCNetAlarmSnapInfo_st;
typedef struct{
	int Delay;
	int EmailEnable;
	int AlarmLevel;
}IPCNetAlarmIoOutputInfo_st;
typedef struct{
	int RecEnable;
	int RecordTime;
	int FtpEnable;
	int EmailEnable;
	int CloudEnable;
	boolean parseJSON(JSONObject &jsdata){
	    jsdata.getInt("EmailEnable",EmailEnable);
		jsdata.getInt("RecordTime",RecordTime);
		jsdata.getInt("FtpEnable",FtpEnable);
		jsdata.getInt("CloudEnable",CloudEnable);
		jsdata.getInt("RecEnable",RecEnable);
	    return true;
	}
}IPCNetAlarmRecordInfo_st;

typedef struct {
	int PresetId;
	String PresetName;
}IPCNetAlarmPresetInfo_st;
typedef struct{
	IPCNetAlarmIoOutputInfo_st IoOutputInfo;
	IPCNetAlarmRecordInfo_st RecordInfo;
	IPCNetAlarmSnapInfo_st SnapInfo;
	IPCNetAlarmPresetInfo_st PresetInfo;
}IPCNetAlarmLinkagePolicyInfo_st;
typedef struct IPCNETMoveAlarmCfg{
	IPCNETMoveAlarmCfg(){
		memset(Week,0,sizeof(Week));
	}
	~IPCNETMoveAlarmCfg(){
	}
	int ViCh;
	IPCNetDefendPeriodInfo_st *Week[8];//Everyday,Weekday,Monday,Tuesday...
	IPCNetVideoMoveInfo_st MoveInfo;
	IPCNetAlarmLinkagePolicyInfo_st PolicyInfo;
	    
	boolean parseJSON(JSONObject &jsdata){
			JSONObject *jsroot= jsdata.getJSONObject("Alarm.MoveAlarm");
			if(jsroot){
				jsroot->getInt("ViCh",ViCh);
				JSONArray *jsWeek = jsroot->getJSONArray("Week");
				if(jsWeek){
					for(int i=0;i<jsWeek->getLength()&&i<8;i++){
						JSONObject *jsIPCNetDefendPeriodInfo_st = jsWeek->getJSONObject(i);
						if(jsIPCNetDefendPeriodInfo_st){
							Week[i] = new IPCNetDefendPeriodInfo_st();
							jsIPCNetDefendPeriodInfo_st->getInt("Flag",Week[i]->Flag);
							
							JSONArray *jsTimePeriod = jsIPCNetDefendPeriodInfo_st->getJSONArray("TimePeriod");
							if(jsTimePeriod){
								for(int j=0;j<jsTimePeriod->getLength()&&j<2;j++){
									JSONObject *jsIPCNetTimePeriod_st = jsTimePeriod->getJSONObject(j);
									if(jsIPCNetTimePeriod_st){
										Week[i]->TimePeriod[j] = new IPCNetTimePeriod_st();
										JSONObject *jsEnd = jsIPCNetTimePeriod_st->getJSONObject("End");
										if(jsEnd){
											jsEnd->getInt("Hour",Week[i]->TimePeriod[j]->End.Hour);
											jsEnd->getInt("Min",Week[i]->TimePeriod[j]->End.Min);
											jsEnd->getInt("Sec",Week[i]->TimePeriod[j]->End.Sec);
											delete jsEnd;
										}
										
										JSONObject *jsStart = jsIPCNetTimePeriod_st->getJSONObject("Start");
										if(jsStart){
											jsStart->getInt("Hour",Week[i]->TimePeriod[j]->Start.Hour);
											jsStart->getInt("Min",Week[i]->TimePeriod[j]->Start.Min);
											jsStart->getInt("Sec",Week[i]->TimePeriod[j]->Start.Sec);
											delete jsStart;
										}
										delete jsIPCNetTimePeriod_st;
									}
								}
								delete jsTimePeriod;
							}
						//Log("line:%d\n",__LINE__);
		//						Week[i].TimePeriodCount = jsIPCNetDefendPeriodInfo_st.getInt("TimePeriodCount");
							delete jsIPCNetDefendPeriodInfo_st;
						}
					}
					delete jsWeek;
				}
				
				JSONObject *jsMoveInfo = jsroot->getJSONObject("MoveInfo");
				if(jsMoveInfo){
//					MoveInfo.AreaCount = jsMoveInfo.getInt("AreaCount");
					jsMoveInfo->getInt("MdEnable",MoveInfo.MdEnable);
					jsMoveInfo->getInt("Sensitive",MoveInfo.Sensitive);
	//					MoveInfo.Columns = jsMoveInfo.getInt("Columns");
	//					MoveInfo.Rows = jsMoveInfo.getInt("Rows");
					JSONArray *jsArea = jsMoveInfo->getJSONArray("Area");
					if(jsArea){
						for(int i=0;i<jsArea->getLength()&&i<256;i++){
							jsArea->getInt(i,MoveInfo.Area[i]);
						}
						delete jsArea;
					}
					delete jsMoveInfo;
				}
				
				JSONObject *jsPolicyInfo = jsroot->getJSONObject("PolicyInfo");
				if(jsPolicyInfo){
					JSONObject *jsIoOutputInfo = jsPolicyInfo->getJSONObject("IoOutputInfo");
					if(jsIoOutputInfo){
						jsIoOutputInfo->getInt("Delay",PolicyInfo.IoOutputInfo.Delay);
						jsIoOutputInfo->getInt("EmailEnable",PolicyInfo.IoOutputInfo.EmailEnable);
						delete jsIoOutputInfo;
					}
					
					JSONObject *jsRecordInfo = jsPolicyInfo->getJSONObject("RecordInfo");
					if(jsRecordInfo){
						PolicyInfo.RecordInfo.parseJSON(*jsRecordInfo);
						delete jsRecordInfo;
					}
	//					PolicyInfo.RecordInfo.EmailEnable = jsRecordInfo.getInt("EmailEnable");
	//					PolicyInfo.RecordInfo.RecordTime = jsRecordInfo.getInt("RecordTime");
	//					PolicyInfo.RecordInfo.FtpEnable = jsRecordInfo.getInt("FtpEnable");
	//					PolicyInfo.RecordInfo.CloudEnable = jsRecordInfo.getInt("CloudEnable");
	//					PolicyInfo.RecordInfo.RecEnable = jsRecordInfo.getInt("RecEnable");
					
					JSONObject *jsSnapInfo = jsPolicyInfo->getJSONObject("SnapInfo");
					if(jsSnapInfo){
						PolicyInfo.SnapInfo.parseJSON(*jsSnapInfo);
						delete jsSnapInfo;
					}
	//					PolicyInfo.SnapInfo.EmailEnable = jsSnapInfo.getInt("EmailEnable");
	//					PolicyInfo.SnapInfo.PictureNum = jsSnapInfo.getInt("PictureNum");
	//					PolicyInfo.SnapInfo.Sec = jsSnapInfo.getInt("Sec");
	//					PolicyInfo.SnapInfo.Msec = jsSnapInfo.getInt("Msec");
	//					PolicyInfo.SnapInfo.FtpEnable = jsSnapInfo.getInt("FtpEnable");
	//					PolicyInfo.SnapInfo.CloudEnable = jsSnapInfo.getInt("CloudEnable");
					
					JSONObject *jsPresetInfo = jsPolicyInfo->getJSONObject("PresetInfo");
					if(jsPresetInfo){
						jsPresetInfo->getInt("PresetId",PolicyInfo.PresetInfo.PresetId);
						jsPresetInfo->getString("PresetName",PolicyInfo.PresetInfo.PresetName);
						delete jsPresetInfo;
					}
					
					delete jsPolicyInfo;
				}
			}
	    return true;
	}
	int toJSONString(String&str){
	    JSONObject jsresult;

			JSONObject jsroot;
			jsroot.put("ViCh",ViCh);
				
			JSONArray jsWeek;
			for(int i=0;i<8;i++){
				JSONObject jsIPCNetDefendPeriodInfo_st;
				if(Week[i]){
					jsIPCNetDefendPeriodInfo_st.put("Flag",Week[i]->Flag);
					
					JSONArray jsTimePeriod;
					for(int j=0;j<2;j++){
						JSONObject jsIPCNetTimePeriod_st;
						
						JSONObject jsEnd;
						if(Week[i]->TimePeriod[j]){
							jsEnd.put("Hour",Week[i]->TimePeriod[j]->End.Hour);
							jsEnd.put("Min",Week[i]->TimePeriod[j]->End.Min);
							jsEnd.put("Sec",Week[i]->TimePeriod[j]->End.Sec);
						}
						jsIPCNetTimePeriod_st.put("End", jsEnd);
						
						JSONObject jsStart;
						if(Week[i]->TimePeriod[j]){
							jsStart.put("Hour",Week[i]->TimePeriod[j]->Start.Hour);
							jsStart.put("Min",Week[i]->TimePeriod[j]->Start.Min);
							jsStart.put("Sec",Week[i]->TimePeriod[j]->Start.Sec);
						}
						jsIPCNetTimePeriod_st.put("Start", jsStart);

						jsTimePeriod.put(j,jsIPCNetTimePeriod_st);
					}
					jsIPCNetDefendPeriodInfo_st.put("TimePeriod", jsTimePeriod);
//					jsIPCNetDefendPeriodInfo_st.put("TimePeriodCount",Week[i].TimePeriodCount);
				}
				jsWeek.put(i,jsIPCNetDefendPeriodInfo_st);
			}
			jsroot.put("Week", jsWeek);
				
			JSONObject jsMoveInfo;
//				jsMoveInfo.put("AreaCount",MoveInfo.AreaCount);
			jsMoveInfo.put("MdEnable",MoveInfo.MdEnable);
			jsMoveInfo.put("Sensitive",MoveInfo.Sensitive);
//				jsMoveInfo.put("Columns",MoveInfo.Columns);
//				jsMoveInfo.put("Rows",MoveInfo.Rows);
			JSONArray jsArea;
			for(int i=0;i<256;i++){
				jsArea.put(i,MoveInfo.Area[i]);
			}
			jsMoveInfo.put("Area",jsArea);
				
			jsroot.put("MoveInfo",jsMoveInfo);
				
			JSONObject jsPolicyInfo;
				
			JSONObject jsIoOutputInfo;
			jsIoOutputInfo.put("Delay",PolicyInfo.IoOutputInfo.Delay);
			jsIoOutputInfo.put("EmailEnable",PolicyInfo.IoOutputInfo.EmailEnable);
			jsPolicyInfo.put("IoOutputInfo",jsIoOutputInfo);
				
			JSONObject jsRecordInfo;
			jsRecordInfo.put("EmailEnable",PolicyInfo.RecordInfo.EmailEnable);
			jsRecordInfo.put("RecordTime",PolicyInfo.RecordInfo.RecordTime);
			jsRecordInfo.put("FtpEnable",PolicyInfo.RecordInfo.FtpEnable);
			jsRecordInfo.put("CloudEnable",PolicyInfo.RecordInfo.CloudEnable);
			jsRecordInfo.put("RecEnable",PolicyInfo.RecordInfo.RecEnable);
			jsPolicyInfo.put("RecordInfo",jsRecordInfo);
				
			JSONObject jsSnapInfo;
			jsSnapInfo.put("EmailEnable",PolicyInfo.SnapInfo.EmailEnable);
			jsSnapInfo.put("PictureNum",PolicyInfo.SnapInfo.PictureNum);
			jsSnapInfo.put("Sec",PolicyInfo.SnapInfo.Sec);
			jsSnapInfo.put("Msec",PolicyInfo.SnapInfo.Msec);
			jsSnapInfo.put("FtpEnable",PolicyInfo.SnapInfo.FtpEnable);
			jsSnapInfo.put("CloudEnable",PolicyInfo.SnapInfo.CloudEnable);
			jsPolicyInfo.put("SnapInfo",jsSnapInfo);
				
			JSONObject jsPresetInfo;
			jsPresetInfo.put("PresetId",PolicyInfo.PresetInfo.PresetId);
			jsPresetInfo.put("PresetName",PolicyInfo.PresetInfo.PresetName);
			jsPolicyInfo.put("PresetInfo",jsPresetInfo);

			jsroot.put("PolicyInfo",jsPolicyInfo);
				
			jsresult.put("Alarm.MoveAlarm", jsroot);
			
		jsresult.toString(str);
		return str.length();
	}
}IPCNETMoveAlarmCfg_st;

#endif
