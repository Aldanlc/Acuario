#ifndef CUBO_H
#define CUBO_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "acuario.h"
#include "pez.h"

void crearCubo();
void liberarCubo();

void crearSueloAcuario(Acuario& acuario);
void crearAcuario(Acuario& acuario);

void dibujarSueloAcuario(const Acuario& acuario, GLuint shaderProgram);
void dibujarParedesAcuario(const Acuario& acuario, GLuint shaderProgram);
void dibujarParteCubicaPez(const PiezaPez& pieza, const glm::mat4& modeloPadre, float angulo, const glm::vec3& eje, GLuint shaderProgram);

extern GLuint VAO_CUBO;
extern GLuint VBO_CUBO;
extern float vertices[];
extern const unsigned int verticesSize;

#endif