//
// Created by aleksander on 01.12.2021.
//

#include "Sphere.h"

std::vector <GLfloat> Sphere::g_vertex_buffer_data_instanced = {};
std::vector <glm::vec3> Sphere::colorData = {};
std::vector <glm::vec3> Sphere::instanceOffset = {};
std::vector <GLfloat> Sphere::scaleInstance = {};

unsigned int Sphere::colorVBO = 0;
unsigned int Sphere::offsetVBO = 0;
unsigned int Sphere::scaleVBO = 0;
unsigned int Sphere::VAO = 0;
unsigned int Sphere::VBO = 0;
unsigned int Sphere::MatrixID = 0;
unsigned int Sphere::LightPositionPlayerID = 0;
unsigned int Sphere::LightPositionUpperID = 0;
unsigned int Sphere::ViewPositionID = 0;


Sphere::Sphere(int N) :
        g_vertex_buffer_data(generateMesh(N)),
        pos(0, 0, 0),
        scale(1, 1, 1)
{
    GLfloat cross_color[3] = {0.0, 1.0, 0.0};
    setShaders();
    setBuffers();
}

Sphere::Sphere()
{}

void Sphere::setVertexBuffer(int N)
{
    g_vertex_buffer_data_instanced = generateMesh(N);
}

std::vector <GLfloat> Sphere::generateMesh(int N)
{

    GLfloat radius = 1;
    // clear memory of prev arrays
    std::vector <GLfloat> vertices;

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
    radius2 = newScale * newScale;
    scale[0] = newScale;
    scale[1] = newScale;
    scale[2] = newScale;
}

void Sphere::setScale(glm::vec3 newScale)
{
    radius = newScale.x;
    radius2 = newScale.x * newScale.x;
    scale[0] = newScale.x;
    scale[1] = newScale.y;
    scale[2] = newScale.z;
}

void Sphere::setScale(float x, float y, float z)
{
    radius = x;
    radius2 = x * x;
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
    return glm::vec3(pos[0], pos[1], pos[2]);
}

float Sphere::getRadius() const
{
    return radius;
}

void Sphere::setIsSpecial(bool newIsSpecial)
{
    isSpecial = newIsSpecial;
}

void Sphere::setReward(int newReward)
{
    reward = newReward;
}

bool Sphere::getIsSpecial() const
{
    return isSpecial;
}

int Sphere::getReward() const
{
    return reward;
}

void Sphere::setSpeed(glm::vec3 newSpeed)
{
    speed = newSpeed;
}

glm::vec3 Sphere::getSpeed() const
{
    return speed;
}

void Sphere::setAlive(bool isAlive)
{
    alive = isAlive;
    if (alive == false)
    {
        setScale(0.0f);
    }
}

bool Sphere::getAlive() const
{
    return alive;
}

void Sphere::setInstance(unsigned int N)
{
    setVertexBuffer(10);
    compileShadersFromFile("objects/sphere/shaders/InstancedSphereVertexShader.glsl",
                           "objects/sphere/shaders/InstancedSphereFragmentShader.glsl");
    MatrixID = glGetUniformLocation(p(), "MVP");
    LightPositionUpperID = glGetUniformLocation(p(), "upperLightPosition");
    LightPositionPlayerID = glGetUniformLocation(p(), "playerLightPosition");
    ViewPositionID = glGetUniformLocation(p(), "viewPosition");
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * N, &colorData[0], GL_DYNAMIC_DRAW);
    glGenBuffers(1, &offsetVBO);
    glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * N, &instanceOffset[0], GL_DYNAMIC_DRAW);
    glGenBuffers(1, &scaleVBO);
    glBindBuffer(GL_ARRAY_BUFFER, scaleVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * N, &scaleInstance[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,  sizeof(GLfloat) * g_vertex_buffer_data_instanced.size(),
                 g_vertex_buffer_data_instanced.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
            0,                 // attribute 0, must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,//24,             // stride
            (void *) 0            // array buffer offset
    );
    glVertexAttribDivisor(0, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);

    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void *) 0);

    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, scaleVBO);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void *) 0);

    glVertexAttribDivisor(3, 1);
    //std::cout << g_vertex_buffer_data_instanced.size() << std::endl;

}

void Sphere::drawInstance(unsigned int N, glm::mat4 MVP, glm::vec3 viewPosition, glm::vec3 playerLightPosition,
                          glm::vec3 upperLightPosition)
{

    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * N, &colorData[0]);

    glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * N, &instanceOffset[0]);

    glBindBuffer(GL_ARRAY_BUFFER, scaleVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * N, &scaleInstance[0]);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    bindProgram();



    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    glUniform3f(LightPositionPlayerID, playerLightPosition.x, playerLightPosition.y, playerLightPosition.z);
    glUniform3f(LightPositionUpperID, upperLightPosition.x, upperLightPosition.y, upperLightPosition.z);
    glUniform3f(ViewPositionID, viewPosition.x, viewPosition.y, viewPosition.z);
    //std::cout<<instanceOffset[0].x<<"\t"<<instanceOffset[0].y<<"\t"<<instanceOffset[0].z<<"\t"<<scaleInstance[0]<<std::endl;
    glDrawArraysInstanced(GL_TRIANGLES,0,g_vertex_buffer_data_instanced.size(),N);
}

void Sphere::addToInstance()
{
    colorData.push_back(glm::vec3(cross_color[0], cross_color[1], cross_color[2]));
    instanceOffset.push_back(pos);
    scaleInstance.push_back(scale.x);

}

void Sphere::updateInstance(size_t index)
{
    instanceOffset[index] = pos;
    scaleInstance[index] = scale.x;
}