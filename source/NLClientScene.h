//
//  NLClientScene.h
//  Network Lab
//
//  This class represents the scene for the client when joining a game. Normally
//  this class would be combined with the class for the client scene (as both
//  initialize the network controller).  But we have separated to make the code
//  a little clearer for this lab.
//
//  Author: Walker White, Aidan Hobler
//  Version: 2/8/22
//
#ifndef __NL_CLIENT_SCENE_H__
#define __NL_CLIENT_SCENE_H__
#include <cugl/cugl.h>
#include <vector>
#include "GlobalConstants.h"
#include "NLInput.h"
#include <string>

using namespace cugl::physics2::net;

/**
 * This class provides the interface to join an existing game.
 *
 * Most games have a since "matching" scene whose purpose is to initialize the
 * network controller.  We have separate the host from the client to make the
 * code a little more clear.
 */
class ClientScene : public cugl::Scene2 {
public:
    enum number {
            /** User has not yet made a choice */
            zero,
            /** User wants to host a game */
            one,
            /** User wants to join a game */
            two,
            three,
            four,
            five,
            six,
            seven,
            eight,
            nine,
            back,
            start
     };   

protected:
    /** The asset manager for this scene. */
    std::shared_ptr<cugl::AssetManager> _assets;
    /** The network connection (as made by this scene) */
    std::shared_ptr<NetEventController> _network;

    /** The menu button for starting a game */
    std::shared_ptr<cugl::scene2::Button> _startgame;
    /** The back button for the menu scene */
    // std::shared_ptr<cugl::scene2::Button> _backout;
    // /** The game id label (for updating) */
     std::shared_ptr<cugl::scene2::TextField> _gameid;
    // /** The players label (for updating) */
    // std::shared_ptr<cugl::scene2::Label> _player;

    std::shared_ptr<cugl::scene2::Button> _zero;
    std::shared_ptr<cugl::scene2::Button> _one;
    std::shared_ptr<cugl::scene2::Button> _two;
    std::shared_ptr<cugl::scene2::Button> _three;
    std::shared_ptr<cugl::scene2::Button> _four;
    std::shared_ptr<cugl::scene2::Button> _five;
    std::shared_ptr<cugl::scene2::Button> _six;
    std::shared_ptr<cugl::scene2::Button> _seven;
    std::shared_ptr<cugl::scene2::Button> _eight;
    std::shared_ptr<cugl::scene2::Button> _nine;
    std::shared_ptr<cugl::scene2::Button> _delete;
    std::shared_ptr<cugl::scene2::Button> _player1;
    std::shared_ptr<cugl::scene2::Button> _player2;

    std::vector<std::vector<std::shared_ptr<cugl::scene2::Button>>> buttonGrid;
    InputController _input;

    float timeSinceLastSwitch;

    float switchFreq;

    int currentRow;
    int currentCol;
    number _number;
    /** The network configuration */
    cugl::net::NetcodeConfig _config;
    
    /** Whether the back button had been clicked. */
    bool _backClicked;

    std::string _value;

public:
#pragma mark -
#pragma mark Constructors
    /**
     * Creates a new client scene with the default values.
     *
     * This constructor does not allocate any objects or start the game.
     * This allows us to use the object without a heap pointer.
     */
    ClientScene() : cugl::Scene2(), _backClicked(false),timeSinceLastSwitch(0.0), switchFreq(0.1),currentRow(0),currentCol(0),_number(zero), _value(""){}
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     *
     * This method is different from dispose() in that it ALSO shuts off any
     * static resources, like the input controller.
     */
    ~ClientScene() { dispose(); }
    
    /**
     * Disposes of all (non-static) resources allocated to this mode.
     */
    void dispose() override;
    
    /**
     * Initializes the controller contents, and starts the game
     *
     * In previous labs, this method "started" the scene.  But in this
     * case, we only use to initialize the scene user interface.  We
     * do not activate the user interface yet, as an active user
     * interface will still receive input EVEN WHEN IT IS HIDDEN.
     *
     * That is why we have the method {@link #setActive}.
     *
     * @param assets    The (loaded) assets for this game mode
     *
     * @return true if the controller is initialized properly, false otherwise.
     */
    bool init(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<NetEventController> network);

    /**
     * Sets whether the scene is currently active
     *
     * This method should be used to toggle all the UI elements.  Buttons
     * should be activated when it is made active and deactivated when
     * it is not.
     *
     * @param value whether the scene is currently active
     */
    virtual void setActive(bool value) override;

    /**
     * The method called to update the scene.
     *
     * We need to update this method to constantly talk to the server
     *
     * @param timestep  The amount of time (in seconds) since the last frame
     */
    void update(float timestep) override;
    
    /**
     * Returns whether the back button has been clicked
     */
    bool getBackClicked() { return _backClicked; }

private:
    /**
     * Updates the text in the given button.
     *
     * Techincally a button does not contain text. A button is simply a scene graph
     * node with one child for the up state and another for the down state. So to
     * change the text in one of our buttons, we have to descend the scene graph.
     * This method simplifies this process for you.
     *
     * @param button    The button to modify
     * @param text      The new text value
     */
    void updateText(const std::shared_ptr<cugl::scene2::Button>& button, const std::string text);

    /**
     * Reconfigures the start button for this scene
     *
     * This is necessary because what the buttons do depends on the state of the
     * networking.
     */
    void configureStartButton();

  
};

#endif /* __NL_GAME_SCENE_H__ */
