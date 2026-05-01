#include "camaras.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static glm::vec3 calcularFrenteCamara(float yaw, float pitch) {
    glm::vec3 frente;

    frente.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frente.y = sin(glm::radians(pitch));
    frente.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    return glm::normalize(frente);
}

static DatosCamara calcularCamaraPezJugador(const PezJugador& pezJugador) {
    DatosCamara camara;

    glm::vec3 frente = pezJugador.pez.direccion;
    glm::vec3 alturaOjos = glm::vec3(0.0f, pezJugador.pez.escala * 0.18f, 0.0f);

    camara.camPos = pezJugador.pez.posicion + frente * pezJugador.pez.escala * 0.45f + alturaOjos;
    camara.frente = frente;
    camara.camTarget = camara.camPos + camara.frente;
    camara.view = glm::lookAt(camara.camPos, camara.camTarget, glm::vec3(0.0f, 1.0f, 0.0f));

    return camara;
}

DatosCamara calcularCamaraAcuario(const Acuario& acuario, const EstadoEntrada& estado, const PezJugador& pezJugador) {
    DatosCamara camara;

    if (estado.modoCamara == 1) {
        camara.camPos = estado.posicionCamaraLibre;
        camara.frente = calcularFrenteCamara(estado.yawCamaraLibre, estado.pitchCamaraLibre);
        camara.camTarget = camara.camPos + camara.frente;
    }
    else if (estado.modoCamara == 2) {
        camara.camPos = acuario.centro + glm::vec3(acuario.dimensiones.x * 1.25f, acuario.dimensiones.y * 0.45f, acuario.dimensiones.z * 1.25f);
        camara.camTarget = acuario.centro;
        camara.frente = glm::normalize(camara.camTarget - camara.camPos);
    }
    else if (estado.modoCamara == 3) {
        camara.camPos = acuario.centro + glm::vec3(0.0f, acuario.dimensiones.y * 1.6f, acuario.dimensiones.z * 0.15f);
        camara.camTarget = acuario.centro;
        camara.frente = glm::normalize(camara.camTarget - camara.camPos);
    }
    else {
        return calcularCamaraPezJugador(pezJugador);
    }

    camara.view = glm::lookAt(camara.camPos, camara.camTarget, glm::vec3(0.0f, 1.0f, 0.0f));

    return camara;
}

void configurarCamaraShader(const DatosCamara& camara, GLuint shaderProgram, const glm::mat4& projection) {
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(camara.view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(shaderProgram, "viewPos"), camara.camPos.x, camara.camPos.y, camara.camPos.z);
}