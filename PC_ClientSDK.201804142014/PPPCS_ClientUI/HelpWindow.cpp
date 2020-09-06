#include "HelpWindow.h"

HelpWindow::HelpWindow(QWidget *parent)
    : BaseDialog(parent)
{
    ui.setupUi(this);

    setWindowTitle("帮助");
    resize(400, 350);
    setMinimizeVisible(false);
    setMaximizeVisible(false);
    setWidgetResizable(false);
    SetAreaBk(s_qcl444858, s_qcl1E2233, s_qcl444858);
    SetArea(30, 0);
    SetBorder(4);
    SetNoBottomBar();

    auto pMainLayout = GetLayout();
    pMainLayout->addWidget(ui.label);
    auto pHLayout = MQ(QHBoxLayout)(this);
    auto pCloseBtn = MQ(QPushButton)(this);

    pHLayout->addStretch();
    pHLayout->addWidget(pCloseBtn);
    pHLayout->setContentsMargins(0,30,20,30);
    pMainLayout->addLayout(pHLayout);
    pMainLayout->setContentsMargins(20,30,20,30);
    pCloseBtn->setText("确定");
    connect(pCloseBtn, &QPushButton::clicked, this, [this]() {
        this->done(0);
    });
}

HelpWindow::~HelpWindow()
{
}
