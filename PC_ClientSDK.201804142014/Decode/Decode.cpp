// Decode.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Decode.h"
#include "IDecoder.h"
#include "H264Decode.h"
#include "vector"

IDecoder::Ptr Decode_CreateDecoder()
{
    return std::make_shared<H264Decode>();
}
