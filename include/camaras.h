#ifndef CAMARAS_H
#define CAMARAS_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include "acuario.h"

struct DatosCamara {
    glm::vec3 camPos;
    glm::vec3 camTarget;
    glm::vec3 frente;
    glm::mat4 view;
};

DatosCamara calcularCamaraAcuario(const Acuario& acuario, int modoCamara);
void configurarCamaraShader(const DatosCamara& camara, GLuint shaderProgram, const glm::mat4& projection);

#endif