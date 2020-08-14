#include "HintServer.h"


namespace ls
{
    IHintServer::Ptr CreateHintServer()
    {
        return std::make_shared<HintServer>();
    }

    IHintCallBack::Ptr CreateHintServerCB()
    {
        return std::make_shared<HintCallBack>();
    }

    HintServer::HintServer()
    {
    }


    HintServer::~HintServer()
    {
    }

    bool HintServer::initialize()
    {
        m_pHintCallBack = CreateHintServerCB();
        addCallback(m_pHintCallBack);
        return true;
    }

    void HintServer::destroy()
    {
        return ;
    }

    void HintServer::OnUserOperateHint(const std::string& strHintInfo, HintLevel level)
    {
        fireNotification(std::bind(&IHintCallBack::OnUserHint, std::placeholders::_1, strHintInfo, level));
    }

    void HintCallBack::OnUserHint(const std::string& strHintInfo, HintLevel level)
    {
        for (auto func : m_CBFunc)
        {
            if (func && func->funcOnUserHint)
            {
                func->funcOnUserHint(strHintInfo, level);
            }
        }
    }

    void HintCallBack::Register(CB::Ptr func)
    {
        auto pCBFunc = std::dynamic_pointer_cast<IHintCallBack::CallBackFunc>(func);
        if (pCBFunc)
        {
            m_CBFunc.push_back(pCBFunc);
        }
    }

    void HintCallBack::UnRegister(CB::Ptr func)
    {
        auto pCBFunc = std::dynamic_pointer_cast<IHintCallBack::CallBackFunc>(func);
        for (auto iterFunc = m_CBFunc.begin(); iterFunc != m_CBFunc.end(); ++iterFunc)
        {
            if (*iterFunc == pCBFunc)
            {
                m_CBFunc.erase(iterFunc);
                break;
            }
        }
    }
}
