//
//  StaticMeleeEnemy.hpp
//  Heavan
//
//  Created by Andrew Cheng on 3/24/24.
//
#pragma once
#ifndef StaticMeleeEnemy_hpp
#define StaticMeleeEnemy_hpp

#include "MeleeEnemy.h"

/**
 * The factory class for crate objects.
 *
 * This class is used to support automatically syncing newly added obstacle mid-simulation.
 * Obstacles added throught the ObstacleFactory class from one client will be added to all
 * clients in the simulations.
 */
class StaticMeleeFactory : public ObstacleFactory
{
public:
    StaticMeleeFactory(){
        _data = nullptr;
        _assets = nullptr;
    };
    virtual ~StaticMeleeFactory(){
        _data = nullptr;
        _assets = nullptr;
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
    static std::shared_ptr<StaticMeleeFactory> alloc(std::shared_ptr<cugl::JsonValue> data, std::shared_ptr<AssetManager> &assets, std::shared_ptr<AudioController> m_audioController)
    {
        auto f = std::make_shared<StaticMeleeFactory>();
        f->init(data, assets, m_audioController);
        return f;
    };

    /**
     * Initializes empty factories using the given AssetManager.
     */
    void init(std::shared_ptr<cugl::JsonValue> data, std::shared_ptr<AssetManager> &assets, std::shared_ptr<AudioController> m_audioController)
    {
        _audioController = m_audioController;
        _data = data;
        _assets = assets;
        int _framecols = 4;
        int _framesize = 4;
        std::vector<std::shared_ptr<cugl::Texture>> textures;
        textures.push_back(_assets->get<Texture>("basicRightWalk"));
        textures.push_back(_assets->get<Texture>("basicRightWalk"));
        textures.push_back(_assets->get<Texture>("basicLeftWalk"));
        textures.push_back(_assets->get<Texture>("basicLeftWalk"));
        textures.push_back(_assets->get<Texture>("basicLeftWalk"));
        textures.push_back(_assets->get<Texture>("basicUpLeftWalk"));
        textures.push_back(_assets->get<Texture>("basicUpRightWalk"));
        textures.push_back(_assets->get<Texture>("basicRightWalk"));
        staticEnemyStruct._walkTextures  = textures;
        
        std::vector<std::shared_ptr<cugl::Texture>> attacks;
        attacks.push_back(_assets->get<Texture>("basicRightAttack"));
        attacks.push_back(_assets->get<Texture>("basicRightAttack"));
        attacks.push_back(_assets->get<Texture>("basicLeftAttack"));
        attacks.push_back(_assets->get<Texture>("basicLeftAttack"));
        attacks.push_back(_assets->get<Texture>("basicLeftAttack"));
        attacks.push_back(_assets->get<Texture>("basicUpLeftAttack"));
        attacks.push_back(_assets->get<Texture>("basicUpRightAttack"));
        attacks.push_back(_assets->get<Texture>("basicRightAttack"));
        staticEnemyStruct._attackTextures  = attacks;
        staticEnemyStruct._framesize = _framesize;
        staticEnemyStruct._framecols = _framecols;
        staticEnemyStruct._framesizeAttack = 5;
        staticEnemyStruct._framecolsAttack = 5;
        staticEnemyStruct._freqAnimations = 7;
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


class StaticMeleeEnemy: public MeleeEnemy{
    
public:
    static std::shared_ptr<StaticMeleeEnemy> alloc(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
        std::shared_ptr<StaticMeleeEnemy> result = std::make_shared<StaticMeleeEnemy>();
        return (result->init(m_pos, m_size, m_health, m_targetIndex) ? result : nullptr);
    }
    StaticMeleeEnemy();
    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);
    virtual ~StaticMeleeEnemy(){
        
    }
    
    virtual void preUpdate(float dt, OverWorld& overWorld) override;
    
    
private:
    cugl::Vec2 original_pos;
    
protected:
    // update state
    virtual void handleChase(OverWorld& overWorld) override;
    virtual void handleLowHealth(OverWorld& overWorld) override;
    virtual void handleAttack(OverWorld& overWorld) override;
    virtual void handleStay(OverWorld& overWorld) override;
    virtual void handleRunaway(OverWorld& overWorld) override;
};

#endif /* StaticMeleeEnemy_hpp */
