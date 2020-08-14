#include "PreviewRealWnd.h"
#include "OpenGLDrawWnd.h"
PreviewRealWnd::PreviewRealWnd(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);

    SetArea(0, 30);
    SetAreaBk(s_qcl292C39, s_qcl292C39, s_qcl292C39);

    auto pDrawWnd = MQ(OpenGLDrawWnd)(this);
    if (pDrawWnd)
    {
        ui.mainLayout->addWidget(pDrawWnd);
    }
    auto pBottomBar = GetBottomWnd();
    if (pBottomBar)
    {
        ui.mainLayout->addWidget(pBottomBar);
    }
}

PreviewRealWnd::~PreviewRealWnd()
{
}

void PreviewRealWnd::StartPreview()
{

}

void PreviewRealWnd::BindDevice(const DeviceData& deviceData)
{
    m_deviceData = deviceData;
}

void PreviewRealWnd::Clear()
{

}
