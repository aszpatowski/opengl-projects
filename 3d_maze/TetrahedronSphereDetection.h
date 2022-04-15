//
// Created by aleksander on 20.12.2021.
//

#ifndef INC_4_ZADANIE_TETRAHEDRONSPHEREDETECTION_H
#define INC_4_ZADANIE_TETRAHEDRONSPHEREDETECTION_H
#include "objects/sphere/Sphere.h"
#include "objects/tetrahedron/Tetrahedron.h"
#include <glm/glm.hpp>
#include <iostream>

class TetrahedronSphereDetection
{
    // https://realtimecollisiondetection.net/blog/?p=103 - algorithm used to check collision

    // Variables needed to check intersection triangle vs sphere
private:

    glm::vec3 preCheck;

    glm::vec3 A;
    glm::vec3 B;
    glm::vec3 C;
    glm::vec3 V;

    glm::vec3 AB;
    glm::vec3 BC;
    glm::vec3 CA;

    glm::vec3 Q1;
    glm::vec3 Q2;
    glm::vec3 Q3;
    glm::vec3 QC;
    glm::vec3 QA;
    glm::vec3 QB;

    float d;
    float e;
    float aa;
    float ab;
    float ac;
    float bb;
    float bc;
    float cc;

    float d1;
    float d2;
    float d3;

    float e1;
    float e2;
    float e3;

    bool sep[7];
    bool collision;

    const int indexes[4][3] = {{0, 1, 2},
                         {1, 2, 3},
                         {0, 2, 3},
                         {0, 1, 3}};
public:
    bool checkCollision(const Sphere &sphere, const Tetrahedron &tetrahedron);
};


#endif //INC_4_ZADANIE_TETRAHEDRONSPHEREDETECTION_H
