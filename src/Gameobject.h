//
// Created by sasha on 2018/04/28.
//

#ifndef OGL_FINAL_GAMEOBJECT_H
#define OGL_FINAL_GAMEOBJECT_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GL/glew.h>

#include <cmath>
#include <string>
#include "geometry.h"

class Gameobject
{
private:
    void genBoundingValues();
public:
    Gameobject();

    // Bound box values
    float x_max = -10000;
    float x_min = 10000;

    float y_max = -10000;
    float y_min = 10000;

    float z_max = -10000;
    float z_min = 10000;


    GLuint colorbuffer;

    GeometryData geom;
    glm::mat4 model;

    glm::vec3 pos;

    void translate(const glm::vec3 &);

    void scale(const glm::vec3 &);

    void rotate(const float &degrees, const glm::vec3 &axis);

    void rotateAround(const float &degrees, const glm::vec3 &axis, const glm::vec3 &point);

    void loadObj(const std::string &file);

};


#endif //OGL_FINAL_GAMEOBJECT_H
