//
//  pauseScene.hpp
//  Heaven
//
//  Created by Henry Gao on 4/9/24.
//

#ifndef LoseScene_h
#define LoseScene_h
#include <cugl/cugl.h>
#include "GlobalConstants.h"


#define PAUSE_SCALE 6



class LoseScene:  public cugl::scene2::SceneNode 
{
    
public:
    /**
     * The menu choice.
     *
     * This state allows the top level application to know what the user
     * chose.
     */
    enum Choice {
        NONE,
        /** User wants to host a game */
        MAIN,
        /** User wants to join a game */
        RETRY
    };
private:
    
    Choice status;
    
    bool _active;
    

    /** The "pause" button */
    std::shared_ptr<cugl::scene2::Button>  mainButton;
    std::shared_ptr<cugl::scene2::Button>  retryButton;
    std::shared_ptr<cugl::AssetManager> _assets;
    cugl::Size _screenSize;
    
public:
    
    LoseScene(){};
    
    bool init(std::shared_ptr<cugl::AssetManager> &assets, cugl::Size screenSize);
    bool init() override;
    
    void dispose() override;
    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    bool isActive() const { return _active; }
    void exitToMain();

    /**
     * Returns true if the gameplay controller is currently active
     *
     * @return true if the gameplay controller is currently active
     */
    void setActive(bool value);
    
    Choice getStatus(){return status;}
    
    
};
#include <stdio.h>

#endif /* pauseScene_hpp */
