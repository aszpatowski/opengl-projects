//
// Created by aleksander on 22.11.2021.
//
#ifndef INC_3_ZADANIE_MYBACKGROUND_H
#define INC_3_ZADANIE_MYBACKGROUND_H
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <GLFW/glfw3.h>
#include <AGL3Drawable.hpp>


class MyBackground : public AGLDrawable {

private:
    float color = 0.5;
    float color2 = 0.5;
    float direction = 0.01;
    float scale = 1.0;
public:

    MyBackground() : AGLDrawable(0) {
        setShaders();
        setBuffers();
    }

    void setShaders();

    void setBuffers();

    void draw();

    void changeColor();

    void changeSize();

    bool endAnimation();

};

#endif //INC_3_ZADANIE_MYBACKGROUND_H

