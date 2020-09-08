#include "PreviewPanel.h"
#include "QtDefine.h"
#include <QComboBox>
#include "utils.h"
#include <QDesktopWidget>
#include "ConfigCenter.h"
#include <QDateTime>
#include <QProcess>
#include <QDesktopServices>
#include "MessageBoxWnd.h"
#include "HelpWindow.h"
#include "ConfigWidget.h"
PreviewPanel::PreviewPanel(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    SetArea(50,0);
    SetAreaBk(s_qcl444858, s_qcl1E2233, s_qcl444858);
    m_pParent = parent;
    m_icon1X1 = QIcon(":/Black/res/1X1.png");
    m_icon2X2 = QIcon(":/Black/res/2X2.png");
    m_icon3X3 = QIcon(":/Black/res/3X3.png");
    m_icon4X3 = QIcon(":/Black/res/4X3.png");
    m_pTimerTopBarCheck = new QTimer(this);
    connect(m_pTimerTopBarCheck, &QTimer::timeout, this, &PreviewPanel::OnTimeout);
    auto pTopBar = InitTopBar();
    if (pTopBar)
    {
        ui.mainLayout->addWidget(pTopBar);
        //ui.mainLayout->addStretch();
    }

    auto pViewWndWidget = MQ(QWidget)(this);
    m_pViewWndWidget = pViewWndWidget;
    m_pRealWnds = MQ(QGridLayout)(pViewWndWidget);
    m_pRealWnds->setHorizontalSpacing(10);
    m_pRealWnds->setVerticalSpacing(10);
    m_pRealWnds->setContentsMargins(0, 10, 0, 0);
    pViewWndWidget->setLayout(m_pRealWnds);
    ui.mainLayout->addWidget(pViewWndWidget);
    ui.mainLayout->setStretch(1, 1);
    InitPreviewRealWnds();
    SetSelectWnd(nullptr);

}

PreviewPanel::~PreviewPanel()
{
}

void PreviewPanel::OnScreenDevideChange(DevideScreen newMode)
{
    int nRow = 1;
    int nCloumn = 1;
    switch (newMode)
    {
    case Screen_1X1:
        nRow = 1;
        nCloumn = 1;
        break;
    case Screen_2X2:
        nRow = 2;
        nCloumn = 2;
        break;
    case Screen_3X3:
        nRow = 3;
        nCloumn = 3;
        break;
    case Screen_3X4:
        nRow = 3;
        nCloumn = 4;
        break;
    case Screen_CUSTOM:
        nRow = m_nCustomRow;
        nCloumn = m_nCustomColumn;
    default:
        break;
    }

    m_curScreenMode = newMode;
    int nTotalWnd = nRow * nCloumn;
    PraperPreviewRealWnds(nTotalWnd);
    if (m_pRealWnds)
    {
        QLayoutItem *child;
        while ((child = m_pRealWnds->takeAt(0)) != 0) {
            child->widget()->hide();
            delete child;
        }
        if (m_bDirExchanged && newMode == Screen_CUSTOM)
        {
            nRow = nRow ^ nCloumn;
            nCloumn = nRow ^ nCloumn;
            nRow = nRow ^ nCloumn;

            for (int j = 0; j < nCloumn; j++)
            {
                for (int i = 0; i < nRow; i++)
                {
                    auto pRealWnd = m_vcPreviewRealWnds[j*nRow + i];
                    m_pRealWnds->addWidget(pRealWnd, i, j, 1, 1);
                    pRealWnd->show();
                    pRealWnd->ResetFullLevel(0);
                }
            }
        }
        else
        {
            for (int i = 0; i < nRow; i++)
            {
                for (int j = 0; j < nCloumn; j++)
                {
                    auto pRealWnd = m_vcPreviewRealWnds[i*nCloumn + j];
                    m_pRealWnds->addWidget(pRealWnd, i, j, 1, 1);
                    pRealWnd->show();
                }
            }
        }
        m_pRealWnds->update();
    }
}

void PreviewPanel::SetCustomWndLayout(int row, int column)
{
    m_nCustomRow = row;
    m_nCustomColumn = column;
    OnScreenDevideChange(DevideScreen::Screen_CUSTOM);
}

void PreviewPanel::OnLayoutDirChanged(bool bChanged)
{
    m_bDirExchanged = bChanged;
    OnScreenDevideChange(DevideScreen::Screen_CUSTOM);
}

void PreviewPanel::OnFullScreen()
{
    m_bFull = !m_bFull;

    if (m_bFull)
    {
        StartTopBarCheck();
        //setFocusPolicy(Qt::StrongFocus);
    }
    else
    {
        StopTopBarCheck();
        //setFocusPolicy(Qt::ClickFocus);
        SetArea(50, 0);
    }

    emit FullScreen(m_bFull);

    if (auto pBtn = qobject_cast<QPushButtonPtr>(sender()))
    {
        pBtn->setText(m_bFull?"返回":"全屏");
    }

    if (m_pSnapMode)
    {
        if (m_bFull)
        {
            m_pSnapMode->hide();
        }
        else
        {
            m_pSnapMode->show();
        }
    }
}

void PreviewPanel::OnTimeout()
{
    auto pCurPos = QCursor::pos();
    auto pTopWnd = GetTopWnd();
    if (m_bFull && pTopWnd)
    {
        if (pCurPos.y() < 200)
        {
            SetArea(50, 0);
        }
        else
        {
            SetArea(10, 0);
        }
    }
}

BarWidget::Ptr PreviewPanel::InitTopBar()
{
    auto pBarWidget = GetTopWnd();
    if (!pBarWidget)
    {
        LogError("Top Bar ptr is null!");
    }

    auto pBarLayout = pBarWidget->GetLayout();
    if (!pBarLayout)
    {
        LogError("Top Bar layout is null!");
    }

    /////Init for PreviewMode
    m_pCbbDevideScreen = MQ(QComboBox)(this);
    m_pCbbDevideScreen->addItem(m_icon1X1, "1画面", DevideScreen::Screen_1X1);
    m_pCbbDevideScreen->addItem(m_icon2X2, "4画面", DevideScreen::Screen_2X2);
    m_pCbbDevideScreen->addItem(m_icon3X3, "9画面", DevideScreen::Screen_3X3);
    m_pCbbDevideScreen->addItem(m_icon4X3, "12画面", DevideScreen::Screen_3X4);
    m_pCbbDevideScreen->setIconSize({ 30,30 });

    m_pCbbDevideScreen->setItemData(0, QColor(RGB(220, 220, 220)), Qt::TextColorRole);
    m_pCbbDevideScreen->setItemData(1, QColor(RGB(220, 220, 220)), Qt::TextColorRole);
    m_pCbbDevideScreen->setItemData(2, QColor(RGB(220, 220, 220)), Qt::TextColorRole);
    m_pCbbDevideScreen->setItemData(3, QColor(RGB(220, 220, 220)), Qt::TextColorRole);
   
    auto pbtnFullScreen = MQ(QPushButton)(this);
    auto pbtnSnapMode = MQ(QPushButton)(this);
    auto pbtnStorage = MQ(QPushButton)(this);
    pbtnFullScreen->setText("全屏");
    pbtnSnapMode->setText("抓拍模式");
    pbtnStorage->setText("存储路径");
    connect(m_pCbbDevideScreen, QOverload<int>::of(&QComboBox::activated), [this](int index) {
        this->OnScreenDevideModeChange(index);
    });
    connect(pbtnSnapMode, &QPushButton::clicked, [this]() {
        this->SetCurrentMode(PictureMode);
    });

    connect(pbtnStorage, &QPushButton::clicked, [this]() {
        ConfigWidget configDlg(ConfigType::StorageConfig, this);
        configDlg.exec();
    });
   
    connect(pbtnFullScreen, &QPushButton::clicked, this, &PreviewPanel::OnFullScreen, Qt::QueuedConnection);
    m_pSnapMode = pbtnSnapMode;
    m_pFullScrren = pbtnFullScreen;
    ///////Init for Snap mode
    auto pbtnReturnPreview = MQ(QPushButton)(this);
    auto pbtnChooseDev = MQ(QPushButton)(this);
    auto pbtnHelp = MQ(QPushButton)(this);
    auto pbtnClearAll = MQ(QPushButton)(this);

    pbtnReturnPreview->setText("预览模式");
    pbtnChooseDev->setText("选择设备");
    pbtnHelp->setText("帮助");
    pbtnClearAll->setText("清空");

    connect(pbtnReturnPreview, &QPushButton::clicked, [this]() {
        this->SetCurrentMode(PreviewMode);
    });
    connect(pbtnChooseDev, &QPushButton::clicked, [this]() {
        emit this->CallPopUpTreeWnd();
    });

    connect(pbtnClearAll, &QPushButton::clicked, [this]() {
        if (msg::showQuestion(this, "警告", "此操作会清空截图数据，是否确认？") == QMessageBox::Ok)
        {
            this->ClearAllWnd();
        }
    });

    connect(pbtnHelp, &QPushButton::clicked, [this]() {
        HelpWindow helpWindw(this);
        helpWindw.exec();
    });

    ///////End widget init

    pBarLayout->addWidget(m_pCbbDevideScreen);
    pBarLayout->addWidget(pbtnReturnPreview);
    pBarLayout->addWidget(pbtnChooseDev);
    pBarLayout->addWidget(pbtnHelp);
    pBarLayout->addWidget(pbtnClearAll);
    pBarLayout->addStretch();
    pBarLayout->addWidget(pbtnStorage);
    pBarLayout->addWidget(pbtnFullScreen);
    pBarLayout->addWidget(pbtnSnapMode);
    pBarLayout->setContentsMargins(20, 10, 20, 10);

    m_mapModeWidgets[PreviewMode] = { m_pCbbDevideScreen ,pbtnFullScreen, pbtnSnapMode , pbtnStorage };
    m_mapModeWidgets[PictureMode] = { pbtnReturnPreview ,pbtnChooseDev ,pbtnHelp , pbtnClearAll };
    
    SetCurrentMode(PreviewMode);

    LogInfo("init top bar for mode %d", m_curPanelmode);

    return pBarWidget;
}

BarWidget::Ptr PreviewPanel::InitBottomBar()
{
    auto pBarWidget = GetBottomWnd();
    return pBarWidget;
}

void PreviewPanel::InitPreviewRealWnds()
{
    OnScreenDevideChange(Screen_1X1);
}

void PreviewPanel::PraperPreviewRealWnds(int nNums)
{
    int nCurrent = m_vcPreviewRealWnds.size();
    if (nCurrent >= nNums)
    {
        return;
    }

    LogInfo("Need %d Wnds, Current %d", nNums, nCurrent);

    while (nCurrent < nNums)
    {
        auto pWnd = new PreviewRealWnd(++nCurrent, m_pViewWndWidget);
        pWnd->SetWndMode(m_curPanelmode);
        m_vcPreviewRealWnds.push_back(pWnd);
        connect(pWnd, &PreviewRealWnd::PreviewWndUserClick, this, &PreviewPanel::OnPreveiwWndSelChange);
        connect(pWnd, &PreviewRealWnd::PreviewWndUserDBClick, this, &PreviewPanel::OnPreveiwWndSelFull);
        connect(pWnd, &PreviewRealWnd::PreviewWndStopPreview, this, &PreviewPanel::OnPreviewStopped);
        connect(pWnd, &PreviewRealWnd::PreviewWndStartPreview, this, &PreviewPanel::OnPreviewStarted);
        pWnd->hide();
    }
}

void PreviewPanel::SetSelectWnd(PreviewRealWnd::Ptr pSelWnd, bool bForceSel)
{
    if (!pSelWnd)
    {
        pSelWnd = m_vcPreviewRealWnds[0];
    }

    if (m_pCurSelWnd && m_pCurSelWnd != pSelWnd)
    {
        m_pCurSelWnd->SetSelectStatu(false);
        m_pCurSelWnd = nullptr;
    }

    pSelWnd->SetSelectStatu(true);
    m_pCurSelWnd = pSelWnd;

    if (m_pCurSelWnd && m_pCurSelWnd->IsInPreview())
    {
        emit SelectPreviewWnd(m_pCurSelWnd);
    }
    else
    {
        emit SelectPreviewWnd(nullptr);
    }
}

void PreviewPanel::SetCurrentMode(PanelMode emMode)
{
    for (auto pairWidgets: m_mapModeWidgets)
    {
        auto vcWidgets = pairWidgets.second;
        bool bShow = (emMode == pairWidgets.first);
        utils::TravelVector(vcWidgets, [bShow](auto pItem) {
            if (bShow)
            {
                pItem->show();
            }
            else
            {
                pItem->hide();
            }
            return false;
        });
    }
    switch (emMode)
    {
    case PanelMode::PreviewMode:
    {
        m_pCbbDevideScreen->setCurrentIndex((int)m_lastScreenMode);
        emit m_pCbbDevideScreen->activated(1);
    }
        break;
    case PanelMode::PictureMode:
    {
        m_lastScreenMode = m_curScreenMode;
        auto pParam = ConfigCenter::GetInstance().GetSnapModeParam();
        if (pParam)
        {
            m_bDirExchanged = pParam->bRowFirst;
            SetCustomWndLayout(pParam->nWndRows, pParam->nWndColumns);
        }
    }
        break;
    default:
        break;
    }
    SetPreviewWndMode(emMode);
  
    emit PanelModeChanged(emMode);
}

void PreviewPanel::SetPreviewWndMode(PanelMode nModetype)
{
    m_curPanelmode = nModetype;
    utils::TravelVector(m_vcPreviewRealWnds, [nModetype](auto pWnd) {
        pWnd->SetWndMode(nModetype);
        return false;
    });
}

void PreviewPanel::StartPreview4PreviewMode(DevNode::Ptr pChannel)
{
    auto pTagetWnd = GetFreeWnd();

    if (pTagetWnd)
    {
        pTagetWnd->StartPreview(pChannel);
    }
    else
    {
        LogError("No wnd could use");
    }
    SetSelectWnd(pTagetWnd, true);
}


PreviewRealWnd::Ptr PreviewPanel::GetFreeWnd()
{
    //Get a free wnd or get the  select wnd 
    PreviewRealWnd::Ptr pTagetWnd = nullptr;
    if (m_pCurSelWnd && (m_pCurSelWnd->GetRuningStatu() == PreviewRealWnd::Status::Empty) && m_pCurSelWnd->isVisible())
    {
        pTagetWnd = m_pCurSelWnd;
    }

    if (!pTagetWnd)
    {
        for (auto pWnd : m_vcPreviewRealWnds)
        {
            if (pWnd->isVisible() && (pWnd->GetRuningStatu() == PreviewRealWnd::Status::Empty))
            {
                pTagetWnd = pWnd;
                break;
            }
        }
    }

    if (!pTagetWnd)
    {
        time_t oldestBusyTime = time(0);
        for (auto pWnd : m_vcPreviewRealWnds)
        {
            if (pWnd->isVisible())
            {
                if (pWnd->GetBusyTime() < oldestBusyTime)
                {
                    pTagetWnd = pWnd;
                    oldestBusyTime = pWnd->GetBusyTime();
                }
            }
        }
    }
    return pTagetWnd;
}

void PreviewPanel::StartTopBarCheck()
{
    if (m_pTimerTopBarCheck)
    {
        m_pTimerTopBarCheck->start(100);
    }
}

void PreviewPanel::StopTopBarCheck()
{
    if (m_pTimerTopBarCheck && m_pTimerTopBarCheck->isActive())
    {
        m_pTimerTopBarCheck->stop();
    }
}

void PreviewPanel::ClearAllWnd()
{
    utils::TravelVector(m_vcPreviewRealWnds, [](auto pWnd) {
        pWnd->ClearPicture();
        return false;
    });
}

void PreviewPanel::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        if (m_bFull)
        {
            if (m_pFullScrren)
            {
                emit m_pFullScrren->clicked();
            }
            else
            {
                OnFullScreen();
            }
        }
    }
    AreableWidget<QWidget>::keyPressEvent(event);
}

void PreviewPanel::OnStartPreview(DevNode::Ptr pChannel)
{
    switch (m_curPanelmode)
    {
    case PanelMode::PreviewMode:
        StartPreview4PreviewMode(pChannel);
        break;
    case PanelMode::PictureMode:
        //StartPreview4SnapMode(pChannel);
        break;
    default:
        break;
    }
}

void PreviewPanel::OnPreviewStopped(const QString& strUid, PreviewRealWnd::Ptr pWnd)
{
    emit PreviewStatuChanged(strUid, false);
}

void PreviewPanel::OnPreviewStarted(const QString& strUid, PreviewRealWnd::Ptr pWnd)
{
    emit PreviewStatuChanged(strUid, true);
}

void PreviewPanel::OnDeviceLostConnect(const QString& strUID)
{
    std::string strTaget = strUID.toStdString();
    for (auto pWnd:m_vcPreviewRealWnds)
    {
        pWnd->CheckDevLostConnect(strTaget);
    }
}

void PreviewPanel::OnPreveiwWndSelChange()
{
    auto pSelWnd = qobject_cast<PreviewRealWnd::Ptr>(sender());
    SetSelectWnd(pSelWnd);
}

void PreviewPanel::OnPreveiwWndSelFull(int nFull)
{
    auto pSelWnd = qobject_cast<PreviewRealWnd::Ptr>(sender());
    int nRealFullLevel = nFull;
    if (pSelWnd && m_pRealWnds && m_pFullScrren)
    {
        if (m_curScreenMode == Screen_1X1)
        {
            if (m_bFull)
            {
                nRealFullLevel = 0;
            }
            else
            {
                nRealFullLevel = 2;
            }
        }

        switch (nRealFullLevel)
        {
        case 0:
        {
            if (m_bFull)
            {
                emit m_pFullScrren->clicked();
            }
            OnScreenDevideChange(m_curScreenMode);
            SetSelectWnd(pSelWnd, true);
        }
            break;
        case 1:
        {
            QLayoutItem *child;
            while ((child = m_pRealWnds->takeAt(0)) != 0)
            {
                child->widget()->hide();
                delete child;
            }

            m_pRealWnds->addWidget(pSelWnd, 1, 1, 1, 1);
            pSelWnd->show();
        }
            break;
        case 2:
        {
            if (m_bFull)
            {
                OnScreenDevideChange(m_curScreenMode);
                SetSelectWnd(pSelWnd, true);
            }
            else
            {
                emit m_pFullScrren->clicked();
            }
        }
            break;
        default:
            break;
        }
    }
}

void PreviewPanel::OnScreenDevideModeChange(int nIndex)
{
    if (m_pCbbDevideScreen)
    {
        auto selData = m_pCbbDevideScreen->currentData();
        OnScreenDevideChange((DevideScreen)selData.toInt());
    }
}

void PreviewPanel::OnStreamSnapShot(SnapData::Ptr pFrame)
{
    if (auto pTagetWnd = GetFreeWnd())
    {
        pTagetWnd->ShowFrame(pFrame);
        SetSelectWnd(pTagetWnd, true);
        if (auto pParam = ConfigCenter::GetInstance().GetSnapModeParam())
        {
            if (pParam->bPopFull)
            {
                auto pNextFreeWnd = GetFreeWnd();
                if (!pNextFreeWnd || pNextFreeWnd->GetRuningStatu() != PreviewRealWnd::Empty)
                {
                    if (m_pViewWndWidget)
                    {
                        auto pixMap = m_pViewWndWidget->grab();
                        QString strPath = ConfigCenter::GetInstance().GetSnapShotSavepath();
                        utils::MakePathExist(strPath);
                        QString strFileName = strPath + "抓屏快照" + QDateTime::currentDateTime().toString("_yyMMdd_hh_mm_ss") + ".jpg";
                        if (pixMap.save(strFileName, "JPG", 100))
                        {
                            QDesktopServices::openUrl(QUrl::fromLocalFile(strFileName));
                            ClearAllWnd();
                        }
                    }
                }
            }
        }
    }
}
