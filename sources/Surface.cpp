#include <EasyWindow/Surface.hpp>

#ifdef EZWINDOW_OPENGL
    #include <GL/glew.h>
#endif

#ifdef EZWINDOW_VULKAN
    #define GLFW_INCLUDE_VULKAN
    #ifdef EZWINDOW_LINUX
        #define VK_USE_PLATFORM_XLIB_KHR
    #elif EZWINDOW_WINDOWS
        #define VK_USE_PLATFORM_WIN32_KHR
    #endif
#endif

#include <GLFW/glfw3.h>

#ifndef EZWINDOW_OPENGL
    #ifdef EZWINDOW_LINUX
        #define GLFW_EXPOSE_NATIVE_X11
    #elif EZWINDOW_WINDOWS
        #define GLFW_EXPOSE_NATIVE_WIN32
    #elif EZWINDOW_OSX
        #define GLFW_EXPOSE_NATIVE_COCOA
    #endif
    #include <GLFW/glfw3native.h>
#endif


EZWINDOW_NAMESPACE_BEGIN

/* ####################################################################################### */
/* Constructors */
/* ####################################################################################### */

Surface::Surface()
{
    if (glfwInit() != GLFW_TRUE)
    {
        EZWINDOW_ERROR("Cant initialize GLFW");
    }

#ifdef EZWINDOW_OPENGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
#endif

    setVisible(m_visible);
    setSamples(m_samples);
    setChannelsBits(m_channels);
    setDoubleBufferEnabled(m_doubleBuffer);

    m_window = glfwCreateWindow(m_size.w, m_size.h, m_title.data(), nullptr, nullptr);

#ifdef EZWINDOW_OPENGL
    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE;

    GLenum GLEWInitResult = glewInit();
    if (GLEWInitResult != GLEW_OK)
    {
        EZWINDOW_ERROR(glewGetErrorString(GLEWInitResult));
    }
#endif
}

/* --------------------------------------------------------------------------------------- */

Surface::~Surface()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

/* ####################################################################################### */
/* Properties setters */
/* ####################################################################################### */

void
Surface::setSize(const Size<uint64_t>& size)
{
    m_size = size;
    glfwSetWindowSize(m_window, m_size.w, m_size.h);
}

/* --------------------------------------------------------------------------------------- */

void
Surface::setVisible(bool visible)
{
    m_visible = visible;
    glfwWindowHint(GLFW_VISIBLE, visible);
}

/* --------------------------------------------------------------------------------------- */

void
Surface::setTitle(const std::string& title)
{
    m_title = title;
    if (m_window)
    {
        glfwSetWindowTitle(m_window, title.data());
    }
}

/* --------------------------------------------------------------------------------------- */

void
Surface::setSamples(int samplesCount)
{
    m_samples = samplesCount;
    glfwWindowHint(GLFW_SAMPLES, samplesCount);
}

/* --------------------------------------------------------------------------------------- */

void
Surface::setDoubleBufferEnabled(bool enabled)
{
    m_doubleBuffer = enabled;
    glfwWindowHint(GLFW_DOUBLEBUFFER, enabled);
}

/* --------------------------------------------------------------------------------------- */

void
Surface::setChannelsBits(const ChannelsBits& bits)
{
    m_channels = bits;
    glfwWindowHint(GLFW_RED_BITS, bits.r);
    glfwWindowHint(GLFW_GREEN_BITS, bits.g);
    glfwWindowHint(GLFW_BLUE_BITS, bits.b);
    glfwWindowHint(GLFW_ALPHA_BITS, bits.a);
    glfwWindowHint(GLFW_DEPTH_BITS, bits.d);
    glfwWindowHint(GLFW_STENCIL_BITS, bits.s);
}

/* ####################################################################################### */
/* Methods */
/* ####################################################################################### */

#ifdef EZWINDOW_VULKAN
std::int64_t
Surface::createVulkanSurface(void* instance, void* surface, const void* allocationCallbacks)
{
    return glfwCreateWindowSurface
    (
        *reinterpret_cast<VkInstance*>(instance),
        m_window,
        reinterpret_cast<const VkAllocationCallbacks*>(allocationCallbacks),
        reinterpret_cast<VkSurfaceKHR*>(surface)
    );
}

/* --------------------------------------------------------------------------------------- */

std::vector<std::string>
Surface::vulkanExtensions()
{
    uint32_t count = 0;
    const char** extensions = glfwGetRequiredInstanceExtensions(&count);

    std::vector<std::string> result;
    result.reserve(count);

    for (uint32_t i = 0; i < count; ++i)
    {
        result.emplace_back(extensions[i]);
    }

    return result;
}
#endif

/* ####################################################################################### */
/* Platform data pointers */
/* ####################################################################################### */

void*
Surface::nativeWindowHandle()
{
#ifdef EZWINDOW_LINUX
    return (void*)(uintptr_t)glfwGetX11Window(m_window);
#elif EZWINDOW_OSX
    return glfwGetCocoaWindow(m_window);
#elif EZWINDOW_WINDOWS
    return glfwGetWin32Window(m_window);
#else
    return nullptr;
#endif
}

/* --------------------------------------------------------------------------------------- */

void*
Surface::nativeDisplayType()
{
#ifdef EZWINDOW_LINUX
    return glfwGetX11Display();
#else
    return nullptr;
#endif
}

EZWINDOW_NAMESPACE_END