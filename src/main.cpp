#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "tiempo.h"
#include "render.h"
#include "input.h"
#include "acuario.h"
#include "pez.h"

EstadoEntrada estadoEntrada;
float deltaTime = 0.0f;

const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

RecursosRender recursos;
Acuario acuario;
const int NUM_PECES = 3;
Pez peces[NUM_PECES];

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
    inicializarPeces(peces, NUM_PECES, acuario);

    float ultimoFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        deltaTime = lapsoDeltaTime(ultimoFrame);

        processInput(window, estadoEntrada, deltaTime);
        actualizarPeces(peces, NUM_PECES, acuario, deltaTime);
        renderizarFrame(acuario, peces, NUM_PECES, estadoEntrada, recursos);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    liberar(recursos);

    return 0;
}