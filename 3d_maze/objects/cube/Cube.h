//
// Created by aleksander on 01.12.2021.
//

#ifndef Cube_H
#define Cube_H


#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <GLFW/glfw3.h>
#include "../../AGL3Drawable.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>
#include <vector>
#include <iostream>

class Cube : public AGLDrawable
{
public:
    Cube() : AGLDrawable(0)
    {
        setShaders();
        if(!vertIsDefined)
        {
            setVertices();
            vertIsDefined = true;
        }
        setBuffers();
    }
    void setShaders();
    void setBuffers();
    void setVertices();
    void draw(glm::mat4x4 MVP, int beginN,int endN);
    void setTranslationMatrix(glm::vec3 translation);
    void setScaleMatrix(float scale);
    void setRotationMatrix(float angle_, glm::vec3 axis);
    void setColorFactor(glm::vec3 newColorFactor);
    void updateAngle();
private:
    static bool vertIsDefined;
    static float vert[12 * 3 * 4]; //We have 12 triangles. Every triangle has 3 vertices. Every vertice has 4 coordinates.
    float angle;
    glm::vec3 axisAngle;
    glm::mat4x4 translationMatrix;
    glm::mat4x4 scaleMatrix;
    glm::mat4x4 rotationMatrix;
    glm::mat4x4 TSRmatrix; // translation*rotation*scale
    glm::vec3 colorFactor;
};


#endif //TETRAHEDRON_H
