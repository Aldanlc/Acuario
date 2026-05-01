#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "tiempo.h"
#include "render.h"
#include "input.h"
#include "acuario.h"

EstadoEntrada estadoEntrada;
float deltaTime = 0.0f;

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

RecursosRender recursos;
Acuario acuario;

int main() {
    GLFWwindow* window = inicializarVentana(SCR_WIDTH, SCR_HEIGHT, "Acuario");

    if (window == nullptr) {
        return -1;
    }

    openGlInit();
    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);

    cargarRecursos(recursos);
    inicializarEstadoEntrada(estadoEntrada);
    inicializarAcuario(acuario);

    float ultimoFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        deltaTime = lapsoDeltaTime(ultimoFrame);

        processInput(window, estadoEntrada, deltaTime);
        renderizarFrame(acuario, estadoEntrada, recursos);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    liberar(recursos);

    return 0;
}