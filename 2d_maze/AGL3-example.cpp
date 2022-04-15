// ==========================================================================
// AGL3:  GL/GLFW init AGLWindow and AGLDrawable class definitions
//
// Ver.3  14.I.2020 (c) A. Łukaszewski
// ==========================================================================
// AGL3 example usage 
//===========================================================================
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <math.h>
#include <chrono>
#include <AGL3Window.hpp>
#include <MyBackground.h>
#include <Triangle.h>

#include <iostream>

// ==========================================================================
// Window Main Loop Inits ...................................................
// ==========================================================================
class MyWin : public AGLWindow {
public:
    MyWin() {};

    MyWin(int _wd, int _ht, const char *name, int vers, int fullscr = 0)
            : AGLWindow(_wd, _ht, name, vers, fullscr) {};

    virtual void KeyCB(int key, int scancode, int action, int mods);

    void MainLoop(int N);
};


// ==========================================================================
void MyWin::KeyCB(int key, int scancode, int action, int mods) {
    AGLWindow::KeyCB(key, scancode, action, mods); // f-key full screen switch
    if ((key == GLFW_KEY_SPACE) && action == GLFW_PRESS) { ; // do something
    }
    if (key == GLFW_KEY_HOME && (action == GLFW_PRESS)) { ; // do something
    }
}


// ==========================================================================
void MyWin::MainLoop(int N) {

    ViewportOne(0, 0, wd, ht);
    int N2 = N * N;
    MyBackground background;
    Triangle triangles[N2 - 2];
    Triangle playerTriangle(false);
    Triangle endTriangle(false);
    int number = 0;
    int index = 0;
    float increment = ((1.8 / N) * (N + 1) / N);
    bool gameOn = true;
    bool endGame = false;

    for (float i = -0.9; i < 0.90; i += increment) {
        for (float j = -0.9; j < 0.90; j += increment) {
            if (number == 0) {
                playerTriangle.setPos(j, i);
                playerTriangle.setRotate(0);
                playerTriangle.setScale(N);
                playerTriangle.setVertex();
                playerTriangle.setColor(0.9, 0.9, 0.1);
                number++;
            } else if (number == N2 - 1) {
                endTriangle.setPos(j, i);
                endTriangle.setRotate(rand() % 360);
                endTriangle.setScale(N);
                endTriangle.setVertex();
                endTriangle.setColor(0.9, 0.4, 0.05);
            } else {
                triangles[index].setPos(j, i);
                triangles[index].setRotate(rand() % 360);
                triangles[index].setScale(N);
                triangles[index].setVertex();
                triangles[index].setColor();
                triangles[index].addToVectorInstance();
                number++;
                index++;
            }
        }
    }
    triangles[0].setInstance(N2-2);
    auto start = std::chrono::high_resolution_clock::now();
    do {
        glClear(GL_COLOR_BUFFER_BIT);

        AGLErrors("main-loopbegin");
        // =====================================================        Drawing

        background.draw();
        /*
        for (int i = 0; i < N2 - 2; i++) {
            triangles[i].draw();
        }*/
        triangles[0].drawInstance(N2-2);
        playerTriangle.draw();
        AGLErrors("main-here-player-draw");
        endTriangle.draw();
        AGLErrors("main-here-end-draw");
        background.changeColor();
        if (!gameOn) {
            background.changeSize();
            if (background.endAnimation()) {
                endGame = true;
            }
        }

        if ((playerTriangle && endTriangle) && (gameOn != false)) {
            std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
            std::cout << "Brawo wygrałeś w: " << elapsed.count() << " sekund" << std::endl;
            gameOn = false;
        }
        for (int i = 0; i < N2 - 2; i++) {
            if ((playerTriangle && triangles[i]) && (gameOn != false)) {
                std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start;
                std::cout << "Niestety ale przegrałeś. Twój czas to: " << elapsed.count() << " sekund" << std::endl;
                gameOn = false;
            }
        }
        AGLErrors("main-afterdraw");
        WaitForFixedFPS(60.0);
        glfwSwapBuffers(win()); // =============================   Swap buffers
        glfwPollEvents();
        if (glfwGetKey(win(), GLFW_KEY_DOWN) == GLFW_PRESS) {
            playerTriangle.up();
        }
        if (glfwGetKey(win(), GLFW_KEY_UP) == GLFW_PRESS) {
            playerTriangle.down();
        }
        if (glfwGetKey(win(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
            playerTriangle.right();
        }
        if (glfwGetKey(win(), GLFW_KEY_LEFT) == GLFW_PRESS) {
            playerTriangle.left();
        }
        if (glfwGetKey(win(), GLFW_KEY_SPACE) == GLFW_PRESS) {
            gameOn = false;
        }
    } while ((glfwGetKey(win(), GLFW_KEY_ESCAPE) != GLFW_PRESS &&
              glfwWindowShouldClose(win()) == 0) && !endGame);
}
int main(int argc, char *argv[]) {
    MyWin win;
    win.Init(1024, 768, "AGL3 example", 0, 33);
    int N;
    if (argc >= 2) {
        N = atoi(argv[1]);
        if (argc >= 3) {
            srand(atoi(argv[2]));
        } else {

            srand(time(nullptr));
        }
    } else {
        N = 10;
        srand(time(nullptr));
    }
    win.MainLoop(N);
    return 0;
}
