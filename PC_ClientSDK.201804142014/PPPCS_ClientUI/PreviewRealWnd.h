#pragma once

#include "ui_PreviewRealWnd.h"
#include "AreableWidget.h"
#include "DataStruct.h"
#include "DrawWnd.h"
#include "IServer.h"
class PreviewRealWnd : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    enum Status
    {
        Empty,
        StartingPreview,
        InPreview,
        StartingRecord,
        Record,

    };
    using Ptr = PreviewRealWnd * ;
    PreviewRealWnd(int nIndex, QWidget *parent = Q_NULLPTR);
    ~PreviewRealWnd();

    void OnFrameData(const std::string& strUid, FrameData::Ptr pFrame);

    void StartPreview(ChannelNode::Ptr pChannel);
    void StopPreview();
    void Clear();
    void SetSelectStatu(bool bSelect);
    bool GetSelectStatu();
    void SetRuningStatu(Status state);
    Status GetRuningStatu();
    time_t GetBusyTime();
    bool IsFull();
    void SetFull(bool bValue);
    int GetPtzSpeed();

public slots:
    void OnPtzCtrl(PtzCommand emCmd, int nParam);

signals:
    void PreviewWndUserClick();
    void PreviewWndUserDBClick(bool bFull);

private:
    //Ui::PreviewRealWnd ui;
    Status m_curStatus = Status::Empty;
    bool m_bSelected = false;
    DrawWnd::Ptr m_DrawWnd = nullptr;
    ChannelNode::Ptr m_pChannel = nullptr;
    ls::IIPCNetServerCallBack::CallBackFunc::Ptr m_CallBackFunc = nullptr;
    time_t m_tmBusy = 0;
    bool m_bFull = false;


protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual BarWidget::Ptr InitBottomBar() override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

};
