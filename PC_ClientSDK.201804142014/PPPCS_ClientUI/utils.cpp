#include "QtDefine.h"


bool CheckPointer(std::initializer_list<void*> list)
{
    for (auto ptr : list)
    {
        if (!ptr)
        {
            return false;
        }
    }
    return true;
}
