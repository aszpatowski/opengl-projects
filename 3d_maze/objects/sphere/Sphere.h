//
// Created by aleksander on 01.12.2021.
//

#ifndef SPHERE_H
#define SPHERE_H


#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <GLFW/glfw3.h>
#include "../../AGL3Drawable.hpp"
#include <glm/glm.hpp>
#include <math.h>
#include <vector>

class Sphere : public AGLDrawable
{
private:
    glm::vec3 pos;
    glm::vec3 scale;
public:
    Sphere(int N);

    Sphere();

    void setShaders();

    void setBuffers();

    void draw(glm::mat4 MVP);

    void setColor(float r, float g, float b);

    void setPos(float x,float y, float z);
    void setPos(glm::vec3 newPos);
    void setScale(float newScale);
    void setScale(glm::vec3 newScale);
    void setScale(float x,float y, float z);
    float getRadius2() const;
    float getRadius() const;
    glm::vec3 getPos() const;



private:
    static std::vector <GLfloat> generateMesh(int N);

    const std::vector <GLfloat> g_vertex_buffer_data;
    const GLfloat *g_color_buffer_data;
    GLfloat cross_color[3] = {0.0, 1.0, 0.0};
    float radius;
    float radius2;
public:
    float getTwoRadius2() const;
};


#endif //SPHERE_H
