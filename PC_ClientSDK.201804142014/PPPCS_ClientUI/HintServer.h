#pragma once

#include "IServer.h"
#include "Service.hpp"
namespace ls
{
    class HintServer:public Service<IHintServer, IHintCallBack>
    {
    public:
        HintServer();
        ~HintServer();

        virtual bool initialize() override;
        virtual void destroy() override;
        virtual void OnUserOperateHint(const std::string& strHintInfo, HintLevel level) override;

    protected:
        ls::IHintCallBack::Ptr m_pHintCallBack = nullptr;

    };

    class HintCallBack :public IHintCallBack
    {
    public:
        using Ptr = std::shared_ptr<IHintCallBack>;

        virtual void OnUserHint(const std::string& strHintInfo, HintLevel level) override;
        virtual void Register(CB::Ptr func) override;
        virtual void UnRegister(CB::Ptr func) override;

    protected:
        std::vector<IHintCallBack::CallBackFunc::Ptr> m_CBFunc;
    };


}

