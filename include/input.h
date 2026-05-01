#ifndef INPUT_H
#define INPUT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct EstadoEntrada {
    int modoCamara;
    bool luzEncendidaFoco;
    bool teclaIPulsada;
    bool luzEncendidaBrazo;
    bool teclaTPulsada;
};

void inicializarEstadoEntrada(EstadoEntrada& estado);
void processInput(GLFWwindow* window, EstadoEntrada& estado, float deltaTime);

#endif
