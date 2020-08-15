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
protected:
    virtual BarWidget::Ptr InitTopBar();
    virtual BarWidget::Ptr InitBottomBar();
    void InitTopBar4SnapMode(QHBoxLayoutPtr pBarLayout);
    void InitTopBar4PreviewMode(QHBoxLayoutPtr pBarLayout);
    void InitPreviewRealWnds();
    void PraperPreviewRealWnds(int nNums);
    void SetSelectWnd(PreviewRealWnd::Ptr pWnd);

public slots:
    void OnStartPreview(ChannelNode::Ptr pChannel);
    void OnPreveiwWndSelChange();
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
