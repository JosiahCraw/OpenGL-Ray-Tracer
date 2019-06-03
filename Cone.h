//
// Created by joscraw on 5/31/19.
//

#ifndef LAB7_CONE_H
#define LAB7_CONE_H

#include <glm/glm.hpp>
#include "SceneObject.h"
#include <vector>


class Cone : public SceneObject {
private:
    glm::vec3 centre;
    float radius, height;

public:

    Cone(void);

    Cone(glm::vec3 c, float r, float h, glm::vec3 col)
            : radius(r), height(h), centre(c) {
        color = col;
    };

    float intersect(glm::vec3 posn, glm::vec3 dir) override;

    glm::vec3 normal(glm::vec3 pt) override;

    std::vector<float> texCoord(glm::vec3 pt) override;

    glm::vec3 procedCol(glm::vec3 pt) override;
};

#endif //LAB7_CONE_H
