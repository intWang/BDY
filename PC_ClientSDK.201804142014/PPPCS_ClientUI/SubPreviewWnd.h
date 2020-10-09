#pragma once

#include <QWidget>
#include "AreableWidget.h"
#include "DataStruct.h"
#include "PreviewRealWnd.h"
class SubPreviewWnd : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    SubPreviewWnd(QWidget *parent = Q_NULLPTR);
    ~SubPreviewWnd();

    void StopPreview();

public slots:
    void OnStartPreview(DevNode::Ptr pChannel);
    void OnEnterSnapMode();
    void OnLeaveSnapMode();
    void OnPreviewStarted(const QString& strUid, PreviewRealWnd::Ptr pWnd);
    void OnPreviewStoped(const QString& strUid, PreviewRealWnd::Ptr pWnd);
    void OnBtnClicked();
signals:
    void PreviewStatuChanged(const QString& strUid, bool bStatu);
    void SelectPreviewWnd(PreviewRealWnd::Ptr pWnd);

private:
    virtual BarWidget::Ptr InitBottomBar();
    virtual BarWidget::Ptr InitTopBar();


protected:
    PreviewRealWnd* m_pPreviewWnd = nullptr;
    bool m_bEnterSnapMode = false;

    QPushButtonPtr m_pBtnZoomIn = nullptr;
    QPushButtonPtr m_pBtnZoomOut = nullptr;
    QPushButtonPtr m_pBtnLeft = nullptr;
    QPushButtonPtr m_pBtnRight = nullptr;
    QPushButtonPtr m_pBtnUp = nullptr;
    QPushButtonPtr m_pBtnDown = nullptr;
};
