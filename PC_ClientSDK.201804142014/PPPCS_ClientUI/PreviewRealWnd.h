#pragma once

#include "ui_PreviewRealWnd.h"
#include "AreableWidget.h"
#include "DataStruct.h"

class PreviewRealWnd : public AreableWidget<QWidget>
{
    Q_OBJECT

public:
    using Ptr = PreviewRealWnd * ;
    PreviewRealWnd(QWidget *parent = Q_NULLPTR);
    ~PreviewRealWnd();

    void StartPreview();
    void BindDevice(const DeviceData& deviceData);
    void Clear();

    enum Status
    {
        Empty,
        StartingPreview,
        InPreview,
        StartingRecord,
        Record,
    };
private:
    Ui::PreviewRealWnd ui;
    DeviceData m_deviceData;
    Status m_curStatus = Status::Empty;
};
