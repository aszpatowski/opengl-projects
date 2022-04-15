//
// Created by aleksander on 01.12.2021.
//

#include "Cube.h"

bool Cube::vertIsDefined = false;

float Cube::vert[12 * 3 * 4];

void Cube::setShaders()
{
    compileShadersFromFile("objects/cube/shaders/CubeVertexShader.glsl",
                           "objects/cube/shaders/CubeFragmentShader.glsl");
}

void Cube::setBuffers()
{
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

void Cube::setVertices()
{
    float vert_[8 * 4] = {
            0, 0, 0, 1,
            1, 0, 0, 1,
            1, 1, 0, 1,
            0, 1, 0, 1,
            0, 1, 1, 1,
            1, 1, 1, 1,
            1, 0, 1, 1,
            0, 0, 1, 1

    };
    std::vector<std::vector<int>> indices = {
            {0, 1, 3},
            {2, 1, 3},

            {0, 1, 7},
            {6, 1, 7},

            {2, 1, 5},
            {6, 1, 5},

            {7, 0, 4},
            {3, 0, 4},

            {7, 4, 6},//needed
            {5, 4, 6},

            {4, 3, 5},//needed
            {2, 3, 5}
    };
    //std::cout << "{ ";
    // cube has 12 triangles.
    for (int i = 0; i < 12; i++)
    {
        // Every triangle has 3 vertices.
        for (int j = 0; j < 3; j++)
        {
            // Every vertice has 4 coordinates.
            for (int k = 0; k < 4; k++)
            {
                vert[i * 12 + j * 4 + k] = vert_[indices[i][j] * 4 + k];
            }
        }
    }
    /*for (int i = 0; i < 144; i++)
    {
        if (i % 12 == 0)
        {
            std::cout << " ";
        }
        if (i % 24 == 0)
        {
            std::cout << "\n";
        }
        std::cout << vert[i] << " ";
    }*/
}

void Cube::draw(glm::mat4x4 MVP, int beginN,int endN)
{
    bindProgram();
    bindBuffers();
    TSRmatrix = translationMatrix * rotationMatrix * scaleMatrix;
    glUniformMatrix4fv(1, 1, GL_FALSE, &TSRmatrix[0][0]);
    glUniformMatrix4fv(2, 1, GL_FALSE, &MVP[0][0]);
    glUniform3f(3, colorFactor.r, colorFactor.g, colorFactor.b);
    glDrawArrays(GL_TRIANGLES, beginN, endN); //144 = 12*3
}

void Cube::setTranslationMatrix(glm::vec3 translation)
{
    translationMatrix = glm::translate(glm::mat4x4(1), translation);
}

void Cube::setScaleMatrix(float scale)
{
    scaleMatrix = glm::scale(glm::mat4x4(1), glm::vec3(scale, scale, scale));
}

void Cube::setRotationMatrix(float angle_, glm::vec3 axis)
{
    angle = angle_;
    axisAngle = axis;
    rotationMatrix = glm::rotate(glm::mat4x4(1), glm::radians(angle), axis);
}

void Cube::setColorFactor(glm::vec3 newColorFactor)
{
    colorFactor = newColorFactor;
}

void Cube::updateAngle()
{
    angle = (int(angle) + 1) % 360;
    setRotationMatrix(angle, axisAngle);
}

