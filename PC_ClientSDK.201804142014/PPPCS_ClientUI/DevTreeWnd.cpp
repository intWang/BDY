#include "DevTreeWnd.h"
#include "QtDefine.h"
DevTreeWnd::DevTreeWnd(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    SetArea(45, 45);
    SetAreaBk(s_qcl292C39, s_qcl292C39, s_qcl292C39);

    auto pTopWnd = InitTopBar();
    auto pBottomWnd = InitBottomBar();
    auto pCenterLayout = InitCenterCtl();
   
    ui.mainLayout->addWidget(pTopWnd);
    ui.mainLayout->addLayout(pCenterLayout);
    ui.mainLayout->addWidget(pBottomWnd);
}

DevTreeWnd::~DevTreeWnd()
{
}

BarWidget::Ptr DevTreeWnd::InitTopBar( )
{
    auto pBarWidget = GetTopWnd();
    if (pBarWidget)
    {
        if (auto pTopLayout = pBarWidget->GetLayout())
        {
            auto pLabelLoaded = MQ(QLabel)(this);
            auto pLabelTotal = MQ(QLabel)(this);

            pTopLayout->addWidget(pLabelLoaded);
            pTopLayout->addWidget(pLabelTotal);
            pTopLayout->addStretch();
        }
    }
    return pBarWidget;
}

BarWidget::Ptr  DevTreeWnd::InitBottomBar()
{
    auto pBarWidget = GetBottomWnd();
    if (pBarWidget)
    {
        if (auto pTopLayout = pBarWidget->GetLayout())
        {
            auto pBtnAddDevice = MQ(QPushButton)(this);
            auto pBtnAddGroup = MQ(QPushButton)(this);
            auto pBtnDelete = MQ(QPushButton)(this);

            pTopLayout->addStretch();
            pTopLayout->addWidget(pBtnAddDevice);
            pTopLayout->addWidget(pBtnAddGroup);
            pTopLayout->addWidget(pBtnDelete);
        }
    }
    return pBarWidget;
}

QLayoutPtr DevTreeWnd::InitCenterCtl()
{
    auto pCenterLayout = MQ(QVBoxLayout)(this);
    auto pSubLayout = MQ(QHBoxLayout)(this);

    auto pInout = MQ(QLineEdit)(this);
    auto pBtnSearch = MQ(QPushButton)(this);
    auto pTreeView = MQ(QTreeView)(this);

    pInout->setFixedHeight(30);
    pBtnSearch->setFixedSize(50,30);
    pSubLayout->addWidget(pInout);
    pSubLayout->addWidget(pBtnSearch);
    pCenterLayout->addLayout(pSubLayout);
    pCenterLayout->addWidget(pTreeView);
    pCenterLayout->setContentsMargins(10, 5, 10, 5);
    pCenterLayout->setSpacing(10);

    return pCenterLayout;
}
