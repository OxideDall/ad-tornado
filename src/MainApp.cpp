#include "ofMain.h"
#include "MainApp.h"
#include <filesystem>
#include "include/cef_app.h"
#include "web/CefBrowserHandler.h"
#include <stdexcept>
#include <GLFW/glfw3.h>

MainApp::MainApp()
    : browserHandler(new CefBrowserHandler())
{
}

MainApp::~MainApp()
{
}

void MainApp::setup()
{
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetFullscreen(true);

    std::string absoluteVideoPath = std::filesystem::canonical(videoPath).generic_string();
    std::string absoluteHtmlPath = std::filesystem::canonical(htmlPath).generic_string();

    if (!std::filesystem::exists(absoluteVideoPath))
    {
        throw std::runtime_error("Failed to load video: " + absoluteVideoPath);
    }

    if (!videoPlayer.load(absoluteVideoPath))
    {
        throw std::runtime_error("Failed to load video");
    }

    videoPlayer.play();
    videoPlayer.setLoopState(OF_LOOP_NORMAL);

    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);
    window_info.shared_texture_enabled = false;
    window_info.external_begin_frame_enabled = true;

    browserHandler->setSize(ofGetWidth(), ofGetHeight() / 2);

    CefBrowserSettings browser_settings;
    browser_settings.windowless_frame_rate = 60;
    browser_settings.background_color = CefColorSetARGB(255, 255, 255, 255);
    browser_settings.webgl = STATE_ENABLED;
    browser_settings.javascript = STATE_ENABLED;
    browser_settings.local_storage = STATE_ENABLED;
    browser_settings.databases = STATE_ENABLED;

    std::string url = "file://" + absoluteHtmlPath;

    if (!CefBrowserHost::CreateBrowser(window_info, browserHandler, url, browser_settings, nullptr, nullptr))
    {
        throw std::runtime_error("Failed to create browser");
    }
}

void MainApp::update()
{
    if (videoPlayer.isLoaded())
    {
        videoPlayer.update();
    }

    // Process CEF messages
    CefDoMessageLoopWork();

    // Ensure smooth animations by sending multiple begin frame signals
    if (browserHandler && browserHandler->getBrowser())
    {
        for (int i = 0; i < 2; i++)
        { // Send multiple frames per update
            browserHandler->getBrowser()->GetHost()->SendExternalBeginFrame();
        }
    }
}

void MainApp::draw()
{
    ofClear(0);

    if (browserHandler && browserHandler->isTextureReady())
    {
        ofPushMatrix();
        ofSetColor(255);
        float browserHeight = ofGetHeight() / 2; // Take half of the screen
        browserHandler->getTexture().draw(0, 0, ofGetWidth(), browserHeight);
        ofPopMatrix();
    }

    if (videoPlayer.isLoaded())
    {
        videoPlayer.draw(0, ofGetHeight() / 2, ofGetWidth(), ofGetHeight() / 2);
    }
}

void MainApp::exit()
{
    if (browserHandler)
    {
        browserHandler->CloseBrowser();
    }
    browserHandler = nullptr;
}