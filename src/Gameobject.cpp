//
// Created by sasha on 2018/04/28.
//

#include <iostream>
#include "Gameobject.h"

Gameobject::Gameobject()
{
    pos = glm::vec3(0, 0, 0);
    model = glm::mat4(1.0f);
    colorbuffer;

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
    genBoundingValues();
}

void Gameobject::rotateAround(const float &degrees, const glm::vec3 &axis, const glm::vec3 &point)
{
    glm::vec3 oldPos = pos;

    translate(point - pos);
    rotate(degrees, axis);

    translate(oldPos - pos);
}

void Gameobject::genBoundingValues()
{
    std::vector<float> verts = geom.getVertices();

    for (int i = 0; i < verts.size(); i += 3)
    {
        if (verts[i] > x_max) x_max = verts[i];
        if (verts[i] < x_min) x_min = verts[i];

        if (verts[i + 1] > y_max) y_max = verts[i + 1];
        if (verts[i + 1] < y_min) y_min = verts[i + 1];

        if (verts[i + 2] > z_max) z_max = verts[i + 2];
        if (verts[i + 2] < z_min) z_min = verts[i + 2];
    }
}


