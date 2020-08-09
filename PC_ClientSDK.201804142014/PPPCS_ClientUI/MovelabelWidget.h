#pragma once

template<typename T>
class MovelabelWidget:public T
{
public:
    MovelabelWidget(QWidget *parent = Q_NULLPTR);
    ~MovelabelWidget();

    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

protected:
    bool m_bPressed;
    QPoint m_point;
};

template<typename T>
MovelabelWidget<T>::~MovelabelWidget()
{

}

template<typename T>
MovelabelWidget<T>::MovelabelWidget(QWidget *parent)
    :T(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}

template<typename T>
void MovelabelWidget<T>::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_bPressed = true;
        m_point = event->pos();
    }
}

template<typename T>
void MovelabelWidget<T>::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bPressed)
        move(event->pos() - m_point + pos());
}

template<typename T>
void MovelabelWidget<T>::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bPressed = false;
}

