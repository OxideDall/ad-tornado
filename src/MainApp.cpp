#include "MainApp.h"
#include <filesystem>
#include "include/cef_app.h"
#include "web/CefBrowserHandler.h"
#include <stdexcept>

MainApp::MainApp()
    : browserHandler(new CefBrowserHandler())
{
    ofLogNotice("MainApp") << "Constructor called";
}

MainApp::~MainApp()
{
    ofLogNotice("MainApp") << "Destructor called";
}

void MainApp::setup()
{
    // Convert paths to absolute
    std::string currentPath = std::filesystem::current_path().string();
    std::string absoluteVideoPath = std::filesystem::canonical(videoPath).generic_string();
    std::string absoluteHtmlPath = std::filesystem::canonical(htmlPath).generic_string();

    if (!std::filesystem::exists(absoluteVideoPath))
    {
        ofLogError("MainApp") << "Video file not found: " << absoluteVideoPath;
        throw std::runtime_error("Failed to load video");
    }

    if (!videoPlayer.load(absoluteVideoPath))
    {
        ofLogError("MainApp") << "Failed to load video";
        throw std::runtime_error("Failed to load video");
    }

    videoPlayer.play();
    videoPlayer.setLoopState(OF_LOOP_NORMAL);

    std::string url = "file://" + absoluteHtmlPath;
    browserHandler->setup(url);

    ofSetVerticalSync(true);
    ofSetFrameRate(60);
}

void MainApp::update()
{
    if (videoPlayer.isLoaded())
    {
        videoPlayer.update();
    }
    CefDoMessageLoopWork(); // Process CEF messages
}

void MainApp::draw()
{
    ofClear(0);

    float videoHeight = ofGetHeight() / 2;
    if (videoPlayer.isLoaded())
    {
        videoPlayer.draw(0, videoHeight, ofGetWidth(), videoHeight);
    }

    if (browserHandler && browserHandler->getTexture().isAllocated())
    {
        browserHandler->getTexture().draw(0, 0, ofGetWidth(), videoHeight);
    }
}

void MainApp::exit()
{
    browserHandler = nullptr;
}