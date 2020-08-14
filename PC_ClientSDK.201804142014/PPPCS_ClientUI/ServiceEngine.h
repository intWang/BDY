#pragma once
#include "IServer.h"
namespace ls
{
    class ServiceEngine :public IServiceEngine
    {
    public:
        ServiceEngine();
        ~ServiceEngine();

        virtual bool initialize() override;
        virtual void destroy() override;
        virtual IIPCNetServer::Ptr GetIPCNetServer() override;
        virtual IHintServer::Ptr GetHintServer() override;

    protected:
        IIPCNetServer::Ptr m_pIPCNetServer;
        IHintServer::Ptr m_pHintServer;
    };
}

