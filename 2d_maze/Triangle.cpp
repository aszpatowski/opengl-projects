//
// Created by aleksander on 22.11.2021.
//

#include "Triangle.h"

#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
#define radToDeg(angleInRadians) ((angleInRadians) * 180.0 / M_PI)

std::vector <glm::vec3> Triangle::translationsVector = {};
unsigned int Triangle::instanceVBO = 0;
unsigned int Triangle::VBO = 0;
unsigned int Triangle::VAO = 0;
void Triangle::setInstance(unsigned int N) {
    compileShadersFromFile("shaders/TriangleInstanceVertexShader.glsl",
                           "shaders/TriangleInstanceFragmentShader.glsl");
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * N, &translationsVector[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    float vert[] = {
            // positions     // colors
            -lx*scale, ly*scale,    1 - colors.r, 1 - colors.g, colors.b,
            -lx*scale, -ly*scale,   1 - colors.r, 1 - colors.g, colors.b,
            lx*scale, 0,     colors.r, colors.g, colors.b

    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.
}
void Triangle::drawInstance(unsigned int N) {
    bindProgram();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, N);
    glBindVertexArray(0);
}


void Triangle::setShaders() {
    compileShadersFromFile("shaders/TriangleVertexShader.glsl", "shaders/TriangleFragmentShader.glsl");
}

void Triangle::setBuffers() {
    if (isInstance == false) {
        bindBuffers();
        GLfloat vert[3][2];
        glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), vert, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,                 // attribute 0, must match the layout in the shader.
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,//24,             // stride
                (void *) 0            // array buffer offset
        );
    } else {
        bindBuffers();
        GLfloat vert[3][2];
        glBufferData(GL_ARRAY_BUFFER, 3 * 2 * sizeof(float), vert, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
                0,                 // attribute 0, must match the layout in the shader.
                2,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,//24,             // stride
                (void *) 0            // array buffer offset
        );
    }

}

void Triangle::draw() {
    bindProgram();
    bindBuffers();
    glUniform2fv(0, 3, pointsArray);
    glUniform3f(3, colors.r, colors.g, colors.b);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}


GLfloat Triangle::getPosX() {
    return pos.x;
}

GLfloat Triangle::getPosY() {
    return pos.y;
}

float Triangle::getRotate() {
    return rotate;

}

void Triangle::setRotate(float angle) {
    rotate = angle;
}

void Triangle::setPos(GLfloat x, GLfloat y) {
    pos.x = x;
    pos.y = y;
}

void Triangle::addToVectorInstance() {
    translationsVector.push_back(glm::vec3(pos.x, pos.y, rotate));
}

void Triangle::up() {
    setPos(getPosX() - 0.01 * cos(degToRad(getRotate())),
           getPosY() - 0.01 * sin(degToRad(getRotate())));
    setVertex();
}

void Triangle::down() {
    setPos(getPosX() + 0.01 * cos(degToRad(getRotate())),
           getPosY() + 0.01 * sin(degToRad(getRotate())));
    setVertex();
}

void Triangle::right() {
    setRotate(getRotate() - 2);
    setVertex();
}

void Triangle::left() {
    setRotate(getRotate() + 2);
    setVertex();
}

void Triangle::setVertex() {
    float s = sin(degToRad(rotate));
    float c = cos(degToRad(rotate));

    pointsArray[0] = (-lx * c - ly * s) * scale + pos.x;
    pointsArray[1] = (-lx * s + ly * c) * scale + pos.y;

    pointsArray[2] = (-lx * c + ly * s) * scale + pos.x;
    pointsArray[3] = (-lx * s - ly * c) * scale + pos.y;

    pointsArray[4] = (lx * c) * scale + pos.x;
    pointsArray[5] = (lx * s) * scale + pos.y;

}

void Triangle::setScale(int N) {
    scale = 10.0 / N;
}

void Triangle::setColor(GLfloat r, GLfloat g, GLfloat b) {
    colors.r = r;
    colors.g = g;
    colors.b = b;
}

Point Triangle::getPoints(int i) {
    return Point{pointsArray[2 * i], pointsArray[2 * i + 1]};
}

bool Triangle::operator&&(Triangle &B) {
    if (intersect_line(this->getPoints(0), this->getPoints(1), B.getPoints(0), B.getPoints(1))) {
        return true;
    } else if (intersect_line(this->getPoints(0), this->getPoints(1), B.getPoints(1), B.getPoints(2))) {
        return true;
    } else if (intersect_line(this->getPoints(0), this->getPoints(1), B.getPoints(0), B.getPoints(2))) {
        return true;
    } else if (intersect_line(this->getPoints(1), this->getPoints(2), B.getPoints(0), B.getPoints(1))) {
        return true;
    } else if (intersect_line(this->getPoints(1), this->getPoints(2), B.getPoints(1), B.getPoints(2))) {
        return true;
    } else if (intersect_line(this->getPoints(1), this->getPoints(2), B.getPoints(0), B.getPoints(2))) {
        return true;
    } else if (intersect_line(this->getPoints(0), this->getPoints(2), B.getPoints(0), B.getPoints(1))) {
        return true;
    } else if (intersect_line(this->getPoints(0), this->getPoints(2), B.getPoints(1), B.getPoints(2))) {
        return true;
    } else if (intersect_line(this->getPoints(0), this->getPoints(2), B.getPoints(0), B.getPoints(2))) {
        return true;
    } else {
        return false;
    }
}

bool Triangle::ccw(Point A, Point B, Point C) {
    return ((C.y - A.y) * (B.x - A.x)) > ((B.y - A.y) * (C.x - A.x));
}

bool Triangle::intersect_line(Point A, Point B, Point C, Point D) {
    return (ccw(A, C, D) != ccw(B, C, D)) && (ccw(A, B, C) != ccw(A, B, D));
}