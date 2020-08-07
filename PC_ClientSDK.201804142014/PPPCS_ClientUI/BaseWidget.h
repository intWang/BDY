#pragma once

#include <QWidget>
#include "ui_BaseWidget.h"
#include <QPoint>
#include <QMouseEvent>
class BaseWidget : public QWidget
{
    Q_OBJECT

public:
    BaseWidget(QWidget *parent = Q_NULLPTR);
    ~BaseWidget();

    void SetArea(int heightTop, int heightBottom);
    void SetAreaBk(const QColor& clrTop, const QColor& clrMid, const QColor& clrBottom);

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::BaseWidget ui;
    bool m_bPressed;
    QPoint m_point;
    int m_heightTop = 0;
    int m_heightBottom = 0;
    QColor m_clrTop = QColor(41, 44, 57);
    QColor m_clrBottom = QColor(41, 44, 57);
    QColor m_clrMid = QColor(30,34,51);
};
