#ifndef ESFERA_H
#define ESFERA_H

#pragma warning(disable:4996)

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <math.h>
#include <iostream>

#include "pez.h"

extern GLuint VAO_esfera;
extern GLuint VBO_esfera;
extern const unsigned int verticesEsferaSize;
extern float vertices_esfera[];

void crearEsfera();
void liberarEsfera();

void crearModeloPez(Pez& pez);
void dibujarParteEsfericaPez(const PiezaPez& pieza, const glm::mat4& modeloPadre, GLuint shaderProgram);

#endif