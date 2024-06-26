//
//  AbsorbEnemy.hpp
//  Heaven
//
//  Created by Lisa Asriev on 4/11/24.
//

#ifndef AbsorbEnemy_h
#define AbsorbEnemy_h

#include <stdio.h>
#include "AbstractEnemy.h"

/**
 * The factory class for crate objects.
 *
 * This class is used to support automatically syncing newly added obstacle mid-simulation.
 * Obstacles added throught the ObstacleFactory class from one client will be added to all
 * clients in the simulations.
 */
class AbsorbFactory : public ObstacleFactory
{
public:
    AbsorbFactory(){
        _data = nullptr;
        _assets = nullptr;
    };
    virtual ~AbsorbFactory(){
        _data = nullptr;
        _assets = nullptr;
    }
    struct AnimationStruct{
        std::vector<std::shared_ptr<cugl::Texture>> _walkTextures;
        std::vector<std::shared_ptr<cugl::Texture>> _attackTextures;
        int _framesize;
        int _framecols;
        int _freqAnimations;
        int _attackframesize;
        int _attackframecols;
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
    static std::shared_ptr<AbsorbFactory> alloc(std::shared_ptr<cugl::JsonValue> data, std::shared_ptr<AssetManager> &assets, std::shared_ptr<AudioController> m_audioController)
    {
        auto f = std::make_shared<AbsorbFactory>();
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
        int _framecols = data->getFloat("sprite cols", 0);
        int _framesize = data->getFloat("sprite size", 0);
        std::vector<std::shared_ptr<cugl::Texture>> textures;
        textures.push_back(_assets->get<Texture>("absorbEnemyRightWalk"));
        textures.push_back(_assets->get<Texture>("absorbEnemyRightWalk"));
        textures.push_back(_assets->get<Texture>("absorbEnemyLeftWalk"));
        textures.push_back(_assets->get<Texture>("absorbEnemyLeftWalk"));
        textures.push_back(_assets->get<Texture>("absorbEnemyLeftWalk"));
        textures.push_back(_assets->get<Texture>("absorbEnemyLeftWalk"));
        textures.push_back(_assets->get<Texture>("absorbEnemyRightWalk"));
        textures.push_back(_assets->get<Texture>("absorbEnemyRightWalk"));
        staticEnemyStruct._walkTextures  = textures;
        staticEnemyStruct._framesize = _framesize;
        staticEnemyStruct._framecols = _framecols;

        int _attackframecols = data->getFloat("sprite cols lick", 0);
        int _attackframesize = data->getFloat("sprite size lick", 0);
        std::vector<std::shared_ptr<cugl::Texture>> attacks;
        attacks.push_back(_assets->get<Texture>("absorbEnemyRightLick"));
        attacks.push_back(_assets->get<Texture>("absorbEnemyRightLick"));
        attacks.push_back(_assets->get<Texture>("absorbEnemyLeftLick"));
        attacks.push_back(_assets->get<Texture>("absorbEnemyLeftLick"));
        attacks.push_back(_assets->get<Texture>("absorbEnemyLeftLick"));
        attacks.push_back(_assets->get<Texture>("absorbEnemyLeftLick"));
        attacks.push_back(_assets->get<Texture>("absorbEnemyRightLick"));
        attacks.push_back(_assets->get<Texture>("absorbEnemyRightLick"));
        staticEnemyStruct._attackTextures  = attacks;
        staticEnemyStruct._attackframesize = _attackframesize;
        staticEnemyStruct._attackframecols = _attackframecols;

        staticEnemyStruct._freqAnimations = 4;
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


class AbsorbEnemy : public AbstractEnemy {
public:
    float _defaultScale;
    float _defaultDimension;
    int _defaultHealth;
    
    static std::shared_ptr<AbsorbEnemy> alloc(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex) {
        std::shared_ptr<AbsorbEnemy> result = std::make_shared<AbsorbEnemy>();
        return (result->init(m_pos, m_size, m_health, m_targetIndex) ? result : nullptr);
    }
    AbsorbEnemy();
    
    bool init(cugl::Vec2 m_pos, cugl::Size m_size, int m_health, int m_targetIndex);
    virtual ~AbsorbEnemy() {
        
    }
    
    virtual void preUpdate(float dt, OverWorld& overWorld) override;
    
    virtual int getDamage() override{
        return _contactDamage;
    }
    
    void setDamage(int damage){
        _contactDamage += damage;
    }
    
    void increaseHealth(int inc_health){
        _health += inc_health;

        if(_health > _maxHealth && _health > _defaultHealth) {
            _health = min((double)_health / _defaultHealth, 9.0) * _defaultHealth;
            _health++;
        }

        _maxHealth = max(_maxHealth,_health);

        //CULog(("new absorb hfakjdfhasjldf " + std::to_string(_maxHealth) + " " + std::to_string(_defaultHealth)).c_str());

        float ratio = sqrt((float)_maxHealth / _defaultHealth);
        float dim = ratio * _defaultDimension;
        cugl::Size nxtSize(dim,dim);
        setDimension(nxtSize);
        topLevelPlaceHolder->setScale(ratio * _defaultScale);
        _healthBar->setProgress((float)_health/_maxHealth);
    }
    
    bool canAttack() const override{
        return _attackCooldown >= 180;
    }
    
    virtual void resetAttack() override{
        _attackCooldown = 0;
    }
    
    virtual int getAbsorbValue() const override{
        CULog("TODO ABSORB VALUE OF ABSORB ENEMY \n");
        return 1;
    }
 
protected:
    int _contactDamage;
    int _attackCooldown;
    
    
    virtual void handleChase(OverWorld& overWorld) override;
    virtual void handleLowHealth(OverWorld& overWorld) override;
    virtual void handleAttack(OverWorld& overWorld) override;
    virtual void handleStay(OverWorld& overWorld) override;
    virtual void handleRunaway(OverWorld& overWorld) override;
    
    
};

#endif /* AbsorbEnemy_h */
