#pragma once


#include <cstdint>
#include <iostream>
#include <type_traits>

#define EZWINDOW_INFO(message) std::cout << "EasyWindow [info]: " << message << std::endl;

/* --------------------------------------------------------------------------------------- */

#define EZWINDOW_WARNING(message) std::cout << "EasyWindow [warning] in " << __FUNCTION__ << "(): " << message << std::endl << __FILE__ << ":" << __LINE__ << std::endl;

/* --------------------------------------------------------------------------------------- */

#define EZWINDOW_ERROR(message) std::cout << "EasyWindow [warning] in " << __FUNCTION__ << "(): " << message << std::endl << __FILE__ << ":" << __LINE__ << std::endl; exit(-1);

/* --------------------------------------------------------------------------------------- */

#define EZWINDOW                    ezwin
#define EZWINDOW_NAMESPACE_BEGIN    namespace EZWINDOW {
#define EZWINDOW_NAMESPACE_END      }

/* --------------------------------------------------------------------------------------- */

EZWINDOW_NAMESPACE_BEGIN

template<typename T>
struct Vector
{
    std::enable_if_t<std::is_arithmetic_v<T>, T> x, y;

    constexpr Vector() = default;
    constexpr Vector(T value) : x(value), y(value) {}
    constexpr Vector(T X, T Y) : x(X), y(Y) {}
};

/* --------------------------------------------------------------------------------------- */

template<typename T>
struct Size
{
    std::enable_if_t<std::is_arithmetic_v<T>, T> w, h;

    constexpr Size() = default;
    constexpr Size(T value) : w(value), h(value) {}
    constexpr Size(T W, T H) : w(W), h(H) {}
};

/* --------------------------------------------------------------------------------------- */

template<typename T>
constexpr std::enable_if_t<std::is_floating_point_v<T>,T>
fit(T value, T omin, T omax, T nmin, T nmax)
{
    if (omin < omax)
    {
        return ((value > omax ? omax : value < omin ? omin : value) - omin) / (omax - omin) * (nmax - nmin) + nmin;
    }
    else
    {
        return ((value > omin ? omin : value < omax ? omax : value) - omax) / (omin - omax) * (nmin - nmax) + nmax;
    }
}

EZWINDOW_NAMESPACE_END