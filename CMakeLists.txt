cmake_minimum_required(VERSION 3.17)

project(EasyWindow
    LANGUAGES CXX
    VERSION 1.2.2
)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_custom_target(
    build-dependencies ALL
    COMMAND "${Python3_EXECUTABLE}" dependencies.py
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/scripts"
    COMMENT "Build all dependencies"
    VERBATIM
)

# ####################################################################################### #
# Library params
# ####################################################################################### #

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    set(EZWINDOW_RENDER_BACKEND "Vulkan" CACHE STRING "Render backend name (Vulkan, OpenGL.")
    set_property(CACHE EZWINDOW_RENDER_BACKEND PROPERTY STRINGS "Vulkan;OpenGL")
    message("[${PROJECT_NAME}]: supported render backends [Vulkan, OpenGL]")
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
    set(EZWINDOW_RENDER_BACKEND "Vulkan" CACHE STRING "Render backend name (Vulkan, OpenGL.")
    set_property(CACHE EZWINDOW_RENDER_BACKEND PROPERTY STRINGS "Vulkan;OpenGL")
    message("[${PROJECT_NAME}]: supported render backends [Vulkan, OpenGL]")
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    set(EZWINDOW_RENDER_BACKEND "Metal" CACHE STRING "Render backend name (Vulkan, OpenGL.")
    set_property(CACHE EZWINDOW_RENDER_BACKEND PROPERTY STRINGS "Vulkan;Metal;OpenGL")
    message("[${PROJECT_NAME}]: supported render backends [Vulkan, OpenGL]")
endif()

# ####################################################################################### #
# Target initialization
# ####################################################################################### #

file(GLOB_RECURSE HEADERS_FILES ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp)
file(GLOB_RECURSE SOURCES_FILES ${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp)

# ####################################################################################### #
# Target configuration
# ####################################################################################### #

add_library(${PROJECT_NAME} ${SOURCES_FILES} ${HEADERS_FILES})

add_dependencies(${PROJECT_NAME} build-dependencies)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD                17
    CXX_STANDARD_REQUIRED       YES
    CXX_EXTENSIONS              NO
    RUNTIME_OUTPUT_DIRECTORY    "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY    "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY    "${CMAKE_BINARY_DIR}/lib"
    DEBUG_POSTFIX               d
)

set(${PROJECT_NAME}_dependencies glfw)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EZWINDOW_WINDOWS)
    message("[${PROJECT_NAME}]: build on Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EZWINDOW_LINUX)
    message("[${PROJECT_NAME}]: build on Linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EZWINDOW_OSX)
    message("[${PROJECT_NAME}]: build on OSX")
endif()

if(EZWINDOW_RENDER_BACKEND STREQUAL Vulkan)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EZWINDOW_VULKAN)
    message("[${PROJECT_NAME}]: render backend is Vulkan")
    find_package(Vulkan REQUIRED)
    list(APPEND ${PROJECT_NAME}_dependencies Vulkan::Vulkan)
elseif(EZWINDOW_RENDER_BACKEND STREQUAL Metal)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EZWINDOW_METAL)
    message("[${PROJECT_NAME}]: render backend is Metal")
elseif(EZWINDOW_RENDER_BACKEND STREQUAL OpenGL)
    target_compile_definitions(${PROJECT_NAME} PUBLIC EZWINDOW_OPENGL)
    list(APPEND ${PROJECT_NAME}_dependencies GLEW::GLEW)
    find_package(GLEW REQUIRED)
    message("[${PROJECT_NAME}]: render backend is OpenGL")
else()
    message(FATAL_ERROR "[${PROJECT_NAME}]: invalid value of <EZWINDOW_RENDER_BACKEND> configuration property. It must be <Vulkan/Metal/OpenGL>.")
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/deps")
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ${${PROJECT_NAME}_dependencies}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
)

# ####################################################################################### #
# Installation
# ####################################################################################### #

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    DIRECTORY include
    DESTINATION ${CMAKE_INSTALL_PREFIX}
)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    INCLUDES DESTINATION ${CMAKE_INSTALL_PREFIX}/include
)

install(
    EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    FILE ${PROJECT_NAME}Config.cmake
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}
)

install(
    FILES "${CMAKE_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/cmake/${PROJECT_NAME}
)
