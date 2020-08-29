#pragma once

#include <QWidget>
#include "ui_PtzCtrlWnd.h"
#include "PTZCtrl.h"
#include "PreviewRealWnd.h"
class PtzCtrlWnd : public QWidget
{
    Q_OBJECT

public:
    using Ptr = PtzCtrlWnd * ;
    PtzCtrlWnd(QWidget *parent = Q_NULLPTR);
    ~PtzCtrlWnd();

    void BindPreviewWnd(PreviewRealWnd::Ptr pWnd);

signals:
    void PtzTriggerCtrl(PtzCommand emCmd, int nParam = 0);

protected:

    void EnableCtrl(bool bEnable);
protected slots:
    void OnPtzCtrls(PtzCommand emCmd);
    void OnBtnClick();
    void OnSlideChange(int nValue);

private:
    Ui::PtzCtrlWnd ui;
    PTZCtrl::Ptr m_pPtzCtrl = nullptr;
    PreviewRealWnd::Ptr m_pCurrentWnd = nullptr;
};
