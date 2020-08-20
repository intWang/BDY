#include "DrawWnd.h"
#include <QPainter>
#include "QtDefine.h"
#include "utils.h"
DrawWnd::DrawWnd(int index, QWidget *parent)
    :QWidget(parent)
    , m_nIndex(index)
{
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
    update();
}

void DrawWnd::SetSelected(bool bSelected)
{
    m_bSelected = bSelected;
    update();
}

void DrawWnd::InputFrameData(FrameData::Ptr pFrame)
{
     SetFrame(pFrame);
     update();
}

void DrawWnd::SetFrame(FrameData::Ptr pFrame)
{
    {
        std::lock_guard<std::mutex> guard(m_mxLockData);
        m_queWaitRender.push(pFrame);
        if (m_queWaitRender.size() > 10)
        {
            m_queWaitRender.pop();
        }
    }
}

void DrawWnd::DrawDefault()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rcWnd({0,0}, size());
    painter.fillRect(rcWnd, s_qcl292C39);

    if (m_bSelected)
    {
        painter.setPen(QPen(s_qclBorder1, m_nBorderWidth));
        painter.drawRect(rcWnd);
    }

    QRect rcDraw = GetDrawArea();
    QFont font("Arial Unicode MS", 30, 150);
    painter.setFont(font);
    QString text = QVariant::fromValue(m_nIndex).toString();
    int widthOfText = painter.fontMetrics().width(text);
    int heightOfText = painter.fontMetrics().height();
    painter.setPen(s_qclTEXT1);
    painter.drawText(rcDraw.left() + ((rcDraw.width() - widthOfText)/ 2), rcDraw.top() + ((rcDraw.height()- heightOfText)/2), text);
}

QRect DrawWnd::GetDrawArea()
{
    QSize wndSize = size();
    QRect rc({0,0}, wndSize);

    if (m_bSelected)
    {
        rc.setRect(m_nBorderWidth, m_nBorderWidth,size().width()-(2* m_nBorderWidth), size().height() - (2 * m_nBorderWidth));
    }
    return rc;
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
        if (auto pFrame = GetFrame())
        {
            QRect rcDst(0,0,size().width(), size().height());
            QRect rcPic(0,0, pFrame->nPicWidth, pFrame->nPicHeight);
            QImage tmpImg((uchar *)pFrame->pBufData, pFrame->nPicWidth, pFrame->nPicHeight, QImage::Format_RGB32);
            painter.drawImage(rcDst, tmpImg, rcPic);
        }
    }
    else
    {

        DrawDefault();
    }
}
