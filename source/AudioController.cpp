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
    enemySounds = 0;
    if(assets == nullptr){
        return false;
    }
    _assets = assets;
    
    sounds = std::make_shared<std::unordered_set<std::string>>();
    
    cugl::AudioEngine::get()->setListener([this](const std::string key, bool normalTermination) {
        if(normalTermination && sounds != nullptr){
            this->sounds->erase(key);
        }
        
        if(isEnemySound(key)){
            enemySounds--;
            //CULog("Decremented enemy sounds to %d", enemySounds);
        }
        
        std::cout << key << " stopped playing sound " <<std::endl;
    });
    
    
    return true;
}

void AudioController::playSFX(std::string key, std::string sound){
    if(_assets == nullptr){
        return;
    }
    
    if(isEnemySound(key) && enemySounds >= MAX_SOUNDS){
        CULog("TOO MANY ENEMY SOUNDS, NOT PLAYING %s", key.c_str());
        return;
    }
    
    if(isEnemySound(key)){
        enemySounds++;
        //CULog("%s played, incrementing enemySounds to %d", key.c_str(), enemySounds);
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
