#ifndef PEZ_JUGADOR_H
#define PEZ_JUGADOR_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "pez.h"
#include "acuario.h"

struct PezJugador {
    Pez pez;

    float yaw;
    float pitch;

    float velocidadActual;
    float velocidadMaxima;
    float aceleracion;
    float rozamiento;

    float velocidadVerticalActual;
    float velocidadVerticalMaxima;
    float aceleracionVertical;
    float rozamientoVertical;

    float velocidadGiro;
};

void inicializarPezJugador(PezJugador& pezJugador, const Acuario& acuario);
void actualizarDireccionPezJugador(PezJugador& pezJugador);
void girarPezJugador(PezJugador& pezJugador, float giroYaw, float deltaTime);
void moverPezJugador(PezJugador& pezJugador, const Acuario& acuario, float acelerar, float subida, float deltaTime);
void actualizarPezJugador(PezJugador& pezJugador, float deltaTime);
void dibujarPezJugador(const PezJugador& pezJugador, GLuint shaderProgram);

#endif