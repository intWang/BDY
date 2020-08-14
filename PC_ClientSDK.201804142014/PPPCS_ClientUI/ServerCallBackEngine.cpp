#include "ServerCallBackEngine.h"


namespace ls
{

    ICallBackEngin::Ptr CreateCallbackEngine()
    {
        return std::make_shared<ServerCallBackEngine>();
    }


    ServerCallBackEngine::ServerCallBackEngine()
    {
    }


    ServerCallBackEngine::~ServerCallBackEngine()
    {
    }

    bool ServerCallBackEngine::initialize()
    {
        m_pIPCNetCB = ls::CreateIPCNetServerCB();
        m_pHintCB = ls::CreateHintServerCB();

        auto pIPCServer = g_pEngine ? g_pEngine->GetIPCNetServer():nullptr;
        if (pIPCServer)
        {
            pIPCServer->addCallback(m_pIPCNetCB);
        }

        auto pHintServer = g_pEngine ? g_pEngine->GetHintServer() : nullptr;
        if (pHintServer)
        {
            pHintServer->addCallback(m_pHintCB);
        }
        return true;
    }

    void ServerCallBackEngine::destroy()
    {
        auto pIPCServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
        if (pIPCServer)
        {
            pIPCServer->removeCallback(m_pIPCNetCB);
        }

        auto pHintServer = g_pEngine ? g_pEngine->GetHintServer() : nullptr;
        if (pHintServer)
        {
            pHintServer->removeCallback(m_pHintCB);
        }
        m_pIPCNetCB = nullptr;
        m_pHintCB = nullptr;
    }

    ls::IIPCNetServerCallBack::Ptr ServerCallBackEngine::GetIPCNetCallBack()
    {
        return m_pIPCNetCB;
    }

    ls::IHintCallBack::Ptr ServerCallBackEngine::GetHintCallBack()
    {
        return m_pHintCB;
    }

}