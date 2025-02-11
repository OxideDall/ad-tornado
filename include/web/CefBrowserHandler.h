#pragma once

#include "include/cef_client.h"
#include "include/cef_render_handler.h"
#include "include/cef_load_handler.h"
#include "include/cef_display_handler.h"
#include "ofMain.h"
#include <mutex>
#include <vector>
#include "ofTexture.h"

class CefBrowserHandler : public CefClient,
                          public CefLifeSpanHandler,
                          public CefRenderHandler,
                          public CefLoadHandler,
                          public CefDisplayHandler
{
public:
    CefBrowserHandler();
    ~CefBrowserHandler();

    void setup(const std::string &url);
    void setSize(int width, int height);
    void CloseBrowser();
    bool isTextureReady() const { return textureReady; }
    int getWidth() const { return viewWidth; }
    int getHeight() const { return viewHeight; }
    CefRefPtr<CefBrowser> getBrowser() { return browser; }
    ofTexture &getTexture() { return browserTexture; }

    // CefClient methods
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }

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

    // CefLoadHandler methods
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             ErrorCode errorCode,
                             const CefString &errorText,
                             const CefString &failedUrl) override;
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           int httpStatusCode) override;

    // CefDisplayHandler methods
    virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                  cef_log_severity_t level,
                                  const CefString &message,
                                  const CefString &source,
                                  int line) override;

private:
    CefRefPtr<CefBrowser> browser;
    std::mutex textureMutex;
    std::vector<unsigned char> rgbaBuffer;
    int viewWidth;
    int viewHeight;
    bool textureReady;
    ofTexture browserTexture;

    IMPLEMENT_REFCOUNTING(CefBrowserHandler);
    DISALLOW_COPY_AND_ASSIGN(CefBrowserHandler);
};