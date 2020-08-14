#ifndef IPCNET_MANAGER_INTERFACE_H
#define IPCNET_MANAGER_INTERFACE_H


#ifdef __cplusplus
extern "C"
{
#endif
#ifdef _WIN32
#else
//#ifndef bool
//#define bool char
//#endif
#define __declspec(x)
#define _stdcall
#endif
typedef enum
{
    GMT_NEGA_12=0,
	GMT_NEGA_11_30,
    GMT_NEGA_11,
    GMT_NEGA_10_30,
    GMT_NEGA_10,
    GMT_NEGA_9_30,	//5
    GMT_NEGA_9,
    GMT_NEGA_8_30,
    GMT_NEGA_8,
    GMT_NEGA_7_30,
    GMT_NEGA_7,		//10
    GMT_NEGA_6_30,
    GMT_NEGA_6,
    GMT_NEGA_5_30,
    GMT_NEGA_5,
    GMT_NEGA_4_30,	//15
    GMT_NEGA_4,
    GMT_NEGA_3_30,
    GMT_NEGA_3,
    GMT_NEGA_2_30,
    GMT_NEGA_2,		//20
    GMT_NEGA_1_30,
    GMT_NEGA_1,
    GMT_NEGA_0_30,
    GMT_0,			//24
    GMT_PLUS_0_30,
    GMT_PLUS_1,
    GMT_PLUS_1_30,
    GMT_PLUS_2,
    GMT_PLUS_2_30,
    GMT_PLUS_3,		//30
    GMT_PLUS_3_30,
    GMT_PLUS_4,
    GMT_PLUS_4_30,
    GMT_PLUS_5,
    GMT_PLUS_5_30,	//35
    GMT_PLUS_6,
    GMT_PLUS_6_30,
    GMT_PLUS_7,
    GMT_PLUS_7_30,
    GMT_PLUS_8,//北京时间,默认值	//40
    GMT_PLUS_8_30,
    GMT_PLUS_9,
    GMT_PLUS_9_30,
    GMT_PLUS_10,
    GMT_PLUS_10_30,
    GMT_PLUS_11,
    GMT_PLUS_11_30,
    GMT_PLUS_12,		//48
}DEV_GMT_e;
typedef struct{
	int Hour;
	int Min;
	int Sec;
}IPCNetTime_t;
typedef struct{
	int Year;
	int Mon;
	int Day;
}IPCNetDate_t;
typedef struct{
	IPCNetDate_t Date;
	IPCNetTime_t Time;
	bool NtpEnable;
	char NtpServ[256];
	int TimeZone;//DEV_GMT_e
}IPCNetTimeCfg_t;

//设备类型
typedef enum
{
	DEV_TYPE_SWITCH_AND_OUTLET = 0,//带开关的插座
	DEV_TYPE_SWITCH,//开关
	DEV_TYPE_OUTLET,//插座
	DEV_TYPE_SCENE_PANEL,//情景面板
	DEV_TYPE_IR_CONTROL_UNIT,//红外转发器(遥控器)
	DEV_TYPE_ROUTER,//路由器
	DEV_TYPE_LIGHT,//灯
	DEV_TYPE_LIGHT_SENSOR,//光线传感器
	DEV_TYPE_CURTAIN_CONTROLER,//窗帘控制器
	DEV_TYPE_TEMPERATURE_SENSOR,//温度传感器
	DEV_TYPE_PRESSURE_SENSOR,//压力传感器
	DEV_TYPE_FLOW_RATE_SENSOR,//流量传感器
	DEV_TYPE_CAMERA,//摄像头
	DEV_TYPE_COMPUTER,//电脑
	DEV_TYPE_SPEAKER,//喇叭音响
	DEV_TYPE_AIR_CONDITIONER_CONTROLLER,//空调控制器
	DEV_TYPE_INVALID,//无效类型
}DEV_TYPE_e;

typedef enum
{
	IPCNET_H264E_NALU_BSLICE = 0,                            /*B SLICE types*/
    IPCNET_H264E_NALU_PSLICE = 1,                         /*P SLICE types*/
    IPCNET_H264E_NALU_ISLICE = 2,                            /*I SLICE types*/
    IPCNET_H264E_NALU_IDRSLICE = 5,                       /*IDR SLICE types*/
    IPCNET_H264E_NALU_SEI    = 6,                         /*SEI types*/
    IPCNET_H264E_NALU_SPS    = 7,                         /*SPS types*/
    IPCNET_H264E_NALU_PPS    = 8,                         /*PPS types*/
	IPCNET_H264E_NALU_BUTT = 10,

	IPCNET_JPEGE_PACK_ECS = 10+5-4,                            /*ECS types*/
    IPCNET_JPEGE_PACK_APP = 10+6-4,                            /*APP types*/
    IPCNET_JPEGE_PACK_VDO = 10+7-4,                            /*VDO types*/
    IPCNET_JPEGE_PACK_PIC = 10+8-4,                            /*PIC types*/
    IPCNET_JPEGE_PACK_DCF = 10+9-4,                            /*DCF types*/
    IPCNET_JPEGE_PACK_DCF_PIC = 10+10-4,                       /*DCF PIC types*/
    IPCNET_JPEGE_PACK_BUTT=19,

	IPCNET_AUDIO_ADPCM = 20,
	IPCNET_AUDIO_G711A,
	IPCNET_AUDIO_G711U,
	IPCNET_AUDIO_G726,
	IPCNET_AUDIO_AMR,
	IPCNET_AUDIO_AAC,//25
	IPCNET_AUDIO_PCM,
	IPCNET_AUDIO_BUTT = 30,

	IPCNET_JPEG = 31,
	IPCNET_JPEG_BUTT = 34,

	IPCNET_IMG_H264_IFRAME = 35,//作为图片的单帧图像
	IPCNET_IMG_H265_IFRAME = 36,//作为图片的单帧图像

	IPCNET_H265E_NALU_BSLICE = 50+0,                             /*B SLICE types*/
    IPCNET_H265E_NALU_PSLICE = 50+1,                          /*P SLICE types*/
    IPCNET_H265E_NALU_ISLICE = 50+2,                             /*I SLICE types*/
    IPCNET_H265E_NALU_IDRSLICE = 50+19,                       /*IDR SLICE types*/
    IPCNET_H265E_NALU_VPS    = 50+32,                         /*VPS types*/
    IPCNET_H265E_NALU_SPS    = 50+33,                         /*SPS types*/
    IPCNET_H265E_NALU_PPS    = 50+34,                         /*PPS types*/
    IPCNET_H265E_NALU_SEI    = 50+39,                         /*SEI types*/
	IPCNET_H265E_NALU_BUTT,
}VideoAudioCodeType_e;

//事件回调，用于上报每个连接所发生的事件
struct IPCNetEventHandler{
	//onStatus 状态上报回调，status请参考SEP2P_Error.h
	void (*onStatus)(const char* uuid,int status);
	//onVideoData 视频数据回调接口，用于接收设备发送的视频，图片等数据, type请参考VideoAudioCodeType_e
	void (*onVideoData)(const char* uuid,int type,unsigned char*data,int len,long timestamp);
	//onAudioData 音频数据回调接口，用于接收设备发送的音频, type请参考VideoAudioCodeType_e
	void (*onAudioData)(const char* uuid,int type,unsigned char*data,int len,long timestamp);
	//onJSONString 包含内容的状态上报接口，一般是命令反馈，也有报警上报，msg_type类型请查看JSONStructProtocal.h中IPCNET*定义
	void (*onJSONString)(const char* uuid,int msg_type,const char* jsonstr);
};

//IPCNetEventHandler内回调声明
void onStatus(const char* uuid,int status);
void onVideoData(const char* uuid,int type,unsigned char*data,int len,long timestamp);
void onAudioData(const char* uuid,int type,unsigned char*data,int len,long timestamp);
void onJSONString(const char* uuid,int msg_type,const char* jsonstr);

struct DevInfo
{
//public:
//	DevInfo(void);
//	~DevInfo(void);
#define DEV_INFO_TOCKEN 'A'
	char mTocken; //设置为A
	char mUUID[64]; //设备UUID
	char mCHCK[16]; //设备UUID校验码/检查码
	char mIP[128]; //设备IP
	char mSockIP[128]; //发送此消息的sock IP
	char mMAC[128];// 设备MAC
	char mSoftVer[64]; //软件版本
	char mHwVer[64]; //硬件版本
	int mIndex;
	char mNetMask[32];		//子网掩码
	char mGateway[128];		//网关
	char mDNS1[128];		//dns1
	char mDNS2[128];		//dns2
	unsigned short          nPort;			//设备端口
	char mDevName[128];		//设备名称
	char mUserName[128];		//修改时会对用户认证
	char mPassword[128];		//修改时会对用户认证
	int mDevType; //设备类型，参见DEV_TYPE_e
	char mFlashID[128];//设备唯一ID
	bool mFlashValid;//设备是否校验
//#ifdef _WIN32
//	char mIPv4Addr[sizeof(SOCKADDR_IN)];//SOCKADDR_IN //struct sockaddr_in
//#endif
	char mIPv4Addr[128];
};
typedef void(*OnSearchDeviceResult_t)(struct DevInfo*dev);
typedef void(*OnLanSettingResult_t)(const char*ip,int ret);//成功ret==1
typedef void(*OnCmdResult_t)(int cmd,const char*uuid,const char*json);

//释放命令与回调绑定的资源，在使用了带R函数之后，在r回调用里面调用IPCNetReleaseCmdResource进行资源释放
/*
举例如下：
void OnGetDevInfoCmdResult(int cmd,const char*uuid,const char*json)
{
	//处理json消息
	....
	//释放命令绑定资源
	IPCNetReleaseCmdResource(cmd, OnGetDevInfoCmdResult);
}
//调用IPCNetGetDevInfoR获取设备信息，结果会从 OnGetDevInfoCmdResult 返回
IPCNetGetDevInfoR(uuid, OnGetDevInfoCmdResult);
*/
void __declspec(dllexport) _stdcall IPCNetReleaseCmdResource(int cmd,const char*uuid,OnCmdResult_t r);

//IPCNet SDK 初始化函数，ssn设置为空即可，如下所示
// IPCNetInitialize("");
int __declspec(dllexport) _stdcall IPCNetInitialize(const char*ssn);
//SDK去初始化，释放内部资源
int __declspec(dllexport) _stdcall IPCNetDeInitial(void);
//获取版本号
int __declspec(dllexport) _stdcall IPCNetGetAPIVersion(unsigned int *version);

//局域网搜索设备，搜索结果会从 osdr 返回，搜索结果会多次返回，需要做好过滤
//比如同个设备，可能会从 osdr 返回多次，需要避免这种干扰
int __declspec(dllexport) _stdcall IPCNetSearchDevice(OnSearchDeviceResult_t osdr);
int __declspec(dllexport) _stdcall IPCNetStopSearchDevice();

//设置局域网设置结果返回 回调，当调用局域网设置函数，结果通过OnLanSettingResult_t返回
int __declspec(dllexport) _stdcall IPCNetSetLanSettingResultCallback(OnLanSettingResult_t r);
//局域网重启设备
int __declspec(dllexport) _stdcall IPCNetRebootDeviceInLAN(const char*ip);
//局域网设置设备信息
int __declspec(dllexport) _stdcall IPCNetSetDeviceInfoInLAN(struct DevInfo *dev);
//让设备重新申请IP
int __declspec(dllexport) _stdcall IPCNetSetDeviceDhcpInLAN();

/*
*h:此session的数据回调接口，回调有如许接口函数当有对应数据就会调用这个接口
*onStatus:session 状态回调，反应了session的运行状态
*onVideoData:视频数据回调
*onAudioData:音频数据回调
*onJSONString:命令数据回调，调用对应功能接口之后，会回调这个函数告知结果，功能函数有：
*	设置网络、设置图像、其他设置等等，想要得到结果，请在此函数等待数据
*/
int __declspec(dllexport) _stdcall IPCNetStartIPCNetSession(const char* uuid,const char* passwd,struct IPCNetEventHandler*h);
int __declspec(dllexport) _stdcall IPCNetStopIPCNetSession(const char* uuid);
//获取设备状态，及时返回
int __declspec(dllexport) _stdcall IPCNetGetIPCNetSessionStatus(const char* uuid);

/*
* IPCNetSendJsonCmd(R) 是基本的命令发送接口，其他命令函数都是基于此命令的，所以，熟悉协议可以使用此函数自行解析开发功能
*cmd:功能命令
*json:命令的详细内容
*/
int __declspec(dllexport) _stdcall IPCNetSendJsonCmd(const char* uuid,int cmd ,const char*json);
int __declspec(dllexport) _stdcall IPCNetSendJsonCmdR(const char* uuid,int cmd ,const char*json,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetSendBinaryDataR(const char* uuid,int cmd ,const char*data, int len,OnCmdResult_t r);
#define IPCNetSendBinaryData(uuid, cmd ,data, len) \
	IPCNetSendBinaryDataR(uuid, cmd ,data, len, 0);

/*
* IPCNetStartVideo(R) 请求视频
*stream:0~1， 0:主码流，高清码流，适合设备端录像， 1:子码流，标清码流，适合网络传输
* 为了良的体验和流性，一般默认请求标清码流
*/
int __declspec(dllexport) _stdcall IPCNetStartVideo(const char* uuid,int stream);
int __declspec(dllexport) _stdcall IPCNetStartVideoR(const char* uuid,int stream,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetStopVideo(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetStopVideoR(const char* uuid,OnCmdResult_t r);
//请求音频
int __declspec(dllexport) _stdcall IPCNetStartAudio(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetStartAudioR(const char* uuid,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetStopAudio(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetStopAudioR(const char* uuid,OnCmdResult_t r);
//请求设备端开启扬声器，准备接收音频数据并播放
//type: audio encode type, no used yet, set to IPCNET_AUDIO_G711A
//type: 是音频编码类型，目前未使用，填 IPCNET_AUDIO_G711A
//type: 如果设备指定编码其他格式，请改成其他格式，默认是 IPCNET_AUDIO_G711A
#define IPCNetStartTalk(x,y) IPCNetStartTalkR(x,y,0)
int __declspec(dllexport) _stdcall IPCNetStartTalkR(const char* uuid,int type,OnCmdResult_t r);
#define IPCNetStopTalk(x) IPCNetStopTalkR(x,0)
int __declspec(dllexport) _stdcall IPCNetStopTalkR(const char* uuid,OnCmdResult_t r);
//IPCNetPutTalkData 发送音频数据
//type:0
//success:position which equal to len, failed:return negative value
//if return ERROR_SEP2P_REMOTE_SITE_BUFFER_FULL,please wait a minute and try again
int __declspec(dllexport) _stdcall IPCNetPutTalkData(const char* uuid,const char*data,int len);

//Use IPCNetAudioCfg_st to parse json string and serialize struct into json string
int __declspec(dllexport) _stdcall IPCNetSetAudioCfg(const char* uuid,const char*json);
int __declspec(dllexport) _stdcall IPCNetSetAudioCfgR(const char* uuid,const char*json,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetAudioCfg(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetAudioCfgR(const char* uuid,OnCmdResult_t r);

//获取设备端信息
int __declspec(dllexport) _stdcall IPCNetGetDevInfo(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetDevInfoR(const char* uuid,OnCmdResult_t r);

//修改设备密码
int __declspec(dllexport) _stdcall IPCNetChangeDevPwd(const char* uuid,const char*pwd);
int __declspec(dllexport) _stdcall IPCNetChangeDevPwdR(const char* uuid,const char*pwd,OnCmdResult_t r);

//设备端恢复出厂设置
int __declspec(dllexport) _stdcall IPCNetRestoreToFactorySetting(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetRestoreToFactorySettingR(const char* uuid,OnCmdResult_t r);

typedef enum{
	IPCNET_RESL_STD_DEF = 0,//standard definition, 标清
	IPCNET_RESL_HIGH_DEF = 1,//high definition, 高清
	IPCNET_RESL_4K_DEF = 2,//4K ultra definition, 4K超清
}IPCNetResolution_t;
//设置设备分辨率
//res为分辨率索引，0 标清，1 高清，2 4K超清
//设置之前，先调用IPCNetGetResolution(R)获取支持的分辨率以及对应分辨率的索引
//res参数请参考IPCNetResolution_t
int __declspec(dllexport) _stdcall IPCNetSetResolution(const char* uuid,int res);
int __declspec(dllexport) _stdcall IPCNetSetResolutionR(const char* uuid,int res,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetResolution(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetResolutionR(const char* uuid,OnCmdResult_t r);
/*
* 设置亮
*bri:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetBrightness(const char* uuid,int bri);
int __declspec(dllexport) _stdcall IPCNetSetBrightnessR(const char* uuid,int bri,OnCmdResult_t r);
//获取亮度，异步，通过OnCmdResult_t返回
int __declspec(dllexport) _stdcall IPCNetGetBrightness(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetBrightnessR(const char* uuid,OnCmdResult_t r);
/*
* 设置对比度
*con:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetContrast(const char* uuid,int con);
int __declspec(dllexport) _stdcall IPCNetSetContrastR(const char* uuid,int con,OnCmdResult_t r);
//获取对比度，异步，通过OnCmdResult_t返回
int __declspec(dllexport) _stdcall IPCNetGetContrast(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetContrastR(const char* uuid,OnCmdResult_t r);
/*
* 设置色度
*hue:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetHue(const char* uuid,int hue);//chroma
int __declspec(dllexport) _stdcall IPCNetSetHueR(const char* uuid,int hue,OnCmdResult_t r);//chroma
//获取色度
int __declspec(dllexport) _stdcall IPCNetGetHue(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetHueR(const char* uuid,OnCmdResult_t r);
/*
* 设置饱和度
*sat:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetSaturation(const char* uuid,int sat);
int __declspec(dllexport) _stdcall IPCNetSetSaturationR(const char* uuid,int sat,OnCmdResult_t r);
//获取饱和度
int __declspec(dllexport) _stdcall IPCNetGetSaturation(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetSaturationR(const char* uuid,OnCmdResult_t r);
/*
* 设置锐度
*acu:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetAcutance(const char* uuid,int acu);
int __declspec(dllexport) _stdcall IPCNetSetAcutanceR(const char* uuid,int acu,OnCmdResult_t r);
//获取锐度信息
int __declspec(dllexport) _stdcall IPCNetGetAcutance(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetAcutanceR(const char* uuid,OnCmdResult_t r);
//恢复图像默认值，当调用以上图像设置函数设置了不理想的效果时，可调用恢复函数恢复成出厂设置
int __declspec(dllexport) _stdcall IPCNetSetCameraColorSettingDefault(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetSetCameraColorSettingDefaultR(const char* uuid,OnCmdResult_t r);

/*
*flip:翻转,mirror:镜像
*有效数据为 0,1
*0无效
*1设置成对应效果
*/
int __declspec(dllexport) _stdcall IPCNetSetFlipMirror(const char* uuid,int flip,int mirror);
int __declspec(dllexport) _stdcall IPCNetSetFlipMirrorR(const char* uuid,int flip,int mirror,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetFlipMirror(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetFlipMirrorR(const char* uuid,OnCmdResult_t r);

/*int __declspec(dllexport) _stdcall IPCNetSetBitrate(const char* uuid,int stream,int br);//br:768 means 768K,1024 means 1M
int __declspec(dllexport) _stdcall IPCNetSetBitrateR(const char* uuid,int stream,int br,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetBitrate(const char* uuid,int stream);//get successfully>0,failed <0
int __declspec(dllexport) _stdcall IPCNetGetBitrateR(const char* uuid,int stream,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetSetFramerate(const char* uuid,int stream,int fr);//fr:>0,normally 1~30
int __declspec(dllexport) _stdcall IPCNetSetFramerateR(const char* uuid,int stream,int fr,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetFramerate(const char* uuid,int steam);
int __declspec(dllexport) _stdcall IPCNetGetFramerateR(const char* uuid,int steam,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetSetEncodeMode(const char* uuid,int em);//em: 0:CBR  1:VBR
int __declspec(dllexport) _stdcall IPCNetSetEncodeModeR(const char* uuid,int em,OnCmdResult_t r);//em: 0:CBR  1:VBR
int __declspec(dllexport) _stdcall IPCNetGetEncodeMode(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetEncodeModeR(const char* uuid,OnCmdResult_t r);*/

/*
	IPCNetVideoEncodeCfg_st ivec;
	OnGetCameraPictureSettingCmdResult(int cmd,const char*uuid,const char*json){
		JSONObject jsdata(json);
		ivec.parseJSON(jsdata);
	}
	getcamerapicsetting(){
		IPCNetGetCameraPictureSettingR(uuid,OnGetCameraPictureSettingCmdResult);
	}
	
	SetCameraPictureSettingOnCmdResult(int cmd,const char*uuid,const char*json){
	}
	setcamerapicsetting(){
		String str;
		ivec.VideoEncode[1]->Witdh=352;
		ivec.VideoEncode[1]->Height=288;
		ivec.toJSONString(str);
		IPCNetSetCameraPictureSettingR(uuid,str.data(),SetCameraPictureSettingOnCmdResult);
	}
 */
int __declspec(dllexport) _stdcall IPCNetGetCameraPictureSettingR(const char* uuid,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetSetCameraPictureSettingR(const char* uuid,const char*json,OnCmdResult_t r);

//重启设备
int __declspec(dllexport) _stdcall IPCNetRebootDevice(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetRebootDeviceR(const char* uuid,OnCmdResult_t r);

//设备时间
int __declspec(dllexport) _stdcall IPCNetSetTime(const char* uuid,IPCNetTimeCfg_t *t);
int __declspec(dllexport) _stdcall IPCNetSetTimeR(const char* uuid,IPCNetTimeCfg_t *t,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetTime(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetTimeR(const char* uuid,OnCmdResult_t r);

//设备录像
//IPCNetRecordCfg_st
int __declspec(dllexport) _stdcall IPCNetSetRecord(const char* uuid,const char*json);
int __declspec(dllexport) _stdcall IPCNetSetRecordR(const char* uuid,const char*json,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetRecord(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetRecordR(const char* uuid,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetFormatDiskR(const char* uuid,int index,OnCmdResult_t r);
#define IPCNetFormatDisk(a,b) IPCNetFormatDiskR(a,b,0);

/*
*有线网络设置
*dhcp:0关闭，1打开
*/
int __declspec(dllexport) _stdcall IPCNetSetWiredNetwork(const char* uuid,int dhcp,const char* ip,const char* netmask,const char* getway,const char* dns1,const char* dns2);
int __declspec(dllexport) _stdcall IPCNetSetWiredNetworkR(const char* uuid,int dhcp,const char* ip,const char* netmask,const char* getway,const char* dns1,const char* dns2,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetWiredNetwork(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetWiredNetworkR(const char* uuid,OnCmdResult_t r);

/*
*设置WiFi,设置之前请先调用Get获取WiFi设置，再调用Search获取wifi列表
*EncType请使用搜到的参数，所以设置之前，需要先调用IPCNetSearchWiFi(R)获取WiFi信息
*如果手动输入，EncType有"WEP-NONE","WEP","WPA-AES","WPA-TKIP","WPA2-AES","WPA2-TKIP"，必须设置正确，否则设备连不上WiFi
*参数请先转换为utf8字符串
*/
int __declspec(dllexport) _stdcall IPCNetSetWiFi(const char* uuid,const char* SSID,const char* Password,const char* EncType);
int __declspec(dllexport) _stdcall IPCNetSetWiFiR(const char* uuid,const char* SSID,const char* Password,const char* EncType,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetWiFi(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetWiFiR(const char* uuid,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetSearchWiFi(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetSearchWiFiR(const char* uuid,OnCmdResult_t r);

/*
*enable: 0关闭移动网络，1打开移动网络
*vpn:设置对应网络vpn,默认设置空
*/
//deprated
int __declspec(dllexport) _stdcall IPCNetSetMobileNetwork(const char* uuid,int enable,const char* vpn);
//deprated
int __declspec(dllexport) _stdcall IPCNetSetMobileNetworkR(const char* uuid,int enable,const char* vpn,OnCmdResult_t r);
//please use IPCNetSetMobileNetwork2R
//use IPCNetMobileNetworkInfo_st to convert json and c/c++ struct
int __declspec(dllexport) _stdcall IPCNetSetMobileNetwork2R(const char* uuid, const char*json, OnCmdResult_t r);
#define IPCNetSetMobileNetwork2(x,y) IPCNetSetMobileNetwork2R(x,y,0)
int __declspec(dllexport) _stdcall IPCNetGetMobileNetwork(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetMobileNetworkR(const char* uuid,OnCmdResult_t r);


//WiFi AP 设备热点
//IPCNetWiFiAPInfo_t  in  JSONStructProtocal.h
int __declspec(dllexport) _stdcall IPCNetGetWiFiAPInfoR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetWiFiAPInfo(x) IPCNetGetWiFiAPInfoR(x,0)
int __declspec(dllexport) _stdcall IPCNetSetWiFiAPInfoR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetWiFiAPInfo(x,y) IPCNetSetWiFiAPInfoR(x,y,0)

//网络策略设置
//IPCNetNetworkStrategy_st  in  JSONStructProtocal.h
int __declspec(dllexport) _stdcall IPCNetGetNetworkStrategyR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetNetworkStrategy(x) IPCNetGetNetworkStrategyR(x,0)
int __declspec(dllexport) _stdcall IPCNetSetNetworkStrategyR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetNetworkStrategy(x,y) IPCNetSetNetworkStrategyR(x,y,0)


//使用 IPCNetPicColorInfo_st 解析json
int __declspec(dllexport) _stdcall IPCNetGetIRMode(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetIRModeR(const char* uuid,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetSetIRMode(const char* uuid,const char*json);
int __declspec(dllexport) _stdcall IPCNetSetIRModeR(const char* uuid,const char*json,OnCmdResult_t r);

//设备OSD，视频水印设置
//IPCNetGetOsdCfg_st
int __declspec(dllexport) _stdcall IPCNetGetOSDR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetGetOSD(x,y) IPCNetGetOSDR(x,y,0)
//IPCNetOsdCfg_st
int __declspec(dllexport) _stdcall IPCNetSetOSDR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetOSD(x,y) IPCNetSetOSDR(x,y,0)

//设备报警
int __declspec(dllexport) _stdcall IPCNetGetAlarmR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetAlarm(x) IPCNetGetAlarmR(x,0)
int __declspec(dllexport) _stdcall IPCNetSetAlarmR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetAlarm(x,y) IPCNetSetAlarmR(x,y,0)

int __declspec(dllexport) _stdcall IPCNetGetGPIOR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetGetGPIO(x,y) IPCNetGetGPIOR(x,y,0)
int __declspec(dllexport) _stdcall IPCNetSetGPIOR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetGPIO(x,y) IPCNetSetGPIOR(x,y,0)



//设置录像配置，设置之前，请先调用 IPCNetGetRecordConfR 获取配置
//具体用法参见 sample_code.cpp
int __declspec(dllexport) _stdcall IPCNetSetRecordConfR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetRecordConf(x,y) IPCNetSetRecordConfR(x,y,0)
int __declspec(dllexport) _stdcall IPCNetGetRecordConfR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetGetRecordConf(x,y) IPCNetGetRecordConfR(x,y,0)

/*****************************   浏览设备端文件   *******************************************
浏览设备端文件函数:IPCNetListRemoteDirInfoR,IPCNetListRemotePageFileR

例子如下，调用入口为：get_sdcard_files_list

static char mCurViewPath[]="/mnt/s0/";//根据自己需求更改此目录

void OnListRemoteDirInfoCmdResult(int cmd,const char*uuid,const char*json);
void get_sdcard_files_list(const char *uuid)
{
	char rootdir[]="/mnt/s0/";
	int vi=0;
	int mode=1;//0: 只扫描文件   1: 扫描目录和文件

	int start=0;//文件开始时间
	int end=240000;//文件结束时间

	//组织json字符串，lir是list remote简写， p为path简写，si是sensor index简写，m是mode简写
	//st是start time， e是end time
	char jsonbuff[1024] = {0};
    sprintf(jsonbuff,"{\"lir\":{\"p\":\"%s\",\"si\":%d,\"m\":%d,\"st\":%d,\"e\":%d}}",
		rootdir, vi, mode, start, end);
	IPCNetListRemoteDirInfoR(uuid, jsonbuff, OnListRemoteDirInfoCmdResult);
}

//以下静态变量根据项目实际情况移至类的成员去
static int mNextStartIndex=0;
static int mTotalNum=0;
static RemoteDirInfo_t*mRemoteRootDirInfo,mSelectedRemoteDirInfo;
static RemoteDirInfo_t*mCurRemoteDirInfo;
//RemoteDirInfo_t 在 JSONStructProtocal.h 定义

void OnListRemotePageFileCmdResult2(int cmd,const char*uuid,const char*json);
void OnListRemoteDirInfoCmdResult(int cmd,const char*uuid,const char*json)
{
	JSONObject jsdata(json);//解析json
	RemoteDirListInfo_t rdi;
	rdi.parseJSON(jsdata);//从返回的json结构体里面获取目录信息

	TRACE("num of files:%d disk size:%d MB used size:%d MB\n",rdi.num, rdi.total, rdi.used);

	//mTotalSize=rdi.total;//获取磁盘总空间
	//mUsedSize=rdi.used;//获取磁盘已使用空间

	int reqnum;
	int curIndex=0;
	mNextStartIndex=0;
	mTotalNum=rdi.num;//设置当前目录总共有几个文件，包括目录（IPCNetListRemoteDirInfoR的mode设置为1的时候，会包含目录）
	if(mTotalNum>10){//每次获取最多10个目录下面的文件
		reqnum=10;
		mNextStartIndex=10;
	}else{
		reqnum=rdi.num;//不足10个，一次性全部获取
		mNextStartIndex=rdi.num;
	}

	//组织json字符串，lp是list path简写， p为path简写，s是start简写，c是count简写
	char jsonbuff[1024] = {0};
    sprintf(jsonbuff,"{\"lp\":{\"p\":\"%s\",\"s\":%d,\"c\":%d}}", mCurViewPath, curIndex, reqnum);
	//按索引获取目录下的文件名，结果通过 OnListRemotePageFileCmdResult2 返回
	IPCNetListRemotePageFileR(uuid, jsonbuff, OnListRemotePageFileCmdResult2);

	//根据获取到的数据更新UI界面
	//lRemoteStorageFileList->updateUI();

	//释放命令绑定资源
	IPCNetReleaseCmdResource(cmd, uuid, OnListRemoteDirInfoCmdResult);
}

void OnListRemotePageFileCmdResult2(int cmd,const char*uuid,const char*json)
{
	JSONObject jsdata(json);//解析json
	RemoteDirInfo_t *rdi=new RemoteDirInfo_t;
	rdi->path=mCurViewPath;
	rdi->parseJSON(jsdata);
	TRACE("name:%s type:%d RemoteFileInfoListSize:%d \n",rdi->name.c_str(),rdi->type,rdi->mRemoteFileInfoList.size());

	if(mCurRemoteDirInfo==0){
		mCurRemoteDirInfo=rdi;
	}else if(mCurRemoteDirInfo->path==rdi->path){
		for(list<RemoteFileInfo_t*>::iterator it= rdi->mRemoteFileInfoList.begin(); it!=rdi->mRemoteFileInfoList.end(); it++){
            RemoteFileInfo_t*rfi = *it;
			RemoteFileInfo_t*rfi_bak=new RemoteFileInfo_t;
			//memcpy(rfi_bak,rfi,sizeof(RemoteFileInfo_t));
			rfi_bak->name=rfi->name;
			rfi_bak->path=rfi->path;
			rfi_bak->type=rfi->type;
			rfi_bak->size=rfi->size;
			mCurRemoteDirInfo->mRemoteFileInfoList.push_back(rfi_bak);//把新文件添加到目录
        }
		delete rdi;
	}

	if(mRemoteRootDirInfo==0){
		mRemoteRootDirInfo=rdi;
	}

	//更新目录文件
	//lRemoteStorageFileList->updateUI();

	IPCNetReleaseCmdResource( cmd,uuid,OnListRemotePageFileCmdResult2);

	if(mTotalNum>mNextStartIndex){//根据当前获取到的索引，继续获取剩下的文件
		int curIndex=mNextStartIndex;
		int reqnum=mTotalNum-curIndex;
		char jsonbuff[1024] = {0};
		if(reqnum>10){
			reqnum=10;
		}
		mNextStartIndex += reqnum;

		sprintf(jsonbuff,"{\"lp\":{\"p\":\"%s\",\"s\":%d,\"c\":%d}}",lRemoteStorageFileList->mCurViewPath.GetBuffer(),curIndex,reqnum);
		//TRACE("json:%s\n",jsonbuff);

		//释放命令绑定资源
		IPCNetListRemotePageFileR(uuid,jsonbuff,OnListRemotePageFileCmdResult2);
	}
}
*/
int __declspec(dllexport) _stdcall IPCNetListRemoteDirInfoR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetListRemoteDirInfo(x,y) IPCNetListRemoteDirInfoR(x,y,0)

int __declspec(dllexport) _stdcall IPCNetListRemotePageFileR(const char* uuid,const char*path,OnCmdResult_t r);
#define IPCNetListRemotePageFile(x,y) IPCNetListRemotePageFileR(x,y,0)

//删除设备文件，path为设备文件在设备的绝对路径
int __declspec(dllexport) _stdcall IPCNetDeleteRemoteFileR(const char* uuid,const char*path,OnCmdResult_t r);
#define IPCNetDeleteRemoteFile(x,y) IPCNetDeleteRemoteFileR(x,y,0)






//下载设备文件
typedef void(*OnDonwloadDataCallback_t)(const char*uuid,const char *file,const char*data,int len,unsigned int offset);
int __declspec(dllexport) _stdcall IPCNetStartDownloadFileR(const char* uuid,const char*path,OnDonwloadDataCallback_t onData,OnCmdResult_t r);
#define IPCNetStartDownloadFile(x,y,z) IPCNetStartDownloadFileR(x,y,z,0)
int __declspec(dllexport) _stdcall IPCNetStopDownloadFileR(const char* uuid,const char*path,OnCmdResult_t r);
#define IPCNetStopDownloadFile(x,y) IPCNetStopDownloadFileR(x,y,0)

//PTZ控制，摇头相关
int __declspec(dllexport) _stdcall IPCNetSetPtzCtrlR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetPtzCtrl(x,y) IPCNetSetPtzCtrlR(x,y,0)
int __declspec(dllexport) _stdcall IPCNetGetPtzCtrlR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetPtzCtrl(x) IPCNetGetPtzCtrlR(x,0)

//曝光类型
int __declspec(dllexport) _stdcall IPCNetGetExpTypeR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetExpType(x) IPCNetGetExpTypeR(x,0)
//IPCNetExpType_st
int __declspec(dllexport) _stdcall IPCNetSetExpTypeR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetExpType(x,y) IPCNetSetExpTypeR(x,y,0)

//手动曝光
//IPCNetManualExpInfo_st
int __declspec(dllexport) _stdcall IPCNetGetManualExpInfoR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetManualExpInfo(x) IPCNetGetManualExpInfoR(x,0)
int __declspec(dllexport) _stdcall IPCNetSetManualExpInfoR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetManualExpInfo(x,y) IPCNetSetManualExpInfoR(x,y,0)

//自动曝光
//IPCNetAutoExp_st
int __declspec(dllexport) _stdcall IPCNetGetAutoExpInfoR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetAutoExpInfo(x) IPCNetGetAutoExpInfoR(x,0)
int __declspec(dllexport) _stdcall IPCNetSetAutoExpInfoR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetAutoExpInfo(x,y) IPCNetSetAutoExpInfoR(x,y,0)



//*******************************  时间轴录像回放例子  ********************************
/*
请参见 sample_code.cpp 的
void get_record_time_period(const char*uuid, int vi, int date)
其中，vi是video index的缩写，指示播放设备哪个摄像头的录像，一般设备只有一个摄像头，所以vi一般传0
date 表示获取哪一天的时间轴时间，格式为 20200214， 表示2020年2月14日， 调用例子如下

const char*uuid = "device uuid";
int vi=0;
int date = 20200214;
get_record_time_period(uuid, vi, date);
*/

//播放指定时间的录像，如果指定时间没有录像，则会播放时间之后的录像
//vi是video index的缩写，指示播放设备哪个摄像头的录像，一般设备只有一个摄像头，所以vi一般传0
//date 表示获取哪一天的时间轴时间，格式为 20200214， 表示2020年2月14日
//time 表示指定的时间点，格式为 161938, 表示16点19分38秒(16:19:38)
//调用格式为 IPCNetStartPlaybackAtTimeR("device uuid", 0, 20200214, 161938, NULL);//OnCmdResult_t r 参数可以设置为接收返回结果的回调函数
int __declspec(dllexport) _stdcall IPCNetStartPlaybackAtTimeR(const char* uuid, int vi, int date, int time, OnCmdResult_t r);
#define IPCNetStartPlaybackAtTime(w,x,y,z) IPCNetStartPlaybackAtTimeR(w,x,y,z,0)
//*************************************************************************************



//单文件录像回放
//回放音视频接口
/*
//type含义如下
if(type<20){
	//H624
}else if(type>=20 && type<=30){
	//audio
}else if(type>=50){
	//h265
}
//timestamp 毫秒为单位，表示从开始播放的时长，当前播放位置的时间戳
*/
typedef void(*OnPlaybackAudioVideoDataCallBack_t)(const char*uuid,int type,unsigned char*data,int len,long timestamp);
int __declspec(dllexport) _stdcall IPCNetSetPlaybackAudioVideoDataCallBack(const char* uuid,OnPlaybackAudioVideoDataCallBack_t cb);
//start play back, the video will come from OnPlaybackAudioVideoDataCallBack_t which set by IPCNetSetPlaybackAudioVideoDataCallBack
int __declspec(dllexport) _stdcall IPCNetStartPlaybackR(const char* uuid,const char* path,OnCmdResult_t r);
#define IPCNetStartPlayback(x,y) IPCNetStartPlaybackR(x,y,0)
//playback stop
int __declspec(dllexport) _stdcall IPCNetStopPlaybackR(const char* uuid,OnCmdResult_t r);
#define IPCNetStopPlayback(x) IPCNetStopPlaybackR(x,0)
//playback pause
int __declspec(dllexport) _stdcall IPCNetPausePlaybackR(const char* uuid,OnCmdResult_t r);
#define IPCNetPausePlayback(x) IPCNetPausePlaybackR(x,0)
#define IPCNetRestorePlaybackAfterPauseR(x,y) IPCNetPausePlaybackR(x,y)
#define IPCNetRestorePlaybackAfterPause(x) IPCNetPausePlaybackR(x,0)






//fast forward
int __declspec(dllexport) _stdcall IPCNetPlaybackFastForwardR(const char* uuid,int speed,OnCmdResult_t r);
#define IPCNetPlaybackFastForward(x,y) IPCNetPlaybackFastForwardR(x,y,0)
//fast backward
int __declspec(dllexport) _stdcall IPCNetPlaybackFastBackwardR(const char* uuid,int speed,OnCmdResult_t r);
#define IPCNetPlaybackFastBackward(x,y) IPCNetPlaybackFastBackwardR(x,y,0)
//playback seek
//seek to timestamp micro second position
//For example, video file is 5 minutes long, and want to seek to 60s, then give timestamp to 60*1000000
int __declspec(dllexport) _stdcall IPCNetPlaybackSeekR(const char* uuid,long timestamp,OnCmdResult_t r);
#define IPCNetPlaybackSeek(x,y) IPCNetPlaybackSeekR(x,y,0)

//timestamp millisecond, just for reference, not the actual time
//call IPCNetSetSnapshotCallBack to register snapshot callback when status return ERROR_SEP2P_SUCCESSFUL
//type -> VideoAudioCodeType_e, jpg or H264/H265 IFrame
typedef void(*OnSnapshotCallBack_t)(const char*uuid,int type,unsigned char*data,int len, unsigned long long timestamp);
int __declspec(dllexport) _stdcall IPCNetSetSnapshotCallBack(const char* uuid,OnSnapshotCallBack_t cb);
int __declspec(dllexport) _stdcall IPCNetSnapshot(const char* uuid, int vi);

//降噪接口
//
int __declspec(dllexport) _stdcall IPCNetGetDenoiseSettingR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetDenoiseSetting(x) IPCNetGetDenoiseSettingR(x,0)
int __declspec(dllexport) _stdcall IPCNetSetDenoiseSettingR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetDenoiseSetting(x,y) IPCNetSetDenoiseSettingR(x,y,0)


//
typedef void* RecSess_t;
/*
//only mp4 support!
例子请见sample_code.cpp 的 video record start 
startVideoRecord/putVideoAudioData
*/
typedef enum IPCNET_VIDEO_ENCODE_TYPE{
	IPCNET_VIDEO_ENCODE_TYPE_H264 = 0,
	IPCNET_VIDEO_ENCODE_TYPE_MJPG = 1,
	IPCNET_VIDEO_ENCODE_TYPE_H265 = 2,
}IPCNET_VIDEO_ENCODE_TYPE_et;
typedef enum IPCNET_AUDIO_ENCODE_TYPE{
	IPCNET_AUDIO_ENCODE_TYPE_G711U = 0,
	IPCNET_AUDIO_ENCODE_TYPE_G711A = 1,
	IPCNET_AUDIO_ENCODE_TYPE_PCM = 2,
	IPCNET_AUDIO_ENCODE_TYPE_AAC = 3,
}IPCNET_AUDIO_ENCODE_TYPE_et;
////0:h264,1:mjpg,2:h265 and so on.
//0:g711u,1:g711a,2:pcm,3:aac
//for example: ret = IPCNetStartRecordLocalVideo(path, (IPCNET_VIDEO_ENCODE_TYPE_et)video_type, framerate,
//		IPCNET_AUDIO_ENCODE_TYPE_PCM, 8000, 16, 1);
RecSess_t __declspec(dllexport) _stdcall IPCNetStartRecordLocalVideo(const char*path, IPCNET_VIDEO_ENCODE_TYPE_et videoType, int fps,
	IPCNET_AUDIO_ENCODE_TYPE_et audioType, int sampleRate, int bitWidth, int channels);
//视频真正开始录制之前，需要输入PPS/SPS/VPS，也可以输入PPS/SPS/VPS与I帧复合的视频帧
//最好第一帧是PPS/SPS/VPS与I帧复合的视频帧，这样录制的视频开始时间才是及时生效的，否则会等待PPS/SPS/VPS以及I帧的到来
//I帧就是关键帧IDR
//type请参考VideoAudioCodeType_e, type必须为视频帧
int __declspec(dllexport) _stdcall IPCNetPutLocalRecordVideoFrame(RecSess_t session, int type, const char*data, int len, unsigned long long timestamp);
//type请参考VideoAudioCodeType_e, type必须为音频帧
int __declspec(dllexport) _stdcall IPCNetPutLocalRecordAudioFrame(RecSess_t session, int type, const char*data, int len, unsigned long long timestamp);
void __declspec(dllexport) _stdcall IPCNetFinishLocalRecord(RecSess_t session);

#ifndef __int8_t_defined
#define __int8_t_defined
typedef signed char             int8_t;
typedef short int               int16_t;
typedef int                     int32_t;
#endif
/***G711A to PCM***/
int16_t __declspec(dllexport) _stdcall IPCNetALawDecode(int8_t number);
/***PCM to G711A***/
int8_t __declspec(dllexport) _stdcall IPCNetALawEncode(int16_t number);
/***G711U to PCM***/
int16_t __declspec(dllexport) _stdcall IPCNetMuLawDecode(int8_t number);
/**PCM to G711u**/
int8_t __declspec(dllexport) _stdcall IPCNetMuLawEncode(int16_t number);


//IPCNET_BINARY_DATA_MSG_REQ
#define IPCNetSendCustomDataToDevR(uuid, data, len, r) IPCNetSendBinaryDataR(uuid, IPCNET_BINARY_DATA_MSG_REQ , data, len, r)
#define IPCNetSendCustomDataToDev(uuid, data, len) IPCNetSendBinaryDataR(uuid, IPCNET_BINARY_DATA_MSG_REQ , data, len, 0)

#ifdef __cplusplus
}
#endif

#endif
