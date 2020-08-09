#include "PreviewWnd.h"

#include "QtDefine.h"

PreviewWnd::PreviewWnd(QWidget* parent)
    :QOpenGLWidget(parent)
{
    auto button = MQ(QPushButton)(this);
}


PreviewWnd::~PreviewWnd()
{
}
