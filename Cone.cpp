//
// Created by joscraw on 5/31/19.
//
#include "Cone.h"
#include <math.h>

float Cone::intersect(glm::vec3 posn, glm::vec3 dir)
{
    float an = posn.x - centre.x;
    float bn = posn.z - centre.z;
    float dn = height - posn.y + centre.y;


    float tan = (radius / height) * (radius / height);

    float a = (dir.x * dir.x) + (dir.z * dir.z) - (tan*(dir.y * dir.y));
    float b = (2*an*dir.x) + (2*bn*dir.z) + (2*tan*dn*dir.y);
    float c = (an*an) + (bn*bn) - (tan*(dn*dn));

    float delta = b*b - 4*(a*c); // Quadratic complex comonent


    if(fabs(delta) < 0.001) return -1.0; // Return no object if complex part is close to zero
    if(delta < 0.0) return -1.0;

    // Full quadratic
    float t1 = (-b - sqrt(delta))/(2*a);
    float t2 = (-b + sqrt(delta))/(2*a);
    float t;

    if (t1>t2) t = t2;
    else t = t1;

    float r = posn.y + t*dir.y;

    if ((r > centre.y) and (r < centre.y + height)) {
        return t;
    }
    else return -1; // No intercestion
}



glm::vec3 Cone::normal(glm::vec3 pt)
{
    float temp = sqrt((pt.x-centre.x)*(pt.x-centre.x) + (pt.z-centre.z)*(pt.z-centre.z));

    glm::vec3 n = glm::vec3(pt.x-centre.x, temp*(radius/height), pt.z-centre.z);

    n = glm::normalize(n);
    return n;
}

std::vector<float> Cone::texCoord(glm::vec3 pt) {
    std::vector<float> out;


    out.push_back(0);
    out.push_back(0);

    return out;
}

glm::vec3 Cone::procedCol(glm::vec3 pt) {
    return glm::vec3(0);
}
