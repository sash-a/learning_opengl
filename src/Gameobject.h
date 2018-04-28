//
// Created by sasha on 2018/04/28.
//

#ifndef OGL_FINAL_GAMEOBJECT_H
#define OGL_FINAL_GAMEOBJECT_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <string>
#include "geometry.h"

class Gameobject
{
private:
public:
    Gameobject();

    GeometryData geom;

    float horizontalAngle;
    float verticalAngle;// Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction;

    // Right vector
    glm::vec3 right;
    // Up vector : perpendicular to both direction and right
    glm::vec3 up;

    float speed;

    glm::mat4 model;


    void translate(const glm::vec3 &);

    void scale(const glm::vec3 &);

    void rotate(const float &degrees, const glm::vec3 &axis);

    void loadObj(const std::string &file);
};


#endif //OGL_FINAL_GAMEOBJECT_H
