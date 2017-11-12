#include <windows.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <World.h>
#include <math.h>


float horizontal_direction = 0;
float vertical_direction = -1.5;

Vector3f position(-8.0f, 10.0f, -6.0f);
bool keys[200] = {false};


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
    }

    if (action == GLFW_RELEASE) {
        keys[key] = false;
    }


}

void handleKeys() {
    if (keys[GLFW_KEY_W]) {

        vertical_direction += 0.1;
    }
    if (keys[GLFW_KEY_A]) {
        horizontal_direction -= 0.1;
    }
    if (keys[GLFW_KEY_S]) {
        vertical_direction -= 0.1;
    }
    if (keys[GLFW_KEY_D]) {
        horizontal_direction += 0.1;
    }

    if (keys[GLFW_KEY_I]) {
        position.x += cos(horizontal_direction) * 0.1;
        position.z += sin(horizontal_direction) * 0.1;
    }
    if (keys[GLFW_KEY_K]) {
        position.x -= cos(horizontal_direction) * 0.1;
        position.z -= sin(horizontal_direction) * 0.1;
    }
    if (keys[GLFW_KEY_J]) {
        position.x += sin(horizontal_direction) * 0.1;
        position.z -= cos(horizontal_direction) * 0.1;
    }
    if (keys[GLFW_KEY_L]) {
        position.x -= sin(horizontal_direction) * 0.1;
        position.z += cos(horizontal_direction) * 0.1;
    }
}

int main() {
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        fprintf(stderr, "ERROR: could not start GLFW3\n");
        return 1;
    }

    // uncomment these lines if on Apple OS X
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Hello Triangle", NULL, NULL);
    if (!window) {
        fprintf(stderr, "ERROR: could not open window with GLFW3\n");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental=GL_TRUE;
    GLenum err = glewInit();
    std::cout << glewGetErrorString(err) << std::endl;
    std::cout << "glew version: " << glewGetString(GLEW_VERSION) << std::endl;
    if (glewIsSupported("GL_VERSION_4_3")) {
        std::cout << "opengl 4.3 is supported!" << std::endl;
    } else{
        std::cout << "opengl 4.3 is not supported, exiting..." << std::endl;
    return 0;
    }

    World world = World(window);
    world.init();

    glfwSetKeyCallback(window, keyCallBack);

    std::vector<Triangle> triangles = std::vector<Triangle>();

    /*for (int i = 0; i < 50; i+= 2) {
        triangles.push_back(Triangle(Vector3f(-5.0, -1.0 * ((float)i), -5.0), Vector3f( 5.0, -1.0 * ((float)i), -5.0), Vector3f(5.0, -1.0 * ((float)i), 5.0)));
        triangles.push_back(Triangle(Vector3f(-5.0, -1.0 * ((float)i), -5.0), Vector3f(-5.0, -1.0 * ((float)i),  5.0), Vector3f(5.0, -1.0 * ((float)i), 5.0)));
    }*/

    triangles.push_back(Triangle(Vector3f( -30.0, 0.0, -30.0), Vector3f(-30.0, 0.0,  30.0), Vector3f(30.0, 0.0, 30.0)));
    triangles.push_back(Triangle(Vector3f( -30.0, 0.0, -30.0), Vector3f(30.0, 0.0,  -30.0), Vector3f(30.0, 0.0, 30.0)));

    triangles.push_back(Triangle(Vector3f( -1.0, 5.0, -1.0), Vector3f(-1.0, 5.0,  1.0), Vector3f(1.0, 5.0, 1.0)));

    world.setTree(triangles);

    while (!glfwWindowShouldClose(window)) {
        handleKeys();
        glfwPollEvents();
        glViewport(0, 0, world.width, world.height);
        world.trace();
        glfwSwapBuffers(window);

        world.view.setHorizontalDirection(horizontal_direction);
        world.view.setVerticalDirection(vertical_direction);
        world.view.setPosition(position);
        world.view.process();

    }
    // close GL context and any other GLFW resources
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glCompileShader(shader);
    glfwTerminate();
    return 0;
}

