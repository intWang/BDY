#pragma once
#include "CmdBar.h"
#include "QtDefine.h"
#include "AreableWidget.h"
#include <QDialog>
class BaseDialog :public AreableWidget<QDialog>
{
    Q_OBJECT

    using QPixmapPtr = QPixmap * ;
public:
    BaseDialog(QWidget *parent = Q_NULLPTR);
    ~BaseDialog();
    QVBoxLayoutPtr GetLayout();
    HintBar::Ptr GetBottomBar();
    void SetNoBottomBar();
    void SetBorder(int nBorderWidth);
    void FullScreen();
public slots:
    virtual void OnCloseCmd();
private:
    CmdBar::Ptr m_pTitleBar = nullptr;
    HintBar::Ptr m_pBottomBar = nullptr;
    QVBoxLayoutPtr m_pMainLayout = nullptr;
protected:
    void setMinimizeVisible(bool bVisiable);
    void setMaximizeVisible(bool bVisiable);
    void setWidgetResizable(bool bVisiable);

    virtual void keyPressEvent(QKeyEvent *) override;


};

