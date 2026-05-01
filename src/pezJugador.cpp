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

static void aplicarRozamiento(float& velocidad, float rozamiento, float deltaTime) {
    velocidad -= velocidad * rozamiento * deltaTime;

    if (velocidad < 0.02f && velocidad > -0.02f) {
        velocidad = 0.0f;
    }
}

static void limitarPezJugadorAlAcuario(PezJugador& pezJugador, const Acuario& acuario) {
    glm::vec3 limiteMin = obtenerLimiteMinAcuario(acuario);
    glm::vec3 limiteMax = obtenerLimiteMaxAcuario(acuario);
    float margen = pezJugador.pez.escala * 0.75f;
    bool choqueHorizontal = false;

    if (pezJugador.pez.posicion.x < limiteMin.x + margen) {
        pezJugador.pez.posicion.x = limiteMin.x + margen;
        choqueHorizontal = true;
    }

    if (pezJugador.pez.posicion.x > limiteMax.x - margen) {
        pezJugador.pez.posicion.x = limiteMax.x - margen;
        choqueHorizontal = true;
    }

    if (pezJugador.pez.posicion.y < limiteMin.y + margen) {
        pezJugador.pez.posicion.y = limiteMin.y + margen;
        pezJugador.velocidadVerticalActual = 0.0f;
    }

    if (pezJugador.pez.posicion.y > limiteMax.y - margen) {
        pezJugador.pez.posicion.y = limiteMax.y - margen;
        pezJugador.velocidadVerticalActual = 0.0f;
    }

    if (pezJugador.pez.posicion.z < limiteMin.z + margen) {
        pezJugador.pez.posicion.z = limiteMin.z + margen;
        choqueHorizontal = true;
    }

    if (pezJugador.pez.posicion.z > limiteMax.z - margen) {
        pezJugador.pez.posicion.z = limiteMax.z - margen;
        choqueHorizontal = true;
    }

    if (choqueHorizontal) {
        pezJugador.velocidadActual = 0.0f;
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

    pezJugador.velocidadActual = 0.0f;
    pezJugador.velocidadMaxima = 3.0f;
    pezJugador.aceleracion = 4.5f;
    pezJugador.rozamiento = 1.15f;

    pezJugador.velocidadVerticalActual = 0.0f;
    pezJugador.velocidadVerticalMaxima = 2.0f;
    pezJugador.aceleracionVertical = 3.0f;
    pezJugador.rozamientoVertical = 1.60f;

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

void girarPezJugador(PezJugador& pezJugador, float giroYaw, float deltaTime) {
    pezJugador.yaw += giroYaw * pezJugador.velocidadGiro * deltaTime;
    actualizarDireccionPezJugador(pezJugador);
}

void moverPezJugador(PezJugador& pezJugador, const Acuario& acuario, float acelerar, float subida, float deltaTime) {
    if (acelerar > 0.0f) {
        pezJugador.velocidadActual += pezJugador.aceleracion * deltaTime;
    }

    if (subida > 0.0f) {
        pezJugador.velocidadVerticalActual += pezJugador.aceleracionVertical * deltaTime;
    }

    if (subida < 0.0f) {
        pezJugador.velocidadVerticalActual -= pezJugador.aceleracionVertical * deltaTime;
    }

    limitarValor(pezJugador.velocidadActual, 0.0f, pezJugador.velocidadMaxima);
    limitarValor(pezJugador.velocidadVerticalActual, -pezJugador.velocidadVerticalMaxima, pezJugador.velocidadVerticalMaxima);

    aplicarRozamiento(pezJugador.velocidadActual, pezJugador.rozamiento, deltaTime);
    aplicarRozamiento(pezJugador.velocidadVerticalActual, pezJugador.rozamientoVertical, deltaTime);

    float pitchObjetivo = 0.0f;

    if (pezJugador.velocidadVerticalActual > 0.08f) {
        pitchObjetivo = 25.0f;
    }

    if (pezJugador.velocidadVerticalActual < -0.08f) {
        pitchObjetivo = -25.0f;
    }

    float velocidadRecuperacionPitch = 3.5f;
    pezJugador.pitch = pezJugador.pitch + (pitchObjetivo - pezJugador.pitch) * velocidadRecuperacionPitch * deltaTime;

    limitarValor(pezJugador.pitch, -35.0f, 35.0f);
    actualizarDireccionPezJugador(pezJugador);

    pezJugador.pez.posicion += pezJugador.pez.direccion * pezJugador.velocidadActual * deltaTime;
    pezJugador.pez.posicion.y += pezJugador.velocidadVerticalActual * deltaTime;

    limitarPezJugadorAlAcuario(pezJugador, acuario);
}

void actualizarPezJugador(PezJugador& pezJugador, float deltaTime) {
    float velocidadAnimacion = 4.0f + pezJugador.velocidadActual * 2.0f;
    pezJugador.pez.faseMovimiento += deltaTime * velocidadAnimacion;
}

void dibujarPezJugador(const PezJugador& pezJugador, GLuint shaderProgram) {
    dibujarPez(pezJugador.pez, shaderProgram);
}