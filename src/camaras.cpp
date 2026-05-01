#include "camaras.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

DatosCamara calcularCamaraAcuario(const Acuario& acuario, int modoCamara) {
    DatosCamara camara;

    if (modoCamara == 1) {
        camara.camPos = acuario.centro + glm::vec3(0.0f, acuario.dimensiones.y * 0.45f, acuario.dimensiones.z * 2.0f);
        camara.camTarget = acuario.centro;
    }
    else if (modoCamara == 2) {
        camara.camPos = acuario.centro + glm::vec3(acuario.dimensiones.x * 1.25f, acuario.dimensiones.y * 0.45f, acuario.dimensiones.z * 1.25f);
        camara.camTarget = acuario.centro;
    }
    else {
        camara.camPos = acuario.centro + glm::vec3(0.0f, acuario.dimensiones.y * 1.6f, acuario.dimensiones.z * 0.15f);
        camara.camTarget = acuario.centro;
    }

    camara.frente = glm::normalize(camara.camTarget - camara.camPos);
    camara.view = glm::lookAt(camara.camPos, camara.camTarget, glm::vec3(0.0f, 1.0f, 0.0f));

    return camara;
}

void configurarCamaraShader(const DatosCamara& camara, GLuint shaderProgram, const glm::mat4& projection) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camara.view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), camara.camPos.x, camara.camPos.y, camara.camPos.z);
}