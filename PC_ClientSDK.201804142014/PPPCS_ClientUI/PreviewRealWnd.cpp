#include "PreviewRealWnd.h"
#include "OpenGLDrawWnd.h"
PreviewRealWnd::PreviewRealWnd(int nIndex, QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    //ui.setupUi(this);

    SetArea(0, 30);
    SetAreaBk(s_qcl292C39, s_qcl292C39, s_qcl444858);

    auto pMainLayout = MQ(QVBoxLayout)(this);
    m_DrawWnd = new DrawWnd(nIndex, this);
    if (m_DrawWnd)
    {
        pMainLayout->addWidget(m_DrawWnd);
        m_DrawWnd->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    if (auto pBottomBar = InitBottomBar())
    {
        pMainLayout->addWidget(pBottomBar);
        //pBottomBar->setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    pMainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pMainLayout);


    m_CallBackFunc = std::make_shared<ls::IIPCNetServerCallBack::CallBackFunc>();
    if (m_CallBackFunc)
    {
        m_CallBackFunc->funcOnFrameData = std::bind(&PreviewRealWnd::OnFrameData, this, std::placeholders::_1, std::placeholders::_2);
    }

    SetRuningStatu(Status::Empty);
}

PreviewRealWnd::~PreviewRealWnd()
{
    StopPreview();
}

void PreviewRealWnd::OnFrameData(const std::string& strUid, FrameData::Ptr pFrame)
{
    if (m_DrawWnd && m_pChannel &&( strUid == m_pChannel->strUID))
    {
        SetRuningStatu(Status::InPreview);
        m_DrawWnd->InputFrameData(pFrame);
    }
}

void PreviewRealWnd::StartPreview(ChannelNode::Ptr pChannel)
{
    if (GetRuningStatu() == Status::InPreview || GetRuningStatu() == Status::StartingPreview)
    {
        StopPreview();
    }
    if (pChannel)
    {
        SetRuningStatu(Status::StartingPreview);
        LogInfo("start preview %s", pChannel->strUID.c_str());
        m_pChannel = pChannel;
        auto pIPCServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
        if (pIPCServer)
        {
            pIPCServer->VideoControl(pChannel->strUID, true);
        }

        auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
        if (pIPCCallBack)
        {
            pIPCCallBack->Register(m_CallBackFunc);
        }

        if (m_DrawWnd)
        {
            m_DrawWnd->SetPreviewStatu(true);
        }
    }
}

void PreviewRealWnd::StopPreview()
{
    if (m_pChannel && (GetRuningStatu() == Status::InPreview || GetRuningStatu() == Status::StartingPreview))
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

        if (m_DrawWnd)
        {
            m_DrawWnd->SetPreviewStatu(false);
        }

        m_pChannel = nullptr;
    }
    SetRuningStatu(Status::Empty);
}

void PreviewRealWnd::Clear()
{
    if (m_CallBackFunc)
    {
        auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
        if (pIPCCallBack)
        {
            pIPCCallBack->UnRegister(m_CallBackFunc);
        }
        m_CallBackFunc = nullptr;
    }
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
    QPoint ptCurrent = QCursor::pos();
    QRect rcCenter = GetCenterArea();
    if (rcCenter.contains(ptCurrent))
    {
        emit PreviewWndUserClick();
        return;
    }
    AreableWidget<QWidget>::mousePressEvent(event);
}

BarWidget::Ptr PreviewRealWnd::InitBottomBar()
{
    auto pBottomBar = GetBottomWnd();
    if (pBottomBar)
    {
        if (auto pLayout = pBottomBar->GetLayout())
        {
            auto pBtnClose = MQ(QPushButton)(pBottomBar);
            auto pBtnRecord = MQ(QPushButton)(pBottomBar);
            auto pBtnSnap = MQ(QPushButton)(pBottomBar);

            pBtnClose->setObjectName("btn_stop_preview");
            pBtnClose->setToolTip(QStringLiteral("停止预览"));
            pBtnClose->setFixedWidth(60);
            pBtnClose->setText(QStringLiteral("关闭"));

            pBtnRecord->setObjectName("btn_record");
            pBtnRecord->setText(QStringLiteral("录像"));
            pBtnRecord->setToolTip(QStringLiteral("录像"));
            pBtnRecord->setFixedWidth(60);

            pBtnSnap->setObjectName("btn_snapshot");
            pBtnSnap->setText(QStringLiteral("截图"));
            pBtnSnap->setToolTip(QStringLiteral("截图"));
            pBtnSnap->setFixedWidth(60);

            pLayout->addStretch();
            pLayout->setSpacing(10);
            pLayout->addWidget(pBtnRecord);
            pLayout->addWidget(pBtnSnap);
            pLayout->addWidget(pBtnClose);

            pBtnRecord->setEnabled(false);
            pBtnSnap->setEnabled(false);

            connect(pBtnClose, &QPushButton::clicked, this, [this](bool) {
                this->StopPreview();
            });
        }
    }
    return pBottomBar;
}
