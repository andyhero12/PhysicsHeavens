//
//  SpawnerEnemy.h
//  Heaven
//
//  Created by Andrew Cheng on 4/9/24.
//

#ifndef SpawnerEnemy_h
#define SpawnerEnemy_h

#include <cugl/cugl.h>
#include "AbstractEnemy.h"


/**
 * The factory class for crate objects.
 *
 * This class is used to support automatically syncing newly added obstacle mid-simulation.
 * Obstacles added throught the ObstacleFactory class from one client will be added to all
 * clients in the simulations.
 */
class SpawnerEnemyFactory : public ObstacleFactory
{
public:
    
    SpawnerEnemyFactory(){
        _data = nullptr;
        _assets = nullptr;
        _audioController = nullptr;
    };
    virtual ~SpawnerEnemyFactory(){
        _data = nullptr;
        _assets = nullptr;
        _audioController = nullptr;
    }
    struct AnimationStruct{
        std::vector<std::shared_ptr<cugl::Texture>> _walkTextures;
        std::vector<std::shared_ptr<cugl::Texture>> _attackTextures;
        int _framesize;
        int _framecols;
        int _framesizeAttack;
        int _framecolsAttack;
        int _freqAnimations;
    };
    /** Pointer to the AssetManager for texture access, etc. */
    std::shared_ptr<cugl::JsonValue> _data;
    std::shared_ptr<cugl::AssetManager> _assets;
    AnimationStruct staticEnemyStruct;
    /** Serializer for supporting parameters */
    LWSerializer _serializer;
    /** Deserializer for supporting parameters */
    LWDeserializer _deserializer;
    std::shared_ptr<AudioController> _audioController;

    /**
     * Allocates a new instance of the factory using the given AssetManager.
     */
    static std::shared_ptr<SpawnerEnemyFactory> alloc(std::shared_ptr<cugl::JsonValue> data, std::shared_ptr<AssetManager> &assets, std::shared_ptr<AudioController> m_audioController)
    {
        auto f = std::make_shared<SpawnerEnemyFactory>();
        f->init(data, assets, m_audioController);
        return f;
    };

    /**
     * Initializes empty factories using the given AssetManager.
     */
    void init(std::shared_ptr<cugl::JsonValue> data, std::shared_ptr<AssetManager> &assets, std::shared_ptr<AudioController> m_audioController)
    {
        _data = data;
        _assets = assets;
        int _framecols = data->getFloat("sprite cols", 0);
        int _framesize = data->getFloat("sprite size", 0);
        std::vector<std::shared_ptr<cugl::Texture>> textures;
        textures.push_back(_assets->get<Texture>("spawnerFrontWalk"));
        textures.push_back(_assets->get<Texture>("spawnerFrontWalk"));
        textures.push_back(_assets->get<Texture>("spawnerFrontWalk"));
        textures.push_back(_assets->get<Texture>("spawnerFrontWalk"));
        textures.push_back(_assets->get<Texture>("spawnerFrontWalk"));
        textures.push_back(_assets->get<Texture>("spawnerFrontWalk"));
        textures.push_back(_assets->get<Texture>("spawnerFrontWalk"));
        textures.push_back(_assets->get<Texture>("spawnerFrontWalk"));
        staticEnemyStruct._walkTextures  = textures;
        
        std::vector<std::shared_ptr<cugl::Texture>> attacks;
        attacks.push_back(_assets->get<Texture>("spawnerSpawn"));
        attacks.push_back(_assets->get<Texture>("spawnerSpawn"));
        attacks.push_back(_assets->get<Texture>("spawnerSpawn"));
        attacks.push_back(_assets->get<Texture>("spawnerSpawn"));
        attacks.push_back(_assets->get<Texture>("spawnerSpawn"));
        attacks.push_back(_assets->get<Texture>("spawnerSpawn"));
        attacks.push_back(_assets->get<Texture>("spawnerSpawn"));
        attacks.push_back(_assets->get<Texture>("spawnerSpawn"));
        staticEnemyStruct._attackTextures  = attacks;
        staticEnemyStruct._framesize = _framesize;
        staticEnemyStruct._framecols = _framecols;
        staticEnemyStruct._framesizeAttack = 9;
        staticEnemyStruct._framecolsAttack = 9;
        staticEnemyStruct._freqAnimations = 10;
        _audioController = m_audioController;
    }

    /**
     * Generate a pair of Obstacle and SceneNode using the given parameters
     */
    std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> createObstacle(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);

    /**
     * Helper method for converting normal parameters into byte vectors used for syncing.
     */
    std::shared_ptr<std::vector<std::byte>> serializeParams(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);

    /**
     * Generate a pair of Obstacle and SceneNode using serialized parameters.
     */
    std::pair<std::shared_ptr<physics2::Obstacle>, std::shared_ptr<scene2::SceneNode>> createObstacle(const std::vector<std::byte> &params) override;
};


class SpawnerEnemy : public AbstractEnemy {
public:
    
    
    static std::shared_ptr<SpawnerEnemy> alloc(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
        std::shared_ptr<SpawnerEnemy> result = std::make_shared<SpawnerEnemy>();
        return (result->init(m_pos, m_size, m_health, m_targetIndex) ? result : nullptr);
    }
    SpawnerEnemy();
    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);
    
    virtual void preUpdate(float dt, OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return 0;
    }
    bool canAttack() const override{
        // Using This as Spawn
        return _attackCooldown >= 300;
    }
    
    virtual void resetAttack() override{
        _attackCooldown = 0;
    }
    virtual int getAbsorbValue() const override{
        return 5;
    }
    virtual ~SpawnerEnemy() {
        
    }
    
    
protected:
    int _attackCooldown;
    
    virtual void handleChase(OverWorld& overWorld) override;
    virtual void handleLowHealth(OverWorld& overWorld) override;
    virtual void handleAttack(OverWorld& overWorld) override;
    virtual void handleStay(OverWorld& overWorld) override;
};

#endif /* SpawnerEnemy_h */
