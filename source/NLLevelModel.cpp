//
//  JSLevelModel.cpp
//  Wireframe
//
//  Author: Walker White and Anthony Perello
//  Version: 3/12/17
//
#include <cugl/assets/CUJsonLoader.h>
#include "NLLevelModel.h"

#pragma mark -
#pragma mark Static Constructors

/**
* Creates a new, empty level.
*/
LevelModel::LevelModel(void) : Asset()
{
}

/**
* Destroys this level, releasing all resources.
*/
LevelModel::~LevelModel(void) {
    unload();
}


#pragma mark -
#pragma mark Asset Loading
/**
 * Loads this game level from the source file
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded in
 * parallel, not in sequence.  If an asset (like a game level) has references to
 * other assets, then these should be connected later, during scene initialization.
 *
 * @return true if successfully loaded the asset from a file
 */
bool LevelModel::preload(const std::string& file) {
    std::shared_ptr<JsonReader> reader = JsonReader::allocWithAsset(file);
    return preload(reader->readJson());
}

/**
 * Loads this game level from the source file
 *
 * This load method should NEVER access the AssetManager.  Assets are loaded in
 * parallel, not in sequence.  If an asset (like a game level) has references to
 * other assets, then these should be connected later, during scene initialization.
 *
 * @return true if successfully loaded the asset from a file
 */
bool LevelModel:: preload(const std::shared_ptr<cugl::JsonValue>& json) {
    if (json == nullptr) {
        CUAssertLog(false, "Failed to load level file");
        return false;
    }
    _levelHeight = json->get("height")->asInt();
    _levelWidth = json->get("width")->asInt();
    _tileHeight = json->get("tileheight")->asInt();
    _tileWidth = json->get("tilewidth")->asInt();
    for (const std::shared_ptr<cugl::JsonValue>& layer: json->get("layers")->children()) {
        loadLayer(layer);
    }
    return true;
}
/**
 Generic Function to Load a certain Layer
 */
void LevelModel::loadLayer(const std::shared_ptr<JsonValue>& json){
    std::string type = json->get("name")->asString();
    if (type == "DrawTiles"){
        loadTiles(json);
    }else if (type == "Boundaries"){
        loadBoundaries(json);
    }else if (type == "Decor"){
        loadDecorations(json);
    }else if (type == "PlayerSpawn"){
        loadPlayer(json);
    }else if (type == "SpawnerLocs"){
        loadSpanwerLocations(json);
    }else if (type == "BaseSpawn"){
        loadBaseLocations(json);
    }else if (type == "PreSpawnedClusters"){
        loadPreSpawnedClusters(json);
    }else{
        CULog("TYPE NOT FOUND %s\n", type.data());
    }
}
/**
* Unloads this game level, releasing all sources
*
* This load method should NEVER access the AssetManager.  Assets are loaded and
* unloaded in parallel, not in sequence.  If an asset (like a game level) has
* references to other assets, then these should be disconnected earlier.
*/
void LevelModel::unload() {
    _tiles.clear();
    _walls.clear();
    _decors.clear();
}

#pragma mark -
#pragma mark Individual Loaders

bool LevelModel::loadBaseLocations(const std::shared_ptr<JsonValue>& json) {
    auto spawnerValues = json->get("objects");
    for (int i = 0 ; i< spawnerValues->size() ;i++){
        float baseX = json->get("objects")->get(i)->get("x")->asFloat();
        float baseY = json->get("objects")->get(i)->get("y")->asFloat();
        auto health = json->get("objects")->get("properties")->get(0)->get("value")->asFloat();
        _basesPos.emplace_back(cugl::Vec3(baseX,baseY, health));
    }
    return true;
}

bool LevelModel::loadPreSpawnedClusters(const std::shared_ptr<JsonValue>& json) {
    auto clusters = json->get("objects");
    for (int i = 0 ; i< clusters->size() ;i++){
        float spawnX = json->get("objects")->get(i)->get("x")->asFloat();
        float spawnY = json->get("objects")->get(i)->get("y")->asFloat();
        auto spawnNum = json->get("objects")->get("properties")->get(0)->get("value")->asFloat();
        _preSpawnLocs.emplace_back(cugl::Vec3(spawnX,spawnY, spawnNum));
    }
    return true;
}
bool LevelModel::loadTiles(const std::shared_ptr<JsonValue>& json) {
    int row = json->get("width")->asInt();
    int column = json->get("height")->asInt();
    auto array = json->get("data")->children();
    _tiles.resize(row,std::vector<int>(column,0));
    for (int i =0 ; i< row; i++){
        for (int j =0 ;j < column ;j ++){
            _tiles.at(i).at(j) = array.at(i*column + j)->asInt();
        }
    }
    return true;
}

bool LevelModel::loadBoundaries(const std::shared_ptr<JsonValue>& json) {
    int row = json->get("width")->asInt();
    int column = json->get("height")->asInt();
    auto array = json->get("data")->children();
    _walls.resize(row,std::vector<int>(column,0));
    for (int i =0 ; i< row; i++){
        for (int j =0 ;j < column ;j ++){
            _walls.at(i).at(j) = array.at(i*column + j)->asInt();
        }
    }
    return true;
}

bool LevelModel::loadDecorations(const std::shared_ptr<JsonValue>& json){
    int row = json->get("width")->asInt();
    int column = json->get("height")->asInt();
    auto array = json->get("data")->children();
    _decors.resize(row,std::vector<int>(column,0));
    for (int i =0 ; i< row; i++){
        for (int j =0 ;j < column ;j ++){
            _decors.at(i).at(j) = array.at(i*column + j)->asInt();
        }
    }
    return true;
}


bool LevelModel::loadPlayer(const std::shared_ptr<JsonValue>& json){
    float playerX = json->get("objects")->get(0)->get("x")->asFloat();
    float playerY = json->get("objects")->get(0)->get("y")->asFloat();
    _playerPos = cugl::Vec2(playerX, playerY);
    return false;
}

bool LevelModel::loadSpanwerLocations(const std::shared_ptr<JsonValue>& json){
    auto spawnerValues = json->get("objects");
    for (int i = 0 ; i< spawnerValues->size() ;i++){
        float spawnerX = json->get("objects")->get(i)->get("x")->asFloat();
        float spawnerY = json->get("objects")->get(i)->get("y")->asFloat();
        _spawnersPos.emplace_back(cugl::Vec2(spawnerX, spawnerY));
    }
    return true;
}






