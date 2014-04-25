//
//  OpenGLSpotLight.h
//  Stonefish
//
//  Created by Patryk Cieslak on 8/20/13.
//  Copyright (c) 2013 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_OpenGLSpotLight__
#define __Stonefish_OpenGLSpotLight__

#include "OpenGLLight.h"

class OpenGLSpotLight : public OpenGLLight
{
public:
    OpenGLSpotLight(const btVector3& position, const btVector3& target, GLfloat cone, GLfloat* color4);
    ~OpenGLSpotLight();
    
    void Render();
    void UpdateLight();
    void RenderLightSurface();
    void RenderDummy();
    void RenderShadowMap(OpenGLPipeline* pipe);
    void ShowShadowMap(GLfloat x, GLfloat y, GLfloat scale);
    
    btVector3 getViewDirection();
    GLfloat getAngle();
    
private:
    btVector3 reldir;
    btVector3 dir;
    GLfloat coneAngle;
    
    GLuint shadowMap;
    GLuint shadowFBO;
    GLuint shadowSize;
    glm::mat4 lightClipSpace;
};

#endif