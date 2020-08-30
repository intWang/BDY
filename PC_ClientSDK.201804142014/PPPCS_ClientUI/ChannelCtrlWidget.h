#pragma once

#include "AreableWidget.h"
#include <QStackedWidget>
#include <QTableWidget>
#include "PtzCtrlWnd.h"
#include "ParamWnd.h"
#include "DecodeWnd.h"
class ChannelCtrlWidget : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    enum TabWnd
    {
        Ptz,
        Param,
        Decode,
    };
    ChannelCtrlWidget(QWidget *parent = Q_NULLPTR);
    ~ChannelCtrlWidget();

public:
    PtzCtrlWnd::Ptr GetPtzCtrlWnd();

    void OnVideoParamData(const std::string& strUid, const IPCNetCamColorCfg_st& stParam);
    void OnPicOverTurnData(const std::string& strUid, const IPCNetPicOverTurn::Ptr& pData);
    void OnVideoEncodeData(const std::string& strUid, const IPCNetStreamInfo::Ptr& pData);

public slots:
    void BindPreviewWnd(PreviewRealWnd::Ptr pWnd);

signals:
    void SelTab(TabWnd emTab);

protected:
    virtual BarWidget::Ptr InitTopBar() override;

    void UpdateParamWnd();
    void UpdatePtzWnd();
    void UpdateDecodeWnd();

private:
    int m_nTopHeight = 50;
    QStackedWidget* m_pStackWidget = nullptr;
    PtzCtrlWnd::Ptr m_pPtzWnd = nullptr;

    ParamWnd::Ptr m_pParamWnd = nullptr;
    PreviewRealWnd::Ptr m_pPreviewpWnd = nullptr;
    DecodeWnd::Ptr m_pDecodeWnd = nullptr;
    ls::IIPCNetServerCallBack::CallBackFunc::Ptr m_CallBackFunc = nullptr;
};
