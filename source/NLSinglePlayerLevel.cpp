//
//  NLSinglePlayerLevel.cpp
//  Heaven
//
//  Created by Andrew Cheng on 4/29/24.
//

#include "NLSinglePlayerLevel.h"

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
bool SinglePlayerLevelScene::init(const std::shared_ptr<AssetManager> &assets, std::shared_ptr<cugl::physics2::net::NetEventController> network)
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
    _network = network;
    // IMMEDIATELY load the splash screen assets
    _assets = assets;
    _assets->loadDirectory("json/singlePlayerLevelSelect.json");
    std::shared_ptr<cugl::scene2::SceneNode> layer = assets->get<scene2::SceneNode>("single_player_level");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen

    _button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("single_player_level_play"));
    _button->addListener([this](const std::string &name, bool down){
    if(down) { // Check if the button is pressed down
        startGame();
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
                CULog("Current Level: L4");
                break;
            case 5:
                _level = Level::L5;
                CULog("Current Level: L4");
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
    
    
    // Create the server configuration
    auto json = _assets->get<JsonValue>("server");
    _config.set(json);
    
    auto _uinode = scene2::SceneNode::alloc();
    background = SpriteAnimationNode::allocWithSheet(_assets->get<cugl::Texture>("BackgroundLevelSingle"), 15, 6, 80, 5);
    background->setScale(4.3);
    background->setPosition(0.5 * background->getSize());
    
    addChild(_uinode);
    _uinode->addChild(background);
    
    layer->setColor(Color4(0, 0, 0, 1));
    Application::get()->setClearColor(Color4(192, 192, 192, 255));
    addChild(layer);
    setActive(false);
    return true;
}

/**
 * Disposes of all (non-static) resources allocated to this mode.
 */
void SinglePlayerLevelScene::dispose()
{
    LevelScene::dispose();
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
void SinglePlayerLevelScene::update(float progress)
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
    if(_network->getStatus() == cugl::physics2::net::NetEventController::Status::CONNECTED){
        if (firsttime)
        {
            _button->activate();
            _button->setVisible(false);
            firsttime = false;
        }
        if(!_startGameClicked){
            if (_input.didPressConfirm()){
                _button->setDown(true);
            }
        }else{
            _button->deactivate();
        }
    }
    if(_input.didPressBack() && readyToChangeLevel()){
        _backClicked = true;
    }
}

void SinglePlayerLevelScene::setActive(bool value)
{

    if (isActive() != value)
    {
        Scene2::setActive(value);
        if (value)
        {
            _level = Level::NONE;
            firsttime = true;
            _network->disconnect();
            _network->connectAsHost();
            _backClicked = false;
            _startGameClicked = false;
        }
        else
        {
            _button->deactivate();
            _button->setDown(false);
            firsttime = true;
            _backClicked = false;
            _startGameClicked = false;
        }
    }
}
/**
 * This method prompts the network controller to start the game.
 */
void SinglePlayerLevelScene::startGame(){
    //TODO: call the network controller to start the game and set the _startGameClicked to true.
#pragma mark BEGIN SOLUTION
    _network->startGame();
    _startGameClicked = true;
#pragma mark END SOLUTION
}

void SinglePlayerLevelScene::endGame(){
    _startGameClicked = false;
}
