#pragma once
#include "IServer.h"
namespace ls
{
    class ServerCallBackEngine :public ICallBackEngin
    {
    public:
        ServerCallBackEngine();
        ~ServerCallBackEngine();

        virtual bool initialize() override;
        virtual void destroy() override;
        virtual IIPCNetServerCallBack::Ptr GetIPCNetCallBack() override;
        virtual IHintCallBack::Ptr GetHintCallBack() override;

    protected:
        IIPCNetServerCallBack::Ptr m_pIPCNetCB = nullptr;
        IHintCallBack::Ptr m_pHintCB = nullptr;
    };

}