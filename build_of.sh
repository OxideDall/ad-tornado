#!/bin/bash

# Проверяем, существует ли уже собранная библиотека
if [ -f "../external/openFrameworks/libs/openFrameworksCompiled/lib/linux64/libopenFrameworks.a" ]; then
    echo "OpenFrameworks уже собран"
    exit 0
fi

# Переходим в директорию OF
cd ../external/openFrameworks

# Собираем OF
make Release -j$(nproc)

# Возвращаемся в исходную директорию
cd ../../ad-tornado 