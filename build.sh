#!/bin/bash

# Делаем скрипты исполняемыми
chmod +x build_cef.sh

# Собираем CEF если нужно
./build_cef.sh

# Создаем и переходим в директорию сборки
mkdir -p build
cd build

# Конфигурируем и собираем проект
cmake .. && make -j$(nproc) 