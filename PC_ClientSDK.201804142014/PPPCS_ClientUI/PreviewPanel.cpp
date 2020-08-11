#include "PreviewPanel.h"
#include "QtDefine.h"
#include <QComboBox>
PreviewPanel::PreviewPanel(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    SetArea(50,0);
    SetAreaBk(s_qcl444858, s_qcl292C39, s_qcl444858);
    auto pTopBar = GetTopWnd();
    if (pTopBar)
    {
        InitTopBar(pTopBar);
        ui.mainLayout->addWidget(pTopBar);
        //ui.mainLayout->addStretch();
    }

    m_pRealWnds = MQ(QGridLayout)(this);

    m_pRealWnds->setHorizontalSpacing(10);
    m_pRealWnds->setVerticalSpacing(10);
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

void PreviewPanel::InitTopBar(BarWidget::Ptr pBarWidget)
{
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

    return;
}

void PreviewPanel::InitBottomBar(BarWidget::Ptr pBarWidget)
{

}

void PreviewPanel::InitTopBar4SnapMode(QHBoxLayoutPtr pBarLayout)
{

}

void PreviewPanel::InitTopBar4PreviewMode(QHBoxLayoutPtr pBarLayout)
{
    auto pbtnDevideMode = MQ(QPushButton)(this);
    auto pbtnFullScreen = MQ(QPushButton)(this);

    pBarLayout->addWidget(pbtnDevideMode);
    pBarLayout->addStretch();
    pBarLayout->addWidget(pbtnFullScreen);
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
        auto pWnd = MQ(PreviewRealWnd)(this);
        m_vcPreviewRealWnds.push_back(pWnd);
        pWnd->hide();
        ++nCurrent;
    }
}
