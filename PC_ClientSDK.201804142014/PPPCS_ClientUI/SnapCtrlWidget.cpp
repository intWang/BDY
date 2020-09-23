#include "SnapCtrlWidget.h"
#include "QtDefine.h"
#include "ConfigCenter.h"
SnapCtrlWidget::SnapCtrlWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    setEnabled(false);
    ui.cboSyncSpeed->addItem("同步播放", SyncSpeed::Sync_1);
    ui.cboSyncSpeed->addItem("1/2 倍速", SyncSpeed::Sync_2);
    ui.cboSyncSpeed->addItem("1/4 倍速", SyncSpeed::Sync_4);
    ui.cboSyncSpeed->addItem("1/8 倍速", SyncSpeed::Sync_8);
    ui.cboSyncSpeed->addItem("1/16 倍速", SyncSpeed::Sync_16);

    QColor clr;
    clr.setRgb(220, 220, 220);
    ui.cboSyncSpeed->setItemData(0, clr, Qt::TextColorRole);
    ui.cboSyncSpeed->setItemData(1, clr, Qt::TextColorRole);
    ui.cboSyncSpeed->setItemData(2, clr, Qt::TextColorRole);
    ui.cboSyncSpeed->setItemData(3, clr, Qt::TextColorRole);
    ui.cboSyncSpeed->setItemData(4, clr, Qt::TextColorRole);

    m_pAutoStopTrack = new QTimer(this);
    connect(m_pAutoStopTrack, &QTimer::timeout, this, &SnapCtrlWidget::OnTimeOut);

    connect(ui.btnStartSync, &QPushButton::clicked, this, &SnapCtrlWidget::OnBtnClicked);
    connect(ui.btnStopSync, &QPushButton::clicked, this, &SnapCtrlWidget::OnBtnClicked);
    connect(ui.btnSnap, &QPushButton::clicked, this, &SnapCtrlWidget::OnBtnClicked);
    connect(ui.btnPreView, &QPushButton::clicked, this, &SnapCtrlWidget::OnBtnClicked);
    connect(ui.btnPostView, &QPushButton::clicked, this, &SnapCtrlWidget::OnBtnClicked);
    connect(ui.btnPause, &QPushButton::clicked, this, &SnapCtrlWidget::OnBtnClicked);


    connect(ui.spinSnapWidth, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapCtrlWidget::OnSpnValueChanged);
    connect(ui.spinSnapHeight, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapCtrlWidget::OnSpnValueChanged);
    connect(ui.spinRow, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapCtrlWidget::OnSpnValueChanged);
    connect(ui.spinColumn, QOverload<int>::of(&QSpinBox::valueChanged), this, &SnapCtrlWidget::OnSpnValueChanged);

    connect(ui.cboSyncSpeed, QOverload<int>::of(&QComboBox::activated), this, &SnapCtrlWidget::OnSelSpeedChanged);

    connect(ui.chkColumnLayout, &QCheckBox::stateChanged, this, &SnapCtrlWidget::OnBtnChecked);
    connect(ui.chkPopSnap, &QCheckBox::stateChanged, this, &SnapCtrlWidget::OnBtnChecked);
    connect(ui.chkLockProportion, &QCheckBox::stateChanged, this, &SnapCtrlWidget::OnBtnChecked);

    LoadConfig();
}

SnapCtrlWidget::~SnapCtrlWidget()
{
}


void SnapCtrlWidget::StartCtrl()
{
    setEnabled(true);
    ui.btnStartSync->setEnabled(true);
    ui.btnStopSync->setEnabled(false);
    ui.btnSnap->setEnabled(false);
    ui.btnPreView->setEnabled(false);
    ui.btnPostView->setEnabled(false);
    ui.btnPause->setEnabled(false);


//     ui.spinSnapWidth->setEnabled(true);
//     ui.spinSnapHeight->setEnabled(true);
//     ui.chkLock->setEnabled(true);
//     ui.cboSyncSpeed->setEnabled(true);
//     ui.spinRow->setEnabled(true);
//     ui.spinColumn->setEnabled(true);
//     ui.chkColumnLayout->setEnabled(true);
//     ui.chkPopSnap->setEnabled(true);
//     ui.spinAutoStop->setEnabled(true);
}

void SnapCtrlWidget::EndCtrl()
{
    setEnabled(false);
}

void SnapCtrlWidget::SetRectMaxSize(QSize stSize)
{
    ui.spinSnapWidth->setMaximum(stSize.width());
    ui.spinSnapHeight->setMaximum(stSize.height());
}

void SnapCtrlWidget::SetCountDown(int nSec)
{
    if (nSec != 0)
    {
        QString strShow = "自动停止(%1s)";
        strShow = strShow.arg(nSec);
        ui.lblAutoCountDown->setText(strShow);
    }
    else
    {
        QString strShow = "自动停止(1~%1s)";
        strShow = strShow.arg(m_nMaxAutoStopTime);
        ui.lblAutoCountDown->setText(strShow);
    }
   
}

void SnapCtrlWidget::OnPlayBackFramIndex(int nFramIndex)
{

}

void SnapCtrlWidget::OnPlayBackDataReady(int nFramTotal)
{
    m_nTotalPBFrame = nFramTotal;
    if (m_nTotalPBFrame)
    {
        ui.btnPreView->setEnabled(true);
        ui.btnPostView->setEnabled(true);
    }
}

void SnapCtrlWidget::OnBtnClicked()
{
    auto pBtn = qobject_cast<QPushButtonPtr>(sender());
    if (pBtn == ui.btnStartSync)
    {
        ui.btnStartSync->setEnabled(false);
        ui.btnStopSync->setEnabled(true);
        ui.btnSnap->setEnabled(true);
        ui.btnPreView->setEnabled(false);
        ui.btnPostView->setEnabled(false);
        ui.btnPause->setEnabled(false);
        emit StartSync();
        StartAutoStopTrack();
    }
    else if(pBtn == ui.btnStopSync)
    {
        ui.btnStartSync->setEnabled(true);
        ui.btnStopSync->setEnabled(false);
        ui.btnPause->setEnabled(false);
        ui.btnSnap->setEnabled(false);
        EndAutoStopTrack();
        emit StopSync();
    }
    else if (pBtn == ui.btnPreView)
    {
        emit StartPrePlay();
        ui.btnPause->setEnabled(true);
        ui.btnSnap->setEnabled(true);
    }
    else if (pBtn == ui.btnPostView)
    {
        emit StartPostPlay();
        ui.btnPause->setEnabled(true);
        ui.btnSnap->setEnabled(true);
    }
    else if (pBtn == ui.btnPause)
    {
        if (ui.btnPause->text() == "暂停")
        {
            emit PauseResume(true);
            ui.btnPause->setText("恢复");
        }
        else
        {
            emit PauseResume(false);
            ui.btnPause->setText("暂停");
        }
    }
    else if (pBtn == ui.btnSnap)
    {
        emit SnapShot();
    }
}

void SnapCtrlWidget::OnSelSpeedChanged(int nIndex)
{
    auto selData = ui.cboSyncSpeed->currentData();

    if (m_pSnapModeParam)
    {
        m_pSnapModeParam->SetSpeed((SyncSpeed)selData.toUInt());
    }

}

void SnapCtrlWidget::OnSpnValueChanged(int nValue)
{
    auto pSpin = qobject_cast<QSpinBox*>(sender());
    if ((pSpin == ui.spinSnapWidth))
    {
        int nWidth = nValue;
        if (m_bLocalProportion != 0)
        {
             int nHeight = (nWidth * 1000.0 / m_bLocalProportion ) / 1000.0;
             ui.spinSnapHeight->setValue(nHeight);
        }
        if (m_pSnapModeParam)
        {
            m_pSnapModeParam->nSelRectWidth.store(nWidth);
        }
    }
    else if (pSpin == ui.spinSnapHeight)
    {
        int nHeight = nValue;
        if (m_bLocalProportion != 0)
        {
            int nWidth =( nHeight * 1000.0 * m_bLocalProportion)/ 1000.0;
            ui.spinSnapWidth->setValue(nWidth);
        }
        if (m_pSnapModeParam)
        {
            m_pSnapModeParam->nSelRectHeight.store(nHeight);
        }
    }
    else if ((pSpin == ui.spinRow))
    {
        emit LayoutChanged(nValue, ui.spinColumn->value());
        if (m_pSnapModeParam)
        {
            m_pSnapModeParam->nWndRows.store(nValue);
        }
    }
    else if ((pSpin == ui.spinColumn))
    {
        emit LayoutChanged(ui.spinRow->value(), nValue);
        if (m_pSnapModeParam)
        {
            m_pSnapModeParam->nWndColumns.store(nValue);
        }
    }
    else if(pSpin == ui.spinAutoStop)
    {
        if (m_pSnapModeParam)
        {
            m_pSnapModeParam->nAutoStop.store(nValue);
        }
    }

}

void SnapCtrlWidget::OnBtnChecked(bool bChecked)
{
    auto pCheckBox = qobject_cast<QCheckBoxPtr>(sender());
    if (pCheckBox == ui.chkColumnLayout)
    {
        emit LayoutDirChanged(bChecked);
        if (m_pSnapModeParam)
        {
            m_pSnapModeParam->bRowFirst.store(bChecked);
        }
    }
    else if (pCheckBox == ui.chkPopSnap)
    {
        emit PopFullPic(bChecked);
        if (m_pSnapModeParam)
        {
            m_pSnapModeParam->bPopFull.store(bChecked);
        }
    }
    else if (pCheckBox == ui.chkLockProportion)
    {
        if (bChecked)
        {
            int nSelrcWidth = ui.spinSnapWidth->value();
            int nSelrcHeight = ui.spinSnapHeight->value();
            if (nSelrcWidth ==0 || nSelrcHeight == 0)
            {
                ui.chkLockProportion->setChecked(false);
                return;
            }
            m_bLocalProportion = nSelrcWidth * 1.0 / nSelrcHeight;
        }
        else
        {
            m_bLocalProportion = 0;
        }

        if (m_pSnapModeParam)
        {
            m_pSnapModeParam->bLockProp.store(bChecked);
        }
    }
}

void SnapCtrlWidget::OnTimeOut()
{
    auto pTimer = qobject_cast<QTimer*>(sender());
    if (pTimer == m_pAutoStopTrack)
    {
        m_nTrackTimes++;
        if (m_pSnapModeParam && m_nTrackTimes >= m_pSnapModeParam->nAutoStop)
        {
            emit ui.btnStopSync->clicked();
        }
        SetCountDown(m_pSnapModeParam->nAutoStop - m_nTrackTimes);
    }
}

void SnapCtrlWidget::StartAutoStopTrack()
{
    if (m_pAutoStopTrack)
    {
        m_pSnapModeParam->nAutoStop = ui.spinAutoStop->value();
        m_pAutoStopTrack->start(1000);
        m_nTrackTimes = 0;
        SetCountDown(m_pSnapModeParam->nAutoStop);
    }
}

void SnapCtrlWidget::EndAutoStopTrack()
{
    if (m_pAutoStopTrack && m_pAutoStopTrack->isActive())
    {
        m_pAutoStopTrack->stop();
    }
    SetCountDown(0);
}

void SnapCtrlWidget::LoadConfig()
{
    m_pSnapModeParam = ConfigCenter::GetInstance().GetSnapModeParam();
    if (m_pSnapModeParam)
    {
        ui.spinSnapWidth->setValue(m_pSnapModeParam->nSelRectWidth);
        ui.spinSnapHeight->setValue(m_pSnapModeParam->nSelRectHeight);
        ui.spinRow->setValue(m_pSnapModeParam->nWndRows);
        ui.spinColumn->setValue(m_pSnapModeParam->nWndColumns);
        ui.cboSyncSpeed->setCurrentIndex(m_pSnapModeParam->nSyncSpped);
        ui.spinAutoStop->setValue(m_pSnapModeParam->nAutoStop);
        ui.chkLockProportion->setChecked(m_pSnapModeParam->bLockProp);
        ui.chkColumnLayout->setChecked(m_pSnapModeParam->bRowFirst);
        ui.chkPopSnap->setChecked(m_pSnapModeParam->bPopFull);

        m_nMaxAutoStopTime = m_pSnapModeParam->nMaxAutoStop;
        ui.spinAutoStop->setMaximum(m_nMaxAutoStopTime);
        SetCountDown(0);
    }
}
