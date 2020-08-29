#pragma once

#include <QWidget>
#include "ui_ConfigBaseInfoWnd.h"
#include "DataStruct.h"
class ConfigBaseInfoWnd : public QWidget
{
    Q_OBJECT

public:
    using Ptr = ConfigBaseInfoWnd*;
    ConfigBaseInfoWnd(DevNode::Ptr pDevNode, QWidget *parent = Q_NULLPTR);
    ~ConfigBaseInfoWnd();

    void SetTime(const QString& strTime);
protected slots:
    void OnBtnClicked();
private:
    Ui::ConfigBaseInfoWnd ui;
    DevNode::Ptr m_pDevNode = nullptr;
};
