#include "World.h"

World::World(GLFWwindow* window)
{
    this->window = window;
}

void World::init() {
    if (glfwInit() != GL_TRUE)
        throw 23;

    const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowPos(window, (vidmode->width - width) / 2, (vidmode->height - height) / 2);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwShowWindow(window);
    //TODO: GLContext.createFromCurrent();

    /* Create all needed GL resources */
    tex = createFramebufferTexture();

    vao = quadFullScreenVao();

    computeProgram = createComputeProgram();
    initComputeProgram();

    quadProgram = createQuadProgram();
    initQuadProgram();
    /* Setup camera
    Camera camera = Camera();
    camera.setFrustumPerspective(60.0f, (float) width / height, 1f, 2f);
    camera.setLookAt(new Vector3f(3.0f, 2.0f, 7.0f), new Vector3f(0.0f, 0.5f, 0.0f), new Vector3f(0.0f, 1.0f, 0.0f));
    */
    view.setHorizontalDirection(0);
    view.setVerticalDirection(0);
    view.setPosition(Vector3f(-2.0f, 2.0f, -2.0f));
    view.process();

    std::cout << "End of init" << std::endl;
    std::cout << "direction: " << view.getDirection().x << ", " << view.getDirection().y << ", " << view.getDirection().z << std::endl;

}

int World::quadFullScreenVao() {
    GLuint* vertexarrays = new GLuint();
    glGenVertexArrays(1, vertexarrays);
    GLuint vao = vertexarrays[0];


    GLuint* buffers = new GLuint();
    glGenBuffers(1, buffers);
    GLuint vbo = buffers[0];
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    std::vector<GLchar> bb = {-1, -1, 1, -1, 1, 1, 1, 1, -1, 1, -1, -1};
    glBufferData(GL_ARRAY_BUFFER, 12, &bb[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_BYTE, false, 0, 0L);
    glBindVertexArray(0);
    return vao;
}

std::string World::readFile(const char* filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cout << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

int World::createShader(char* resource, int type) {
    GLint shader = glCreateShader(type);
    std::string sourceCodestr = readFile(resource);
    const char* sourceCodesrc= sourceCodestr.c_str();
    glShaderSource(shader, 1, &sourceCodesrc, NULL);
    glCompileShader(shader);

    GLint result = GL_FALSE;
    int logLength;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    std::vector<GLchar> shaderError((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);

    while (logLength >= 1) {
        logLength -= 1;
        std::cout << shaderError[logLength] << std::endl;
    }
    if (result == 0) {
        throw 22;
    }
    return shader;
}

int World::createQuadProgram() {
    int quadProgram = glCreateProgram();
    int vshader = createShader("quad.vs", GL_VERTEX_SHADER);
    int fshader = createShader("quad.fs", GL_FRAGMENT_SHADER);
    glAttachShader(quadProgram, vshader);
    glAttachShader(quadProgram, fshader);
    glBindAttribLocation(quadProgram, 0, "vertex");
    glBindFragDataLocation(quadProgram, 0, "color");
    glLinkProgram(quadProgram);
    GLint linked = GL_FALSE;
    glGetProgramiv(quadProgram, GL_LINK_STATUS, &linked);

    GLint logLength;
    glGetProgramiv(quadProgram, GL_INFO_LOG_LENGTH, &logLength);
    GLint actualLength;
    GLchar log;
    glGetProgramInfoLog(quadProgram, logLength, &actualLength, &log);
    if (actualLength > 0) {
        std::cout << log << std::endl;
    }
    if (linked == 0) {
        throw 21;
    }
    return quadProgram;
}

int World::createComputeProgram() {
    int program = glCreateProgram();
    int cshader = createShader("tracer.glslcs", GL_COMPUTE_SHADER);
    glAttachShader(program, cshader);
    glLinkProgram(program);
    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    GLint actualLength;
    GLchar log;
    glGetProgramInfoLog(program, logLength, &actualLength, &log);
    if (actualLength > 0) {
        std::cout << log << std::endl;
    }
    if (linked == 0) {
        throw 20;
    }
    return program;
}

void World::initQuadProgram() {
    glUseProgram(quadProgram);
    GLint texUniform = glGetUniformLocation(quadProgram, "tex");
    glUniform1i(texUniform, 0);
    glUseProgram(0);
}

void World::initComputeProgram() {
    glUseProgram(computeProgram);
    GLint workGroupSize[3];
    glGetProgramiv(computeProgram, GL_COMPUTE_WORK_GROUP_SIZE, workGroupSize);
    workGroupSizeX = workGroupSize[0];
    workGroupSizeY = workGroupSize[1];
    eyeUniform = glGetUniformLocation(computeProgram, "eye");
    ray00Uniform = glGetUniformLocation(computeProgram, "ray00");
    ray10Uniform = glGetUniformLocation(computeProgram, "ray10");
    ray01Uniform = glGetUniformLocation(computeProgram, "ray01");
    ray11Uniform = glGetUniformLocation(computeProgram, "ray11");
    glUseProgram(0);
}

int World::createFramebufferTexture()
{
    std::cout << width << ", " << height << std::endl;
    GLuint* textures = new GLuint();
    glGenTextures(1, textures);
    GLuint tex = textures[0];
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    char* black = nullptr;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, black);
    glBindTexture(GL_TEXTURE_2D, 0);
    return tex;
}

void World::trace() {
    std::cout << "test" << std::endl;
    glUseProgram(computeProgram);

    glUniform3f(eyeUniform, view.getPosition().x, view.getPosition().y, view.getPosition().z);
    Vector3f eyeRay = view.getEyeRay(-1, -1);
    glUniform3f(ray00Uniform, eyeRay.x, eyeRay.y, eyeRay.z);
    eyeRay = view.getEyeRay(-1, 1);
    glUniform3f(ray01Uniform, eyeRay.x, eyeRay.y, eyeRay.z);
    eyeRay = view.getEyeRay(1, -1);
    glUniform3f(ray10Uniform, eyeRay.x, eyeRay.y, eyeRay.z);
    eyeRay = view.getEyeRay(1, 1);
    glUniform3f(ray11Uniform, eyeRay.x, eyeRay.y, eyeRay.z);

    /* Bind level 0 of framebuffer texture as writable image in the shader. */
    glBindImageTexture(0, tex, 0, false, 0, GL_WRITE_ONLY, GL_RGBA32F);

    /* Compute appropriate invocation dimension. */
    int worksizeX = nextPowerOfTwo(width);
    int worksizeY = nextPowerOfTwo(height);

    /* Invoke the compute shader. */
    glDispatchCompute(worksizeX / workGroupSizeX, worksizeY / workGroupSizeY, 1);

    /* Reset image binding. */
    glBindImageTexture(0, 0, 0, false, 0, GL_READ_WRITE, GL_RGBA32F);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glUseProgram(0);

    /*
     * Draw the rendered image on the screen using textured full-screen
     * quad.
     */
    glUseProgram(quadProgram);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

int World::nextPowerOfTwo(int x) {
    x--;
    x |= x >> 1; // handle 2 bit numbers
    x |= x >> 2; // handle 4 bit numbers
    x |= x >> 4; // handle 8 bit numbers
    x |= x >> 8; // handle 16 bit numbers
    x |= x >> 16; // handle 32 bit numbers
    x++;
    return x;
}
