#pragma once

#include <QWidget>
#include "AreableWidget.h"
#include "ui_PreviewPanel.h"
#include "PreviewRealWnd.h"
#include "DataStruct.h"
#include <QIcon>
class PreviewPanel : public AreableWidget<QWidget>
{
    Q_OBJECT
public:

    using Ptr = PreviewPanel*;
    PreviewPanel(QWidget *parent = Q_NULLPTR);
    ~PreviewPanel();

    void OnScreenDevideChange(DevideScreen newMode);
signals:
    void PreviewStatuChanged(const QString& strUid, bool bStatu);
    void SelectPreviewWnd(PreviewRealWnd::Ptr pWnd);
    void PanelModeChanged(PanelMode emMode, DevNode::Ptr pNode);
    void CallPopUpTreeWnd();
    void FullScreen(bool bFull);
protected:
    virtual BarWidget::Ptr InitTopBar();
    virtual BarWidget::Ptr InitBottomBar();
    void InitPreviewRealWnds();
    void PraperPreviewRealWnds(int nNums);
    void SetSelectWnd(PreviewRealWnd::Ptr pWnd, bool bForceSel = false);
    void SetCurrentMode(PanelMode emMode);
    void SetPreviewWndMode(PanelMode nModetype);
    void StartPreview4PreviewMode(DevNode::Ptr pChannel);
    PreviewRealWnd::Ptr GetFreeWnd();

    void StartTopBarCheck();
    void StopTopBarCheck();
    void ClearAllWnd();

    virtual void keyPressEvent(QKeyEvent *event) override;


public slots:
    void OnStartPreview(DevNode::Ptr pChannel);
    void OnPreviewStopped(const QString& strUid, PreviewRealWnd::Ptr pWnd);
    void OnPreviewStarted(const QString& strUid, PreviewRealWnd::Ptr pWnd);
    void OnDeviceLostConnect(const QString& strUID);
    void OnPreveiwWndSelChange();
    void OnPreveiwWndSelFull(int nFull);
    void OnScreenDevideModeChange(int nIndex);
    void OnStreamSnapShot(SnapData::Ptr pFrame);
    void SetCustomWndLayout(int row, int column);
    void OnLayoutDirChanged(bool bChanged);
    void OnFullScreen();
    void OnTimeout();
private:
    Ui::PreviewPanel ui;
    PanelMode m_curPanelmode = PanelMode::PreviewMode;
    std::vector<PreviewRealWnd::Ptr> m_vcPreviewRealWnds;
    std::map<PanelMode, ::vector<QWidgetPtr>> m_mapModeWidgets;
    PreviewRealWnd::Ptr m_pCurSelWnd = nullptr;
    DevideScreen m_curScreenMode = DevideScreen::Screen_1X1;
    DevideScreen m_lastScreenMode = DevideScreen::Screen_1X1;
    QGridLayoutPtr m_pRealWnds = nullptr;
    QComboBoxPtr m_pCbbDevideScreen = nullptr;

    QPushButtonPtr m_pSnapMode = nullptr;
    QPushButtonPtr m_pFullScrren = nullptr;
    QIcon m_icon1X1;
    QIcon m_icon2X2;
    QIcon m_icon3X3;
    QIcon m_icon4X3;
    int m_nCustomRow = 0;
    int m_nCustomColumn = 0;
    bool m_bDirExchanged = false;
    bool m_bFull = false;
    QWidget* m_pParent = nullptr;
    QWidget* m_pViewWndWidget = nullptr;
    QTimer* m_pTimerTopBarCheck = nullptr;
};
