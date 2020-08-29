#include "ConfigWifiConfirm.h"
#include "MessageBoxWnd.h"
ConfigWifiConfirm::ConfigWifiConfirm( QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    ui.lineEdit->setPlaceholderText("请输入密码");
    connect(ui.btnConfirm, &QPushButton::clicked, this, [this]() {
        QString strPwd = ui.lineEdit->text();
        if (strPwd.isEmpty())
        {
            msg::showWarning(this, "警告", "请输入密码！");
            return;
        }
        emit this->ConfirmClicked(strPwd);
    });

    connect(ui.btnCancell, &QPushButton::clicked, this, [this]() {
        emit this->CancellClicked();
    });

    connect(ui.checkBox, &QCheckBox::clicked, this, [this](bool checked) {
        this->ui.lineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });
}

ConfigWifiConfirm::~ConfigWifiConfirm()
{
}
