#pragma once

#include <string>
#include <vector>
#include <EasyWindow/Global.hpp>


class GLFWwindow;

EZWINDOW_NAMESPACE_BEGIN

struct ChannelsBits
{
    uint32_t r {8};     // red
    uint32_t g {8};     // green
    uint32_t b {8};     // blue
    uint32_t a {8};     // alpha
    uint32_t d {32};    // depth
    uint32_t s {16};    // stencil
};

class Surface
{

/* ####################################################################################### */
public: /* Constructors */
/* ####################################################################################### */

    Surface();

    virtual
    ~Surface();

/* ####################################################################################### */
public: /* Properties getters */
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
public: /* Methods */
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
protected: /* Window protected properties */
/* ####################################################################################### */

    GLFWwindow*
    m_window {nullptr};

/* ####################################################################################### */
private: /* Window private properties */
/* ####################################################################################### */

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

EZWINDOW_NAMESPACE_END