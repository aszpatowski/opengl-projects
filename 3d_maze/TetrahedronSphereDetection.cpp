//
// Created by aleksander on 20.12.2021.
//

#include "TetrahedronSphereDetection.h"

bool TetrahedronSphereDetection::checkCollision(const Sphere &sphere, const Tetrahedron &tetrahedron)
{
    preCheck = tetrahedron.getPoints(0)-sphere.getPos();
    if(glm::dot(preCheck,preCheck)<(sphere.getRadius()))
    {
        for (auto &triangleIndexes: indexes)
        {
            A = tetrahedron.getPoints(triangleIndexes[0]) - sphere.getPos();
            B = tetrahedron.getPoints(triangleIndexes[1]) - sphere.getPos();
            C = tetrahedron.getPoints(triangleIndexes[2]) - sphere.getPos();
            V = glm::cross(B - A, C - A);
            d = glm::dot(A, V);
            e = glm::dot(V, V);
            sep[0] = (d * d) > (sphere.getRadius2() * e);
            if(sep[0]) return false;
            aa = glm::dot(A, A);
            ab = glm::dot(A, B);
            ac = glm::dot(A, C);
            bb = glm::dot(B, B);
            bc = glm::dot(B, C);
            cc = glm::dot(C, C);
            sep[1] = (aa > sphere.getRadius2()) && (ab > aa) && (ac > aa);
            if(sep[1]) return false;
            sep[2] = (bb > sphere.getRadius2()) && (ab > bb) && (bc > bb);
            if(sep[2]) return false;
            sep[3] = (cc > sphere.getRadius2()) && (ac > cc) && (bc > cc);
            if(sep[3]) return false;
            AB = B - A;
            BC = C - B;
            CA = A - C;
            d1 = ab - aa;
            d2 = bc - bb;
            d3 = ac - cc;
            e1 = glm::dot(AB, AB);
            e2 = glm::dot(BC, BC);
            e3 = glm::dot(CA, CA);
            Q1 = A * e1 - d1 * AB;
            Q2 = B * e2 - d2 * BC;
            Q3 = C * e3 - d3 * CA;
            QC = C * e1 - Q1;
            QA = A * e2 - Q2;
            QB = B * e3 - Q3;
            sep[4] = (glm::dot(Q1, Q1) > sphere.getRadius2() * e1 * e1) && (glm::dot(Q1, QC) > 0);
            if(sep[4]) return false;
            sep[5] = (glm::dot(Q2, Q2) > sphere.getRadius2() * e2 * e2) && (glm::dot(Q2, QA) > 0);
            if(sep[5]) return false;
            sep[6] = (glm::dot(Q3, Q3) > sphere.getRadius2() * e3 * e3) && (glm::dot(Q3, QB) > 0);
            if(sep[6]) return false;
            collision = !(sep[0] || sep[1] || sep[2] || sep[3] || sep[4] || sep[5] || sep[6]);
            if(collision)
                return collision;
        }
        return false;
    } else
    {
        return false;
    }
}