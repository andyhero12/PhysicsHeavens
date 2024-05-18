//
//  NLSinglePlayerLevel.h
//  Heaven
//
//  Created by Andrew Cheng on 4/29/24.
//

#ifndef NLSinglePlayerLevel_h
#define NLSinglePlayerLevel_h
#include <cugl/cugl.h>
#include "GlobalConstants.h"
#include "NLInput.h"
#include "LevelScene.h"
#include <map>
#include "AudioController.h"


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
#define PING_TEST_COUNT 5
class SinglePlayerLevelScene : public LevelScene {
    cugl::Size computeActiveSize() const;
protected:
    /** The network connection (as made by this scene) */
    std::shared_ptr<cugl::physics2::net::NetEventController> _network;
    
    std::shared_ptr<AudioController> _audioController;
    
    /** Whether the startGame button had been pressed. */
    bool _startGameClicked;
    
    /** The network configuration */
    cugl::net::NetcodeConfig _config;
public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new loading mode with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    SinglePlayerLevelScene() : LevelScene(), _startGameClicked(false) {}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    virtual ~SinglePlayerLevelScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    virtual void dispose() override;
    
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
    virtual bool init(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<cugl::physics2::net::NetEventController> network);
    
    void setAudioController(std::shared_ptr<AudioController> audioController)
    {
        _audioController = audioController;
    }
    
    
#pragma mark -
#pragma mark Progress Monitoring
    virtual void update(float timestep) override;
    virtual void setActive(bool value) override;
    void startGame();
    void endGame();
};


#endif /* NLSinglePlayerLevel_h */
