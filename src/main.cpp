#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "tiempo.h"
#include "render.h"
#include "input.h"
#include "acuario.h"
#include "pez.h"
#include "pezJugador.h"

// Estado de entrada (teclado, cámara, etc.)
EstadoEntrada estadoEntrada;

// DeltaTime para movimiento independiente de FPS
float deltaTime = 0.0f;

// Tamaño de ventana
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// Recursos gráficos (shaders, texturas...)
RecursosRender recursos;

// Escena principal
Acuario acuario;

// Peces normales
const int NUM_PECES = 20;
Pez peces[NUM_PECES];

// Pez controlado por el jugador
PezJugador pezJugador;

int main() {
    GLFWwindow* window = inicializarVentana(SCR_WIDTH, SCR_HEIGHT, "Acuario");
    if (window == nullptr) {
        return -1;
    }

    // Inicialización OpenGL y escena
    openGlInit();
    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);
    cargarRecursos(recursos);

    inicializarEstadoEntrada(estadoEntrada);
    inicializarAcuario(acuario);

    // Inicialización de entidades
    inicializarPeces(peces, NUM_PECES, acuario);
    inicializarPezJugador(pezJugador, acuario);

    // Carga de texturas y asignación a objetos
    cargarTexturasEscena(recursos,acuario,peces,NUM_PECES,pezJugador);

    float ultimoFrame = glfwGetTime();

    // Bucle principal
    while (!glfwWindowShouldClose(window)) {
        deltaTime = lapsoDeltaTime(ultimoFrame);

        // Input y actualización lógica
        processInput(window, estadoEntrada, pezJugador, acuario, deltaTime);
        actualizarPeces(peces, NUM_PECES, acuario, deltaTime);
        actualizarPezJugador(pezJugador, deltaTime);

        // Render de la escena
        renderizarFrame(acuario, peces, NUM_PECES, pezJugador, estadoEntrada, recursos);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Liberación de recursos
    liberar(recursos);

    return 0;
}