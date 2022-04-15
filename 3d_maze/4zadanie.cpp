#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <chrono>
#include "AGL3Window.hpp"
#include "objects/sphere/Sphere.h"
#include "objects/tetrahedron/Tetrahedron.h"
#include "objects/cube/Cube.h"
#include "Camera.h"
#include "TetrahedronSphereDetection.h"
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
    int N3 = N * N * N;
    Cube box;
    box.setTranslationMatrix(glm::vec3(0.0, 0.0, 0.0));
    box.setRotationMatrix(0, glm::vec3(1.0, 1.0, 1.0));
    box.setScaleMatrix(1);
    box.setColorFactor(glm::vec3(1.0, 0.2, 0.8));

    float startLoc = 0.05;
    float endLoc = 0.95;

    Tetrahedron tetrahedrons[N3 - 2];
    Tetrahedron tetrahedronsReward[N3 - 2];

    float scale = 0.23f / N;
    float increment = (((endLoc - startLoc) / N) * (N + 1) / N);


    //const float speed = 0.04f / N;
    const float speed = 0.004f;
    float movementSpeed = speed;
    constexpr float cameraSpeed = 0.4f;
    constexpr float mouseSensitivity = 15.0f;

    int points = 0;


    size_t number = 0;
    Camera camera1;
    Sphere player(15);
    Sphere end(15);


    float xMouse = 0;
    float yMouse = 0;

    bool gameOn = true;
    bool endGame = false;

    setCursor(wd / 2, ht / 2);
    setPoints(points);

    TetrahedronSphereDetection collision;

    size_t obstaclesSize = 0;
    size_t rewardsSize = 0;

    for (float x = startLoc; x <= endLoc; x += increment)
    {
        for (float y = startLoc; y <= endLoc; y += increment)
        {
            for (float z = startLoc; z <= endLoc; z += increment)
            {
                if (number == 0)
                {
                    player.setPos(x+scale, y+scale, z+scale);
                    player.setScale(scale);

                    camera1.setPos(glm::vec3(x+scale, y+scale, z+scale));
                    camera1.setScale(scale);
                    number++;
                } else if (number == N3 - 1)
                {

                    end.setPos(x-scale, y-scale, z-scale);
                    end.setScale(scale);
                    camera1.setFront(glm::vec3(x, y, z));
                    number++;
                } else
                {
                    if (int(rand() % 100) < 10) //zara
                    {
                        tetrahedronsReward[rewardsSize].setUniqueId(number);
                        tetrahedronsReward[rewardsSize].setTranslationMatrix(glm::vec3(x, y, z));
                        tetrahedronsReward[rewardsSize].setRotationMatrix(float(rand() % 360),
                                                                          glm::vec3(float(rand() %
                                                                                          1000) /
                                                                                    1000.0,
                                                                                    float(rand() %
                                                                                          1000) /
                                                                                    1000.0,
                                                                                    float(rand() %
                                                                                          1000) /
                                                                                    1000.0));
                        tetrahedronsReward[rewardsSize].setReward(true);
                        tetrahedronsReward[rewardsSize].setScaleMatrix(scale);
                        tetrahedronsReward[rewardsSize].setPoints();
                        tetrahedronsReward[rewardsSize].updatePoints();
                        tetrahedronsReward[rewardsSize].setMVP(camera1.getMVP());
                        tetrahedronsReward[rewardsSize].addToInstanceMatrixesReward();
                        rewardsSize++;
                        number++;
                    } else
                    {
                        tetrahedrons[obstaclesSize].setUniqueId(number);
                        tetrahedrons[obstaclesSize].setTranslationMatrix(glm::vec3(x, y, z));
                        tetrahedrons[obstaclesSize].setRotationMatrix(float(rand() % 360),
                                                                      glm::vec3(float(rand() % 1000) / 1000.0,
                                                                                float(rand() % 1000) / 1000.0,
                                                                                float(rand() % 1000) /
                                                                                1000.0));
                        tetrahedrons[obstaclesSize].setReward(false);
                        tetrahedrons[obstaclesSize].setScaleMatrix(scale);
                        tetrahedrons[obstaclesSize].setPoints();
                        tetrahedrons[obstaclesSize].updatePoints();
                        tetrahedrons[obstaclesSize].setMVP(camera1.getMVP());
                        tetrahedrons[obstaclesSize].addToInstanceMatrixes();
                        obstaclesSize++;
                        number++;
                    }


                }
            }
        }
    }

    tetrahedrons->setInstance(obstaclesSize);
    tetrahedronsReward->setInstanceReward(rewardsSize);
    glm::vec3 distancePlayerEnd;
    float sumRadPlayerEnd = (player.getRadius() + end.getRadius()) * (player.getRadius() + end.getRadius());

    auto start = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed;

    //Instrukcja i klawiszologia:
    /*
     W - poruszanie do przodu
     S - poruszanie do tyłu
     */
    std::cout << "Klawiszologia:" << std::endl << std::endl;
    std::cout << "------------------------------------------" << std::endl << std::endl;
    std::cout << "ESC - Wyjście z gry" << std::endl;
    std::cout << "WSAD - poruszanie do przodu, tyłu, lewo, prawo" << std::endl;
    std::cout << "Lewy SHIFT - poruszanie do góry" << std::endl;
    std::cout << "Spacja - poruszanie w dół" << std::endl;
    std::cout << "Lewy CTRL - przyśpieszenie" << std::endl;
    std::cout << "Strzałki lub myszka - obrót kamerą" << std::endl<<std::endl;
    std::cout << "Przełączanie widoku: " << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "F - Włączenie/wyłączenie trybu pełnoekranowego" << std::endl;
    std::cout << "B - widok z pierwszej osoby" << std::endl;
    std::cout << "N - widok z trzeciej osoby" << std::endl;
    std::cout << "M - Mapa" << std::endl<<std::endl;
    std::cout << "Cel gry:" << std::endl;
    std::cout << "------------------------------------------" << std::endl;
    std::cout << "Przejdź na drugi koniec mapy do niebieskiej kulki." << std::endl;
    std::cout << "Po drodze zbieraj złote artefakty." << std::endl;
    std::cout << "Pamiętaj, tym większy artefakt tym więcej jest wart." << std::endl;
    std::cout << "Końcowy wynik zależy od czasu przejścia oraz od ilości uzbieranych punktów." << std::endl;
    std::cout << "Powodzenia." << std::endl;




    do
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, wd, ht);
        glScissor(0, 0, wd, ht);
        glEnable(GL_DEPTH_TEST);

        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);

        AGLErrors("main-loop-begin");
        // =====================================================        Drawing
        switch (viewMode)
        {
            case View::firstPerson:
                tetrahedrons->setMVP(camera1.getMVP());
                tetrahedronsReward->setMVP(camera1.getMVP());
                box.draw(camera1.getMVP(), 0, 36);
                end.draw(camera1.getMVP());
                break;
            case View::thirdPerson:
                tetrahedrons->setMVP(camera1.getMVPThirdPerson());
                tetrahedronsReward->setMVP(camera1.getMVPThirdPerson());
                box.draw(camera1.getMVPThirdPerson(), 6, 12);
                box.draw(camera1.getMVPThirdPerson(), 24, 36);
                player.draw(camera1.getMVPThirdPerson());
                end.draw(camera1.getMVPThirdPerson());
                break;
            case View::miniMap:
                tetrahedrons->setMVP(camera1.getMVPMiniMap());
                tetrahedronsReward->setMVP(camera1.getMVPMiniMap());
                box.draw(camera1.getMVPMiniMap(), 0, 18);
                end.draw(camera1.getMVPMiniMap());
                player.draw(camera1.getMVPMiniMap());
                break;
        }
        for (size_t i = 0; i < obstaclesSize; i++)
        {
            tetrahedrons[i].updateInstanceMatrixes(i);
        }
        tetrahedrons->drawInstance(obstaclesSize);
        for (size_t i = 0; i < rewardsSize; i++)
        {
            tetrahedronsReward[i].updateInstanceMatrixesReward(i);
        }
        tetrahedronsReward->drawInstanceReward(rewardsSize);
        AGLErrors("main-after-draw");
        WaitForFixedFPS(60.0);
        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();

        //Checking collision



        for (size_t i = 0; i < obstaclesSize; i++)
        {
            tetrahedrons[i].updateAngle();
            tetrahedrons[i].updatePoints();
            if (collision.checkCollision(player, tetrahedrons[i]))
            {
                elapsed = std::chrono::high_resolution_clock::now() - start;
                std::cout << "Niestety przegrałeś, uderzyłeś w ostrosłupa ;( i nie udało Ci się osiągnać celu."
                          << std::endl;
                std::cout << "Punkty które zdobyłeś: " << points << std::endl;
                std::cout << "W czasie: " << elapsed.count() << " sekund" << std::endl;
                endGame = true;
                break;
            }
        }
        for (size_t i = 0; i < rewardsSize; i++)
        {
            if (tetrahedronsReward[i].isAliveF())
            {
                tetrahedronsReward[i].updateScale();
                tetrahedronsReward[i].updatePoints();
                if (collision.checkCollision(player, tetrahedronsReward[i]))
                {
                    elapsed = std::chrono::high_resolution_clock::now() - start;
                    points += tetrahedronsReward[i].getReward();
                    setPoints(points);
                    tetrahedronsReward[i].die();
                    std::cout << "Twoje punkty: " << points << std::endl << "Oby tak dalej." << std::endl;
                    std::cout << "Twój czas jak na razie to: " << elapsed.count() << " sekund. Dawaj dawaj."
                              << std::endl;
                    break;

                }
            }

        }
        distancePlayerEnd = player.getPos() - end.getPos();
        if (glm::dot(distancePlayerEnd, distancePlayerEnd) < sumRadPlayerEnd)
        {
            elapsed = std::chrono::high_resolution_clock::now() - start;
            std::cout << "Brawo wygrałeś, nie uderzyłeś w ostrosłupa ;) i udało Ci się osiągnać cel."
                      << std::endl;
            std::cout << "Punkty które zdobyłeś: " << points << std::endl;
            std::cout << "W czasie: " << elapsed.count() << " sekund" << std::endl;
            std::cout << "Wynik końcowy to: " << static_cast<float>(points) / elapsed.count() << std::endl;
            endGame = true;
        }
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
        if (glfwGetKey(win(), GLFW_KEY_B) == GLFW_PRESS)
        {

            viewMode = View::firstPerson;
        }
        if (glfwGetKey(win(), GLFW_KEY_N) == GLFW_PRESS)
        {
            viewMode = View::thirdPerson;
        }
        if (glfwGetKey(win(), GLFW_KEY_M) == GLFW_PRESS)
        {
            viewMode = View::miniMap;
        }
        GetCursorNDC(&xMouse, &yMouse);
        camera1.rotate(xMouse * mouseSensitivity, yMouse * mouseSensitivity);
        setCursor(wd / 2, ht / 2);
        player.setPos(camera1.getPos());
        camera1.update();

    } while ((glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
              glfwWindowShouldClose(win()) == 0) && !endGame);
}

int main(int argc, char *argv[])
{
    MyWin win;
    win.Init(1024, 768, "Zadanie 4", 0, 33);
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
