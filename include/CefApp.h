#pragma once

#include <include/cef_app.h>
#include "ofMain.h"

class MyCefApp : public CefApp,
                 public CefBrowserProcessHandler {
public:
    MyCefApp();
    virtual ~MyCefApp();

    // CefApp методы
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override {
        return this;
    }

    // CefBrowserProcessHandler методы
    virtual void OnContextInitialized() override;

private:
    IMPLEMENT_REFCOUNTING(MyCefApp);
    DISALLOW_COPY_AND_ASSIGN(MyCefApp);
}; 