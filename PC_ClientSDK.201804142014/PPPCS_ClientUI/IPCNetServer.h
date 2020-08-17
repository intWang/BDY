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

        virtual void onStatus(const char* uuid, int status) override;
        virtual void onVideoData(const char* uuid, int type, unsigned char*data, int len, long timestamp) override;
        virtual void onAudioData(const char* uuid, int type, unsigned char*data, int len, long timestamp) override;
        virtual void onJSONString(const char* uuid, int msg_type, const char* jsonstr) override;

    protected:
        void AddTask(ITask::Ptr ptask);
        ITask::Ptr PopTask();
        void Work();
    protected:
        std::atomic<bool> m_Quit{ false };
        std::vector<std::string> m_RuningVideoTask;
        std::mutex m_mxTmp;
        std::thread m_thWork;
        std::condition_variable m_covQuit;
        std::mutex m_mxTaskList;
        std::queue<ITask::Ptr> m_queWorkList;
        IPCNetEventHandler m_IPCNetEventHandler;
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
        virtual void OnVideo(const std::string& strUid, unsigned char*data, int len, long timestamp) override;
        virtual void OnDecodeCallBack(DeocdHandl handle, const unsigned char* pBuf, int width, int height, int len);
    protected:
        void PrepareDecoder(const std::string& strUid);
        void DispatchVideoData(const std::string& strUid, const unsigned char*data, int width, int height, int len);
    protected:
        std::vector<IIPCNetServerCallBack::CallBackFunc::Ptr> m_CBFunc;
        std::map<std::string, DeocdHandl> m_videoDecoder;
        std::map<DeocdHandl, std::string> m_decoderUser;

    };


}
