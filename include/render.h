#ifndef RENDER_H
#define RENDER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "input.h"
#include "acuario.h"
#include "pez.h"
#include "pezJugador.h"

struct RecursosRender {
    GLuint shaderProgram;

    GLuint texturaSuelo;
    GLuint texturaAcuario;
    GLuint texturaPez;
};

GLFWwindow* inicializarVentana(unsigned int ancho, unsigned int alto, const char* titulo);
void openGlInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
glm::mat4 obtenerProjection();
void cargarRecursos(RecursosRender& recursos);
void renderizarFrame(const Acuario& acuario, const Pez peces[], int numeroPeces, const PezJugador& pezJugador, const EstadoEntrada& estado, const RecursosRender& recursos);
void liberar(RecursosRender& recursos);
void cargarTexturasEscena(RecursosRender& recursos, Acuario& acuario, Pez peces[], int numeroPeces, PezJugador& pezJugador);

#endif