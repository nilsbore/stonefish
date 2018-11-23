//
//  FallingTestManager.cpp
//  Stonefish
//
//  Created by Patryk Cieslak on 04/03/2014.
//  Copyright (c) 2014-2018 Patryk Cieslak. All rights reserved.
//

#include "FallingTestManager.h"

#include "FallingTestApp.h"
#include <core/Robot.h>
#include <utils/SystemUtil.hpp>
#include <entities/statics/Plane.h>
#include <entities/statics/Obstacle.h>
#include <entities/solids/Polyhedron.h>
#include <entities/solids/Box.h>
#include <entities/solids/Sphere.h>
#include <entities/solids/Torus.h>
#include <entities/solids/Cylinder.h>
#include <graphics/OpenGLContent.h>
#include <sensors/scalar/IMU.h>
#include <sensors/scalar/RotaryEncoder.h>

FallingTestManager::FallingTestManager(btScalar stepsPerSecond) : SimulationManager(sf::UnitSystems::MKS, true, stepsPerSecond, sf::SI, sf::EXCLUSIVE)
{
}

void FallingTestManager::BuildScenario()
{
    sf::OpenGLPipeline* glPipeline = ((sf::GraphicalSimulationApp*)sf::SimulationApp::getApp())->getGLPipeline();
    glPipeline->setVisibleHelpers(true, false, false, false, true, false, false);
	glPipeline->setDebugSimulation(false);
    setICSolverParams(false);
	
    ///////MATERIALS////////
    getMaterialManager()->CreateMaterial("Ground", 1000.0, 1.0);
    getMaterialManager()->CreateMaterial("Steel", sf::UnitSystem::Density(sf::CGS, sf::MKS, 1.0), 0.1);
    getMaterialManager()->SetMaterialsInteraction("Ground", "Ground", 0.5, 0.3);
    getMaterialManager()->SetMaterialsInteraction("Ground", "Steel", 0.5, 0.3);
    getMaterialManager()->SetMaterialsInteraction("Steel", "Steel", 0.5, 0.3);
    
	///////LOOKS///////////
    int grid = sf::OpenGLContent::getInstance()->CreateSimpleLook(glm::vec3(1.f, 1.f, 1.f), 0.f, 0.1f, sf::GetShaderPath() + "grid.png");
    int yellow = sf::OpenGLContent::getInstance()->CreatePhysicalLook(glm::vec3(1.f, 0.6f, 0.2f), 0.2f, 0.f);
    int green = sf::OpenGLContent::getInstance()->CreatePhysicalLook(glm::vec3(0.3f, 1.0f, 0.2f), 0.2f, 0.f);
	
    ////////OBJECTS
    sf::Plane* floor = new sf::Plane("Floor", 1000.f, getMaterialManager()->getMaterial("Ground"), grid);
    AddStaticEntity(floor, btTransform::getIdentity());
    
    //Mechanical parts
    sf::Polyhedron* obj = new sf::Polyhedron("Base", sf::GetDataPath() + "sphere_R=1.obj", 0.1, btTransform(btQuaternion(0,0,0), btVector3(0,0,0)), getMaterialManager()->getMaterial("Steel"), green);
    
    sf::Cylinder* cyl = new sf::Cylinder("Cyl", 0.2, 1.0, btTransform::getIdentity(), getMaterialManager()->getMaterial("Steel"), green);
    sf::Box* link1 = new sf::Box("Link1", btVector3(0.1,0.02,0.5), btTransform(btQuaternion(0,0,0), btVector3(0.0,0.0,-0.2)), getMaterialManager()->getMaterial("Steel"), green);
    sf::Box* link2 = new sf::Box("Link2", btVector3(0.1,0.02,0.5), btTransform(btQuaternion(0,0,0), btVector3(0.0,0.0,-0.2)), getMaterialManager()->getMaterial("Steel"), green);
    sf::Box* link3 = new sf::Box("Link3", btVector3(0.5,0.01,0.01), btTransform::getIdentity(), getMaterialManager()->getMaterial("Steel"), green);
	
    std::vector<sf::SolidEntity*> links;
    links.push_back(link1);
    links.push_back(link2);
    //links.push_back(link3);
    
    //Sensors
    sf::IMU* imu = new sf::IMU("IMU", -1, 1000);
    sf::RotaryEncoder* enc = new sf::RotaryEncoder("Encoder", -1, 1000);
    
    //Robot
    sf::Robot* robot = new sf::Robot("Robot", false);
    
    robot->DefineLinks(obj, links);
    robot->DefineRevoluteJoint("Joint1", "Base", "Link1",
                               btTransform(btQuaternion::getIdentity(), btVector3(0,0.25,0)), btVector3(0,1,0), std::make_pair(1.0, -1.0), 10.0);
    robot->DefineRevoluteJoint("Joint2", "Base", "Link2",
                               btTransform(btQuaternion::getIdentity(), btVector3(0,-0.25,0)), btVector3(0,1,0), std::make_pair(1.0, -1.0), 10.0);
    
    robot->AddLinkSensor(imu, "Link2", btTransform::getIdentity());
    robot->AddJointSensor(enc, "Joint2");
    
    AddRobot(robot, btTransform(btQuaternion::getIdentity(), btVector3(0.0,0.0,1.0)));
    
    /*FeatherstoneEntity* fe = new FeatherstoneEntity("Test", 4, base, true);
    fe->AddLink(link1, btTransform(btQuaternion(0,0,0), btVector3(0.25,0,0)));
    fe->AddLink(link2, btTransform(btQuaternion(0,0,0), btVector3(0.75,0,0)));
    fe->AddLink(link3, btTransform(btQuaternion(0,0,0), btVector3(-0.5,0,0)));
    fe->AddRevoluteJoint("Joint1", 0, 1, btVector3(0,0,0), btVector3(0,1,0));
    fe->AddRevoluteJoint("Joint2", 1, 2, btVector3(0.5,0,0), btVector3(0,1,0));
    fe->AddRevoluteJoint("Joint3", 0, 3, btVector3(0,0,0), btVector3(0,1,0));
    AddFeatherstoneEntity(fe, btTransform(btQuaternion::getIdentity(), btVector3(0.0,0.0,1.0)));*/
}
