#pragma once

#include <QWidget>
#include "AreableWidget.h"
#include "ui_PreviewPanel.h"
#include "PreviewRealWnd.h"
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
private:
    Ui::PreviewPanel ui;
    PanelMode m_curPanelmode = PanelMode::PreviewMode;
    std::vector<PreviewRealWnd::Ptr> m_vcPreviewRealWnds;
    DevideScreen m_curScreenMode = DevideScreen::Screen_1X1;
    QGridLayoutPtr m_pRealWnds = nullptr;
};
