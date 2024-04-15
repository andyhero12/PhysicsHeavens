//
//  BaseSet.cpp
//
//  Created by Andrew Cheng on 2/26/24.
//
#include "BaseSet.h"
#include <limits>

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
bool BaseSet::init(const std::vector<cugl::Vec3>& basePoses)
{
    _bases.clear();
    for (const cugl::Vec3& base : basePoses){
        _bases.emplace_back(std::make_shared<Base>(base.z, cugl::Vec2(base.x,base.y)));
    }
    return true;
}


bool BaseSet::init(std::shared_ptr<cugl::JsonValue> data)
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
                pos.x = entry->get(0)->get(0)->asFloat(0);
                pos.y = entry->get(0)->get(1)->asFloat(0);
                int health = entry->get(1)->asInt(0);
                _bases.emplace_back(std::make_shared<Base>(health, pos));
            }
        }
        return true;
    }
    return false;
}

void BaseSet::setTexture(const std::shared_ptr<cugl::Texture> &value)
{
    _texture = value;
    for (auto& base : _bases){
        std::shared_ptr<cugl::scene2::SceneNode> baseNode = cugl::scene2::PolygonNode::allocWithTexture(_texture);
        baseNode->setContentSize(cugl::Vec2(1, 1));
        baseNode->setPosition(base->getPos());
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

void BaseSet::recallBase(std::shared_ptr<Dog> dog){
    float shortestDist = std::numeric_limits<float>::max();
    std::shared_ptr<Base> nearest;
    for (auto& base : _bases){
        Vec2 norm = base->getPos() - dog->getPosition();
        float dist = norm.length();
        if (dist < shortestDist){
            shortestDist = dist;
            nearest = base;
        }
    }
//    CULog("Nearest base x: %fu", nearest->getPos().x);
//    CULog("Nearest base y: %fu", nearest->getPos().y);
    dog->setPosition(nearest->getPos());
}
