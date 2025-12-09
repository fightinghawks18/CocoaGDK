#include <stdio.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main() {
    if (!glfwInit()) {
        printf("Failed to initialize glfw!\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    GLFWwindow *window = glfwCreateWindow(800, 600, "everest", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load GL with glad!\n");
        return -1;
    }
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glViewport(0, 0, 800, 600);

        glClearColor(0.35f, 0.15f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}