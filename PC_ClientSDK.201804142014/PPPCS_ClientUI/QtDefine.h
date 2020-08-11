#pragma once
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTreeView>
#include "LogManager.h"

using QLabelPtr = QLabel*;
using QPushButtonPtr = QPushButton*;
using QObjectPtr = QObject *;
using QWidgetPtr = QWidget * ;
using QHBoxLayoutPtr = QHBoxLayout * ;
using QVBoxLayoutPtr = QVBoxLayout * ;
using QGridLayoutPtr = QGridLayout * ;
using QLayoutPtr = QLayout * ;
using QLineEditPtr = QLineEdit * ;
using QQWidgetPtr = QWidget * ;
using QTreeViewPtr = QTreeView * ;


class PreviewWnd;
using PreviewWndPtr = PreviewWnd * ;
using SPreviewWndPtr = std::shared_ptr<PreviewWnd>;


class PreviePanel;
using PreviePanelPtr = PreviePanel * ;
using SPreviePanelPtr = std::shared_ptr<PreviePanel>;

enum DevideScreen
{
    Screen_1X1,
    Screen_2X2,
    Screen_3X3,
    Screen_3X4,
};

template<typename T>
auto makeQTObject(QWidgetPtr pParent = nullptr) -> T*
{
    return new T(pParent);
}

#define  MQ(type) makeQTObject<type>
#define COLOR_TOP_444858 68,72,88
#define COLOR_TOP_292C39 41,44,57
#define COLOR_MID_1E2233 30,34,51

static QColor s_qcl444858(COLOR_TOP_444858);
static QColor s_qcl292C39(COLOR_TOP_292C39);
static QColor s_qcl1E2233(COLOR_MID_1E2233);
