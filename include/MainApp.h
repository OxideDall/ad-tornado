#pragma once

#include "ofMain.h"
#include "web/CefBrowserHandler.h"
#include <string>

class MainApp : public ofBaseApp
{
public:
    MainApp();
    ~MainApp();

    void setup();
    void update();
    void draw();
    void exit();

    void setHtmlPath(const std::string &path) { htmlPath = path; }
    void setVideoPath(const std::string &path) { videoPath = path; }

private:
    CefRefPtr<CefBrowserHandler> browserHandler;
    ofVideoPlayer videoPlayer;
    std::string htmlPath;
    std::string videoPath;
};