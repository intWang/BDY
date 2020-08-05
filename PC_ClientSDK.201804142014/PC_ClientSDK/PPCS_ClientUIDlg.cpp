// PPCS_ClientUIDlg.cpp : implementation file
//

#include "stdafx.h"

#include "JSONStructProtocal.h"
#include "IPCNetManagerInterface.h"
#include "SEP2P_API.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "PPCS_ClientUI.h"
#include "PPCS_ClientUIDlg.h"
#include <WinSock2.h>

#include "Picture.h"
#include "wave_out.h"
#include "H264CodecLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CPPCS_ClientUIDlg*lCPPCS_ClientUIDlg;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

//=====================================================






//--define---------------------------------------------
#define MAX_SIZE_BUF	64*1024

#define CHANNEL_DATA	1
#define CHANNEL_IOCTRL	0
#define TIME_SPAN_IDLE	200 //in ms

#define OMSG_UPDATE_LOG		WM_USER+10
#define WPARAM_LOGIN_OK		1
#define WPARAM_LOGIN_FAIL	2


CPPCS_ClientUIDlg::CPPCS_ClientUIDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPPCS_ClientUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_handleSession		 =-1;
	m_threadID_RecvAVData=0L;
	m_threadID_RecvIOData=0L;

	m_bStartVideo	=false;
	m_bStartAudio	=false;
	m_bRunning		=false;
	
	m_rectVideo.SetRect(0,0,0,0);
	m_framePara[0]=m_framePara[1]=0;
	m_framePara[2]=m_framePara[3]=0;
	m_nInitH264Decoder=-1;
	m_pBufBmp24=new BYTE[1024*1024*1.5];

	m_bmiHead.biSize  = sizeof(BITMAPINFOHEADER);
	m_bmiHead.biWidth = 0;
	m_bmiHead.biHeight= 0;
	m_bmiHead.biPlanes= 1;
	m_bmiHead.biBitCount = 24;
	m_bmiHead.biCompression = BI_RGB;
	m_bmiHead.biSizeImage   = 0;
	m_bmiHead.biXPelsPerMeter = 0;
	m_bmiHead.biYPelsPerMeter = 0;
	m_bmiHead.biClrUsed       = 0;
	m_bmiHead.biClrImportant  = 0;

	m_pic	  =NULL;
}

void CPPCS_ClientUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ctrlLog);
	DDX_Control(pDX, IDC_VIDEO, m_ctlVideo);
	DDX_Control(pDX, IDC_START, m_btnStart);
	DDX_Control(pDX, IDC_STOP, m_btnStop);
	DDX_Control(pDX, IDC_CHECK1, m_ctlChkAudio);
	DDX_Control(pDX, IDC_CHECK2, m_ctlChkVideo);
	DDX_Control(pDX, IDC_CHECK3, mMobileEnableChk);
	DDX_Control(pDX, IDC_CHECK4, mWiFiChk);
	DDX_Control(pDX, IDC_CHECK_DEV_NAME, mShowDevNameOSDChk);
	DDX_Control(pDX, IDC_EDIT_DEV_NAME_OSD_X, mDevNameOSDXEdit);
	DDX_Control(pDX, IDC_EDIT_DEV_NAME_OSD_Y, mDevNameOSDYEdit);
	DDX_Control(pDX, IDC_CHECK_ENABLE_ALARM, mEnableAlarmChk);
	DDX_Control(pDX, IDC_CHECK_ALARM_RECORD, mEnableAlarmRecordChk);
	DDX_Control(pDX, IDC_CHECK_ALARM_SNAPSHOT, mEnableAlarmSnapshotChk);
}

BEGIN_MESSAGE_MAP(CPPCS_ClientUIDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START, &CPPCS_ClientUIDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDC_STOP, &CPPCS_ClientUIDlg::OnBnClickedStop)
	ON_WM_DESTROY()
	ON_MESSAGE(OMSG_UPDATE_LOG, OnUpdateLog)

	ON_BN_CLICKED(IDC_CHECK1, &CPPCS_ClientUIDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CPPCS_ClientUIDlg::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPPCS_ClientUIDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPPCS_ClientUIDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON12, &CPPCS_ClientUIDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CPPCS_ClientUIDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CPPCS_ClientUIDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CPPCS_ClientUIDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON3, &CPPCS_ClientUIDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_CHECK3, &CPPCS_ClientUIDlg::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_BUTTON5, &CPPCS_ClientUIDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &CPPCS_ClientUIDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &CPPCS_ClientUIDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CPPCS_ClientUIDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CPPCS_ClientUIDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &CPPCS_ClientUIDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &CPPCS_ClientUIDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CPPCS_ClientUIDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON_GET_OSD, &CPPCS_ClientUIDlg::OnBnClickedButtonGetOsd)
	ON_BN_CLICKED(IDC_BUTTON_SET_OSD, &CPPCS_ClientUIDlg::OnBnClickedButtonSetOsd)
	ON_BN_CLICKED(IDC_BUTTON_GET_ALARM, &CPPCS_ClientUIDlg::OnBnClickedButtonGetAlarm)
	ON_BN_CLICKED(IDC_BUTTON_SET_ALARM, &CPPCS_ClientUIDlg::OnBnClickedButtonSetAlarm)
END_MESSAGE_MAP()


// CPPCS_ClientUIDlg message handlers

BOOL CPPCS_ClientUIDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	lCPPCS_ClientUIDlg=this;

	mIPCNetEventHandler.onAudioData=onAudioData;
	mIPCNetEventHandler.onJSONString=onJSONString;
	mIPCNetEventHandler.onStatus=onStatus;
	mIPCNetEventHandler.onVideoData=onVideoData;

	IPCNetInitialize("");

	CWnd *pWnd=GetDlgItem(IDC_VIDEO);
	if(pWnd) m_pVideoDC=pWnd->GetDC();
	m_ctlVideo.GetClientRect(&m_rectVideo);
	//m_rectVideo.right=3*m_rectVideo.Height()/4;

	Set_WIN_Params(INVALID_FILEDESC, 8000.0f, 16, 1);
	m_nInitH264Decoder=InitCodec(1);

	InitUI();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPPCS_ClientUIDlg::OnDestroy()
{
	CDialog::OnDestroy();
	Logout();
	
	IPCNetDeInitial();
	
	if(m_pic!=NULL) {
		delete m_pic;
		m_pic=NULL;
	}

	WIN_Audio_close();
	if(m_nInitH264Decoder>=0) {
		UninitCodec();
		m_nInitH264Decoder=-1;
	}
	if(m_pBufBmp24) {
		delete []m_pBufBmp24;
		m_pBufBmp24=NULL;
	}
}



void CPPCS_ClientUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPPCS_ClientUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPPCS_ClientUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CPPCS_ClientUIDlg::SetLog(LPCTSTR lpText)
{
	CString csText;
	GetDlgItemText(IDC_EDIT1, csText);
	csText+=lpText;
	SetDlgItemText(IDC_EDIT1, csText);
	m_ctrlLog.LineScroll(m_ctrlLog.GetLineCount(), 0);
}

LRESULT CPPCS_ClientUIDlg::OnUpdateLog(WPARAM wParam, LPARAM lParam)
{

	return 0L;
}

void CPPCS_ClientUIDlg::InitUI()
{
	UINT32 APIVersion=0;
	CString csText;
	INT32 nRet=0;

	IPCNetGetAPIVersion(&APIVersion);

	csText.Format("SEP2P_API Version: %d.%d.%d.%d\n", (APIVersion & 0xFF000000)>>24, (APIVersion & 0x00FF0000)>>16, (APIVersion & 0x0000FF00)>>8, (APIVersion & 0x000000FF) >> 0 );
	SetDlgItemText(IDC_APIVER, csText);
	
	st_SEP2P_NetInfo NetInfo;
	nRet = SEP2P_NetworkDetect(&NetInfo,0);
	
	switch(NetInfo.NAT_Type)
	{
		case 0:
			csText="Unknow";
			break;
		case 1:
			csText="IP-Restricted Cone";
			break;
		case 2:
			csText="Port-Restricted Cone";
			break;
		case 3:
			csText="Symmetric";
			break;
		default:;
	}

	TRACE("  -------------- NetInfo: -------------------\n");
	TRACE("  Internet Reachable     : %s\n", (NetInfo.bFlagInternet == 1) ? "YES":"NO");
	TRACE("  P2P Server IP resolved : %s\n", (NetInfo.bFlagHostResolved == 1) ? "YES":"NO");
	TRACE("  P2P Server Hello Ack   : %s\n", (NetInfo.bFlagServerHello == 1) ? "YES":"NO");
	TRACE("  Local NAT Type         : %s\n", csText);
	TRACE("  My Wan IP : %s\n", NetInfo.MyWanIP);
	TRACE("  My Lan IP : %s\n", NetInfo.MyLanIP);

	CString csTmp;
	csTmp.Format("  My Wan IP : %s\r\n", NetInfo.MyWanIP); SetLog(csTmp);
	csTmp.Format("  My Lan IP : %s\r\n", NetInfo.MyLanIP); SetLog(csTmp);
	csTmp.Format("  Local NAT Type: %d: %s\r\n", NetInfo.NAT_Type, csText); SetLog(csTmp);

	BtnSwitch();
}


void CPPCS_ClientUIDlg::DispPic(BYTE *pData, BITMAPINFOHEADER *pBmiHead)
{
	if(m_pic==NULL) m_pic=new CPicture();

	if(m_pic->PushData(pData, pBmiHead)) {
		m_pic->Show(m_pVideoDC, m_rectVideo);
	}
}

void CPPCS_ClientUIDlg::DispPic(BYTE *pData, int nSize)
{
	if(m_pic==NULL) m_pic=new CPicture();

	if(m_pic->PushData(pData, nSize)) {		
		m_pic->Show(m_pVideoDC, m_rectVideo);
	}
}


//=={{audio: ADPCM codec==============================================================
INT32 g_nAudioPreSample=0;
INT32 g_nAudioIndex=0;

static int gs_index_adjust[8]= {-1,-1,-1,-1,2,4,6,8};
static int gs_step_table[89] = 
{
	7,8,9,10,11,12,13,14,16,17,19,21,23,25,28,31,34,37,41,45,
	50,55,60,66,73,80,88,97,107,118,130,143,157,173,190,209,230,253,279,307,337,371,
	408,449,494,544,598,658,724,796,876,963,1060,1166,1282,1411,1552,1707,1878,2066,
	2272,2499,2749,3024,3327,3660,4026,4428,4871,5358,5894,6484,7132,7845,8630,9493,
	10442,11487,12635,13899,15289,16818,18500,20350,22385,24623,27086,29794,32767
};

void Encode(unsigned char *pRaw, int nLenRaw, unsigned char *pBufEncoded)
{
	short *pcm = (short *)pRaw;
	int cur_sample;
	int i;
	int delta;
	int sb;
	int code;
	nLenRaw >>= 1;

	for(i = 0; i<nLenRaw; i++)
	{
		cur_sample = pcm[i]; 
		delta = cur_sample - g_nAudioPreSample;
		if (delta < 0){
			delta = -delta;
			sb = 8;
		}else sb = 0;

		code = 4 * delta / gs_step_table[g_nAudioIndex];	
		if (code>7)	code=7;

		delta = (gs_step_table[g_nAudioIndex] * code) / 4 + gs_step_table[g_nAudioIndex] / 8;
		if(sb) delta = -delta;

		g_nAudioPreSample += delta;
		if (g_nAudioPreSample > 32767) g_nAudioPreSample = 32767;
		else if (g_nAudioPreSample < -32768) g_nAudioPreSample = -32768;

		g_nAudioIndex += gs_index_adjust[code];
		if(g_nAudioIndex < 0) g_nAudioIndex = 0;
		else if(g_nAudioIndex > 88) g_nAudioIndex = 88;

		if(i & 0x01) pBufEncoded[i>>1] |= code | sb;
		else pBufEncoded[i>>1] = (code | sb) << 4;
	}
}

void Decode(char *pDataCompressed, int nLenData, char *pDecoded)
{
	int i;
	int code;
	int sb;
	int delta;
	short *pcm = (short *)pDecoded;
	nLenData <<= 1;

	for(i=0; i<nLenData; i++)
	{
		if(i & 0x01) code = pDataCompressed[i>>1] & 0x0f;
		else code = pDataCompressed[i>>1] >> 4;

		if((code & 8) != 0) sb = 1;
		else sb = 0;
		code &= 7;

		delta = (gs_step_table[g_nAudioIndex] * code) / 4 + gs_step_table[g_nAudioIndex] / 8;
		if(sb) delta = -delta;

		g_nAudioPreSample += delta;
		if(g_nAudioPreSample > 32767) g_nAudioPreSample = 32767;
		else if (g_nAudioPreSample < -32768) g_nAudioPreSample = -32768;

		pcm[i] = g_nAudioPreSample;
		g_nAudioIndex+= gs_index_adjust[code];
		if(g_nAudioIndex < 0) g_nAudioIndex = 0;
		if(g_nAudioIndex > 88) g_nAudioIndex= 88;
	}
}
//==}}audio: ADPCM codec==============================================================

#define READSIZE_EVERY_TIME	2560
UCHAR g_out_pBufTmp[READSIZE_EVERY_TIME];

void CPPCS_ClientUIDlg::PlayAudio_adpcm(BYTE *pData, int nDataSize)
{
	int nSize=0;
	char bufTmp[640];
	for(int i=0; i<nDataSize/160; i++){
		Decode((char *)pData+i*160, 160, bufTmp);
		memcpy(g_out_pBufTmp+nSize, bufTmp, 640);
		nSize+=640;
	}

	WIN_Play_Samples(g_out_pBufTmp, nSize);
}

void CPPCS_ClientUIDlg::PlayAudio_pcm(BYTE *pData, int nDataSize)
{
	WIN_Play_Samples(pData, nDataSize);
}

/*INT32 CPPCS_ClientUIDlg::myGetDataSizeFrom(st_AVStreamIOHead *pStreamIOHead)
{
	INT32 nDataSize=pStreamIOHead->nStreamIOHead;
	nDataSize &=0x00FFFFFF;
	return nDataSize;
}*/

void CPPCS_ClientUIDlg::myDoAudioData(CHAR *pData,int len)
{
	//st_AVFrameHead stFrameHead;
	//int nLenFrameHead=sizeof(st_AVFrameHead);
	//memcpy(&stFrameHead, pData, nLenFrameHead);
	//switch(stFrameHead.nCodecID)
	//{
	//	case CODECID_A_PCM:
	//		PlayAudio_pcm((BYTE *)&pData[nLenFrameHead], stFrameHead.nDataSize);
	//		break;

	//	case CODECID_A_ADPCM:{
				//TRACE("myThreadRecvAVData: stFrameHead.nDataSize=%d\n", stFrameHead.nDataSize);
	//			PlayAudio_adpcm((BYTE *)&pData[nLenFrameHead], stFrameHead.nDataSize);
	//		}
	//		break;
	//	default:;
	//}
	PlayAudio_adpcm((BYTE *)pData, len);
}

void CPPCS_ClientUIDlg::myDoVideoData(CHAR *pData,int len)
{
#if 1
	static BOOL bFirstFrame=TRUE;
	//st_AVFrameHead stFrameHead;
	//int nLenFrameHead=sizeof(st_AVFrameHead);
	//memcpy(&stFrameHead, pData, nLenFrameHead);
	//switch(stFrameHead.nCodecID)
	//{
		//case CODECID_V_H264:
			//Decode
			if(m_nInitH264Decoder>=0){
				TRACE("m_nInitH264Decoder\n");
				//TRACE("myDoVideoData(.): stFrameHead.flag=%d\n", stFrameHead.flag);
				//if(bFirstFrame && stFrameHead.flag!=VFRAME_FLAG_I) break;
				bFirstFrame=false;

				bool bChgedWidth  =0;
				int consumed_bytes=0;
				int nFrameSize=len;//stFrameHead.nDataSize;
				BYTE *pFrame=(BYTE *)(pData);

				while(nFrameSize>0){
AGAIN_DECODER_NAL:
					TRACE("H264Decode\n");
					consumed_bytes=H264Decode(m_pBufBmp24, pFrame, nFrameSize, m_framePara, 1);
					if(consumed_bytes<0){
						nFrameSize=0;
						break;
					}
					if(!m_bRunning) break;
					
					if(m_framePara[0]>0){
						if(m_framePara[2]>0 && m_framePara[2]!=m_bmiHead.biWidth){
							if(m_bmiHead.biWidth!=0) bChgedWidth=1;

							m_bmiHead.biWidth		=m_framePara[2];
							m_bmiHead.biHeight		=m_framePara[3];
							m_bmiHead.biSizeImage	=m_framePara[2]*m_framePara[3]*3;

							TRACE("  myDoVideoData(..): DecoderNal(.)>=0, %dX%d, pFrame[2,3,4,5]=%X,%X,%X,%X\n",
									m_framePara[2], m_framePara[3], pFrame[2],pFrame[3],pFrame[4],pFrame[5]);
							if(bChgedWidth) goto AGAIN_DECODER_NAL; //goto-------------------------------------
						}
						TRACE("DispPic\n");
						DispPic(m_pBufBmp24, &m_bmiHead);
						//TRACE("DispPic=%d ms \n", (GetTickCount()-test3));
					}
					nFrameSize-=consumed_bytes;
					if(nFrameSize>0) memcpy(pFrame, pFrame+consumed_bytes, nFrameSize);
					else nFrameSize=0;
				}//while--end
			}
	//		break;
	//	default:;
	//}
#endif
}
/*
void CPPCS_ClientUIDlg::myDoVideoData(CHAR *pData,int len)
{
#if 0
	static BOOL bFirstFrame=TRUE;
	//st_AVFrameHead stFrameHead;
	//int nLenFrameHead=sizeof(st_AVFrameHead);
	//memcpy(&stFrameHead, pData, nLenFrameHead);
	//switch(stFrameHead.nCodecID)
	//{
		//case CODECID_V_H264:
			//Decode
			if(m_nInitH264Decoder>=0){
				//TRACE("myDoVideoData(.): stFrameHead.flag=%d\n", stFrameHead.flag);
				//if(bFirstFrame && stFrameHead.flag!=VFRAME_FLAG_I) break;
				bFirstFrame=false;

				bool bChgedWidth  =0;
				int consumed_bytes=0;
				int nFrameSize=len;//stFrameHead.nDataSize;
				BYTE *pFrame=(BYTE *)(pData);

				while(nFrameSize>0){
			AGAIN_DECODER_NAL:
					consumed_bytes=H264Decode(m_pBufBmp24, pFrame, nFrameSize, m_framePara, 1);
					if(consumed_bytes<0){
						nFrameSize=0;
						break;
					}
					if(!m_bRunning) break;
					
					if(m_framePara[0]>0){
						if(m_framePara[2]>0 && m_framePara[2]!=m_bmiHead.biWidth){
							if(m_bmiHead.biWidth!=0) bChgedWidth=1;

							m_bmiHead.biWidth		=m_framePara[2];
							m_bmiHead.biHeight		=m_framePara[3];
							m_bmiHead.biSizeImage	=m_framePara[2]*m_framePara[3]*3;

							TRACE("  myDoVideoData(..): DecoderNal(.)>=0, %dX%d, pFrame[2,3,4,5]=%X,%X,%X,%X\n",
									m_framePara[2], m_framePara[3], pFrame[2],pFrame[3],pFrame[4],pFrame[5]);
							if(bChgedWidth) goto AGAIN_DECODER_NAL; //goto-------------------------------------
						}
						DispPic(m_pBufBmp24, &m_bmiHead);
						//TRACE("DispPic=%d ms \n", (GetTickCount()-test3));
					}
					nFrameSize-=consumed_bytes;
					if(nFrameSize>0) memcpy(pFrame, pFrame+consumed_bytes, nFrameSize);
					else nFrameSize=0;
				}//while--end
			}
	//		break;
	//	default:;
	//}
#endif
}*/


void CPPCS_ClientUIDlg::OnBnClickedStart()
{
	CString csUID, csTmp;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	IPCNetStartIPCNetSession((char*)csUID.GetBuffer(),"admin",&mIPCNetEventHandler);
	csUID.ReleaseBuffer();
}

void CPPCS_ClientUIDlg::OnBnClickedCheck1() //audio
{
	CString csUID, csTmp;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	if(m_ctlChkAudio.GetCheck()){
		IPCNetStartAudio((char*)csUID.GetBuffer());
		m_bStartAudio=true;
	}else{
		IPCNetStopAudio((char*)csUID.GetBuffer());
		m_bStartAudio=false;
	}

	csUID.ReleaseBuffer();
}

void CPPCS_ClientUIDlg::OnBnClickedCheck2()//video
{
	CString csUID, csTmp;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	if(m_ctlChkVideo.GetCheck()){
		IPCNetStartVideo((char*)csUID.GetBuffer(),1);
		m_bStartVideo=true;
	}else{
		IPCNetStopVideo((char*)csUID.GetBuffer());
		m_bStartVideo=false;
	}

	csUID.ReleaseBuffer();
}

void CPPCS_ClientUIDlg::OnBnClickedStop()
{
	Logout();
	BtnSwitch();
}

void CPPCS_ClientUIDlg::Logout()
{
	CString csUID, csTmp;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(!csUID.IsEmpty()){
		m_bRunning   =false;
		IPCNetStopIPCNetSession((char*)csUID.GetBuffer());
	
		csUID.ReleaseBuffer();
	}
}

void CPPCS_ClientUIDlg::BtnSwitch()
{
	if(m_bRunning){
		m_ctlChkAudio.EnableWindow(SW_SHOW);
		m_ctlChkVideo.EnableWindow(SW_SHOW);
		m_btnStop.EnableWindow(SW_SHOW);
	}else{
		m_ctlChkAudio.SetCheck(FALSE);
		m_ctlChkVideo.SetCheck(FALSE);
		m_ctlChkAudio.EnableWindow(SW_HIDE);
		m_ctlChkVideo.EnableWindow(SW_HIDE);
		m_btnStop.EnableWindow(SW_HIDE);
	}
}

#define MAX_LOG_BUF_SIZE 4096
void Log(const char * format, ...)
{
	va_list vlArgs;
	TCHAR tzText[4096]={0};
	va_start(vlArgs, format);
	wvsprintf(tzText, format, vlArgs);
	OutputDebugString(tzText);
	va_end(vlArgs);
}

void CPPCS_ClientUIDlg::onStatus0(const char* uuid,int status)
{
	//TRACE("uuid:%s status:%d\n",uuid,status);
	switch(status){
	case ERROR_SEP2P_SUCCESSFUL:
		//SetDlgItemText(IDC_STATIC_STATUS, "online");
		m_bRunning=true;
		BtnSwitch();
		//UpdateDevStatus(uuid,"online");

		//get video setting
		//mIPCNetManager.GetBrightness(uuid);
		IPCNetGetBrightness(uuid);
		break;
	case ERROR_SEP2P_INVALID_ID:
		//SetDlgItemText(IDC_STATIC_STATUS, "invalid id");
		//UpdateDevStatus(uuid,"invalid id");
		break;
	case ERROR_SEP2P_STATUS_INVALID_PASSWD:
		m_bRunning=false;
		//UpdateDevStatus(uuid,"wrong password");
		break;
	default:
		//SetDlgItemText(IDC_STATIC_STATUS, "offline");
		if(m_bRunning==true){
			m_bRunning=false;
			BtnSwitch();
		}
		//TRACE("%s:%d\n",__func__,__LINE__);
		//UpdateDevStatus(uuid,"offline");
		//TRACE("%s:%d\n",__func__,__LINE__);
	}
}
void CPPCS_ClientUIDlg::onVideoData0(const char* uuid,int type,unsigned char*data,int len,long timestamp)
{
	TRACE("uuid:%s video len:%d\n",uuid,len);
	myDoVideoData((CHAR*)data,len);
}
void CPPCS_ClientUIDlg::onAudioData0(const char* uuid,int type,unsigned char*data,int len,long timestamp)
{
	TRACE("uuid:%s audio len:%d\n",uuid,len);
	myDoAudioData((CHAR*)data,len);
}


void CPPCS_ClientUIDlg::onJSONString0(const char* uuid,int msg_type,const char* jsonstr)
{
	TRACE("uuid:%s msg_type:%d json:%s\n",uuid,msg_type,jsonstr);
	switch(msg_type){
	case IPCNET_GET_CAM_PIC_CFG_RESP:
		//mCtrlSliderBrightness.SetPos(IPCNetGetBrightness(uuid));
		//mCtrlSliderContrast.SetPos(IPCNetGetContrast(uuid));
		//mCtrlSliderSaturation.SetPos(IPCNetGetSaturation(uuid));
		//mCtrlSliderAcutance.SetPos(IPCNetGetAcutance(uuid));
		//mCtrlSliderChroma.SetPos(IPCNetGetHue(uuid));
		//mCtrlSliderBitrate.SetRange(0,100);
		break;
	}
}

void onStatus(const char* uuid,int status){
	//TRACE("%s:%d %s\n",__func__,__LINE__,uuid);
	lCPPCS_ClientUIDlg->onStatus0(uuid,status);
	//TRACE("%s:%d %s\n",__func__,__LINE__,uuid);
}
void onVideoData(const char* uuid,int type,unsigned char*data,int len,long timestamp){
	lCPPCS_ClientUIDlg->onVideoData0(uuid,type,data,len,timestamp);
}
void onAudioData(const char* uuid,int type,unsigned char*data,int len,long timestamp){
	lCPPCS_ClientUIDlg->onAudioData0(uuid,type,data,len,timestamp);
}
void onJSONString(const char* uuid,int msg_type,const char* jsonstr)
{
	lCPPCS_ClientUIDlg->onJSONString0(uuid,msg_type,jsonstr);
}

void GetCameraPictureSettingCmdResult(int cmd,const char*uuid,const char*json)
{
	JSONObject jsdata(json);
	lCPPCS_ClientUIDlg->mIPCNetVideoEncodeCfg.parseJSON(jsdata);
}
void CPPCS_ClientUIDlg::OnBnClickedButton1()
{
	CString csUID, csTmp;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	IPCNetGetCameraPictureSettingR(csUID.GetBuffer(),GetCameraPictureSettingCmdResult);
	csUID.ReleaseBuffer();
}

void SetCameraPictureSettingOnCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton2()
{
	CString csUID, csTmp;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	String str;
	mIPCNetVideoEncodeCfg.VideoEncode[1]->Witdh=320;
	mIPCNetVideoEncodeCfg.VideoEncode[1]->Height=180;
	mIPCNetVideoEncodeCfg.toJSONString(str);
	TRACE("\n\n%s\n",str.data());
	IPCNetSetCameraPictureSettingR(csUID.GetBuffer(),str.data(),SetCameraPictureSettingOnCmdResult);
	csUID.ReleaseBuffer();
}

void RebootDeviceOnCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton12()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID, csTmp;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	IPCNetRebootDeviceR(csUID.GetBuffer(),RebootDeviceOnCmdResult);
	csUID.ReleaseBuffer();
}

IPCNetCamColorCfg_st mIPCNetCamColorCfg_st;
void GetBrightnessOnCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);

	JSONObject jsdata(json);
	mIPCNetCamColorCfg_st.parseJSON(jsdata);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton13()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	//Brightness,Contrast,Hue,Saturation,Acutance will feedback in this call
	IPCNetGetBrightnessR(csUID.GetBuffer(),GetBrightnessOnCmdResult);
	csUID.ReleaseBuffer();
}


void CPPCS_ClientUIDlg::OnBnClickedButton14()
{
	// TODO: 在此添加控件通知处理程序代码
	//String str;
	//mIPCNetCamColorCfg_st.toJSONString(str);
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	IPCNetSetHue(csUID.GetBuffer(),mIPCNetCamColorCfg_st.Chroma);
	csUID.ReleaseBuffer();
	IPCNetSetSaturation(csUID.GetBuffer(),mIPCNetCamColorCfg_st.Saturtion);
	csUID.ReleaseBuffer();
	IPCNetSetAcutance(csUID.GetBuffer(),mIPCNetCamColorCfg_st.Acutance);
	csUID.ReleaseBuffer();
	IPCNetSetContrast(csUID.GetBuffer(),mIPCNetCamColorCfg_st.Contrast);
	csUID.ReleaseBuffer();
}

void GetDevInfoOnCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton15()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	IPCNetGetDevInfoR(csUID.GetBuffer(),GetDevInfoOnCmdResult);
	csUID.ReleaseBuffer();
}

IPCNetMobileNetworkInfo_st ipcNetMobileNetworkInfo;
void GetMobileNetworkOnCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	JSONObject jsdata(json);
	ipcNetMobileNetworkInfo.parseJSON(jsdata);

	lCPPCS_ClientUIDlg->mMobileEnableChk.SetCheck(ipcNetMobileNetworkInfo.enable);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	IPCNetGetMobileNetworkR(csUID.GetBuffer(),GetMobileNetworkOnCmdResult);
	csUID.ReleaseBuffer();
}


void CPPCS_ClientUIDlg::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	ipcNetMobileNetworkInfo.enable=mMobileEnableChk.GetCheck();
	IPCNetSetMobileNetwork(csUID.GetBuffer(),mMobileEnableChk.GetCheck(),"");
	csUID.ReleaseBuffer();
}

IPCNetWifiAplist_st ipcNetWifiAplist_st;
void OnCmdSearchWiFiResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	JSONObject jsdata(json);
	ipcNetWifiAplist_st.parseJSON(jsdata);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	IPCNetSearchWiFiR(csUID.GetBuffer(),OnCmdSearchWiFiResult);
	csUID.ReleaseBuffer();
}


IPCNetWirelessConfig_st ipcNetWirelessConfig;
void OnCmdGetWiFiNetworkResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	JSONObject jsdata(json);
	ipcNetWirelessConfig.parseJSON(jsdata);

	lCPPCS_ClientUIDlg->mWiFiChk.SetCheck(ipcNetWirelessConfig.WirelessEnable);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	IPCNetGetWiFiR(csUID.GetBuffer(),OnCmdGetWiFiNetworkResult);
	csUID.ReleaseBuffer();
}

void OnCmdSetWiFiNetworkResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	JSONObject jsdata(json);
	ipcNetWirelessConfig.parseJSON(jsdata);

	lCPPCS_ClientUIDlg->mWiFiChk.SetCheck(ipcNetWirelessConfig.WirelessEnable);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	//ipcNetWirelessConfig.SSID=ipcNetWifiAplist_st.ApItem[0]->SSID;
	//ipcNetWirelessConfig.Password="12345678";
	//ipcNetWirelessConfig.EncType=ipcNetWifiAplist_st.ApItem[0]->EncType;
	IPCNetSetWiFiR(csUID.GetBuffer(),ipcNetWirelessConfig.SSID.data(),ipcNetWirelessConfig.Password.data(),ipcNetWirelessConfig.EncType.data(),OnCmdSetWiFiNetworkResult);
	csUID.ReleaseBuffer();
}

DevInfo searchdev;
void OnSearchDeviceResult(DevInfo*dev)
{
	CString csstr;
	csstr.Format("Got dev:%s ip:%s\n",dev->mUUID,dev->mIP);
	lCPPCS_ClientUIDlg->SetLog(csstr);

	memcpy(&searchdev,dev,sizeof(searchdev));
}
void CPPCS_ClientUIDlg::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	IPCNetSearchDevice(OnSearchDeviceResult);
}


void CPPCS_ClientUIDlg::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	//modify IP
	DevInfo dev;//this element must be copied from the Search runtine! mIPv4Addr will be used as a pointer of SOCKADDR_IN of the target device
	memcpy(&dev,&searchdev,sizeof(searchdev));
	IPCNetSetDeviceInfoInLAN(&dev);
}


void CPPCS_ClientUIDlg::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	IPCNetRebootDeviceInLAN(searchdev.mIP);
}

IPCNetTimeCfg_st ipcNetTimeCfg_st;
void OnCmdGetTimeCfgResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	JSONObject jsdata(json);
	ipcNetTimeCfg_st.parseJSON(jsdata);

	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
void CPPCS_ClientUIDlg::OnBnClickedButton10()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	IPCNetGetTimeR(csUID.GetBuffer(),OnCmdGetTimeCfgResult);
	csUID.ReleaseBuffer();
}

void OnCmdSetTimeCfgResult(int cmd,const char*uuid,const char*json)
{
	TRACE("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	
	CString csstr;
	csstr.Format("cmd:%d,uuid:%s JSON:%s\n",cmd,uuid,json);
	lCPPCS_ClientUIDlg->SetLog(csstr);
}
int translate_gmt(int time_zone)
{
	if(time_zone>0){
		return GMT_0+time_zone*2;
	}else if(time_zone<0){
		return 24+time_zone*2;
	}else
		return GMT_0;
}
void CPPCS_ClientUIDlg::OnBnClickedButton11()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	
	CTime time = CTime::GetCurrentTime();
	tm localtime;
	time.GetLocalTm(&localtime);
	tm utctime;
	time.GetGmtTm(&utctime);
	int time_zone = localtime.tm_hour-utctime.tm_hour;
	if(time_zone < -12){
		time_zone += 24;
	}else if(time_zone > 12){
		time_zone -= 24;
	}
	TRACE("timezone:%d tm_year:%d tm_mon:%d tm_yday:%d tm_hour:%d tm_min:%d tm_sec:%d\n",
		time_zone,utctime.tm_year,utctime.tm_mon,utctime.tm_mday,utctime.tm_hour,utctime.tm_min,utctime.tm_sec);


	IPCNetTimeCfg_t tmcfg;
	memset(&tmcfg,0,sizeof(tmcfg));
	tmcfg.Date.Day=utctime.tm_mday;
	tmcfg.Date.Mon=utctime.tm_mon;
	tmcfg.Date.Year=utctime.tm_year+1900;
	tmcfg.NtpEnable=ipcNetTimeCfg_st.NtpEnable;
	strncpy(tmcfg.NtpServ,ipcNetTimeCfg_st.NtpServ.data(),256);
	tmcfg.Time.Hour=utctime.tm_hour;
	tmcfg.Time.Min=utctime.tm_min;
	tmcfg.Time.Sec=utctime.tm_sec;
	tmcfg.TimeZone=translate_gmt(time_zone);
	IPCNetSetTimeR(csUID.GetBuffer(),&tmcfg,OnCmdSetTimeCfgResult);
	csUID.ReleaseBuffer();
}

IPCNetOsdCfg_st mIPCNetOsdCfg;
void OnGetOSDCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("uuid:%s cmd:%d json:%s\n",uuid,cmd,json);
	JSONObject jsdata(json);
	mIPCNetOsdCfg.parseJSON(jsdata);
}
void CPPCS_ClientUIDlg::OnBnClickedButtonGetOsd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	String str;
	IPCNetGetOsdCfg_st ipcNetGetOsdCfg;
	ipcNetGetOsdCfg.Vich=0;
	ipcNetGetOsdCfg.toJSONString(str);
	//TRACE("osd:%s\n",str.data());
	IPCNetGetOSDR(csUID.GetBuffer(),str.data(),OnGetOSDCmdResult);
	csUID.ReleaseBuffer();
}

void OnSetOSDCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("OnSetOSDCmdResult uuid:%s cmd:%d json:%s\n",uuid,cmd,json);

	IPCNetReleaseCmdResource( cmd,uuid,OnSetOSDCmdResult);
}
void CPPCS_ClientUIDlg::OnBnClickedButtonSetOsd()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	CString cstr;
	mIPCNetOsdCfg.OsdNameInfo->NameConf->Enable=mShowDevNameOSDChk.GetCheck()==BST_CHECKED;
	mDevNameOSDXEdit.GetWindowText(cstr);
	mIPCNetOsdCfg.OsdNameInfo->NameConf->Xcord=atoi(cstr);
	mDevNameOSDYEdit.GetWindowText(cstr);
	mIPCNetOsdCfg.OsdNameInfo->NameConf->Ycord=atoi(cstr);

	String str;
	mIPCNetOsdCfg.toJSONString(str);
	IPCNetSetOSDR(csUID.GetBuffer(),str.data(),OnSetOSDCmdResult);
	csUID.ReleaseBuffer();
}

IPCNETMoveAlarmCfg_st mIPCNETMoveAlarmCfg;
void OnGetAlarmCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("uuid:%s cmd:%d json:%s\n",uuid,cmd,json);
	JSONObject jsdata(json);
	mIPCNETMoveAlarmCfg.parseJSON(jsdata);


	IPCNetReleaseCmdResource( cmd,uuid,OnGetAlarmCmdResult);
}
void CPPCS_ClientUIDlg::OnBnClickedButtonGetAlarm()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}
	IPCNetGetAlarmR(csUID.GetBuffer(),OnGetAlarmCmdResult);
	csUID.ReleaseBuffer();
}

void OnSetAlarmCmdResult(int cmd,const char*uuid,const char*json)
{
	TRACE("uuid:%s cmd:%d json:%s\n",uuid,cmd,json);
	//JSONObject jsdata(json);

	IPCNetReleaseCmdResource(cmd,uuid,OnSetAlarmCmdResult);
}
void CPPCS_ClientUIDlg::OnBnClickedButtonSetAlarm()
{
	// TODO: 在此添加控件通知处理程序代码
	CString csUID;
	GetDlgItemText(IDC_COMBO1, csUID);
	if(csUID.IsEmpty()){
		MessageBox("UID must be input.", "Tips", MB_ICONINFORMATION|MB_OK);
		return;
	}

	mIPCNETMoveAlarmCfg.MoveInfo.MdEnable=mEnableAlarmChk.GetCheck()==BST_CHECKED;
	mIPCNETMoveAlarmCfg.PolicyInfo.RecordInfo.RecEnable=mEnableAlarmRecordChk.GetCheck()==BST_CHECKED;
	mIPCNETMoveAlarmCfg.PolicyInfo.SnapInfo.PushEnable=mEnableAlarmSnapshotChk.GetCheck()==BST_CHECKED;

	//全屏区域
	for(int i=0;i<mIPCNETMoveAlarmCfg.MoveInfo.AreaCount;i++)
		mIPCNETMoveAlarmCfg.MoveInfo.Area[i]=1;

	//为了保证报警开启，需要设置对应的执行时间，默认开一个全天的就行了
	mIPCNETMoveAlarmCfg.Week[0]->Flag=1;
	IPCNetTime_st *time=&mIPCNETMoveAlarmCfg.Week[0]->TimePeriod[0]->Start;
	time->Hour=0;
	time->Min=0;
	time->Sec=0;
	time=&mIPCNETMoveAlarmCfg.Week[0]->TimePeriod[0]->End;
	time->Hour=11;
	time->Min=59;
	time->Sec=59;

	String str;
	mIPCNETMoveAlarmCfg.toJSONString(str);
	IPCNetSetAlarmR(csUID.GetBuffer(),str.data(),OnSetAlarmCmdResult);
	csUID.ReleaseBuffer();
}
