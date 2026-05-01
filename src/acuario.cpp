#include "acuario.h"
#include "cubo.h"

void inicializarAcuario(Acuario& acuario) {
    inicializarAcuario(acuario, glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(10.0f, 5.0f, 6.0f));
}

void inicializarAcuario(Acuario& acuario, const glm::vec3& centro, const glm::vec3& dimensiones) {
    acuario.centro = centro;
    acuario.dimensiones = dimensiones;
    acuario.grosorCristal = 0.04f;

    glm::vec3 mitad = dimensiones * 0.5f;

    acuario.limiteMin = centro - mitad + glm::vec3(acuario.grosorCristal);
    acuario.limiteMax = centro + mitad - glm::vec3(acuario.grosorCristal);

    crearSueloAcuario(acuario);
    crearAcuario(acuario);
}

void dibujarAcuario(const Acuario& acuario, GLuint shaderProgram) {
    dibujarSueloAcuario(acuario, shaderProgram);
    dibujarParedesAcuario(acuario, shaderProgram);
}

glm::vec3 obtenerLimiteMinAcuario(const Acuario& acuario) {
    return acuario.limiteMin;
}

glm::vec3 obtenerLimiteMaxAcuario(const Acuario& acuario) {
    return acuario.limiteMax;
}