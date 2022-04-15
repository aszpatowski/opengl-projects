//
// Created by aleksander on 01.12.2021.
//

#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H


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

class Tetrahedron : public AGLDrawable
{
public:
    Tetrahedron()
    {
        isAlive = true;
        if(!vertIsDefined)
        {
            setVertices();
            vertIsDefined = true;
        }
    }
    void setShaders();
    void setBuffers();
    void setVertices();
    void setInstance(unsigned int N);
    void drawInstance(unsigned int N);
    void addToInstanceMatrixes();
    void updateInstanceMatrixes(size_t index);

    void setInstanceReward(unsigned int N);
    void drawInstanceReward(unsigned int N);
    void addToInstanceMatrixesReward();
    void updateInstanceMatrixesReward(size_t index);

    void draw(glm::mat4x4 MVP);
    void setTranslationMatrix(glm::vec3 translation);
    void setScaleMatrix(float newScale);
    void setRotationMatrix(float angle_, glm::vec3 axis);
    void setReward(bool reward);
    int getReward();
    void setColorFactor(glm::vec3 newColorFactor);
    void updateAngle();
    void updateScale();
    void setPoints();
    void updatePoints();
    void setMVP(glm::mat4x4 newMVP);
    glm::vec3 getPoints(int whatPoint) const;
    void die();
    bool isAliveF();
    size_t getUniqueId() const;
    void setUniqueId(size_t uniqueId);
private:
    static bool vertIsDefined;
    static float vert[4 * 3 * 4]; //We have 4 faces. Every face has 3 vertices. Every vertice has 4 coordinates.
    static float vert_[4 * 4];
    size_t uniqueId; // for debugging
    float angle;
    bool isReward;
    bool isAlive;
    int rewardValue;
    float scale;
    float animScale = 1.0;
    bool directionScale = true;
    glm::vec3 axisAngle;
    glm::mat4x4 translationMatrix;
    glm::mat4x4 scaleMatrix;
    glm::mat4x4 rotationMatrix;
    glm::mat4x4 TSRmatrix; // translation*rotation*scale
    glm::vec4 points[4];
    glm::vec3 colorFactor;
    static glm::mat4x4 MVP;
    static std::vector <glm::mat4x4> MVPTSRMatrixes;
    static unsigned int instanceVBO, VAO, VBO;

    static std::vector <glm::mat4x4> MVPTSRMatrixesRewards;
    static unsigned int instanceVBOr, VAOr, VBOr;
};


#endif //TETRAHEDRON_H
