#pragma once
#include <memory>
#include <string>
#include <functional>
#include "DataStruct.h"
//Local Service
namespace ls
{
    class ICallback :public std::enable_shared_from_this<ICallback>
    {
    public:
        struct CB
        {
            using Ptr = std::shared_ptr<CB>;
            virtual ~CB() {};
        };
        virtual ~ICallback() = default;
        virtual void Register(CB::Ptr func) = 0;
        virtual void UnRegister(CB::Ptr func) = 0;
    };

    enum class ReceivePriority
    {
        High = 0,
        Normal,
        Low
    };

    enum class HintLevel
    {
        Info = 0,
        Warning,
        Error,
    };

    using ICallbackWeakPtr = std::weak_ptr<ICallback>;
    class IService : public std::enable_shared_from_this<IService>
    {
    public:
        using Ptr = std::shared_ptr<IService>;

        virtual bool initialize() = 0;
        virtual void destroy() = 0;

        virtual bool addCallback(ICallbackWeakPtr callback, ReceivePriority priority = ReceivePriority::Normal) = 0;
        virtual bool removeCallback(ICallbackWeakPtr callback) = 0;

        virtual ~IService() = default;
    };

    class IIPCNetServerCallBack :public ICallback
    {
    public:
        struct CallBackFunc :public ICallback::CB
        {
            using Ptr = std::shared_ptr<CallBackFunc>;
            std::function<void(const DeviceData&)> funcOnDeviceConnected = nullptr;
            virtual ~CallBackFunc() {};
        };

        using Ptr = std::shared_ptr<IIPCNetServerCallBack>;
        virtual void OnDeviceConnected(const std::string& strDevJsonInfo) = 0;
    private:
    };

    class IHintCallBack :public ICallback
    {
    public:
        struct CallBackFunc :public ICallback::CB
        {
            using Ptr = std::shared_ptr<CallBackFunc>;
            std::function<void(const std::string&, ls::HintLevel)> funcOnUserHint = nullptr;
            virtual ~CallBackFunc() {};
        };

        using Ptr = std::shared_ptr<IHintCallBack>;
        virtual void OnUserHint(const std::string& strHintInfo, ls::HintLevel level) = 0;

    private:
    };


    class IIPCNetServer :public IService
    {
    public:
        using Ptr = std::shared_ptr<IIPCNetServer>;

        virtual void ConnectDevice(std::string& strUid, std::string& strPwd) = 0;
        virtual void DisconnectDevice(std::string& strUid) = 0;
        virtual void VideoControl(std::string& strUid, bool bStatu, int param1 = 0) = 0;
        virtual void AudioControl(std::string& strUid, bool bStatu, int param1 = 0) = 0;
    private:
    };


    class IHintServer :public IService
    {
    public:
        using Ptr = std::shared_ptr<IHintServer>;

        virtual void OnUserOperateHint(const std::string& strHintInfo, HintLevel level) = 0;
    private:
    };


    class IServiceEngine : public std::enable_shared_from_this<IServiceEngine>
    {
    public:
        using Ptr = std::shared_ptr<IServiceEngine>;

        virtual bool initialize() = 0;
        virtual void destroy() = 0;
        virtual IIPCNetServer::Ptr GetIPCNetServer() = 0;
        virtual IHintServer::Ptr GetHintServer() = 0;
    };

    class ICallBackEngin :public std::enable_shared_from_this<ICallBackEngin>
    {
    public:
        using Ptr = std::shared_ptr<ICallBackEngin>;

        virtual bool initialize() = 0;
        virtual void destroy() = 0;
        virtual IIPCNetServerCallBack::Ptr GetIPCNetCallBack() = 0;
        virtual IHintCallBack::Ptr GetHintCallBack() = 0;
    };

    IIPCNetServer::Ptr CreateIPCNetServer();
    IIPCNetServerCallBack::Ptr CreateIPCNetServerCB();
    IHintServer::Ptr CreateHintServer();
    IHintCallBack::Ptr CreateHintServerCB();

    IServiceEngine::Ptr CreateServiceEngine();
    ICallBackEngin::Ptr CreateCallbackEngine();
}
extern ls::IServiceEngine::Ptr g_pEngine;
extern ls::ICallBackEngin::Ptr g_pCallBack;
