#pragma once
#include "QtDefine.h"
#include <initializer_list>
#include "LogManager.h"
namespace utils
{
    bool CheckPointer(std::initializer_list<void*> list);
    bool PtinWnd(QWidgetPtr pWidget);
}


