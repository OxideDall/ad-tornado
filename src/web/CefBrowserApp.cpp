#include "web/CefBrowserApp.h"
#include "include/wrapper/cef_helpers.h"
#include "ofLog.h"
#include "include/base/cef_bind.h"

void CefBrowserApp::OnBeforeCommandLineProcessing(const CefString &process_type,
                                                  CefRefPtr<CefCommandLine> command_line)
{
    command_line->AppendSwitch("off-screen-rendering-enabled");
    command_line->AppendSwitch("in-process-gpu");
    command_line->AppendSwitch("no-sandbox");
    command_line->AppendSwitch("disable-web-security");
    command_line->AppendSwitch("allow-file-access-from-files");
    command_line->AppendSwitch("allow-universal-access-from-files");
    command_line->AppendSwitch("disable-extensions");
    command_line->AppendSwitch("disable-plugins");
    command_line->AppendSwitch("disable-pdf-extension");
    command_line->AppendSwitch("disable-pinch");
    command_line->AppendSwitch("enable-begin-frame-scheduling");
}

void CefBrowserApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();
    ofLogNotice("CefBrowserApp") << "Context initialized";
}

void CefBrowserApp::OnWebKitInitialized()
{
    ofLogNotice("CefBrowserApp") << "WebKit initialized";
}

void CefBrowserApp::OnContextCreated(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefV8Context> context)
{
    ofLogNotice("CefBrowserApp") << "Context created";

    // Store browser reference
    if (!this->browser)
    {
        this->browser = browser;
    }
}

bool CefBrowserApp::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                             CefRefPtr<CefFrame> frame,
                                             CefProcessId source_process,
                                             CefRefPtr<CefProcessMessage> message)
{
    ofLogNotice("CefBrowserApp") << "Process message received";
    return false;
}
