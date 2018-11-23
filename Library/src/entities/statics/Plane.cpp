//
//  Plane.cpp
//  Stonefish
//
//  Created by Patryk Cieslak on 1/30/13.
//  Copyright(c) 2013-2017 Patryk Cieslak. All rights reserved.
//

#include "entities/statics/Plane.h"

#include "core/SimulationApp.h"

using namespace sf;

Plane::Plane(std::string uniqueName, btScalar planeSize, Material m, int lookId) : StaticEntity(uniqueName, m, lookId)
{
    btScalar size = UnitSystem::SetLength(planeSize);
    mesh = OpenGLContent::BuildPlane(size/2.f);
	
    btCollisionShape* shape = new btStaticPlaneShape(SimulationApp::getApp()->getSimulationManager()->isZAxisUp() ? btVector3(0,0,1) : btVector3(0,0,-1),0);
    BuildRigidBody(shape);
}

void Plane::GetAABB(btVector3 &min, btVector3 &max)
{
    //Plane shouldn't affect shadow calculation
    min.setValue(BT_LARGE_FLOAT, BT_LARGE_FLOAT, BT_LARGE_FLOAT);
    max.setValue(-BT_LARGE_FLOAT, -BT_LARGE_FLOAT, -BT_LARGE_FLOAT);
}

StaticEntityType Plane::getStaticType()
{
    return STATIC_PLANE;
}
