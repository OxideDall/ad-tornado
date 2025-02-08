cmake_minimum_required(VERSION 3.16)
project(cef_of_player)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к CEF и OpenFrameworks
set(CEF_ROOT "${CMAKE_SOURCE_DIR}/../cef-unpacked")
set(OF_ROOT "${CMAKE_SOURCE_DIR}/../openFrameworks")

# Добавление CEF
include_directories(
    ${CEF_ROOT}
    ${CEF_ROOT}/include
)

# Добавление путей к библиотекам
link_directories(
    ${OF_ROOT}/libs/tess2/lib/linux64
    ${CEF_ROOT}/Release
    ${CEF_ROOT}/build/libcef_dll_wrapper
    /usr/lib/x86_64-linux-gnu
)

# Добавление OpenFrameworks
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
    /usr/include/GL
    /usr/include/freetype2
    /usr/include/cairo
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
)

# Добавление исходных файлов
set(SOURCES
    src/main.cpp
    src/MainApp.cpp
    src/CefApp.cpp
    src/SimpleHandler.cpp
    kissfft/kiss_fft.c
    kissfft/kiss_fftr.c
)

# Добавление заголовочных файлов
set(HEADERS
    include/MainApp.h
    include/CefApp.h
)

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Включение директории с заголовочными файлами
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Линковка с CEF
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${CEF_ROOT}/Release/libcef.so
    ${CEF_ROOT}/build/libcef_dll_wrapper/libcef_dll_wrapper.a
)

# Линковка с OpenFrameworks и зависимостями
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OF_ROOT}/libs/openFrameworksCompiled/lib/linux64/libopenFrameworks.a
    ${OF_ROOT}/libs/tess2/lib/linux64/libtess2.a
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
    -lopenal
    -lsndfile
)

# Копирование ресурсов
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    ${CMAKE_BINARY_DIR}/assets
)

# Установка выходной директории
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_definitions(
    -DOF_DISABLE_SOUND
    -DOF_USING_MPG123
    -DTARGET_NO_SOUND
    -Dkiss_fft_scalar=float
)

include_directories(
    ${PROJECT_SOURCE_DIR}/../cef-unpacked
    ${PROJECT_SOURCE_DIR}/../cef-unpacked/include
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/3d
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/app
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/communication
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/events
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/gl
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/graphics
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/math
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/sound
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/types
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/utils
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/openFrameworks/video
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/tess2/include
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/utf8/include
    ${PROJECT_SOURCE_DIR}/../openFrameworks/libs/json/include
    ${PROJECT_SOURCE_DIR}/kissfft
    /usr/include/GL
    /usr/include/freetype2
    /usr/include/cairo
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
    ${PROJECT_SOURCE_DIR}/include
)

# Добавляем библиотеку dl
target_link_libraries(${PROJECT_NAME} PRIVATE dl) 