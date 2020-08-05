#ifndef _SEP2P_API___INC_H_
#define _SEP2P_API___INC_H_

#ifdef _WIN32
#ifdef LIBFBP2P_EXPORTS
#define SEP2P_API_API __declspec(dllexport)
#else
#define SEP2P_API_API __declspec(dllimport)
#endif
#endif //// #ifdef WIN32DLL

#if defined(__APPLE__) && defined(__GNUC__)
#define __MAC_IOS_
#elif defined(__MACOSX__)
#define __MAC_IOS_
#endif

#if defined(LINUX) || defined(__MAC_IOS_) || defined(__linux__)
#include <netinet/in.h>
#define SEP2P_API_API 
#endif //// #ifdef LINUX

//#ifdef _ARC_COMPILER
//#include "net_api.h"
//#define SEP2P_API_API 
//#endif //// #ifdef _ARC_COMPILER

#include "SEP2P_Type.h"
#include "SEP2P_Error.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


typedef struct{	
	CHAR bFlagInternet;		// Internet Reachable? 1: YES, 0: NO
	CHAR bFlagHostResolved;	// P2P Server IP resolved? 1: YES, 0: NO
	CHAR bFlagServerHello;	// P2P Server Hello? 1: YES, 0: NO
	CHAR NAT_Type;			// NAT type, 0: Unknow, 1: IP-Restricted Cone type,   2: Port-Restricted Cone type, 3: Symmetric 
	CHAR MyLanIP[128];		// My LAN IP. If (bFlagInternet==0) || (bFlagHostResolved==0) || (bFlagServerHello==0), MyLanIP will be "0.0.0.0"
	CHAR MyWanIP[128];		// My Wan IP. If (bFlagInternet==0) || (bFlagHostResolved==0) || (bFlagServerHello==0), MyWanIP will be "0.0.0.0"
} st_SEP2P_NetInfo;

typedef struct{
	INT32  Skt;					// Sockfd
	struct sockaddr_in RemoteAddr;	// Remote IP:Port
	struct sockaddr_in MyLocalAddr;	// My Local IP:Port
	struct sockaddr_in MyWanAddr;	// My Wan IP:Port
	UINT32 ConnectTime;				// Connection build in ? Sec Before
	CHAR DID[24];					// Device ID
	CHAR bCorD;						// I am Client or Device, 0: Client, 1: Device
	CHAR bMode;						// Connection Mode: 0: P2P, 1:Relay Mode
	CHAR devType;
	CHAR Reserved;				
} st_SEP2P_Session;

enum SEP2PDeviceType
{
	SEP2P_CLIENT_TYPE_USER = 1,
	SEP2P_CLIENT_TYPE_CAMERA,
	SEP2P_CLIENT_TYPE_WIFI_COM_CONTROLLER,
};

SEP2P_API_API UINT32 SEP2P_GetAPIVersion(void);
SEP2P_API_API INT32 SEP2P_QueryDID(const CHAR* DeviceName, CHAR* DID, INT32 DIDBufSize);
SEP2P_API_API INT32 SEP2P_Initialize(CHAR *Parameter);
SEP2P_API_API INT32 SEP2P_DeInitialize(void);
SEP2P_API_API INT32 SEP2P_SetLoginInfo(CHAR *username,CHAR*passwd,INT32 type);
SEP2P_API_API INT32 SEP2P_NetworkDetect(st_SEP2P_NetInfo *NetInfo, UINT16 UDP_Port);
SEP2P_API_API INT32 SEP2P_Share_Bandwidth(CHAR bOnOff);
SEP2P_API_API INT32 SEP2P_Listen(const CHAR *MyID, const UINT32 TimeOut_Sec, UINT16 UDP_Port, CHAR bEnableInternet, const CHAR* APILicense);
SEP2P_API_API INT32 SEP2P_Listen_Break(void);
SEP2P_API_API INT32 SEP2P_LoginStatus_Check(CHAR* bLoginStatus);
SEP2P_API_API INT32 SEP2P_Connect(const CHAR *TargetID, CHAR bEnableLanSearch, UINT16 UDP_Port);
SEP2P_API_API INT32 SEP2P_Connect_Break(const CHAR *TargetID);
SEP2P_API_API INT32 SEP2P_Check(INT32 SessionHandle, st_SEP2P_Session *SInfo);
SEP2P_API_API INT32 SEP2P_Close(INT32 SessionHandle);
SEP2P_API_API INT32 SEP2P_ForceClose(INT32 SessionHandle);
SEP2P_API_API INT32 SEP2P_Write(INT32 SessionHandle, UCHAR Channel, CHAR *DataBuf, INT32 DataSizeToWrite);
SEP2P_API_API INT32 SEP2P_Read(INT32 SessionHandle, UCHAR Channel, CHAR *DataBuf, INT32 *DataSize, UINT32 TimeOut_ms);
SEP2P_API_API INT32 SEP2P_Check_Buffer(INT32 SessionHandle, UCHAR Channel, UINT32 *WriteSize, UINT32 *ReadSize);
#ifdef __cplusplus
}
#endif // __cplusplus
#endif ////#ifndef _SEP2P_API___INC_H_