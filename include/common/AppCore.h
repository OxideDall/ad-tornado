#pragma once

#include "ofMain.h"
#include "include/cef_app.h"
#include "include/cef_client.h"

class AppCore : public ofBaseApp {
public:
    AppCore();
    ~AppCore();

    // Методы openFrameworks
    void setup() override;
    void update() override;
    void draw() override;
    void exit() override;

    // Методы для работы с окном
    void windowResized(int w, int h) override;
    void keyPressed(int key) override;
    void keyReleased(int key) override;
    void mouseMoved(int x, int y) override;
    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;

private:
    // Инициализация CEF
    bool initCEF();
    void shutdownCEF();

    // Инициализация OpenFrameworks
    bool initOF();
    void shutdownOF();

    // Флаги состояния
    bool bCEFInitialized;
    bool bOFInitialized;

    // Размеры окна
    int windowWidth;
    int windowHeight;

    // Указатели на объекты CEF
    CefRefPtr<CefApp> app;
    CefRefPtr<CefClient> client;
    CefRefPtr<CefBrowser> browser;

    IMPLEMENT_REFCOUNTING(AppCore);
}; 