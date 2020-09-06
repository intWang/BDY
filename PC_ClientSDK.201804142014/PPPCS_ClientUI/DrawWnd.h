#pragma once

#include <QOpenGLWidget>
#include "DataStruct.h"
#include "YUV420P_Render.h"
#include <queue>
#include <mutex>
#include <QTimer>
#define TIMER_CHECK_FRAME_RATE_INTERVAL 1000
#define TIMER_CHECK_FRAME_UPDATE_INTERVAL (TIMER_CHECK_FRAME_RATE_INTERVAL/20)
#define DEFAULT_FRAME_RATE 20
class DrawWnd:public QWidget
{
    Q_OBJECT
public:
    using Ptr = DrawWnd * ;
    DrawWnd(int index, QWidget *parent = Q_NULLPTR);
    ~DrawWnd();


    void SetPreviewStatu(bool bPreview = false);
    void SetSelected(bool bSelected);
    void InputFrameData(FrameData::Ptr pFrame);
    void AddFrame(FrameData::Ptr pFrame);
    void SetHintString(const QString& strHint);
    void OnPtzCtrl(PtzCommand emCmd, int nParam);
    int GetPtzSpeed();
    bool SnapShot(const QString& strPicName);
    SnapData::Ptr  SnapShot();
    void SetWndMode(PanelMode emMode);

    void StartSync(SnapModeParam::Ptr pParam);
    void StopSync();

    int GetPlayBackSize();
    void ShowFrame(SnapData::Ptr pFrame);
    void ClearPicture();

    void OnPBCtrl(int nFramIndex);
    void OnMouseMove(QPoint curPt);
    void OnMouseLeave();
    void OnMouseClicked();

signals:
    void FrameReady();
    void PBDataReady(int nDataSize);
    void CustomedSnap(SnapData::Ptr pSnap);

private:
    bool m_bInPreview = false;
    int m_nIndex = 0;
    bool m_bSelected = false;
    int m_nBorderWidth = 2;
    std::mutex m_mxLockData;
    std::queue<FrameData::Ptr> m_queWaitRender;
    std::vector<FrameData::Ptr> m_vcSyncRender;
    QString m_strHint;
    FrameData::Ptr m_pLastFrame = nullptr;
    FrameData::Ptr m_pCurFrame = nullptr; // 抓屏模式当前显示的帧
    std::atomic<PanelMode> m_nWndMode = { PanelMode::PreviewMode };

    bool m_bInPB = false;
    int m_nZoom = 1;
    int m_nPtzSpeed = 1;
    std::atomic<int> m_nFrameCome = 0;
    int m_nFrameRate = 15;
    double m_dbCoef = 1;
    QPoint m_viewPos = { 0,0 };
    QPoint m_mousePos = {0,0};
    SnapModeParam::Ptr m_pSnapModeParam = nullptr;
    SnapData::Ptr m_pSnapFrame = nullptr;
    QTimer* m_pFrameRateTimer = nullptr;
    QTimer* m_pFrameUpdateTimer = nullptr;
    std::mutex m_mxCurImageData;
protected:
    //virtual void paintEvent(QPaintEvent *event) override;
    void DrawDefault(QPainter& painter);
    void DrawText(QPainter& painter, QString& strText);
    void Drawborder(QPainter& painter, QRect& rcArea);
    FrameData::Ptr GetFrame();
//     virtual void initializeGL() override;
//     virtual void paintGL() override;
    void DrawImage(QPainter& painter, QImage& img);
    void DrawSnapRect(QPainter& painter);

    virtual void paintEvent(QPaintEvent *event) override;
    void DrawIntimeStream(QPainter& painter);
    void Draw4PictureMode(QPainter& painter);

    void StartFrameRateCheck();
    void StartFrameUpdate(int nFrameRate = DEFAULT_FRAME_RATE);
    void EndFrameRateCheck();
    void EndFrameUpdate();
    void OnTimeOut();
};

