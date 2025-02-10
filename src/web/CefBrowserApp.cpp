#include "web/CefBrowserApp.h"
#include "include/wrapper/cef_helpers.h"
#include "ofLog.h"

void CefBrowserApp::OnBeforeCommandLineProcessing(const CefString &process_type,
                                                  CefRefPtr<CefCommandLine> command_line)
{
    command_line->AppendSwitch("off-screen-rendering-enabled");
    command_line->AppendSwitch("disable-gpu");
    command_line->AppendSwitch("disable-gpu-compositing");
    command_line->AppendSwitch("disable-gpu-vsync");
    command_line->AppendSwitch("disable-gpu-process");
    command_line->AppendSwitch("in-process-gpu");
    command_line->AppendSwitch("disable-gpu-sandbox");
    command_line->AppendSwitch("disable-d3d11");
    command_line->AppendSwitch("disable-accelerated-2d-canvas");
    command_line->AppendSwitch("disable-accelerated-compositing");
    command_line->AppendSwitch("disable-accelerated-layers");
    command_line->AppendSwitch("disable-accelerated-plugins");
    command_line->AppendSwitch("disable-accelerated-video");
    command_line->AppendSwitch("disable-webgl");
    command_line->AppendSwitch("enable-begin-frame-scheduling");
    command_line->AppendSwitch("disable-smooth-scrolling");
    command_line->AppendSwitch("no-sandbox");
    command_line->AppendSwitch("disable-web-security");
    command_line->AppendSwitch("allow-file-access-from-files");
    command_line->AppendSwitch("allow-universal-access-from-files");
}

void CefBrowserApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();
    ofLogNotice("CefBrowserApp") << "Context initialized";
}

void CefBrowserApp::OnWebKitInitialized()
{
    CEF_REQUIRE_RENDERER_THREAD();
    ofLogNotice("CefBrowserApp") << "WebKit initialized";

    // Force a repaint
    if (browser)
    {
        browser->GetHost()->WasResized();
        browser->GetHost()->Invalidate(PET_VIEW);
    }
}

void CefBrowserApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefV8Context> context)
{
    CEF_REQUIRE_RENDERER_THREAD();
    ofLogNotice("CefBrowserApp") << "Context created";

    this->browser = browser;

    // Force a repaint
    browser->GetHost()->WasResized();
    browser->GetHost()->Invalidate(PET_VIEW);
}

bool CefBrowserApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             CefProcessId source_process,
                                             CefRefPtr<CefProcessMessage> message)
{
    CEF_REQUIRE_RENDERER_THREAD();
    ofLogNotice("CefBrowserApp") << "Process message received";
    return false;
}
