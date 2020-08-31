#pragma once
#include "IServer.h"
#include "Service.hpp"
#include <thread>
#include <condition_variable>
#include <queue>
#include "Tasks.h"
#include "Decode.h"
namespace ls
{
    class IPCNetServer :public Service<IIPCNetServer, IIPCNetServerCallBack>
    {
    public:
        IPCNetServer();
        ~IPCNetServer();

        virtual bool initialize() override;
        virtual void destroy() override;
        virtual void ConnectDevice(std::string& strUid, std::string& strPwd) override;
        virtual void DisconnectDevice(std::string& strUid) override;
        virtual void VideoControl(std::string& strUid, bool bStatu, int param1 = 0) override;
        virtual void AudioControl(std::string& strUid, bool bStatu, int param1 = 0) override;
        virtual int VideoParamCtrl(std::string& strUid, bool bSet, VideoParam tagParam, int nParamValue) override;

        virtual void onStatus(const char* uuid, int status) override;
        virtual void onVideoData(const char* uuid, int type, unsigned char*data, int len, long timestamp) override;
        virtual void onAudioData(const char* uuid, int type, unsigned char*data, int len, long timestamp) override;
        virtual void onJSONString(const char* uuid, int msg_type, const char* jsonstr) override;
        virtual void OnDecodeCallBack(const char* pUserCode, const unsigned char* pBuf, int width, int height, int len);
        virtual void OnDecodeStatuCallBack(const char* pUserCode, DecodeStatus emStatus, void* pParam);
        virtual void OnNetOperteResult(int cmd, const char*uuid, const char*json) override;

        virtual void RestartDevice(std::string& strUid) override;
        virtual void ResetDevice(std::string& strUid) override;
        virtual void GetDevTime(std::string& strUid) override;

        virtual void SearchWifi(std::string& strUid) override;
        virtual void SetWifi(std::string& strUid, std::string& strSSID, std::string& strPwd, std::string& strEncType) override;
        virtual void ChangeDevPwd(std::string& strUid, std::string& strPwd) override;
        virtual void GetHotSpot(std::string& strUid) override;
        virtual void SetHotSpot(std::string& strUid, std::string& strJsonParam) override;
        virtual void GetNetStrategy(std::string& strUid) override;
        virtual void SetNetStrategy(std::string& strUid, std::string& strJsonParam) override;
        virtual void GetClarity(std::string& strUid) override;
        virtual void SetClarity(std::string& strUid, int nClarity) override;
        virtual void GetFlipMirror(std::string& strUid) override;
        virtual void SetFlipMirror(std::string& strUid, int nFilp, int nMirror) override;
        virtual void SwitchStream(std::string& strUid, int nStream) override;
        virtual void RecordControl(std::string& strUi, std::string& strName, bool bStart) override;

    protected:
        void PrepareDecoder(const std::string& strUid);
        void RecycleDecoder(const std::string& strUid);
        //void OnRecordStatuCB(const std::string & strUID, IRecoder::RecordStatu emStatu);
        void onParamCmd(std::string& strUid, IPCNetCamColorCfg_st& stIPCNetCamColorCfg);
        void onDevTimeCmd(std::string& strUid, IPCNetTimeCfg_st& stIPCNetTimeCfg);
        void onSearchWifiCmd(std::string& strUid, IPCNetWifiAplist::Ptr& stNetWirelessConfig);
        void onGetWifiCmd(std::string& strUid, IPCNetWirelessConfig_st::Ptr& pData);
        void onGetHotSpotCmd(std::string& strUid, IPCNetWiFiAPInfo_t::Ptr& pData);
        void onGetNetStrategy(std::string& strUid, IPCNetNetworkStrategy::Ptr& pData);
        void onGetClarityCmd(std::string& strUid, IPCNetStreamInfo::Ptr& pData);
        void onGetFlipMirrorCmd(std::string& strUid, IPCNetPicOverTurn::Ptr& pData);
        void OnHintMsg(std::string& strHint, ls::HintLevel emLevel = HintLevel::Info);
        void AddTask(ITask::Ptr ptask);
        ITask::Ptr PopTask();
        void Work();
    protected:
        std::atomic<bool> m_Quit{ false };
        std::map<std::string, int> m_RuningVideoTask;
        std::thread m_thWork;
        std::condition_variable m_covQuit;
        std::mutex m_mxWorkThread;
        std::mutex m_mxTaskList;
        std::mutex m_mxDecoder;
        std::queue<ITask::Ptr> m_queWorkList;
        IPCNetEventHandler m_IPCNetEventHandler;
        std::map<std::string, IDecoder::Ptr> m_videoDecoder;
    };

    class IPCNetServerCallBack : public ls::IIPCNetServerCallBack
    {
    public:
        using Ptr = std::shared_ptr<IPCNetServerCallBack>;
        using WeakPtr = std::weak_ptr<IPCNetServerCallBack>;
        virtual void Register(CB::Ptr func) override;
        virtual void UnRegister(CB::Ptr func) override;

        virtual void OnDeviceConnected(const std::string& strDevJsonInfo) override;
        virtual void OnDeviceStatuChanged(const std::string& strUid, int nStatu) override;
        virtual void OnVideo(const std::string& strUid, const unsigned char*data, int width, int height, int len, long timestamp) override;
        virtual void onParamCmd(const std::string& strUid, const IPCNetCamColorCfg_st& stParam) override;
        virtual void onDevTimeCmd(const std::string& strUid, const IPCNetTimeCfg_st& stData) override;
        virtual void onSearchWifiCmd(std::string& strUid, const IPCNetWifiAplist::Ptr& stNetWirelessConfig) override;
        virtual void onGetWifiCmd(std::string& strUid, const IPCNetWirelessConfig_st::Ptr& pData) override;
        virtual void onGetHotSpotCmd(std::string& strUid, const IPCNetWiFiAPInfo_t::Ptr& pData) override;
        virtual void onGetNetStrategy(std::string& strUid, const IPCNetNetworkStrategy::Ptr& pData) override;
        virtual void onGetClarityCmd(std::string& strUid, const IPCNetStreamInfo::Ptr& pData) override;
        virtual void onGetFlipMirrorCmd(std::string& strUid, const IPCNetPicOverTurn::Ptr& pData) override;
        virtual void OnRecordStatuNotify(const std::string& strUid, bool bStart) override;

    protected:
        void DispatchVideoData(const std::string& strUid, const unsigned char*data, int width, int height, int len);
    protected:
        std::vector<IIPCNetServerCallBack::CallBackFunc::Ptr> m_CBFunc;

    private:
        std::mutex m_mxLostTrack;
        std::vector<std::string> m_LostDevice;
    };


}
