#include "PreviewRealWnd.h"
#include "OpenGLDrawWnd.h"
PreviewRealWnd::PreviewRealWnd(int nIndex, QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);

    SetArea(0, 30);
    SetAreaBk(s_qcl292C39, s_qcl292C39, s_qcl292C39);

    m_DrawWnd = new DrawWnd(nIndex, this);
    if (m_DrawWnd)
    {
        ui.mainLayout->addWidget(m_DrawWnd);
        m_DrawWnd->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    auto pBottomBar = GetBottomWnd();
    if (pBottomBar)
    {
        ui.mainLayout->addWidget(pBottomBar);
        pBottomBar->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    m_CallBackFunc = std::make_shared<ls::IIPCNetServerCallBack::CallBackFunc>();
    if (m_CallBackFunc)
    {
        m_CallBackFunc->funcOnFrameData = std::bind(&PreviewRealWnd::OnFrameData, this, std::placeholders::_1, std::placeholders::_2);
    }

    SetRuningStatu(Status::Empty);
}

PreviewRealWnd::~PreviewRealWnd()
{
}

void PreviewRealWnd::OnFrameData(std::string strUid, FrameData::Ptr pFrame)
{
    SetRuningStatu(Status::InPreview);
    //TODO
}

void PreviewRealWnd::StartPreview(ChannelNode::Ptr pChannel)
{
    if (m_CallBackFunc && pChannel)
    {
        SetRuningStatu(Status::StartingPreview);
        LogInfo("start preview %s", pChannel->strUID.c_str());
        m_pChannel = pChannel;
        auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
        if (pIPCCallBack)
        {
            pIPCCallBack->Register(m_CallBackFunc);
        }

        auto pIPCServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
        if (pIPCServer)
        {
            pIPCServer->VideoControl(pChannel->strUID, true);
        }
    }
}

void PreviewRealWnd::StopPreview()
{
    if (m_CallBackFunc && m_pChannel && (GetRuningStatu() == Status::InPreview || GetRuningStatu() == Status::StartingPreview))
    {
        auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
        if (pIPCCallBack)
        {
            pIPCCallBack->UnRegister(m_CallBackFunc);
        }

        auto pIPCServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
        if (pIPCServer)
        {
            pIPCServer->VideoControl(m_pChannel->strUID, false);
        }
        SetRuningStatu(Status::Empty);
    }
}

void PreviewRealWnd::Clear()
{

}

void PreviewRealWnd::SetSelectStatu(bool bSelect)
{
    m_bSelected = bSelect;
    if (m_DrawWnd)
    {
        m_DrawWnd->SetSelected(bSelect);
    }
}

bool PreviewRealWnd::GetSelectStatu()
{
    return m_bSelected;
}

void PreviewRealWnd::SetRuningStatu(Status state)
{
    m_curStatus = state;
    switch (m_curStatus)
    {
    case PreviewRealWnd::Empty:
        m_tmBusy = 0;
        break;
    case PreviewRealWnd::StartingRecord:
    case PreviewRealWnd::StartingPreview:
        m_tmBusy = time(0);
        break;
    case PreviewRealWnd::InPreview:
    case PreviewRealWnd::Record:
        break;
    default:
        break;
    }
}

PreviewRealWnd::Status PreviewRealWnd::GetRuningStatu()
{
    return m_curStatus;
}


time_t PreviewRealWnd::GetBusyTime()
{
    return m_tmBusy;
}

void PreviewRealWnd::mousePressEvent(QMouseEvent *event)
{
    emit PreviewWndUserClick();
}
