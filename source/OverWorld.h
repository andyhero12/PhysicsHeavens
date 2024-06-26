//
//  OverWorld.hpp
//  Ship
//
//  Created by Andrew Cheng on 3/14/24.
//
#pragma once
#ifndef OverWorld_hpp
#define OverWorld_hpp
#include <cugl/cugl.h>
#include <unordered_set>
#include "Base.h"
#include "NLDog.h"
#include "Animation.h"
#include "BaseSet.h"
#include "NLInput.h"
#include "AttackPolygons.h"
#include "DecoySet.h"
#include "NLLevelModel.h"
#include "NLDecoyEvent.h"
#include "NLRecallEvent.h"
#include "NLBiteEvent.h"
#include "NLExplodeEvent.h"
#include "NLBaseHealthEvent.h"
#include "NLShootEvent.h"
#include "World.h"
#include "NLDashEvent.h"
#include "NLSizeEvent.h"
#include "NLClientHealthEvent.h"
#include "Constants.h"
#include "AudioController.h"
#include "GateUIController.h"

class OverWorld
{
private:
    std::shared_ptr<LevelModel> _level;
    std::shared_ptr<NetEventController> _network;
    cugl::Size _activeSize;
    std::shared_ptr<Dog> _dog;
    std::shared_ptr<Dog> _dogClient;
    std::shared_ptr<DecoySet> _decoys;
    std::shared_ptr<BaseSet> _bases;
    std::shared_ptr<cugl::JsonValue> _constants;
    std::shared_ptr<cugl::AssetManager> _assets;
    AttackPolygons _attackPolygonSet;
    AttackPolygons _clientAttackPolygonSet;
    std::shared_ptr<World> _world;
    std::shared_ptr<AudioController> _audioController;

    int dogSeq;
    std::shared_ptr<GateUIController> _gateUIController;

public:
    bool _isHost;
    OverWorld()
    {
    }
    ~OverWorld()
    {
    }

    void reset();
    bool init(const std::shared_ptr<cugl::AssetManager> &assets, const std::shared_ptr<LevelModel> &_level, cugl::Size activeSize, std::shared_ptr<cugl::physics2::net::NetEventController> network, bool isHost, std::shared_ptr<World> world, std::shared_ptr<AudioController> audioController);
    bool initDogModel();
    bool initBases();
    bool initDecoys();
    bool initPolygons();
    bool initWorld();

    bool setRootNode(const std::shared_ptr<scene2::SceneNode> &_worldNode, const std::shared_ptr<scene2::SceneNode> &_debugNode, std::shared_ptr<cugl::physics2::net::NetWorld> _world);
    // will add Obstacle nodes too
    void dogUpdate(InputController &_input, cugl::Size totalSize);
    void update(InputController &input, cugl::Size totalSize, float timestep, bool b);
    void postUpdate();
    void ownedDogUpdate(InputController &_input, cugl::Size, std::shared_ptr<Dog> _curDog);
    void processShootEvent(const std::shared_ptr<ShootEvent> &shootEvent);
    void processDecoyEvent(const std::shared_ptr<DecoyEvent> &decoyEvent);
    void processSizeEvent(const std::shared_ptr<SizeEvent> &sizeEvent);
    void processBiteEvent(const std::shared_ptr<BiteEvent> &biteEvent);
    void processRecallEvent(const std::shared_ptr<RecallEvent> &recallEvent);
    void processExplodeEvent(const std::shared_ptr<ExplodeEvent> &explodeEvent);
    void processDashEvent(const std::shared_ptr<DashEvent> &dashEvent);
    void processBaseHealthEvent(const std::shared_ptr<BaseHealthEvent> &basEvent);
    void processClientHealthEvent(const std::shared_ptr<ClientHealthEvent> &clientHealthEvent);
    void recallDogToClosetBase(std::shared_ptr<Dog> _curDog);
    void draw(const std::shared_ptr<cugl::SpriteBatch> &batch, cugl::Size totalSize);
    std::shared_ptr<Dog> getDog() const
    {
        return _dog;
    }
    std::shared_ptr<Dog> getClientDog() const
    {
        return _dogClient;
    }
    std::shared_ptr<DecoySet> getDecoys() const
    {
        return _decoys;
    }
    std::shared_ptr<BaseSet> getBaseSet()
    {
        return _bases;
    }
    std::shared_ptr<NetEventController> getNetwork()
    {
        return _network;
    };
    std::shared_ptr<LevelModel> getLevelModel()
    {
        return _level;
    }
    World *getWorld()
    {
        return _world.get();
    }

    int getTotalTargets() const
    {
        return 1 + (_network->getNumPlayers() == 2)  + (int)_bases->_bases.size() + (int)_decoys->getCurrentDecoys().size();
    }
    AttackPolygons &getAttackPolygons()
    {
        return _attackPolygonSet;
    }
    AttackPolygons &getAttackPolygonsClient()
    {
        return _clientAttackPolygonSet;
    }
    std::shared_ptr<GateUIController> getGateUIController()
    {
        return _gateUIController;
    }
    void dispose();

    void playSound(std::string key, std::string sound);
};
#endif /* OverWorld_hpp */
