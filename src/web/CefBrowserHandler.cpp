#include "web/CefBrowserHandler.h"
#include "ofLog.h"
#include <filesystem>

CefBrowserHandler::CefBrowserHandler()
    : viewWidth(1280), viewHeight(360)
{
    pixelBuffer.resize(viewWidth * viewHeight * 4);
    ofLogNotice("CefBrowserHandler") << "Created with size: " << viewWidth << "x" << viewHeight;
}

CefBrowserHandler::~CefBrowserHandler()
{
    if (browser)
    {
        browser->GetHost()->CloseBrowser(true);
    }
}

void CefBrowserHandler::setup(const std::string &url)
{
    CefWindowInfo window_info;
    window_info.windowless_rendering_enabled = true;
    window_info.shared_texture_enabled = false;

    CefBrowserSettings browser_settings;
    browser_settings.windowless_frame_rate = 60;
    browser_settings.webgl = STATE_DISABLED;
    browser_settings.javascript = STATE_ENABLED;
    browser_settings.background_color = CefColorSetARGB(255, 51, 51, 51);

    browser = CefBrowserHost::CreateBrowserSync(
        window_info,
        this,
        url,
        browser_settings,
        nullptr,
        nullptr);

    if (!browser)
    {
        ofLogError("CefBrowserHandler") << "Failed to create browser";
        throw std::runtime_error("Failed to create browser");
    }

    browser->GetHost()->WasResized();
    browser->GetHost()->Invalidate(PET_VIEW);
}

void CefBrowserHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{
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

    std::lock_guard<std::mutex> lock(textureMutex);

    if (width != viewWidth || height != viewHeight)
    {
        ofLogWarning("CefBrowserHandler") << "Size mismatch: " << width << "x" << height
                                          << " vs " << viewWidth << "x" << viewHeight;
        return;
    }

    // Copy pixel data
    const uint8_t *src = static_cast<const uint8_t *>(buffer);
    std::memcpy(pixelBuffer.data(), src, width * height * 4);

    // Update texture
    if (!texture.isAllocated())
    {
        texture.allocate(width, height, GL_RGBA);
        ofLogNotice("CefBrowserHandler") << "Allocated texture: " << width << "x" << height;
    }

    texture.loadData(pixelBuffer.data(), width, height, GL_BGRA);
    ofLogNotice("CefBrowserHandler") << "Updated texture with new frame";
}

void CefBrowserHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    this->browser = browser;
    ofLogNotice("CefBrowserHandler") << "Browser created";

    // Force initial paint
    browser->GetHost()->WasResized();
    browser->GetHost()->Invalidate(PET_VIEW);
}

bool CefBrowserHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    return false;
}

void CefBrowserHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    this->browser = nullptr;
}