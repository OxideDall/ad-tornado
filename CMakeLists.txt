cmake_minimum_required(VERSION 3.16)
project(ad-tornado)

# Set C++ standard and compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add definitions to disable sound
add_definitions(
    -DOF_DISABLE_SOUND
    -DOF_USING_MPG123
    -DTARGET_NO_SOUND
)

# Paths to dependencies
set(CEF_ROOT "${CMAKE_SOURCE_DIR}/deps/cef")
set(OF_ROOT "${CMAKE_SOURCE_DIR}/deps/openFrameworks")

# Add CEF
include_directories(
    ${CEF_ROOT}
    ${CEF_ROOT}/include
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# Add link directories
link_directories(
    ${OF_ROOT}/libs/tess2/lib/linux64
    ${OF_ROOT}/libs/kiss/lib/linux64
    ${CEF_ROOT}/Release
    ${CEF_ROOT}/build/libcef_dll_wrapper
    /usr/lib/x86_64-linux-gnu
)

# Add OpenFrameworks
include_directories(
    ${OF_ROOT}/libs/openFrameworks
    ${OF_ROOT}/libs/openFrameworks/3d
    ${OF_ROOT}/libs/openFrameworks/app
    ${OF_ROOT}/libs/openFrameworks/communication
    ${OF_ROOT}/libs/openFrameworks/events
    ${OF_ROOT}/libs/openFrameworks/gl
    ${OF_ROOT}/libs/openFrameworks/graphics
    ${OF_ROOT}/libs/openFrameworks/math
    ${OF_ROOT}/libs/openFrameworks/sound
    ${OF_ROOT}/libs/openFrameworks/types
    ${OF_ROOT}/libs/openFrameworks/utils
    ${OF_ROOT}/libs/openFrameworks/video
    ${OF_ROOT}/libs/tess2/include
    ${OF_ROOT}/libs/utf8/include
    ${OF_ROOT}/libs/json/include
    ${OF_ROOT}/libs/kiss/include
    ${OF_ROOT}/libs/glew/include
    ${OF_ROOT}/libs/glfw/include
    ${OF_ROOT}/libs/freeimage/include
    ${OF_ROOT}/libs/freetype/include
    ${OF_ROOT}/libs/sndfile/include
    ${OF_ROOT}/libs/openal/include
    /usr/include/GL
    /usr/include/freetype2
    /usr/include/cairo
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
)

# Add source files
set(SOURCES
    src/main.cpp
    src/MainApp.cpp
    src/web/CefBrowserHandler.cpp
    src/web/CefBrowserApp.cpp
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OF_ROOT}/libs/openFrameworksCompiled/lib/linux64/libopenFrameworks.a
    ${CEF_ROOT}/Release/libcef.so
    ${CEF_ROOT}/build/libcef_dll_wrapper/libcef_dll_wrapper.a
    ${OF_ROOT}/libs/kiss/lib/linux64/libkiss.a
    -lGL
    -lGLEW
    -lglfw
    -lpthread
    -lX11
    -lXrandr
    -lXxf86vm
    -lXi
    -lXcursor
    -lfontconfig
    -lfreetype
    -lgstreamer-1.0
    -lgstapp-1.0
    -lgstvideo-1.0
    -lgstbase-1.0
    -lgobject-2.0
    -lglib-2.0
    -lfreeimage
    -luriparser
    -lcurl
    -ltess2
    -lopenal
    -lsndfile
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Copy CEF resources and libraries
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
    ${CMAKE_BINARY_DIR}/bin/locales
)

# Copy CEF Release files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CEF_ROOT}/Release/
    ${CMAKE_BINARY_DIR}/bin/
)

# Copy CEF Resources
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CEF_ROOT}/Resources/
    ${CMAKE_BINARY_DIR}/bin/
)

# Copy CEF locales
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CEF_ROOT}/Resources/locales/
    ${CMAKE_BINARY_DIR}/bin/locales/
)

# Copy data directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/data
    ${CMAKE_BINARY_DIR}/bin/data
)