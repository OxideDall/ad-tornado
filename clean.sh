#!/bin/bash

# Удаляем директорию сборки
rm -rf build/

# Очищаем CEF
cd ../cef-boilerplate
rm -rf build/

# Возвращаемся в исходную директорию
cd ../ad-tornado 