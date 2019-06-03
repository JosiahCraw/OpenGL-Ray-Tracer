//
// Created by joscraw on 5/29/19.
//

#ifndef LAB7_CYLINDER_H
#define LAB7_CYLINDER_H

#include <glm/glm.hpp>
#include "SceneObject.h"


class Cylinder : public SceneObject
{

private:
    glm::vec3 center;
    float radius, height;

public:
    Cylinder()
            : center(glm::vec3(0)), radius(1), height(10)  //Default constructor creates a unit sphere
    {
        color = glm::vec3(1);
    };

    Cylinder(glm::vec3 c, float r, float h, glm::vec3 col)
            : center(c), radius(r), height(h)
    {
        color = col;
    };

    float intersect(glm::vec3 posn, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 p);

    std::vector<float> texCoord(glm::vec3 pt);

    glm::vec3 procedCol (glm::vec3 pt);

};

#endif //LAB7_CYLINDER_H
