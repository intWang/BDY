#pragma once

#include <QWidget>
#include "ui_PTZCtrl.h"
#include "QtDefine.h"
class PTZCtrl : public QWidget
{
    Q_OBJECT

public:
    using Ptr = PTZCtrl * ;
    PTZCtrl(QWidget *parent = Q_NULLPTR);
    ~PTZCtrl();
    void EnableCtrl(bool bEnable);

public slots:
    void OnBtnClick();

signals:
    void DirBtnClick(PtzCommand emCtrl);

private:
    Ui::PTZCtrl ui;
    std::vector<QPushButtonPtr> m_vcBtns;

    QPixmap m_pixMask;
    QPixmap m_pixDirMask;
protected:
    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void paintEvent(QPaintEvent *event) override;

};
