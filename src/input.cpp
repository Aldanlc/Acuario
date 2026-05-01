#include "input.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

static bool teclaPresionada(GLFWwindow* window, int tecla) {
    return glfwGetKey(window, tecla) == GLFW_PRESS;
}

static bool teclaSoltada(GLFWwindow* window, int tecla) {
    return glfwGetKey(window, tecla) == GLFW_RELEASE;
}

static void limitarValor(float& valor, float minimo, float maximo) {
    if (valor < minimo) {
        valor = minimo;
    }

    if (valor > maximo) {
        valor = maximo;
    }
}

static glm::vec3 calcularFrenteCamaraLibre(const EstadoEntrada& estado) {
    glm::vec3 frente;

    frente.x = cos(glm::radians(estado.yawCamaraLibre)) * cos(glm::radians(estado.pitchCamaraLibre));
    frente.y = sin(glm::radians(estado.pitchCamaraLibre));
    frente.z = sin(glm::radians(estado.yawCamaraLibre)) * cos(glm::radians(estado.pitchCamaraLibre));

    return glm::normalize(frente);
}

static void procesarSalida(GLFWwindow* window) {
    if (teclaPresionada(window, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(window, true);
    }
}

static void procesarCamaras(GLFWwindow* window, EstadoEntrada& estado) {
    if (teclaPresionada(window, GLFW_KEY_1)) {
        estado.modoCamara = 1;
    }

    if (teclaPresionada(window, GLFW_KEY_2)) {
        estado.modoCamara = 2;
    }

    if (teclaPresionada(window, GLFW_KEY_3)) {
        estado.modoCamara = 3;
    }

    if (teclaPresionada(window, GLFW_KEY_4)) {
        estado.modoCamara = 4;
    }
}

static void procesarMovimientoCamaraLibre(GLFWwindow* window, EstadoEntrada& estado, float deltaTime) {
    if (estado.modoCamara != 1) {
        return;
    }

    const float velocidadMovimiento = 7.0f;
    const float velocidadGiro = 80.0f;

    glm::vec3 frente = calcularFrenteCamaraLibre(estado);
    glm::vec3 derecha = glm::normalize(glm::cross(frente, glm::vec3(0.0f, 1.0f, 0.0f)));

    if (teclaPresionada(window, GLFW_KEY_I)) {
        estado.posicionCamaraLibre += frente * velocidadMovimiento * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_K)) {
        estado.posicionCamaraLibre -= frente * velocidadMovimiento * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_J)) {
        estado.posicionCamaraLibre -= derecha * velocidadMovimiento * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_L)) {
        estado.posicionCamaraLibre += derecha * velocidadMovimiento * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_U)) {
        estado.posicionCamaraLibre.y += velocidadMovimiento * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_O)) {
        estado.posicionCamaraLibre.y -= velocidadMovimiento * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_LEFT)) {
        estado.yawCamaraLibre -= velocidadGiro * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_RIGHT)) {
        estado.yawCamaraLibre += velocidadGiro * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_UP)) {
        estado.pitchCamaraLibre += velocidadGiro * deltaTime;
    }

    if (teclaPresionada(window, GLFW_KEY_DOWN)) {
        estado.pitchCamaraLibre -= velocidadGiro * deltaTime;
    }

    limitarValor(estado.pitchCamaraLibre, -89.0f, 89.0f);
}

static void procesarMovimientoPezJugador(GLFWwindow* window, PezJugador& pezJugador, const Acuario& acuario, float deltaTime) {
    float acelerar = 0.0f;
    float subida = 0.0f;
    float giroYaw = 0.0f;

    if (teclaPresionada(window, GLFW_KEY_W)) {
        acelerar = 1.0f;
    }

    if (teclaPresionada(window, GLFW_KEY_A)) {
        giroYaw += 1.0f;
    }

    if (teclaPresionada(window, GLFW_KEY_D)) {
        giroYaw -= 1.0f;
    }

    if (teclaPresionada(window, GLFW_KEY_SPACE)) {
        subida += 1.0f;
    }

    if (teclaPresionada(window, GLFW_KEY_C)) {
        subida -= 1.0f;
    }

    girarPezJugador(pezJugador, giroYaw, deltaTime);
    moverPezJugador(pezJugador, acuario, acelerar, subida, deltaTime);
}

static void procesarLuces(GLFWwindow* window, EstadoEntrada& estado, Acuario& acuario) {
    if (teclaPresionada(window, GLFW_KEY_T) && !estado.teclaFocosSuperioresPulsada) {
        alternarFocosSuperiores(acuario);
        estado.teclaFocosSuperioresPulsada = true;
    }

    if (teclaSoltada(window, GLFW_KEY_T)) {
        estado.teclaFocosSuperioresPulsada = false;
    }

    if (teclaPresionada(window, GLFW_KEY_B) && !estado.teclaFocosInferioresPulsada) {
        alternarFocosInferiores(acuario);
        estado.teclaFocosInferioresPulsada = true;
    }

    if (teclaSoltada(window, GLFW_KEY_B)) {
        estado.teclaFocosInferioresPulsada = false;
    }

    if (teclaPresionada(window, GLFW_KEY_G) && !estado.teclaTodosFocosPulsada) {
        alternarTodosLosFocos(acuario);
        estado.teclaTodosFocosPulsada = true;
    }

    if (teclaSoltada(window, GLFW_KEY_G)) {
        estado.teclaTodosFocosPulsada = false;
    }
}

void inicializarEstadoEntrada(EstadoEntrada& estado) {
    estado.modoCamara = 2;

    estado.posicionCamaraLibre = glm::vec3(0.0f, 4.0f, 18.0f);
    estado.yawCamaraLibre = -90.0f;
    estado.pitchCamaraLibre = -8.0f;

    estado.teclaFocosSuperioresPulsada = false;
    estado.teclaFocosInferioresPulsada = false;
    estado.teclaTodosFocosPulsada = false;
}

void processInput(GLFWwindow* window, EstadoEntrada& estado, PezJugador& pezJugador, Acuario& acuario, float deltaTime) {
    procesarSalida(window);
    procesarCamaras(window, estado);
    procesarMovimientoCamaraLibre(window, estado, deltaTime);
    procesarMovimientoPezJugador(window, pezJugador, acuario, deltaTime);
    procesarLuces(window, estado, acuario);
}