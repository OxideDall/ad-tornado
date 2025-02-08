#include "web/VideoControlHandler.h"

VideoControlHandler::VideoControlHandler(VideoPlayer* player)
    : videoPlayer(player) {
}

bool VideoControlHandler::Execute(const CefString& name,
                                CefRefPtr<CefV8Value> object,
                                const CefV8ValueList& arguments,
                                CefRefPtr<CefV8Value>& retval,
                                CefString& exception) {
    if (name == "play") {
        videoPlayer->play();
        return true;
    }
    else if (name == "pause") {
        videoPlayer->pause();
        return true;
    }
    else if (name == "stop") {
        videoPlayer->stop();
        return true;
    }
    else if (name == "nextFrame") {
        videoPlayer->nextFrame();
        return true;
    }
    else if (name == "previousFrame") {
        videoPlayer->previousFrame();
        return true;
    }
    else if (name == "setPosition") {
        if (arguments.size() > 0 && arguments[0]->IsDouble()) {
            float position = static_cast<float>(arguments[0]->GetDoubleValue());
            videoPlayer->setPosition(position);
            return true;
        }
    }
    else if (name == "setSpeed") {
        if (arguments.size() > 0 && arguments[0]->IsDouble()) {
            float speed = static_cast<float>(arguments[0]->GetDoubleValue());
            videoPlayer->setSpeed(speed);
            return true;
        }
    }
    else if (name == "getVideoInfo") {
        retval = GetVideoInfo();
        return true;
    }

    return false;
}

CefRefPtr<CefV8Value> VideoControlHandler::GetVideoInfo() {
    CefRefPtr<CefV8Value> info = CefV8Value::CreateObject(nullptr);

    // Текущее время и длительность
    float position = videoPlayer->getPosition();
    float duration = videoPlayer->getDuration();
    float currentTime = position * duration;

    // Форматирование времени в MM:SS
    int currentMinutes = static_cast<int>(currentTime) / 60;
    int currentSeconds = static_cast<int>(currentTime) % 60;
    int totalMinutes = static_cast<int>(duration) / 60;
    int totalSeconds = static_cast<int>(duration) % 60;

    char currentTimeStr[10];
    char durationStr[10];
    snprintf(currentTimeStr, sizeof(currentTimeStr), "%d:%02d", currentMinutes, currentSeconds);
    snprintf(durationStr, sizeof(durationStr), "%d:%02d", totalMinutes, totalSeconds);

    // Заполняем объект информацией
    info->SetValue("currentTime", CefV8Value::CreateString(currentTimeStr), V8_PROPERTY_ATTRIBUTE_NONE);
    info->SetValue("duration", CefV8Value::CreateString(durationStr), V8_PROPERTY_ATTRIBUTE_NONE);
    info->SetValue("currentFrame", CefV8Value::CreateInt(videoPlayer->getCurrentFrame()), V8_PROPERTY_ATTRIBUTE_NONE);
    info->SetValue("totalFrames", CefV8Value::CreateInt(videoPlayer->getTotalNumFrames()), V8_PROPERTY_ATTRIBUTE_NONE);
    info->SetValue("speed", CefV8Value::CreateDouble(videoPlayer->getSpeed()), V8_PROPERTY_ATTRIBUTE_NONE);
    info->SetValue("position", CefV8Value::CreateDouble(position), V8_PROPERTY_ATTRIBUTE_NONE);

    return info;
} 