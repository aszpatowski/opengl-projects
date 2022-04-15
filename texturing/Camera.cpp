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

glm::vec3 Camera::getPos() const
{
    return pos;
}
glm::vec3 Camera::getPosMiniMap() const
{
    return posMiniMap;
}
glm::vec3 Camera::getPosThirdPerson() const
{
    return posThirdPerson;
}

void Camera::setPos(glm::vec3 newPos)
{
    pos = newPos;
}

void Camera::moveFront(float direction)
{
    pos = pos + front * direction;
}

void Camera::moveRight(float direction)
{
    pos = pos + right * direction;
}

void Camera::moveUp(float direction)
{
    pos = pos + up * direction;
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
    posThirdPerson =pos - 0.6f*front;
    ViewThirdPerson = glm::lookAt(
            posThirdPerson, // Camera is at (4,3,-3), in World Space
            pos, // and looks at the origin
            up  // Head is up (set to 0,-1,0 to look upside-down)
    );
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

    if (newPos.x + scale > 2 || newPos.x - scale < 0)
        return true;
    else if (newPos.y + scale > 0.8 || newPos.y - scale < 0)
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

