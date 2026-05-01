#include "input.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

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

// calcula el vector frente de la cámara libre a partir de yaw y pitch
static glm::vec3 calcularFrenteCamaraLibre(const EstadoEntrada& estado) {
    glm::vec3 frente;

    frente.x = cos(glm::radians(estado.yawCamaraLibre)) * cos(glm::radians(estado.pitchCamaraLibre));
    frente.y = sin(glm::radians(estado.pitchCamaraLibre));
    frente.z = sin(glm::radians(estado.yawCamaraLibre)) * cos(glm::radians(estado.pitchCamaraLibre));

    return glm::normalize(frente);
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

// cambia entre los distintos modos de cámara
static void procesarCamaras(GLFWwindow* window, EstadoEntrada& estado) {
    if (teclaPresionada(window, GLFW_KEY_1))
        estado.modoCamara = 1;

    if (teclaPresionada(window, GLFW_KEY_2))
        estado.modoCamara = 2;

    if (teclaPresionada(window, GLFW_KEY_3))
        estado.modoCamara = 3;
}

// mueve únicamente la cámara libre, es decir, solo afecta al modoCamara == 1
static void procesarMovimientoCamaraLibre(GLFWwindow* window, EstadoEntrada& estado, float deltaTime) {
    if (estado.modoCamara != 1)
        return;

    const float velocidadMovimiento = 5.0f;
    const float velocidadGiro = 80.0f;

    glm::vec3 frente = calcularFrenteCamaraLibre(estado);
    glm::vec3 derecha = glm::normalize(glm::cross(frente, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (teclaPresionada(window, GLFW_KEY_W))
        estado.posicionCamaraLibre += frente * velocidadMovimiento * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_S))
        estado.posicionCamaraLibre -= frente * velocidadMovimiento * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_A))
        estado.posicionCamaraLibre -= derecha * velocidadMovimiento * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_D))
        estado.posicionCamaraLibre += derecha * velocidadMovimiento * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_Q))
        estado.posicionCamaraLibre.y += velocidadMovimiento * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_E))
        estado.posicionCamaraLibre.y -= velocidadMovimiento * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_LEFT))
        estado.yawCamaraLibre -= velocidadGiro * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_RIGHT))
        estado.yawCamaraLibre += velocidadGiro * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_UP))
        estado.pitchCamaraLibre += velocidadGiro * deltaTime;

    if (teclaPresionada(window, GLFW_KEY_DOWN))
        estado.pitchCamaraLibre -= velocidadGiro * deltaTime;

    limitarValor(estado.pitchCamaraLibre, -89.0f, 89.0f);
}

// valores iniciales del input
void inicializarEstadoEntrada(EstadoEntrada& estado) {
    estado.modoCamara = 2;

    estado.posicionCamaraLibre = glm::vec3(0.0f, 3.0f, 12.0f);
    estado.yawCamaraLibre = -90.0f;
    estado.pitchCamaraLibre = -8.0f;

    estado.luzEncendidaFoco = true;
    estado.teclaIPulsada = false;
    estado.luzEncendidaBrazo = true;
    estado.teclaTPulsada = false;
}

// función principal que agrupa todo el control por teclado
void processInput(GLFWwindow* window, EstadoEntrada& estado, float deltaTime) {
    procesarSalida(window);
    procesarCamaras(window, estado);
    procesarMovimientoCamaraLibre(window, estado, deltaTime);
}