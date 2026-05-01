#include "pezJugador.h"

#include <cmath>
#include <glm/glm.hpp>

static void limitarValor(float& valor, float minimo, float maximo) {
    if (valor < minimo) {
        valor = minimo;
    }

    if (valor > maximo) {
        valor = maximo;
    }
}

static void limitarPezJugadorAlAcuario(PezJugador& pezJugador, const Acuario& acuario) {
    glm::vec3 limiteMin = obtenerLimiteMinAcuario(acuario);
    glm::vec3 limiteMax = obtenerLimiteMaxAcuario(acuario);
    float margen = pezJugador.pez.escala * 0.75f;

    if (pezJugador.pez.posicion.x < limiteMin.x + margen) {
        pezJugador.pez.posicion.x = limiteMin.x + margen;
    }

    if (pezJugador.pez.posicion.x > limiteMax.x - margen) {
        pezJugador.pez.posicion.x = limiteMax.x - margen;
    }

    if (pezJugador.pez.posicion.y < limiteMin.y + margen) {
        pezJugador.pez.posicion.y = limiteMin.y + margen;
    }

    if (pezJugador.pez.posicion.y > limiteMax.y - margen) {
        pezJugador.pez.posicion.y = limiteMax.y - margen;
    }

    if (pezJugador.pez.posicion.z < limiteMin.z + margen) {
        pezJugador.pez.posicion.z = limiteMin.z + margen;
    }

    if (pezJugador.pez.posicion.z > limiteMax.z - margen) {
        pezJugador.pez.posicion.z = limiteMax.z - margen;
    }
}

void actualizarDireccionPezJugador(PezJugador& pezJugador) {
    glm::vec3 direccion;

    direccion.x = sin(glm::radians(pezJugador.yaw)) * cos(glm::radians(pezJugador.pitch));
    direccion.y = sin(glm::radians(pezJugador.pitch));
    direccion.z = cos(glm::radians(pezJugador.yaw)) * cos(glm::radians(pezJugador.pitch));

    pezJugador.pez.direccion = glm::normalize(direccion);
}

void inicializarPezJugador(PezJugador& pezJugador, const Acuario& acuario) {
    pezJugador.yaw = 0.0f;
    pezJugador.pitch = 0.0f;
    pezJugador.velocidad = 2.0f;
    pezJugador.velocidadGiro = 150.0f;

    glm::vec3 posicionInicial = acuario.centro + glm::vec3(0.0f, 0.6f, 0.0f);
    glm::vec3 colorJugador = glm::vec3(0.75f, 0.20f, 1.0f);

    inicializarPez(pezJugador.pez, posicionInicial, glm::vec3(0.0f, 0.0f, 1.0f), colorJugador, 0.0f, 0.85f);

    pezJugador.pez.cabeza.color = glm::vec3(0.55f, 0.12f, 0.95f);
    pezJugador.pez.cola.color = glm::vec3(1.0f, 0.45f, 0.95f);
    pezJugador.pez.aletaIzquierda.color = glm::vec3(1.0f, 0.45f, 0.95f);
    pezJugador.pez.aletaDerecha.color = glm::vec3(1.0f, 0.45f, 0.95f);

    actualizarDireccionPezJugador(pezJugador);
}

void girarPezJugador(PezJugador& pezJugador, float giroYaw, float giroPitch, float deltaTime) {
    pezJugador.yaw += giroYaw * pezJugador.velocidadGiro * deltaTime;
    actualizarDireccionPezJugador(pezJugador);
}

void moverPezJugador(PezJugador& pezJugador, const Acuario& acuario, float avance, float subida, float deltaTime) {
    float pitchObjetivo = 0.0f;
    float velocidadSubida = pezJugador.velocidad * 0.55f;
    float velocidadRecuperacionPitch = 3.5f;

    if (subida > 0.0f) {
        pitchObjetivo = 28.0f;
    }

    if (subida < 0.0f) {
        pitchObjetivo = -28.0f;
    }

    pezJugador.pitch = pezJugador.pitch + (pitchObjetivo - pezJugador.pitch) * velocidadRecuperacionPitch * deltaTime;

    limitarValor(pezJugador.pitch, -35.0f, 35.0f);
    actualizarDireccionPezJugador(pezJugador);

    pezJugador.pez.posicion += pezJugador.pez.direccion * avance * pezJugador.velocidad * deltaTime;
    pezJugador.pez.posicion.y += subida * velocidadSubida * deltaTime;

    limitarPezJugadorAlAcuario(pezJugador, acuario);
}

void actualizarPezJugador(PezJugador& pezJugador, float deltaTime) {
    pezJugador.pez.faseMovimiento += deltaTime * 8.0f;
}

void dibujarPezJugador(const PezJugador& pezJugador, GLuint shaderProgram) {
    dibujarPez(pezJugador.pez, shaderProgram);
}