//
//  BaseSet.cpp
//
//  Created by Andrew Cheng on 2/26/24.
//
#include "BaseSet.h"

BaseSet::BaseSet()
{
}
BaseSet::~BaseSet()
{
}

void BaseSet::update()
{
    for (auto &base : _bases)
    {
        base->update();
    }
}
int BaseSet::getFirstHealth()
{
    return (*_bases.begin())->getHealth();
}
bool BaseSet::init(std::shared_ptr<cugl::JsonValue> data,float scale)
{
    if (data)
    {
        _bases.clear();
        if (data->get("start"))
        {
            auto baseValues = data->get("start")->children();
            for (auto it = baseValues.begin(); it != baseValues.end(); it++)
            {
                std::shared_ptr<cugl::JsonValue> entry = (*it);
                cugl::Vec2 pos;
                // TEMPORARY MAGIC NUMBERS
                pos.x = entry->get(0)->get(0)->asFloat(0) / 64;
                pos.y = entry->get(0)->get(1)->asFloat(0) / 64;
                int health = entry->get(1)->asInt(0);
                _bases.emplace_back(std::make_shared<Base>(health, pos));
            }
        }
        _scale = scale;
        return true;
    }
    return false;
}

void BaseSet::setTexture(const std::shared_ptr<cugl::Texture> &value)
{
    _texture = value;
    for (auto& base : _bases){
        std::shared_ptr<cugl::scene2::SceneNode> baseNode = cugl::scene2::PolygonNode::allocWithTexture(_texture);
        baseNode->setPosition(base->getPos() * _scale);
        baseNode->setAnchor(cugl::Vec2::ANCHOR_CENTER);
        base->setSceneNode(baseNode);
    }
}
bool BaseSet::baseLost()
{
    for (auto &base : _bases)
    {
        if (base->lost())
        {
            return true;
        }
    }
    return false;
}
