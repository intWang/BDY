#pragma once
#include <QWidget>
#include "QtDefine.h"
#include "IServer.h"
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

class HintBar :public QWidget
{
    Q_OBJECT
public:

    using Ptr = HintBar * ;
    explicit HintBar(QWidget *parent = 0);
    ~HintBar();

    void SetHint(const QString& strText, ls::HintLevel level);

    void OnUserHint(const std::string& strHintInfo, ls::HintLevel level);
protected:
    ls::IHintCallBack::CallBackFunc::Ptr m_pCBFunc = nullptr;

    QLabelPtr m_pLabel = nullptr;

protected:

};