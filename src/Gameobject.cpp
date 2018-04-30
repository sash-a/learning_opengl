//
// Created by sasha on 2018/04/28.
//

#include <iostream>
#include "Gameobject.h"

Gameobject::Gameobject()
{
    pos = glm::vec3(0, 0, 0);

    model = glm::mat4(1.0f);
}

void Gameobject::translate(const glm::vec3 &amount)
{
    pos += amount;
    model = glm::translate(model, amount);
}

void Gameobject::scale(const glm::vec3 &amount)
{
    model = glm::scale(model, amount);
}

void Gameobject::rotate(const float &degrees, const glm::vec3 &axis)
{
    model = glm::rotate(model, degrees, axis);
}

void Gameobject::loadObj(const std::string &file)
{
    geom.loadFromOBJFile(file);
}

void Gameobject::rotateAround(const float &degrees, const glm::vec3 &axis, const glm::vec3 &point)
{
    glm::vec3 oldPos = pos;

    translate(point - pos);
    rotate(degrees, axis);

    translate(oldPos - pos);
}


