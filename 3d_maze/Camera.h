//
// Created by aleksander on 02.12.2021.
//

#ifndef INC_4_ZADANIE_CAMERA_H
#define INC_4_ZADANIE_CAMERA_H

#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <math.h>
#include <vector>
#include <iostream>

class Camera
{
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 newUp = glm::vec3(0.0f, 1.0f, 0.0f), float newYaw = 38.0f, float newPitch = 35.0f, float radius = 0.02f)
    {
        pos = position;
        worldUp = newUp;
        yaw = newYaw;
        pitch = newPitch;
        Projection = glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, radius, 100.0f);
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
        right = glm::normalize(glm::cross(front, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));

        View       = glm::lookAt(
                pos, // Camera is at (4,3,-3), in World Space
                pos+front, // and looks at the origin
                up  // Head is up (set to 0,-1,0 to look upside-down)
        );

        ViewThirdPerson = glm::lookAt(
                pos - 0.2f*front, // Camera is at (4,3,-3), in World Space
                pos, // and looks at the origin
                up  // Head is up (set to 0,-1,0 to look upside-down)
        );

        ViewMiniMap = glm::lookAt(
                glm::vec3(-1.16707, 2.15028, 2.15827
                ), // Camera is at (4,3,-3), in World Space
                glm::vec3(-0.60768, 1.55842, 1.57794
                ), // Camera is at (4,3,-3), in World Space
                // and looks at the origin
                glm::vec3(0.410748, 0.806041, -0.426126
                )  // and looks at the origin
                // Head is up (set to 0,-1,0 to look upside-down)
        );

        Model = glm::mat4(1.0f);
        MVP = Projection*View*Model;
        MVPThirdPerson = Projection * ViewThirdPerson * Model;


    }
    glm::mat4 getMVP() const;
    glm::mat4 getMVPMiniMap() const;
    glm::mat4 getMVPThirdPerson() const;
    glm::vec3 getPos();
    void setPos(glm::vec3 newPos);
    void setFront(glm::vec3 newFront);
    void setScale(float newScale);
    void moveFront(float direction);
    void moveRight(float direction);
    void moveUp(float direction);
    bool checkCollisionFloor(glm::vec3 newPos);
    void rotate(float x,float y);
    void update();
    float getRadius2();

    //Debugging function;
    void printYawAndPitch();
    void printPos();
    void printUp();
    void printFront();
    void printPosPlusFront();



private:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::mat4 Projection;
    glm::mat4 View;
    glm::mat4 ViewThirdPerson;
    glm::mat4 ViewMiniMap;
    glm::mat4 Model;
    glm::mat4 MVP;
    glm::mat4 MVPThirdPerson;
    glm::mat4 MVPMiniMap;
    glm::vec3 direction;
    glm::vec3 tempPos;
    float scale = 1;
    float yaw = 0;
    float pitch = 90;
};


#endif //INC_4_ZADANIE_CAMERA_H
