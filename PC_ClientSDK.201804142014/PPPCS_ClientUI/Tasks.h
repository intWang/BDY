#pragma once
#include "IPCNetManagerInterface.h"
#include <memory>
#include <string>

namespace ls
{
    class ITask
    {
    public:
        using Ptr = std::shared_ptr<ITask>;
        virtual void Run() = 0;
        virtual int Exec() { return 0; };
    protected:
        std::string m_strUID;
    };

    class ConnectTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<ConnectTask>;
        ConnectTask(std::string& strUID, std::string& strPwd, IPCNetEventHandler* pIPCNetEventHandler);
        virtual void Run() override;
    protected:
        std::string m_strPwd;
        IPCNetEventHandler* m_pIPCNetEventHandler;
    };

    class DisconnectTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<DisconnectTask>;
        DisconnectTask(std::string& strUID);
        virtual void Run() override;
    };

    class AudioTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<AudioTask>;
        AudioTask(std::string& strUID, bool bStatu);
        virtual void Run() override;
    private:
        bool m_bStatu = 0;
    };

    class VideoTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<VideoTask>;
        VideoTask(std::string& strUID, bool bStatu, OnCmdResult_t pCB, int param1 = 0);
        virtual void Run() override;
    private:
        bool m_bStatu = 0;
        int m_param1 = 0;
        OnCmdResult_t m_pCB = nullptr;
    };


    class VideoParamTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<VideoParamTask>;
        VideoParamTask(std::string& strUID, bool bSet, IIPCNetServer::VideoParam tagParam, OnCmdResult_t pCB, int nParamValue = 0);
        virtual void Run() override;
        virtual int Exec() override;
    private:
        IIPCNetServer::VideoParam m_tagParam;
        int m_nParamValue = 0;
        OnCmdResult_t m_pCB = nullptr;
        bool m_bSet = false;
    };

    class DeviceRestartTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<DeviceRestartTask>;
        DeviceRestartTask(std::string& strUID, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
    };

    class DeviceResetTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<DeviceResetTask>;
        DeviceResetTask(std::string& strUID, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
    };

    class GetDeviceTimeTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<GetDeviceTimeTask>;
        GetDeviceTimeTask(std::string& strUID, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
    };

    class GetWifiTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<GetWifiTask>;
        GetWifiTask(std::string& strUID, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
    };

    class SearchWifiTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<SearchWifiTask>;
        SearchWifiTask(std::string& strUID, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
    };

    class SetWifiTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<SetWifiTask>;
        SetWifiTask(std::string& strUid, std::string& strSSID, std::string& strPwd, std::string& strEncType, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
        std::string m_strSSID;
        std::string m_strPwd;
        std::string m_strEncType;
    };

    class ChangeDevPwdTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<ChangeDevPwdTask>;
        ChangeDevPwdTask(std::string& strUID, std::string& strPwd, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
        std::string m_strPwd;
    };

    class HotSpotTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<HotSpotTask>;
        HotSpotTask(std::string& strUID, OnCmdResult_t pCB, const char* pJsonData = nullptr);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
        std::string m_strJsonParam;
    };

    class NetStrategyTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<NetStrategyTask>;
        NetStrategyTask(std::string& strUID, OnCmdResult_t pCB, const char* pJsonData = nullptr);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
        std::string m_strJsonParam;
    };

    class ResolutionTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<ResolutionTask>;
        //nResolution - 0 标清，1 高清，2 4K超清
        ResolutionTask(std::string& strUID, bool bSet, int nResolution, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
        int m_nResolution;
        bool m_bSet;
    };

    class FlipMirrorTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<FlipMirrorTask>;
        FlipMirrorTask(std::string& strUID, bool bSet, int nFlip, int nMirror, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
        int m_nFlip;
        int m_nMirror;
        bool m_bSet;
    };

    class SwitchStreamTask : public ITask
    {
    public:
        using Ptr = std::shared_ptr<SwitchStreamTask>;
        SwitchStreamTask(std::string& strUID, int nStream, OnCmdResult_t pCB);
        virtual void Run() override;
    private:
        OnCmdResult_t m_pCB = nullptr;
        int m_nStream;
    };
}
