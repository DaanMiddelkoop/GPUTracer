#include "World.h"
#include <String.h>
#include <stdlib.h>

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

    std::cout << "initing computeprogram" << std::endl;

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

    int maxLogLength = logLength;
    while (logLength >= 1) {
        logLength -= 1;
        std::cout << shaderError[maxLogLength - logLength - 1];
    }
    std::cout << std::endl;
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
    std::vector<GLchar> log((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(quadProgram, logLength, &actualLength, &log[0]);


    int maxLogLength = logLength;
    while (logLength >= 1) {
        logLength -= 1;
        std::cout << log[maxLogLength - logLength - 1];
    }
    std::cout << std::endl;
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

    std::cout << "reaching this? " << std::endl;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    GLint logLength;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    GLint actualLength;
    std::vector<GLchar> log((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, &actualLength, &log[0]);
    std::cout << "i am confused" << std::endl;

    int maxLogLength = logLength;
    while (logLength >= 1) {
        logLength -= 1;
        std::cout << log[maxLogLength - logLength - 1];
    }
    std::cout << std::endl;
    if (linked == 0) {
        throw 20;
    }

    std::cout << "end" << std::endl;
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



void World::setTree(std::vector<Triangle*> triangles) {
    std::vector<TreeNode*> tree;

    for (int i = 0; i < triangles.size(); i++) {
        triangles[i]->index = i;
    }


    Box bounding = boundaries(&triangles);
    tree.push_back(new TreeNode(bounding, -1, -1, -1, -1, 1));
    int node_index = tree.size() - 1;

    std::cout << "reaching this? abc" << std::endl;

    std::vector<Triangle*> triangles1;
    std::vector<Triangle*> triangles2;
    split_triangles(&triangles, &triangles1, &triangles2);
    std::cout << "Starting tree, amount of triangles: " << triangles.size() << std::endl;
    std::cout << "Start tree branches, length of first: " << triangles1.size() << ", second: " << triangles2.size() << std::endl;

    buildTree(&triangles1, &tree, node_index, false);
    std::cout << "Finished building half the tree." << std::endl;
    buildTree(&triangles2, &tree, node_index, true);
    std::cout << "Finished building tree." << std::endl;

    int max_depth = -1;

    void* tree_data = malloc(sizeof(float) * tree.size() * 12);
    for (int i = 0; i < tree.size(); i++) {
        //std::cout << "treenode min " << i << ": x " << tree[i]->bounding.minimal.x << ", y " << tree[i]->bounding.minimal.y << ", z " << tree[i]->bounding.minimal.z << std::endl;
        //std::cout << "treenode max " << i << ": x " << tree[i]->bounding.maximal.x << ", y " << tree[i]->bounding.maximal.y << ", z " << tree[i]->bounding.maximal.z << std::endl;
        //std::cout << tree[i].child1 << ", " << tree[i].child2 << std::endl;
        //std::cout << tree[i].t1 << ", " << tree[i].t2 << std::endl;
        //std::cout << "depth of " << i << ": " << tree[i]->depth << std::endl;
        if (max_depth < tree[i]->depth) {
            max_depth = tree[i]->depth;
        }

        ((float*)tree_data)[(i * 12) + 0] = tree[i]->bounding.minimal.x;
        ((float*)tree_data)[(i * 12) + 1] = tree[i]->bounding.minimal.y;
        ((float*)tree_data)[(i * 12) + 2] = tree[i]->bounding.minimal.z;

        ((float*)tree_data)[(i * 12) + 4] = tree[i]->bounding.maximal.x;
        ((float*)tree_data)[(i * 12) + 5] = tree[i]->bounding.maximal.y;
        ((float*)tree_data)[(i * 12) + 6] = tree[i]->bounding.maximal.z;

        ((int*)tree_data)[(i * 12) + 8] = tree[i]->child1;
        ((int*)tree_data)[(i * 12) + 9] = tree[i]->child2;
        ((int*)tree_data)[(i * 12) + 10] = tree[i]->t1;
        ((int*)tree_data)[(i * 12) + 11] = tree[i]->t2;
    }

    std::cout << "MAX DEPTH: " << max_depth << ", " << tree.size() << std::endl;

    GLuint ssbo = 0;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * tree.size() * 12, tree_data, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    memcpy(p, tree_data, sizeof(float) * tree.size() * 12);
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    GLuint block_index = 0;
    block_index = glGetProgramResourceIndex(computeProgram, GL_SHADER_STORAGE_BLOCK, "tree_b");

    GLuint ssbo_binding_point_index = 1;
    glShaderStorageBlockBinding(computeProgram, block_index, ssbo_binding_point_index);

    glShaderStorageBlockBinding(computeProgram, block_index, 10);
    GLuint binding_point_index = 10;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point_index, ssbo);





    ssbo = 0;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Triangle) * triangles.size(), NULL, GL_DYNAMIC_COPY);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);

    for (int i = 0; i < triangles.size(); i++) {
        memcpy(p + (sizeof(Triangle) * i), triangles[i], sizeof(Triangle));
    }

    //memcpy(p, triangle_data, sizeof(Triangle) * triangles.size());
    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    block_index = 0;
    block_index = glGetProgramResourceIndex(computeProgram, GL_SHADER_STORAGE_BLOCK, "triangles_b");

    ssbo_binding_point_index = 2;
    glShaderStorageBlockBinding(computeProgram, block_index, ssbo_binding_point_index);


    glShaderStorageBlockBinding(computeProgram, block_index, 11);
    binding_point_index = 11;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding_point_index, ssbo);

}

void World::buildTree(std::vector<Triangle*>* triangles, std::vector<TreeNode*>* tree, int parent, bool childn) {
    Box bounding = boundaries(triangles);

    if (triangles->size() <= 2) {
        if (triangles->size() == 2) {
            tree->push_back(new TreeNode(bounding, -1, -1, (*triangles)[0]->index, (*triangles)[1]->index, (*tree)[parent]->depth + 1));
        } else {
            tree->push_back(new TreeNode(bounding, -1, -1, (*triangles)[0]->index, -1, (*tree)[parent]->depth + 1));
        }

        if (childn == false) {
            (*tree)[parent]->child1 = tree->size() - 1;
        } else {
            (*tree)[parent]->child2 = tree->size() - 1;
        }
        return;
    }


    tree->push_back(new TreeNode(bounding, -1, -1, -1, -1, (*tree)[parent]->depth + 1));
    int node_index = tree->size() - 1;

    if (childn == false) {
        (*tree)[parent]->child1 = node_index;
    } else {
        (*tree)[parent]->child2 = node_index;
    }

    std::vector<Triangle*> triangles1;
    std::vector<Triangle*> triangles2;
    split_triangles(triangles, &triangles1, &triangles2);
    buildTree(&triangles1, tree, node_index, false);
    buildTree(&triangles2, tree, node_index, true);
}

Box World::boundaries(std::vector<Triangle*>* triangles) {
    Box box = Box();

    if (triangles->size() <= 0) {
        Vector3f minimal = Vector3f(0.0, 0.0, 0.0);
        Vector3f maximal = Vector3f(0.0, 0.0, 0.0);
        box.minimal = minimal;
        box.maximal = maximal;
        return box;
    }

    Vector3f minimal((*triangles)[0]->a.x, (*triangles)[0]->a.y, (*triangles)[0]->a.z);
    Vector3f maximal((*triangles)[0]->a.x, (*triangles)[0]->a.y, (*triangles)[0]->a.z);

    for (int i = 0; i < triangles->size(); i++) {
        Triangle* triangle = (*triangles)[i];
        if (triangle->a.x < minimal.x) {
            minimal.x = triangle->a.x;
        }
        if (triangle->a.y < minimal.y) {
            minimal.y = triangle->a.y;
        }
        if (triangle->a.z < minimal.z) {
            minimal.z = triangle->a.z;
        }

        if (triangle->b.x < minimal.x) {
            minimal.x = triangle->b.x;
        }
        if (triangle->b.y < minimal.y) {
            minimal.y = triangle->b.y;
        }
        if (triangle->b.z < minimal.z) {
            minimal.z = triangle->b.z;
        }

        if (triangle->c.x < minimal.x) {
            minimal.x = triangle->c.x;
        }
        if (triangle->c.y < minimal.y) {
            minimal.y = triangle->c.y;
        }
        if (triangle->c.z < minimal.z) {
            minimal.z = triangle->c.z;
        }

        if (triangle->a.x > maximal.x) {
            maximal.x = triangle->a.x;
        }
        if (triangle->a.y > maximal.y) {
            maximal.y = triangle->a.y;
        }
        if (triangle->a.z > maximal.z) {
            maximal.z = triangle->a.z;
        }

        if (triangle->b.x > maximal.x) {
            maximal.x = triangle->b.x;
        }
        if (triangle->b.y > maximal.y) {
            maximal.y = triangle->b.y;
        }
        if (triangle->b.z > maximal.z) {
            maximal.z = triangle->b.z;
        }

        if (triangle->c.x > maximal.x) {
            maximal.x = triangle->c.x;
        }
        if (triangle->c.y > maximal.y) {
            maximal.y = triangle->c.y;
        }
        if (triangle->c.z > maximal.z) {
            maximal.z = triangle->c.z;
        }
    }
    box.minimal = minimal;
    box.maximal = maximal;
    return box;
}

void World::split_triangles(std::vector<Triangle*>* source, std::vector<Triangle*>* part1, std::vector<Triangle*>* part2) {
    float av_x = 0.0f;
    float av_y = 0.0f;
    float av_z = 0.0f;

    Box bounding = boundaries(source);

    std::vector<float> xs;
    std::vector<float> ys;
    std::vector<float> zs;

    for (int i = 0; i < source->size(); i++) {
        Triangle* t = (*source)[i];
        float x = t->a.x + t->b.x + t->c.x;
        float y = t->a.y + t->b.y + t->c.y;
        float z = t->a.z + t->b.z + t->c.z;

        xs.push_back(x);
        ys.push_back(y);
        zs.push_back(z);

        av_x += x;
        av_y += y;
        av_z += z;
    }

    av_x /= source->size();
    av_y /= source->size();
    av_z /= source->size();

    float stdx = 0.0f;
    float stdy = 0.0f;
    float stdz = 0.0f;



    for (int i = 0; i < source->size(); i++) {
        stdx += sqrt((xs[i] - av_x) * (xs[i] - av_x));
        stdy += sqrt((ys[i] - av_y) * (ys[i] - av_y));
        stdz += sqrt((zs[i] - av_z) * (zs[i] - av_z));
    }

    bool switching = true;

    if (stdx >= stdy && stdx >= stdz) {

        for (int i = 0; i < source->size(); i++) {
            if (xs[i] > av_x) {
                part1->push_back((*source)[i]);
            } else if (xs[i] == av_x) {
                if (switching) {
                    part2->push_back((*source)[i]);
                } else {
                    part1->push_back((*source)[i]);
                }
                switching = !switching;

            } else {
                part2->push_back((*source)[i]);
            }
        }
        return;
    }
    if (stdy >= stdx && stdy >= stdz) {
        for (int i = 0; i < source->size(); i++) {
             if (ys[i] > av_y) {
                part1->push_back((*source)[i]);
            } else if (ys[i] == av_y) {
                if (switching) {
                    part2->push_back((*source)[i]);
                } else {
                    part1->push_back((*source)[i]);
                }

                switching = !switching;
            } else {
                part2->push_back((*source)[i]);
            }
        }
        return;
    }
    if (stdz >= stdz && stdz >= stdx) {
        for (int i = 0; i < source->size(); i++) {
             if (zs[i] > av_z) {
                part1->push_back((*source)[i]);
            } else if (zs[i] == av_z) {
                if (switching) {
                    part2->push_back((*source)[i]);
                } else {
                    part1->push_back((*source)[i]);
                }
                switching = !switching;
            } else {
                part2->push_back((*source)[i]);
            }
        }
        return;
    }
}

void World::trace() {
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
