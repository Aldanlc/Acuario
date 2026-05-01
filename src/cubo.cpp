#include "cubo.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

GLuint VAO_CUBO;
GLuint VBO_CUBO;

float vertices[] = {
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   1.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,

     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 1.0f
};

const unsigned int verticesSize = sizeof(vertices);

static void prepararColorCubo(GLuint shaderProgram, const glm::vec3& color, float alpha) {
    glUniform1i(glGetUniformLocation(shaderProgram, "usarTextura"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarTexturaSuelo"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarArbusto"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarColorUniform"), 1);
    glUniform1f(glGetUniformLocation(shaderProgram, "alphaUniform"), alpha);
    glUniform3f(glGetUniformLocation(shaderProgram, "colorUniform"), color.r, color.g, color.b);
}

static glm::mat4 crearMatrizPiezaAcuario(const PiezaAcuario& pieza) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, pieza.posicion);
    model = glm::scale(model, pieza.escala);
    return model;
}

static glm::mat4 crearMatrizParteCubicaPez(const PiezaPez& pieza, const glm::mat4& modeloPadre, float angulo, const glm::vec3& eje) {
    glm::mat4 model = modeloPadre;
    model = glm::translate(model, pieza.posicionLocal);
    model = glm::rotate(model, glm::radians(angulo), eje);
    model = glm::scale(model, pieza.escala);
    return model;
}

static void dibujarCuboBase() {
    glBindVertexArray(VAO_CUBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

static void dibujarPiezaAcuario(const PiezaAcuario& pieza, GLuint shaderProgram) {
    glm::mat4 model = crearMatrizPiezaAcuario(pieza);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    prepararColorCubo(shaderProgram, pieza.color, 0.25f);
    dibujarCuboBase();
}

void crearSueloAcuario(Acuario& acuario) {
    acuario.suelo.grosor = 0.08f;
    acuario.suelo.dimensiones = glm::vec3(acuario.dimensiones.x - 2.0f * acuario.grosorCristal, acuario.suelo.grosor, acuario.dimensiones.z - 2.0f * acuario.grosorCristal);
    acuario.suelo.centro = glm::vec3(acuario.centro.x, acuario.limiteMin.y + acuario.suelo.grosor * 0.5f, acuario.centro.z);
    acuario.suelo.divisionesX = (int)acuario.suelo.dimensiones.x;
    acuario.suelo.divisionesZ = (int)acuario.suelo.dimensiones.z;

    if (acuario.suelo.divisionesX < 1) {
        acuario.suelo.divisionesX = 1;
    }

    if (acuario.suelo.divisionesZ < 1) {
        acuario.suelo.divisionesZ = 1;
    }

    acuario.limiteMin.y += acuario.suelo.grosor;
}

void crearAcuario(Acuario& acuario) {
    float g = acuario.grosorCristal;
    float ancho = acuario.dimensiones.x;
    float alto = acuario.dimensiones.y;
    float fondo = acuario.dimensiones.z;
    float xMin = acuario.centro.x - ancho * 0.5f;
    float xMax = acuario.centro.x + ancho * 0.5f;
    float yMin = acuario.centro.y - alto * 0.5f;
    float yMax = acuario.centro.y + alto * 0.5f;
    float zMin = acuario.centro.z - fondo * 0.5f;
    float zMax = acuario.centro.z + fondo * 0.5f;

    glm::vec3 colorCristal = glm::vec3(0.20f, 0.65f, 0.90f);

    acuario.base.posicion = glm::vec3(acuario.centro.x, yMin + g * 0.5f, acuario.centro.z);
    acuario.base.escala = glm::vec3(ancho, g, fondo);
    acuario.base.color = colorCristal;

    acuario.techo.posicion = glm::vec3(acuario.centro.x, yMax - g * 0.5f, acuario.centro.z);
    acuario.techo.escala = glm::vec3(ancho, g, fondo);
    acuario.techo.color = colorCristal;

    acuario.paredIzquierda.posicion = glm::vec3(xMin + g * 0.5f, acuario.centro.y, acuario.centro.z);
    acuario.paredIzquierda.escala = glm::vec3(g, alto, fondo);
    acuario.paredIzquierda.color = colorCristal;

    acuario.paredDerecha.posicion = glm::vec3(xMax - g * 0.5f, acuario.centro.y, acuario.centro.z);
    acuario.paredDerecha.escala = glm::vec3(g, alto, fondo);
    acuario.paredDerecha.color = colorCristal;

    acuario.paredTrasera.posicion = glm::vec3(acuario.centro.x, acuario.centro.y, zMin + g * 0.5f);
    acuario.paredTrasera.escala = glm::vec3(ancho, alto, g);
    acuario.paredTrasera.color = colorCristal;

    acuario.paredFrontal.posicion = glm::vec3(acuario.centro.x, acuario.centro.y, zMax - g * 0.5f);
    acuario.paredFrontal.escala = glm::vec3(ancho, alto, g);
    acuario.paredFrontal.color = colorCristal;
}

void dibujarParedesAcuario(const Acuario& acuario, GLuint shaderProgram) {
    dibujarPiezaAcuario(acuario.base, shaderProgram);
    dibujarPiezaAcuario(acuario.techo, shaderProgram);
    dibujarPiezaAcuario(acuario.paredIzquierda, shaderProgram);
    dibujarPiezaAcuario(acuario.paredDerecha, shaderProgram);
    dibujarPiezaAcuario(acuario.paredTrasera, shaderProgram);
    dibujarPiezaAcuario(acuario.paredFrontal, shaderProgram);
}

void dibujarSueloAcuario(const Acuario& acuario, GLuint shaderProgram) {
    float tamX = acuario.suelo.dimensiones.x / acuario.suelo.divisionesX;
    float tamZ = acuario.suelo.dimensiones.z / acuario.suelo.divisionesZ;
    float inicioX = acuario.suelo.centro.x - acuario.suelo.dimensiones.x * 0.5f;
    float inicioZ = acuario.suelo.centro.z - acuario.suelo.dimensiones.z * 0.5f;

    for (int x = 0; x < acuario.suelo.divisionesX; x++) {
        for (int z = 0; z < acuario.suelo.divisionesZ; z++) {
            glm::vec3 color = ((x + z) % 2 == 0) ? glm::vec3(0.70f, 0.55f, 0.30f) : glm::vec3(0.58f, 0.44f, 0.24f);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(inicioX + tamX * 0.5f + x * tamX, acuario.suelo.centro.y, inicioZ + tamZ * 0.5f + z * tamZ));
            model = glm::scale(model, glm::vec3(tamX, acuario.suelo.grosor, tamZ));

            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            prepararColorCubo(shaderProgram, color, 1.0f);
            dibujarCuboBase();
        }
    }
}

void dibujarParteCubicaPez(const PiezaPez& pieza, const glm::mat4& modeloPadre, float angulo, const glm::vec3& eje, GLuint shaderProgram) {
    glm::mat4 model = crearMatrizParteCubicaPez(pieza, modeloPadre, angulo, eje);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
    prepararColorCubo(shaderProgram, pieza.color, 1.0f);
    dibujarCuboBase();
}

void crearCubo() {
    glGenVertexArrays(1, &VAO_CUBO);
    glGenBuffers(1, &VBO_CUBO);

    glBindVertexArray(VAO_CUBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_CUBO);
    glBufferData(GL_ARRAY_BUFFER, verticesSize, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void liberarCubo() {
    glDeleteVertexArrays(1, &VAO_CUBO);
    glDeleteBuffers(1, &VBO_CUBO);
}