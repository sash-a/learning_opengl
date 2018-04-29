#include <iostream>
#include <stdio.h>

#include "SDL.h"
#include <GL/glew.h>
#include <algorithm>

#include "glwindow.h"

using namespace std;
using namespace glm;


const char *glGetErrorString(GLenum error)
{
    switch (error)
    {
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "UNRECOGNIZED";
    }
}

void glPrintError(const char *label = "Unlabelled Error Checkpoint", bool alwaysPrint = false)
{
    GLenum error = glGetError();
    if (alwaysPrint || (error != GL_NO_ERROR))
    {
        printf("%s: OpenGL error flag is %s\n", label, glGetErrorString(error));
    }
}

GLuint loadShader(const char *shaderFilename, GLenum shaderType)
{
    FILE *shaderFile = fopen(shaderFilename, "r");
    if (!shaderFile)
    {
        return 0;
    }

    fseek(shaderFile, 0, SEEK_END);
    long shaderSize = ftell(shaderFile);
    fseek(shaderFile, 0, SEEK_SET);

    char *shaderText = new char[shaderSize + 1];
    size_t readCount = fread(shaderText, 1, shaderSize, shaderFile);
    shaderText[readCount] = '\0';
    fclose(shaderFile);

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, (const char **) &shaderText, NULL);
    glCompileShader(shader);

    delete[] shaderText;

    return shader;
}

GLuint loadShaderProgram(const char *vertShaderFilename,
                         const char *fragShaderFilename)
{
    GLuint vertShader = loadShader(vertShaderFilename, GL_VERTEX_SHADER);
    GLuint fragShader = loadShader(fragShaderFilename, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if (linkStatus != GL_TRUE)
    {
        GLsizei logLength = 0;
        GLchar message[1024];
        glGetProgramInfoLog(program, 1024, &logLength, message);
        cout << "Shader load error: " << message << endl;
        return 0;
    }

    return program;
}

OpenGLWindow::OpenGLWindow()
{
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    // Or, for an ortho camera :
    // projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

    // Camera matrix
    view = glm::lookAt(
            vec3(5, 5, 5), // Camera is at (4,3,3), in World Space
            vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );

    state[0] = "t";
    state[1] = "x";
}

void OpenGLWindow::initGL()
{
    // We need to first specify what type of OpenGL context we need before we can create the window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    sdlWin = SDL_CreateWindow("OpenGL Prac 1",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, SDL_WINDOW_OPENGL);

    if (!sdlWin)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Error", "Unable to create window", 0);
    }

    SDL_GLContext glc = SDL_GL_CreateContext(sdlWin);
    SDL_GL_MakeCurrent(sdlWin, glc);
    SDL_GL_SetSwapInterval(1);

    glewExperimental = true;
    GLenum glewInitResult = glewInit();
    glGetError(); // Consume the error erroneously set by glewInit()
    if (glewInitResult != GLEW_OK)
    {
        const GLubyte *errorString = glewGetErrorString(glewInitResult);
        cout << "Unable to initialize glew: " << errorString;
        return;
    }

    int glMajorVersion;
    int glMinorVersion;
    glGetIntegerv(GL_MAJOR_VERSION, &glMajorVersion);
    glGetIntegerv(GL_MINOR_VERSION, &glMinorVersion);
    cout << "Loaded OpenGL " << glMajorVersion << "." << glMinorVersion << " with:" << endl;
    cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
    cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
    cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
    cout << "\tGLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glClearColor(1, 1, 1, 1); // BACKGROUND

    // Note that this path is relative to your working directory
    // when running the program (IE if you run from within build
    // then you need to place these files in build as well)
    shader = loadShaderProgram("SimpleTransform.vertexshader", "SingleColor.fragmentshader");
    glUseProgram(shader);

    int colorLoc = glGetUniformLocation(shader, "objectColor");
    glUniform3f(colorLoc, 0.0f, 0.0f, 0.0f);

    // Load the model that we want to use and buffer the vertex attributes
    go.geom.loadFromOBJFile("../objects/doggo.obj");
//    gos.push_back(go);

    go1.geom.loadFromOBJFile("../objects/tri.obj");
//    GLuint v = 1;
//    vaos.push_back(v);
//    GLuint vert = 1;
//    vertexBuffers.push_back(vert);

//    Gameobject go1;
//    go1.geom.loadFromOBJFile("../objects/tri.obj");
//    gos.push_back(go1);
//    GLuint v1 = 2;
//    vao.push_back(v1);
//    GLuint vert1 = 2;
//    vertexBuffer.push_back(vert1);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // for wire mesh

    glPrintError("Setup complete", true);
}

void OpenGLWindow::render()
{
    objToGL(1);

    handleInput();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 mvp = projection * view * go.model;

    // Send our transformation to the currently bound shader,
    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &mvp[0][0]);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vao);
    glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void *) 0          // array buffer offset
    );

    glDrawArrays(GL_TRIANGLES, 0, go.geom.vertexCount() + go1.geom.vertexCount());

    // Swap the front and back buffers on the window, effectively putting what we just "drew"
    // onto the screen (whereas previously it only existed in memory)
    SDL_GL_SwapWindow(sdlWin);
}

// The program will exit if this function returns false
bool OpenGLWindow::handleEvent(SDL_Event e)
{
    // A list of keycode constants is available here: https://wiki.libsdl.org/SDL_Keycode
    // Note that SDL provides both Scancodes (which correspond to physical positions on the keyboard)
    // and Keycodes (which correspond to symbols on the keyboard, and might differ across layouts)
    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_ESCAPE)
        {
            return false;
        }
    }
    return true;
}

void OpenGLWindow::cleanup()
{

    glDeleteBuffers(1, &(vertexBuffer));
    glDeleteVertexArrays(1, &(vao));


    SDL_DestroyWindow(sdlWin);
}

void OpenGLWindow::handleInput()
{
    int fixCode = 1073741881 - 128;

    vec3 x = vec3(1.f, 0, 0);
    vec3 y = vec3(0, 1.f, 0);
    vec3 z = vec3(0, 0, 1.f);
    vec3 axis;
    if (state[1] == "x")
    {
        axis = x;
    } else if (state[1] == "y")
    {
        axis = y;
    } else
    {
        axis = z;
    }


    if (inputHandler[SDLK_LEFT - fixCode])
    {
        if (state[0] == "t")
        {
//            for (Gameobject &go : gos)
                go.translate(-axis * 0.01f);


        } else if (state[0] == "s")
        {
//            for (Gameobject &go : gos)
                go.scale(vec3(0.8, 0.8, 0.8));

        } else if (state[0] == "r")
        {
//            for (Gameobject &go : gos)
                go.rotate(-0.1f, axis);

        }
    } else if (inputHandler[SDLK_RIGHT - fixCode])
    {
        if (state[0] == "t")
        {
//            for (Gameobject &go : gos)
                go.translate(axis * 0.01f);

        } else if (state[0] == "s")
        {
//            for (Gameobject &go : gos)
                go.scale(vec3(1.2, 1.2, 1.2));

        } else if (state[0] == "r")
        {
//            for (Gameobject &go : gos)
                go.rotate(0.1f, axis);

        }
    } else if (inputHandler[SDLK_UP - fixCode])
    {
        if (state[0] == "t")
        {
//            for (Gameobject &go : gos)
                go.translate(axis * 0.01f);

        } else if (state[0] == "s")
        {
//            for (Gameobject &go : gos)
                go.scale(vec3(1.2, 1.2, 1.2));

        } else if (state[0] == "r")
        {
//            for (Gameobject &go : gos)
                go.rotate(0.1f, axis);
        }
    } else if (inputHandler[SDLK_DOWN - fixCode])
    {
        if (state[0] == "t")
        {
//            for (Gameobject &go : gos)
                go.translate(-axis * 0.01f);

        } else if (state[0] == "s")
        {
//            for (Gameobject &go : gos)
                go.scale(vec3(0.8, 0.8, 0.8));

        } else if (state[0] == "r")
        {
//            for (Gameobject &go : gos)
                go.rotate(-0.1f, axis);
        }
    } else if (inputHandler[SDLK_t])
    {
        state[0] = "t";
    } else if (inputHandler[SDLK_s])
    {
        state[0] = "s";
    } else if (inputHandler[SDLK_r])
    {
        state[0] = "r";
    } else if (inputHandler[SDLK_x])
    {
        state[1] = "x";
    } else if (inputHandler[SDLK_y])
    {
        state[1] = "y";
    } else if (inputHandler[SDLK_z])
    {
        state[1] = "z";
    }
}

void OpenGLWindow::objToGL(const int &current)
{
    int bufSize = 0;
    for (Gameobject &go : gos)
    {
        bufSize += (go.geom.vertexCount() * 3 * sizeof(float));

    }

    // NEED SPECIFIC BUFFER FOR EACH OBJ
    glGenVertexArrays(1, &(vao));
    glBindVertexArray(vao);

    glGenBuffers(1, &(vertexBuffer));
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 (go.geom.vertexCount() + go1.geom.vertexCount()) * 3 * sizeof(float),
                 0,
                 GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER,
                    0,
                    go.geom.vertexCount() * 3 * sizeof(float),
                    go.geom.vertexData());
    glBufferSubData(GL_ARRAY_BUFFER,
                    go.geom.vertexCount() * 3 * sizeof(float),
                    go1.geom.vertexCount() * 3 * sizeof(float),
                    go1.geom.vertexData());

}
