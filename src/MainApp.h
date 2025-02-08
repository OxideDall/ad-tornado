#pragma once

#include "ofMain.h"
#include "include/cef_app.h"
#include "SimpleHandler.h"

class MainApp : public ofBaseApp {
public:
    MainApp();
    
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

private:
    CefRefPtr<SimpleHandler> handler;
    CefRefPtr<CefBrowser> browser;
}; 