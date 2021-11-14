#pragma once


#include <EasyWindow/Global.hpp>


EZWINDOW_NAMESPACE_BEGIN

enum class EModifier : std::int64_t
    {
        Alt         = 0x0004,
        Ctrl        = 0x0002,
        Shift       = 0x0001,
        Super       = 0x0008
};

EZWINDOW_NAMESPACE_END