// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

bool active;


float rot = 0.0f;
float rot2 = 0.0f;
float rot3 = 0.0f;


bool reinicio = false;
bool cambio = false;
int movimiento = 0;
float movx = 0.0f;
float movz = 0.0f;

float movx1 = 0.0f;


bool anim = true;
float posicion = 0.0;

bool recorrido1 = true;
bool recorrido2 = false;

bool recorrido3 = true;
bool recorrido4 = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");

    // Load models
    Model restaurante((char*)"Models/RESTAURANTE/RESTAURANTE.obj");
    Model almeja((char*)"Models/RESTAURANTE/ALMEJA.obj");
    Model nieve((char*)"Models/RESTAURANTE/NIEVE.obj");
    Model medusa((char*)"Models/RESTAURANTE/MEDUSA.obj");
    Model medusa2((char*)"Models/RESTAURANTE/MEDUSA2.obj");


    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        restaurante.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.954f, 2.312f, 2.34f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        almeja.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-2.212f, 0.655f, 3.271f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        nieve.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-10.0f + movx, 5.0f, -3.0f + movz));
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-4.0f + movx, 5.0f, -1.0f + movz));
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.0f + movx, 3.0f, 2.0f + movz));
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(2.0f + movx, 2.0f, 1.0f + movz));
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(5.0f + movx, 0.0f, 0.0f + movz));
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-10.0f + movx, 1.0f, 3.0f + movz));
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa.Draw(shader);


        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(10.0f + movx1, 3.0f, 3.0f + movz));
        model = glm::rotate(model, glm::radians(rot3), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa2.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.0f + movx1, 3.0f, 0.0f + movz));
        model = glm::rotate(model, glm::radians(rot3), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa2.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f + movx1, 3.0f, 1.0f + movz));
        model = glm::rotate(model, glm::radians(rot3), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa2.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(6.0f + movx1, 1.0f, 4.0f + movz));
        model = glm::rotate(model, glm::radians(rot3), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa2.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f + movx1, 1.0f, 4.0f + movz));
        model = glm::rotate(model, glm::radians(rot3), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa2.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f + movx1, 5.0f, -4.0f + movz));
        model = glm::rotate(model, glm::radians(rot3), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        medusa2.Draw(shader);
        ////Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (reinicio)
    {
        if (cambio == false) {
            rot -= 0.05f;
            if (rot <= -80.0f) {
                cambio = true;
            }
        }
        if (cambio == true) {
            rot += 0.05f;
            if (rot >= 0.0f) {
                cambio = false;
            }
        }
    }

    if (anim)
    {
        if (recorrido1)
        {
            if (movx > 25.0f)
            {
                recorrido1 = false;
                recorrido2 = true;
                rot2 = 30.0f;
            }
            else
            {
                movx += 0.03f;
            }
        }
        if (recorrido2)
        {
            if (movx < 5.0f)
            {
                recorrido2 = false;
                recorrido1 = true;
                rot2 = 0.0f;

            }
            else
            {
                movx -= 0.03f;
            }
        }
    }

    if (anim)
    {
        if (recorrido3)
        {
            if (movx1 < -20.0f)
            {
                recorrido3 = false;
                recorrido4 = true;
                rot3 = -50.0f;
            }
            else
            {
                movx1 -= 0.03f;
            }
        }
        if (recorrido4)
        {
            if (movx1 > 15.0f)
            {
                recorrido4 = false;
                recorrido3 = true;
                rot3 = 30.0f;

            }
            else
            {
                movx1 += 0.03f;
            }
        }
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_Z]) {
        movimiento += 1;
        if (movimiento % 2 == 0)
        {
            reinicio = false;
        }
        else
        {
            reinicio = true;
        }

    }

    if (keys[GLFW_KEY_X])
    {
        active = !active;
        anim = !anim;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
