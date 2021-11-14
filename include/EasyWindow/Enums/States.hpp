#pragma once


#include <EasyWindow/Global.hpp>


EZWINDOW_NAMESPACE_BEGIN

enum class EState : std::int64_t
{
    Release     = 0,
    Press       = 1,
    Repeat      = 2
};

EZWINDOW_NAMESPACE_END