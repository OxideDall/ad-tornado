#pragma once

#include "include/cef_app.h"

class CefBrowserApp : public CefApp,
                      public CefBrowserProcessHandler,
                      public CefRenderProcessHandler
{
public:
    CefBrowserApp() = default;

    // CefApp methods
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override { return this; }
    virtual void OnBeforeCommandLineProcessing(const CefString &process_type,
                                               CefRefPtr<CefCommandLine> command_line) override;

    // CefBrowserProcessHandler methods
    virtual void OnContextInitialized() override;

    // CefRenderProcessHandler methods
    virtual void OnWebKitInitialized() override;
    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefV8Context> context) override;
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefRefPtr<CefFrame> frame,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) override;

private:
    CefRefPtr<CefBrowser> browser;
    IMPLEMENT_REFCOUNTING(CefBrowserApp);
    DISALLOW_COPY_AND_ASSIGN(CefBrowserApp);
};