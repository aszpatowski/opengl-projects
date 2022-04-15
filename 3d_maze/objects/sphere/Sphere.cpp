//
// Created by aleksander on 01.12.2021.
//

#include "Sphere.h"

Sphere::Sphere(int N) :
        g_vertex_buffer_data(generateMesh(N)),
        pos(0, 0, 0),
        scale(1, 1, 1)
{
    GLfloat cross_color[3] = {0.0, 1.0, 0.0};
    setShaders();
    setBuffers();
}

Sphere::Sphere() : Sphere(10)
{}

std::vector<GLfloat> Sphere::generateMesh(int N)
{

    GLfloat radius = 1;
    // clear memory of prev arrays
    std::vector<GLfloat> vertices;

    // vertex texCoord

    GLfloat sectorStep = (GLfloat) M_PI / (GLfloat) N;
    GLfloat stackStep = (GLfloat) M_PI / (GLfloat) N;

    for (int i = 0; i <= N * 2; ++i)
    {
        GLfloat stackAngle = i * stackStep;

        for (int j = 0; j <= N * 2; ++j)
        {
            GLfloat sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            vertices.push_back(radius * cosf(stackAngle) * cos(sectorAngle));
            vertices.push_back(radius * sinf(stackAngle));
            vertices.push_back(radius * cosf(stackAngle) * sin(sectorAngle));

            vertices.push_back(radius * cosf(stackAngle + stackStep) * cos(sectorAngle));
            vertices.push_back(radius * sinf(stackAngle + stackStep));
            vertices.push_back(radius * cosf(stackAngle + stackStep) * sin(sectorAngle));

            vertices.push_back(radius * cosf(stackAngle) * cos(sectorAngle + sectorStep));
            vertices.push_back(radius * sinf(stackAngle));
            vertices.push_back(radius * cosf(stackAngle) * sin(sectorAngle + sectorStep));

            vertices.push_back(radius * cosf(stackAngle) * cos(sectorAngle + sectorStep));
            vertices.push_back(radius * sinf(stackAngle));
            vertices.push_back(radius * cosf(stackAngle) * sin(sectorAngle + sectorStep));

            vertices.push_back(radius * cosf(stackAngle + stackStep) * cos(sectorAngle));
            vertices.push_back(radius * sinf(stackAngle + stackStep));
            vertices.push_back(radius * cosf(stackAngle + stackStep) * sin(sectorAngle));

            vertices.push_back(radius * cosf(stackAngle + stackStep) * cos(sectorAngle + sectorStep));
            vertices.push_back(radius * sinf(stackAngle + stackStep));
            vertices.push_back(radius * cosf(stackAngle + stackStep) * sin(sectorAngle + sectorStep));
        }
    }

    return vertices;
}

void Sphere::setShaders()
{
    compileShadersFromFile("objects/sphere/shaders/SphereVertexShader.glsl",
                           "objects/sphere/shaders/SphereFragmentShader.glsl");
}

void Sphere::setBuffers()
{
    bindBuffers();

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * g_vertex_buffer_data.size(), g_vertex_buffer_data.data(),
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0            // array buffer offset
    );
}

void Sphere::draw(glm::mat4 MVP)
{
    bindProgram();
    bindBuffers();
    glUniform4f(0, pos[0], pos[1], pos[2], 0);
    glUniform4f(1, scale[0], scale[1], scale[2], 1);
    glUniformMatrix4fv(2, 1, GL_FALSE, &MVP[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, g_vertex_buffer_data.size()); // 12*3 indices starting at 0 -> 12 triangles
}

void Sphere::setColor(float r, float g, float b)
{
    cross_color[0] = r;
    cross_color[1] = g;
    cross_color[2] = b;
}

void Sphere::setPos(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void Sphere::setPos(glm::vec3 newPos)
{
    pos[0] = newPos.x;
    pos[1] = newPos.y;
    pos[2] = newPos.z;
}

void Sphere::setScale(float newScale)
{
    radius = newScale;
    radius2 = newScale*newScale;
    scale[0] = newScale;
    scale[1] = newScale;
    scale[2] = newScale;
}

void Sphere::setScale(glm::vec3 newScale)
{
    radius = newScale.x;
    radius2 = newScale.x*newScale.x;
    scale[0] = newScale.x;
    scale[1] = newScale.y;
    scale[2] = newScale.z;
}

void Sphere::setScale(float x, float y, float z)
{
    radius = x;
    radius2 = x*x;
    scale[0] = x;
    scale[1] = y;
    scale[2] = z;
}

float Sphere::getRadius2() const
{
    return radius2;
}

glm::vec3 Sphere::getPos() const
{
    return glm::vec3(pos[0],pos[1],pos[2]);
}

float Sphere::getRadius() const
{
    return radius;
}
