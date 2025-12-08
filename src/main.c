#include <GLFW/glfw3.h>
#include <stdio.h>

int main() {
    if (glfwInit() != GLFW_TRUE) {
        printf("Failed to initialize glfw!");
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "everest", NULL, NULL);
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}