#pragma once

#include "ofMain.h"

class VideoPlayer {
public:
    VideoPlayer();
    ~VideoPlayer();

    void setup();
    void update();
    void draw(int x, int y, int width, int height);

    // Управление воспроизведением
    void play();
    void pause();
    void stop();
    void togglePause();
    void setPosition(float pct);
    void setSpeed(float speed);
    void nextFrame();
    void previousFrame();
    void firstFrame();

    // Загрузка видео
    bool loadVideo(const std::string& path);
    
    // Получение информации о видео
    float getPosition() const;
    float getDuration() const;
    int getCurrentFrame() const;
    int getTotalNumFrames() const;
    bool isPlaying() const;
    bool isDone() const;
    float getSpeed() const;
    
    // Настройки воспроизведения
    void setLoopState(ofLoopType state);
    void setVolume(float vol);
    
    // Получение размеров видео
    int getWidth() const;
    int getHeight() const;

private:
    ofVideoPlayer player;
    bool frameByframe;
}; 