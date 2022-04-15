//
// Created by aleksander on 22.11.2021.
//

#ifndef INC_3_ZADANIE_TRIANGLE_H
#define INC_3_ZADANIE_TRIANGLE_H


#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <GLFW/glfw3.h>
#include <AGL3Drawable.hpp>
#include <glm/glm.hpp>
#include <math.h>
#include <vector>

struct Point {
    GLfloat x;
    GLfloat y;
};

struct Points_triangle {
    Point A;
    Point B;
    Point C;
};

struct Color {
    GLfloat r;
    GLfloat g;
    GLfloat b;
};

class Triangle : public AGLDrawable {
public:
    Triangle(bool instance = true){
        this->isInstance = instance;
        if(!instance)
        {
            AGLDrawable(0);
        }
    }

    void setInstance(unsigned int N);

    void setShaders();

    void setBuffers();

    void draw();

    void drawInstance(unsigned int N);

    GLfloat getPosX();

    GLfloat getPosY();

    float getRotate();

    void setRotate(float angle);

    void setPos(GLfloat x, GLfloat y);

    void addToVectorInstance();

    void up();

    void down();

    void right();

    void left();

    void setVertex();

    void setScale(int N);

    void setColor(GLfloat r = 0.3, GLfloat g = 0.6, GLfloat b = 0.8);

    Point getPoints(int i);

    bool operator &&(Triangle &B);

private:
    float rotate;
    float scale;
    bool isInstance;
    float lx = 0.07;
    float ly = 0.02;
    float pointsArray[6];
    Color colors;
    Point pos;
    Point points[3];
    bool ccw(Point A, Point B, Point C);
    bool intersect_line(Point A, Point B, Point C, Point D);
    static std::vector <glm::vec3> translationsVector;
    static unsigned int instanceVBO, VAO, VBO;

};

#endif //INC_3_ZADANIE_TRIANGLE_H

