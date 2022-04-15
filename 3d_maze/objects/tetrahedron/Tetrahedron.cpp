//
// Created by aleksander on 01.12.2021.
//

#include "Tetrahedron.h"

bool Tetrahedron::vertIsDefined = false;

float Tetrahedron::vert[4 * 3 * 4];
float Tetrahedron::vert_[4 * 4];

std::vector<glm::mat4x4> Tetrahedron::MVPTSRMatrixes = {};

unsigned int Tetrahedron::instanceVBO = 0;
unsigned int Tetrahedron::VBO = 0;
unsigned int Tetrahedron::VAO = 0;

std::vector<glm::mat4x4> Tetrahedron::MVPTSRMatrixesRewards = {};

unsigned int Tetrahedron::instanceVBOr = 0;
unsigned int Tetrahedron::VBOr = 0;
unsigned int Tetrahedron::VAOr = 0;

glm::mat4x4 Tetrahedron::MVP = {};

void Tetrahedron::setShaders()
{
    if (isReward)
    {
        compileShadersFromFile("objects/tetrahedron/shaders/TetrahedronRewardVertexShader.glsl",
                               "objects/tetrahedron/shaders/TetrahedronRewardFragmentShader.glsl");
    } else
    {
        compileShadersFromFile("objects/tetrahedron/shaders/TetrahedronVertexShader.glsl",
                               "objects/tetrahedron/shaders/TetrahedronFragmentShader.glsl");
    }
}

void Tetrahedron::setBuffers()
{
    AGLDrawable(0);
    bindBuffers();
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                 // attribute 0, must match the layout in the shader.
            4,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,//24,             // stride
            (void *) 0            // array buffer offset
    );

}

void Tetrahedron::setVertices()
{//https://en.wikipedia.org/wiki/Tetrahedron
    glm::vec4 temp = {1.0f, -sqrt(2.0f / 3.0f) / 4.0f, 0.0f, 1.0};
    for (size_t angle = 0; angle < 3; angle++)
    {
        glm::mat4x4 rotMatrix = glm::rotate(glm::mat4x4(1), glm::radians(90.0f + angle * 120.0f),
                                            glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec4 newVertex = rotMatrix * temp;
        for (size_t i = 0; i < 4; i++)
        {
            vert_[angle * 4 + i] = newVertex[i];
        }
    }
    vert_[12] = 0.0f;
    vert_[13] = 3.0f * (sqrt(2.0f / 3.0f) / 4.0f);
    vert_[14] = 0.0f;
    vert_[15] = 1.0f;

    std::vector<std::vector<int>> indices = {
            {0, 1, 2},
            {0, 2, 3},
            {0, 1, 3},
            {1, 2, 3}
    };
    //std::cout << "{ ";
    // Tetrahedron has 4 faces.
    for (int i = 0; i < 4; i++)
    {
        // Every face has 3 vertices.
        for (int j = 0; j < 3; j++)
        {
            // Every vertice has 4 coordinates.
            for (int k = 0; k < 4; k++)
            {
                vert[i * 12 + j * 4 + k] = vert_[indices[i][j] * 4 + k];
                //std::cout<< vert[i * 12 + j * 4 + k]<<", ";
            }
            //std::cout<<"\n";
        }
        //std::cout<<"\n";
    }
}

void Tetrahedron::draw(glm::mat4x4 MVP)
{
    if (isAlive)
    {
        bindProgram();
        bindBuffers();
        glUniformMatrix4fv(1, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(2, 1, GL_FALSE, &TSRmatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 12);
    }
}

void Tetrahedron::setTranslationMatrix(glm::vec3 translation)
{
    translationMatrix = glm::translate(glm::mat4x4(1), translation);
}

void Tetrahedron::setScaleMatrix(float newScale)
{
    scale = (static_cast<float>(rewardValue) / (450.0f)) * newScale;
    scaleMatrix = glm::scale(glm::mat4x4(1), glm::vec3(scale, scale, scale));
}

void Tetrahedron::setRotationMatrix(float angle_, glm::vec3 axis)
{
    angle = angle_;
    axisAngle = axis;
    rotationMatrix = glm::rotate(glm::mat4x4(1), glm::radians(angle), axis);
}

void Tetrahedron::setColorFactor(glm::vec3 newColorFactor)
{
    colorFactor = newColorFactor;
}

void Tetrahedron::updateAngle()
{
    angle = (int(angle) + 1) % 360;
    setRotationMatrix(angle, axisAngle);
}


void Tetrahedron::updateScale()
{
    if (directionScale)
    {
        animScale += 0.005;
        if (animScale > 1.30)
            directionScale = false;
    } else
    {
        animScale -= 0.005;
        if (animScale < 0.70)
            directionScale = true;
    }
    scaleMatrix = glm::scale(glm::mat4x4(1), glm::vec3(scale * animScale, scale * animScale, scale * animScale));
}

void Tetrahedron::setReward(bool reward)
{
    isReward = reward;
    if (isReward)
    {
        rewardValue = int(100 + rand() % 1000);
    } else
    {
        rewardValue = 450;
    }
}

int Tetrahedron::getReward()
{
    return rewardValue;
}

void Tetrahedron::updatePoints()
{
    TSRmatrix = translationMatrix * rotationMatrix * scaleMatrix;
    for (size_t i = 0; i < 4; i++)
    {
        points[i] = glm::vec4(vert_[i * 4], vert_[i * 4 + 1], vert_[i * 4 + 2], vert_[i * 4 + 3]);
        points[i] = TSRmatrix * points[i];
    }
}

void Tetrahedron::setPoints()
{
    for (size_t i = 0; i < 4; i++)
    {
        points[i] = glm::vec4(vert_[i * 4], vert_[i * 4 + 1], vert_[i * 4 + 2], vert_[i * 4 + 3]);
    }
}

glm::vec3 Tetrahedron::getPoints(int whatPoint) const
{
    return glm::vec3(points[whatPoint].x, points[whatPoint].y, points[whatPoint].z);
}

void Tetrahedron::die()
{
    isAlive = false;
    scaleMatrix = glm::scale(glm::mat4x4(1), glm::vec3(0, 0, 0));
    updatePoints();
}

bool Tetrahedron::isAliveF()
{
    return isAlive;
}

size_t Tetrahedron::getUniqueId() const
{
    return uniqueId;
}

void Tetrahedron::setUniqueId(size_t uniqueId)
{
    Tetrahedron::uniqueId = uniqueId;
}

void Tetrahedron::setInstance(unsigned int N)
{
    compileShadersFromFile("objects/tetrahedron/shaders/TetrahedronVertexShader.glsl",
                           "objects/tetrahedron/shaders/TetrahedronFragmentShader.glsl");
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * N, &MVPTSRMatrixes[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                 // attribute 0, must match the layout in the shader.
            4,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,//24,             // stride
            (void *) 0            // array buffer offset
    );
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (1 * vec4Size));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (2 * vec4Size));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (3 * vec4Size));
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
}

void Tetrahedron::drawInstance(unsigned int N)
{
    // we have to update buffer
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(glm::mat4) * N, &MVPTSRMatrixes[0]);

    bindProgram();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 12, N);
}


void Tetrahedron::addToInstanceMatrixes()
{
    MVPTSRMatrixes.push_back(MVP*TSRmatrix);
}

void Tetrahedron::updateInstanceMatrixes(size_t index)
{
    MVPTSRMatrixes[index] = MVP*TSRmatrix;
}

void Tetrahedron::setInstanceReward(unsigned int N)
{
    compileShadersFromFile("objects/tetrahedron/shaders/TetrahedronRewardVertexShader.glsl",
                           "objects/tetrahedron/shaders/TetrahedronRewardFragmentShader.glsl");
    glGenBuffers(1, &instanceVBOr);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * N, &MVPTSRMatrixesRewards[0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenVertexArrays(1, &VAOr);
    glGenBuffers(1, &VBOr);
    glBindVertexArray(VAOr);
    glBindBuffer(GL_ARRAY_BUFFER, VBOr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                 // attribute 0, must match the layout in the shader.
            4,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,//24,             // stride
            (void *) 0            // array buffer offset
    );
    std::size_t vec4Size = sizeof(glm::vec4);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOr);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) 0);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (1 * vec4Size));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (2 * vec4Size));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void *) (3 * vec4Size));
    glVertexAttribDivisor(1, 1);
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
}

void Tetrahedron::drawInstanceReward(unsigned int N)
{
    // we have to update buffer
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBOr);
    glBufferSubData(GL_ARRAY_BUFFER, 0,sizeof(glm::mat4) * N, &MVPTSRMatrixesRewards[0]);

    bindProgram();
    glBindVertexArray(VAOr);
    glBindBuffer(GL_ARRAY_BUFFER, VBOr);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 12, N);
}


void Tetrahedron::addToInstanceMatrixesReward()
{
    MVPTSRMatrixesRewards.push_back(MVP*TSRmatrix);
}

void Tetrahedron::updateInstanceMatrixesReward(size_t index)
{
    MVPTSRMatrixesRewards[index] = MVP*TSRmatrix;
}

void Tetrahedron::setMVP(glm::mat4x4 newMVP)
{
    MVP = newMVP;
}


