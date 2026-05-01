#include "acuario.h"
#include "cubo.h"

static void inicializarFocosAcuario(Acuario& acuario) {
    float margen = 0.35f;

    float xMin = acuario.limiteMin.x + margen;
    float xMax = acuario.limiteMax.x - margen;
    float yMin = acuario.limiteMin.y + margen;
    float yMax = acuario.limiteMax.y - margen;
    float zMin = acuario.limiteMin.z + margen;
    float zMax = acuario.limiteMax.z - margen;

    glm::vec3 posiciones[NUM_FOCOS_ACUARIO] = {
        glm::vec3(xMin, yMax, zMin),
        glm::vec3(xMax, yMax, zMin),
        glm::vec3(xMin, yMax, zMax),
        glm::vec3(xMax, yMax, zMax),
        glm::vec3(xMin, yMin, zMin),
        glm::vec3(xMax, yMin, zMin),
        glm::vec3(xMin, yMin, zMax),
        glm::vec3(xMax, yMin, zMax)
    };

    for (int i = 0; i < NUM_FOCOS_ACUARIO; i++) {
        acuario.focos[i].posicion = posiciones[i];
        acuario.focos[i].direccion = glm::normalize(acuario.centro - posiciones[i]);
        acuario.focos[i].superior = i < 4;
        acuario.focos[i].encendido = true;
        acuario.focos[i].cutOff = 30.0f;
        acuario.focos[i].outerCutOff = 50.0f;
        acuario.focos[i].colorApagado = glm::vec3(0.12f, 0.12f, 0.14f);

        if (acuario.focos[i].superior) {
            acuario.focos[i].colorEncendido = glm::vec3(1.35f, 1.60f, 1.80f);
        }
        else {
            acuario.focos[i].colorEncendido = glm::vec3(0.45f, 0.85f, 1.45f);
        }
    }
}

void inicializarAcuario(Acuario& acuario) {
    inicializarAcuario(acuario, glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(32.0f, 14.0f, 20.0f));
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
    inicializarFocosAcuario(acuario);
}

void dibujarAcuario(const Acuario& acuario, GLuint shaderProgram) {
    dibujarSueloAcuario(acuario, shaderProgram);
    dibujarParedesAcuario(acuario, shaderProgram);
}

void alternarFocosSuperiores(Acuario& acuario) {
    for (int i = 0; i < NUM_FOCOS_ACUARIO; i++) {
        if (acuario.focos[i].superior) {
            acuario.focos[i].encendido = !acuario.focos[i].encendido;
        }
    }
}

void alternarFocosInferiores(Acuario& acuario) {
    for (int i = 0; i < NUM_FOCOS_ACUARIO; i++) {
        if (!acuario.focos[i].superior) {
            acuario.focos[i].encendido = !acuario.focos[i].encendido;
        }
    }
}

void alternarTodosLosFocos(Acuario& acuario) {
    bool algunoEncendido = false;

    for (int i = 0; i < NUM_FOCOS_ACUARIO; i++) {
        if (acuario.focos[i].encendido) {
            algunoEncendido = true;
        }
    }

    for (int i = 0; i < NUM_FOCOS_ACUARIO; i++) {
        acuario.focos[i].encendido = !algunoEncendido;
    }
}

glm::vec3 obtenerLimiteMinAcuario(const Acuario& acuario) {
    return acuario.limiteMin;
}

glm::vec3 obtenerLimiteMaxAcuario(const Acuario& acuario) {
    return acuario.limiteMax;
}