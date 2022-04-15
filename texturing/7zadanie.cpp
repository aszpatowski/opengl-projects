#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <chrono>
//#include "AGL3Window.hpp"
#include "Mesh.hpp"
#include "Camera.h"
#include <vector>
#include <iostream>
#include <algorithm>

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================



class MyWin : public AGLWindow
{
public:
    MyWin()
    {};

    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr = 0)
            : AGLWindow(_wd, _ht, name, vers, fullscr)
    {};

    virtual void KeyCB(int key, int scancode, int action, int mods);

    void MainLoop(int N);
};


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods)
{
    AGLWindow::KeyCB(key, scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS)
    { ; // do something
    }
    if (key == GLFW_KEY_HOME && (action == GLFW_PRESS))
    { ; // do something
    }
}


// ==========================================================================
void MyWin::MainLoop(int N)
{
    enum class View
    {
        firstPerson,
        thirdPerson,
        miniMap
    };

    View viewMode = View::firstPerson;

    ViewportOne(0, 0, wd, ht);


    const float speed = 0.2f;
    float movementSpeed = speed;
    constexpr float cameraSpeed = 10.0f;
    constexpr float mouseSensitivity = 15.0f;

    float radius = 0.05f;
    Camera camera1;
    camera1.setPos(glm::vec3(-0.15, -11, 9.05));
    camera1.setScale(radius);


    Mesh Cat("./objects/cat");
    Cat.scale *= 0.1;
    Cat.setShader("./shaders/mesh");

    Mesh Monkey("./objects/monkey");
    Monkey.scale *= 0.1;
    Monkey.pos =glm::vec3(6.0,0,0);
    Monkey.setShader("./shaders/mesh");

    Mesh Dog("./objects/dog");
    Dog.scale *= 0.1;
    Dog.pos =glm::vec3(-6.0,0,0);
    Dog.setShader("./shaders/perlin");

    float xMouse = 0;
    float yMouse = 0;



    int clickCounter = 0;

    setCursor(wd / 2, ht / 2);
    setPoints(points);


    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);


    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, wd, ht);
        glScissor(0, 0, wd, ht);

        //AGLErrors("main-loop-begin");
        // =====================================================        Drawing
        switch (viewMode)
        {
            case View::firstPerson:
                Cat.draw(camera1.getMVP(),camera1.getPos());
                Monkey.draw(camera1.getMVP(),camera1.getPos());
                Dog.draw(camera1.getMVP(),camera1.getPos());
                break;
            case View::thirdPerson:
                break;
            case View::miniMap:
                break;
        }
        //AGLErrors("main-after-draw");
        WaitForFixedFPS(60.0);
        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();

        //Checking collision

        if (glfwGetKey(win(), GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {
            movementSpeed = speed * 2;
        }
        if (glfwGetKey(win(), GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
        {
            movementSpeed = speed * 2;
        }
        if(glfwGetKey(win(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            movementSpeed = speed;
        }
        if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            camera1.rotate(0, -cameraSpeed);
        }
        if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS)
        {
            camera1.rotate(0, cameraSpeed);
        }
        if (glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            camera1.rotate(cameraSpeed, 0);
        }
        if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            camera1.rotate(-cameraSpeed, 0);
        }
        if (glfwGetKey(win(), GLFW_KEY_W) == GLFW_PRESS)
        {
            camera1.moveFront(movementSpeed);
        }
        if (glfwGetKey(win(), GLFW_KEY_S) == GLFW_PRESS)
        {
            camera1.moveFront(-movementSpeed);
        }
        if (glfwGetKey(win(), GLFW_KEY_D) == GLFW_PRESS)
        {
            camera1.moveRight(movementSpeed);
        }
        if (glfwGetKey(win(), GLFW_KEY_A) == GLFW_PRESS)
        {
            camera1.moveRight(-movementSpeed);
        }
        if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            camera1.moveUp(movementSpeed);
        }
        if (glfwGetKey(win(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            camera1.moveUp(-movementSpeed);
        }
        if (glfwGetKey(win(), GLFW_KEY_TAB) == GLFW_PRESS)
        {

            if (clickCounter >= 20)
            {
                if (viewMode == View::firstPerson) viewMode = View::thirdPerson;
                else if (viewMode == View::thirdPerson) viewMode = View::miniMap;
                else viewMode = View::firstPerson;
                clickCounter = 0;
            }

        }
        GetCursorNDC(&xMouse, &yMouse);
        camera1.rotate(xMouse * mouseSensitivity, yMouse * mouseSensitivity);
        setCursor(wd / 2, ht / 2);
        camera1.update();
        if (clickCounter < 20) clickCounter++;
    } while ((glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
              glfwWindowShouldClose(win()) == 0));
}

int main(int argc, char *argv[])
{
    MyWin win;
    win.Init(1024, 768, "Zadanie 5", 0, 33);
    int N;
    if (argc >= 2)
    {
        N = atoi(argv[1]);
        if (N <= 1)
        {
            N = 2;
            std::cout << "Minimal N = 2" << std::endl;
        }
        if (argc >= 3)
        {
            srand(atoi(argv[2]));
        } else
        {

            srand(time(nullptr));
        }
    } else
    {
        N = 10;
        srand(time(nullptr));
    }
    win.MainLoop(N);
    return 0;
}
