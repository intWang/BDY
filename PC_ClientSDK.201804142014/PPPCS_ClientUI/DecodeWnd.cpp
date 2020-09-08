#include "DecodeWnd.h"
#include "MessageBoxWnd.h"

DecodeWnd::DecodeWnd(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setEnabled(false);
    //ui.chkMirror->hide();

    connect(ui.chkClarity, &QCheckBox::clicked, this, &DecodeWnd::OnClicked);
    connect(ui.chkFlip, &QCheckBox::clicked, this, &DecodeWnd::OnClicked);
    connect(ui.chkMirror, &QCheckBox::clicked, this, &DecodeWnd::OnClicked);

}

DecodeWnd::~DecodeWnd()
{
}

void DecodeWnd::BindDevNode(DevNode::Ptr pDevNode)
{
    std::lock_guard<std::mutex> guard(m_mxLockDevNode);
    m_pDevNode = pDevNode;
    if (m_pDevNode)
    {
        m_pDevNode->GetFlipMirror();
        m_pDevNode->GetClarity();
        setEnabled(true);
    }
    else
    {
        setEnabled(false);
    }
    update();
}

void DecodeWnd::Untie()
{
    std::lock_guard<std::mutex> guard(m_mxLockDevNode);
    m_pDevNode = nullptr;
    setEnabled(false);
    update();
}

void DecodeWnd::OnPicOverTurnData(const std::string& strUid, const IPCNetPicOverTurn::Ptr& pData)
{
    std::lock_guard<std::mutex> guard(m_mxLockDevNode);
    if (m_pDevNode && strUid == m_pDevNode->GetDevUid())
    {
        ui.chkFlip->setChecked(pData->Flip);
        ui.chkMirror->setChecked(pData->Mirror);
        update();
    }
}

void DecodeWnd::OnVideoEncodeData(const std::string& strUid, const IPCNetStreamInfo::Ptr& pData)
{
    std::lock_guard<std::mutex> guard(m_mxLockDevNode);
    if (pData && m_pDevNode && strUid == m_pDevNode->GetDevUid())
    {
        ui.chkClarity->setChecked(pData->EncCh);
        LogInfo("OnVideoEncodeData %d", pData->EncCh);
        update();
    }
}

void DecodeWnd::OnClicked(bool bChecked)
{
    if (!m_pDevNode)
    {
        return;
    }

    auto pChk = qobject_cast<QCheckBoxPtr>(sender());

    auto tmNow = time(NULL);
    int tmSpace = tmNow - m_tmLastOperate;
    if (tmSpace <= 5)
    {
        if (pChk)
        {
            pChk->setChecked(!bChecked);
        }

        QString strWarn = "您操作的太快了，请 %1 秒后再试";
        strWarn = strWarn.arg(5 - tmSpace);
        msg::showWarning(this, "警告", strWarn);
        
        return;
    }
    m_tmLastOperate = tmNow;

    if (pChk == ui.chkClarity)
    {
        m_pDevNode->SetClarity(pChk->isChecked());
    }
    else if (pChk == ui.chkFlip)
    {
        m_pDevNode->SetFlipMirror(1,0);
        //m_pDevNode->GetFlipMirror();
    }
    else if (pChk == ui.chkMirror)
    {
        m_pDevNode->SetFlipMirror(0, 1);
        //m_pDevNode->GetFlipMirror();
    }
   
}
