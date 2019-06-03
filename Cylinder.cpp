//
// Created by joscraw on 5/29/19.
//

#include "Cylinder.h"
#include <math.h>
#include <iostream>


float Cylinder::intersect(glm::vec3 pos, glm::vec3 dir)
{
    float a = (dir.x * dir.x) + (dir.z * dir.z);
    float b = 2*(dir.x*(pos.x-center.x) + dir.z*(pos.z-center.z));
    float c = (pos.x - center.x) * (pos.x - center.x) + (pos.z - center.z) * (pos.z - center.z) - (radius*radius);

    float delta = b*b - 4*(a*c);
    if(fabs(delta) < 0.001) return -1.0;
    if(delta < 0.0) return -1.0;

    float t1 = (-b - sqrt(delta))/(2*a);
    float t2 = (-b + sqrt(delta))/(2*a);
    float t;

    if (t1>t2) t = t2;
    else t = t1;

    float r = pos.y + t*dir.y;

    if ((r >= center.y) and (r <= center.y + height))return t;
    else return -1;

}


glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n = glm::vec3 (p.x-center.x,0,p.z-center.z);
    n = glm::normalize(n);
    return n;
}

std::vector<float> Cylinder::texCoord(glm::vec3 pt) {
    std::vector<float> out;

    float s = (glm::atan((pt.x-center.x), (pt.z-center.z)) + (3.14159 / 2)) / (2*3.14159);

    float t = (pt.y-center.y) / height;


    out.push_back(s);
    out.push_back(t);

    return out;
}

glm::vec3 Cylinder::procedCol (glm::vec3 pt)  {
    float y = pt.y - center.y;
    if ((long)glm::mod(y, 2.0f) == 0) {
        return color;
    } else {
        return proCol;
    }
}
