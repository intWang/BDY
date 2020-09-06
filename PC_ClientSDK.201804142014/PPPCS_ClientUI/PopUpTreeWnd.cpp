#include "PopUpTreeWnd.h"
#include "DevTreeWnd.h"

PopUpTreeWnd::PopUpTreeWnd(QWidget* pParent)
    :BaseDialog(pParent)
{
    resize(398, 586);
    setWindowTitle(QStringLiteral("选择设备"));
    setMinimizeVisible(false);
    setMaximizeVisible(false);
    setWidgetResizable(false);
    SetArea(30, 0);
    SetBorder(4);
    SetAreaBk(s_qcl444858, s_qcl1E2233, s_qcl444858);
    SetNoBottomBar();
}


PopUpTreeWnd::~PopUpTreeWnd()
{
}

void PopUpTreeWnd::SetTreeView(DevTreeWnd* pTree)
{
    auto pMainLayout = GetLayout();
    pMainLayout->addWidget(pTree);
    m_pTree = pTree;
}

DevTreeWnd* PopUpTreeWnd::GetTreeView()
{
    return m_pTree;
}
