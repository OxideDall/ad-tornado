#pragma once

#include "include/cef_client.h"
#include "ofMain.h"
#include "CefApp.h"
#include <list>
#include <mutex>
#include <vector>

class SimpleHandler : public CefClient,
                     public CefDisplayHandler,
                     public CefLifeSpanHandler,
                     public CefLoadHandler,
                     public CefRenderHandler {
public:
    explicit SimpleHandler(bool use_views);
    ~SimpleHandler();

    // Предоставляем доступ к единственному экземпляру
    static SimpleHandler* GetInstance();

    // Получаем экземпляр CefApp
    CefRefPtr<MyCefApp> GetCefApp() const { return cef_app_; }
    void SetCefApp(CefRefPtr<MyCefApp> app) { cef_app_ = app; }

    // CefClient методы
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
        return this;
    }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { 
        return this; 
    }
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return this;
    }

    // CefLifeSpanHandler методы
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // CefLoadHandler методы
    void OnLoadStart(CefRefPtr<CefBrowser> browser,
                    CefRefPtr<CefFrame> frame,
                    TransitionType transition_type) override;
    void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                  CefRefPtr<CefFrame> frame,
                  int httpStatusCode) override;
    void OnLoadError(CefRefPtr<CefBrowser> browser,
                    CefRefPtr<CefFrame> frame,
                    ErrorCode errorCode,
                    const CefString& errorText,
                    const CefString& failedUrl) override;

    // CefRenderHandler методы
    void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    void OnPaint(CefRefPtr<CefBrowser> browser,
                PaintElementType type,
                const RectList& dirtyRects,
                const void* buffer,
                int width,
                int height) override;

    void CloseAllBrowsers(bool force_close);
    bool IsClosing() const { return is_closing_; }

    // Texture data
    ofTexture& getTexture() { return webTexture; }
    void updateTexture(const void* buffer, int width, int height);
    bool isBrowserInitialized() const { return !browser_list_.empty(); }

private:
    const bool use_views_;
    bool is_closing_;
    std::mutex texture_mutex_;
    std::vector<unsigned char> pending_buffer_;
    bool has_pending_update_ = false;
    int pending_width_ = 0;
    int pending_height_ = 0;

    typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
    BrowserList browser_list_;

    static SimpleHandler* g_instance;

    ofTexture webTexture;
    CefRefPtr<MyCefApp> cef_app_;

    IMPLEMENT_REFCOUNTING(SimpleHandler);
    DISALLOW_COPY_AND_ASSIGN(SimpleHandler);
}; 