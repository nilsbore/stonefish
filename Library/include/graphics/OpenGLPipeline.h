//
//  OpenGLPipeline.h
//  Stonefish
//
//  Created by Patryk Cieslak on 30/03/2014.
//  Copyright (c) 2014-2017 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_OpenGLPipeline__
#define __Stonefish_OpenGLPipeline__

#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <SDL2/SDL_thread.h>
#include "common.h"

#define DUMMY_COLOR glm::vec4(1.f, 0.4f, 0.1f, 1.f)
#define CONTACT_COLOR glm::vec4(1.f, 0, 0, 1.f)

#define SUN_ILLUMINANCE 107527.f //Sun average illuminance Lux
#define SUN_SKY_FACTOR 10.f //Sun illuminance to sky illuminance factor (sky treated as reference)

#define TEX_BASE				((GLint)0)
#define TEX_GUI1                ((GLint)1)
#define TEX_GUI2                ((GLinr)2)

#define TEX_ATM_TRANSMITTANCE	((GLint)3)
#define TEX_ATM_SCATTERING 		((GLint)4)
#define TEX_ATM_IRRADIANCE 		((GLint)5)

#define TEX_POSTPROCESS1		((GLint)6)
#define TEX_POSTPROCESS2		((GLint)7)
#define TEX_POSTPROCESS3		((GLint)8)
#define TEX_POSTPROCESS4		((GLint)9)
#define TEX_POSTPROCESS5		((GLint)10)

#define TEX_SUN_SHADOW			((GLint)11)
#define TEX_SUN_DEPTH			((GLint)12)
#define TEX_SPOT_SHADOW			((GLint)13)
#define TEX_SPOT_DEPTH			((GLint)14)
#define TEX_POINT_SHADOW        ((GLint)15) //Not used
#define TEX_POINT_DEPTH         ((GLint)16) //Not used

namespace sf
{

typedef enum {SOLID = 0, SOLID_CS, HYDRO_CYLINDER, HYDRO_ELLIPSOID, HYDRO_CS, HYDRO_LINES, HYDRO_LINE_STRIP, SENSOR_CS, SENSOR_LINES, SENSOR_LINE_STRIP, SENSOR_POINTS, ACTUATOR_LINES} RenderableType;

typedef enum {QUALITY_DISABLED = 0, QUALITY_LOW, QUALITY_MEDIUM, QUALITY_HIGH} RenderQuality;

struct Renderable
{
    RenderableType type;
    int lookId;
    int objectId;
    glm::mat4 model;
    std::vector<glm::vec3> points;
};

struct RenderSettings
{
    GLint windowW;
    GLint windowH;
    RenderQuality shadows;
    RenderQuality ao;
    RenderQuality atmosphere;
    RenderQuality ocean;
    bool msaa;
    
    RenderSettings()
    {
        windowW = 800;
        windowH = 600;
        shadows = RenderQuality::QUALITY_MEDIUM;
        ao = RenderQuality::QUALITY_MEDIUM;
        atmosphere = RenderQuality::QUALITY_MEDIUM;
        ocean = RenderQuality::QUALITY_MEDIUM;
        msaa = false;
    }
};

class SimulationManager;
class OpenGLCamera;

/*! A class representing OpenGL rendering pipeline */
class OpenGLPipeline
{
public:
    OpenGLPipeline(RenderSettings s);
    ~OpenGLPipeline();
	void AddToDrawingQueue(Renderable r);
	void DrawDisplay();
    void Render(SimulationManager* sim);
    void DrawObjects();
    
    bool isDrawingQueueEmpty();
    SDL_mutex* getDrawingQueueMutex();
    void setVisibleHelpers(bool coordSystems, bool joints, bool actuators, bool sensors, bool lights, bool cameras, bool fluidDynamics);
    void setDebugSimulation(bool enabled);
    RenderSettings getSettings() const;
    
    GLuint getScreenTexture();
	
private:
	std::vector<Renderable> drawingQueue;
    std::vector<Renderable> drawingQueueCopy;
	SDL_mutex* drawingQueueMutex;
    
    RenderSettings settings;
    
    bool drawDebug;
    bool showCoordSys;
    bool showJoints;
    bool showActuators;
    bool showSensors;
	bool showLightMeshes;
    bool showCameraFrustums;
    bool showFluidDynamics;
    
    GLuint screenFBO;
    GLuint screenTex;
};
    
}

#endif
