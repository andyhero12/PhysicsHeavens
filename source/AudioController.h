//
//  AudioController.hpp
//  Heaven
//
//  Created by Henry Gao on 5/3/24.
//

#ifndef AudioController_h
#define AudioController_h

#include <stdio.h>
#include <cugl/cugl.h>
#include "Constants.h"

// Maximum number of enemy sounds to be played
#define MAX_SOUNDS 10

class AudioController
{
protected:
    std::shared_ptr<cugl::AssetManager> _assets;
    // set of all sfx & music
    
    /** set of all active sounds */
    std::shared_ptr<std::unordered_set<std::string>> sounds;
    
    bool isEnemySound(std::string key){
        return key.find("nemy") != std::string::npos;
    };
    
    int enemySounds;

public:
    /**
     * Creates a new Audio controller with the default settings
     *
     */
    AudioController();

    /**
     * Disposses this Audio controller, releasing all resources.
     */
    ~AudioController() {
        dispose();
    }
    
    
    static std::shared_ptr<AudioController> alloc(std::shared_ptr<cugl::AssetManager> assets) {
        std::shared_ptr<AudioController> controller = std::make_shared<AudioController>();
        return (controller->init(assets) ? controller : nullptr);
    }
    
    bool init(std::shared_ptr<cugl::AssetManager> assets);

    void playSFX(std::string key, std::string sound);
    
    void stopSFX(std::string key);
    
    void dispose(){
        cugl::AudioEngine::get()->clearEffects();
        cugl::AudioEngine::get()->clear();
        sounds = nullptr;
    }
    
    void playMusic(std::string key, std::string sound);
    
};



// assets
// control the # of sfx played
// remove sfx when finished



#endif /* AudioController_hpp */
