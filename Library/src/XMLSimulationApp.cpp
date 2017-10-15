//
//  XMLSimulationApp.cpp
//  Stonefish
//
//  Created by Patryk Cieslak on 11/10/12.
//  Copyright (c) 2017 Patryk Cieslak. All rights reserved.
//

#include "XMLSimulationApp.h"

XMLSimulationApp::XMLSimulationApp(std::string name, std::string xmlPath, std::string dataDirPath, std::string shaderDirPath, int windowWidth, int windowHeight, SimulationManager* sim)
    : SimulationApp(name, dataDirPath, shaderDirPath, windowWidth, windowHeight, sim)
{
    
}