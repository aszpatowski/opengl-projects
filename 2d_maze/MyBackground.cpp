//
// Created by aleksander on 22.11.2021.
//

#include "MyBackground.h"

void MyBackground::setShaders() {
    compileShadersFromFile("shaders/myBackgroundVertexShader.glsl","shaders/myBackgroundFragmentShader.glsl");
}

void MyBackground::setBuffers() {
    bindBuffers();
    GLfloat vert[6][2] =
            {{-0.07, 0.02},
             {-0.07, -0.02},
             {0.07,  0.00}};
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), vert, GL_STATIC_DRAW);
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

void MyBackground::draw() {
    bindProgram();
    bindBuffers();
    glUniform1f(0, color);
    glUniform1f(1, color2);
    glUniform1f(2, scale);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void MyBackground::changeColor() {
    if (color < 1.0 && color > 0) {
        color += direction;
        color2 -= direction;
    } else {
        direction *= -1;
        color += direction;
        color2 -= direction;
    }
}

void MyBackground::changeSize() {
    scale -= 0.01;
}
bool MyBackground::endAnimation() {
    if (scale<-1.1)
        return true;
    else
        return false;
}