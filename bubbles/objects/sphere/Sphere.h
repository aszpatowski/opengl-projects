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
public:
    Sphere(int N);

    Sphere();

    void setShaders();

    void setBuffers();

    void setVertexBuffer(int N);

    void draw(glm::mat4 MVP);

    void setColor(float r, float g, float b);

    void setPos(float x,float y, float z);
    void setPos(glm::vec3 newPos);
    void setScale(float newScale);
    void setScale(glm::vec3 newScale);
    void setScale(float x,float y, float z);
    void setIsSpecial(bool newIsSpecial);
    void setReward(int newReward);
    void setSpeed(glm::vec3 newSpeed);
    void setAlive(bool isAlive);
    float getRadius2() const;
    float getRadius() const;
    glm::vec3 getPos() const;
    bool getIsSpecial() const;
    int getReward() const;
    glm::vec3 getSpeed() const;
    bool getAlive() const;

    //instancing functions

    void setInstance(unsigned int N);
    void drawInstance(unsigned int N,glm::mat4 MVP, glm::vec3 viewPosition, glm::vec3 playerLightPosition, glm::vec3 upperLightPosition);
    void addToInstance();
    void updateInstance(size_t index);



private:
    glm::vec3 pos;
    glm::vec3 scale;
    static std::vector <GLfloat> generateMesh(int N);

    const std::vector <GLfloat> g_vertex_buffer_data;
    const GLfloat *g_color_buffer_data;
    GLfloat cross_color[3] = {0.0, 1.0, 0.0};
    float radius;
    float radius2;
    glm::vec3 speed;
    bool isSpecial;
    bool alive;
    int reward;

    //instancing


    static std::vector <GLfloat> g_vertex_buffer_data_instanced;
    static std::vector <glm::vec3> colorData;
    static std::vector <glm::vec3> instanceOffset;
    static std::vector <GLfloat> scaleInstance;
    static unsigned int colorVBO,offsetVBO,scaleVBO, VAO, VBO, MatrixID, LightPositionUpperID, LightPositionPlayerID, ViewPositionID;


public:
    float getTwoRadius2() const;
};


#endif //SPHERE_H
