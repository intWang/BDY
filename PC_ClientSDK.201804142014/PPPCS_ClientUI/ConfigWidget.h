#pragma once

#include "AreableWidget.h"
#include "MovelabelWidget.h"
#include "IHttpSupport.h"
#include "BaseDialog.h"
enum ConfigType
{
    AddDevice,
    AddGroup,
    ModifyDevice,
    ModifyGroup,
    StorageConfig,
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
    std::string strShortUID;
    std::string strPwd;
    std::string strDevName;
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

struct StorageData :public ConfigData
{
    using Ptr = std::shared_ptr<StorageData>;
    std::string strSnapShotPath;
    std::string strRecordPath;
    StorageData()
        : ConfigData()
    {
        emType = ConfigType::StorageConfig;
    }

    virtual ~StorageData()
    {
    }
};


struct ModDeviceData :public AddDeviceData
{
    enum MOD
    {
        MOD_UID =  0x00000001,
        MOD_PWD =  0x00000010,
        MOD_NAME = 0x00000100,
    };

    using Ptr = std::shared_ptr<ModDeviceData>;
    int nModLevel = MOD_UID| MOD_PWD| MOD_NAME;
    ModDeviceData(DevNode::Ptr pData)
        :AddDeviceData()
    {
        emType = ConfigType::ModifyDevice;
        if (pData)
        {
            strUID = pData->strUID;
            strShortUID = pData->strShortID;
            strPwd = pData->strPwd;
            strDevName = pData->strCustomName;
            if (pData->IsDevLoaded())
            {
                nModLevel = MOD_NAME;
            }
            else
            {
                nModLevel = MOD_UID | MOD_PWD | MOD_NAME;
            }
        }
    }
    virtual ~ModDeviceData()
    {
    }
};

struct ModGroupData :public AddGroupData
{
    using Ptr = std::shared_ptr<ModGroupData>;
    ModGroupData(GroupNode::Ptr pData)
        : AddGroupData()
    {
        emType = ConfigType::ModifyGroup;
        if (pData)
        {
            strGroupName = pData->strGroupName;
        }
    }

    virtual ~ModGroupData()
    {
    }
};


class HttpHelper;
class QNetworkReply;

class ConfigWidget:public BaseDialog
{
    Q_OBJECT
public:
    ConfigWidget(ConfigType type, QWidget* parent = nullptr, TreeNode::Ptr pNode = nullptr);
    ~ConfigWidget();
    ConfigData::Ptr GetConfigData() {return m_pCurData;}

signals:
    void OnDataConfiged(ConfigData::Ptr pData);
    void OnDataModified(ConfigData::Ptr pData);
private:
    ConfigData::Ptr m_pCurData = nullptr;

    QLineEditPtr m_pInputUID = nullptr;
    QLineEditPtr m_pInputName = nullptr;
    QLineEditPtr m_pInputPwd = nullptr;
    QLineEditPtr m_pInputGroupName = nullptr;

    QPushButtonPtr m_pbtnOK = nullptr;
    QPushButtonPtr m_pbtnCancel = nullptr;
    bool m_bRequesting = false;
protected:

    void CreateWidget4Device(QFormLayoutPtr pLayout, bool bModify = false);
    void CreateWidget4Group(QFormLayoutPtr pLayout, bool bModify = false);
    void CreateWidget4Storage(QFormLayoutPtr pLayout);
    void CreateReturnBtn(QHBoxLayoutPtr pLayout);

    void CollectData4Device();
    void CollectData4Group();

    void OnOk();
    void OnCancel();

    QString ChooseFilePath(QString& strDefultPath);
    void RequestLongCode(std::string& strShortCode);


    virtual void keyPressEvent(QKeyEvent *) override;

protected slots:
    void OnHttpReplyFinished(QNetworkReply* replay);
    void OnHttpTimeour();
protected:
    HttpHelper* m_pHttpHelper = nullptr;
};

