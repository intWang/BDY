#pragma once
//DO NOT USE
#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include "QtDefine.h"
#include "MovelabelWidget.h"
class BaseWidget : public MovelabelWidget<QWidget>
{
    Q_OBJECT

public:
    BaseWidget(QWidget *parent = Q_NULLPTR);
    ~BaseWidget();

    void SetArea(int heightTop, int heightBottom);
    void SetAreaBk(const QColor& clrTop, const QColor& clrMid, const QColor& clrBottom);

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    int m_heightTop = 0;
    int m_heightBottom = 0;
    QColor m_clrTop = s_qcl292C39;
    QColor m_clrBottom = s_qcl292C39;
    QColor m_clrMid = s_qcl1E2233;
};
