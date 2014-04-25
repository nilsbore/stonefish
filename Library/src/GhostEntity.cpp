//
//  GhostEntity.cpp
//
//
//  Created by Patryk Cieslak on 1/3/13.
//  Copyright (c) 2013 Patryk Cieslak. All rights reserved.
//

#include "GhostEntity.h"

GhostEntity::GhostEntity(std::string uniqueName) : Entity(uniqueName)
{
    ghost = new btPairCachingGhostObject();
    ghost->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);
}

GhostEntity::~GhostEntity()
{
}

EntityType GhostEntity::getType()
{
    return GHOST;
}

btPairCachingGhostObject* GhostEntity::getGhost()
{
    return ghost;
}

btTransform GhostEntity::getTransform()
{
    return ghost->getWorldTransform();
}

void GhostEntity::setTransform(const btTransform &trans)
{
    ghost->setWorldTransform(trans);
}

void GhostEntity::AddToDynamicsWorld(btDynamicsWorld *world)
{
    world->addCollisionObject(ghost);
}