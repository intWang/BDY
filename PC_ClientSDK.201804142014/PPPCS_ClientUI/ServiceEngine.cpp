#include "ServiceEngine.h"
#include "IPCNetServer.h"
#include "HintServer.h"
namespace ls
{
    IServiceEngine::Ptr CreateServiceEngine()
    {
        return std::make_shared<ServiceEngine>();
    }

    ServiceEngine::ServiceEngine()
    {
    }


    ServiceEngine::~ServiceEngine()
    {
    }

    bool ServiceEngine::initialize()
    {
        m_pIPCNetServer = ls::CreateIPCNetServer();
        m_pHintServer = ls::CreateHintServer();
        m_pHttpSupport = ls::CreateHttpSupport();
        if (m_pIPCNetServer)
        {
            m_pIPCNetServer->initialize();
        }
        if (m_pHintServer)
        {
            m_pHintServer->initialize();
        }
        return true;
    }

    void ServiceEngine::destroy()
    {
    }

    IIPCNetServer::Ptr ServiceEngine::GetIPCNetServer()
    {
        return m_pIPCNetServer;
    }

    ls::IHintServer::Ptr ServiceEngine::GetHintServer()
    {
        return m_pHintServer;
    }

    IHttpSupport::Ptr ServiceEngine::GetHttpSupport()
    {
        return m_pHttpSupport;
    }

}
