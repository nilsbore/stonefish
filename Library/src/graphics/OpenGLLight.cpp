/*    
    This file is a part of Stonefish.

    Stonefish is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Stonefish is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//
//  OpenGLLight.cpp
//  Stonefish
//
//  Created by Patryk Cieslak on 12/12/12.
//  Copyright (c) 2012-2018 Patryk Cieslak. All rights reserved.
//

#include "graphics/OpenGLLight.h"

#include "graphics/GLSLShader.h"
#include "graphics/OpenGLPipeline.h"
#include "graphics/OpenGLRealCamera.h"

namespace sf
{

GLuint OpenGLLight::spotShadowArrayTex = 0;
GLuint OpenGLLight::spotDepthSampler = 0;
GLuint OpenGLLight::spotShadowSampler = 0;
OpenGLCamera* OpenGLLight::activeView = NULL;

OpenGLLight::OpenGLLight(glm::vec3 position, glm::vec3 c, GLfloat illuminance)
{
    pos = tempPos = position;
    color = c * illuminance / MEAN_SUN_ILLUMINANCE;
    active = true;
    surfaceDistance = 1.f; // attenuation = 1 / distance^2 * intesity [Lux]
}

OpenGLLight::~OpenGLLight()
{
}

bool OpenGLLight::isActive()
{
    return active;
}

void OpenGLLight::setLightSurfaceDistance(GLfloat dist)
{
    surfaceDistance = dist;
}

glm::vec3 OpenGLLight::getColor()
{
    return color;
}

glm::vec3 OpenGLLight::getPosition()
{
    return pos;
}

void OpenGLLight::UpdatePosition(glm::vec3 p)
{
    tempPos = p;
}

void OpenGLLight::UpdateTransform()
{
    pos = tempPos;
}

void OpenGLLight::Activate()
{
    active = true;
}

void OpenGLLight::Deactivate()
{
    active = false;
}

void OpenGLLight::InitShadowmap(GLint shadowmapLayer)
{
}
    
void OpenGLLight::BakeShadowmap(OpenGLPipeline* pipe)
{
}

void OpenGLLight::ShowShadowMap(glm::vec4 rect)
{
}

//////////////////static//////////////////////////////
void OpenGLLight::Init(std::vector<OpenGLLight*>& lights)
{
	if(lights.size() == 0)
		return;
	
	//Count spotlights
	unsigned int numOfSpotLights = 0;
	for(unsigned int i=0; i < lights.size(); ++i)
		if(lights[i]->getType() == LightType::SPOT_LIGHT) ++numOfSpotLights;
		
	//Generate shadowmap array
	glGenTextures(1, &spotShadowArrayTex);
	glBindTexture(GL_TEXTURE_2D_ARRAY, spotShadowArrayTex);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, SPOT_LIGHT_SHADOWMAP_SIZE, SPOT_LIGHT_SHADOWMAP_SIZE, numOfSpotLights, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
	
	//Generate samplers
	glGenSamplers(1, &spotDepthSampler);
	glSamplerParameteri(spotDepthSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(spotDepthSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(spotDepthSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(spotDepthSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glGenSamplers(1, &spotShadowSampler);
	glSamplerParameteri(spotShadowSampler, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glSamplerParameteri(spotShadowSampler, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glSamplerParameteri(spotShadowSampler, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(spotShadowSampler, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glSamplerParameteri(spotShadowSampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glSamplerParameteri(spotShadowSampler, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	
	//Initialize lights shadow FBOs
	numOfSpotLights = 0;
	for(unsigned int i=0; i < lights.size(); ++i)
		if(lights[i]->getType() == LightType::SPOT_LIGHT) lights[i]->InitShadowmap(numOfSpotLights++);		
		
	//Bind textures and samplers
	glActiveTexture(GL_TEXTURE0 + TEX_SPOT_SHADOW);
	glBindTexture(GL_TEXTURE_2D_ARRAY, spotShadowArrayTex);
	glBindSampler(TEX_SPOT_SHADOW, spotShadowSampler);
	
	glActiveTexture(GL_TEXTURE0 + TEX_SPOT_DEPTH);
	glBindTexture(GL_TEXTURE_2D_ARRAY, spotShadowArrayTex);
	glBindSampler(TEX_SPOT_DEPTH, spotDepthSampler);
}

void OpenGLLight::Destroy()
{
	if(spotShadowArrayTex != 0) glDeleteTextures(1, &spotShadowArrayTex);
	if(spotDepthSampler != 0) glDeleteSamplers(1, &spotDepthSampler);
	if(spotShadowSampler != 0) glDeleteSamplers(1, &spotShadowSampler);
}

void OpenGLLight::SetCamera(OpenGLCamera* view)
{
    activeView = view;
}

void OpenGLLight::SetupShader(GLSLShader* shader)
{
	shader->SetUniform("spotLightsShadowMap", TEX_SPOT_SHADOW);
	shader->SetUniform("spotLightsDepthMap", TEX_SPOT_DEPTH);
}

}



