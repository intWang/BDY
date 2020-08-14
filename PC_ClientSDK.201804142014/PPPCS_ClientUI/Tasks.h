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
        VideoTask(std::string& strUID, bool bStatu, int param1 = 0);
        virtual void Run() override;
    private:
        bool m_bStatu = 0;
        int m_param1 = 0;
    };

}
