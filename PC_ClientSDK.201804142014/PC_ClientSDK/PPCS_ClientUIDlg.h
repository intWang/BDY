// PPCS_ClientUIDlg.h : header file
//

#pragma once

#include "IPCNetManagerInterface.h"
#include "JSONStructProtocal.h"
#include "afxwin.h"


// CPPCS_ClientUIDlg dialog
class CPicture;
class IPCNetManager;
class CPPCS_ClientUIDlg : public CDialog
{
// Construction
public:
	CPPCS_ClientUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_PPCS_CLIENTUI_DIALOG };

	
	void onStatus0(const char* uuid,int status);
	void onVideoData0(const char* uuid,int type,unsigned char*data,int len,long timestamp);
	void onAudioData0(const char* uuid,int type,unsigned char*data,int len,long timestamp);
	void onJSONString0(const char* uuid,int msg_type,const char* jsonstr);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	
// Implementation
protected:
	HICON m_hIcon;
	INT32 m_handleSession;
	IPCNetEventHandler mIPCNetEventHandler;

	DWORD m_threadID_RecvAVData;
	bool  m_bRunning;
	bool  m_bStartAudio, m_bStartVideo;

	DWORD m_threadID_RecvIOData;
	bool m_bIOCtrlRunning;

	int   m_nInitH264Decoder;
	int   m_framePara[4];
	BITMAPINFOHEADER m_bmiHead;
	BYTE  *m_pBufBmp24;


	CPicture *m_pic;
	CDC		 *m_pVideoDC;
	CRect	 m_rectVideo;

	void InitUI();
	void DispPic(BYTE *pData, BITMAPINFOHEADER *pBmiHead);
	void DispPic(BYTE *pData, int nSize);
	void PlayAudio_pcm(BYTE *pData, int nDataSize);
	void PlayAudio_adpcm(BYTE *pData, int nDataSize);

	DWORD static WINAPI myThreadRecvIOCtrl(void* arg);
	DWORD static WINAPI myThreadRecvAVData(void* arg);
	void  myDoIOCtrl(INT32 SessionHandle, CHAR *pData);
	void  myDoAudioData(CHAR *pData,int len);
	void  myDoVideoData(CHAR *pData,int len);
	//INT32 myGetDataSizeFrom(st_AVStreamIOHead *pStreamIOHead);

	void Logout();
	void BtnSwitch();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUpdateLog(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()


public:
	CEdit m_ctrlLog;
	CStatic m_ctlVideo;
	CButton m_btnStart;
	CButton m_btnStop;
	CButton m_ctlChkAudio;
	CButton m_ctlChkVideo;
	IPCNetVideoEncodeCfg_st mIPCNetVideoEncodeCfg;

	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedStop();
	afx_msg void OnDestroy();
	
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton12();
	void SetLog(LPCTSTR lpText);
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedCheck3();
	CButton mMobileEnableChk;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	CButton mWiFiChk;
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButtonGetOsd();
	CButton mShowDevNameOSDChk;
	afx_msg void OnBnClickedButtonSetOsd();
	CEdit mDevNameOSDXEdit;
	CEdit mDevNameOSDYEdit;
	afx_msg void OnBnClickedButtonGetAlarm();
	afx_msg void OnBnClickedButtonSetAlarm();
	CButton mEnableAlarmChk;
	CButton mEnableAlarmRecordChk;
	CButton mEnableAlarmSnapshotChk;
};
