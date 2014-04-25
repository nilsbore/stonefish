//
//  Joint.cpp
//  Stonefish
//
//  Created by Patryk Cieslak on 1/13/13.
//  Copyright (c) 2013 Patryk Cieslak. All rights reserved.
//

#include "Joint.h"

NameManager Joint::nameManager;

Joint::Joint(std::string uniqueName, bool collideLinkedEntities)
{
    name = nameManager.AddName(uniqueName);
    renderable = false;
    collisionEnabled = collideLinkedEntities;
}

Joint::~Joint(void)
{
    nameManager.RemoveName(name);
}

void Joint::setRenderable(bool render)
{
    renderable = render;
}

bool Joint::isRenderable()
{
    return renderable;
}

btTypedConstraint* Joint::getConstraint()
{
    return constraint;
}

std::string Joint::getName()
{
    return name;
}

void Joint::setConstraint(btTypedConstraint *constr)
{
    constraint = constr;
}

void Joint::AddToDynamicsWorld(btDynamicsWorld *world)
{
    btJointFeedback* fb = new btJointFeedback();
    constraint->enableFeedback(true);
    constraint->setJointFeedback(fb);
    
    world->addConstraint(constraint, !collisionEnabled);
}