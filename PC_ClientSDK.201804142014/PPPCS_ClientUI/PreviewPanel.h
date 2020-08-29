#pragma once

#include <QWidget>
#include "AreableWidget.h"
#include "ui_PreviewPanel.h"
#include "PreviewRealWnd.h"
#include "DataStruct.h"
class PreviewPanel : public AreableWidget<QWidget>
{
    Q_OBJECT
public:
    enum PanelMode
    {
        PreviewMode,
        SnapMode,
    };

    using Ptr = PreviewPanel*;
    PreviewPanel(QWidget *parent = Q_NULLPTR);
    ~PreviewPanel();

    void OnScreenDevideChange(DevideScreen newMode);

signals:
    void PreviewStatuChanged(const QString& strUid, bool bStatu);
    void SelectPreviewWnd(PreviewRealWnd::Ptr pWnd);

protected:
    virtual BarWidget::Ptr InitTopBar();
    virtual BarWidget::Ptr InitBottomBar();
    void InitTopBar4SnapMode(QHBoxLayoutPtr pBarLayout);
    void InitTopBar4PreviewMode(QHBoxLayoutPtr pBarLayout);
    void InitPreviewRealWnds();
    void PraperPreviewRealWnds(int nNums);
    void SetSelectWnd(PreviewRealWnd::Ptr pWnd, bool bForceSel = false);
    void SetWndFull(PreviewRealWnd::Ptr pWnd);

public slots:
    void OnStartPreview(DevNode::Ptr pChannel);
    void OnStopPreview(const QString& strUid, PreviewRealWnd::Ptr pWnd);
    void OnDeviceLostConnect(const QString& strUID);
    void OnPreveiwWndSelChange();
    void OnPreveiwWndSelFull(bool bFull);
    void OnScreenDevideModeChange(int nIndex);

private:
    Ui::PreviewPanel ui;
    PanelMode m_curPanelmode = PanelMode::PreviewMode;
    std::vector<PreviewRealWnd::Ptr> m_vcPreviewRealWnds;
    PreviewRealWnd::Ptr m_pCurSelWnd = nullptr;
    DevideScreen m_curScreenMode = DevideScreen::Screen_1X1;
    QGridLayoutPtr m_pRealWnds = nullptr;
    QComboBoxPtr m_pCbbDevideScreen = nullptr;
};
