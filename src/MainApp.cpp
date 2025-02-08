#include "MainApp.h"
#include "SimpleHandler.h"
#include <include/cef_app.h>
#include <filesystem>
#include <include/internal/cef_string.h>
#include <include/base/cef_logging.h>
#include <dlfcn.h>
#include <include/base/cef_build.h>
#include <include/cef_path_util.h>
#include <fstream>
#include <unistd.h>

MainApp::MainApp() {
    ofSetLogLevel(OF_LOG_NOTICE);
    ofLogNotice("MainApp") << "Creating MainApp";
}

void MainApp::setup() {
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofBackground(0);
    
    handler = new SimpleHandler(false);
    
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);
    
    CefBrowserSettings browser_settings;
    browser_settings.windowless_frame_rate = 60;
    
    // Get absolute path to HTML file
    std::string projectPath = std::filesystem::absolute(ofFilePath::getCurrentExeDir() + "/../..").string();
    std::string htmlPath = projectPath + "/assets/index.html";
    
    if (!std::filesystem::exists(htmlPath)) {
        ofLogError("MainApp") << "HTML file not found: " << htmlPath;
        return;
    }
    
    ofLogNotice("MainApp") << "Loading HTML file: " << htmlPath;
    std::string url = "file://" + htmlPath;
    
    browser = CefBrowserHost::CreateBrowserSync(window_info, handler.get(),
                                              url, browser_settings, nullptr, nullptr);
    
    if (!browser) {
        ofLogError("MainApp") << "Failed to create browser";
        return;
    }
    
    ofLogNotice("MainApp") << "Browser created successfully";
}

void MainApp::update() {
    CefDoMessageLoopWork();
}

void MainApp::draw() {
    ofSetColor(255);
    handler->getTexture().draw(0, 0);
}

void MainApp::exit() {
    handler->CloseAllBrowsers(true);
    browser = nullptr;
    handler = nullptr;
} 