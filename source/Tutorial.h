//
//  Tutorial.hpp
//  Heaven
//
//  Created by Henry Gao on 4/23/24.
//

#ifndef Tutorial_h
#define Tutorial_h

#include <stdio.h>
#include <functional>
#include <stdexcept>
#include <string>
#include <cugl/cugl.h>
#include "SpriteAnimationNode.h"

class Tutorial
{
public:
    enum MODE
    {
        GREETING = 0,
        MOVEMENT = 1,
        DASH = 2,
        BITE = 3,
        GROW = 4,
        DESTROYSPAWNER = 5,
        RECALLGIVE = 6,
        SPECIALSONE = 7,
        DEFENDGATE = 8,
        CHANGEABILITYTWO = 9,
        SPECIALSTWO = 10,
        BARKGIVE = 11,
        CHANGEABILITYTHREE = 12,
        SPECIALSTHREE = 13,
        BAITGIVE = 14,
        CHANGEABILITYFOUR = 15,
        BOMBGIVE = 16,
        SPECIALSFOUR = 17,
        SPEECH = 18

    };


protected:
    bool laugh;
    int tileX;
    bool passed;
    MODE progress;
    bool enter;
    std::shared_ptr<SpriteAnimationNode> sprite;
    std::shared_ptr<SpriteAnimationNode> spriteRepeat;
    std::shared_ptr<SpriteAnimationNode> pressA;
    std::shared_ptr<cugl::scene2::PolygonNode> textBox;
    std::shared_ptr<cugl::scene2::Label> message;
    std::string text;

public:
    Tutorial() {}
    bool init(int tile, MODE m_progress, std::string m_text, bool val);
    bool getLaughing(){return laugh;}
    static std::shared_ptr<Tutorial> alloc(int tile, MODE progress, std::string text)
    {
        std::shared_ptr<Tutorial> node = std::make_shared<Tutorial>();
        return (node->init(tile, progress, text, false) ? node : nullptr);
    }
    
    static std::shared_ptr<Tutorial> alloc(int tile, MODE progress, std::string text, bool val)
    {
        std::shared_ptr<Tutorial> node = std::make_shared<Tutorial>();
        return (node->init(tile, progress, text, val) ? node : nullptr);
    }
    
    int getX() const { return tileX; }
    bool didPass() const { return passed; }
    MODE getProgress() const { return progress; }
    void setPass(bool change) { passed = change; }
    // returns true if your x coordinate is at the right spot
    bool atArea(int tile);

    void setSprite(std::shared_ptr<SpriteAnimationNode> node)
    {
        sprite = node;
    }
    
    void setTextBox(std::shared_ptr<cugl::scene2::PolygonNode> node)
    {
        textBox = node;
    }
    
    void setSpriteRepeat(std::shared_ptr<SpriteAnimationNode> node)
    {
        spriteRepeat = node;
    }
    
    void setPressButton(std::shared_ptr<SpriteAnimationNode> node)
    {
        pressA = node;
    }

    std::shared_ptr<SpriteAnimationNode> getSprite() const
    {
        return sprite;
    }
    std::shared_ptr<SpriteAnimationNode> getSpriteRepeat() const
    {
        return spriteRepeat;
    }

    std::shared_ptr<SpriteAnimationNode> getPressA() const
    {
        return pressA;
    }
    void setMessage(std::shared_ptr<cugl::scene2::Label> node)
    {
        message = node;
    }

    std::shared_ptr<cugl::scene2::Label> getMessage() const
    {
        return message;
    }

    std::string getText() const
    {
        return text;
    }
};

#endif /* tutorial_hpp */
