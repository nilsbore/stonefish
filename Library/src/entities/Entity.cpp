//
//  Entity.cpp
//  Stonefish
//
//  Created by Patryk Cieslak on 11/28/12.
//  Copyright (c) 2012 Patryk Cieslak. All rights reserved.
//

#include "entities/Entity.h"

#include "core/SimulationApp.h"

btVector3 Entity::findInertiaAxis(btMatrix3x3 I, btScalar value)
{
	//Check if not I matrix already diagonal
	if(btFuzzyZero(I.getRow(0).getY()) && btFuzzyZero(I.getRow(0).getZ())
		&& btFuzzyZero(I.getRow(1).getX()) && btFuzzyZero(I.getRow(1).getZ())
		&& btFuzzyZero(I.getRow(2).getX()) && btFuzzyZero(I.getRow(2).getY()))
		{
			return btVector3(0,0,0);
		}
	
	//Diagonalize
    btMatrix3x3 L;
    btVector3 candidates[3];
    btVector3 axis;
    
    //Characteristic matrix
    L = I - btMatrix3x3::getIdentity().scaled(btVector3(value,value,value));
    
    //Candidates (orthogonal vectors)
    candidates[0] = (L.getRow(0).cross(L.getRow(1)));
    candidates[1] = (L.getRow(0).cross(L.getRow(2)));
    candidates[2] = (L.getRow(1).cross(L.getRow(2)));
    
    //Find best candidate
    if(candidates[0].length2() >= candidates[1].length2())
    {
        if(candidates[0].length2() >= candidates[2].length2())
            axis = candidates[0].normalized();
        else
            axis = candidates[2].normalized();
    }
    else
    {
        if(candidates[1].length2() >= candidates[2].length2())
            axis = candidates[1].normalized();
        else
            axis = candidates[2].normalized();
    }
    
    return axis;
}

//Class
Entity::Entity(std::string uniqueName)
{
    name = SimulationApp::getApp()->getSimulationManager()->getNameManager()->AddName(uniqueName);
    renderable = true;
}

Entity::~Entity(void)
{
    SimulationApp::getApp()->getSimulationManager()->getNameManager()->RemoveName(name);
}

void Entity::setRenderable(bool render)
{
    renderable = render;
}

bool Entity::isRenderable()
{
    return renderable;
}

std::string Entity::getName()
{
    return name;
}