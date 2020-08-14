#pragma once

#include "AreableWidget.h"

enum ConfigType
{
    AddDevice,
    AddGroup,
};

struct ConfigData
{
    using Ptr = std::shared_ptr<ConfigData>;
    ConfigType emType;
    virtual ~ConfigData()
    {
    }
};

struct AddDeviceData :public ConfigData
{
    using Ptr = std::shared_ptr<AddDeviceData>;
    std::string strUID;
    std::string strPwd;
    AddDeviceData()
        :ConfigData()
    {
        emType = ConfigType::AddDevice;
    }
    virtual ~AddDeviceData()
    {
    }
};

struct AddGroupData:public ConfigData
{
    using Ptr = std::shared_ptr<AddGroupData>;
    std::string strGroupName;
    AddGroupData()
        : ConfigData()
    {
        emType = ConfigType::AddGroup;
    }

    virtual ~AddGroupData()
    {
    }
};


class ConfigWidget:public AreableWidget<QWidget>
{
    Q_OBJECT
public:
    ConfigWidget(ConfigType type, QWidget* parent = nullptr);
    ~ConfigWidget();
    ConfigData::Ptr GetConfigData() {return m_pCurData;}

signals:
    void OnDataConfiged(ConfigData::Ptr pData);
private:
    ConfigData::Ptr m_pCurData = nullptr;

    QLineEditPtr m_pInputUID = nullptr;
    QLineEditPtr m_pInputPwd = nullptr;
    QLineEditPtr m_pInputGroupName = nullptr;
protected:
    virtual BarWidget::Ptr InitTopBar() override;

    void CreateWidget4Device(QFormLayoutPtr pLayout);
    void CreateWidget4Group(QFormLayoutPtr pLayout);
    void CreateReturnBtn(QHBoxLayoutPtr pLayout);

    void CollectData4Device();
    void CollectData4Group();

    void OnOk();
    void OnCancel();
};

