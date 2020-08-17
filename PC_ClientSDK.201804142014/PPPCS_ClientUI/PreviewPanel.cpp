#include "PreviewPanel.h"
#include "QtDefine.h"
#include <QComboBox>
PreviewPanel::PreviewPanel(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    SetArea(50,0);
    SetAreaBk(s_qcl444858, s_qcl1E2233, s_qcl444858);
    auto pTopBar = InitTopBar();
    if (pTopBar)
    {
        ui.mainLayout->addWidget(pTopBar);
        //ui.mainLayout->addStretch();
    }

    m_pRealWnds = MQ(QGridLayout)(this);

    m_pRealWnds->setHorizontalSpacing(10);
    m_pRealWnds->setVerticalSpacing(10);
    m_pRealWnds->setContentsMargins(0, 10, 0, 0);
    ui.mainLayout->addLayout(m_pRealWnds);
    ui.mainLayout->setStretch(1, 1);
    InitPreviewRealWnds();
//     auto pBottomBar = GetBottomWnd();
//     if (pBottomBar)
//     {
//         InitBottomBar(pBottomBar);
//         ui.mainLayout->addWidget(pBottomBar);
//     }
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


}

BarWidget::Ptr PreviewPanel::InitTopBar()
{
    auto pBarWidget = GetTopWnd();
    if (!pBarWidget)
    {
        LogError("Top Bar ptr is null!");
    }

    switch (m_curPanelmode)
    {
    case PreviewPanel::PreviewMode:
        InitTopBar4PreviewMode(pBarWidget->GetLayout());
        break;
    case PreviewPanel::SnapMode:
        InitTopBar4SnapMode(pBarWidget->GetLayout());
        break;
    default:
        break;
    }
    LogInfo("init top bar for mode %d", m_curPanelmode);

    return pBarWidget;
}

BarWidget::Ptr PreviewPanel::InitBottomBar()
{
    auto pBarWidget = GetBottomWnd();
    return pBarWidget;
}

void PreviewPanel::InitTopBar4SnapMode(QHBoxLayoutPtr pBarLayout)
{

}

void PreviewPanel::InitTopBar4PreviewMode(QHBoxLayoutPtr pBarLayout)
{
    m_pCbbDevideScreen = MQ(QComboBox)(this);
    auto pbtnFullScreen = MQ(QPushButton)(this);

    m_pCbbDevideScreen->addItem("1画面", DevideScreen::Screen_1X1);
    m_pCbbDevideScreen->addItem("4画面", DevideScreen::Screen_2X2);
    m_pCbbDevideScreen->addItem("9画面", DevideScreen::Screen_3X3);
    m_pCbbDevideScreen->addItem("12画面", DevideScreen::Screen_3X4);

    pBarLayout->addWidget(m_pCbbDevideScreen);
    pBarLayout->addStretch();
    pBarLayout->addWidget(pbtnFullScreen);

    connect(m_pCbbDevideScreen, QOverload<int>::of(&QComboBox::activated), [this](int index) {
        this->OnScreenDevideModeChange(index);
    });
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
        auto pWnd = new PreviewRealWnd(++nCurrent, this);
        m_vcPreviewRealWnds.push_back(pWnd);
        connect(pWnd, &PreviewRealWnd::PreviewWndUserClick, this, &PreviewPanel::OnPreveiwWndSelChange);
        pWnd->hide();
    }
}

void PreviewPanel::SetSelectWnd(PreviewRealWnd::Ptr pSelWnd)
{
    if (pSelWnd)
    {
        if (m_pCurSelWnd && m_pCurSelWnd != pSelWnd)
        {
            m_pCurSelWnd->SetSelectStatu(false);
        }

        bool bOriginStatu = pSelWnd->GetSelectStatu();
        if (bOriginStatu)
        {
            pSelWnd->SetSelectStatu(false);
            m_pCurSelWnd = nullptr;
        }
        else
        {
            pSelWnd->SetSelectStatu(true);
            m_pCurSelWnd = pSelWnd;
        }
    }
    else
    {
        if (m_pCurSelWnd)
        {
            m_pCurSelWnd->SetSelectStatu(false);
        }
    }
}

void PreviewPanel::OnStartPreview(ChannelNode::Ptr pChannel)
{
    //Get a free wnd or get the  select wnd 
    PreviewRealWnd::Ptr pTagetWnd = nullptr;
    if (m_pCurSelWnd && (m_pCurSelWnd->GetRuningStatu() == PreviewRealWnd::Status::Empty))
    {
        pTagetWnd = m_pCurSelWnd;
    }

    if(!pTagetWnd)
    {
        for (auto pWnd:m_vcPreviewRealWnds)
        {
            if (pWnd->isVisible() &&( pWnd->GetRuningStatu() == PreviewRealWnd::Status::Empty))
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
            if (pWnd->isVisible() && (pWnd->GetBusyTime() < oldestBusyTime))
            {
                pTagetWnd = pWnd;
            }
        }
    }

    SetSelectWnd(pTagetWnd);

    if (pTagetWnd)
    {
        pTagetWnd->StartPreview(pChannel);
    }
    else
    {
        LogError("No wnd could use");
    }
}

void PreviewPanel::OnPreveiwWndSelChange()
{
    auto pSelWnd = qobject_cast<PreviewRealWnd::Ptr>(sender());
    SetSelectWnd(pSelWnd);
}

void PreviewPanel::OnScreenDevideModeChange(int nIndex)
{
    if (m_pCbbDevideScreen)
    {
        auto selData = m_pCbbDevideScreen->currentData();
        OnScreenDevideChange((DevideScreen)selData.toInt());
    }
}
