#include "MainApp.h"
#include <filesystem>
#include "include/cef_app.h"
#include "web/CefBrowserHandler.h"
#include <stdexcept>
#include <GLFW/glfw3.h>
#include <chrono>

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
    ofLogNotice("MainApp") << "Setup started";

    // Setup OpenGL first
    ofSetVerticalSync(true);
    ofSetFrameRate(60);

    // Check OpenGL context
    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    ofLogNotice("MainApp") << "OpenGL version: " << major << "." << minor;

    // Convert paths to absolute
    std::string currentPath = std::filesystem::current_path().string();
    std::string absoluteVideoPath = std::filesystem::canonical(videoPath).generic_string();
    std::string absoluteHtmlPath = std::filesystem::canonical(htmlPath).generic_string();

    ofLogNotice("MainApp") << "Current path: " << currentPath;
    ofLogNotice("MainApp") << "Absolute HTML path: " << absoluteHtmlPath;
    ofLogNotice("MainApp") << "HTML file exists: " << (std::filesystem::exists(absoluteHtmlPath) ? "Yes" : "No");

    // Setup video player first
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

    ofLogNotice("MainApp") << "Video loaded: " << absoluteVideoPath;
    videoPlayer.play();
    videoPlayer.setLoopState(OF_LOOP_NORMAL);

    // Setup browser last
    CefWindowInfo window_info;
    window_info.SetAsWindowless(0);
    window_info.shared_texture_enabled = false;
    window_info.external_begin_frame_enabled = true;

    CefBrowserSettings browser_settings;
    browser_settings.windowless_frame_rate = 60;
    browser_settings.background_color = CefColorSetARGB(255, 255, 255, 255);
    browser_settings.webgl = STATE_DISABLED;
    browser_settings.javascript = STATE_ENABLED;
    browser_settings.local_storage = STATE_ENABLED;
    browser_settings.databases = STATE_ENABLED;

    std::string url = "file://" + absoluteHtmlPath;
    ofLogNotice("MainApp") << "Loading URL: " << url;

    // Create browser asynchronously
    if (!CefBrowserHost::CreateBrowser(
            window_info,
            browserHandler,
            url,
            browser_settings,
            nullptr,
            nullptr))
    {
        ofLogError("MainApp") << "Failed to create browser";
        return;
    }

    ofLogNotice("MainApp") << "Browser creation initiated";
    ofLogNotice("MainApp") << "Setup completed";
}

void MainApp::update()
{
    if (videoPlayer.isLoaded())
    {
        videoPlayer.update();
    }

    // Process CEF messages in single-threaded mode
    CefDoMessageLoopWork();

    // Request frame from CEF browser
    if (browserHandler && browserHandler->getBrowser())
    {
        browserHandler->getBrowser()->GetHost()->SendExternalBeginFrame();
    }

    // Update browser texture if needed
    if (browserHandler)
    {
        browserHandler->updateTexture();

        // Log browser status periodically
        static int frameCount = 0;
        if (++frameCount % 60 == 0) // Every ~1 second
        {
            if (browserHandler->getBrowser())
            {
                ofLogNotice("MainApp") << "Browser is active, texture ready: " << (browserHandler->isTextureReady() ? "Yes" : "No");
            }
            else
            {
                ofLogNotice("MainApp") << "Browser is not active";
            }
        }
    }
}

void MainApp::draw()
{
    ofClear(0);

    float videoHeight = ofGetHeight() / 2;
    if (videoPlayer.isLoaded())
    {
        videoPlayer.draw(0, videoHeight, ofGetWidth(), videoHeight);
    }

    if (browserHandler && browserHandler->isTextureReady())
    {
        ofLogVerbose("MainApp") << "Drawing browser texture";

        // Use OF's built-in methods for drawing textures
        ofPushMatrix();
        ofSetColor(255);

        // Create a temporary texture wrapper
        ofTexture tex;
        tex.setUseExternalTextureID(browserHandler->getTextureId());
        tex.texData.width = browserHandler->getWidth();
        tex.texData.height = browserHandler->getHeight();
        tex.texData.tex_w = browserHandler->getWidth();
        tex.texData.tex_h = browserHandler->getHeight();
        tex.texData.textureTarget = GL_TEXTURE_2D;
        tex.texData.glInternalFormat = GL_RGBA8;

        // Draw the texture using OF's methods
        tex.draw(0, 0, ofGetWidth(), videoHeight);

        ofPopMatrix();
    }
    else
    {
        ofLogNotice("MainApp") << "Browser texture not ready";
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