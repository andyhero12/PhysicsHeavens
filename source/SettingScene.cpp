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
#include "Setting.h"

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
bool SettingScene::init(const std::shared_ptr<AssetManager> &assets)
{
    // Initialize the scene to a locked width
    Size dimen = Application::get()->getDisplaySize();
    // Lock the scene to a reasonable resolution
    if (dimen.width > dimen.height)
    {
        dimen *= SCENE_SIZE / dimen.width;
    }
    else
    {
        dimen *= SCENE_SIZE / dimen.height;
    }
    if (assets == nullptr)
    {
        return false;
    }
    else if (!Scene2::init(dimen))
    {
        return false;
    }
    //_input.init_withlistener();
    _input.init();
    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/setting.json");
    std::shared_ptr<cugl::scene2::SceneNode> layer = assets->get<scene2::SceneNode>("setting");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen

    //_bar = std::dynamic_pointer_cast<scene2::ProgressBar>(assets->get<scene2::SceneNode>("level_bar"));
    //_brand = assets->get<scene2::SceneNode>("level_name");
    _button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("setting_play"));
    _button->addListener([this](const std::string &name, bool down){
    if(down) { // Check if the button is pressed down
        switch (level) { // Use the current level stored in _level
            case 1:
                CULog("Current Level: L1");
                _buttonselection = button::b1; // Move to next level
                break;
            case 2:
                CULog("Current Level: L2");
                _buttonselection = button::b2;
                break;
            case 3:
                CULog("Current Level: L3");
                _buttonselection = button::b3; // Assuming there's a level 4
                break;
            case 4:
                CULog("Current Level: L4");
                break;
            default:
                CULog("Unknown Level");
                break;
        }
    }
});
    background = cugl::scene2::SpriteNode::allocWithSheet(_assets->get<cugl::Texture>("setting_background"), 1, 9);
    background->setScale(4);
    background->setPosition(0.5 * background->getSize());
    addChild(background);
    layer->setColor(Color4(0, 0, 0, 1));
    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void SettingScene::dispose()
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
void SettingScene::update(float progress)
{
    _input.update();
    
    if (curMoveAnim <= moveCooldown){
        curMoveAnim++;
    }

    if(_input._Leftright == 1 && readyToChangeLevel()){
        _goright = true;
    }

    if(_input._Leftright == -1 && readyToChangeLevel()){
        _goleft = true;
    }

    if(_input.didPressRight() && readyToChangeLevel()){
        _goright = true;
    }
    if(_input.didPressLeft() && readyToChangeLevel()){
        _goleft = true;
    }
    
    updatelevelscene();
    resetgochange();
    adjustFrame(level);

    if (_input.didPressConfirm() && readyToChangeLevel()){
        _button->setDown(true);
    }

    if(_input.didPressBack() && readyToChangeLevel()){
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
bool SettingScene::isPending() const
{
    return _button != nullptr && _button->isVisible();
}

void SettingScene::setActive(bool value)
{

    if (isActive() != value)
    {
        Scene2::setActive(value);
        if (value)
        {
            _buttonselection = button::NONE;
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

void SettingScene::adjustFrame(int level){
    if (readToAnim()) {
            int targetFrame = frameTargets[level];
            if (background->getFrame() != targetFrame) {
                resetAnimCD();
                if (background->getFrame() < targetFrame) {
                    background->setFrame(background->getFrame() + 1);
                } else if (background->getFrame() > targetFrame) {
                    background->setFrame(background->getFrame() - 1);
                }
            }
    }
}