//
//  AudioController.cpp
//  Heaven
//
//  Created by Henry Gao on 5/3/24.
//

#include "AudioController.h"

AudioController::AudioController() {
    _assets = nullptr;
    sounds = nullptr;
};


bool AudioController::init(std::shared_ptr<cugl::AssetManager> assets){
    if(assets == nullptr){
        return false;
    }
    _assets = assets;
    
    sounds = std::make_shared<std::unordered_set<std::string>>();
    
    cugl::AudioEngine::get()->setListener([this](const std::string key, bool normalTermination) {
        if(normalTermination && sounds != nullptr){
            this->sounds->erase(key);
        }
        std::cout << key << " stopped playing sound " <<std::endl;
    });
    
    
    return true;
}

void AudioController::playSFX(std::string key, std::string sound){
    if(_assets == nullptr){
        return;
    }
    auto source = _assets->get<cugl::Sound>(sound);
    cugl::AudioEngine::get()->play(key, source, false, source->getVolume(), true);
}

void AudioController::stopSFX(std::string key){
    if(_assets == nullptr){
        return;
    }
    cugl::AudioEngine::get()->clear(key);
}

void AudioController::playMusic(std::string key, std::string sound){
    if(_assets == nullptr){
        return;
    }
    auto source = _assets->get<cugl::Sound>(sound);
    cugl::AudioEngine::get()->getMusicQueue()->enqueue(source, true, source->getVolume(), false);
}
