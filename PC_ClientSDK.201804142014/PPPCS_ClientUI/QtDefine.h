#pragma once
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include "LogManager.h"
#include <QJsonObject>
#include <QFormLayout>
#include <QCheckBox>
#include <QComboBox>
#include <string>
#include <vector>
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
using QStandardItemModelPtr = QStandardItemModel * ;
using QStandardItemPtr = QStandardItem * ;
using QFormLayoutPtr = QFormLayout * ;
using QCheckBoxPtr = QCheckBox * ;
using QComboBoxPtr = QComboBox * ;


class PreviewWnd;
using PreviewWndPtr = PreviewWnd * ;
using SPreviewWndPtr = std::shared_ptr<PreviewWnd>;


class PreviePanel;
using PreviePanelPtr = PreviePanel * ;
using SPreviePanelPtr = std::shared_ptr<PreviePanel>;

enum DevideScreen
{
    Screen_1X1 = 0,
    Screen_2X2,
    Screen_3X3,
    Screen_3X4,
};

enum DevTreeNodeType
{
    Group,
    Device,
    Channel,
};

enum DevTreeNodeStatu
{
    Default,
    Connecting,
    Pause,
    Play,
};

enum class PtzCommand
{
    ZoomIn,
    ZoomOut,
    MoveLT,
    MoveL,
    MoveLD,
    MoveD,
    MoveRD,
    MoveR,
    MoveRT,
    MoveT,
    Restore,
    SetSpeed,
};
Q_DECLARE_METATYPE(PtzCommand);

template<typename T>
auto makeQTObject(QWidgetPtr pParent = nullptr) -> T*
{
    return new T(pParent);
}

template<typename T>
auto makeQTObject(QObjectPtr pParent = nullptr) -> T*
{
    return new T(pParent);
}

#define  MQ(type) makeQTObject<type>
#define COLOR_TOP_444858 68,72,88
#define COLOR_TOP_292C39 41,44,57
#define COLOR_MID_1E2233 30,34,51
#define COLOR_BORDER 66,172,230
#define COLOR_BORDER2 0,0,0
#define COLOR_BK 28,32,48
#define COLOR_TEXT_1 199,202,217 
#define COLOR_GROUP_BTN_SEL 61,79,114

static QColor s_qcl444858(COLOR_TOP_444858);
static QColor s_qcl292C39(COLOR_TOP_292C39);
static QColor s_qcl1E2233(COLOR_MID_1E2233);
static QColor s_qclBorder1(COLOR_BORDER);
static QColor s_qclBorder2(COLOR_BORDER2);
static QColor s_qclTEXT1(COLOR_TEXT_1);
static QColor s_qclGroupBtnSel(COLOR_GROUP_BTN_SEL);
static QColor s_qclBK283248(COLOR_BK);


#define TREEROOTID 0
#define BASEID 1
#define BASESPACE_DEVICE 1000
#define MAKEDEVID(nGroupID, nDevIndex) (nGroupID*BASESPACE_DEVICE + nDevIndex)
#define MAKECHANNELID(nDevID, nChannlIndex) (nDevID*100 + nChannlIndex)
#define GETDEVID(nID) (nID%BASESPACE_DEVICE)
#define GETGROUPID(nID) (nID/BASESPACE_DEVICE)

///GroupID 1~99
///DevID 1001~ 99999
///channelID 100101~9999999

#define max(a, b) a>b?a:b
#define min(a, b) a>b?b:a

#define CUSTOMIZED_STATU_BASE 100
#define CSTATU_RECONNECTED (CUSTOMIZED_STATU_BASE + 1)