#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp>

#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <string>
#include <vector>

#include "LoadShaders.h"

GLuint vbo;
GLuint vao;
GLuint programa;

glm::mat4 Model, View, Projection;
glm::mat3 NormalMatrix;
GLfloat angle = 0.0f;

const GLuint NumVertices = 6 * 2 * 3;

void init(void);
void display(void);
float fov = 45.0f;
float zoomSpeed = 0.1f;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

#define WIDTH 800
#define HEIGHT 600



int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cobble Example", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context of the window current
    glfwMakeContextCurrent(window);

    // Inicia o gestor de extens�es GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    init();

    //glfwSetScrollCallback(window, scrollCallback);


    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        display();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    return 0;
}

void init(void) {

    float xCoord = 1.25f;
    float yCoord = 0.25f;
    float zCoord = 1.25f;
    
    GLfloat cobbleVertices[NumVertices * 6] = {
        //*************************************************
        //                       X+ (face #0)
        // ************************************************
        // Primeiro tri�ngulo
        // Posi��es
        xCoord, -yCoord,  zCoord,		1.0f, 0.0f, 0.0f,
        xCoord, -yCoord, -zCoord,		1.0f, 0.0f, 0.0f,
        xCoord,  yCoord,  zCoord,		1.0f, 0.0f, 0.0f,
        // Segundo tri�ngulo
        // Posi��es
        xCoord,  yCoord,  zCoord,		1.0f, 0.0f, 0.0f,
        xCoord, -yCoord, -zCoord,		1.0f, 0.0f, 0.0f,
        xCoord,  yCoord, -zCoord,		1.0f, 0.0f, 0.0f,
        // ************************************************
        //                       X- (face #1)
        // ************************************************
        // Primeiro tri�ngulo
        // Posi��es
        -xCoord, -yCoord, -zCoord,	-1.0f, 0.0f, 0.0f,
        -xCoord, -yCoord,  zCoord,	-1.0f, 0.0f, 0.0f,
        -xCoord,  yCoord, -zCoord,	-1.0f, 0.0f, 0.0f,
        // Segundo tri�ngulo
        // Posi��es
        -xCoord,  yCoord, -zCoord,	-1.0f, 0.0f, 0.0f,
        -xCoord, -yCoord,  zCoord,	-1.0f, 0.0f, 0.0f,
        -xCoord,  yCoord,  zCoord,	-1.0f, 0.0f, 0.0f,
        // ************************************************
        //                       Y+ (face #2)
        // ************************************************
        // Primeiro tri�ngulo
        // Posi��es
        -xCoord,  yCoord,  zCoord,	0.0f, 1.0f, 0.0f,
         xCoord,  yCoord,  zCoord,	0.0f, 1.0f, 0.0f,
        -xCoord,  yCoord, -zCoord,	0.0f, 1.0f, 0.0f,
        // Segundo tri�ngulo
        // Posi��es
        -xCoord,  yCoord, -zCoord,	0.0f, 1.0f, 0.0f,
         xCoord,  yCoord,  zCoord,	0.0f, 1.0f, 0.0f,
         xCoord,  yCoord, -zCoord,	0.0f, 1.0f, 0.0f,
         // ************************************************
         //                       Y- (face #3)
         // ************************************************
         // Primeiro tri�ngulo
         // Posi��es
         -xCoord, -yCoord, -zCoord,	0.0f, -1.0f, 0.0f,
          xCoord, -yCoord, -zCoord,	0.0f, -1.0f, 0.0f,
         -xCoord, -yCoord,  zCoord,	0.0f, -1.0f, 0.0f,
         // Segundo tri�ngulo
         // Posi��es
         -xCoord, -yCoord,  zCoord,	0.0f, -1.0f, 0.0f,
          xCoord, -yCoord, -zCoord,	0.0f, -1.0f, 0.0f,
          xCoord, -yCoord,  zCoord,	0.0f, -1.0f, 0.0f,
          // ************************************************
          //                       Z+ (face #4)
          // ************************************************
          // Primeiro tri�ngulo
          // Posi��es
          -xCoord, -yCoord, zCoord,	0.0f, 0.0f, 1.0f,
           xCoord, -yCoord, zCoord,	0.0f, 0.0f, 1.0f,
          -xCoord,  yCoord, zCoord,	0.0f, 0.0f, 1.0f,
          // Segundo tri�ngulo
          // Posi��es
          -xCoord,  yCoord, zCoord,	0.0f, 0.0f, 1.0f,
           xCoord, -yCoord, zCoord,	0.0f, 0.0f, 1.0f,
           xCoord,  yCoord, zCoord,	0.0f, 0.0f, 1.0f,
           // ************************************************
           //                       Z- (face #5)
           // ************************************************
           // Primeiro tri�ngulo
           // Posi��es
            xCoord, -yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
           -xCoord, -yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
            xCoord,  yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
            // Segundo tri�ngulo
            // Posi��es
             xCoord,  yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
            -xCoord, -yCoord, -zCoord,	0.0f, 0.0f, -1.0f,
            -xCoord,  yCoord, -zCoord,	0.0f, 0.0f, -1.0f

    };

    glGenVertexArrays(1, &vao);
    // Faz bind do VAO, cujo nome est� definido em 'VAO', com o contexto do OpenGL.
    // Um VAO � criado no primero bind que lhe seja feito.
    // Este VAO passa a estar ativo at� que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(cobbleVertices), cobbleVertices, 0);

    ShaderInfo  shaders[] = {
        { GL_VERTEX_SHADER,   "shaders/poolballs.vert" },
        { GL_FRAGMENT_SHADER, "shaders/poolballs.frag" },
        { GL_NONE, NULL }
    };

    programa = LoadShaders(shaders);
    if (!programa) exit(EXIT_FAILURE);
    glUseProgram(programa);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLint positionId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "position");
    // Obt�m a localiza��o do atributo 'vNormal' no 'programa'.
    GLint normalId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "normal");

    glVertexAttribPointer(positionId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, (3 + 3) * sizeof(float) /*stride*/, (void*)0);
    glVertexAttribPointer(normalId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_TRUE, (3 + 3) * sizeof(float) /*stride*/, (void*)(3 * sizeof(float)));


    glEnableVertexAttribArray(positionId);
    glEnableVertexAttribArray(normalId);

    Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    View = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 5.0f),	// eye (posição da câmara).
        glm::vec3(0.0f, 0.0f, 0.0f),	// center (para onde está a "olhar")
        glm::vec3(0.0f, 1.0f, 0.0f)		// up
    );
    Model = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 ModelView = View * Model;
    NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));

    // ****************************************************
    // Uniforms
    // ****************************************************

    // Atribui valor ao uniform Model
    GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
    glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));
    // Atribui valor ao uniform View
    GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
    glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
    // Atribui valor ao uniform ModelView
    GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
    glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
    // Atribui valor ao uniform Projection
    GLint projectionId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Projection");
    glProgramUniformMatrix4fv(programa, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));
    // Atribui valor ao uniform NormalMatrix
    GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
    glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));


    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void display(void) {

    static const GLfloat black[] = {
    0.0f, 0.0f, 0.0f, 0.0f
    };
    // Limpa o buffer de cor
    glClearBufferfv(GL_COLOR, 0, black);
    // Limpa o buffer de profundidade
    glClear(GL_DEPTH_BUFFER_BIT);

    // Use the shader program
    glUseProgram(programa);
    
    /*Model = glm::rotate(glm::mat4(1.0f), angle += 0.0002f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
    glm::mat4 ModelView = View * Model;
    NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));
    // Atribui valor ao uniform Model
    GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
    glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));
    GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
    glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
    GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
    glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
    // Atribui valor ao uniform NormalMatrix
    GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
    glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));*/

    glBindVertexArray(vao);

    // Draw the cobble
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
};


void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    // Adjust the field of view based on the vertical scroll offset
    fov -= static_cast<float>(yoffset) * zoomSpeed;

    // Limit the field of view to avoid extreme zooming
    if (fov < 1.0f) {
        fov = 1.0f;
    }
    else if (fov > 45.0f) {
        fov = 45.0f;
    }

    // Update any necessary matrices or variables used for rendering
    // ...
}