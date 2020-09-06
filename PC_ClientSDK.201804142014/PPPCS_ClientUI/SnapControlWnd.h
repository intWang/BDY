#pragma once

#include <QWidget>
#include "AreableWidget.h"
#include "DataStruct.h"
class PreviewRealWnd;
class SnapCtrlWidget;
class SnapControlWnd : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    SnapControlWnd(QWidget *parent = Q_NULLPTR);
    ~SnapControlWnd();

signals:
    void StreamSnapShot(SnapData::Ptr pFrame);
    void PreviewStatuChanged(const QString& strUid, bool bStatu);
    void CustomWndLayout(int nRow, int nColumn);
    void LayoutDirChanged(bool bChanged);

public slots:

    void OnEnterSnapMode();
    void OnLeaveSnapMode();

    void OnPreviewStopped(const QString& strUid);
    void OnPreviewStarted(const QString& strUid);

    void OnStartPreview(DevNode::Ptr pChannel);
    void OnStopPreview();
    void OnStartSync();
    void OnStopSync();
    void OnSnapShot();
    void OnStartPrePlay();
    void OnStartPostPlay();
    void OnPauseResume(bool bPause);
    void OnSelRectSizeChanged(int width, int height);
    void OnLayoutChanged(int nRow, int nColumn);
    void OnLayoutDirChanged(bool bRow);
    void OnPopFullPic(bool bPop);
    void OnCustomedSnap(SnapData::Ptr pSnap);

protected:
    void StopSync();
    void StartSync();

private:
    bool m_bEnterSnapMode = false;

    DevNode::Ptr m_pChannel = nullptr;
    PreviewRealWnd* m_pPreviewRealWnd = nullptr;
    SnapCtrlWidget* m_pSnapCtrlWidget = nullptr;
    
};
