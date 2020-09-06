#include "PtzCtrlWnd.h"
#include "QtDefine.h"
PtzCtrlWnd::PtzCtrlWnd(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    m_pPtzCtrl = MQ(PTZCtrl)(this);
    ui.ptzctrlLayout->addWidget(m_pPtzCtrl);

    ui.speedSlider->setMinimum(1);
    ui.speedSlider->setMaximum(10);
    ui.speedSlider->setSingleStep(1);
    ui.speedSlider->setPageStep(1);
    EnableCtrl(false);

    connect(m_pPtzCtrl, &PTZCtrl::DirBtnClick, this, &PtzCtrlWnd::OnPtzCtrls);
    connect(ui.btnZoomIn, &QPushButton::clicked, this, &PtzCtrlWnd::OnBtnClick);
    connect(ui.btnZoomout, &QPushButton::clicked, this, &PtzCtrlWnd::OnBtnClick);
    connect(ui.speedSlider, &QSlider::valueChanged, this, &PtzCtrlWnd::OnSlideChange);

    ui.btnZoomIn->setAutoRepeat(true); //启用长按
    ui.btnZoomIn->setAutoRepeatDelay(400);//触发长按的时间
    ui.btnZoomIn->setAutoRepeatInterval(100);//长按时click信号间隔
    ui.btnZoomout->setAutoRepeat(true); //启用长按
    ui.btnZoomout->setAutoRepeatDelay(400);//触发长按的时间
    ui.btnZoomout->setAutoRepeatInterval(100);//长按时click信号间隔
}

PtzCtrlWnd::~PtzCtrlWnd()
{
}

void PtzCtrlWnd::BindPreviewWnd(PreviewRealWnd::Ptr pWnd)
{
    if (m_pCurrentWnd)
    {
        disconnect(this, 0, m_pCurrentWnd, 0);
    }

    m_pCurrentWnd = pWnd;
    EnableCtrl(m_pCurrentWnd != nullptr);

    if (m_pCurrentWnd)
    {
        ui.speedSlider->setValue(m_pCurrentWnd->GetPtzSpeed());
        connect(this, &PtzCtrlWnd::PtzTriggerCtrl, m_pCurrentWnd, &PreviewRealWnd::OnPtzCtrl);
    }
}

void PtzCtrlWnd::EnableCtrl(bool bEnable)
{
    ui.btnZoomIn->setEnabled(bEnable);
    ui.btnZoomout->setEnabled(bEnable);
    ui.speedSlider->setEnabled(bEnable);
    if (m_pPtzCtrl)
    {
        m_pPtzCtrl->EnableCtrl(bEnable);
    }
}

void PtzCtrlWnd::OnPtzCtrls(PtzCommand emCmd)
{
    emit PtzTriggerCtrl(emCmd, 0);
}

void PtzCtrlWnd::OnBtnClick()
{
    auto pBtn = qobject_cast<QPushButtonPtr>(sender());
    if (pBtn == ui.btnZoomIn)
    {
        OnPtzCtrls(PtzCommand::ZoomIn);
    }
    else if (pBtn == ui.btnZoomout)
    {
        OnPtzCtrls(PtzCommand::ZoomOut);
    }
}

void PtzCtrlWnd::OnSlideChange(int nValue)
{
    ui.lblSliderValue->setText(QVariant::fromValue(nValue).toString());
    emit PtzTriggerCtrl(PtzCommand::SetSpeed, nValue);

}
