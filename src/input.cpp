#include "input.h"
#include <cmath>

// comprueba si una tecla está pulsada
static bool teclaPresionada(GLFWwindow* window, int tecla) {
    return glfwGetKey(window, tecla) == GLFW_PRESS;
}

// comprueba si una tecla está soltada
static bool teclaSoltada(GLFWwindow* window, int tecla) {
    return glfwGetKey(window, tecla) == GLFW_RELEASE;
}

// evita que un valor se salga de unos límites
static void limitarValor(float& valor, float minimo, float maximo) {
    if (valor < minimo)
        valor = minimo;

    if (valor > maximo)
        valor = maximo;
}

// cambia el estado de una luz solo una vez por pulsación
static void procesarToggleLuz(GLFWwindow* window, int tecla, bool& luzEncendida, bool& teclaPulsada) {
    if (teclaPresionada(window, tecla) && !teclaPulsada) {
        luzEncendida = !luzEncendida;
        teclaPulsada = true;
    }
    if (teclaSoltada(window, tecla)) {
        teclaPulsada = false;
    }
}

// cierra la ventana al pulsar escape
static void procesarSalida(GLFWwindow* window) {
    if (teclaPresionada(window, GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, true);
}

/*// controla el avance, retroceso y giro de la grúa
static void procesarMovimiento(GLFWwindow* window, float deltaTime) {
    const float aceleracion = 10.0f;
    const float velocidadGiro = 80.0f;

    if (teclaPresionada(window, GLFW_KEY_W))
        grua.velocidad += aceleracion * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_X))
        grua.velocidad -= aceleracion * deltaTime;

    // solo gira si la grúa se está moviendo un poco
    if (teclaPresionada(window, GLFW_KEY_A) && std::abs(grua.velocidad) > 0.05f)
        grua.direccion += velocidadGiro * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_D) && std::abs(grua.velocidad) > 0.05f)
        grua.direccion -= velocidadGiro * deltaTime;
}*/

// cambia entre los distintos modos de cámara
static void procesarCamaras(GLFWwindow* window, EstadoEntrada& estado) {
    if (teclaPresionada(window, GLFW_KEY_1))
        estado.modoCamara = 1;

    if (teclaPresionada(window, GLFW_KEY_2))
        estado.modoCamara = 2;

    if (teclaPresionada(window, GLFW_KEY_3))
        estado.modoCamara = 3;
}
/*
// procesa las teclas que encienden y apagan las luces
static void procesarLuces(GLFWwindow* window, EstadoEntrada& estado) {
    procesarToggleLuz(window, GLFW_KEY_I, estado.luzEncendidaFoco, estado.teclaIPulsada);
    procesarToggleLuz(window, GLFW_KEY_T, estado.luzEncendidaBrazo, estado.teclaTPulsada);
}*/

// valores iniciales del input
void inicializarEstadoEntrada(EstadoEntrada& estado) {
    estado.modoCamara = 2;
}

// función principal que agrupa todo el control por teclado
void processInput(GLFWwindow* window, EstadoEntrada& estado, float deltaTime)
{
    procesarSalida(window);
    //procesarMovimiento(window, deltaTime);
    procesarCamaras(window, estado);
    //procesarLuces(window, estado);
}