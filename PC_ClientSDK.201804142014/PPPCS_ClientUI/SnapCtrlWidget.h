#pragma once

#include <QWidget>
#include "ui_SnapCtrlWidget.h"
#include "DataStruct.h"
#include <QTimer>
class SnapCtrlWidget : public QWidget
{
    Q_OBJECT

public:
    SnapCtrlWidget(QWidget *parent = Q_NULLPTR);
    ~SnapCtrlWidget();

    void StartCtrl();
    void EndCtrl();
    void SetRectMaxSize(QSize stSize);
    void SetCountDown(int nSec);
signals:
    void StartSync();
    void StopSync();
    void SnapShot();
    void StartPrePlay();
    void StartPostPlay();
    void PauseResume(bool bPause);
    void SelRectSizeChanged(int width, int height);
    void PlaySpeedChanged(int nSpeed);
    void LayoutChanged(int nRow, int nColumn);
    void LayoutDirChanged(bool bRow);
    void PopFullPic(bool bPop);

public slots :
    void OnPlayBackFramIndex(int nFramIndex);
    void OnPlayBackDataReady(int nFramTotal);
    void OnBtnClicked();
    void OnSelSpeedChanged(int nIndex);
    void OnSpnValueChanged(int nValue);
    void OnBtnChecked(bool bChecked);
    void OnTimeOut();
protected:

    void StartAutoStopTrack();
    void EndAutoStopTrack();
    void LoadConfig();
private:
    Ui::SnapCtrlWidget ui;
    SnapModeParam::Ptr m_pSnapModeParam = nullptr;
    QTimer* m_pAutoStopTrack = nullptr;
    int m_nTrackTimes = 0;
    int m_nTotalPBFrame = 0;
    int m_nMaxAutoStopTime = 100;
    double m_bLocalProportion = 0;
};
