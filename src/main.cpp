#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "tiempo.h"
#include "render.h"
#include "input.h"

// Estado global de la escena
EstadoEntrada estadoEntrada; //el estado de entrada lo utilizamos para comprobar que teclas hay pulsadas, estado de las luces, camaras...
//se almacena todo en una estructura de datos
float deltaTime = 0.0f;

// Configuración ventana
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

// shaders
//hay una estructura de datos en render que se encarga de manejar la creacion e inicializacion de los shaders y las texturas
RecursosRender recursos;

int main() {
    GLFWwindow* window = inicializarVentana(SCR_WIDTH, SCR_HEIGHT, "Grúa");
    if (window == nullptr)
        return -1;

    openGlInit();
    framebuffer_size_callback(window, SCR_WIDTH, SCR_HEIGHT);

    cargarRecursos(recursos); //aqui es donde cargamos todos los shaders y texturas
    inicializarEstadoEntrada(estadoEntrada); //inicializamos posiciones de la camara, estados iniciales de los focos...

    float ultimoFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        deltaTime = lapsoDeltaTime(ultimoFrame);

        processInput(window, estadoEntrada, deltaTime); //leemos teclas pulsadas
        renderizarFrame(estadoEntrada, recursos); //renderizamos por completo el frame

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    liberar(recursos);
    return 0;
}
