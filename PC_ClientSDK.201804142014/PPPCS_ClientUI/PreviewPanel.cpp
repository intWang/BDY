#include "PreviewPanel.h"
#include "QtDefine.h"
PreviewPanel::PreviewPanel(QWidget *parent)
    : AreableWidget<QWidget>(parent)
{
    ui.setupUi(this);
    SetArea(50,30);
    SetAreaBk(s_qcl444858, s_qcl292C39, s_qcl444858);
}

PreviewPanel::~PreviewPanel()
{
}
