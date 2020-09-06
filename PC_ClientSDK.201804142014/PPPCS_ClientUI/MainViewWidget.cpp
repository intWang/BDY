#include "MainViewWidget.h"
#include "QtDefine.h"
#include "PreviewPanel.h"
#include "utils.h"
#include "LogManager.h"
#include "DevTreeWnd.h"
#include "ChannelCtrlWidget.h"
#include "SubPreviewWnd.h"
#include "SnapControlWnd.h"
#include "PopUpTreeWnd.h"
#include <QDesktopWidget>
MainViewWidget::MainViewWidget(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
   
    auto pLeftLayout = ui.leftLayout;
    auto pRightLayout = ui.rightLayout;
    auto pPreviewPanel = MQ(PreviewPanel)(this);

    //////Preview Mode Wnds
    auto pTabWnd = MQ(ChannelCtrlWidget)(this);
    auto pTreeView = MQ(DevTreeWnd)(this);
    m_pPopTreeWnd = MQ(PopUpTreeWnd)(this);
    m_pPopTreeWnd->setWindowModality(Qt::NonModal);

    if (utils::CheckPointer({ pLeftLayout , pPreviewPanel}))
    {
        LogError("Error param  %d %d ", pLeftLayout, pPreviewPanel);
    }
    connect(pTreeView, &DevTreeWnd::ChannelNodeDBClick, pPreviewPanel, &PreviewPanel::OnStartPreview);
    connect(pTreeView, &DevTreeWnd::DeviceLostConnect, pPreviewPanel, &PreviewPanel::OnDeviceLostConnect, Qt::QueuedConnection);
    connect(pPreviewPanel, &PreviewPanel::SelectPreviewWnd, pTabWnd, &ChannelCtrlWidget::BindPreviewWnd);
    connect(pPreviewPanel, &PreviewPanel::PreviewStatuChanged, pTreeView, &DevTreeWnd::OnPreviewStatuChanged);
    connect(pPreviewPanel, &PreviewPanel::PanelModeChanged, this, &MainViewWidget::OnPageModeChanged);
    connect(pPreviewPanel, &PreviewPanel::CallPopUpTreeWnd, this, &MainViewWidget::OnCallPopUpTreeWnd);
    connect(pPreviewPanel, &PreviewPanel::FullScreen, this, &MainViewWidget::OnPanelFullScreen);
    
    m_pTreeWnd = pTreeView;
    /////Snap Mode wnds
    auto pSubPreviewWnd = MQ(SubPreviewWnd)(this);
    auto pSnapControlWnd = MQ(SnapControlWnd)(this);

    connect(pTreeView, &DevTreeWnd::ChannelNodeDBClick, pSubPreviewWnd, &SubPreviewWnd::OnStartPreview);
    connect(pTreeView, &DevTreeWnd::ChannelNodeDBClick, pSnapControlWnd, &SnapControlWnd::OnStartPreview);
    connect(this, &MainViewWidget::EnterSnapMode, pSubPreviewWnd, &SubPreviewWnd::OnEnterSnapMode);
    connect(this, &MainViewWidget::LeaveSnapMode, pSubPreviewWnd, &SubPreviewWnd::OnLeaveSnapMode);
    connect(this, &MainViewWidget::EnterSnapMode, pSnapControlWnd, &SnapControlWnd::OnEnterSnapMode);
    connect(this, &MainViewWidget::LeaveSnapMode, pSnapControlWnd, &SnapControlWnd::OnLeaveSnapMode);
    connect(pSubPreviewWnd, &SubPreviewWnd::PreviewStatuChanged, pTreeView, &DevTreeWnd::OnPreviewStatuChanged);
    connect(pSnapControlWnd, &SnapControlWnd::PreviewStatuChanged, pTreeView, &DevTreeWnd::OnPreviewStatuChanged);
    connect(pSnapControlWnd, &SnapControlWnd::StreamSnapShot, pPreviewPanel, &PreviewPanel::OnStreamSnapShot);
    connect(pSnapControlWnd, &SnapControlWnd::CustomWndLayout, pPreviewPanel, &PreviewPanel::SetCustomWndLayout);
    connect(pSnapControlWnd, &SnapControlWnd::LayoutDirChanged, pPreviewPanel, &PreviewPanel::OnLayoutDirChanged);

    m_pSubPreview = pSubPreviewWnd;
    ////End Init widget
    pLeftLayout->addWidget(pPreviewPanel);
    pRightLayout->addWidget(pTabWnd);
    pRightLayout->addWidget(pTreeView);
    pRightLayout->addWidget(pSubPreviewWnd);
    pRightLayout->addWidget(pSnapControlWnd);

    m_mapModeWidget[PanelMode::PreviewMode] = { pTabWnd ,pTreeView };
    m_mapModeWidget[PanelMode::PictureMode] = { pSubPreviewWnd ,pSnapControlWnd };

    OnPageModeChanged(PanelMode::PreviewMode);
    m_pPopTreeWnd->hide();
    LogInfo("MainViewWidget Created!");
}

MainViewWidget::~MainViewWidget()
{
}

void MainViewWidget::OnPageModeChanged(PanelMode nMode)
{
    if (m_pPopTreeWnd)
    {
        if (m_pPopTreeWnd->isVisible())
        {
            m_pPopTreeWnd->hide();
        }

        if (auto pTree = m_pPopTreeWnd->GetTreeView())
        {
            pTree->setParent(this);
            auto pRightLayout = ui.rightLayout;
            pRightLayout->insertWidget(1, pTree);
            m_pPopTreeWnd->SetTreeView(nullptr);
        }
    }

    for (auto pairWidgets : m_mapModeWidget)
    {
        auto vcWidgets = pairWidgets.second;
        bool bShow = (nMode == pairWidgets.first);
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

    auto pRightLayout = ui.rightLayout;
    switch (nMode)
    {
    case PreviewMode:
    {
        pRightLayout->setStretch(0, 2);
        pRightLayout->setStretch(1, 3);
        emit LeaveSnapMode();
    }
        break;
    case PictureMode:
    {
        pRightLayout->setStretch(2, 3);
        pRightLayout->setStretch(3, 5);
        emit EnterSnapMode();
    }
        break;
    default:
        break;
    }
}

void MainViewWidget::OnCallPopUpTreeWnd()
{
    if (m_pTreeWnd && m_pPopTreeWnd)
    {
        m_pTreeWnd->setParent(m_pPopTreeWnd);
        m_pPopTreeWnd->SetTreeView(m_pTreeWnd);
        m_pPopTreeWnd->show();
        m_pTreeWnd->show();
    }
}

void MainViewWidget::OnPanelFullScreen(bool bFull)
{
    if (auto pWidget = qobject_cast<QWidgetPtr>(sender()))
    {
        if (bFull)
        {
            auto pLeftLayout = ui.leftLayout;
            pLeftLayout->removeWidget(pWidget);
            pWidget->setParent(nullptr);
            pWidget->setWindowFlags(Qt::Dialog);
            pWidget->showFullScreen();
            QDesktopWidget* desktopWidget = QApplication::desktop();
            pWidget->setMaximumSize(desktopWidget->width(), desktopWidget->height());
        }
        else
        {
            pWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            pWidget->setParent(this);
            pWidget->setWindowFlags(Qt::SubWindow);
            pWidget->showNormal();
            auto pLeftLayout = ui.leftLayout;
            pLeftLayout->addWidget(pWidget);
        }
    }
}

void MainViewWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        return;
    }
}
