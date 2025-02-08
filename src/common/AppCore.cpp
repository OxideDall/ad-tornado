#include "common/AppCore.h"

AppCore::AppCore() 
    : bCEFInitialized(false)
    , bOFInitialized(false)
    , windowWidth(1280)
    , windowHeight(720)
{
}

AppCore::~AppCore() {
    shutdownCEF();
    shutdownOF();
}

void AppCore::setup() {
    // Инициализация OpenFrameworks
    if (!initOF()) {
        ofLogError("AppCore") << "Failed to initialize OpenFrameworks";
        return;
    }

    // Инициализация CEF
    if (!initCEF()) {
        ofLogError("AppCore") << "Failed to initialize CEF";
        return;
    }

    ofSetWindowTitle("AD Tornado");
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
}

void AppCore::update() {
    if (!bCEFInitialized || !bOFInitialized) {
        return;
    }

    // Обновление CEF
    CefDoMessageLoopWork();
}

void AppCore::draw() {
    if (!bCEFInitialized || !bOFInitialized) {
        return;
    }

    ofBackground(0);
    
    // Здесь будет отрисовка CEF и OpenFrameworks
}

void AppCore::exit() {
    shutdownCEF();
    shutdownOF();
}

void AppCore::windowResized(int w, int h) {
    windowWidth = w;
    windowHeight = h;

    if (browser) {
        // Обновляем размер окна браузера
        CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();
        if (hwnd) {
            // Здесь будет обновление размера окна браузера
        }
    }
}

void AppCore::keyPressed(int key) {
}

void AppCore::keyReleased(int key) {
}

void AppCore::mouseMoved(int x, int y) {
}

void AppCore::mouseDragged(int x, int y, int button) {
}

void AppCore::mousePressed(int x, int y, int button) {
}

void AppCore::mouseReleased(int x, int y, int button) {
}

bool AppCore::initCEF() {
    CefSettings settings;
    settings.no_sandbox = true;
    settings.multi_threaded_message_loop = false;
    
    // Инициализация CEF
    if (!CefInitialize(settings, nullptr)) {
        return false;
    }

    bCEFInitialized = true;
    return true;
}

void AppCore::shutdownCEF() {
    if (bCEFInitialized) {
        if (browser) {
            browser->GetHost()->CloseBrowser(true);
            browser = nullptr;
        }
        CefShutdown();
        bCEFInitialized = false;
    }
}

bool AppCore::initOF() {
    // Базовая настройка OpenFrameworks
    ofSetLogLevel(OF_LOG_NOTICE);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    
    bOFInitialized = true;
    return true;
}

void AppCore::shutdownOF() {
    if (bOFInitialized) {
        // Очистка ресурсов OpenFrameworks
        bOFInitialized = false;
    }
} 