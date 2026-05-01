#include "pez.h"
#include "esfera.h"
#include "cubo.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static glm::mat4 crearMatrizModeloPez(const Pez& pez) {
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 direccion = glm::normalize(pez.direccion);

    float yaw = atan2(direccion.x, direccion.z);
    float longitudHorizontal = sqrt(direccion.x * direccion.x + direccion.z * direccion.z);
    float pitch = atan2(direccion.y, longitudHorizontal);

    model = glm::translate(model, pez.posicion);
    model = glm::rotate(model, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, -pitch, glm::vec3(1.0f, 0.0f, 0.0f));

    return model;
}

void inicializarPez(Pez& pez, const glm::vec3& posicion, const glm::vec3& direccion, const glm::vec3& color, float velocidad, float escala) {
    pez.posicion = posicion;
    pez.direccion = glm::normalize(direccion);
    pez.color = color;
    pez.velocidad = velocidad;
    pez.escala = escala;
    pez.faseMovimiento = 0.0f;

    crearModeloPez(pez);
}

void inicializarPeces(Pez peces[], int numeroPeces, const Acuario& acuario) {
    if (numeroPeces <= 0) {
        return;
    }

    glm::vec3 centro = acuario.centro;

    inicializarPez(peces[0], centro + glm::vec3(-7.0f, 1.0f, -3.0f), glm::vec3(1.0f, 0.0f, 0.25f), glm::vec3(1.0f, 0.35f, 0.15f), 1.20f, 0.70f);

    if (numeroPeces > 1) {
        inicializarPez(peces[1], centro + glm::vec3(5.5f, 2.0f, -5.0f), glm::vec3(-0.8f, 0.15f, 0.5f), glm::vec3(0.15f, 0.55f, 1.0f), 0.90f, 0.55f);
    }

    if (numeroPeces > 2) {
        inicializarPez(peces[2], centro + glm::vec3(0.0f, 3.0f, 4.5f), glm::vec3(0.5f, -0.1f, -1.0f), glm::vec3(0.95f, 0.85f, 0.15f), 0.75f, 0.50f);
    }

    if (numeroPeces > 3) {
        inicializarPez(peces[3], centro + glm::vec3(-10.0f, 4.0f, 2.0f), glm::vec3(1.0f, 0.1f, -0.35f), glm::vec3(0.20f, 0.95f, 0.45f), 1.05f, 0.60f);
    }

    if (numeroPeces > 4) {
        inicializarPez(peces[4], centro + glm::vec3(9.0f, 1.5f, 6.0f), glm::vec3(-1.0f, 0.0f, -0.25f), glm::vec3(1.0f, 0.25f, 0.55f), 1.35f, 0.45f);
    }

    if (numeroPeces > 5) {
        inicializarPez(peces[5], centro + glm::vec3(-3.0f, 5.0f, -7.0f), glm::vec3(0.35f, -0.1f, 1.0f), glm::vec3(0.55f, 0.25f, 1.0f), 0.80f, 0.75f);
    }

    if (numeroPeces > 6) {
        inicializarPez(peces[6], centro + glm::vec3(3.0f, 6.0f, 0.0f), glm::vec3(-0.6f, 0.05f, 1.0f), glm::vec3(0.10f, 0.90f, 0.95f), 1.10f, 0.52f);
    }

    if (numeroPeces > 7) {
        inicializarPez(peces[7], centro + glm::vec3(11.0f, 3.5f, -2.0f), glm::vec3(-1.0f, 0.05f, 0.15f), glm::vec3(0.95f, 0.45f, 0.10f), 0.95f, 0.68f);
    }

    if (numeroPeces > 8) {
        inicializarPez(peces[8], centro + glm::vec3(-12.0f, 2.2f, 7.0f), glm::vec3(1.0f, 0.0f, -0.45f), glm::vec3(0.75f, 1.0f, 0.25f), 1.25f, 0.48f);
    }

    if (numeroPeces > 9) {
        inicializarPez(peces[9], centro + glm::vec3(0.0f, 6.5f, -8.0f), glm::vec3(0.2f, -0.1f, 1.0f), glm::vec3(1.0f, 0.70f, 0.20f), 0.70f, 0.90f);
    }
}

void actualizarPez(Pez& pez, const Acuario& acuario, float deltaTime) {
    glm::vec3 limiteMin = obtenerLimiteMinAcuario(acuario);
    glm::vec3 limiteMax = obtenerLimiteMaxAcuario(acuario);
    float margen = pez.escala * 0.70f;

    pez.posicion += pez.direccion * pez.velocidad * deltaTime;
    pez.faseMovimiento += deltaTime * 6.0f;

    if (pez.posicion.x < limiteMin.x + margen) {
        pez.posicion.x = limiteMin.x + margen;
        pez.direccion.x *= -1.0f;
    }

    if (pez.posicion.x > limiteMax.x - margen) {
        pez.posicion.x = limiteMax.x - margen;
        pez.direccion.x *= -1.0f;
    }

    if (pez.posicion.y < limiteMin.y + margen) {
        pez.posicion.y = limiteMin.y + margen;
        pez.direccion.y *= -1.0f;
    }

    if (pez.posicion.y > limiteMax.y - margen) {
        pez.posicion.y = limiteMax.y - margen;
        pez.direccion.y *= -1.0f;
    }

    if (pez.posicion.z < limiteMin.z + margen) {
        pez.posicion.z = limiteMin.z + margen;
        pez.direccion.z *= -1.0f;
    }

    if (pez.posicion.z > limiteMax.z - margen) {
        pez.posicion.z = limiteMax.z - margen;
        pez.direccion.z *= -1.0f;
    }

    pez.direccion = glm::normalize(pez.direccion);
}

void actualizarPeces(Pez peces[], int numeroPeces, const Acuario& acuario, float deltaTime) {
    for (int i = 0; i < numeroPeces; i++) {
        actualizarPez(peces[i], acuario, deltaTime);
    }
}

void dibujarPez(const Pez& pez, GLuint shaderProgram) {
    glm::mat4 modeloPez = crearMatrizModeloPez(pez);

    float anguloCola = sin(pez.faseMovimiento) * 25.0f;
    float anguloAletas = sin(pez.faseMovimiento + 1.5f) * 18.0f;

    dibujarParteEsfericaPez(pez.cuerpo, modeloPez, shaderProgram);
    dibujarParteEsfericaPez(pez.cabeza, modeloPez, shaderProgram);
    dibujarParteCubicaPez(pez.cola, modeloPez, anguloCola, glm::vec3(0.0f, 1.0f, 0.0f), shaderProgram);
    dibujarParteCubicaPez(pez.aletaIzquierda, modeloPez, -25.0f + anguloAletas, glm::vec3(0.0f, 0.0f, 1.0f), shaderProgram);
    dibujarParteCubicaPez(pez.aletaDerecha, modeloPez, 25.0f - anguloAletas, glm::vec3(0.0f, 0.0f, 1.0f), shaderProgram);
    dibujarParteEsfericaPez(pez.ojoIzquierdo, modeloPez, shaderProgram);
    dibujarParteEsfericaPez(pez.ojoDerecho, modeloPez, shaderProgram);
}

void dibujarPeces(const Pez peces[], int numeroPeces, GLuint shaderProgram) {
    for (int i = 0; i < numeroPeces; i++) {
        dibujarPez(peces[i], shaderProgram);
    }
}