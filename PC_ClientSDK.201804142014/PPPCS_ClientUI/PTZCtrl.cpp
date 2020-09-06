#include <QResizeEvent>
#include "PTZCtrl.h"
#include "QtDefine.h"
#include "utils.h"
#include "QBitmap"

PTZCtrl::PTZCtrl(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_vcBtns = { ui.btnLT, ui.btnT , ui.btnRT,
                ui.btnL, ui.btnRestore, ui.btnR,
                ui.btnLD, ui.btnD, ui.btnRD };

    setWindowFlags(Qt::FramelessWindowHint);

    m_pixMask.load(":/Black/res/bk/bk_dir.png");
    setFixedSize(m_pixMask.width(), m_pixMask.height());

//     int base = 0;
//     QMatrix matrix;
//     m_pixDirMask.load(":/Black/res/dirb4_l.png");
    utils::TravelVector(m_vcBtns, [this](QPushButtonPtr pItem) {
        if (pItem)
        {
//             matrix.rotate(45);
//             auto pixMap = m_pixDirMask.transformed(matrix, Qt::SmoothTransformation);
//             pixMap.save(QVariant::fromValue(base++).toString() + ".png");
            if (pItem != this->ui.btnRestore)
            {
                pItem->setAutoRepeat(true); //启用长按
                pItem->setAutoRepeatDelay(400);//触发长按的时间
                pItem->setAutoRepeatInterval(100);//长按时click信号间隔
            }
            pItem->setText("");

            connect(pItem, &QPushButton::clicked, this, &PTZCtrl::OnBtnClick);
        }
        return false;
    });

    setAutoFillBackground(true);
    setMask(QBitmap(m_pixMask.mask()));

    ui.btnLT->setGeometry(25, 23, 30, 30);
    ui.btnRT->setGeometry(108, 23, 30, 30);
    ui.btnLD->setGeometry(25, 108, 30, 30);
    ui.btnRD->setGeometry(108, 108, 30, 30);

    ui.btnT->setGeometry(66, 8, 30, 30);
    ui.btnR->setGeometry(125, 66, 30, 30);
    ui.btnD->setGeometry(66, 125, 30, 30);
    ui.btnL->setGeometry(8, 66, 30, 30);

    ui.btnRestore->setGeometry(61,62,40,40);
}

PTZCtrl::~PTZCtrl()
{
}

void PTZCtrl::EnableCtrl(bool bEnable)
{
    utils::TravelVector(m_vcBtns, [this, bEnable](QPushButtonPtr pItem) {
        if (pItem)
        {
            pItem->setEnabled(bEnable);
        }
        return false;
    });
}

void PTZCtrl::OnBtnClick()
{
    auto pBtn = qobject_cast<QPushButtonPtr>(sender());
    if (pBtn == ui.btnLT)
    {
        emit DirBtnClick(PtzCommand::MoveLT);
    }
    else if (pBtn == ui.btnL)
    {
        emit DirBtnClick(PtzCommand::MoveL);
    }
    else if (pBtn == ui.btnLD)
    {
        emit DirBtnClick(PtzCommand::MoveLD);
    }
    else if(pBtn == ui.btnD)
    {
        emit DirBtnClick(PtzCommand::MoveD);
    }
    else if (pBtn == ui.btnRD)
    {
        emit DirBtnClick(PtzCommand::MoveRD);
    }
    else if (pBtn == ui.btnR)
    {
        emit DirBtnClick(PtzCommand::MoveR);
    }
    else if (pBtn == ui.btnRT)
    {
        emit DirBtnClick(PtzCommand::MoveRT);
    }
    else if (pBtn == ui.btnT)
    {
        emit DirBtnClick(PtzCommand::MoveT);
    }
    else if (pBtn == ui.btnRestore)
    {
        emit DirBtnClick(PtzCommand::Restore);
    }
}

void PTZCtrl::resizeEvent(QResizeEvent *event)
{
    if (event)
    {
//         int nBtnW = (min(szWnd.width(), szWnd.height()) - 20) / 3;
//         int nBtnH = nBtnW;
//         utils::TravelVector(m_vcBtns, [this, nBtnW, nBtnH](QPushButtonPtr pItem) {
//             if (pItem)
//             {
//                 pItem->setFixedSize(nBtnW, nBtnH);
//             }
//             return false;
//         });
    }
}

void PTZCtrl::paintEvent(QPaintEvent *event)
{
    auto bgPalette = palette();
    bgPalette.setBrush(QPalette::Background, m_pixMask);
    setPalette(bgPalette);
}
