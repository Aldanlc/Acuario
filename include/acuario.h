#ifndef ACUARIO_H
#define ACUARIO_H

#include <glad/glad.h>
#include <glm/glm.hpp>

const int NUM_FOCOS_ACUARIO = 8;

struct PiezaAcuario {
    glm::vec3 posicion;
    glm::vec3 escala;
    glm::vec3 color;
    GLuint textura_acuario;
};

struct SueloAcuario {
    glm::vec3 centro;
    glm::vec3 dimensiones;
    int divisionesX;
    int divisionesZ;
    float grosor;
    GLuint textura_suelo;
};

struct FocoAcuario {
    glm::vec3 posicion;
    glm::vec3 direccion;
    glm::vec3 colorEncendido;
    glm::vec3 colorApagado;
    bool encendido;
    bool superior;
    float cutOff;
    float outerCutOff;
};

struct Acuario {
    glm::vec3 centro;
    glm::vec3 dimensiones;
    glm::vec3 limiteMin;
    glm::vec3 limiteMax;
    float grosorCristal;
    SueloAcuario suelo;
    PiezaAcuario base;
    PiezaAcuario techo;
    PiezaAcuario paredIzquierda;
    PiezaAcuario paredDerecha;
    PiezaAcuario paredTrasera;
    PiezaAcuario paredFrontal;
    FocoAcuario focos[NUM_FOCOS_ACUARIO];
};

void inicializarAcuario(Acuario& acuario);
void inicializarAcuario(Acuario& acuario, const glm::vec3& centro, const glm::vec3& dimensiones);
void dibujarAcuario(const Acuario& acuario, GLuint shaderProgram);
void alternarFocosSuperiores(Acuario& acuario);
void alternarFocosInferiores(Acuario& acuario);
void alternarTodosLosFocos(Acuario& acuario);
glm::vec3 obtenerLimiteMinAcuario(const Acuario& acuario);
glm::vec3 obtenerLimiteMaxAcuario(const Acuario& acuario);

#endif