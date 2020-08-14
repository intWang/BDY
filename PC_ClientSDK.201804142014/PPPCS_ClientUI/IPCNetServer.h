#pragma once
#include "IServer.h"
#include "Service.hpp"
#include <thread>
#include <condition_variable>
#include <queue>
#include "Tasks.h"
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

    protected:
        void AddTask(ITask::Ptr ptask);
        ITask::Ptr PopTask();
        void Work();
    protected:
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
        virtual void OnDeviceConnected(const std::string& strDevJsonInfo) override;

        virtual void Register(CB::Ptr func) override;
        virtual void UnRegister(CB::Ptr func) override;
    protected:
        std::vector<IIPCNetServerCallBack::CallBackFunc::Ptr> m_CBFunc;

    };


}
