//
// Created by sasha on 2018/04/28.
//

#ifndef OGL_FINAL_INPUTHANDLER_H
#define OGL_FINAL_INPUTHANDLER_H

#include "SDL.h"

class InputHandler
{
public:
    bool keys [322] = {false};

    InputHandler();

    bool &operator[](const int &x);
};


#endif //OGL_FINAL_INPUTHANDLER_H
