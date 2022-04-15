#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <chrono>
#include "AGL3Window.hpp"
#include "objects/sphere/Sphere.h"
#include "objects/cube/Cube.h"
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
    Cube box;
    box.setTranslationMatrix(glm::vec3(0.0, 0.0, 0.0));
    box.setRotationMatrix(0, glm::vec3(1.0, 1.0, 1.0));
    box.setScaleMatrix(glm::vec3(2, 0.8, 1));
    box.setColorFactor(glm::vec3(1.0, 0.2, 0.8));


    Sphere bubbles[120];

    int currentLevel = 0;
    int maxLevel = 8;

    const float speed = 0.004f;
    float movementSpeed = speed;
    constexpr float cameraSpeed = 0.4f;
    constexpr float mouseSensitivity = 15.0f;

    int points = 0;

    float radius = 0.05f;
    float startBubbleRadius = 0.05f;
    Camera camera1;
    camera1.setPos(glm::vec3(0 + radius, 0.4, 0.5));
    camera1.setScale(radius);
    Sphere player(10);
    player.setScale(radius);
    float xMouse = 0;
    float yMouse = 0;

    glm::vec3 viewPosition;
    glm::vec3 upperLightPosition = {2.0f, 3.0f, 0.0};

    bool nextLevel = false;
    bool endGame = false;
    bool isWin = false;

    int randomNumber;

    int clickCounter = 0;

    setCursor(wd / 2, ht / 2);
    setPoints(points);
    glm::vec3 distancePlayerEnd;


    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    do
    {

        if (currentLevel < maxLevel) currentLevel++;
        else {
            isWin = true;
            endGame = true;
        }
        nextLevel = false;
        camera1.setPos(glm::vec3(0 + radius, 0.4, 0.5));
        size_t numberOfBubbles = 40 + currentLevel * 10;
        std::cout<<"Level: "<<currentLevel <<std::endl;
        std::cout<<"Ilosc bąbelek: "<<numberOfBubbles<<std::endl;
        float speedOfBubbles = 0.0001f * float(currentLevel); // + float(rand() % 1000) / 1000.0f;
        for (size_t i = 0; i < numberOfBubbles; i++)
        {
            bubbles[i].setScale(startBubbleRadius);
            bubbles[i].setPos(glm::vec3(float(rand() % 1800) / 1000.0f +0.2f, float(rand() % 800) / 1000.0f,
                                        float(rand() % 1000) / 1000.0f));
            bubbles[i].setSpeed(glm::vec3(0.0f, speedOfBubbles + float(rand() % 100) / 30000.0f, 0.0f));
            randomNumber = rand() % 10;
            bubbles[i].setAlive(true);
            if (randomNumber < 3)
            {
                bubbles[i].setIsSpecial(true);
                switch (randomNumber)
                {
                    case 0:
                        bubbles[i].setReward(100);
                        bubbles[i].setColor(0.2f, 0.9f, 0.1f);
                        break;
                    case 1:
                        bubbles[i].setReward(200);
                        bubbles[i].setColor(0.1f, 0.1f, 0.8f);
                        break;
                    case 2:
                        bubbles[i].setReward(400);
                        bubbles[i].setColor(0.3f, 0.1f, 0.7f);
                        break;
                }
            } else
            {
                bubbles[i].setIsSpecial(false);
                bubbles[i].setColor(0.8f, 0.0f, 0.2f);
            }
            bubbles[i].addToInstance();
        }
        bubbles[0].setInstance(numberOfBubbles);


        do
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glViewport(0, 0, wd, ht);
            glScissor(0, 0, wd, ht);

            AGLErrors("main-loop-begin");
            // =====================================================        Drawing
            switch (viewMode)
            {
                case View::firstPerson:
                    viewPosition = camera1.getPos();
                    box.draw(camera1.getMVP(), 0, 36);
                    bubbles[0].drawInstance(numberOfBubbles, camera1.getMVP(), viewPosition, player.getPos(),
                                            upperLightPosition);
                    break;
                case View::thirdPerson:
                    viewPosition = camera1.getPosThirdPerson();
                    box.draw(camera1.getMVPThirdPerson(), 0, 18);
                    box.draw(camera1.getMVPThirdPerson(), 24, 36);
                    player.draw(camera1.getMVPThirdPerson());
                    bubbles[0].drawInstance(numberOfBubbles, camera1.getMVPThirdPerson(), viewPosition, player.getPos(),
                                            upperLightPosition);
                    break;
                case View::miniMap:
                    viewPosition = camera1.getPosMiniMap();
                    box.draw(camera1.getMVPMiniMap(), 0, 18);
                    player.draw(camera1.getMVPMiniMap());
                    bubbles[0].drawInstance(numberOfBubbles, camera1.getMVPMiniMap(), viewPosition, player.getPos(),
                                            upperLightPosition);
                    break;
            }
            AGLErrors("main-after-draw");
            WaitForFixedFPS(60.0);
            glfwSwapBuffers(win()); // =============================   Swap buffers
            glfwPollEvents();

            //Checking collision

            if (glfwGetKey(win(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
            {
                movementSpeed = speed * 2;
            } else
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
            player.setPos(camera1.getPos());
            camera1.update();
            if (clickCounter < 20) clickCounter++;
            if (player.getPos().x + radius > 1.9f)
            {
                std::cout<<"Brawo następny level!!"<<std::endl;
                nextLevel = true;
            }
            for (size_t i = 0; i < numberOfBubbles; i++)
            {
                if(bubbles[i].getAlive())
                {
                    bubbles[i].setPos(bubbles[i].getPos() + bubbles[i].getSpeed());
                    if (bubbles[i].getPos().y - bubbles[i].getRadius() > 0.8f)
                    {
                        bubbles[i].setScale(startBubbleRadius);
                        bubbles[i].setPos(
                                glm::vec3(float(rand() % 1800) / 1000.0f + 0.2f, float(rand() % 100) / 1000.0f,
                                          float(rand() % 1000) / 1000.0f));
                    }
                    bubbles[i].setScale(startBubbleRadius * (bubbles[i].getPos().y + 0.5));
                    distancePlayerEnd = player.getPos() - bubbles[i].getPos();
                    if (glm::dot(distancePlayerEnd, distancePlayerEnd) <
                        (player.getRadius() + bubbles[i].getRadius()) * (player.getRadius() + bubbles[i].getRadius()))
                    {
                        if (!bubbles[i].getIsSpecial())
                        {
                            std::cout << "twoj koniec" << std::endl;
                            std::cout << "Level: " << currentLevel << std::endl;
                            std::cout << "Udało Ci się zebrać: " << points << " punktow" << std::endl;
                            //endGame = true;
                        } else
                        {
                            std::cout << "brawo" << std::endl;
                            std::cout << "dostałeś: " << bubbles[i].getReward() << std::endl;
                            points += bubbles[i].getReward();
                            setPoints(points);
                            bubbles[i].setAlive(false);
                            bubbles[i].setScale(0.0f);
                        }
                    }
                }
                bubbles[i].updateInstance(i);

            }
        } while ((glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
                  glfwWindowShouldClose(win()) == 0) && (!nextLevel && !endGame));
    } while ((glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
              glfwWindowShouldClose(win()) == 0) && !endGame || !isWin);
    if(isWin)
    {
        std::cout<<"BRAWO UDALO CI SIĘ PRZEJŚĆ GRE!!"<<std::endl;
        std::cout<<"Ilość zdobytych punktów to: "<<points<<std::endl;
    }
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
