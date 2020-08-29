#include "ConfigWifiList.h"
#include "ConfigWifiWidget.h"
ConfigWifiList::ConfigWifiList(QWidget *parent)
    : QListWidget(parent)
{
    connect(this, &ConfigWifiList::DataReady, this, [this]() {
        this->RefreshList();
    }, Qt::QueuedConnection);
}

ConfigWifiList::~ConfigWifiList()
{
}

void ConfigWifiList::SetWifiData(const IPCNetWifiAplist::Ptr& stData)
{
    m_pData = stData;
    emit DataReady();
}

void ConfigWifiList::RefreshList()
{
    if (m_pData)
    {
        ClearList();
        for (int i = 0; i< m_pData->num; i++)
        {
            if (auto pWidget = CreateItemWidget(*m_pData->ApItem[i], i))
            {
                QListWidgetItem *newItem = new QListWidgetItem();
                insertItem(count(), newItem);
                setItemWidget(newItem, pWidget);
                newItem->setSizeHint(QSize(0, 60));
                connect(pWidget, &ConfigWifiWidget::OnBtnClick, this, [this](int nIndex) {
                    emit this->LinkClicked(nIndex);
                });
            }
        }
    }
}

void ConfigWifiList::SetConnectedWif(const IPCNetWirelessConfig_st::Ptr& stData)
{
    m_pConnectedWifi = stData;
}

void ConfigWifiList::ClearList()
{
    int nCount = count();
    for (int index = 0; index < nCount; index++)
    {
        QListWidgetItem *item = takeItem(0);
        delete item;
    }
    clear();
}

ConfigWifiWidget* ConfigWifiList::CreateItemWidget(const IPCNetWifiApItem_st& stData, int nIndex)
{
    if (auto pRet = new ConfigWifiWidget(this))
    {
        pRet->SetItemData(stData, nIndex);
        return pRet;
    }
    return nullptr;
}
