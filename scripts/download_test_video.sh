#!/bin/bash

# Создаем директорию для видео, если она не существует
mkdir -p assets/video

# Загружаем тестовое видео с помощью ffmpeg
ffmpeg -f lavfi -i testsrc=duration=10:size=1280x720:rate=30 -vcodec libx264 assets/video/test.mp4

echo "Тестовое видео загружено в assets/video/test.mp4" 