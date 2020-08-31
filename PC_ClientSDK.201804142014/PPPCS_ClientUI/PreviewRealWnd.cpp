#include "PreviewRealWnd.h"
#include "OpenGLDrawWnd.h"
#include "ConfigDlg.h"
#include "MessageBoxWnd.h"
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
        m_CallBackFunc->funcOnRecordNotify = std::bind(&PreviewRealWnd::OnRecordNotify, this, std::placeholders::_1, std::placeholders::_2);
    }

    SetRuningStatu(Status::Empty);
    if (m_pRecordBtn)
    {
        connect(this, &PreviewRealWnd::RecordStatuChanged, this, &PreviewRealWnd::UpdateRecordStatu);
    }
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

void PreviewRealWnd::OnRecordNotify(const std::string& strUid, bool bStart)
{
    if (m_pChannel && strUid == m_pChannel->GetDevUid())
    {
        emit RecordStatuChanged(bStart);
    }
}

void PreviewRealWnd::StartPreview(DevNode::Ptr pChannel)
{
    if (GetRuningStatu() == Status::InPreview || GetRuningStatu() == Status::StartingPreview)
    {
        if (pChannel->GetDevUid() != m_pChannel->GetDevUid())
        {
            StopPreview();
        }
        else
        {
            LogInfo("start preview %s duplicate", pChannel->strUID.c_str());
            return;
        }
    }
    if (pChannel)
    {
        SetRuningStatu(Status::StartingPreview);
        LogInfo("start preview %s", pChannel->strUID.c_str());
        m_pChannel = pChannel;
        auto pIPCServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
        if (pIPCServer)
        {
            auto pStreamInfo = m_pChannel->GetStreamData();
            int nStreamData = pStreamInfo ? pStreamInfo->EncCh : 1;
            pIPCServer->VideoControl(pChannel->strUID, true, nStreamData);
        }

        auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
        if (pIPCCallBack)
        {
            pIPCCallBack->Register(m_CallBackFunc);
        }

        if (m_DrawWnd)
        {
            QString strWndHint = "监控点 [" + QString::fromStdString(pChannel->GetName()) + "]  正在预览....";
            m_DrawWnd->SetHintString(strWndHint);
            m_DrawWnd->SetPreviewStatu(true);
        }

        pChannel->StartPreview();
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
        m_pChannel->StopPreview();
        emit PreviewWndStopPreview(QString::fromStdString(m_pChannel->GetDevUid()), this);
        m_pChannel = nullptr;
    }
    SetRuningStatu(Status::Empty);
}

void PreviewRealWnd::UpdateRecordStatu(bool bStart)
{
    if (m_pRecordBtn)
    {
        if (bStart)
        {
            m_pRecordBtn->setText("录像中");
        }
        else
        {
            m_pRecordBtn->setText("");
        }

        m_bRecord = bStart;
    }
}

void PreviewRealWnd::CallConfig()
{
    ConfigDlg config(m_pChannel, this);
    config.exec();
}

void PreviewRealWnd::SnapShot()
{
    if (m_DrawWnd && m_pChannel)
    {
        m_DrawWnd->SnapShot(QString::fromStdString(m_pChannel->GetName()));
    }
}

void PreviewRealWnd::DoRecord()
{
    if (m_pChannel)
    {
        if (m_bRecord)
        {
            m_pChannel->StopRecord();
        }
        else
        {
            m_pChannel->StartRecord();
        }
    }
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
        SetBottomEnable(false);
        break;
    case PreviewRealWnd::StartingRecord:
    case PreviewRealWnd::StartingPreview:
        m_tmBusy = time(0);
        SetBottomEnable(true);
        break;
    case PreviewRealWnd::InPreview:
    case PreviewRealWnd::Record:
        SetBottomEnable(true);
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

bool PreviewRealWnd::IsFull()
{
    return m_bFull;
}

void PreviewRealWnd::SetFull(bool bValue)
{
    m_bFull = bValue;
}

int PreviewRealWnd::GetPtzSpeed()
{
    if (m_DrawWnd)
    {
        return m_DrawWnd->GetPtzSpeed();
    }
    return 1;
}

void PreviewRealWnd::SetBottomEnable(bool bEnable)
{
    auto pBottomBar = GetBottomWnd();
    if (pBottomBar)
    {
        pBottomBar->setEnabled(bEnable);
    }
}

int PreviewRealWnd::IsInPreview()
{
    auto emCurStatu = GetRuningStatu();
    return (emCurStatu == InPreview)|| (emCurStatu == StartingPreview)|| (emCurStatu == Reconnecting);
}

DevNode::Ptr PreviewRealWnd::GetDevNode()
{
    return m_pChannel;
}

bool PreviewRealWnd::CheckDevLostConnect(const std::string& strLostDev)
{
    if (m_pChannel && m_pChannel->GetDevUid() == strLostDev)
    {
        QString strWndHint = "监控点 [" + QString::fromStdString(m_pChannel->GetName()) + "]  丢失连接,正在重连....";
        m_DrawWnd->SetHintString(strWndHint);
        m_DrawWnd->update();
        SetRuningStatu(Status::Reconnecting);
    }
    return false;
}

void PreviewRealWnd::OnPtzCtrl(PtzCommand emCmd, int nParam)
{
    if(m_DrawWnd)
    {
        m_DrawWnd->OnPtzCtrl(emCmd, nParam);
    }
}

void PreviewRealWnd::mousePressEvent(QMouseEvent *event)
{
    QPoint ptCurrent = QCursor::pos();
    QRect rcCenter = GetCenterArea();
    if (rcCenter.contains(mapFromGlobal(ptCurrent)))
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
            auto pBtnConfig = MQ(QPushButton)(pBottomBar);

            pBtnClose->setObjectName("btn_stop_preview");
            pBtnClose->setToolTip(QStringLiteral("停止预览"));
            pBtnClose->setFixedWidth(32);

            pBtnRecord->setObjectName("btn_record");
            pBtnRecord->setToolTip(QStringLiteral("录像"));
            pBtnRecord->setFixedWidth(32);

            pBtnSnap->setObjectName("btn_snapshot");
            pBtnSnap->setToolTip(QStringLiteral("截图"));
            pBtnSnap->setFixedWidth(32);

            pBtnConfig->setObjectName("btn_config");
            pBtnConfig->setToolTip(QStringLiteral("配置"));
            pBtnConfig->setFixedWidth(32);

            pLayout->addStretch();
            pLayout->setSpacing(5);
            pLayout->addWidget(pBtnRecord);
            pLayout->addWidget(pBtnSnap);
            pLayout->addWidget(pBtnClose);
            pLayout->addWidget(pBtnConfig);

            m_pRecordBtn = pBtnRecord;
            //pBtnRecord->setEnabled(false);

            connect(pBtnClose, &QPushButton::clicked, this, [this](bool) {
                if (this->GetRuningStatu() == PreviewRealWnd::Status::Reconnecting)
                {
                    msg::showInformation(this, "信息", "设备正在重连，请稍后再试!");
                    return;
                }
                this->StopPreview();
            });

            connect(pBtnSnap, &QPushButton::clicked, this, &PreviewRealWnd::SnapShot);
            
            connect(pBtnConfig, &QPushButton::clicked, this, &PreviewRealWnd::CallConfig);

            connect(pBtnRecord, &QPushButton::clicked, this, &PreviewRealWnd::DoRecord);
            //pBtnRecord->hide();
        }
    }
    return pBottomBar;
}


void PreviewRealWnd::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit PreviewWndUserDBClick(!m_bFull);
    m_bFull = !m_bFull;
}
