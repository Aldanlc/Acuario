#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "acuario.h"
#include "pezJugador.h"

struct EstadoEntrada {
    int modoCamara;

    glm::vec3 posicionCamaraLibre;
    float yawCamaraLibre;
    float pitchCamaraLibre;

    bool luzEncendidaFoco;
    bool teclaIPulsada;
    bool luzEncendidaBrazo;
    bool teclaTPulsada;
};

void inicializarEstadoEntrada(EstadoEntrada& estado);
void processInput(GLFWwindow* window, EstadoEntrada& estado, PezJugador& pezJugador, const Acuario& acuario, float deltaTime);

#endif