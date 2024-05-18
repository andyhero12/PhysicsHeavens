//
//  NLClientScene.cpp
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
#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "NLClientScene.h"

using namespace cugl;
using namespace cugl::physics2::net;

#pragma mark -
#pragma mark Level Layout

/** Regardless of logo, lock the height to this */
#define SCENE_HEIGHT  800

/** The key for the font reference */
#define PRIMARY_FONT "retro"


/**
 * Converts a decimal string to a hexadecimal string
 *
 * This function assumes that the string is a decimal number less
 * than 65535, and therefore converts to a hexadecimal string of four
 * characters or less (as is the case with the lobby server). We
 * pad the hexadecimal string with leading 0s to bring it to four
 * characters exactly.
 *
 * @param dec the decimal string to convert
 *
 * @return the hexadecimal equivalent to dec
 */
static std::string dec2hex(const std::string dec) {
    Uint32 value = strtool::stou32(dec);
    if (value >= 655366) {
        value = 0;
    }
    return strtool::to_hexstring(value,4);
}

/**
 * Initializes the controller contents, and starts the game
 *
 * The constructor does not allocate any objects or memory.  This allows
 * us to have a non-pointer reference to this controller, reducing our
 * memory allocation.  Instead, allocation happens in this method.
 *
 * @param assets    The (loaded) assets for this game mode
 *
 * @return true if the controller is initialized properly, false otherwise.
 */
bool ClientScene::init(const std::shared_ptr<cugl::AssetManager>& assets, std::shared_ptr<NetEventController> network) {
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_HEIGHT/dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    
    // Start up the input handler
    _assets = assets;
    _assets->loadDirectory("json/joinview.json");
    _network = network;
    _input.init();
    // Acquire the scene built by the asset loader and resize it the scene
    std::shared_ptr<scene2::SceneNode> scene = _assets->get<scene2::SceneNode>("joinview");
    scene->setContentSize(dimen);
    scene->doLayout(); // Repositions the HUD
    
    _startgame = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button_start"));
    //_backout = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("client_back"));
    _gameid = scene2::TextField::allocWithText("0123012", _assets->get<Font>(PRIMARY_FONT));
//    std::dynamic_pointer_cast<scene2::TextField>(_assets->get<scene2::SceneNode>("client_center_game_field_text"));
    //_player = std::dynamic_pointer_cast<scene2::Label>(_assets->get<scene2::SceneNode>("client_center_players_field_text"));
    _zero = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button0"));
    _one = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button1"));
    _two = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button2"));
    _three = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button3"));
    _four = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button4"));
    _five = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button5"));
    _six = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button6"));
    _seven = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button7"));
    _eight = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button8"));
    _nine = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button9"));
    _player1 = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button_player1"));
    _player2 = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button_player2"));
    _delete = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("joinview_button_back"));
    // _backout->addListener([this](const std::string& name, bool down) {
    //     if (down) {
    //         _network->disconnect();
    //         _backClicked = true;
    //     }
    // });
    

    _startgame->addListener([=](const std::string& name, bool down) {
        if (down) {
            // This will call the _gameid listener
            _gameid->releaseFocus();
        }
    });


     _gameid->addExitListener([this](const std::string& name, const std::string& value) {
     /**
      * TODO: Call the network controller to connect as a client (Remember to convert the string from decimal to hex)
      */
 #pragma mark BEGIN SOLUTION
         _network->connectAsClient(dec2hex(value));
 #pragma mark END SOLUTION
     });

    
    // Create the server configuration
    auto json = _assets->get<JsonValue>("server");
    _config.set(json);
    
    _gameid->setScale(0.75f);
    cugl::Size size = 0.5 * (dimen - _gameid->getSize());
    _gameid->setPosition(size.width, size.height + _gameid->getSize().height/10);
    scene->addChild(_gameid);
    addChild(scene);
//    addChild(_gameid);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void ClientScene::dispose() {
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
void ClientScene::setActive(bool value) {
    if (isActive() != value) {
        Scene2::setActive(value);
        
        /**
         * TODO: This is similar to HostScene. if value is true, you need to activate the _backout button, and set the clicked variable to false. However, you should start a connection this time. If the value is false, you should disconnect the network controller, and reset all buttons and textfields to their original state.
         */
#pragma mark BEGIN SOLUTION
        if (value) {
            _gameid->activate();
            //_backout->activate();
            //_player->setText("1");
            configureStartButton();
            _backClicked = false;
            // Don't reset the room id
        } else {
            _gameid->deactivate();
            _startgame->deactivate();
            //_backout->deactivate();
            //_network = nullptr;
            // If any were pressed, reset them
            _startgame->setDown(false);
            //_backout->setDown(false);
            
        }
#pragma mark END SOLUTION
    }
}

/**
 * Checks that the network connection is still active.
 *
 * Even if you are not sending messages all that often, you need to be calling
 * this method regularly. This method is used to determine the current state
 * of the scene.
 *
 * @return true if the network connection is still active.
 */
void ClientScene::updateText(const std::shared_ptr<scene2::Button>& button, const std::string text) {
    auto label = std::dynamic_pointer_cast<scene2::Label>(button->getChildByName("up")->getChildByName("label"));
    label->setText(text);

}

/**
 * The method called to update the scene.
 *
 * We need to update this method to constantly talk to the server
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void ClientScene::update(float timestep) {
    // Do this last for button safety
    configureStartButton();
    _input.update();

//     timeSinceLastSwitch += timestep;

//   if (timeSinceLastSwitch >= switchFreq) {
//     if (_input._updown != 0) {  // Handling up/down navigation
//         buttonGrid[currentRow][currentCol]->setDown(false); // Deselect current button
//         if (_input._updown == 1 && currentRow > 0) { // Moving up
//             currentRow--;
//         } else if (_input._updown == -1 && currentRow < buttonGrid.size() - 1) { // Moving down
//             currentRow++;
//         }
//         buttonGrid[currentRow][currentCol]->setDown(true); // Select new button
//     } else if (_input._Leftright != 0) {  // Handling left/right navigation
//         buttonGrid[currentRow][currentCol]->setDown(false); // Deselect current button
//         if (_input._Leftright == -1 && currentCol > 0) { // Moving left
//             currentCol--;
//         } else if (_input._Leftright == 1 && currentCol < buttonGrid[currentRow].size() - 1) { // Moving right
//             currentCol++;
//         }
//         buttonGrid[currentRow][currentCol]->setDown(true); // Select new button
//     }
//     timeSinceLastSwitch = 0; // Reset the timer
// }
    
    if(_network->getStatus() == NetEventController::Status::CONNECTED || _network->getStatus() == NetEventController::Status::HANDSHAKE){
        //_player->setText(std::to_string(_network->getNumPlayers()));
    }

    if(_input.didPressConfirm()){
        _startgame->setDown(true);
    }

    if(_input.didPressBack()){
        //_backout->setDown(true);
    }

}

/**
 * Reconfigures the start button for this scene
 *
 * This is necessary because what the buttons do depends on the state of the
 * networking.
 */
void ClientScene::configureStartButton() {
    if (_network->getStatus() == NetEventController::Status::IDLE) {
        _startgame->setDown(false);
        _startgame->activate();
        //updateText(_startgame, "Start Game");
    }
    else if (_network->getStatus() == NetEventController::Status::CONNECTING) {
        _startgame->setDown(false);
        _startgame->deactivate();
        //updateText(_startgame, "Connecting");
    }
    else if (_network->getStatus() == NetEventController::Status::CONNECTED) {
        _startgame->setDown(false);
        _startgame->deactivate();
        //updateText(_startgame, "Waiting");
    }
}
