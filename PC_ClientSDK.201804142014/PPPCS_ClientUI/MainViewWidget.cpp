#include "MainViewWidget.h"
#include "QtDefine.h"
#include "PreviewPanel.h"
#include "utils.h"
#include "LogManager.h"
#include "DevTreeWnd.h"
MainViewWidget::MainViewWidget(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    auto pLeftLayout = ui.leftLayout;
    auto pRightLayout = ui.rightLayout;
    auto pPreviewPanel = MQ(PreviewPanel)(this);
    //auto pTabWnd = MQ(AreableWidget<QWidget>)(this);
    auto pTreeView = MQ(DevTreeWnd)(this);
    if (utils::CheckPointer({ pLeftLayout , pPreviewPanel}))
    {
        LogError("Error param  %d %d ", pLeftLayout, pPreviewPanel);
    }

    connect(pTreeView, &DevTreeWnd::ChannelNodeDBClick, pPreviewPanel, &PreviewPanel::OnStartPreview);

    pLeftLayout->addWidget(pPreviewPanel);
    //pRightLayout->addWidget(pTabWnd);
    pRightLayout->addWidget(pTreeView);
    pRightLayout->setStretch(0, 2);
    pRightLayout->setStretch(1, 3);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    LogInfo("MainViewWidget Created!");
}

MainViewWidget::~MainViewWidget()
{
}
