from common import cmake
from common.paths import PATHS

glfw = cmake.create('glfw')
glfw.options['GLFW_BUILD_DOCS'] = 'OFF'
glfw.options['GLFW_BUILD_EXAMPLES'] = 'OFF'
glfw.options['GLFW_BUILD_TESTS'] = 'OFF'
glfw.options['GLFW_INSTALL'] = 'ON'
glfw.options['GLFW_USE_HYBRID_HPG'] = 'OFF'
glfw.options['GLFW_VULKAN_STATIC'] = 'OFF'
glfw.options['USE_MSVC_RUNTIME_LIBRARY_DLL'] = 'ON'

glfw.setup()
