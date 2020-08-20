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
        virtual IHttpSupport::Ptr GetHttpSupport() override;
    protected:
        IIPCNetServer::Ptr m_pIPCNetServer = nullptr;
        IHintServer::Ptr m_pHintServer = nullptr;
        IHttpSupport::Ptr m_pHttpSupport = nullptr;
    };
}

