#include "MainViewWnd.h"



MainViewWnd::MainViewWnd(QWidget *parent)
    :BaseWidget(parent)
{
    QColor qcl(255,255,255);
    SetAreaBk(qcl, qcl, qcl);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    hide();
}


MainViewWnd::~MainViewWnd()
{
}
