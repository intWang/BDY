#pragma once

#include "ui_PreviewRealWnd.h"
#include "AreableWidget.h"
#include "DataStruct.h"
#include "DrawWnd.h"
#include "IServer.h"
#include <QIcon>
class PreviewRealWnd : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    using BaseClass = AreableWidget<QWidget>;
    enum Status
    {
        Empty,
        StartingPreview,
        InPreview,
        StartingRecord,
        Record,
        Reconnecting,
        InSync,
        InPicture,
    };

    struct StageInfo
    {
        DevNode::Ptr pChannel = nullptr;
        bool bSelected = false;

        void Clear()
        {
            pChannel = nullptr;
            bSelected = false;
        }
    };

    using Ptr = PreviewRealWnd * ;
    PreviewRealWnd(int nIndex, QWidget *parent = Q_NULLPTR);
    ~PreviewRealWnd();

    void OnFrameData(const std::string& strUid, FrameData::Ptr pFrame);
    void OnRecordNotify(const std::string& strUid, bool bStart);

    void StartPreview(DevNode::Ptr pChannel);
    void StopPreview();

    void StartStream(DevNode::Ptr pChannel);
    void StopStream();
    
    void StartSync(DevNode::Ptr pChannel);
    void StopSync();

    void Clear();
    void ClearPicture(); // picture mode
    void SetSelectStatu(bool bSelect);
    bool GetSelectStatu();
    void SetRuningStatu(Status state);
    Status GetRuningStatu();
    time_t GetBusyTime();
    int GetPtzSpeed();
    void SetBottomEnable(bool bEnable);
    int IsInPreview();
    DevNode::Ptr GetDevNode();
    SnapData::Ptr StreamSnapShot();
    bool CheckDevLostConnect(const std::string& strLostDev);

    void SetWndMode(PanelMode emMode);

    void StartPlayBackTimer();
    void StopPlayBackTimer();

    void StartHideBottomTimer();
    void StopHideBottomTimer();

    void OnTimeout();

    void ShowFrame(SnapData::Ptr  pFrame);

    void ResetFullLevel(int nLevel);
public slots:
    void OnPtzCtrl(PtzCommand emCmd, int nParam);
    void CallConfig();
    void SnapShot();
    void DoRecord();
    void UpdateRecordStatu(bool bStart);
    void StartPlayBack(bool bPre = false);
    void StopPlayBack();
    void PauseResumePlayBack(bool bPause = true);
    void OnPBDataReady(int nDataSize);
    void OnCustomedSnap(SnapData::Ptr pSnap);

signals:
    void MouseMove(QPoint pt);
    void MouseLeave();
    void MouseClicked();
    void PreviewWndUserClick();
    void PreviewWndUserDBClick(int nFull); // 0-normal 1-full 2-fullscreen
    void PreviewWndStopPreview(const QString& strUid, PreviewRealWnd::Ptr pWnd);
    void PreviewWndStartPreview(const QString& strUid, PreviewRealWnd::Ptr pWnd);
    void RecordStatuChanged(bool bStart);
    void PBDataReady(int nDataSize);
    void CustomedSnap(SnapData::Ptr pSnap);
private:
    //Ui::PreviewRealWnd ui;
    Status m_curStatus = Status::Empty;
    DrawWnd::Ptr m_DrawWnd = nullptr;
    DevNode::Ptr m_pChannel = nullptr;
    ls::IIPCNetServerCallBack::CallBackFunc::Ptr m_CallBackFunc = nullptr;
    time_t m_tmBusy = 0;
    bool m_bSelected = false;
    bool m_bRecord = false;
    std::atomic<PanelMode> m_nWndMode = {PanelMode::PreviewMode};
    VideoParamData m_stVideParam = { 0 };

    QPushButtonPtr m_pRecordBtn = nullptr;
    QIcon m_iconRecordOff;
    QIcon m_iconRecordOn;
    StageInfo m_stageInfo;

    SnapModeParam::Ptr m_pSnapModeParam = nullptr;
    QSlider* m_pSliderPB = nullptr;
    QTimer* m_pTimerAutoPlay = nullptr;
    QTimer* m_pTimerHideBottom = nullptr;
    int m_nTotalFrame = 0;
    int m_nCurIndex = 0;
    int m_nCurDir = 1;
    int m_nfullLevel = 0;
    bool m_bHideBottom = false;
    time_t m_tmLastMove = 0;
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual BarWidget::Ptr InitBottomBar() override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

protected:
    void LoadSnapModeParam();
    void HideBottom(bool bHide);
};
