#include "pez.h"
#include "esfera.h"
#include "cubo.h"

#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static glm::mat4 crearMatrizModeloPez(const Pez& pez) {
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 direccion = glm::normalize(pez.direccion);

    // Calculamos los ángulos para orientar el pez hacia su dirección de movimiento
    float yaw = atan2(direccion.x, direccion.z);
    float longitudHorizontal = sqrt(direccion.x * direccion.x + direccion.z * direccion.z);
    float pitch = atan2(direccion.y, longitudHorizontal);

    // Primero colocamos el pez en su posición y luego lo orientamos
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

    // Crea las partes que forman el modelo del pez
    crearModeloPez(pez);
}

void inicializarPeces(Pez peces[], int numeroPeces, const Acuario& acuario) {
    if (numeroPeces <= 0) return;

    glm::vec3 centro = acuario.centro;

    for (int i = 0; i < numeroPeces; i++) {
        // Distribución inicial de los peces dentro del acuario
        float x = -13.0f + (i % 5) * 6.5f;
        float z = -8.0f + (i / 5) * 5.0f;

        float y;
        // Peces más altos
        if (i < numeroPeces / 2) {
            y = 2.5f + (i % 3) * 1.4f;
        }
        // Peces más cerca del suelo
        else {
            y = 0.4f + ((i - numeroPeces / 2) % 3) * 0.6f;
        }

        glm::vec3 posicion = centro + glm::vec3(x, y, z);

        // Dirección inicial distinta para cada pez
        glm::vec3 direccion = glm::normalize(glm::vec3(
            sin(i * 1.7f),
            0.12f * sin(i * 0.8f),
            cos(i * 2.1f)
        ));

        // Color generado con senos/cosenos para variar entre peces
        glm::vec3 color = glm::vec3(
            0.35f + 0.65f * fabs(sin(i * 1.3f)),
            0.35f + 0.65f * fabs(sin(i * 2.1f)),
            0.35f + 0.65f * fabs(cos(i * 1.6f))
        );

        float velocidad = 0.70f + 0.05f * (i % 6);
        float escala = 0.45f + 0.05f * (i % 5);

        inicializarPez(peces[i], posicion, direccion, color, velocidad, escala);
    }
}

static void evitarChoquesPeces(Pez peces[], int numeroPeces, float deltaTime) {
    for (int i = 0; i < numeroPeces; i++) {
        glm::vec3 direccionEvasion(0.0f);

        for (int j = 0; j < numeroPeces; j++) {
            if (i == j) continue;

            glm::vec3 separacion = peces[i].posicion - peces[j].posicion;
            float distancia = glm::length(separacion);
            float distanciaMinima = peces[i].radioColision + peces[j].radioColision + 0.6f;

            // Si dos peces están demasiado cerca, se calcula una dirección de evasión
            if (distancia > 0.001f && distancia < distanciaMinima) {
                direccionEvasion += glm::normalize(separacion) * (distanciaMinima - distancia);
            }
        }

        // Suavizamos el cambio de dirección para que no gire de golpe
        if (glm::length(direccionEvasion) > 0.001f) {
            glm::vec3 nuevaDireccion = glm::normalize(peces[i].direccion + direccionEvasion * 2.5f);
            peces[i].direccion = glm::normalize(glm::mix(peces[i].direccion, nuevaDireccion, 4.0f * deltaTime));
        }
    }
}

void actualizarPez(Pez& pez, const Acuario& acuario, float deltaTime) {
    glm::vec3 limiteMin = obtenerLimiteMinAcuario(acuario);
    glm::vec3 limiteMax = obtenerLimiteMaxAcuario(acuario);

    // Margen para que el pez no atraviese las paredes
    float margen = pez.escala * 0.70f;

    pez.posicion += pez.direccion * pez.velocidad * deltaTime;
    pez.faseMovimiento += deltaTime * 6.0f;

    // Rebote en los límites del acuario
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
    // Primero se ajustan direcciones para evitar choques entre peces
    evitarChoquesPeces(peces, numeroPeces, deltaTime);

    for (int i = 0; i < numeroPeces; i++) {
        actualizarPez(peces[i], acuario, deltaTime);
    }
}

void dibujarPez(const Pez& pez, GLuint shaderProgram) {
    glm::mat4 modeloPez = crearMatrizModeloPez(pez);

    // Movimiento oscilatorio para cola y aletas
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