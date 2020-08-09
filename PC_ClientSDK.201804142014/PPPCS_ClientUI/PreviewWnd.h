#pragma once

#include <QOpenGLWidget>
#include <QOpenGLFunctions>

class PreviewWnd : public QOpenGLWidget
    , protected QOpenGLFunctions
{
public:
    PreviewWnd(QWidget* parent = nullptr);
    ~PreviewWnd();
};

