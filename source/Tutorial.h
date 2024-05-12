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
        DEFENDGATE = 2,
        BITE = 3,
        GROW = 4,
        DESTROYSPAWNER = 5,
        DASH = 6,
        SPECIALSONE = 7,
        CHANGEABILITYTWO = 8,
        SPECIALSTWO = 9,
        CHANGEABILITYTHREE = 10,
        SPECIALSTHREE = 11,
        CHANGEABILITYFOUR = 12,
        SPECIALSFOUR = 13
    };

    static std::string toString(MODE mode)
    {
        switch (mode)
        {
        case GREETING:
            return "GREETING";
        case MOVEMENT:
            return "MOVEMENT";
        case BITE:
            return "BITE";
        case DEFENDGATE:
            return "DEFENDGATE";
        case GROW:
            return "GROW";
        case DASH:
            return "DASH";
        case SPECIALSONE:
            return "SPECIALSONE";
        case DESTROYSPAWNER:
            return "DESTROYSPAWNER";
        case CHANGEABILITYTWO:
            return "CHANGEABILITYTWO";
        case SPECIALSTWO:
            return "SPECIALSTWO";
        case CHANGEABILITYTHREE:
            return "CHANGEABILITYTHREE";
        case SPECIALSTHREE:
            return "SPECIALSTHREE";
        case CHANGEABILITYFOUR:
            return "CHANGEABILITYFOUR";
        case SPECIALSFOUR:
            return "SPECIALSFOUR";
        default:
            return "Unknown Progress";
        }
    }
    static MODE fromString(const std::string &progressString)
    {
        if (progressString == "GREETING")
        {
            return GREETING;
        }
        else if (progressString == "MOVEMENT")
        {
            return MOVEMENT;
        }
        else if (progressString == "BITE")
        {
            return BITE;
        }
        else if (progressString == "DEFENDGATE")
        {
            return DEFENDGATE;
        }
        else if (progressString == "GROW")
        {
            return GROW;
        }
        else if (progressString == "DASH")
        {
            return DASH;
        }
        else if (progressString == "SPECIALSONE")
        {
            return SPECIALSONE;
        }
        else if (progressString == "DESTROYSPAWNER")
        {
            return DESTROYSPAWNER;
        }
        else if (progressString == "CHANGEABILITYTWO")
        {
            return CHANGEABILITYTWO;
        }
        else if (progressString == "SPECIALSTWO")
        {
            return SPECIALSTWO;
        }
        else if (progressString == "CHANGEABILITYTHREE")
        {
            return CHANGEABILITYTHREE;
        }
        else if (progressString == "SPECIALSTHREE")
        {
            return SPECIALSTHREE;
        }
        else if (progressString == "CHANGEABILITYFOUR")
        {
            return CHANGEABILITYFOUR;
        }
        else if (progressString == "SPECIALSFOUR")
        {
            return SPECIALSFOUR;
        }
        else
        {
            throw std::invalid_argument("Invalid progress string: " + progressString);
        }
    }

protected:
    int tileX;
    bool passed;
    MODE progress;
    bool enter;
    std::shared_ptr<SpriteAnimationNode> sprite;
    std::shared_ptr<cugl::scene2::Label> message;
    std::string text;

public:
    Tutorial() {}
    bool init(int tile, MODE progress, std::string m_text);
    static std::shared_ptr<Tutorial> alloc(int tile, MODE progress, std::string text)
    {
        std::shared_ptr<Tutorial> node = std::make_shared<Tutorial>();
        return (node->init(tile, progress, text) ? node : nullptr);
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

    std::shared_ptr<SpriteAnimationNode> getSprite() const
    {
        return sprite;
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
