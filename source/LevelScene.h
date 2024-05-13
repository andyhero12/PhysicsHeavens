//
//  NLLoadingScene.h
//  Networked Physics Demo
//
//  This module provides a very barebones loading screen.  Most of the time you
//  will not need a loading screen, because the assets will load so fast.  But
//  just in case, this is a simple example you can use in your games.
//
//  We know from 3152 that you all like to customize this screen.  Therefore,
//  we have kept it as simple as possible so that it is easy to modify.
//
//  Author: Walker White
//  Version: 1/10/17
//
#ifndef __NL_LEVEL_SCENE_H__
#define __NL_LEVEL_SCENE_H__
#include <cugl/cugl.h>
#include "GlobalConstants.h"
#include "NLInput.h"
#include <map>
#include "SpriteAnimationNode.h"


/**
 * This class is a simple loading screen for asychronous asset loading.
 *
 * The screen will display a very minimal progress bar that displays the
 * status of the asset manager.  Make sure that all asychronous load requests
 * are issued BEFORE calling update for the first time, or else this screen
 * will think that asset loading is complete.
 *
 * Once asset loading is completed, it will display a play button.  Clicking
 * this button will inform the application root to switch to the gameplay mode.
 */
class LevelScene : public cugl::Scene2 {
public:
    enum Level {
            /** User has not yet made a choice */
            L1,
            /** User wants to host a game */
            L2,
            /** User wants to join a game */
            L3,

            L4,

            L5,
        
            NONE,
        };
    
protected:
    /** The asset manager for loading. */
    std::shared_ptr<cugl::AssetManager> _assets;
    
    // NO CONTROLLER (ALL IN SEPARATE THREAD)
    
    // VIEW
    /** The animated progress bar */
    std::shared_ptr<cugl::scene2::ProgressBar>  _bar;
    /** The engine name */
    std::shared_ptr<cugl::scene2::SceneNode>  _brand;
    /** The "play" button */
    std::shared_ptr<cugl::scene2::Button>    _button;

    std::shared_ptr<cugl::scene2::SpriteNode> background;
    
    std::shared_ptr<JsonWriter> _writer;
    
    // The number of the highest unlocked level
    int unlockedLevels = 1;

    Level _level;
    InputController _input;

    std::map<int, int> frameTargets;
    // MODEL
    /** The progress displayed on the screen */
    float _progress;
    /** Whether or not the player has pressed play to continue */
    bool  _completed;
    
    int frame;

    int curMoveAnim;

    int moveCooldown;

    int level;
    
    bool _goright;
    bool _goleft;
    bool firsttime;
    bool _backClicked;
    /**
     * Returns the active screen size of this scene.
     *
     * This method is for graceful handling of different aspect
     * ratios
     */
    cugl::Size computeActiveSize() const;
        
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new loading mode with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    LevelScene() : cugl::Scene2(), 
    _progress(0.0f), 
    frame(0.0f), 
    curMoveAnim(6), 
    moveCooldown(6),
    _goleft(false), 
    _goright(false), 
    _level(NONE), 
    level(1),
    firsttime(true),
    _backClicked(false){
    frameTargets = {
        {1, 2},
        {2, 8},
        {3, 14},
        {4, 20},
        {5, 26}
        // Add new levels and frames as needed
    };

    }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~LevelScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose() override;
    
    /**
     * Initializes the controller contents, making it ready for loading
     *
     * The constructor does not allocate any objects or memory.  This allows
     * us to have a non-pointer reference to this controller, reducing our
     * memory allocation.  Instead, allocation happens in this method.
     *
     * @param assets    The (loaded) assets for this game mode
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    virtual bool init(const std::shared_ptr<cugl::AssetManager>& assets);
    
    
#pragma mark -
#pragma mark Progress Monitoring
    /**
     * The method called to update the game mode.
     *
     * This method updates the progress bar amount.
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    virtual void update(float timestep) override;
    
    /**
     * Returns true if loading is complete, but the player has not pressed play
     *
     * @return true if loading is complete, but the player has not pressed play
     */
    bool isPending( ) const;

    virtual void setActive(bool value) override;

    Level getLevel() const { return _level;}

    bool readToAnim(){
        return curMoveAnim >= moveCooldown;
    }
    
    bool readyToChangeLevel(){
        return finAnimLevel1() || finAnimLevel2() || finAnimLevel3()||finAnimLevel4()||finAnimLevel5();
    }
    bool finAnimLevel1(){
        return level == 1 && background->getFrame() == 2;
    }
    bool finAnimLevel2(){
        return level == 2 && background->getFrame() == 8;
    }
    bool finAnimLevel3(){
        return level == 3 && background->getFrame() == 14;
    }
    bool finAnimLevel4(){
        return level == 4 && background->getFrame() == 20;
    }
    bool finAnimLevel5(){
        return level == 5 && background->getFrame() == 26;
    }

    // bool readyToChangeLevel() {
    //     return isAnimationFinishedForLevel(level);
    // }

    // bool isAnimationFinishedForLevel(int lvl) {
    //     auto it = frameTargets.find(lvl);
    //     if (it != frameTargets.end()) {
    //         return background->getFrame() == it->second;
    //     }
    //     return false;  // Return false if level is not found
    // }

    void resetAnimCD(){
        curMoveAnim = 0;
    }
    void resetLevel(){
        _level = Level::NONE;
    }
    void resetgochange(){
        _goleft = false;
        _goright = false;
    }
    
    void updatelevelscene(){
       if (_goright && level < unlockedLevels) {
        level += 1;
        } else if (_goleft && level > 1) {
            level -= 1;
        }
    }

    void adjustFrame(int level);

    bool getBackclick(){
        return _backClicked;
    }

    
};


#endif /* __NL_LOADING_SCENE_H__ */
