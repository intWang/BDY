#ifndef IPCNET_MANAGER_INTERFACE_H
#define IPCNET_MANAGER_INTERFACE_H

extern "C"{
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

typedef enum
{
	DEV_TYPE_SWITCH_AND_OUTLET = 0,
	DEV_TYPE_SWITCH,
	DEV_TYPE_OUTLET,//插座
	DEV_TYPE_SCENE_PANEL,
	DEV_TYPE_IR_CONTROL_UNIT,//红外转发器(遥控器)
	DEV_TYPE_ROUTER,
	DEV_TYPE_LIGHT,
	DEV_TYPE_LIGHT_SENSOR,
	DEV_TYPE_CURTAIN_CONTROLER,//窗帘控制器
	DEV_TYPE_TEMPERATURE_SENSOR,
	DEV_TYPE_PRESSURE_SENSOR,
	DEV_TYPE_FLOW_RATE_SENSOR,
	DEV_TYPE_CAMERA,
	DEV_TYPE_COMPUTER,
	DEV_TYPE_SPEAKER,
	DEV_TYPE_AIR_CONDITIONER_CONTROLLER,
	DEV_TYPE_INVALID,
}DEV_TYPE_e;

struct IPCNetEventHandler{
public:
	void (*onStatus)(const char* uuid,int status);
	void (*onVideoData)(const char* uuid,int type,unsigned char*data,int len,long timestamp);
	void (*onAudioData)(const char* uuid,int type,unsigned char*data,int len,long timestamp);
	void (*onJSONString)(const char* uuid,int msg_type,const char* jsonstr);
};

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
	char mTocken;
	char mUUID[64];
	char mCHCK[16];
	char mIP[128];
	char mSockIP[128];
	char mMAC[128];
	char mSoftVer[64];
	char mHwVer[64];
	int mIndex;
	char mNetMask[32];		//子网掩码
	char mGateway[128];		//网关
	char mDNS1[128];		//dns1
	char mDNS2[128];		//dns2
	unsigned short          nPort;			//设备端口
	char mDevName[128];		//设备名称
	char mUserName[128];		//修改时会对用户认证
	char mPassword[128];		//修改时会对用户认证
	int mDevType;
	char mFlashID[128];
	bool mFlashValid;
//#ifdef _WIN32
//	char mIPv4Addr[sizeof(SOCKADDR_IN)];//SOCKADDR_IN //struct sockaddr_in
//#endif
	char mIPv4Addr[128];
};
typedef void(*OnSearchDeviceResult_t)(DevInfo*dev);
typedef void(*OnLanSettingResult_t)(const char*ip,int ret);//成功ret==1
typedef void(*OnCmdResult_t)(int cmd,const char*uuid,const char*json);
void __declspec(dllexport) _stdcall IPCNetReleaseCmdResource(int cmd,const char*uuid,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetInitialize(const char*ssn);
int __declspec(dllexport) _stdcall IPCNetDeInitial(void);
int __declspec(dllexport) _stdcall IPCNetGetAPIVersion(unsigned int *version);

int __declspec(dllexport) _stdcall IPCNetSearchDevice(OnSearchDeviceResult_t osdr);
int __declspec(dllexport) _stdcall IPCNetStopSearchDevice();

int __declspec(dllexport) _stdcall IPCNetSetLanSettingResultCallback(OnLanSettingResult_t r);
int __declspec(dllexport) _stdcall IPCNetRebootDeviceInLAN(const char*ip);
int __declspec(dllexport) _stdcall IPCNetSetDeviceInfoInLAN(DevInfo *dev);
int __declspec(dllexport) _stdcall IPCNetSetDeviceDhcpInLAN();
/*
*h:此session的数据回调接口，回调有如许接口函数当有对应数据就会调用这个接口
*onStatus:session 状态回调，反应了session的运行状态
*onVideoData:视频数据回调
*onAudioData:音频数据回调
*onJSONString:命令数据回调，调用对应功能接口之后，会回调这个函数告知结果，功能函数有：
*	设置网络、设置图像、其他设置等等，想要得到结果，请在此函数等待数据
*/
int __declspec(dllexport) _stdcall IPCNetStartIPCNetSession(const char* uuid,const char* passwd,IPCNetEventHandler*h);
int __declspec(dllexport) _stdcall IPCNetStopIPCNetSession(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetIPCNetSessionStatus(const char* uuid);
/*
*cmd:功能命令
*json:命令的详细内容
*/
int __declspec(dllexport) _stdcall IPCNetSendJsonCmd(const char* uuid,int cmd ,const char*json);
int __declspec(dllexport) _stdcall IPCNetSendJsonCmdR(const char* uuid,int cmd ,const char*json,OnCmdResult_t r);
/*
*stream:0~1
*/
int __declspec(dllexport) _stdcall IPCNetStartVideo(const char* uuid,int stream);
int __declspec(dllexport) _stdcall IPCNetStartVideoR(const char* uuid,int stream,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetStopVideo(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetStopVideoR(const char* uuid,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetStartAudio(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetStartAudioR(const char* uuid,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetStopAudio(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetStopAudioR(const char* uuid,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetGetDevInfo(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetDevInfoR(const char* uuid,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetChangeDevPwd(const char* uuid,const char*pwd);
int __declspec(dllexport) _stdcall IPCNetChangeDevPwdR(const char* uuid,const char*pwd,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetRestoreToFactorySetting(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetRestoreToFactorySettingR(const char* uuid,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetSetResolution(const char* uuid,int res);
int __declspec(dllexport) _stdcall IPCNetSetResolutionR(const char* uuid,int res,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetResolution(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetResolutionR(const char* uuid,OnCmdResult_t r);
/*
*bri:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetBrightness(const char* uuid,int bri);
int __declspec(dllexport) _stdcall IPCNetSetBrightnessR(const char* uuid,int bri,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetBrightness(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetBrightnessR(const char* uuid,OnCmdResult_t r);
/*
*con:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetContrast(const char* uuid,int con);
int __declspec(dllexport) _stdcall IPCNetSetContrastR(const char* uuid,int con,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetContrast(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetContrastR(const char* uuid,OnCmdResult_t r);
/*
*hue:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetHue(const char* uuid,int hue);//chroma
int __declspec(dllexport) _stdcall IPCNetSetHueR(const char* uuid,int hue,OnCmdResult_t r);//chroma
int __declspec(dllexport) _stdcall IPCNetGetHue(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetHueR(const char* uuid,OnCmdResult_t r);
/*
*sat:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetSaturation(const char* uuid,int sat);
int __declspec(dllexport) _stdcall IPCNetSetSaturationR(const char* uuid,int sat,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetSaturation(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetSaturationR(const char* uuid,OnCmdResult_t r);
/*
*acu:0~255
*/
int __declspec(dllexport) _stdcall IPCNetSetAcutance(const char* uuid,int acu);
int __declspec(dllexport) _stdcall IPCNetSetAcutanceR(const char* uuid,int acu,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetAcutance(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetAcutanceR(const char* uuid,OnCmdResult_t r);
//恢复图像默认值
int __declspec(dllexport) _stdcall IPCNetSetCameraColorSettingDefault(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetSetCameraColorSettingDefaultR(const char* uuid,OnCmdResult_t r);
/*
*flip,mirror
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

int __declspec(dllexport) _stdcall IPCNetSetTime(const char* uuid,IPCNetTimeCfg_t *t);
int __declspec(dllexport) _stdcall IPCNetSetTimeR(const char* uuid,IPCNetTimeCfg_t *t,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetTime(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetTimeR(const char* uuid,OnCmdResult_t r);

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
*EncType请参考Search列表
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
int __declspec(dllexport) _stdcall IPCNetSetMobileNetwork(const char* uuid,int enable,const char* vpn);
int __declspec(dllexport) _stdcall IPCNetSetMobileNetworkR(const char* uuid,int enable,const char* vpn,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetGetMobileNetwork(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetMobileNetworkR(const char* uuid,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetGetIRMode(const char* uuid);
int __declspec(dllexport) _stdcall IPCNetGetIRModeR(const char* uuid,OnCmdResult_t r);
int __declspec(dllexport) _stdcall IPCNetSetIRMode(const char* uuid,const char*json);
int __declspec(dllexport) _stdcall IPCNetSetIRModeR(const char* uuid,const char*json,OnCmdResult_t r);

int __declspec(dllexport) _stdcall IPCNetGetOSDR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetGetOSD(x,y) IPCNetGetOSDR(x,y,0)
int __declspec(dllexport) _stdcall IPCNetSetOSDR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetOSD(x,y) IPCNetSetOSDR(x,y,0)

int __declspec(dllexport) _stdcall IPCNetGetAlarmR(const char* uuid,OnCmdResult_t r);
#define IPCNetGetAlarm(x) IPCNetGetAlarmR(x,0)
int __declspec(dllexport) _stdcall IPCNetSetAlarmR(const char* uuid,const char*json,OnCmdResult_t r);
#define IPCNetSetAlarm(x,y) IPCNetSetAlarmR(x,y,0)

}

#endif
