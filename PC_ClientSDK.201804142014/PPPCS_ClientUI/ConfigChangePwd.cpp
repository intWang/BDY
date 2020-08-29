#include "ConfigChangePwd.h"
#include "MessageBoxWnd.h"
ConfigChangePwd::ConfigChangePwd(DevNode::Ptr pDevNode, QWidget *parent)
    : QWidget(parent)
    , m_pDevNode(pDevNode)
{
    ui.setupUi(this);
    ui.edtOldPwd->setPlaceholderText("请输入密码");
    ui.edtNewPwd->setPlaceholderText("请输入密码");

    connect(ui.btnConfirm, &QPushButton::clicked, this, &ConfigChangePwd::OnConfirm);
}

ConfigChangePwd::~ConfigChangePwd()
{
}

void ConfigChangePwd::OnConfirm()
{
    auto strOldPwd = ui.edtOldPwd->text().toStdString();
    auto strNewPwd = ui.edtNewPwd->text().toStdString();

    if (!strOldPwd.size() || !strNewPwd.size())
    {
        msg::showInformation(this, "提示", "密码不能为空！");
        return;
    }

    if (m_pDevNode)
    {
        if (m_pDevNode->CheckPwd(strOldPwd))
        {
            m_pDevNode->ChangePwd(strNewPwd);
        }
        else
        {
            msg::showError(this, "错误", "密码输入错误，操作无效！");
        }
    }
}
