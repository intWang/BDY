#include "SnapControlWnd.h"
#include "QtDefine.h"
#include "PreviewRealWnd.h"
#include "SnapCtrlWidget.h"
SnapControlWnd::SnapControlWnd(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    SetArea(0, 0);
    SetAreaBk(s_qcl292C39, s_qcl292C39, s_qcl292C39);
    auto pMainLayout = MQ(QVBoxLayout)(this);
    auto pViewWnd = new PreviewRealWnd(-1, this);
    auto pDevideLine = MQ(QLabel)(this);
    auto pSnapWidget = MQ(SnapCtrlWidget)(this);
    pDevideLine->setObjectName("DevideLine");
    pDevideLine->setFrameShape(QFrame::HLine);
    pDevideLine->setFrameShadow(QFrame::Raised);
    pDevideLine->setFixedHeight(2);
    pMainLayout->addWidget(pViewWnd);
    pMainLayout->addWidget(pDevideLine);
    pMainLayout->addWidget(pSnapWidget);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pViewWnd->SetNoBottom();
    pViewWnd->SetWndMode(PanelMode::SnapMode);
    //setEnabled(false);
    setLayout(pMainLayout);

    m_pPreviewRealWnd = pViewWnd;
    m_pSnapCtrlWidget = pSnapWidget;

    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::StartSync, this, &SnapControlWnd::OnStartSync);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::StopSync, this, &SnapControlWnd::OnStopSync);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::SnapShot, this, &SnapControlWnd::OnSnapShot);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::StartPrePlay, this, &SnapControlWnd::OnStartPrePlay);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::StartPostPlay, this, &SnapControlWnd::OnStartPostPlay);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::PauseResume, this, &SnapControlWnd::OnPauseResume);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::SelRectSizeChanged, this, &SnapControlWnd::OnSelRectSizeChanged);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::LayoutChanged, this, &SnapControlWnd::OnLayoutChanged);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::LayoutDirChanged, this, &SnapControlWnd::OnLayoutDirChanged);
    connect(m_pSnapCtrlWidget, &SnapCtrlWidget::PopFullPic, this, &SnapControlWnd::OnPopFullPic);
    
    connect(m_pPreviewRealWnd, &PreviewRealWnd::PBDataReady, m_pSnapCtrlWidget, &SnapCtrlWidget::OnPlayBackDataReady);
    connect(m_pPreviewRealWnd, &PreviewRealWnd::PreviewWndStopPreview, this, &SnapControlWnd::OnPreviewStopped);
    connect(m_pPreviewRealWnd, &PreviewRealWnd::PreviewWndStartPreview, this, &SnapControlWnd::OnPreviewStarted);
    connect(m_pPreviewRealWnd, &PreviewRealWnd::CustomedSnap, this, &SnapControlWnd::OnCustomedSnap);
}

SnapControlWnd::~SnapControlWnd()
{

}

void SnapControlWnd::OnEnterSnapMode()
{
    m_bEnterSnapMode = true;
}

void SnapControlWnd::OnLeaveSnapMode()
{
    m_bEnterSnapMode = false;
    StopSync();
    if (m_pSnapCtrlWidget)
    {
        m_pSnapCtrlWidget->EndCtrl();
    }
}


void SnapControlWnd::OnPreviewStopped(const QString& strUid)
{
    emit PreviewStatuChanged(strUid, false);
}

void SnapControlWnd::OnPreviewStarted(const QString& strUid)
{
    emit PreviewStatuChanged(strUid, true);
}

void SnapControlWnd::OnStartPreview(DevNode::Ptr pChannel)
{
    if (m_bEnterSnapMode)
    {
        if (m_pChannel && m_pChannel != pChannel)
        {
            StopSync();
        }
        m_pChannel = pChannel;

        if (m_pChannel && m_pSnapCtrlWidget)
        {
            m_pSnapCtrlWidget->StartCtrl();
        }
    }
}

void SnapControlWnd::OnStopPreview()
{

    if (m_pPreviewRealWnd)
    {
        m_pPreviewRealWnd->StopSync();
    }
    if (m_pSnapCtrlWidget)
    {
        m_pSnapCtrlWidget->EndCtrl();
    }

    if (m_pChannel)
    {
        m_pChannel = nullptr;
    }
}

void SnapControlWnd::OnStartSync()
{
    StartSync();
}

void SnapControlWnd::OnStopSync()
{
    StopSync();
}

void SnapControlWnd::OnSnapShot()
{
    if (m_pPreviewRealWnd)
    {
        auto pFrame = m_pPreviewRealWnd->StreamSnapShot();
        if (pFrame)
        {
            emit StreamSnapShot(pFrame);
        }
    }
}

void SnapControlWnd::OnStartPrePlay()
{
    if (m_pPreviewRealWnd)
    {
        m_pPreviewRealWnd->StartPlayBack(true);
    }
}

void SnapControlWnd::OnStartPostPlay()
{
    if (m_pPreviewRealWnd)
    {
        m_pPreviewRealWnd->StartPlayBack(false);
    }
}

void SnapControlWnd::OnPauseResume(bool bPause)
{
    if (m_pPreviewRealWnd)
    {
        m_pPreviewRealWnd->PauseResumePlayBack(bPause);
    }
}

void SnapControlWnd::OnSelRectSizeChanged(int width, int height)
{

}

void SnapControlWnd::OnLayoutChanged(int nRow, int nColumn)
{
    emit CustomWndLayout(nRow, nColumn);
}

void SnapControlWnd::OnLayoutDirChanged(bool bRow)
{
    emit LayoutDirChanged(bRow);
}

void SnapControlWnd::OnPopFullPic(bool bPop)
{

}

void SnapControlWnd::OnCustomedSnap(SnapData::Ptr pSnap)
{
    emit StreamSnapShot(pSnap);
}

void SnapControlWnd::StopSync()
{
    if (m_pChannel && m_pPreviewRealWnd)
    {
        m_pPreviewRealWnd->StopSync();
        m_pPreviewRealWnd->StopPlayBack();
    }
}

void SnapControlWnd::StartSync()
{
    if (m_pChannel && m_pPreviewRealWnd)
    {
        m_pPreviewRealWnd->StartSync(m_pChannel);
    }

}

