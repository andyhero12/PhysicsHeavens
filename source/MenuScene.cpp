//
//  NLMenuScene.h
//  Network Lab
//
//  This class presents the initial menu scene.  It allows the player to
//  choose to be a host or a client.
//
//  Author: Walker White, Aidan Hobler
//  Version: 2/8/22
//
#include <cugl/cugl.h>
#include <iostream>
#include <sstream>

#include "MenuScene.h"

using namespace cugl;
using namespace std;

#pragma mark -
#pragma mark Level Layout

/** Regardless of logo, lock the height to this */
#define SCENE_HEIGHT  800


#pragma mark -
#pragma mark Constructors
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
bool MenuScene::init(const std::shared_ptr<cugl::AssetManager>& assets) {
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
    _input.init();
    // Acquire the scene built by the asset loader and resize it the scene
    std::shared_ptr<scene2::SceneNode> scene = _assets->get<scene2::SceneNode>("menu");
    scene->setContentSize(dimen);
    scene->doLayout(); // Repositions the HUD
    _choice = Choice::NONE;
    _buttonset.push_back(_hostbutton = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("menu_host")));
    _buttonset.push_back(_joinbutton = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("menu_join")));
    _buttonset.push_back(_back = std::dynamic_pointer_cast<scene2::Button>(_assets->get<scene2::SceneNode>("menu_back")));
    _hostbutton->addListener([this](const std::string& name, bool down) {
        if (down) {  
            if(_input.getState()==InputController::State::CONTROLLER){
                _isdown = Isdown::isHOST;
            }
            else{
                _choice = Choice::HOST;
            }
        }
    });
    _joinbutton->addListener([this](const std::string& name, bool down) {
        if (down) {  
            if(_input.getState()==InputController::State::CONTROLLER){
                _isdown = Isdown::isJOIN;
            }
            else{
                _choice = Choice::JOIN;
            }
        }
    });

    _back->addListener([this](const std::string& name, bool down) {
        if (down) {  
            if(_input.getState()==InputController::State::CONTROLLER){
                _isdown = Isdown::isBACK;
            }
            else{
                _backclicked = true;
            }
        }
    });

    _backclicked = false;
    addChild(scene);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void MenuScene::dispose() {
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
void MenuScene::setActive(bool value) {
    if (isActive() != value) {
        Scene2::setActive(value);
        if (value) {
            _choice = NONE;
            _hostbutton->activate();
            _joinbutton->activate();
            _back->activate();
            _backclicked = false;
            _firstset = true;
        } else {
            _hostbutton->deactivate();
            _joinbutton->deactivate();
            _back->deactivate();
            // If any were pressed, reset them
            _hostbutton->setDown(false);
            _joinbutton->setDown(false);
            _back->setDown(false);
        }
    }
}

void MenuScene::update(float timestep){
    _input.update();
    if(_firstset&&_input.getState()==InputController::State::CONTROLLER){
        _hostbutton->setDown(true);
        _firstset = false;
    }
    timeSinceLastSwitch += timestep;
    //std::cout << timeSinceLastSwitch << std::endl;
    if (timeSinceLastSwitch >= switchFreq) {
        if (_input._updown != 0) {
            if (_input._updown == 1 && _counter > 0) {
                _buttonset.at(_counter)->setDown(false);
                _counter--;
                _buttonset.at(_counter)->setDown(true);
            }
            else if (_input._updown == -1 && _counter < _buttonset.size() - 1) {
                _buttonset.at(_counter)->setDown(false);
                _counter++;
                _buttonset.at(_counter)->setDown(true);
            }
            timeSinceLastSwitch = 0;

        }
    }
    //std::cout << _input._confirm << std::endl;
    if (_isdown == Isdown::isHOST &&_input.didPressConfirm() ){
        _choice = Choice::HOST;
    }
    else if(_isdown == Isdown::isJOIN && _input.didPressConfirm()){
        _choice = Choice::JOIN;
    }
    else if (_isdown == Isdown::isBACK && _input.didPressConfirm()) {
        _backclicked = true;
    }
    else if (_isdown == Isdown::isNONE && _input.didPressConfirm()) {
    }



}
