#pragma once

#include <QWidget>
#include "ui_ConfigChangePwd.h"
#include "DataStruct.h"
class ConfigChangePwd : public QWidget
{
    Q_OBJECT

public:
    using Ptr = ConfigChangePwd * ;
    ConfigChangePwd(DevNode::Ptr pDevNode, QWidget *parent = Q_NULLPTR);
    ~ConfigChangePwd();

public slots:
    void OnConfirm();
private:
    Ui::ConfigChangePwd ui;
    DevNode::Ptr m_pDevNode = nullptr;
};
