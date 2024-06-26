//
//  NLDeathEvent.cpp
//  Networked Physics Lab
//
//  This class represents an event of creating an extra-large crate
//
//  Created by Andrew Cheng.
//

#include "NLDeathEvent.h"

/**
 * This method is used by the NetEventController to create a new event of using a
 * reference of the same type.
 *
 * Not that this method is not static, it differs from the static alloc() method
 * and all methods must implement this method.
 */
std::shared_ptr<NetEvent> DeathEvent::newEvent(){
    return std::make_shared<DeathEvent>();
}

std::shared_ptr<NetEvent> DeathEvent::allocDeathEvent(Vec2 pos, bool isHost, Size mSize, bool isBomb, bool isGate){
    //TODO: make a new shared copy of the event and set its _pos to pos.
#pragma mark BEGIN SOLUTION
    auto event = std::make_shared<DeathEvent>();
    event->_pos = pos;
    event->_isHost = isHost;
    event->_size = mSize;
    event->_isBomb = isBomb;
    event->_isGate = isGate;
    return event;
#pragma mark END SOLUTION
}

/**
 * Serialize any paramater that the event contains to a vector of bytes.
 */
std::vector<std::byte> DeathEvent::serialize(){
    //TODO: serialize _pos
#pragma mark BEGIN SOLUTION
    _serializer.reset();
    _serializer.writeFloat(_pos.x);
    _serializer.writeFloat(_pos.y);
    _serializer.writeFloat(_size.width);
    _serializer.writeFloat(_size.height);
    _serializer.writeBool(_isHost);
    _serializer.writeBool(_isBomb);
    _serializer.writeBool(_isGate);
    return _serializer.serialize();
#pragma mark END SOLUTION
}
/**
 * Deserialize a vector of bytes and set the corresponding parameters.
 *
 * @param data  a byte vector packed by serialize()
 *
 * This function should be the "reverse" of the serialize() function: it
 * should be able to recreate a serialized event entirely, setting all the
 * useful parameters of this class.
 */
void DeathEvent::deserialize(const std::vector<std::byte>& data){
    //TODO: deserialize data and set _pos
    //NOTE: You might be tempted to write Vec2(_deserializer.readFloat(),_deserializer.readFloat()), however, C++ doesn't specify the order in which function arguments are evaluated, so you might end up with <y,x> instead of <x,y>.
    
#pragma mark BEGIN SOLUTION
    _deserializer.reset();
    _deserializer.receive(data);
    float x = _deserializer.readFloat();
    float y = _deserializer.readFloat();
    _pos = Vec2(x,y);
    
    float w = _deserializer.readFloat();
    float h = _deserializer.readFloat();
    _size = Size(w, h);
    _isHost = _deserializer.readBool();
    _isBomb = _deserializer.readBool();
    _isGate = _deserializer.readBool();
#pragma mark END SOLUTION
}
