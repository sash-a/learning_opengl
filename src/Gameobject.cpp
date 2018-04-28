//
// Created by sasha on 2018/04/28.
//

#include "Gameobject.h"

Gameobject::Gameobject()
{
    horizontalAngle = 3.14f;
    verticalAngle = 0.0f;

    direction = glm::vec3(cos(verticalAngle) * sin(horizontalAngle),
                          sin(verticalAngle),
                          cos(verticalAngle) * cos(horizontalAngle));

    right = glm::vec3(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f)
    );

    up = glm::cross(right, direction);

    speed = 0.01f;

    model = glm::mat4(1.0f);
}

void Gameobject::translate(const glm::vec3 &amount)
{
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


