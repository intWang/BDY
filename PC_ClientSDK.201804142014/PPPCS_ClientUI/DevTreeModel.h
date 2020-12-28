#pragma once

#include <QStandardItemModel>

class DevTreeModel : public QStandardItemModel
{
    Q_OBJECT

public:
    DevTreeModel(QObject *parent);
    ~DevTreeModel();

};
