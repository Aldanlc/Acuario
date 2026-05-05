#ifndef DECORACION_H
#define DECORACION_H

#include <glad/glad.h>
#include "acuario.h"

void dibujarAlgas(const Acuario& acuario, GLuint shaderProgram, GLuint texturaAlga);
void dibujarCorales(const Acuario& acuario, GLuint shaderProgram, GLuint texturaCoral);

#endif