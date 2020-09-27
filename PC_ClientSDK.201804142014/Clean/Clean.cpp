#include "Clean.h"
#include <QMessageBox>
#include <QPushButton>
#include <QDir>
Clean::Clean(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowIcon(QIcon(":/Clean/res/logo_b.ico"));
    setWindowTitle("卸载");

    resize(300, 200);

    connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
    translateUI();
}

void Clean::translateUI()
{
    auto pButtonBox = ui.buttonBox;
    QPushButton *pYesButton = pButtonBox->button(QDialogButtonBox::Yes);
    if (pYesButton != NULL)
        pYesButton->setText(tr("是"));

    QPushButton *pNoButton = pButtonBox->button(QDialogButtonBox::No);
    if (pNoButton != NULL)
        pNoButton->setText(tr("否"));

    QPushButton *pOkButton = pButtonBox->button(QDialogButtonBox::Ok);
    if (pOkButton != NULL)
        pOkButton->setText(tr("确定"));

    QPushButton *pCancelButton = pButtonBox->button(QDialogButtonBox::Cancel);
    if (pCancelButton != NULL)
        pCancelButton->setText(tr("取消"));

    QPushButton *pOpenButton = pButtonBox->button(QDialogButtonBox::Open);
    if (pOpenButton != NULL)
        pOpenButton->setText(tr("打开"));
}

bool Clean::deleteFolder(QString & strPath)
{
    if (strPath.isEmpty() || !QDir().exists(strPath))
        return true;

    QFileInfo FileInfo(strPath);

    if (FileInfo.isFile())
        return QFile::remove(strPath);
    else if (FileInfo.isDir())
    {
        QDir qDir(strPath);
        return qDir.removeRecursively();
    }
    return false;
}

void Clean::onButtonClicked(QAbstractButton *button)
{
    auto ret = ui.buttonBox->standardButton(button);
    if(ret == QDialogButtonBox::No)
    {
        /// do delete 
        QString strTargetPath = QDir::tempPath() + "/PPCSClient/";
        if (deleteFolder(strTargetPath))
        {
            QMessageBox::information(this, "提示", "清理完成！");
        }
        else
        {
            QMessageBox::warning(this, "错误", "清理失败！请手动删除 %tmp%/PPCSClient！");
        }
    }
    close();
}
