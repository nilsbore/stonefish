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
//  OpenGLOceanParticles.h
//  Stonefish
//
//  Created by Patryk Cieslak on 04/08/19.
//  Copyright (c) 2019 Patryk Cieslak. All rights reserved.
//

#include "graphics/OpenGLOceanParticles.h"

#include "graphics/GLSLShader.h"
#include "graphics/OpenGLCamera.h"
#include "entities/forcefields/Ocean.h"

namespace sf
{
	
GLSLShader* OpenGLOceanParticles::particleShader = NULL;

OpenGLOceanParticles::OpenGLOceanParticles(size_t numOfParticles, GLfloat visibleRange) : OpenGLParticles(numOfParticles), uniformd(0, visibleRange), normald(0, 1.f)
{
	initialised = false;
	range = fabsf(visibleRange);
	lastEyePos = glm::vec3(0);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
	static const GLfloat billboard[] = { 
		 -0.5f, -0.5f, 0.0f,
		  0.5f, -0.5f, 0.0f,
		 -0.5f,  0.5f, 0.0f,
		  0.5f,  0.5f, 0.0f,
	};
	
	glGenBuffers(1, &vboParticle);
	glBindBuffer(GL_ARRAY_BUFFER, vboParticle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(billboard), billboard, GL_STATIC_DRAW);

	glGenBuffers(1, &vboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glBufferData(GL_ARRAY_BUFFER, nParticles * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glBindVertexArray(0);
}
	
OpenGLOceanParticles::~OpenGLOceanParticles()
{
	if(vboParticle > 0) glDeleteBuffers(1, &vboParticle);
	if(vboPosition > 0) glDeleteBuffers(1, &vboPosition);
	if(vao > 0) glDeleteVertexArrays(1, &vao);
}

void OpenGLOceanParticles::Create(glm::vec3 eyePos)
{
	lastEyePos = eyePos;
	initialised = true;
	
	//Create particles randomly (uniformly) distributed inside a sphere
	for(size_t i=0; i<nParticles; ++i)
	{
		GLfloat r = cbrtf(uniformd(generator));
		positions[i] = r * glm::normalize(glm::vec3(normald(generator), normald(generator), normald(generator))) + eyePos;
		velocities[i] = 0.01f * glm::vec3(normald(generator), normald(generator), normald(generator));
	}
}
	
void OpenGLOceanParticles::Update(OpenGLCamera* cam, Ocean* ocn, GLfloat dt)
{
	glm::vec3 eyePos = cam->GetEyePosition();
	
	//Check if ever updated
	if(!initialised)
	{
		Create(eyePos);
		return;
	}
	
	//Simulate motion
	for(size_t i=0; i<nParticles; ++i)
	{
		positions[i] += velocities[i]*dt;
		Vector3 v = ocn->GetFluidVelocity(Vector3(positions[i].x, positions[i].y, positions[i].z));
		velocities[i] = glm::vec3((GLfloat)v.x(), (GLfloat)v.y(), (GLfloat)v.z());
	}
	
	//Determine camera moving direction
	glm::vec3 dP = eyePos - lastEyePos;
	lastEyePos = eyePos;
	bool cameraMoved = glm::length2(dP) > 0.01f*0.01f; //Camera moved more than 1 cm?
	if(cameraMoved) 
		dP = glm::normalize(dP);
		
	//Kill and create (relocate) particles 
	GLfloat range2 = range*range;
	
	cameraMoved = false;
	if(cameraMoved)
	{
		//Relocate particles out of range to a random position on a hemisphere around camera, aligned with moving direction
		for(size_t i=0; i<nParticles; ++i)
		{
			if(glm::length2(positions[i] - eyePos) > range2) //If particle is out of range
			{
				
			
			
			}
		}
	} 
	else
	{
		//Relocate particles out of range to a random position on a sphere around camera
		for(size_t i=0; i<nParticles; ++i)
		{
			if(glm::length2(positions[i] - eyePos) > range2) //If particle is out of range
			{
				GLfloat r = cbrtf(uniformd(generator));
				positions[i] = r * glm::normalize(glm::vec3(normald(generator), normald(generator), normald(generator))) + eyePos;
				velocities[i] = 0.1f * glm::vec3(normald(generator), normald(generator), normald(generator));
			}
		}
	}
}
	
void OpenGLOceanParticles::Draw(OpenGLCamera* cam)
{
	glm::mat4 projection = cam->GetProjectionMatrix();
	glm::mat4 view = cam->GetViewMatrix();
	
	glBindVertexArray(vao);
	
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glBufferData(GL_ARRAY_BUFFER, nParticles * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, nParticles * sizeof(GLfloat) * 3, &positions[0].x);
	
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboParticle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glVertexAttribDivisor(0, 0);
	glVertexAttribDivisor(1, 1);
	
	particleShader->Use();
	particleShader->SetUniform("MVP", projection * view);
	particleShader->SetUniform("camRight", glm::vec3(view[0][0], view[1][0], view[2][0]));
	particleShader->SetUniform("camUp", glm::vec3(view[0][1], view[1][1], view[2][1]));
	particleShader->SetUniform("billboardSize", 0.002f);
	
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, nParticles);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	
	glUseProgram(0);
	glBindVertexArray(0);
}
	
void OpenGLOceanParticles::Init()
{
	particleShader = new GLSLShader("billboard.frag", "billboard.vert");
	particleShader->AddUniform("MVP", ParameterType::MAT4);
	particleShader->AddUniform("camRight", ParameterType::VEC3);
	particleShader->AddUniform("camUp", ParameterType::VEC3);
	particleShader->AddUniform("billboardSize", ParameterType::FLOAT);
}

void OpenGLOceanParticles::Destroy()
{
	if(particleShader != NULL) delete particleShader;
}
	
}