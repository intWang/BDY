#pragma once
#include <qopenglwidget.h>
class OpenGLDrawWnd :
    public QOpenGLWidget
{
public:
    using Ptr = OpenGLDrawWnd * ;
    OpenGLDrawWnd(QWidget* parent = nullptr);
    ~OpenGLDrawWnd();
};

