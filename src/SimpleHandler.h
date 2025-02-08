#ifndef CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_
#define CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_

#include "include/cef_client.h"
#include "ofMain.h"
#include <list>

class SimpleHandler : public CefClient,
                     public CefDisplayHandler,
                     public CefLifeSpanHandler,
                     public CefRenderHandler {
public:
    explicit SimpleHandler(bool use_views);
    ~SimpleHandler();

    // CefClient methods
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override {
        return this;
    }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override {
        return this;
    }
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override {
        return this;
    }

    // CefLifeSpanHandler methods
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

    // CefRenderHandler methods
    virtual void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
    virtual void OnPaint(CefRefPtr<CefBrowser> browser,
                        PaintElementType type,
                        const RectList& dirtyRects,
                        const void* buffer,
                        int width,
                        int height) override;

    void CloseAllBrowsers(bool force_close);

    ofTexture& getTexture() { return webTexture; }

private:
    bool use_views_;
    typedef std::list<CefRefPtr<CefBrowser>> BrowserList;
    BrowserList browser_list_;
    ofTexture webTexture;

    static SimpleHandler* g_instance;

    IMPLEMENT_REFCOUNTING(SimpleHandler);
    DISALLOW_COPY_AND_ASSIGN(SimpleHandler);
};

#endif  // CEF_TESTS_CEFSIMPLE_SIMPLE_HANDLER_H_ 