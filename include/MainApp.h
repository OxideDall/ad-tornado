#pragma once

#include "ofMain.h"
#include "web/CefBrowserHandler.h"
#include "include/cef_app.h"

class MainApp : public ofBaseApp
{
public:
    MainApp();
    ~MainApp();

    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    void setHtmlPath(const std::string &path) { htmlPath = path; }
    void setVideoPath(const std::string &path) { videoPath = path; }

private:
    ofVideoPlayer videoPlayer;
    CefRefPtr<CefBrowserHandler> browserHandler;
    std::string htmlPath;
    std::string videoPath;
};