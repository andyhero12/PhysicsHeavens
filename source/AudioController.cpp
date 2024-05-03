//
//  AudioController.cpp
//  Heaven
//
//  Created by Henry Gao on 5/3/24.
//

#include "AudioController.h"

bool AudioController::init(std::shared_ptr<cugl::AssetManager> assets){
    if(assets==nullptr){
        return false;
    }
    _assets = assets;
    return true;
}

void AudioController::playSFX(std::string key, std::string sound){
    if(_assets == nullptr){
        return;
    }
    auto source = _assets->get<cugl::Sound>(sound);
    cugl::AudioEngine::get()->play(key, source, false, source->getVolume(), true);
}

void AudioController::playMusic(std::string key, std::string sound){
    if(_assets == nullptr){
        return;
    }
    auto source = _assets->get<cugl::Sound>(sound);
    cugl::AudioEngine::get()->getMusicQueue()->enqueue(source, true, source->getVolume(), false);
}
