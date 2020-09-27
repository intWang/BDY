#pragma once

#include <QtWidgets/QMainWindow>
#include <QString>
#include "ui_Clean.h"

class Clean : public QMainWindow
{
    Q_OBJECT

public:
    Clean(QWidget *parent = Q_NULLPTR);

protected:
    void translateUI();
    bool deleteFolder(QString & strFilePath);

private slots:
    void onButtonClicked(QAbstractButton *button);

private:
    Ui::CleanClass ui;
};
