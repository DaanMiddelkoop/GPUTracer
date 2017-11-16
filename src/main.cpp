#include <windows.h>
#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <World.h>
#include <math.h>
#include <time.h>


float horizontal_direction = 0;
float vertical_direction = -1.5;

Vector3f position(0.0f, -60.0f, 0.0f);
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

    if (keys[GLFW_KEY_M]) {
        position.y += 0.1;
    }
    if (keys[GLFW_KEY_N]) {
        position.y -= 0.1;
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


    std::vector<Triangle*> triangles = std::vector<Triangle*>(100 * 100 * 2);
    std::cout << 100 * 100 * 2 + 1 << std::endl;
    std::cout << triangles.max_size() << std::endl;
    for (int ax = 0; ax < 100; ax++) {
        for (int ay = 0; ay < 100; ay++) {
            Triangle* tr = new Triangle();

            float x = ax - 50;
            float y = ay - 50;

            float tx;
            float ty;
            float tz;

            float r2 = 75.0 * 75.0;

            tx = x - 1.0;
            ty = -abs(sqrt(r2 - (pow((float)x - 1.0, 2)) - pow((float)y - 1.0, 2)));
            tz = y - 1.0;
            tr->a = Vector3f(tx, ty, tz);

            tx = (float)x - 1.0;
            ty = -abs(sqrt(r2 - (pow((float)x - 1.0, 2)) - pow((float)y, 2)));
            tz = (float)y;
            tr->b = Vector3f(tx, ty, tz);

            tx = (float)x;
            ty = -abs(sqrt(r2 - (pow((float)x, 2)) - pow((float)y, 2)));
            tz = (float)y;
            tr->c = Vector3f(tx, ty, tz);
            triangles[((ax) * 100 + (ay) ) * 2 + 0] = tr;


            tr = new Triangle();
            tx = x - 1.0;
            ty = -abs(sqrt(r2 - (pow((float)x - 1.0, 2)) - pow((float)y - 1.0, 2)));
            tz = y - 1.0;
            tr->a = Vector3f(tx, ty, tz);

            tx = (float)x;
            ty = -abs(sqrt(r2 - (pow((float)x, 2)) - pow((float)y - 1.0, 2)));
            tz = (float)y - 1.0;
            tr->b = Vector3f(tx, ty, tz);

            tx = (float)x;
            ty = -abs(sqrt(r2 - (pow((float)x, 2)) - pow((float)y, 2)));
            tz = (float)y;
            tr->c = Vector3f(tx, ty, tz);
            triangles[((ax) * 100 + (ay) ) * 2 + 1] = tr;
        }
    }

    world.setTree(triangles);
    int frame_count = 0;
    double timer;


    while (!glfwWindowShouldClose(window)) {
        frame_count += 1;
        if (time(NULL) > timer + 1) {
            timer = time(NULL);
            std::cout << frame_count << std::endl;
            frame_count = 0;
        }


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

