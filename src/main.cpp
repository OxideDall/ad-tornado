#include "ofMain.h"
#include "MainApp.h"
#include "include/cef_app.h"
#include "web/CefBrowserApp.h"
#include <filesystem>

void printUsage()
{
    std::cout << "Usage: " << std::endl;
    std::cout << "  --html <path>  : Path to HTML file (default: data/html/index.html)" << std::endl;
    std::cout << "  --video <path> : Path to video file (default: data/video/test.mp4)" << std::endl;
}

int main(int argc, char *argv[])
{
    // Parse command line arguments
    std::string htmlPath = "data/html/index.html";
    std::string videoPath = "data/video/test.mp4";

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--html" && i + 1 < argc)
        {
            htmlPath = argv[++i];
        }
        else if (arg == "--video" && i + 1 < argc)
        {
            videoPath = argv[++i];
        }
        else if (arg == "--help" || arg == "-h")
        {
            printUsage();
            return 0;
        }
    }

    // Initialize CEF
    CefMainArgs main_args(argc, argv);
    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;
    settings.external_message_pump = true;
    settings.multi_threaded_message_loop = false;
    settings.command_line_args_disabled = false;

    // Set paths
    std::string exePath = std::filesystem::canonical("/proc/self/exe").parent_path().string();
    CefString(&settings.resources_dir_path) = exePath;
    CefString(&settings.locales_dir_path) = exePath + "/locales";
    CefString(&settings.cache_path) = exePath + "/cache";

    // Create CEF app
    CefRefPtr<CefBrowserApp> cef_app(new CefBrowserApp());

    // Initialize CEF
    if (!CefInitialize(main_args, settings, cef_app.get(), nullptr))
    {
        std::cerr << "Failed to initialize CEF" << std::endl;
        return 1;
    }

    // Create OpenGL window
    ofGLFWWindowSettings windowSettings;
    windowSettings.setSize(1280, 720);
    windowSettings.windowMode = OF_WINDOW;
    windowSettings.setGLVersion(3, 2);
    ofCreateWindow(windowSettings);

    // Create and start app
    auto app = make_shared<MainApp>();
    app->setHtmlPath(htmlPath);
    app->setVideoPath(videoPath);
    ofRunApp(app);

    // Cleanup
    CefShutdown();
    return 0;
}