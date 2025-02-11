#include "web/CefBrowserHandler.h"
#include "ofLog.h"
#include <filesystem>
#include <iostream>
#include "include/wrapper/cef_helpers.h"

CefBrowserHandler::CefBrowserHandler()
    : viewWidth(1280), viewHeight(720), textureReady(false)
{
    rgbaBuffer.resize(viewWidth * viewHeight * 4);
}

CefBrowserHandler::~CefBrowserHandler()
{
    CloseBrowser();
}

void CefBrowserHandler::CloseBrowser()
{
    if (browser)
    {
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
        return;
    }

    try
    {
        std::lock_guard<std::mutex> lock(textureMutex);

        if (width != viewWidth || height != viewHeight)
        {
            ofLogNotice("CefBrowserHandler") << "OnPaint: resizing texture from " << viewWidth << "x" << viewHeight << " to " << width << "x" << height;
            viewWidth = width;
            viewHeight = height;
            rgbaBuffer.resize(width * height * 4);
            browserTexture.clear();
        }

        // Convert BGRA to RGBA silently
        const uint8_t *src = static_cast<const uint8_t *>(buffer);
        for (size_t i = 0; i < width * height * 4; i += 4)
        {
            rgbaBuffer[i + 0] = src[i + 2];
            rgbaBuffer[i + 1] = src[i + 1];
            rgbaBuffer[i + 2] = src[i + 0];
            rgbaBuffer[i + 3] = src[i + 3];
        }

        if (!browserTexture.isAllocated())
        {
            browserTexture.allocate(width, height, GL_RGBA);
        }

        browserTexture.loadData(rgbaBuffer.data(), width, height, GL_RGBA);
        textureReady = true;

        if (browser)
        {
            browser->GetHost()->SendExternalBeginFrame();
        }
    }
    catch (const std::exception &e)
    {
        ofLogError("CefBrowserHandler") << "Exception in OnPaint: " << e.what();
        textureReady = false;
    }
}

void CefBrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    this->browser = browser;
    browser->GetHost()->SendExternalBeginFrame();
}

bool CefBrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
    return false;
}

void CefBrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    CEF_REQUIRE_UI_THREAD();
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
                                    << " Error: " << errorText.ToString();
}

void CefBrowserHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  int httpStatusCode)
{
    CEF_REQUIRE_UI_THREAD();
    if (browser)
    {
        browser->GetHost()->WasResized();
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
    if (level == LOGSEVERITY_ERROR)
    {
        ofLogError("JavaScript") << message.ToString();
    }
    return true;
}

void CefBrowserHandler::setSize(int width, int height)
{
    std::lock_guard<std::mutex> lock(textureMutex);
    if (width != viewWidth || height != viewHeight)
    {
        viewWidth = width;
        viewHeight = height;
        rgbaBuffer.resize(width * height * 4);
        browserTexture.clear();
        if (browser)
        {
            browser->GetHost()->WasResized();
            browser->GetHost()->Invalidate(PET_VIEW);
            browser->GetHost()->SendExternalBeginFrame();
        }
    }
}
