#include "ofMain.h"
#include "MainApp.h"
#include "CefApp.h"
#include <include/cef_app.h>
#include <filesystem>

int main(int argc, char* argv[]) {
    // Get absolute path to executable directory
    std::string exePath = std::filesystem::absolute(ofFilePath::getCurrentExeDir()).string();
    
    // Initialize CEF
    CefMainArgs main_args(argc, argv);
    CefRefPtr<MyCefApp> app(new MyCefApp());
    
    int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
    if (exit_code >= 0) {
        return exit_code;
    }
    
    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;
    
    // Set CEF resource paths
    CefString(&settings.resources_dir_path) = exePath;
    CefString(&settings.locales_dir_path) = exePath + "/locales";
    CefString(&settings.framework_dir_path) = exePath;
    
    // Set ICU data path
    std::string icuPath = exePath + "/icudtl.dat";
    if (!std::filesystem::exists(icuPath)) {
        ofLogError("main") << "ICU file not found at: " << icuPath;
        return 1;
    }
    ofLogNotice("main") << "Using ICU file at: " << icuPath;
    CefString(&settings.resources_dir_path).FromString(exePath);
    
    if (!CefInitialize(main_args, settings, app.get(), nullptr)) {
        ofLogError("main") << "Failed to initialize CEF";
        return 1;
    }
    
    // Create OF window
    ofSetupOpenGL(1024, 768, OF_WINDOW);
    
    // Run application
    auto mainApp = std::make_shared<MainApp>();
    ofRunApp(mainApp);
    
    CefShutdown();
    return 0;
} 