#pragma once

#include "include/cef_client.h"
#include "ofMain.h"
#include <mutex>
#include <vector>

class CefBrowserHandler : public CefClient,
                          public CefLifeSpanHandler,
                          public CefRenderHandler
{
public:
    CefBrowserHandler();
    ~CefBrowserHandler();

    void setup(const std::string &url);
    void resize(int width, int height);
    ofTexture &getTexture() { return texture; }

    // CefClient methods
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }

    // CefRenderHandler methods
    virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
    virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                         PaintElementType type,
                         const RectList &dirtyRects,
                         const void *buffer,
                         int width,
                         int height) override;

    // CefLifeSpanHandler methods
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

private:
    CefRefPtr<CefBrowser> browser;
    ofTexture texture;
    std::mutex textureMutex;
    std::vector<unsigned char> pixelBuffer;
    int viewWidth;
    int viewHeight;

    IMPLEMENT_REFCOUNTING(CefBrowserHandler);
};