#pragma once

#include "AreableWidget.h"
#include "ui_MainViewWidget.h"

class PopUpTreeWnd;
class DevTreeWnd;
class SubPreviewWnd;
class MainViewWidget : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    using Ptr = MainViewWidget * ;
    MainViewWidget(QWidget *parent = Q_NULLPTR);
    ~MainViewWidget();


signals:
    void EnterSnapMode();
    void LeaveSnapMode();

protected slots:
    void OnPageModeChanged(PanelMode nMode);
    void OnCallPopUpTreeWnd();
    void OnPanelFullScreen(bool bFull);

protected:


    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainViewWidget ui;
    std::map<int, std::vector<QWidgetPtr>> m_mapModeWidget;
protected:
    PopUpTreeWnd* m_pPopTreeWnd = nullptr;
    DevTreeWnd* m_pTreeWnd = nullptr;
    SubPreviewWnd* m_pSubPreview = nullptr;
};
