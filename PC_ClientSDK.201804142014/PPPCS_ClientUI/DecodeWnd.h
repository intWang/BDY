#pragma once

#include <QWidget>
#include "ui_DecodeWnd.h"
#include "DataStruct.h"
#include <mutex>
class DecodeWnd : public QWidget
{
    Q_OBJECT

public:
    using Ptr = DecodeWnd * ;
    DecodeWnd(QWidget *parent = Q_NULLPTR);
    ~DecodeWnd();

    void BindDevNode(DevNode::Ptr pDevNode);
    void Untie();

    void OnPicOverTurnData(const std::string& strUid, const IPCNetPicOverTurn::Ptr& pData);
    void OnVideoEncodeData(const std::string& strUid, const IPCNetStreamInfo::Ptr& pData);

public slots:
    void OnClicked(bool bChecked);

private:
    Ui::DecodeWnd ui;
    DevNode::Ptr m_pDevNode = nullptr;
    std::mutex m_mxLockDevNode;
    time_t m_tmLastOperate = 0;
};
