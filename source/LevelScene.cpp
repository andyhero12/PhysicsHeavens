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
#include "LevelScene.h"

using namespace cugl;

/** This is the ideal size of the logo */
#define SCENE_SIZE  800
/** This is the size of the active portion of the screen */
#define SCENE_WIDTH 1280
#define SCENE_HEIGHT 800
#pragma mark -
#pragma mark Constructors


/**
 * Returns the active screen size of this scene.
 *
 * This method is for graceful handling of different aspect
 * ratios
 */
cugl::Size LevelScene::computeActiveSize() const
{
    cugl::Size dimen = cugl::Application::get()->getDisplaySize();
    float ratio1 = dimen.width / dimen.height;
    float ratio2 = ((float)SCENE_WIDTH) / ((float)SCENE_HEIGHT);
    if (ratio1 < ratio2)
    {
        dimen *= SCENE_WIDTH / dimen.width;
    }
    else
    {
        dimen *= SCENE_HEIGHT / dimen.height;
    }
    return dimen;
}
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
bool LevelScene::init(const std::shared_ptr<AssetManager> &assets)
{
    // Initialize the scene to a locked width
    Size dimen = computeActiveSize();

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
    _assets->loadDirectory("json/levelselection.json");
    std::shared_ptr<cugl::scene2::SceneNode> layer = assets->get<scene2::SceneNode>("level");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen

    //_bar = std::dynamic_pointer_cast<scene2::ProgressBar>(assets->get<scene2::SceneNode>("level_bar"));
    //_brand = assets->get<scene2::SceneNode>("level_name");
    _button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("level_play"));
    _button->addListener([this](const std::string &name, bool down){
    if(down) { // Check if the button is pressed down
        switch (level) { // Use the current level stored in _level
            case 1:
                CULog("Current Level: L1");
                _level = Level::L1; // Move to next level
                break;
            case 2:
                CULog("Current Level: L2");
                _level = Level::L2;
                break;
            case 3:
                CULog("Current Level: L3");
                _level = Level::L3; // Assuming there's a level 4
                break;
            case 4:
                _level = Level::L4;
                break;
            case 5:
                _level = Level::L5;
                break;
            case 6:
                _level = Level::L6;
                break;
            case 7:
                _level = Level::L7;
                break;
            case 8:
                _level = Level::L8;
                break;
            case 9:
                _level = Level::L9;
                break;
            case 10:
                _level = Level::L10;
                break;
            case 11:
                _level = Level::L11;
                break;
            case 12:
                _level = Level::L12;
                break;
            case 13:
                _level = Level::L13;
                break;
            case 14:
                _level = Level::L14;
                break;
            case 15:
                _level = Level::L15;
                break;
            default:
                CULog("Unknown Level");
                break;
        }
    }
});
    
    auto _uinode = scene2::SceneNode::alloc();
    background = SpriteAnimationNode::allocWithSheet(_assets->get<cugl::Texture>("LongBackground"), 3, 27, 81, 5);
    background->setScale(4.3);
    background->setPosition(0.5 * background->getSize());
    autoBackground = SpriteAnimationNode::allocWithSheet(_assets->get<cugl::Texture>("repeat_win"), 2, 5, 8, 8);
    autoBackground->setScale(2* 800/autoBackground->getTexture()->getHeight());
    autoBackground->setPosition(0.5 * autoBackground->getSize());
    
    addChild(_uinode);
    _uinode->addChild(background);
    _uinode->addChild(autoBackground);
    autoBackground->setVisible(false);
    layer->setColor(Color4(0, 0, 0, 1));
    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void LevelScene::dispose()
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
void LevelScene::update(float progress)
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
bool LevelScene::isPending() const
{
    return _button != nullptr && _button->isVisible();
}

void LevelScene::setActive(bool value)
{
    auto source = _assets->get<Sound>(SUBWAY);
    if (isActive() != value)
    {
        Scene2::setActive(value);
        if (value)
        {
            _level = Level::NONE;
            _button->activate();
            firsttime = true;
            _backClicked = false;
            AudioEngine::get()->play(SUBWAY, source, true, source->getVolume(), true);
        }
        else
        {
            _button->deactivate();
            _button->setDown(false);
            firsttime = true;
            _backClicked = false;
            AudioEngine::get()->clear(SUBWAY);
        }
    }
}

void LevelScene::adjustFrame(int level){
    if (readToAnim()) {
            int targetFrame = frameTargets.at(level);
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

void LevelScene::autoStartGame(LevelScene::Level level){
    autoStart = true;
    _level = level;
    setAutoVisible(true);
}

void LevelScene::resetAutoStart() {
    autoStart = false;
}


void LevelScene::setAutoVisible(bool b){
    autoBackground->setVisible(b);
}
