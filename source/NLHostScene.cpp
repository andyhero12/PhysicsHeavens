//
//  NLHostScene.cpp
//  Network Lab
//
//  This class represents the scene for the host when creating a game. Normally
//  this class would be combined with the class for the client scene (as both
//  initialize the network controller).  But we have separated to make the code
//  a little clearer for this lab.
//
//  Author: Walker White, Aidan Hobler
//  Version: 2/8/22
//
#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "NLHostScene.h"

using namespace cugl;
using namespace cugl::net;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** Regardless of logo, lock the height to this */
#define SCENE_HEIGHT  800

/** The key for the font reference */
#define PRIMARY_FONT "retro"

/**
 * Converts a hexadecimal string to a decimal string
 *
 * This function assumes that the string is 4 hexadecimal characters
 * or less, and therefore it converts to a decimal string of five
 * characters or less (as is the case with the lobby server). We
 * pad the decimal string with leading 0s to bring it to 5 characters
 * exactly.
 *
 * @param hex the hexadecimal string to convert
 *
 * @return the decimal equivalent to hex
 */
static std::string hex2dec(const std::string hex) {
    Uint32 value = strtool::stou32(hex,0,16);
    std::string result = strtool::to_string(value);
    if (result.size() < 5) {
        size_t diff = 5-result.size();
        std::string alt(5,'0');
        for(size_t ii = 0; ii < result.size(); ii++) {
            alt[diff+ii] = result[ii];
        }
        result = alt;
    }
    return result;
}

#pragma mark -
#pragma mark Provided Methods
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
bool HostScene::init(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<NetEventController> network) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT/dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }

    _network = network;
    
    // Start up the input handler
    _assets = assets;
    
    _assets->loadDirectory("json/hostview.json");
    
    // Acquire the scene built by the asset loader and resize it the scene
    std::shared_ptr<scene2::SceneNode> scene = _assets->get<scene2::SceneNode>("hostview");
    scene->setContentSize(dimen);
    scene->doLayout(); // Repositions the HUD
    _input.init();
    _startgame = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("hostview_button_start"));
    _player1 = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("hostview_button_player1"));
    _player2 = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("hostview_button_player2"));
    _backout = scene2::Button::alloc(scene2::PolygonNode::allocWithTexture(_assets->get<cugl::Texture>("")));
    _gameid = scene2::TextField::allocWithText("999999", _assets->get<Font>(PRIMARY_FONT));

    // Program the buttons
    _backout->addListener([this](const std::string& name, bool down) {
        if (down) {
            _backClicked = true;
        }
    });

    _startgame->addListener([this](const std::string& name, bool down) {
        if (down) {
            std::cout << "starting game from host " << std::endl;
            startGame();
        }
    });
    
    _player1->setColor(Color4::ORANGE);
    _player2->setColor(Color4::ORANGE);
    _startgame->setColor(Color4::GRAY);
    _gameid->setText("");
    _gameid->setScale(0.75f);
    cugl::Size size = 0.5 * (dimen - _gameid->getSize());
    _gameid->setPosition(size.width, size.height + _gameid->getSize().height/10);
    scene->addChild(_gameid);
    
    // Create the server configuration
    auto json = _assets->get<JsonValue>("server");
    _config.set(json);
    addChild(scene);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void HostScene::dispose() {
    if (_active) {
        removeAllChildren();
        _active = false;
    }
}

/**
 * Sets whether the scene is currently active
 *
 * This method should be used to toggle all the UI elements.  Buttons
 * should be activated when it is made active and deactivated when
 * it is not.
 *
 * @param value whether the scene is currently active
 */
void HostScene::setActive(bool value) {
    if (isActive() != value) {
        Scene2::setActive(value);
        
        /**
         * TODO: if value is true, you need to activate the _backout button, and set the clicked variable to false. You need to also call the network controller to start a connection as a host. If the value is false, and reset all buttons and textfields to their original state.
         */
#pragma mark BEGIN SOLUTION
        if (value) {
            _startgame->activate();
            _backout->activate();
            _network->disconnect();
            _network->connectAsHost();
            _backClicked = false;
        } else {
            _gameid->setText("");
            _startgame->deactivate();
//            updateText(_startgame, "INACTIVE");
            _backout->deactivate();
            // If any were pressed, reset them
            _startgame->setDown(false);
            _backout->setDown(false);
            _player1->setColor(Color4::ORANGE);
            _player2->setColor(Color4::ORANGE);
        }
#pragma mark END SOLUTION
    }
}


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
void HostScene::updateText(const std::shared_ptr<scene2::Button>& button, const std::string text) {
    auto label = std::dynamic_pointer_cast<scene2::Label>(button->getChildByName("up")->getChildByName("label"));
    label->setText(text);

}

#pragma mark -
#pragma mark Student Methods
/**
 * The method called to update the scene.
 *
 * We need to update this method to constantly talk to the server
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void HostScene::update(float timestep) {
    /**
     * TODO: check for the status of `_network` (The NetworkController). If it is CONNECTED, you would need to update the scene nodes so that _gameId displays the id of the room (converted from hex to decimal) and _player displays the number of players. Additionally, you should check whether the `_startgame` button has been pressed and update its text. If it is not pressed yet, then its should display "Start Game" and be activated, otherwise, it should be deactivated and show "Starting".
     */

    _input.update();
#pragma mark BEGIN SOLUTION
    if(_network->getStatus() == NetEventController::Status::CONNECTED){
        if (!_startGameClicked) {
            _player1->setColor(Color4::GREEN);
            std::cout<<_network->getNumPlayers() <<std::endl;
            if(_network->getNumPlayers() == 2){
                _startgame->setColor(Color4::GREEN);
                _player2->setColor(Color4::GREEN);
                _startgame->activate();
                if(_input.didPressConfirm()){
                    _startgame->setDown(true);
                }
            }else{
                _startgame->setColor(Color4::GRAY);
                _startgame->deactivate();
            }

            if(_input.didPressBack()){
                _backout->setDown(true);
            }     
        }
        else {
            _startgame->deactivate();

        }
		_gameid->setText(hex2dec(_network->getRoomID()));
        
	}

#pragma mark END SOLUTION


}

/**
 * This method prompts the network controller to start the game.
 */
void HostScene::startGame(){
    //TODO: call the network controller to start the game and set the _startGameClicked to true.
#pragma mark BEGIN SOLUTION
    _network->startGame();
    _startGameClicked = true;
    
#pragma mark END SOLUTION
}

void HostScene::endGame(){
    _startGameClicked = false;
}
