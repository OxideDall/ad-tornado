#include "ofMain.h"
#include "MainApp.h"
#include "include/cef_app.h"
#include "web/CefBrowserApp.h"
#include <filesystem>
#include <thread>
#include <chrono>

void printUsage()
{
    std::cout << "Usage: " << std::endl;
    std::cout << "  --html <path>  : Path to HTML file (default: data/html/index.html)" << std::endl;
    std::cout << "  --video <path> : Path to video file (default: data/video/test.mp4)" << std::endl;
}

int main(int argc, char *argv[])
{
    // Check if this is the main process
    CefMainArgs main_args(argc, argv);
    CefRefPtr<CefBrowserApp> app(new CefBrowserApp());

    // Execute sub-process if needed
    int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
    if (exit_code >= 0)
    {
        // This is a sub-process, just return
        return exit_code;
    }

    // Main process continues here
    ofLogNotice("Main") << "Starting application in main process...";

    // Add CEF command line arguments to disable GPU
    std::vector<std::string> cef_args = {
        "--disable-gpu",
        "--disable-gpu-compositing",
        "--disable-gpu-vsync",
        "--disable-accelerated-2d-canvas",
        "--disable-accelerated-video-decode",
        "--disable-webgl"};

    ofLogNotice("Main") << "Setting up CEF arguments...";
    // Create new argv with CEF arguments
    std::vector<const char *> new_argv;
    new_argv.push_back(argv[0]); // Program name
    for (const auto &arg : cef_args)
    {
        new_argv.push_back(arg.c_str());
        ofLogNotice("Main") << "Added CEF arg: " << arg;
    }
    for (int i = 1; i < argc; ++i)
    {
        new_argv.push_back(argv[i]);
    }
    int new_argc = new_argv.size();

    // Parse command line arguments
    std::string htmlPath = "data/html/index.html";
    std::string videoPath = "data/video/test.mp4";

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--html" && i + 1 < argc)
        {
            htmlPath = argv[++i];
            ofLogNotice("Main") << "HTML path set to: " << htmlPath;
        }
        else if (arg == "--video" && i + 1 < argc)
        {
            videoPath = argv[++i];
            ofLogNotice("Main") << "Video path set to: " << videoPath;
        }
        else if (arg == "--help" || arg == "-h")
        {
            printUsage();
            return 0;
        }
    }

    ofLogNotice("Main") << "Initializing CEF...";
    // Initialize CEF with modified arguments
    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;
    settings.external_message_pump = true;
    settings.multi_threaded_message_loop = false;
    settings.command_line_args_disabled = false;
    settings.background_color = 0;

    // Set paths
    std::string exePath = std::filesystem::canonical("/proc/self/exe").parent_path().string();
    CefString(&settings.resources_dir_path) = exePath;
    CefString(&settings.locales_dir_path) = exePath + "/locales";
    CefString(&settings.cache_path) = exePath + "/cache";

    ofLogNotice("Main") << "CEF paths set:";
    ofLogNotice("Main") << "  Resources: " << exePath;
    ofLogNotice("Main") << "  Locales: " << exePath + "/locales";
    ofLogNotice("Main") << "  Cache: " << exePath + "/cache";

    // Create CEF app
    CefRefPtr<CefBrowserApp> cef_app(new CefBrowserApp());
    ofLogNotice("Main") << "CEF app created";

    // Initialize CEF first
    if (!CefInitialize(main_args, settings, cef_app.get(), nullptr))
    {
        ofLogError("Main") << "Failed to initialize CEF";
        return 1;
    }
    ofLogNotice("Main") << "CEF initialized successfully";

    ofLogNotice("Main") << "Creating OpenGL window...";
    // Create OpenGL window
    ofGLFWWindowSettings windowSettings;
    windowSettings.setSize(1280, 720);
    windowSettings.windowMode = OF_WINDOW;
    windowSettings.setGLVersion(3, 2);
    ofCreateWindow(windowSettings);
    ofLogNotice("Main") << "OpenGL window created";

    // Create and start app
    ofLogNotice("Main") << "Creating main application...";
    auto app_main = make_shared<MainApp>();
    app_main->setHtmlPath(htmlPath);
    app_main->setVideoPath(videoPath);

    ofLogNotice("Main") << "Starting OF application...";
    // Run OF app
    int result = ofRunApp(app_main);

    ofLogNotice("Main") << "Application finished, cleaning up...";
    // Cleanup CEF
    CefShutdown();
    ofLogNotice("Main") << "CEF shutdown complete";
    return result;
}