//
//  OpenGLTrackball.h
//  Stonefish
//
//  Created by Patryk Cieslak on 5/29/13.
//  Copyright (c) 2013-2019 Patryk Cieslak. All rights reserved.
//

#ifndef __Stonefish_OpenGLTrackball__
#define __Stonefish_OpenGLTrackball__

#include "graphics/OpenGLCamera.h"

namespace sf
{
    //! A class implementing a camera that can be rotated as a trackball, with mouse.
    class OpenGLTrackball : public OpenGLCamera
    {
    public:
        //! A constructor.
        /*!
         \param centerPosition the position of the camera orbit center in world space [m]
         \param orbitRadius the radius of the camera orbit [m]
         \param up a unit vector pointing to the top edge of the image
         \param originX the x coordinate of the view origin in the program window
         \param originY the y coordinate of the view origin in the program window
         \param width the width of the view
         \param height the height of the view
         \param horizontalFovDeg the horizontal field of view of the camera [deg]
         \param horizonDistance the distance to the camera far plane [m]
         \param spp number of samples used (>1 means multisampling)
         \param ao a flag to decide if ambient occlusion should be rendered
         */
        OpenGLTrackball(glm::vec3 centerPosition, GLfloat orbitRadius, glm::vec3 up,
                        GLint originX, GLint originY, GLint width, GLint height,
                        GLfloat horizontalFovDeg, GLfloat horizonDistance, GLuint spp = 1, bool ao = false);
        
        //! A method to apply a rotation to the trackball.
        /*!
         \param rot a quaternion designating rotation
         */
        void Rotate(glm::quat rot);
        
        //! A method to move the center of the trackball orbit.
        /*!
         \param step a position step [m]
         */
        void MoveCenter(glm::vec3 step);
        
        //! A method to glue the trackball to a rigid body.
        /*!
         \param solid a pointer to a rigid body
         */
        void GlueToEntity(SolidEntity* solid);
        
        //! A method servicing the mouse down event.
        /*!
         \param x the x coordinate of the mouse pointer
         \param y the y coordinate of the mouse pointer
         \param translate a flag to decide between translate and rotate
         */
        void MouseDown(GLfloat x, GLfloat y, bool translate);
        
        //! A method servicing the mouse move event.
        /*!
         \param x the x coordinate of the mouse pointer
         \param y the y coordinate of the mouse pointer
         */
        void MouseMove(GLfloat x, GLfloat y);
        
        //! A method servicing the mouse up event.
        void MouseUp();
        
        //! A method servicing the mouse scroll event.
        /*!
         \param s amount of scrolling
         */
        void MouseScroll(GLfloat s);
        
        //! A method returning the view matrix.
        glm::mat4 GetViewMatrix() const;
        
        //! A method returning the eye position in the world frame.
        glm::vec3 GetEyePosition() const;
        
        //! A method returning a unit vector parallel to the optical axis of the camera.
        glm::vec3 GetLookingDirection() const;
        
        //! A method returning a unit vector pointing to the top edge of the image.
        glm::vec3 GetUpDirection() const;
        
        //! A method returning the type of the view.
        ViewType getType();
        
        //! A method that informs if the camera needs update.
        bool needsUpdate();
        
    private:
        void UpdateTrackballTransform();
        GLfloat calculateZ(GLfloat x, GLfloat y);
        
        SolidEntity* holdingEntity;
        
        glm::mat4 trackballTransform;
        glm::quat rotation;
        glm::vec3 center;
        glm::vec3 up;
        GLfloat radius;
        
        //Mouse interaction
        GLfloat x_start, y_start, z_start;
        glm::quat rotation_start;
        glm::vec3 translation_start;
        bool dragging;
        bool transMode;
    };
}

#endif
