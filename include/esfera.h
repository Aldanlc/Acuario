#pragma warning(disable:4996)
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include <math.h> 
#include <iostream>

// Geometría de la esfera: array de vértices y tamaño total en memoria.
// Se declaran como extern porque su definición real está en el archivo .cpp.
extern const unsigned int verticesEsferaSize;
extern float vertices_esfera[];

// Crea y configura la geometría base de la esfera.
void crearEsfera();

// Libera la memoria asociada a la geometría de la esfera.
void liberarEsfera();