//
// Created by joscraw on 5/30/19.
//

#ifndef LAB7_CUBE_H
#define LAB7_CUBE_H
#include <glm/glm.hpp>
#include "SceneObject.h"
#include "Plane.h"
#include <vector>

class Cube : public SceneObject
{
private:
    float  width, height, depth;  //The four vertices
    glm::vec3 origin;
    std::vector<Plane*> planes;
public:
    Cube(void);

    Cube(glm::vec3 ori, float w, float h, float d, glm::vec3 col)
            : origin(ori), width(w), height(h), depth(d)
    {
        Plane *plane0 = new Plane (glm::vec3(origin.x, origin.y+height, origin.z),
                                  glm::vec3(origin.x, origin.y, origin.z),
                                  glm::vec3(origin.x+width, origin.y, origin.z),
                                  glm::vec3(origin.x+width, origin.y+height, origin.z),
                                  col);
        Plane *plane1 = new Plane (glm::vec3(origin.x, origin.y+height, origin.z+depth),
                                  glm::vec3(origin.x, origin.y, origin.z+depth),
                                  glm::vec3(origin.x, origin.y, origin.z),
                                  glm::vec3(origin.x, origin.y+height, origin.z),
                                  col);
        Plane *plane2 = new Plane (glm::vec3(origin.x+width, origin.y+height, origin.z),
                                   glm::vec3(origin.x+width, origin.y, origin.z),
                                   glm::vec3(origin.x+width, origin.y, origin.z+depth),
                                   glm::vec3(origin.x+width, origin.y+height, origin.z+depth),
                                   col);
        Plane *plane3 = new Plane (glm::vec3(origin.x, origin.y+height, origin.z+depth),
                                  glm::vec3(origin.x, origin.y+height, origin.z),
                                  glm::vec3(origin.x+width, origin.y+height, origin.z),
                                  glm::vec3(origin.x+width, origin.y+height, origin.z+depth),
                                  col);
        Plane *plane4 = new Plane (glm::vec3(origin.x+width, origin.y+height, origin.z+depth),
                                  glm::vec3(origin.x+width, origin.y, origin.z+depth),
                                  glm::vec3(origin.x, origin.y, origin.z+depth),
                                  glm::vec3(origin.x, origin.y+height, origin.z+depth),
                                  col);
        //Plane *plane5 = new Plane (glm::vec3(-300., -10, 200),
        //                        glm::vec3(300., -10, 200),
        //                        glm::vec3(300., -10, -200),
        //                        glm::vec3(-300., -10, -200),
        //                        col);
        color = col;
        planes.push_back(plane0);
        planes.push_back(plane1);
        planes.push_back(plane2);
        planes.push_back(plane3);
        planes.push_back(plane4);
    };

    float intersect(glm::vec3 posn, glm::vec3 dir);

    glm::vec3 normal(glm::vec3 pt);

    std::vector<float> texCoord(glm::vec3 pt) override;

    glm::vec3 procedCol (glm::vec3 pt);


};

#endif //LAB7_CUBE_H
