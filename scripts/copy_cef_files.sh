#!/bin/bash

# Получаем пути
CEF_ROOT="../cef-unpacked"
BUILD_DIR="build"
RELEASE_DIR="$BUILD_DIR/bin"

# Создаем необходимые директории
mkdir -p "$RELEASE_DIR"
mkdir -p "$RELEASE_DIR/locales"

# Копируем основные файлы
cp "$CEF_ROOT/Release/"*.so "$RELEASE_DIR/"
cp "$CEF_ROOT/Resources/"*.pak "$RELEASE_DIR/"
cp "$CEF_ROOT/Resources/icudtl.dat" "$RELEASE_DIR/"
cp "$CEF_ROOT/Release/v8_context_snapshot.bin" "$RELEASE_DIR/"
cp "$CEF_ROOT/Release/snapshot_blob.bin" "$RELEASE_DIR/"
cp "$CEF_ROOT/Release/chrome-sandbox" "$RELEASE_DIR/"

# Копируем локализации
cp "$CEF_ROOT/Resources/locales/"*.pak "$RELEASE_DIR/locales/"

# Устанавливаем правильные права
chmod 755 "$RELEASE_DIR/chrome-sandbox"
chmod 644 "$RELEASE_DIR/icudtl.dat"
chmod 644 "$RELEASE_DIR/"*.pak
chmod 644 "$RELEASE_DIR/locales/"*.pak
chmod 755 "$RELEASE_DIR/"*.so 