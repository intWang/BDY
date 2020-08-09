#include "MainViewWnd.h"
#include "PreviewWnd.h"
MainViewWnd::MainViewWnd(QWidget *parent)
    :AreableWidget<QWidget>(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    SetAreaBk(s_qcl444858, s_qcl444858, s_qcl444858);

    InitPreviewWnds();
    QGridLayoutPtr pLayout = MQ(QGridLayout)(this);
    setLayout(pLayout);
    hide();

    SetStyle(Screen_1X1);
}


MainViewWnd::~MainViewWnd()
{
    ClearPreviewWnds();
}

void MainViewWnd::SetStyle(DevideScreen devideType)
{
    OrderWnds(devideType);
}

void MainViewWnd::InitPreviewWnds()
{
    const int totalWnds = 12;
    for (int i; i< totalWnds; i++)
    {
        auto pData = std::make_shared<PreivewData>();
        auto pWnd = MQ(PreviewWnd)(this);
        pData->pWnd = pWnd;
        m_vcPreviewwnds.push_back(pData);
    }
}

void MainViewWnd::ClearPreviewWnds()
{
    m_vcPreviewwnds.clear();
}

QGridLayoutPtr MainViewWnd::GetLayout()
{
    return dynamic_cast<QGridLayoutPtr>(layout());
}

void MainViewWnd::OrderWnds(DevideScreen devideType)
{
    int nRow = 1;
    int nColumn = 1;
    switch (devideType)
    {
    case Screen_1X1:
    {
        nRow = 1;
        nColumn = 1;
    }
        break;
    case Screen_2X2:
    {
        nRow = 2;
        nColumn = 2;
    }
        break;
    case Screen_3X3:
    {
        nRow = 3;
        nColumn = 3;
    }
        break;
    case Screen_3X4:
    {
        nRow = 3;
        nColumn = 4;
    }
        break;
    default:
        break;
    }

    auto pLayout = GetLayout();
    int i = 0;
    int j = 0;
    for (auto pItem : m_vcPreviewwnds)
    {
        pLayout->addWidget(pItem->pWnd, i, j++);
        if (j == nColumn)
        {
            j = 0;
            i++;
        }
    }

    m_currentStyle = devideType;
}
