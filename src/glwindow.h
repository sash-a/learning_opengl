#ifndef GL_WINDOW_H
#define GL_WINDOW_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "geometry.h"
#include "Gameobject.h"
#include "InputHandler.h"

class OpenGLWindow
{
public:
    OpenGLWindow();

    void initGL();
    void render();
    bool handleEvent(SDL_Event e);
    void cleanup();

    void genObject();

    InputHandler inputHandler;
    /**
     * T = Translate
     * R = Rotate
     * S = Scale
     * X,Y,Z = Change to that axis
     *
     * example: state[0] = T
     *          state[1] = X
     * This will translate on the x axis
     */
    std::string state [2];
private:
    SDL_Window* sdlWin;

    std::vector<GLuint> vaos;
    GLuint shader;
    std::vector<GLuint> vertexBuffers;

    std::vector<Gameobject> gos;

    glm::mat4 projection;
    glm::mat4 view;

    void handleInput();
    void objToGL(const int &);
};

#endif
