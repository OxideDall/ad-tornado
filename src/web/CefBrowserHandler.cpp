#include "web/CefBrowserHandler.h"
#include "ofLog.h"
#include <filesystem>
#include <iostream>
#include "include/wrapper/cef_helpers.h"

CefBrowserHandler::CefBrowserHandler()
    : viewWidth(1280), viewHeight(720), needsTextureUpdate(false), textureId(0), textureReady(false)
{
    ofLogNotice("CefBrowserHandler") << "Created with size: " << viewWidth << "x" << viewHeight;
    pixelBuffer.resize(viewWidth * viewHeight * 4);
}

CefBrowserHandler::~CefBrowserHandler()
{
    ofLogNotice("CefBrowserHandler") << "Destructor called";
    deleteTexture();
    CloseBrowser();
}

void CefBrowserHandler::createTexture()
{
    CEF_REQUIRE_UI_THREAD();

    if (textureId != 0)
    {
        deleteTexture();
    }

    // Save current OpenGL state
    GLint last_texture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

    // Create and initialize texture
    glGenTextures(1, &textureId);
    if (textureId == 0)
    {
        ofLogError("CefBrowserHandler") << "Failed to generate texture";
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Initialize texture storage with RGBA format
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, viewWidth, viewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    // Check for errors
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        ofLogError("CefBrowserHandler") << "OpenGL error during texture creation: " << err;
        deleteTexture();
    }
    else
    {
        ofLogNotice("CefBrowserHandler") << "Texture created successfully: " << textureId;
        textureReady = true;
    }

    // Restore previous texture binding
    glBindTexture(GL_TEXTURE_2D, last_texture);
}

void CefBrowserHandler::deleteTexture()
{
    CEF_REQUIRE_UI_THREAD();

    if (textureId != 0)
    {
        glDeleteTextures(1, &textureId);
        textureId = 0;
        textureReady = false;
        ofLogNotice("CefBrowserHandler") << "Texture deleted";
    }
}

void CefBrowserHandler::CloseBrowser()
{
    if (browser)
    {
        ofLogNotice("CefBrowserHandler") << "Closing browser";
        browser->GetHost()->CloseBrowser(true);
        browser = nullptr;
    }
}

void CefBrowserHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
    CEF_REQUIRE_UI_THREAD();

    rect.x = 0;
    rect.y = 0;
    rect.width = viewWidth;
    rect.height = viewHeight;
    ofLogNotice("CefBrowserHandler") << "GetViewRect called: " << viewWidth << "x" << viewHeight;
}

void CefBrowserHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                                PaintElementType type,
                                const RectList &dirtyRects,
                                const void *buffer,
                                int width,
                                int height)
{
    CEF_REQUIRE_UI_THREAD();

    if (!buffer || width <= 0 || height <= 0)
    {
        ofLogWarning("CefBrowserHandler") << "OnPaint called with invalid buffer or dimensions";
        return;
    }

    try
    {
        static int frameCount = 0;
        frameCount++;

        // Log every 30th frame to avoid spam
        if (frameCount % 30 == 0)
        {
            const uint8_t *pixels = static_cast<const uint8_t *>(buffer);
            ofLogNotice("CefBrowserHandler") << "OnPaint frame " << frameCount
                                             << ", first pixel (BGRA): "
                                             << (int)pixels[0] << ", "
                                             << (int)pixels[1] << ", "
                                             << (int)pixels[2] << ", "
                                             << (int)pixels[3];
        }

        std::lock_guard<std::mutex> lock(textureMutex);

        if (width != viewWidth || height != viewHeight)
        {
            ofLogNotice("CefBrowserHandler") << "Resizing buffer from " << viewWidth << "x" << viewHeight << " to " << width << "x" << height;
            viewWidth = width;
            viewHeight = height;
            pixelBuffer.resize(width * height * 4);
            deleteTexture();
        }

        // Create texture if needed
        if (textureId == 0)
        {
            createTexture();
        }

        // Convert BGRA to RGBA directly and update texture
        std::vector<unsigned char> rgbaBuffer(width * height * 4);
        const uint8_t *src = static_cast<const uint8_t *>(buffer);

        for (size_t i = 0; i < width * height; i++)
        {
            size_t srcIdx = i * 4;
            size_t dstIdx = i * 4;
            rgbaBuffer[dstIdx + 0] = src[srcIdx + 2]; // R = B
            rgbaBuffer[dstIdx + 1] = src[srcIdx + 1]; // G = G
            rgbaBuffer[dstIdx + 2] = src[srcIdx + 0]; // B = R
            rgbaBuffer[dstIdx + 3] = src[srcIdx + 3]; // A = A
        }

        // Update texture immediately
        glBindTexture(GL_TEXTURE_2D, textureId);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rgbaBuffer.data());

        // Save current state
        GLint lastTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &lastTexture);

        // Ensure texture update is complete
        glFinish();

        // Restore state
        glBindTexture(GL_TEXTURE_2D, lastTexture);

        // Request next frame at a reasonable rate
        if (browser)
        {
            browser->GetHost()->SendExternalBeginFrame();
        }
    }
    catch (const std::exception &e)
    {
        ofLogError("CefBrowserHandler") << "Exception in OnPaint: " << e.what();
    }
}

void CefBrowserHandler::updateTexture()
{
    // This method is now empty as we update texture directly in OnPaint
}

void CefBrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();

    ofLogNotice("CefBrowserHandler") << "OnAfterCreated called";
    this->browser = browser;

    // Schedule initial paint for next update
    needsTextureUpdate = true;
    ofLogNotice("CefBrowserHandler") << "Initial paint scheduled";
}

bool CefBrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    ofLogNotice("CefBrowserHandler") << "DoClose called";
    return false;
}

void CefBrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    ofLogNotice("CefBrowserHandler") << "OnBeforeClose called";
    this->browser = nullptr;
}

void CefBrowserHandler::OnLoadError(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    ErrorCode errorCode,
                                    const CefString &errorText,
                                    const CefString &failedUrl)
{
    CEF_REQUIRE_UI_THREAD();
    ofLogError("CefBrowserHandler") << "Failed to load URL: " << failedUrl.ToString()
                                    << " Error: " << errorText.ToString()
                                    << " Code: " << errorCode;
}

void CefBrowserHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  int httpStatusCode)
{
    CEF_REQUIRE_UI_THREAD();
    ofLogNotice("CefBrowserHandler") << "Page loaded with status code: " << httpStatusCode;

    // Force a repaint after page load
    if (browser)
    {
        browser->GetHost()->Invalidate(PET_VIEW);
        browser->GetHost()->SendExternalBeginFrame();
    }
}

bool CefBrowserHandler::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                         cef_log_severity_t level,
                                         const CefString &message,
                                         const CefString &source,
                                         int line)
{
    std::string levelStr;
    switch (level)
    {
    case LOGSEVERITY_DEBUG:
        levelStr = "DEBUG";
        break;
    case LOGSEVERITY_INFO:
        levelStr = "INFO";
        break;
    case LOGSEVERITY_WARNING:
        levelStr = "WARNING";
        break;
    case LOGSEVERITY_ERROR:
        levelStr = "ERROR";
        break;
    default:
        levelStr = "UNKNOWN";
    }

    ofLogNotice("JavaScript") << "[" << levelStr << "] "
                              << "Console message from " << source.ToString()
                              << " (line " << line << "): "
                              << message.ToString();
    return true;
}
