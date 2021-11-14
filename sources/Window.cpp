#include <EasyWindow/Window.hpp>

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

Window::~Window()
{
    if (m_window)
    {
        glfwTerminate();
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

/* --------------------------------------------------------------------------------------- */

Window::Window(EOriginCorner originCorner)
    : m_originCorner(originCorner)
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
    
    if (!m_window)
    {
        EZWINDOW_ERROR("Cant create GLFW window");
    }

    glfwSetWindowUserPointer(m_window, this);

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int w, int h)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->setSize({uint64_t(w), uint64_t(h)});
        self->resizeEvent();
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->keyEvent
        (
            static_cast<EKey>(key),
            static_cast<EState>(action),
            static_cast<EModifier>(mods)
        );
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        double ys[2] = {y, self->size().h - y};

        self->mouseMoveEvent(Vector<uint64_t>{uint64_t(x), uint64_t(ys[uint8_t(self->m_originCorner)])});
    });

    glfwSetCursorEnterCallback(m_window, [](GLFWwindow* window, int entered)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->mouseAreaEvent(bool(entered));
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->buttonEvent
        (
            static_cast<EButton>(button),
            static_cast<EState>(action),
            static_cast<EModifier>(mods)
        );
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double x, double y)
    {
        auto* self = static_cast<Window*>(glfwGetWindowUserPointer(window));
        self->scrollEvent(Vector<double>{x,y});
    });
}

/* ####################################################################################### */
/* Properties setters */
/* ####################################################################################### */

void
Window::setSize(const Size<uint64_t>& size)
{
    m_size = size;
    glfwSetWindowSize(m_window, m_size.w, m_size.h);
}

/* --------------------------------------------------------------------------------------- */

void
Window::setVisible(bool visible)
{
    m_visible = visible;
    glfwWindowHint(GLFW_VISIBLE, visible);
}

/* --------------------------------------------------------------------------------------- */

void
Window::setTitle(const std::string& title)
{
    m_title = title;
    if (m_window)
    {
        glfwSetWindowTitle(m_window, title.data());
    }
}

/* --------------------------------------------------------------------------------------- */

void
Window::setSamples(int samplesCount)
{
    m_samples = samplesCount;
    glfwWindowHint(GLFW_SAMPLES, samplesCount);
}

/* --------------------------------------------------------------------------------------- */

void
Window::setDoubleBufferEnabled(bool enabled)
{
    m_doubleBuffer = enabled;
    glfwWindowHint(GLFW_DOUBLEBUFFER, enabled);
}

/* --------------------------------------------------------------------------------------- */

void
Window::setChannelsBits(const ChannelsBits& bits)
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
/* Getters */
/* ####################################################################################### */

Vector<uint64_t>
Window::mousePosition()
{
    double x = 0;
    double y = 0;

    glfwGetCursorPos(m_window, &x, &y);

    double ys[2] = {y, size().h - y};

    return {uint64_t(x), uint64_t(ys[uint8_t(m_originCorner)])};
}

/* --------------------------------------------------------------------------------------- */

MouseOffset
Window::mouseOffset()
{
    MouseOffset result {};
    result.intValue = mousePosition();

    {
        const auto currentPos = relative<double>(result.intValue);
        result.rel11Value.x = currentPos.x - m_prev_tick_mouse_pos.rel11Value.x;
        result.rel11Value.y = currentPos.y - m_prev_tick_mouse_pos.rel11Value.y;
    }
    {
        const auto currentPos = relative01<double>(result.intValue);
        result.rel01Value.x = currentPos.x - m_prev_tick_mouse_pos.rel01Value.x;
        result.rel01Value.y = currentPos.y - m_prev_tick_mouse_pos.rel01Value.y;
    }

    result.intValue.x -= m_prev_tick_mouse_pos.intValue.x;
    result.intValue.y -= m_prev_tick_mouse_pos.intValue.y;

    return result;
}

/* --------------------------------------------------------------------------------------- */

bool
Window::isMouseInWindow()
{
    return bool(glfwGetWindowAttrib(m_window, GLFW_HOVERED));
}

/* --------------------------------------------------------------------------------------- */

double
Window::time() const
{
    return m_time;
}

/* --------------------------------------------------------------------------------------- */

double
Window::tickDelta() const
{
    return m_curr_tick;
}

/* ####################################################################################### */
/* Platform data pointers */
/* ####################################################################################### */

void*
Window::nativeWindowHandle()
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
Window::nativeDisplayType()
{
#ifdef EZWINDOW_LINUX
    return glfwGetX11Display();
#else
    return nullptr;
#endif
}

/* ####################################################################################### */
/* Render backend methods */
/* ####################################################################################### */

#ifdef EZWINDOW_VULKAN
std::int64_t
Window::createVulkanSurface(void* instance, void* surface, const void* allocationCallbacks)
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
Window::vulkanExtensions()
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
/* Methods */
/* ####################################################################################### */

void
Window::run()
{
    if (!m_window)
    {
        EZWINDOW_ERROR("Cant start window event loop. GLFW window was not created.");
    }

    beforeLoop();

    glfwSetTime(0.0);

    while (!glfwWindowShouldClose(m_window))
    {
        m_time = glfwGetTime();
        m_curr_tick = m_time - m_prev_tick;
        m_prev_tick = m_time;

        glfwPollEvents();

        tickEvent();
        clearEvent();
        renderEvent();
    }

    afterLoop();
}

/* --------------------------------------------------------------------------------------- */

void
Window::close()
{
    glfwSetWindowShouldClose(m_window, true);
}

/* --------------------------------------------------------------------------------------- */

void
Window::swapFrameBuffers()
{
    glfwSwapBuffers(m_window);
}

/* --------------------------------------------------------------------------------------- */

EState
Window::buttonState(EButton button)
{
    auto val = glfwGetMouseButton(m_window, size_t(button));
    return *reinterpret_cast<const EState*>(&val);
}

/* --------------------------------------------------------------------------------------- */

EState
Window::keyState(EKey key)
{
    auto val = glfwGetKey(m_window, size_t(key));
    return *reinterpret_cast<const EState*>(&val);
}

/* ####################################################################################### */
/* Window events */
/* ####################################################################################### */

void
Window::beforeLoop()
{

}

/* --------------------------------------------------------------------------------------- */

void
Window::tickEvent()
{
    m_prev_tick_mouse_pos.intValue      = mousePosition();
    m_prev_tick_mouse_pos.rel01Value    = relative01<double>(mousePosition());
    m_prev_tick_mouse_pos.rel11Value    = relative<double>(mousePosition());
}

/* --------------------------------------------------------------------------------------- */

void
Window::clearEvent()
{

}

/* --------------------------------------------------------------------------------------- */

void
Window::renderEvent()
{

}

/* --------------------------------------------------------------------------------------- */

void
Window::afterLoop()
{

}

/* --------------------------------------------------------------------------------------- */

void
Window::resizeEvent()
{

}

/* --------------------------------------------------------------------------------------- */

void
Window::keyEvent(EKey key, EState state, EModifier modifier)
{
    if (key == EKey::Escape && state == EState::Release)
    {
        close();
    }
}

/* --------------------------------------------------------------------------------------- */

void
Window::mouseAreaEvent(bool entered)
{

}

/* --------------------------------------------------------------------------------------- */

void
Window::mouseMoveEvent(Vector<uint64_t> position)
{

}

/* --------------------------------------------------------------------------------------- */

void
Window::buttonEvent(EButton button, EState state, EModifier modifier)
{

}

/* --------------------------------------------------------------------------------------- */

void
Window::scrollEvent(Vector<double> offset)
{

}

EZWINDOW_NAMESPACE_END