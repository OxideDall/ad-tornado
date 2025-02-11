#include "ofMain.h"
#include "MainApp.h"
#include "include/cef_app.h"
#include "web/CefBrowserApp.h"
#include <filesystem>

void printUsage()
{
    std::cout << "Usage: " << std::endl;
    std::cout << "  --html <path>  : Path to HTML file (required)" << std::endl;
    std::cout << "  --video <path> : Path to video file (required)" << std::endl;
}

int main(int argc, char *argv[])
{
    CefMainArgs main_args(argc, argv);
    CefRefPtr<CefBrowserApp> app(new CefBrowserApp());

    int exit_code = CefExecuteProcess(main_args, app.get(), nullptr);
    if (exit_code >= 0)
    {
        return exit_code;
    }

    ofLogNotice("Main") << "Starting application in main process...";

    std::vector<std::string> cef_args = {
        "--disable-accelerated-video-decode",
    };

    std::vector<const char *> new_argv;
    new_argv.push_back(argv[0]);
    for (const auto &arg : cef_args)
    {
        new_argv.push_back(arg.c_str());
    }
    for (int i = 1; i < argc; ++i)
    {
        new_argv.push_back(argv[i]);
    }

    std::string htmlPath;
    std::string videoPath;
    bool htmlPathSet = false;
    bool videoPathSet = false;

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--help" || arg == "-h")
        {
            printUsage();
            return 0;
        }

        if (arg == "--html" && i + 1 < argc)
        {
            htmlPath = argv[++i];
            htmlPathSet = true;
            continue;
        }

        if (arg == "--video" && i + 1 < argc)
        {
            videoPath = argv[++i];
            videoPathSet = true;
        }
    }

    if (!htmlPathSet || !videoPathSet)
    {
        ofLogError("Main") << "Both HTML and video paths must be provided!";
        printUsage();
        return 1;
    }

    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = true;
    settings.external_message_pump = true;
    settings.multi_threaded_message_loop = false;
    settings.command_line_args_disabled = false;
    settings.background_color = 0;

    std::string exePath = std::filesystem::canonical("/proc/self/exe").parent_path().string();
    CefString(&settings.resources_dir_path) = exePath;
    CefString(&settings.locales_dir_path) = exePath + "/locales";
    CefString(&settings.cache_path) = exePath + "/cache";

    CefRefPtr<CefBrowserApp> cef_app(new CefBrowserApp());
    if (!CefInitialize(main_args, settings, cef_app.get(), nullptr))
    {
        ofLogError("Main") << "Failed to initialize CEF";
        return 1;
    }

    ofGLFWWindowSettings windowSettings;
    windowSettings.setGLVersion(3, 2);
    windowSettings.windowMode = OF_FULLSCREEN;
    ofCreateWindow(windowSettings);

    auto app_main = make_shared<MainApp>();
    app_main->setHtmlPath(htmlPath);
    app_main->setVideoPath(videoPath);

    int result = ofRunApp(app_main);

    CefShutdown();
    return result;
}