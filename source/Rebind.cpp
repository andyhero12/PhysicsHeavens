//
//  NLLoadingScene.cpp
//  Networked Physics Demo
//
//  This module provides a very barebones loading screen.  Most of the time you
//  will not need a loading screen, because the assets will load so fast.  But
//  just in case, this is a simple example you can use in your games.
//
//  We know from 3152 that you all like to customize this screen.  Therefore,
//  we have kept it as simple as possible so that it is easy to modify. In
//  fact, this loading screen uses the new modular JSON format for defining
//  scenes.  See the file "loading.json" for how to change this scene.
//
//  Author: Walker White
//  Version: 1/10/17
//
#include "Rebind.h"

using namespace cugl;

/** This is the ideal size of the logo */
#define SCENE_SIZE  800

#pragma mark -
#pragma mark Constructors

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
bool RebindScene::init(const std::shared_ptr<AssetManager> &assets)
{
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    dimen *= SCENE_SIZE/dimen.height;
    if (assets == nullptr) {
        return false;
    } else if (!Scene2::init(dimen)) {
        return false;
    }
    //_input.init_withlistener();
    _input.init();
    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/RebindControl.json");
    std::shared_ptr<cugl::scene2::SceneNode> layer = assets->get<scene2::SceneNode>("Rebind");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen
    _button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("Rebind_button1"));
    _button->addListener([this](const std::string &name, bool down){
    if(down) { // Check if the button is pressed down
    }
    });
    background = cugl::scene2::SpriteNode::allocWithSheet(_assets->get<cugl::Texture>("background_rebind"), 1, 1);
//    std::cout << "height of level scene "<< background->getTexture()->getHeight()<<std::endl;
    background->setScale(2);
    background->setPosition(0.5 * background->getSize());
    addChild(background);
    layer->setColor(Color4(0, 0, 0, 1));
    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void RebindScene::dispose()
{
    // Deactivate the button (platform dependent)
    if (isPending())
    {
        _button->deactivate();
    }
    _button = nullptr;
    _assets = nullptr;
}

#pragma mark -
#pragma mark Progress Monitoring
/**
 * The method called to update the game mode.
 *
 * This method updates the progress bar amount.
 *
 * @param timestep  The amount of time (in seconds) since the last frame
 */
void RebindScene::update(float progress)
{
    _input.update();

    if (_input.didPressConfirm()){
        _button->setDown(true);
    }

    if(_input.didPressBack()){
        _backClicked = true;
    }

    if (firsttime)
    {
        _button->activate();
        _button->setVisible(false);
        firsttime = false;
    }
}

/**
 * Returns true if loading is complete, but the player has not pressed play
 *
 * @return true if loading is complete, but the player has not pressed play
 */
bool RebindScene::isPending() const
{
    return _button != nullptr && _button->isVisible();
}

void RebindScene::setActive(bool value)
{

    if (isActive() != value)
    {
        Scene2::setActive(value);
        if (value)
        {
            _button->activate();
            firsttime = true;
            _backClicked = false;
        }
        else
        {
            _button->deactivate();
            _button->setDown(false);
            firsttime = true;
            _backClicked = false;
        }
    }
}

