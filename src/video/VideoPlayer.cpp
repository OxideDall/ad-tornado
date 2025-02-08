#include "video/VideoPlayer.h"

VideoPlayer::VideoPlayer() : frameByframe(false) {
}

VideoPlayer::~VideoPlayer() {
    stop();
}

void VideoPlayer::setup() {
    ofSetVerticalSync(true);
}

void VideoPlayer::update() {
    player.update();
}

void VideoPlayer::draw(int x, int y, int width, int height) {
    ofSetHexColor(0xFFFFFF);
    player.draw(x, y, width, height);
}

bool VideoPlayer::loadVideo(const std::string& path) {
    return player.load(path);
}

void VideoPlayer::play() {
    player.play();
}

void VideoPlayer::pause() {
    player.setPaused(true);
}

void VideoPlayer::stop() {
    player.stop();
}

void VideoPlayer::togglePause() {
    player.setPaused(!player.isPaused());
}

void VideoPlayer::setPosition(float pct) {
    player.setPosition(pct);
}

void VideoPlayer::setSpeed(float speed) {
    player.setSpeed(speed);
}

void VideoPlayer::nextFrame() {
    player.nextFrame();
}

void VideoPlayer::previousFrame() {
    player.previousFrame();
}

void VideoPlayer::firstFrame() {
    player.firstFrame();
}

float VideoPlayer::getPosition() const {
    return player.getPosition();
}

float VideoPlayer::getDuration() const {
    return player.getDuration();
}

int VideoPlayer::getCurrentFrame() const {
    return player.getCurrentFrame();
}

int VideoPlayer::getTotalNumFrames() const {
    return player.getTotalNumFrames();
}

bool VideoPlayer::isPlaying() const {
    return player.isPlaying();
}

bool VideoPlayer::isDone() const {
    return player.getIsMovieDone();
}

float VideoPlayer::getSpeed() const {
    return player.getSpeed();
}

void VideoPlayer::setLoopState(ofLoopType state) {
    player.setLoopState(state);
}

void VideoPlayer::setVolume(float vol) {
    player.setVolume(vol);
}

int VideoPlayer::getWidth() const {
    return player.getWidth();
}

int VideoPlayer::getHeight() const {
    return player.getHeight();
} 