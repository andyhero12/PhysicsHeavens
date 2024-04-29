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
bool SinglePlayerLevelScene::init(const std::shared_ptr<AssetManager> &assets)
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
    _assets->loadDirectory("json/singlePlayerLevelSelect.json");
    std::shared_ptr<cugl::scene2::SceneNode> layer = assets->get<scene2::SceneNode>("single_player_level");
    layer->setContentSize(dimen);
    layer->doLayout(); // This rearranges the children to fit the screen

    _button = std::dynamic_pointer_cast<scene2::Button>(assets->get<scene2::SceneNode>("single_player_level_play"));
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
                CULog("Current Level: L4");
                break;
            default:
                CULog("Unknown Level");
                break;
        }
    }
});
    background = cugl::scene2::SpriteNode::allocWithSheet(_assets->get<cugl::Texture>("Background"), 1, 15);
//    std::cout << "height of level scene "<< background->getTexture()->getHeight()<<std::endl;
    background->setScale(4.3);
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

void SinglePlayerLevelScene::setActive(bool value)
{

    if (isActive() != value)
    {
        Scene2::setActive(value);
        if (value)
        {
            _level = Level::NONE;
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
