#include "PreviewRealWnd.h"
#include "OpenGLDrawWnd.h"
#include "ConfigDlg.h"
#include "MessageBoxWnd.h"
#include "ConfigCenter.h"
PreviewRealWnd::PreviewRealWnd(int nIndex, QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    HideBottom(true);
    SetAreaBk(s_qcl292C39, s_qcl292C39, s_qcl444858);
    m_iconRecordOff = QIcon(":/Black/res/record-off.png");
    m_iconRecordOn = QIcon(":/Black/res/recorder.png");
    auto pMainLayout = MQ(QVBoxLayout)(this);
    m_pSliderPB = MQ(QSlider)(this);
    m_DrawWnd = new DrawWnd(nIndex, this);
    m_pTimerAutoPlay = new QTimer(this);
    m_pTimerHideBottom = new QTimer(this);
    m_pTimerApplyStage = new QTimer(this);
    connect(m_pTimerAutoPlay, &QTimer::timeout, this, &PreviewRealWnd::OnTimeout);
    connect(m_pTimerHideBottom, &QTimer::timeout, this, &PreviewRealWnd::OnTimeout);
    connect(m_pTimerApplyStage, &QTimer::timeout, this, &PreviewRealWnd::OnTimeout);
    if (m_DrawWnd)
    {
        pMainLayout->addWidget(m_DrawWnd);
        m_DrawWnd->setAttribute(Qt::WA_TransparentForMouseEvents);
        connect(m_DrawWnd, &DrawWnd::PBDataReady, this, &PreviewRealWnd::OnPBDataReady);
        connect(m_DrawWnd, &DrawWnd::CustomedSnap, this, &PreviewRealWnd::OnCustomedSnap);

        connect(this, &PreviewRealWnd::MouseMove, m_DrawWnd, &DrawWnd::OnMouseMove);
        connect(this, &PreviewRealWnd::MouseLeave, m_DrawWnd, &DrawWnd::OnMouseLeave, Qt::QueuedConnection);
        connect(this, &PreviewRealWnd::MouseClicked, m_DrawWnd, &DrawWnd::OnMouseClicked, Qt::QueuedConnection);
        if (m_pSliderPB)
        {

            m_pSliderPB->setOrientation(Qt::Horizontal);
            connect(m_pSliderPB, &QSlider::valueChanged, m_DrawWnd, &DrawWnd::OnPBCtrl);
            connect(m_pSliderPB, &QSlider::sliderPressed, this, [this]() {
                this->PauseResumePlayBack();
            });
            connect(m_pSliderPB, &QSlider::sliderReleased, this, [this]() {
                this->PauseResumePlayBack(false);
            });
            m_pSliderPB->setFixedHeight(20);
            pMainLayout->addWidget(m_pSliderPB);
            m_pSliderPB->hide();
        }
    }

    if (auto pBottomBar = InitBottomBar())
    {
        //pBottomBar->hide();
        pMainLayout->addWidget(pBottomBar);
        //pBottomBar->setAttribute(Qt::WA_TransparentForMouseEvents);
        //pBottomBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    }
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pMainLayout);
    //setSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);


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
    switch (m_nWndMode.load())
    {
    case PanelMode::PreviewMode:
    {
        if (m_DrawWnd && m_pChannel && (strUid == m_pChannel->strUID))
        {
            SetRuningStatu(Status::InPreview);
            m_DrawWnd->InputFrameData(pFrame);
        }
    }
    break;
    case PanelMode::SnapMode:
    {
        if (m_DrawWnd && m_pChannel && (strUid == m_pChannel->strUID))
        {
            SetRuningStatu(Status::InSync);
            m_DrawWnd->InputFrameData(pFrame);
        }
    }
    default:
        break;
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
        StartStream(pChannel);

        if (m_DrawWnd)
        {
            QString strWndHint = "监控点 [" + QString::fromStdString(pChannel->GetName()) + "]  正在预览....";
            m_DrawWnd->SetHintString(strWndHint);
            m_DrawWnd->SetPreviewStatu(true);
        }

        pChannel->StartPreview();
        emit PreviewWndStartPreview(QString::fromStdString(m_pChannel->GetDevUid()), this);
        StartHideBottomTimer();
    }
}

void PreviewRealWnd::StopPreview()
{
    if (m_bRecord)
    {
        DoRecord();
    }

    if (m_pChannel && (GetRuningStatu() == Status::InPreview || GetRuningStatu() == Status::StartingPreview))
    {
        StopStream();
        if (m_DrawWnd)
        {
            m_DrawWnd->SetPreviewStatu(false);
        }
        m_pChannel->StopPreview();
        emit PreviewWndStopPreview(QString::fromStdString(m_pChannel->GetDevUid()), this);
        m_pChannel = nullptr;
    }
    StopHideBottomTimer();
    SetRuningStatu(Status::Empty);
}

void PreviewRealWnd::StartStream(DevNode::Ptr pChannel)
{
    m_pChannel = pChannel;
    auto pIPCServer = g_pEngine ? g_pEngine->GetIPCNetServer() : nullptr;
    if (pIPCServer)
    {
        auto pStreamInfo = m_pChannel->GetStreamData();
        int nStreamData = 0; //默认高清码流
        pIPCServer->VideoControl(pChannel->strUID, true, nStreamData);
    }

    auto pIPCCallBack = g_pCallBack ? g_pCallBack->GetIPCNetCallBack() : nullptr;
    if (pIPCCallBack)
    {
        pIPCCallBack->Register(m_CallBackFunc);
    }
}

void PreviewRealWnd::StopStream()
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

}

void PreviewRealWnd::StartSync(DevNode::Ptr pChannel)
{
    StopPlayBack();
    if (GetRuningStatu() == Status::InSync)
    {
        if (pChannel->GetDevUid() != m_pChannel->GetDevUid())
        {
            StopSync();
        }
        else
        {
            LogInfo("start sync %s duplicate", pChannel->strUID.c_str());
            return;
        }
    }
    if (pChannel)
    {
        LoadSnapModeParam();
        SetWndMode(SnapMode);
        SetRuningStatu(Status::InSync);
        LogInfo("start sync %s", pChannel->strUID.c_str());
        StartStream(pChannel);
        if (m_DrawWnd)
        {
            QString strWndHint = "监控点 [" + QString::fromStdString(pChannel->GetName()) + "]  正在同步....";
            m_DrawWnd->SetHintString(strWndHint);
            m_DrawWnd->SetPreviewStatu(true);
            m_DrawWnd->StartSync(m_pSnapModeParam);
        }
        pChannel->StartPreview();
        emit PreviewWndStartPreview(QString::fromStdString(m_pChannel->GetDevUid()), this);
    }
}

void PreviewRealWnd::StopSync()
{
    if (m_pChannel && (GetRuningStatu() == Status::InSync))
    {
        StopStream();
        if (m_DrawWnd)
        {
            m_DrawWnd->SetPreviewStatu(false);
            m_DrawWnd->StopSync();
        }
        m_pChannel->StopPreview();
        emit PreviewWndStopPreview(QString::fromStdString(m_pChannel->GetDevUid()), this);

        m_nCurDir = 0;
        m_nCurIndex = 0;
    }
    SetRuningStatu(Status::Empty);
}

void PreviewRealWnd::UpdateRecordStatu(bool bStart)
{
    if (m_pRecordBtn)
    {
        if (bStart)
        {
            m_pRecordBtn->setIcon(m_iconRecordOff);
        }
        else
        {
            m_pRecordBtn->setIcon(m_iconRecordOn);
        }

        m_bRecord = bStart;
    }
}

void PreviewRealWnd::StartPlayBack(bool bPre)
{
    SetWndMode(PanelMode::PictureMode);
    if (m_pSliderPB && m_DrawWnd)
    {
        m_nTotalFrame = m_DrawWnd->GetPlayBackSize();
        m_pSliderPB->setMinimum(0);
        m_pSliderPB->setMaximum(m_nTotalFrame);
        m_pSliderPB->setSingleStep(1);
        m_pSliderPB->setPageStep(10);
        m_pSliderPB->show();

        if (m_nCurIndex == 0 && bPre)
        {
            m_nCurIndex = m_nTotalFrame;
        }
        else if(m_nCurIndex == m_nTotalFrame && !bPre)
        {
            m_nCurIndex = 0;
        }
       
        m_nCurDir = bPre ? -1 : 1;
        m_pSliderPB->setValue(m_nCurIndex);
    }

    StartPlayBackTimer();
}

void PreviewRealWnd::StopPlayBack()
{
    StopPlayBackTimer();
    SetWndMode(PanelMode::SnapMode);
    if (m_pSliderPB)
    {
        m_pSliderPB->hide();
    }
}

void PreviewRealWnd::PauseResumePlayBack(bool bPause)
{
    if (bPause)
    {
        StopPlayBackTimer();
    }
    else
    {
        m_nCurIndex = m_pSliderPB->value();
        StartPlayBackTimer();
    }
}

void PreviewRealWnd::OnPBDataReady(int nDataSize)
{
    emit PBDataReady(nDataSize);
}

void PreviewRealWnd::OnCustomedSnap(SnapData::Ptr pSnap)
{
    emit CustomedSnap(pSnap);
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

void PreviewRealWnd::ClearPicture()
{
    if (GetRuningStatu() == Status::InPicture)
    {
        if (m_DrawWnd)
        {
            m_DrawWnd->ClearPicture();
        }
        SetRuningStatu(Status::Empty);
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
    case PreviewRealWnd::InPicture:
        m_tmBusy = time(0);
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

SnapData::Ptr  PreviewRealWnd::StreamSnapShot()
{
    if (m_DrawWnd && m_pChannel)
    {
        return m_DrawWnd->SnapShot();
    }
    return nullptr;
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

void PreviewRealWnd::SetWndMode(PanelMode emMode)
{
    m_nWndMode.store(emMode);
    if (m_DrawWnd)
    {
        m_DrawWnd->SetWndMode(emMode);
    }

    switch (emMode)
    {
    case PreviewMode:
        if (m_pTimerApplyStage)
        {
            m_pTimerApplyStage->start(2000);
        }
        ShowFrame(nullptr);
        break;
    case PictureMode:
        m_stageInfo.bValied = true;
        m_stageInfo.pChannel = m_pChannel;
        m_stageInfo.bSelected = m_bSelected;
        StopPreview();
        break;
    default:
        break;
    }
}

void PreviewRealWnd::StartPlayBackTimer()
{
    StopPlayBackTimer();
    if (m_pTimerAutoPlay)
    {
        m_pTimerAutoPlay->start(300);
    }
}

void PreviewRealWnd::StopPlayBackTimer()
{
    if (m_pTimerAutoPlay && m_pTimerAutoPlay->isActive())
    {
        m_pTimerAutoPlay->stop();
    }
}

void PreviewRealWnd::StartHideBottomTimer()
{
    StopHideBottomTimer();
    if (m_pTimerHideBottom)
    {
        m_pTimerHideBottom->start(1000);
    }
}

void PreviewRealWnd::StopHideBottomTimer()
{
    if (m_pTimerHideBottom && m_pTimerHideBottom->isActive())
    {
        m_pTimerHideBottom->stop();
    }
}

void PreviewRealWnd::OnTimeout()
{
    auto pTimer = qobject_cast<QTimer*>(sender());
    if (pTimer == m_pTimerAutoPlay)
    {
        if (m_pSliderPB)
        {
            m_nCurIndex += m_nCurDir;
            m_nCurIndex = max(m_nCurIndex, 0);
            m_nCurIndex = min(m_nTotalFrame, m_nCurIndex);
            m_pSliderPB->setValue(m_nCurIndex);
        }
    }
    if (pTimer == m_pTimerHideBottom)
    {
        if (time(NULL) - m_tmLastMove > 7)
        {
            HideBottom(true);
        }
    }
    if (pTimer == m_pTimerApplyStage)
    {
        if (m_stageInfo.bValied)
        {
            StartPreview(m_stageInfo.pChannel);
            SetSelectStatu(m_stageInfo.bSelected);
            m_stageInfo.Clear();
        }
        m_pTimerApplyStage->stop();
    }
}

void PreviewRealWnd::ShowFrame(SnapData::Ptr pFrame)
{
    if (m_DrawWnd)
    {
        if (pFrame)
        {
            SetRuningStatu(Status::InPicture);
            m_DrawWnd->ShowFrame(pFrame);
        }
        else
        {
            ClearPicture();
        }
    }\
}

void PreviewRealWnd::ResetFullLevel(int nLevel)
{
    m_nfullLevel = nLevel;
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
        emit MouseClicked();

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
            pLayout->setContentsMargins(0,1,0,0);

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
    m_nfullLevel = (++m_nfullLevel) % 3;
    emit PreviewWndUserDBClick(m_nfullLevel);
}

void PreviewRealWnd::enterEvent(QEvent *event)
{
    setMouseTracking(true);
    if (m_nWndMode.load() == PanelMode::PreviewMode)
    {
        auto pBottomBar = GetBottomWnd();
        if (pBottomBar )
        {
            HideBottom(false);
        }
    }
    else
    {
        BaseClass::enterEvent(event);
    }
}

void PreviewRealWnd::leaveEvent(QEvent *event)
{
    if (m_nWndMode.load() == PanelMode::PreviewMode)
    {
        auto pBottomBar = GetBottomWnd();
        if (pBottomBar && !m_bHideBottom)
        {
            HideBottom(true);
        }
    }
    else
    {
        emit MouseLeave();

        BaseClass::leaveEvent(event);
    }
    setMouseTracking(false);
}

void PreviewRealWnd::mouseMoveEvent(QMouseEvent *event)
{
    switch (m_nWndMode.load())
    {
    case PanelMode::SnapMode:
    case PanelMode::PictureMode:
        if (m_DrawWnd)
        {
            emit MouseMove(event->pos());
        }
        break;
    case PanelMode::PreviewMode:
        HideBottom(false);
        m_tmLastMove = time(NULL);
        break;
    default:
        break;
    }
}

void PreviewRealWnd::LoadSnapModeParam()
{
    m_pSnapModeParam = ConfigCenter::GetInstance().GetSnapModeParam();
}

void PreviewRealWnd::HideBottom(bool bHide)
{
    if (bHide && !m_bHideBottom)
    {
        SetArea(0, 1);
        m_bHideBottom = bHide;
    }
    else if(!bHide && m_bHideBottom)
    {
        SetArea(0, 30);
        m_bHideBottom = false;
    }
}

