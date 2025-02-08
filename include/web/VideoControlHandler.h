#pragma once

#include <include/cef_client.h>
#include "video/VideoPlayer.h"

class VideoControlHandler : public CefV8Handler {
public:
    explicit VideoControlHandler(VideoPlayer* player);

    // Обработка JavaScript функций
    bool Execute(const CefString& name,
                CefRefPtr<CefV8Value> object,
                const CefV8ValueList& arguments,
                CefRefPtr<CefV8Value>& retval,
                CefString& exception) OVERRIDE;

    // Получение информации о видео для JavaScript
    CefRefPtr<CefV8Value> GetVideoInfo();

private:
    VideoPlayer* videoPlayer;

    IMPLEMENT_REFCOUNTING(VideoControlHandler);
    DISALLOW_COPY_AND_ASSIGN(VideoControlHandler);
}; 