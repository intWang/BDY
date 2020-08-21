#pragma once

#include <QOpenGLWidget>
#include "DataStruct.h"
#include "YUV420P_Render.h"
#include <queue>
#include <mutex>
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
    void SetFrame(FrameData::Ptr pFrame);
    void SetHintString(const QString& strHint);
    void OnPtzCtrl(PtzCommand emCmd, int nParam);
    int GetPtzSpeed();

signals:
    void FrameReady(FrameData::Ptr pFrame);

private:
    bool m_bInPreview = false;
    int m_nIndex = 0;
    bool m_bSelected = false;
    int m_nBorderWidth = 2;
    std::mutex m_mxLockData;
    std::queue<FrameData::Ptr> m_queWaitRender;
    QString m_strHint;
    FrameData::Ptr m_pLastFrame = nullptr;

    int m_nZoom = 1;
    int m_nPtzSpeed = 1;
    double m_dbCoef = 1;
    QPoint m_viewPos = { 0,0 };

protected:
    //virtual void paintEvent(QPaintEvent *event) override;
    void DrawDefault();
    void DrawText(QPainter& painter, QString& strText);
    void Drawborder(QPainter& painter, QRect& rcArea);
    FrameData::Ptr GetFrame();
//     virtual void initializeGL() override;
//     virtual void paintGL() override;

    virtual void paintEvent(QPaintEvent *event) override;

};

