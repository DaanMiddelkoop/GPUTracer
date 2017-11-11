#ifndef WORLD_H
#define WORLD_H

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <Camera.h>
#include <View.h>


class World
{
    public:
        World(GLFWwindow* window);

        void init();
        void trace();
        int width = 1024;
        int height = 768;

        View view;

    protected:

    private:
        int quadFullScreenVao();
        std::string readFile(const char* filePath);
        int createShader(char* resource, int type);
        int createQuadProgram();
        int createComputeProgram();
        void initQuadProgram();
        void initComputeProgram();
        int createFramebufferTexture();
        int nextPowerOfTwo(int x);


        GLint workGroupSizeX;
        GLint workGroupSizeY;
        GLuint computeProgram;
        GLuint quadProgram;
        GLint eyeUniform;
        GLint ray00Uniform;
        GLint ray10Uniform;
        GLint ray01Uniform;
        GLint ray11Uniform;
        GLint tex;
        GLint vao;

        GLFWwindow* window;


};

#endif // WORLD_H
