#include "DrawWnd.h"
#include <QPainter>
#include "QtDefine.h"
#include "utils.h"
#include "ConfigCenter.h"
#include "MessageBoxWnd.h"
#include <QTime>
#include <QDesktopServices>
#include <QProcess>
#include "LogManager.h"
DrawWnd::DrawWnd(int index, QWidget *parent)
    :QWidget(parent)
    , m_nIndex(index)
{
    m_strHint = QStringLiteral("监控点正在预览....");

    m_pFrameRateTimer = new QTimer(this);
    m_pFrameUpdateTimer = new QTimer(this);

    connect(m_pFrameRateTimer, &QTimer::timeout, this, &DrawWnd::OnTimeOut);
    connect(m_pFrameUpdateTimer, &QTimer::timeout, this, &DrawWnd::OnTimeOut);

}


DrawWnd::~DrawWnd()
{
    EndFrameRateCheck();
    EndFrameUpdate();
}

void DrawWnd::SetPreviewStatu(bool bPreview /*= false*/)
{
    m_bInPreview = bPreview;
    if (bPreview)
    {
        StartFrameRateCheck();
        StartFrameUpdate();
    }
    else 
    {
        EndFrameRateCheck();
        EndFrameUpdate();
        m_queWaitRender.swap(std::queue<FrameData::Ptr>());
    }
    m_pLastFrame = nullptr;
    update();
}

void DrawWnd::SetSelected(bool bSelected)
{
    m_bSelected = bSelected;
    update();
}

void DrawWnd::InputFrameData(FrameData::Ptr pFrame)
{
    if (isVisible())
    {
       // LogDebug("Frame come %d", pFrame);
        AddFrame(pFrame);
    }
}

void DrawWnd::AddFrame(FrameData::Ptr pFrame)
{
    {
        std::lock_guard<std::mutex> guard(m_mxLockData);
        m_queWaitRender.push(pFrame);
        if (m_nWndMode.load() == PanelMode::SnapMode)
        {
            static DWORD tmLast = 0;
            DWORD tmNow = GetTickCount();
            //0.3s 取一帧
            if (tmNow - tmLast > 300)
            {
                m_vcSyncRender.push_back(pFrame);
                tmLast = tmNow;
            }
        }
        m_nFrameCome++;
    }
}

void DrawWnd::SetHintString(const QString& strHint)
{
    m_pLastFrame = nullptr;
    m_strHint = strHint;
}

void DrawWnd::OnPtzCtrl(PtzCommand emCmd, int nParam)
{
    int nMoveX = 0;
    int nMoveY = 0;
    int nSpeed = 10;

    switch (emCmd)
    {
    case PtzCommand::ZoomIn:
        ++m_nZoom;
        m_nZoom = min(10, m_nZoom);
        break;
    case PtzCommand::ZoomOut:
        --m_nZoom;
        m_nZoom = max(1, m_nZoom);
        break;
    case PtzCommand::MoveLT:
        nMoveX = m_nPtzSpeed * nSpeed * -1;
        nMoveY = m_nPtzSpeed * nSpeed * -1;
        break;
    case PtzCommand::MoveL:
        nMoveX = m_nPtzSpeed * nSpeed * -1;
        break;
    case PtzCommand::MoveLD:
        nMoveX = m_nPtzSpeed * nSpeed * -1;
        nMoveY = m_nPtzSpeed * nSpeed;
        break;
    case PtzCommand::MoveD:
        nMoveY = m_nPtzSpeed * nSpeed;
        break;
    case PtzCommand::MoveRD:
        nMoveX = m_nPtzSpeed * nSpeed;
        nMoveY = m_nPtzSpeed * nSpeed;
        break;
    case PtzCommand::MoveR:
        nMoveX = m_nPtzSpeed * nSpeed;
        break;
    case PtzCommand::MoveRT:
        nMoveX = m_nPtzSpeed * nSpeed ;
        nMoveY = m_nPtzSpeed * nSpeed * -1;
        break;
    case PtzCommand::MoveT:
        nMoveY = m_nPtzSpeed * nSpeed * -1;
        break;
    case PtzCommand::Restore:
        m_nZoom = 1;
        nMoveX = m_viewOffset.x() * -1;
        nMoveY = m_viewOffset.y() * -1;
        break;
    case PtzCommand::SetSpeed:
        m_nPtzSpeed = nParam;
        break;
    default:
        break;
    }

    m_dbCoef = 1 - (m_nZoom - 1) * 0.1;
    m_viewOffset.setX(m_viewOffset.x() + nMoveX);
    m_viewOffset.setY(m_viewOffset.y() + nMoveY);
    
    update();
}

int DrawWnd::GetPtzSpeed()
{
    return m_nPtzSpeed;
}

bool DrawWnd::SnapShot(const QString& strPicName)
{
    if (auto pCurFrame = m_pLastFrame)
    {
        QImage tmpImg((uchar *)pCurFrame->pBufData, pCurFrame->nPicWidth, pCurFrame->nPicHeight, QImage::Format_RGB32);
        QString strPath = ConfigCenter::GetInstance().GetSnapShotSavepath();
        utils::MakePathExist(strPath);
        QString strFileName = strPath + strPicName + QDateTime::currentDateTime().toString("_yyMMdd_hh_mm_ss") + ".jpg";
        if (tmpImg.save(strFileName, "JPG", 100))
        {
            QString strInfo = "截图已保存至: " + strFileName;
            if (msg::showInformation(this, QStringLiteral("截图"), strInfo, QMessageBox::Ok | QMessageBox::Open) == QMessageBox::Open)
            {
                QProcess process;
                QString filePath = strFileName;
                filePath.replace("/", "\\"); // 只能识别 "\"
                QString cmd = QString("explorer.exe /select,\"%1\"").arg(filePath);
                process.startDetached(cmd);
            }
        }
        else
        {
            msg::showError(this, QStringLiteral("错误"), QStringLiteral("保存失败!"));

        }
    }
    else
    {
        msg::showWarning(this, QStringLiteral("警告"), QStringLiteral("当前无画面，请稍后再试!"));
    }
    return false;
}

SnapData::Ptr DrawWnd::SnapShot()
{
    switch (m_nWndMode.load())
    {
    case PanelMode::PreviewMode:
    case PanelMode::SnapMode:
    {
        if (m_pLastFrame)
        {
            return std::make_shared<SnapData>(m_pLastFrame);
        }
    }
        break;
    case PanelMode::PictureMode:
    {
        std::lock_guard<mutex> guard(m_mxCurImageData);
        if (m_pSnapFrame)
        {
            return m_pSnapFrame;
        }
    }
        break;
    default:
        break;
    }


    
    return nullptr;
}

void DrawWnd::SetWndMode(PanelMode emMode)
{
    m_nWndMode.store(emMode);
    m_pLastFrame = nullptr;
    m_pSnapFrame = nullptr;
    update();
}

void DrawWnd::StartSync(SnapModeParam::Ptr pParam)
{
    m_vcSyncRender.clear();
    m_pSnapModeParam = pParam;
    SetWndMode(PanelMode::SnapMode);
}

void DrawWnd::StopSync()
{
    SetWndMode(PanelMode::PreviewMode);
    //m_vcSyncRender.clear();
    //m_pSnapModeParam = nullptr;
    m_bInPB = false;
    if (auto nFrameSize = m_vcSyncRender.size())
    {
        emit PBDataReady(nFrameSize);
    }
}

int DrawWnd::GetPlayBackSize()
{
    return m_vcSyncRender.size();
}


void DrawWnd::ShowFrame(SnapData::Ptr pFrame)
{
    {
        std::lock_guard<mutex> guard(m_mxCurImageData);
        m_pSnapFrame = pFrame;
    }
    repaint();
}

void DrawWnd::ClearPicture()
{
    if (m_pSnapFrame)
    {
        m_pSnapFrame = nullptr;
        update();
    }
}

void DrawWnd::OnMouseMove(QPoint curPt)
{
    m_mousePos = curPt;
    update();
}

void DrawWnd::OnMouseLeave()
{
    m_mousePos = { 0,0 };
    update();
}

void DrawWnd::OnMouseClicked()
{
    FrameData::Ptr pFrame = nullptr;

    if (m_nWndMode.load() == PanelMode::SnapMode)
    {
        if (m_pLastFrame)
        {
            pFrame = m_pLastFrame;
        }
    }
    else if ((m_nWndMode.load() == PanelMode::PictureMode && m_bInPB))
    {
        if (m_pSnapFrame)
        {
            pFrame = m_pSnapFrame->pFrame;
        }
    }

    if (pFrame && !m_mousePos.isNull() && m_pSnapModeParam)
    {
        if (auto pSnapData = std::make_shared<SnapData>(pFrame))
        {
            auto rcWnd = rect();
            pSnapData->x = m_mousePos.x() - (m_pSnapModeParam->nSelRectWidth / 2);
            pSnapData->y = m_mousePos.y() - (m_pSnapModeParam->nSelRectHeight / 2);
            pSnapData->nWidth = m_pSnapModeParam->nSelRectWidth;
            pSnapData->nHeight = m_pSnapModeParam->nSelRectHeight;

            if (pSnapData->x > 0)
            {
                pSnapData->nWidth = min(m_pSnapModeParam->nSelRectWidth, rcWnd.right() - pSnapData->x);
            }
            else
            {
                pSnapData->nWidth = min((pSnapData->x + m_pSnapModeParam->nSelRectWidth), rcWnd.right());
            }

            if (pSnapData->y > 0)
            {
                pSnapData->nHeight = min(m_pSnapModeParam->nSelRectHeight, rcWnd.bottom() - pSnapData->y);
            }
            else
            {
                pSnapData->nHeight = min((pSnapData->y + m_pSnapModeParam->nSelRectHeight), rcWnd.bottom());
            }

            pSnapData->x = max(pSnapData->x, 0);
            pSnapData->y = max(pSnapData->y, 0);

            pSnapData->x = pSnapData->x*1.0 / rcWnd.width() * pSnapData->pFrame->nPicWidth;
            pSnapData->y = pSnapData->y*1.0 / rcWnd.height() * pSnapData->pFrame->nPicHeight;
            pSnapData->nWidth = (pSnapData->nWidth *1.0) / rcWnd.width() * pSnapData->pFrame->nPicWidth;
            pSnapData->nHeight = (pSnapData->nHeight *1.0) / rcWnd.height() * pSnapData->pFrame->nPicHeight;

            emit CustomedSnap(pSnapData);
        }
    }
  
    
}

void DrawWnd::OnPBCtrl(int nFramIndex)
{
    SetWndMode(PictureMode);
    m_bInPB = true;
    if (nFramIndex< m_vcSyncRender.size())
    {
        SnapData::Ptr pSnapData = std::make_shared<SnapData>(m_vcSyncRender[nFramIndex]);
        ShowFrame(pSnapData);
    }
}

void DrawWnd::DrawDefault(QPainter& painter)
{
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rcWnd(rect());
    painter.fillRect(rcWnd, s_qcl292C39);

    if (m_nIndex>=0)
    {
        DrawText(painter, QVariant::fromValue(m_nIndex).toString());
    }
    Drawborder(painter, rcWnd);
}

void DrawWnd::DrawText(QPainter& painter, QString& strText)
{
    QRect rcWnd = rect();
    QFont font("Arial Unicode MS", 20, 50);
    painter.setFont(font);
    int widthOfText = painter.fontMetrics().width(strText);
    int heightOfText = painter.fontMetrics().height();
    painter.setPen(s_qclTEXT1);
    painter.drawText(rcWnd.left() + ((rcWnd.width() - widthOfText) / 2), rcWnd.top() + ((rcWnd.height() - heightOfText) / 2), strText);

}

void DrawWnd::Drawborder(QPainter& painter, QRect& rcArea)
{
    if (m_bSelected)
    {
        painter.setPen(QPen(s_qclBorder1, m_nBorderWidth));
        painter.drawRect(rcArea);
    }
}

FrameData::Ptr DrawWnd::GetFrame()
{
    FrameData::Ptr pRet = nullptr;
    if (m_queWaitRender.size())
    {
        pRet = m_queWaitRender.front();
        {
            std::lock_guard<std::mutex> guard(m_mxLockData);
            m_queWaitRender.pop();
        }

//         if ((m_nWndMode.load() == PanelMode::PreviewMode)
//             || ((m_nWndMode.load() == PanelMode::SnapMode) && m_pSnapModeParam && (m_pSnapModeParam->nSyncSpped == SyncSpeed::Sync_1)))
//         {
//             if (m_queWaitRender.size() > 1)
//             {
//                 update();
//             }
//         }
    }
    return pRet;
}

void DrawWnd::DrawImage(QPainter& painter, QImage& img)
{
    QRect rcDst(0, 0, size().width(), size().height());
    int nShowPicWidth = img.width()* m_dbCoef;
    int nShowPicHeight = img.height() * m_dbCoef;
    int nZoomedX = (img.width() - nShowPicWidth) / 2;
    int nZoomedY = (img.height() - nShowPicHeight) / 2;
    int nShowPosX = max(0, nZoomedX + m_viewOffset.x());
    int nShowPosY = max(0, nZoomedY + m_viewOffset.y());
    nShowPosX = min(img.width() - nShowPicWidth, nShowPosX);
    nShowPosY = min(img.height() - nShowPicHeight, nShowPosY);

    m_viewOffset.setX(nShowPosX - nZoomedX);
    m_viewOffset.setY(nShowPosY - nZoomedY);
    QRect rcPic(nShowPosX, nShowPosY, nShowPicWidth, nShowPicHeight);
    painter.drawImage(rcDst, img, rcPic);
}

void DrawWnd::DrawSnapRect(QPainter& painter)
{
    if (!m_mousePos.isNull() && m_pSnapModeParam)
    {
        int x = m_mousePos.x();
        int y = m_mousePos.y();
        int nRectWidth = m_pSnapModeParam->nSelRectWidth;
        int nRectHeight = m_pSnapModeParam->nSelRectHeight;
        QRect rcArea;
        rcArea.setLeft(x - nRectWidth / 2);
        rcArea.setTop(y - nRectHeight / 2);
        rcArea.setWidth(nRectWidth);
        rcArea.setHeight(nRectHeight);
        painter.setPen(QPen(s_qclBorder3, 3));
        painter.drawRect(rcArea);
    }
}

void DrawWnd::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    switch (m_nWndMode.load())
    {
    case PanelMode::PreviewMode:
        DrawIntimeStream(painter);
        break;
    case  PanelMode::PictureMode:
        Draw4PictureMode(painter);
        break;
    case  PanelMode::SnapMode:
        DrawIntimeStream(painter);
        //Draw4SnapMode(painter);
        break;
    default:
        break;
    }
}

void DrawWnd::DrawIntimeStream(QPainter& painter)
{
    if (m_bInPreview)
    {
        QRect rcDst(0, 0, size().width(), size().height());
        if (auto pFrame = m_pCurFrame)
        {
            m_pLastFrame = pFrame;
        }

        if (m_pLastFrame)
        {
            QImage tmpImg((uchar *)m_pLastFrame->pBufData, m_pLastFrame->nPicWidth, m_pLastFrame->nPicHeight, QImage::Format_RGB32);
            DrawImage(painter, tmpImg);
        }
        else
        {
            DrawText(painter, m_strHint);
        }
        Drawborder(painter, rcDst);
        DrawSnapRect(painter);
    }
    else
    {
        DrawDefault(painter);
    }
}

void DrawWnd::Draw4PictureMode(QPainter& painter)
{
    std::lock_guard<mutex> guard(m_mxCurImageData);
    if (m_pSnapFrame)
    {
        auto pFrame = m_pSnapFrame->pFrame;
        QImage Image = QImage((uchar *)pFrame->pBufData, pFrame->nPicWidth, pFrame->nPicHeight, QImage::Format_RGB32);
        QImage showImg = Image.copy(m_pSnapFrame->x, m_pSnapFrame->y, m_pSnapFrame->nWidth, m_pSnapFrame->nHeight);
        QRect rcWnd(0,0,size().width(), size().height());
        DrawImage(painter, showImg);
        Drawborder(painter, rcWnd);
        if (m_bInPB)
        {
            DrawSnapRect(painter);
        }
    }
    else
    {
        DrawDefault(painter);
    }
}

void DrawWnd::StartFrameRateCheck()
{
    EndFrameRateCheck();
    if (m_pFrameRateTimer)
    {
        m_pFrameRateTimer->start(TIMER_CHECK_FRAME_RATE_INTERVAL);
    }
}

void DrawWnd::StartFrameUpdate(int nFrameRate)
{
    static int lastSpeed = 1;
    if (lastSpeed != nFrameRate)
    {
        EndFrameUpdate();
        if (m_pFrameUpdateTimer)
        {
            m_pFrameUpdateTimer->start(max(((1000 / nFrameRate) - 25), 1));
        }
        lastSpeed = nFrameRate;
    }
}

void DrawWnd::EndFrameRateCheck()
{
    if (m_pFrameRateTimer && m_pFrameRateTimer->isActive())
    {
        m_pFrameRateTimer->stop();
    }
}

void DrawWnd::EndFrameUpdate()
{
    if (m_pFrameUpdateTimer && m_pFrameUpdateTimer->isActive())
    {
        m_pFrameUpdateTimer->stop();
    }
}

void DrawWnd::OnTimeOut()
{
    auto pTimer = qobject_cast<QTimer*>(sender());
    if (pTimer == m_pFrameUpdateTimer)
    {
        m_pCurFrame = GetFrame();
        //LogDebug("Frame out %d", m_pCurFrame);
        repaint();
    }
    else if (pTimer == m_pFrameRateTimer)
    {
        m_nFrameRate = m_nFrameCome;
        m_nFrameCome = 0;
        int nShowFrameRate = m_nFrameRate;
        if (m_nWndMode == PanelMode::SnapMode && m_pSnapModeParam)
        {
            nShowFrameRate = 0.5 + (m_nFrameRate / m_pSnapModeParam->GetSpeedCoe());
        }
        
        nShowFrameRate = max(nShowFrameRate, 1) + 1;
        if (m_nFrameRate)
        {
            LogInfo("Current Frame Rate %d real speed %d ", m_nFrameRate, nShowFrameRate + 1);
        }
        StartFrameUpdate(nShowFrameRate);
    }
}
