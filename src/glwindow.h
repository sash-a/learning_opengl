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

    GLuint vao;
    GLuint shader;
    GLuint vertexBuffer;

    std::vector<Gameobject> gos;

    glm::mat4 projection;
    glm::mat4 view;

    void handleInput();
    void objToGL(Gameobject &);
};

#endif
