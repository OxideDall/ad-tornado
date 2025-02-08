#include "SimpleHandler.h"
#include <include/wrapper/cef_helpers.h>

SimpleHandler* SimpleHandler::g_instance = nullptr;

SimpleHandler::SimpleHandler(bool use_views) : use_views_(use_views) {
    g_instance = this;
    
    // Create fixed size texture
    webTexture.allocate(ofGetWidth(), ofGetHeight()/2, GL_RGBA);
}

SimpleHandler::~SimpleHandler() {
    g_instance = nullptr;
}

void SimpleHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) {
    rect.Set(0, 0, ofGetWidth(), ofGetHeight()/2);
}

void SimpleHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                          PaintElementType type,
                          const RectList& dirtyRects,
                          const void* buffer,
                          int width,
                          int height) {
    if (type == PET_VIEW && buffer && width > 0 && height > 0) {
        webTexture.loadData((const unsigned char*)buffer, width, height, GL_RGBA);
    }
}

void SimpleHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser) {
    browser_list_.push_back(browser);
}

void SimpleHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser) {
    for (auto it = browser_list_.begin(); it != browser_list_.end(); ++it) {
        if ((*it)->IsSame(browser)) {
            browser_list_.erase(it);
            break;
        }
    }
}

void SimpleHandler::CloseAllBrowsers(bool force_close) {
    for (auto& browser : browser_list_) {
        browser->GetHost()->CloseBrowser(force_close);
    }
} 