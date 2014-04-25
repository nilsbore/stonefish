//
//  OpenGLOmniLight.h
//  Stonefish
//
//  Created by Patryk Cieslak on 8/20/13.
//  Copyright (c) 2013 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_OpenGLOmniLight__
#define __Stonefish_OpenGLOmniLight__

#include "OpenGLLight.h"

class OpenGLOmniLight : public OpenGLLight
{
public:
    OpenGLOmniLight(const btVector3& position, GLfloat* color4);
    ~OpenGLOmniLight();
    
    void Render();
    void UpdateLight();
    void RenderLightSurface();
    void RenderDummy();
    void RenderShadowMap(OpenGLPipeline* pipe);
    void ShowShadowMap(GLfloat x, GLfloat y, GLfloat scale);
};

#endif