#include "DrawWnd.h"
#include <QPainter>
#include "QtDefine.h"
#include "utils.h"
DrawWnd::DrawWnd(int index, QWidget *parent)
    :QWidget(parent)
    , m_nIndex(index)
{
    m_strHint = QStringLiteral("监控点正在预览....");
}


DrawWnd::~DrawWnd()
{
}

void DrawWnd::SetPreviewStatu(bool bPreview /*= false*/)
{
    m_bInPreview = bPreview;
    if (!bPreview)
    {
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
        SetFrame(pFrame);
        update();
    }
}

void DrawWnd::SetFrame(FrameData::Ptr pFrame)
{
    {
        std::lock_guard<std::mutex> guard(m_mxLockData);
        m_queWaitRender.push(pFrame);
    }
}

void DrawWnd::SetHintString(const QString& strHint)
{
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
        nMoveX = m_viewPos.x() * -1;
        nMoveY = m_viewPos.y() * -1;
        break;
    case PtzCommand::SetSpeed:
        m_nPtzSpeed = nParam;
        break;
    default:
        break;
    }

    m_dbCoef = 1 - (m_nZoom - 1) * 0.1;
    m_viewPos.setX(m_viewPos.x() + nMoveX);
    m_viewPos.setY(m_viewPos.y() + nMoveY);
    
    update();
}

int DrawWnd::GetPtzSpeed()
{
    return m_nPtzSpeed;
}

void DrawWnd::DrawDefault()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rcWnd(rect());
    painter.fillRect(rcWnd, s_qcl292C39);

    DrawText(painter, QVariant::fromValue(m_nIndex).toString());
    Drawborder(painter, rcWnd);
}

void DrawWnd::DrawText(QPainter& painter, QString& strText)
{
    QRect rcWnd = rect();
    QFont font("Arial Unicode MS", 30, 150);
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
        if (m_queWaitRender.size() > 3)
        {
            update();
        }
    }
    return pRet;
}
// 
// void DrawWnd::initializeGL()
// {
//     m_pReander = std::make_shared<YUV420P_Render>(this);
//     if (m_pReander)
//     {
//         m_pReander->initialize();
//     }
// //     auto ret = connect(this, &DrawWnd::FrameReady, this, [this](FrameData::Ptr pFrame) {
// //         makeCurrent();
// //         this->SetFrame(pFrame);
// //         this->paintGL();
// //         this->update();
// //         doneCurrent();
// //     });
// }

// #include <QFile>
// void DrawWnd::paintGL()
// {
//     if (auto pFrameCurrent = GetFrame())
//     {
//         std::string fileName = utils::GetTmpPath() + utils::GetUUID() + ".bmp";
//         QFile file(QString::fromStdString(fileName));
//         if (file.open(QFile::WriteOnly | QFile::Truncate))
//         {
//             file.write((const char*)pFrameCurrent->pBufData, pFrameCurrent->nBuffLen);
//             file.close();
//         }
// //         QImage tmpImg((uchar *)pFrameCurrent->pBufData, pFrameCurrent->nPicWidth, pFrameCurrent->nPicHeight, QImage::Format_RGB32);
// //         QPainter paint;
// //         paint.drawImage(0,0,tmpImg);
//     }
// 
// // 
// //     if (!m_pReander)
// //     {
// //         return;
// //     }
// //     if (m_bInPreview)
// //     {
// //         auto pFrameCurrent = GetFrame();
// //         if (pFrameCurrent)
// //         {
// //             int yLen = pFrameCurrent->nPicWidth * pFrameCurrent->nPicHeight;
// //             int uLen = yLen / 4;
// //             m_pReander->render(pFrameCurrent->pBufData, pFrameCurrent->pBufData + yLen, pFrameCurrent->pBufData + yLen + uLen, pFrameCurrent->nPicWidth, pFrameCurrent->nPicHeight, 0);
// //             //m_pReander->render(pFrameCurrent->pBufData, pFrameCurrent->nPicWidth, pFrameCurrent->nPicHeight,0);
// //         }
// //     }
// //     else
// //     {
// //         m_pReander->renderBK();
// //     }
//     
// }

void DrawWnd::paintEvent(QPaintEvent *event)
{
    if (m_bInPreview)
    {
        QPainter painter(this);
        QRect rcDst(0, 0, size().width(), size().height());
        if (auto pFrame = GetFrame())
        {
            m_pLastFrame = pFrame;
        }

        if (m_pLastFrame)
        {
            int nShowPicWidth = m_pLastFrame->nPicWidth * m_dbCoef;
            int nShowPicHeight = m_pLastFrame->nPicHeight * m_dbCoef;
            int nShowPosX = max(0,m_viewPos.x());
            int nShowPosY = max(0,m_viewPos.y());
            nShowPosX = min(m_pLastFrame->nPicWidth - nShowPicWidth, nShowPosX);
            nShowPosY = min(m_pLastFrame->nPicHeight - nShowPicHeight, nShowPosY);

            m_viewPos.setX(nShowPosX);
            m_viewPos.setY(nShowPosY);
            QRect rcPic(nShowPosX, nShowPosY, nShowPicWidth, nShowPicHeight);
            QImage tmpImg((uchar *)m_pLastFrame->pBufData, m_pLastFrame->nPicWidth, m_pLastFrame->nPicHeight, QImage::Format_RGB32);
            painter.drawImage(rcDst, tmpImg, rcPic);
        }
        else
        {
            DrawText(painter, m_strHint);
        }
        Drawborder(painter, rcDst);
    }
    else
    {
        DrawDefault();
    }
}
