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
    _levelHeight = json->get("height")->asFloat();
    _levelWidth = json->get("width")->asFloat();
    _tileHeight = json->get("tileheight")->asFloat();
    _tileWidth = json->get("tilewidth")->asFloat();
    auto tileSetArray = json->get("tilesets");
    for (int i =0 ; i< tileSetArray->size(); i++){
        const std::shared_ptr<cugl::JsonValue>& tileImage = tileSetArray->get(i);
        int firstGid = tileImage->get("firstgid")->asInt();
        std::string source = tileImage->get("source")->asString();
        std::string::size_type pos = source.find('.');
        if (pos != std::string::npos){
            source = source.substr(0, pos);
        }else{
            CUAssertLog(false, "failed with %s not ending with Json", source.data());
        }
        tileSetMapping.insert({firstGid,source});
    }
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
        auto health = json->get("objects")->get(i)->get("properties")->get(0)->get("value")->asFloat();
        _basesPos.emplace_back(cugl::Vec3(baseX/_tileWidth,(_levelHeight * _tileHeight - baseY)/_tileWidth, health));
    }
    return true;
}

bool LevelModel::loadPreSpawnedClusters(const std::shared_ptr<JsonValue>& json) {
    auto clusters = json->get("objects");
    for (int i = 0 ; i< clusters->size() ;i++){
        float spawnX = json->get("objects")->get(i)->get("x")->asFloat();
        float spawnY = json->get("objects")->get(i)->get("y")->asFloat();
        auto spawnNum = json->get("objects")->get(i)->get("properties")->get(0)->get("value")->asFloat();
         
        _preSpawnLocs.emplace_back(cugl::Vec3(spawnX/_tileWidth,(_levelHeight * _tileHeight - spawnY)/_tileWidth, spawnNum));
    }
    return true;
}
bool LevelModel::loadTiles(const std::shared_ptr<JsonValue>& json) {
    int columns = json->get("width")->asInt();
    int rows = json->get("height")->asInt();
    auto array = json->get("data")->children();
    _tiles.resize(rows,std::vector<int>(columns,0));
    for (int i =0 ; i< rows; i++){
        for (int j =0 ;j <columns ;j ++){
            _tiles.at(rows - i - 1).at(j) = array.at(i*columns + j)->asInt();
        }
    }
    return true;
}

bool LevelModel::loadBoundaries(const std::shared_ptr<JsonValue>& json) {
    int columns = json->get("width")->asInt();
    int rows = json->get("height")->asInt();
    auto array = json->get("data")->children();
    _walls.resize(rows,std::vector<int>(columns,0));
    for (int i =0 ; i< rows; i++){
        for (int j =0 ;j <columns ;j ++){
            _walls.at(rows - i - 1).at(j) = array.at(i*columns + j)->asInt();
        }
    }
    return true;
}

bool LevelModel::loadDecorations(const std::shared_ptr<JsonValue>& json){
    int columns = json->get("width")->asInt();
    int rows = json->get("height")->asInt();
    auto array = json->get("data")->children();
    _decors.resize(rows,std::vector<int>(columns,0));
    for (int i =0 ; i< rows; i++){
        for (int j =0 ;j <columns ;j ++){
            _decors.at(rows - i - 1).at(j) = array.at(i*columns + j)->asInt();
        }
    }
    return true;
}


bool LevelModel::loadPlayer(const std::shared_ptr<JsonValue>& json){
    float playerX = json->get("objects")->get(0)->get("x")->asFloat();
    float playerY = json->get("objects")->get(0)->get("y")->asFloat();
    _playerPos = cugl::Vec2(playerX/_tileWidth, (_levelHeight * _tileHeight  -  playerY)/_tileWidth);
    return false;
}

bool LevelModel::loadSpanwerLocations(const std::shared_ptr<JsonValue>& json){
    auto spawnerValues = json->get("objects");
    for (int i = 0 ; i< spawnerValues->size() ;i++){
        float spawnerX = json->get("objects")->get(i)->get("x")->asFloat();
        float spawnerY = json->get("objects")->get(i)->get("y")->asFloat();
        float hp;
        float initDelay;
        float regularDelay;
        std::string primaryEnemy;
        std::string secondaryEnemy;
        std::string tertiaryEnemy;
        std::shared_ptr<JsonValue> properties = json->get("objects")->get(i)->get("properties");
        for (int i = 0; i < properties->size(); i++){
            std::string name = properties->get(i)->get("name")->asString();
            if(name == "HP") {
                hp = properties->get(i)->get("value")->asFloat();
            }else if (name == "InitDelay") {
                initDelay = properties->get(i)->get("value")->asFloat();
            }else if (name == "PrimaryEnemy"){
                primaryEnemy = properties->get(i)->get("value")->asString();
            }else if (name == "RegDelay"){
                regularDelay = properties->get(i)->get("value")->asFloat();
            }else if (name == "SecondaryEnemy"){
                secondaryEnemy = properties->get(i)->get("value")->asString();
            }else if (name == "TertiaryEnemy"){
                tertiaryEnemy = properties->get(i)->get("value")->asString();
            }
        }
        Spawner spawner = { spawnerX / _tileWidth, (_levelHeight * _tileHeight - spawnerY) / _tileWidth, hp, initDelay, regularDelay, primaryEnemy, secondaryEnemy, tertiaryEnemy };
        _spawnersPos.emplace_back(spawner);
    }
    return true;
}






