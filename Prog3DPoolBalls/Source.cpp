/*#define GLEW_STATIC
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

using namespace std;

void init(void);
void display(void);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);

#define WIDTH 800
#define HEIGHT 600

GLuint vbo;
GLuint programa;

float fov = 45.0f;
float zoomSpeed = 0.1f;

// Define the cobble geometry
float cobbleVertices[] = {
    // Triangle 1
    -0.5f, -0.5f,
    0.5f, -0.5f,
    -0.5f, 0.5f,
    // Triangle 2
    0.5f, -0.5f,
    0.5f, 0.5f,
    -0.5f, 0.5f
};

GLfloat angle = 0.0f;
glm::mat4 Model, View, Projection;
glm::mat3 NormalMatrix;

const GLuint NumVertices = 6 * 2 * 3;

int main(void) {
	GLFWwindow* window;

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "Pool balls - Project", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    // Initialize GLEW
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cout << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        glfwTerminate();
        return -1;
    }

    init();

    //glfwSetScrollCallback(window, scrollCallback);

	while (!glfwWindowShouldClose(window)) {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

    // Cleanup
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(programa);

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}

void init(void) {

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cobbleVertices), cobbleVertices, GL_STATIC_DRAW);

    ShaderInfo  shaders[] = {
        { GL_VERTEX_SHADER,   "shaders/poolballs.vert" },
        { GL_FRAGMENT_SHADER, "shaders/poolballs.frag" },
        { GL_NONE, NULL }
    };

    programa = LoadShaders(shaders);
    if (!programa) exit(EXIT_FAILURE);
    glUseProgram(programa);
}

void display() {

    glUseProgram(programa);

    // Set up vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Draw the cobble
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
*/

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

// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec2 position;

    void main()
    {
        gl_Position = vec4(position, 0.0, 1.0);
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Red color for simplicity
    }
)";

GLuint vbo;
GLuint programa;

void init(void);
void display(void);

float cobbleVertices[] = {
    // Triangle 1
    -0.5f, -0.5f,
    0.5f, -0.5f,
    -0.5f, 0.5f,
    // Triangle 2
    0.5f, -0.5f,
    0.5f, 0.5f,
    -0.5f, 0.5f
};

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Cobble Example", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context of the window current
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    init();


    // Create a vertex buffer object (VBO) to store the cobble's geometry
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cobbleVertices), cobbleVertices, GL_STATIC_DRAW);

    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        display();

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glfwTerminate();
    return 0;
}

void init(void) {

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cobbleVertices), cobbleVertices, GL_STATIC_DRAW);

    ShaderInfo  shaders[] = {
        { GL_VERTEX_SHADER,   "shaders/poolballs.vert" },
        { GL_FRAGMENT_SHADER, "shaders/poolballs.frag" },
        { GL_NONE, NULL }
    };

    programa = LoadShaders(shaders);
    if (!programa) exit(EXIT_FAILURE);
    glUseProgram(programa);
}

void display(void) {
    // Use the shader program
    glUseProgram(programa);

    // Set up vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    // Draw the cobble
    glDrawArrays(GL_TRIANGLES, 0, 6);
};