#include "tiempo.h"
#include <GLFW/glfw3.h>

float lapsoDeltaTime(float& ultimoFrame) {
    // Calculamos el tiempo entre frames para que el movimiento sea constante
    float tiempoActual = glfwGetTime();
    float deltaTime = tiempoActual - ultimoFrame;

    ultimoFrame = tiempoActual;

    return deltaTime;
}