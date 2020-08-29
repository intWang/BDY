#pragma once

#include <QWidget>
#include "ui_ConfigWifiWidget.h"
#include "JSONStructProtocal.h"

class ConfigWifiWidget : public QWidget
{
    Q_OBJECT

public:
    ConfigWifiWidget(QWidget *parent = Q_NULLPTR);
    ~ConfigWifiWidget();

signals:
    void OnBtnClick(int nIndex);

public:
    void SetItemData(const IPCNetWifiApItem_st& stData, int index);
private:
    Ui::ConfigWifiWidget ui;
    int nIndex = 0;
};
