//
//  pauseScene.hpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#ifndef PauseScene_h
#define PauseScene_h
#include <cugl/cugl.h>
#include "GlobalConstants.h"
#include "NLInput.h"


#define PAUSE_SCALE 6



class PauseScene:  public cugl::scene2::SceneNode 
{
    
public:
    /**
     * The menu choice.
     *
     * This state allows the top level application to know what the user
     * chose.
     */
    enum Choice {
        /** User wants to host a game */
        GAME,
        /** User wants to join a game */
        EXIT
    };

    enum ContorllerChoice {
        /** User wants to host a game */
        isGAME,
        /** User wants to join a game */
        isEXIT,

        isNONE
    };
private:
    
    Choice status;
    ContorllerChoice constatus;

    bool _paused;
    

    /** The "pause" button */
    std::shared_ptr<cugl::scene2::Button>  resumeButton;
    std::shared_ptr<cugl::scene2::Button>  exitButton;
    std::shared_ptr<cugl::AssetManager> _assets;
    cugl::Size _screenSize;
    std::vector<std::shared_ptr<cugl::scene2::Button>> _buttonset;
    InputController _input;
    float timeSinceLastSwitch;
    float switchFreq;
    int _counter;
    
public:
    
    PauseScene(): _counter(1), timeSinceLastSwitch(0.0),switchFreq(0.20),_buttonset(){};
    
    bool init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize);
    bool init() override;
    
    void dispose() override;
    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    bool getPause() const { return _paused; }
    void togglePause();
    void exitToMain();

    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    void setPause(bool value);
    
    Choice getStatus(){return status;}

    ContorllerChoice getControlStatus(){return constatus;}

    void update(float timestep, int leftright, bool comfirm);
    
    
};
#include <stdio.h>

#endif /* pauseScene_hpp */
