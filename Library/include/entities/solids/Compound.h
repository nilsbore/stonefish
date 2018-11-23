//
//  Compound.h
//  Stonefish
//
//  Created by Patryk Cieslak on 19/09/17.
//  Copyright (c) 2017 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_Compound__
#define __Stonefish_Compound__

#include "entities/SolidEntity.h"

namespace sf
{

typedef struct
{
    SolidEntity* solid;
    btTransform position;
    bool isExternal;
} Part;

class Compound : public SolidEntity
{
public:
    Compound(std::string uniqueName, SolidEntity* firstExternalPart, const btTransform& position);
    ~Compound();
	
	void AddInternalPart(SolidEntity* solid, const btTransform& position);
    void AddExternalPart(SolidEntity* solid, const btTransform& position);
	void RecalculatePhysicalProperties();
    void ComputeFluidForces(HydrodynamicsSettings settings, const Ocean* liquid);
	
    SolidType getSolidType();
    std::vector<Vertex>* getMeshVertices();
    btCollisionShape* BuildCollisionShape();
	void BuildGraphicalObject();
	
	std::vector<Renderable> Render();

private:
    std::vector<Part> parts; //Parts of the compound solid
};

}

#endif
