#ifndef CUBO_H
#define CUBO_H

#include <glad/glad.h>
#include "acuario.h"

void crearCubo();
void liberarCubo();

void crearSueloAcuario(Acuario& acuario);
void crearAcuario(Acuario& acuario);

void dibujarSueloAcuario(const Acuario& acuario, GLuint shaderProgram);
void dibujarParedesAcuario(const Acuario& acuario, GLuint shaderProgram);

extern GLuint VAO_CUBO;
extern GLuint VBO_CUBO;
extern float vertices[];
extern const unsigned int verticesSize;

#endif