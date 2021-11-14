#pragma once


#include <chrono>
#include <string>
#include <vector>
#include <EasyWindow/Global.hpp>
#include <EasyWindow/Enums/Keys.hpp>
#include <EasyWindow/Enums/States.hpp>
#include <EasyWindow/Enums/Buttons.hpp>
#include <EasyWindow/Enums/Modifiers.hpp>
#include <EasyWindow/Enums/OriginCorner.hpp>


class GLFWwindow;

EZWINDOW_NAMESPACE_BEGIN

struct ChannelsBits
{
    std::uint32_t r {8};     // red
    std::uint32_t g {8};     // green
    std::uint32_t b {8};     // blue
    std::uint32_t a {8};     // alpha
    std::uint32_t d {32};    // depth
    std::uint32_t s {16};    // stencil
};

struct MouseOffset
{
    Vector<uint64_t>
    intValue {};

    Vector<double>
    rel01Value {};

    Vector<double>
    rel11Value {};
};

class Window
{

/* ####################################################################################### */
public: /* Constructors */
/* ####################################################################################### */

    virtual
    ~Window();

    explicit
    Window(EOriginCorner originCorner);


/* ####################################################################################### */
public: /* Properties setters */
/* ####################################################################################### */

    /**
     * Set window size.
     * @param size Window size.
     */
    void
    setSize(const Size<uint64_t>& size);

    /**
     * Set window visibility
     * @param visible Window visibility
     */
    void
    setVisible(bool visible);

    /**
     * Set window title
     * @param title Title string
     */
    void
    setTitle(const std::string& title);

    /**
     * Set window samples
     * @param samplesCount Window samples count
     */
    void
    setSamples(int samplesCount);

    /**
     * Set double buffer enabled or disabled
     * @param enabled Enabled or disabled double buffer
     */
    void
    setDoubleBufferEnabled(bool enabled);

    /**
     * Set window channel bits.
     * @param bits Channel bits depth
     */
    void
    setChannelsBits(const ChannelsBits& bits);

/* ####################################################################################### */
public: /* Platform data pointers */
/* ####################################################################################### */

    /**
     * Gets native window handle pointer.
     * @param native window handle pointer.
     */
    void*
    nativeWindowHandle();

    /**
     * Gets native display type pointer.
     * @param native display type pointer.
     */
    void*
    nativeDisplayType();

/* ####################################################################################### */
public: /* Render backend methods */
/* ####################################################################################### */

#ifdef EZWINDOW_VULKAN
    /**
     * Creates VkSurface object.
     * @param surface Pointer to VkSurface object.
     * @return VkResult enum.
     */
    std::int64_t
    createVulkanSurface(void* instance, void* surface, const void* allocationCallbacks);

    /**
     * Fetch GLFW extensions list (for vulkan).
     * @return GLFW extensions list.
     */
    static std::vector<std::string>
    vulkanExtensions();
#endif

/* ####################################################################################### */
public: /* Getters */
/* ####################################################################################### */

        /** Get window size */
    Size<uint64_t>
    size() const
    {
        return m_size;
    }

    /** Is window visible */
    bool
    visible() const
    {
        return m_visible;
    }

    /** Get window title */
    std::string
    title() const
    {
        return m_title;
    }

    /** Get window samples */
    uint32_t
    samples() const
    {
        return m_samples;
    }

    /** Check whether double buffer enabled */
    bool
    doubleBuffered() const
    {
        return m_doubleBuffer;
    }

    /** Get window channels bits */
    ChannelsBits
    channelsBits() const
    {
        return m_channels;
    }

    /**
     * Gets mouse position.
     * @return mouse position.
     */
    Vector<uint64_t>
    mousePosition();

    /**
     * Gets mouse offset between position in current tick and previous tick.
     * @return Mouse position offset.
     */
    MouseOffset
    mouseOffset();

    /**
     * Checks is mouse in window area.
     * @return True if mouse in window area, false otherwise.
     */
    bool
    isMouseInWindow();

    /**
     * Gets current time (in seconds).
     * @return Time since event loop was started.
     */
    double
    time() const;

    /**
     * Gets delta between previous and current ticks (in seconds).
     * @return Tick delta time.
     */
    double
    tickDelta() const;

    /**
     * Gets window aspect ratio (width / height).
     * @return Aspect ratio.
     */
    template<typename T=float>
    std::enable_if_t<std::is_floating_point_v<T>, T>
    aspect() const;

/* ####################################################################################### */
public: /* Methods */
/* ####################################################################################### */

    /**
     * Start window main loop.
     */
    virtual void
    run();

    /**
     * Close window.
     */
    virtual void
    close();

    /**
     * Swap frame buffers.
     */

    void
    swapFrameBuffers();

    /**
     * Convert pixel coordinate to relative coordinate [-1,1].
     * @param pos Pixel coordinate to convert.
     * @return Converted coord.
     */
    template<typename T>
    Vector<T>
    relative(const Vector<uint64_t>& pos) const;

    /**
     * Convert pixel coordinate to relative coordinate [0,1].
     * @param pos Pixel coordinate to convert.
     * @return Converted coord.
     */
    template<typename T>
    Vector<T>
    relative01(const Vector<uint64_t>& pos) const;

    /**
     * Gets state of key.
     * @param key Key to check.
     * @return key state.
     */
    EState
    keyState(EKey key);

    /**
     * Gets state of mouse button.
     * @param button Button to check.
     * @return button state.
     */
    EState
    buttonState(EButton button);

/* ####################################################################################### */
protected: /* Window events */
/* ####################################################################################### */

    /**
     * Call this handler before window main loop started.
     */
    virtual void
    beforeLoop();

    /**
     * Window tick event handler.
     */
    virtual void
    tickEvent();

    /**
     * Window clear event. Called before 'renderEvent'.
     */
    virtual void
    clearEvent();

    /**
     * Window render event handler.
     */
    virtual void
    renderEvent();

    /**
     * Call this handler after window main loop finished.
     */
    virtual void
    afterLoop();

    /**
     * Resize event handler.
     */
    virtual void
    resizeEvent();

    /**
     * Key event handler.
     * @param key Input key.
     * @param state Key state.
     * @param modifier Modifier key pressed.
     */
    virtual void
    keyEvent(EKey key, EState state, EModifier modifier);

    /**
     * Mouse area event handler.
     * @param entered If it is True, mouse was entered to window area, otherwise mouse was leave.
     */
    virtual void
    mouseAreaEvent(bool entered);

    /**
     * Mouse move event handler.
     * @param position Mouse position.
     */
    virtual void
    mouseMoveEvent(Vector<uint64_t> position);

    /**
     * Mouse button event handler.
     * @param button Input button.
     * @param state Button state.
     * @param modifier Modifier key pressed.
     */
    virtual void
    buttonEvent(EButton button, EState state, EModifier modifier);

    /**
     * Scroll event handler.
     * @param offset Scroll offset.
     */
    virtual void
    scrollEvent(Vector<double> offset);

/* ####################################################################################### */
private: /* Internals */
/* ####################################################################################### */

    GLFWwindow*
    m_window {nullptr};

    MouseOffset
    m_prev_tick_mouse_pos {};

    double
    m_time = 0.0;

    double
    m_curr_tick = 0.0;

    double
    m_prev_tick = 0.0;

    const EOriginCorner
    m_originCorner;

    ChannelsBits
    m_channels {};

    std::string
    m_title {"Easy Window"};

    Size<uint64_t>
    m_size {1280, 720};

    uint32_t
    m_samples {0};

    bool
    m_visible {true};

    bool
    m_doubleBuffer {true};
};





template<typename T>
std::enable_if_t<std::is_floating_point_v<T>, T>
Window::aspect() const
{
    const auto [w,h] = size();
    return static_cast<T>(w) / T(h);
}

/* --------------------------------------------------------------------------------------- */

template<typename T>
Vector<T>
Window::relative(const Vector<uint64_t>& pos) const
{
    const auto [w,h] = size();

    return
    {
        fit<T>(T(pos.x), T(0), T(w), T(-1.0), T(1.0)),
        fit<T>(T(pos.y), T(0), T(h), T(-1.0), T(1.0))
    };
}

/* --------------------------------------------------------------------------------------- */

template<typename T>
Vector<T>
Window::relative01(const Vector<uint64_t>& pos) const
{
    const auto [w,h] = size();

    return
    {
        fit<T>(T(pos.x), T(0), T(w), T(0.0), T(1.0)),
        fit<T>(T(pos.y), T(0), T(h), T(0.0), T(1.0))
    };
}

EZWINDOW_NAMESPACE_END