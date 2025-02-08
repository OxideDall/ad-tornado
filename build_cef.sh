#!/bin/bash

# Проверяем, существует ли уже собранная библиотека
if [ -f "../cef-boilerplate/build/libcef_dll_wrapper/libcef_dll_wrapper.a" ]; then
    echo "CEF уже собран"
    exit 0
fi

# Переходим в директорию CEF
cd ../cef-boilerplate

# Создаем и переходим в директорию сборки
mkdir -p build
cd build

# Конфигурируем проект с помощью CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

# Собираем проект
make -j$(nproc)

# Возвращаемся в исходную директорию
cd ../../ad-tornado 