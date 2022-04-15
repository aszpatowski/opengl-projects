
//
// Created by aleksander on 01.02.2022.
//

//https://learnopengl.com/Guest-Articles/2021/Tessellation/Height-map
// https://learnopengl.com/Guest-Articles/2021/Tessellation/Tessellation
//Source of maps:
//http://www.viewfinderpanoramas.org/Coverage%20map%20viewfinderpanoramas_org3.htm
#include <iostream>     // std::cout
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <cstdlib>      // std::rand, std::srand
// Include standard headers
#include <cstdio>

// Include GLEW
#include <epoxy/gl.h>


// Include GLFW
#include <GLFW/glfw3.h>

GLFWwindow *window;

// Include GLM
#include <glm/glm.hpp>


#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include "chunk.cpp"

#include "shader_builder.cpp"

#include "map_function.cpp"

#include <unistd.h>


float horizontalAngle = 29.00f;
float verticalAngle = 2.50f;

float blkscale = 25.0;

glm::vec3 CameraDirection;
glm::mat4 ViewMatrix;

void MessageCallback(GLenum source,
                     GLenum type,
                     GLuint id,
                     GLenum severity,
                     GLsizei length,
                     const GLchar *message,
                     const void *userParam)
{
    fprintf(stderr, "GL CALLBACK: %s \n type = 0x%x, \nseverity = 0x%x, \nmessage = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);

    if (type == GL_DEBUG_TYPE_ERROR)
    {
        int *a = NULL; // halt gdb. or entire system if kernel is not fast enough;
        int payload = 2000;
        *a = payload;
    }
}

float camz = -15.0;

void handle_controls(glm::vec3 &PerhapsCameraPosition, glm::vec3 &up, bool process)
{
    double mousex, mousey;
    glfwGetCursorPos(window, &mousex, &mousey);
    double mouseSpeed = 0.01;
    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1200 / 2, 1200 / 2);
    if (process)
    {
        horizontalAngle -= mouseSpeed * float(1200 / 2 - mousex);
        verticalAngle += mouseSpeed * float(1200 / 2 - mousey);
    }

    if (verticalAngle < 0.5 * 3.14)
        verticalAngle = 0.5 * 3.14;
    if (verticalAngle > 1.5 * 3.14)
        verticalAngle = 1.5 * 3.14;

    float speed;
    std::cout << "horizontal: " << horizontalAngle << "  vertical: " << verticalAngle << std::endl;
    if (viewMode == TOPDOWN)
    {
        speed = PerhapsCameraPosition[2] * 0.025f;

        CameraDirection = glm::vec3(
                cos(3.1415) * sin(3.1415),
                sin(3.1415),
                cos(3.1415) * cos(3.1415)
        );

        glm::vec3 right = glm::vec3(sin(3.1415 - 3.14f / 2.0f), 0, cos(3.1415 - 3.14f / 2.0f));
        up = glm::cross(right, CameraDirection);
    } else
    {
        speed = abs(PerhapsCameraPosition[2] - 5.0) * 0.005f;
        CameraDirection = glm::vec3(
                cos(verticalAngle) * sin(horizontalAngle),
                sin(verticalAngle),
                cos(verticalAngle) * cos(horizontalAngle)
        );

        glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));
        up = glm::cross(right, CameraDirection);
    }

    float deltaTime = 1.0;

    if (viewMode == TOPDOWN)
    {
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            PerhapsCameraPosition -= vec3(0.0, 1.0, 0.0) * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            PerhapsCameraPosition += vec3(0.0, 1.0, 0.0) * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            PerhapsCameraPosition += vec3(1.0, 0.0, 0.0) * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            PerhapsCameraPosition -= vec3(1.0, 0.0, 0.0) * deltaTime * speed;
        }
    } else
    {
        float speed = 0.05 * abs(camz);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            PerhapsCameraPosition += vec3(0.0, 1.0, 0.0) * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            PerhapsCameraPosition -= vec3(0.0, 1.0, 0.0) * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            PerhapsCameraPosition += vec3(1.0, 0.0, 0.0) * deltaTime * speed;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            PerhapsCameraPosition -= vec3(1.0, 0.0, 0.0) * deltaTime * speed;
        }
    }

    if (PerhapsCameraPosition.x > 0.0) PerhapsCameraPosition.x = -359.0;
    if (PerhapsCameraPosition.x < -360.0) PerhapsCameraPosition.x = -1.0;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    if (viewMode == TOPDOWN)
    {
        camz -= 0.025 * (float) yoffset * camz;
    } else
    {
        camz -= 0.1 * (float) yoffset * camz;
    }
}

bool shouldInclude(int Off_x, int Off_y, int latl, int lath, int lonl, int lonh)
{
    if ((latl <= 360 - (180 - Off_x) && lath >= 360 - (180 - Off_x)) && (lonl <= -Off_y && lonh >= -Off_y))
        return true;

    return false;
}

int main(int argc, char **argv)
{
    string path;
    int latl;
    int lath;
    int lonl;
    int lonh;
    bool loadall = false;

    if (argc == 2)
    {
        path = argv[1];
        loadall = true;
        std::cout << "using " << path << " as file folder path, loading all!\n";

    } else if (argc == 6)
    {
        loadall = false;
        path = argv[1];
        lonl = atoi(argv[2]);
        lonh = atoi(argv[3]);
        latl = atoi(argv[4]);
        lath = atoi(argv[5]);
        std::cout << "using " << path << " as file folder path, loading " << latl << "-" << lath << " " << lonl << "-"
                  << lonh << "\n";
    } else
    {


        std::cout << "Run with: " << std::endl;

        std::cout << "./zadanie6 path_to_maps" << std::endl;
        std::cout << "./zadanie6 path_to_maps lat_low lat_high lon_low lon_high" << std::endl;
        std::cout << "For example: " << std::endl;
        std::cout << "./zadanie6 maps 8 9 30 31" << std::endl;

        exit(1);
    }

    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "6_ZADANIE_TEREN", NULL, NULL);
    if (window == NULL)
    {
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.7f, 0.7f, 0.7f, 0.0f);

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetCursorPos(window, 1024 / 2, 768 / 2);


    float lastCommit = glfwGetTime();

    int gameBind = 2;

    float latencyKey = 0.1f;

    bool isLinear = false;

    std::pair<int, int> shaderResult = BuildPipeline();
    unsigned int shaderHandle = shaderResult.first;
    unsigned int bufferLocation = shaderResult.second;

    std::vector<Chunk> mapData;

    std::vector<std::string> mapTargets = getTargets(path);
    glm::vec3 PerhapsCameraPosition = glm::vec3(blkscale, blkscale, blkscale);

    int xt = 0, yt = 0;
    unsigned int Lod = 0;
    for (int i = 0; i < mapTargets.size(); i += 1)
    {
        int maptLen = mapTargets[i].size();
        int mOffsetx = 180 - ((mapTargets[i][maptLen - 7] - '0') * 100 + (mapTargets[i][maptLen - 6] - '0') * 10 +
                              (mapTargets[i][maptLen - 5] - '0') * 1);
        if (mapTargets[i][maptLen - 8] == 'E')
            mOffsetx *= -1;
        else
            mOffsetx -= 360;

        int mOffsety = (mapTargets[i][maptLen - 10] - '0') * 10 + (mapTargets[i][maptLen - 9] - '0') * 1;
        if (mapTargets[i][maptLen - 11] == 'N')
            mOffsety *= -1;

        std::cout << mapTargets[i] << " " << mOffsetx << " " << mOffsety << std::endl;
        if (loadall || shouldInclude(mOffsetx, mOffsety, latl, lath, lonl, lonh))
        {
            std::cout << "LOADED" << std::endl;
            unsigned short int *map_data = readfile(mapTargets[i]);
            Chunk tmp_patch;
            tmp_patch.init(shaderHandle, bufferLocation, 10, 10, 4, map_data);
            tmp_patch.setBeginOffset(glm::vec3((float) mOffsetx, (float) mOffsety, 0.0));
            xt += mOffsetx;
            yt += mOffsety;
            mapData.push_back(tmp_patch);
        }
    }

    Chunk background;
    background.init(shaderHandle, bufferLocation, 10, 10, 4, NULL);
    background.setBeginOffset(glm::vec3(0.0, 0.0, 0.0));

    PerhapsCameraPosition = glm::vec3((float) xt / (float) mapData.size() + 0.5,
                                      (float) yt / (float) mapData.size() + 0.5, -15.0);
    glm::vec3 up;
    double last_time = glfwGetTime() - 1.0;
    std::cout << "Loaded: " << mapData.size() << " chunks." << std::endl;
    do
    {

        double time = glfwGetTime();
        double deltaTime = time - last_time;
        last_time = time;
        std::cout<< deltaTime << "s per frame, " << 1.0 / deltaTime << " fps, "<< pow(2, Lod)<<" lod "<<std::endl;
        PerhapsCameraPosition.z = camz;
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (gameBind == 2)
            handle_controls(PerhapsCameraPosition, up, true);

        // recalculate ViewMatrix and MVP.
        glm::vec3 apparent_camera_position;
        if (viewMode == SPHERE)
        {
            float earthRadius = PerhapsCameraPosition[2] + 5.0;
            float hRadius = earthRadius * cos(3.1415f / 180.0 * PerhapsCameraPosition[1]);
            apparent_camera_position = glm::vec3(
                    hRadius * cos(3.1415 / 180.0 * PerhapsCameraPosition[0]),
                    earthRadius * sin(3.1415 / 180.0 * PerhapsCameraPosition[1]),
                    hRadius * sin(3.1415 / 180.0 * PerhapsCameraPosition[0])
            );
        } else apparent_camera_position = PerhapsCameraPosition;

        glm::vec3 centerPoint = apparent_camera_position + CameraDirection;
        ViewMatrix = glm::lookAt(apparent_camera_position, centerPoint, up);
        glm::mat4 Projection = glm::infinitePerspective(glm::radians(45.0f), 4.0f / 3.0f, 0.001f);
        glm::mat4 Model = glm::mat4(1.0);
        glm::mat4 MVP = Projection * ViewMatrix * Model;


        for (int i = 0; i < mapData.size(); ++i)
        {
            mapData[i].draw(apparent_camera_position, Model, Projection, MVP, Lod, isLinear);
        }


        background.draw(apparent_camera_position, Model, Projection, MVP, Lod, isLinear);


        AGLErrors("main-loopBegin");
        glfwSwapBuffers(window);

        if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey + 0.5f)
            {
                lastCommit = glfwGetTime();
                if (viewMode == TOPDOWN)
                {
                    PerhapsCameraPosition.z = 5.0;
                    camz = 5.0;
                    viewMode = SPHERE;
                } else
                {
                    viewMode = TOPDOWN;
                    PerhapsCameraPosition.z = -5.0;
                    camz = -5.0;
                }
            }
        }
        if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                PerhapsCameraPosition -= vec3(0.0, 0.0, 0.01);
            }
        }

        if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                PerhapsCameraPosition += vec3(0.0, 0.0, 0.01);
            }
        }

        // unbind cursor from window
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                gameBind -= 1;
            }
        }

        /* rebind cursor to window */
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                gameBind = 2;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                Lod = 0;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                Lod = 1;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                Lod = 2;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                Lod = 3;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                Lod = 4;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                Lod = 5;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                Lod = 6;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey)
            {
                lastCommit = glfwGetTime();
                Lod = 7;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            if (glfwGetTime() > lastCommit + latencyKey + 0.5f)
            {
                lastCommit = glfwGetTime();
                if (isLinear)
                    isLinear = false;
                else
                    isLinear = true;
            }
        }

    } while (gameBind > 0 && glfwWindowShouldClose(window) == 0);

    glfwTerminate();

    return 0;
}

