#pragma once
#include <QWidget>
#include "QtDefine.h"

class CmdBar:public QWidget
{
    Q_OBJECT
public:

    using Ptr = CmdBar * ;
    explicit CmdBar(QWidget *parent = 0);
    ~CmdBar();

private slots:
    void onClicked();

private:
    void updateMaximize();

private:
    QLabelPtr m_pIconLabel;
    QLabelPtr m_pTitleLabel;
    QPushButtonPtr m_pMinimizeButton;
    QPushButtonPtr m_pMaximizeButton;
    QPushButtonPtr m_pCloseButton;
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

};

class BottomBar :public QWidget
{
    Q_OBJECT
public:

    using Ptr = BottomBar * ;
    explicit BottomBar(QWidget *parent = 0);
    ~BottomBar();

protected:

    QLabelPtr m_pLabel = nullptr;

};