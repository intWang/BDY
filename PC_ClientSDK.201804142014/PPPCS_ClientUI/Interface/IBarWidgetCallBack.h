#pragma once

class IBarWidgetCallBack
{
public:
    using Ptr = IBarWidgetCallBack * ;
    virtual bool OnButtonClick(int nButtonID) { return false; };

private:

};
