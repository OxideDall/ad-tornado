#pragma once

#include "ofMain.h"
#include "CefApp.h"
#include "SimpleHandler.h"

class MainApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();
    void exit();

    void keyPressed(int key);
    void windowResized(int w, int h);
    
    void setCefApp(CefRefPtr<MyCefApp> app) { cefApp = app; }
    void setMainArgs(int argc, char** argv) { 
        this->argc = argc;
        this->argv = argv;
    }

private:
    // CEF
    CefRefPtr<MyCefApp> cefApp;
    CefRefPtr<SimpleHandler> handler;
    bool cefInitialized;
    int argc;
    char** argv;

    // Video
    ofVideoPlayer videoPlayer;
    bool videoLoaded;

    // Window dimensions
    int windowWidth;
    int windowHeight;
}; 