#pragma once
#include "QtDefine.h"
#include "AreableWidget.h"
class MainViewWnd: public AreableWidget<QWidget>
{
    struct PreivewData
    {
        bool bBusy = false;
        PreviewWndPtr pWnd = nullptr;
    };

    using PreivewDataPtr = std::shared_ptr<PreivewData>;
public:
    using Ptr = MainViewWnd * ;
    MainViewWnd(QWidget *parent = Q_NULLPTR);
    ~MainViewWnd();

    void SetStyle(DevideScreen devideType);
protected:
    void InitPreviewWnds();
    void ClearPreviewWnds();

    QGridLayoutPtr GetLayout();
    void OrderWnds(DevideScreen devideType);


protected:
    std::vector<PreivewDataPtr> m_vcPreviewwnds;
    DevideScreen m_currentStyle = Screen_1X1;
};

