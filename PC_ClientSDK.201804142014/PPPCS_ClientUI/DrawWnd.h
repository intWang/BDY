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

signals:
    void FrameReady(FrameData::Ptr pFrame);

private:
    bool m_bInPreview = false;
    int m_nIndex = 0;
    bool m_bSelected = false;
    int m_nBorderWidth = 2;
    YUV420P_Render::Ptr m_pReander = nullptr;
    std::mutex m_mxLockData;
    std::queue<FrameData::Ptr> m_queWaitRender;
protected:
    //virtual void paintEvent(QPaintEvent *event) override;
    void DrawDefault();
    QRect GetDrawArea();
    FrameData::Ptr GetFrame();
//     virtual void initializeGL() override;
//     virtual void paintGL() override;

    virtual void paintEvent(QPaintEvent *event) override;

};

