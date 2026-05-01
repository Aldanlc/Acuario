#ifndef LUCES_H
#define LUCES_H

#include <glad/glad.h>
#include "acuario.h"

void configurarLucesAcuario(const Acuario& acuario, GLuint shaderProgram);
void dibujarFocosAcuario(const Acuario& acuario, GLuint shaderProgram);

#endif