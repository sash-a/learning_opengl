//
// Created by sasha on 2018/04/28.
//

#include <iostream>
#include "InputHandler.h"

InputHandler::InputHandler()
{}

bool &InputHandler::operator[](const int &x)
{
    return keys[x];
}
