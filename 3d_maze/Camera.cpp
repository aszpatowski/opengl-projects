//
// Created by aleksander on 02.12.2021.
//

#include "Camera.h"

glm::mat4 Camera::getMVP() const
{
    return MVP;
}

glm::mat4 Camera::getMVPThirdPerson() const
{
    return MVPThirdPerson;
}

glm::vec3 Camera::getPos()
{
    return pos;
}

void Camera::setPos(glm::vec3 newPos)
{
    pos = newPos;
}

void Camera::moveFront(float direction)
{
    tempPos = pos + front * direction;
    if (!checkCollisionFloor(tempPos))
        pos = tempPos;
    else
        pos -= front * (direction * 0.02f);
}

void Camera::moveRight(float direction)
{
    tempPos = pos + right * direction;
    if (!checkCollisionFloor(tempPos))
        pos = tempPos;
    else
        pos -= right * (direction * 0.02f);
}

void Camera::moveUp(float direction)
{
    tempPos = pos + up * direction;
    if (!checkCollisionFloor(tempPos))
        pos = tempPos;
    else
        pos -= up * (direction * 0.02f);
}

void Camera::rotate(float x, float y)
{
    yaw += x;
    pitch += y;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(direction);
    right = glm::normalize(glm::cross(front,
                                      worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up = glm::normalize(glm::cross(right, front));
    //targetOfSight = glm::normalize(pos-direction);
    //printYawAndPitch();
    //printPos();

}

void Camera::update()
{
    View = glm::lookAt(
            pos, // Camera is at (4,3,-3), in World Space
            pos + front, // and looks at the origin
            up  // Head is up (set to 0,-1,0 to look upside-down)
    );
    ViewThirdPerson = glm::lookAt(
            pos - 0.2f*front, // Camera is at (4,3,-3), in World Space
            pos, // and looks at the origin
            up  // Head is up (set to 0,-1,0 to look upside-down)
    );
    /*ViewMiniMap = glm::lookAt(
            glm::vec3(-1.16707, 2.15028, 2.15827
            ), // Camera is at (4,3,-3), in World Space
            glm::vec3(-0.60768, 1.55842, 1.57794
            ), // Camera is at (4,3,-3), in World Space
            // and looks at the origin
            glm::vec3(0.410748, 0.806041, -0.426126
            )  // and looks at the origin
            // Head is up (set to 0,-1,0 to look upside-down)
    );*/
    //std::cout<<front.x<<" "<<front.y<<" "<<front.z<<"\n";
    //old
    /*ViewThirdPerson = glm::lookAt(
            glm::vec3(-3.21866, 7.09954, 6.96629), // Camera is at (4,3,-3), in World Space
            glm::vec3(-2.57348, 6.57608, 6.40975), // Camera is at (4,3,-3), in World Space
            // and looks at the origin
            glm::vec3(0.396364, 0.852054, -0.341906)  // and looks at the origin
            // Head is up (set to 0,-1,0 to look upside-down)
    );*/
    /*ViewThirdPerson = glm::lookAt(
            glm::vec3(-1.16707, 2.15028, 2.15827
            ), // Camera is at (4,3,-3), in World Space
            glm::vec3(-0.60768, 1.55842, 1.57794
            ), // Camera is at (4,3,-3), in World Space
            // and looks at the origin
            glm::vec3(0.410748, 0.806041, -0.426126
            )  // and looks at the origin
            // Head is up (set to 0,-1,0 to look upside-down)
    );*/
    /*printPos();
    printPosPlusFront();
    printUp();*/
    MVP = Projection * View * Model;
    MVPThirdPerson = Projection * ViewThirdPerson * Model;
    MVPMiniMap = Projection * ViewMiniMap * Model;
}


void Camera::setFront(glm::vec3 newFront)
{
    front = newFront;
}

void Camera::setScale(float newScale)
{
    scale = newScale;
    Projection = glm::perspective(glm::radians(30.0f), 4.0f / 3.0f, scale, 100.0f);
}

void Camera::printYawAndPitch()
{
    std::cout << "Yaw: " << yaw << "\t" << "Pitch: " << "\t" << pitch << std::endl;
}

void Camera::printPos()
{
    std::cout << "Pos: " << pos.x << "\t" << pos.y << "\t" << pos.z << std::endl;
}

void Camera::printUp()
{
    std::cout << "UP: " << up.x << "\t" << up.y << "\t" << up.z << std::endl;
}

void Camera::printFront()
{
    std::cout << "Front: " << front.x << "\t" << front.y << "\t" << front.z << std::endl;
}

void Camera::printPosPlusFront()
{
    std::cout << "Pos+Front: " << pos.x + front.x << "\t" << pos.y + front.y << "\t" << pos.z + front.z << std::endl;
}

bool Camera::checkCollisionFloor(glm::vec3 newPos)
{

    if (newPos.x + scale > 1 || newPos.x - scale < 0)
        return true;
    else if (newPos.y + scale > 1 || newPos.y - scale < 0)
        return true;
    else if (newPos.z + scale > 1 || newPos.z - scale < 0)
        return true;
    else
        return false;


}

glm::mat4 Camera::getMVPMiniMap() const
{
    return MVPMiniMap;
}

