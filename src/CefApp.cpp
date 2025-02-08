#include "CefApp.h"
#include <filesystem>
#include "include/wrapper/cef_helpers.h"

MyCefApp::MyCefApp() {
}

MyCefApp::~MyCefApp() {
}

void MyCefApp::OnContextInitialized() {
    CEF_REQUIRE_UI_THREAD();
} 