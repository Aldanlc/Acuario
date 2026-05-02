#ifndef TEXTURA_H
#define TEXTURA_H

#include "glad/glad.h"

GLuint cargarTextura(const char* textura);
void eliminarTextura(GLuint& textura);
void activarTextura(GLuint shaderProgram, GLuint textura);


#endif