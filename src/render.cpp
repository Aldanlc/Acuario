#include "render.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "lecturaShader_0_9.h"
#include "cubo.h"
#include "esfera.h"
#include "camaras.h"
#include "luces.h"
#include "pez.h"
#include "pezJugador.h"
#include "textura.h"

static glm::mat4 projection;

GLFWwindow* inicializarVentana(unsigned int ancho, unsigned int alto, const char* titulo) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(ancho, alto, titulo, NULL, NULL);

    if (window == NULL) {
        std::cout << "Error creando ventana" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error inicializando GLAD" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    return window;
}

void openGlInit() {
    glClearDepth(1.0f);
    glClearColor(0.02f, 0.08f, 0.12f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (height == 0) {
        height = 1;
    }

    glViewport(0, 0, width, height);
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

glm::mat4 obtenerProjection() {
    return projection;
}

void cargarRecursos(RecursosRender& recursos) {
    crearCubo();
    crearEsfera();

    recursos.shaderProgram = setShaders("shaders/shader.vert", "shaders/shader.frag");
}

void renderizarFrame(const Acuario& acuario, const Pez peces[], int numeroPeces, const PezJugador& pezJugador, const EstadoEntrada& estado, const RecursosRender& recursos) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(recursos.shaderProgram);

    DatosCamara camara = calcularCamaraAcuario(acuario, estado, pezJugador);
    configurarCamaraShader(camara, recursos.shaderProgram, projection);
    configurarLucesAcuario(acuario, recursos.shaderProgram);

    dibujarSueloAcuario(acuario, recursos.shaderProgram);
    dibujarPeces(peces, numeroPeces, recursos.shaderProgram);

    if (estado.modoCamara != 4) {
        dibujarPezJugador(pezJugador, recursos.shaderProgram);
    }

    dibujarFocosAcuario(acuario, recursos.shaderProgram);
    glDepthMask(GL_FALSE);
    dibujarParedesAcuario(acuario, recursos.shaderProgram);
    glDepthMask(GL_TRUE);
}

void liberar(RecursosRender& recursos) {
    liberarEsfera();
    liberarCubo();

    eliminarTextura(recursos.texturaSuelo);
    eliminarTextura(recursos.texturaAcuario);
    eliminarTextura(recursos.texturaPez);

    glDeleteProgram(recursos.shaderProgram);
    glfwTerminate();
}

void cargarTexturasEscena(RecursosRender& recursos, Acuario& acuario, Pez peces[], int numeroPeces, PezJugador& pezJugador) {
    recursos.texturaSuelo = cargarTextura("texturas/arena.png");
    recursos.texturaAcuario = cargarTextura("texturas/acuario.png");
    recursos.texturaPez = cargarTextura("texturas/pez.png");

    acuario.suelo.textura_suelo = recursos.texturaSuelo;

    acuario.base.textura_acuario = recursos.texturaAcuario;
    acuario.techo.textura_acuario = recursos.texturaAcuario;
    acuario.paredIzquierda.textura_acuario = recursos.texturaAcuario;
    acuario.paredDerecha.textura_acuario = recursos.texturaAcuario;
    acuario.paredTrasera.textura_acuario = recursos.texturaAcuario;
    acuario.paredFrontal.textura_acuario = recursos.texturaAcuario;

    for (int i = 0; i < numeroPeces; i++) {
        peces[i].cuerpo.textura_pez = recursos.texturaPez;
        peces[i].cabeza.textura_pez = recursos.texturaPez;
        peces[i].cola.textura_pez = recursos.texturaPez;
        peces[i].aletaIzquierda.textura_pez = recursos.texturaPez;
        peces[i].aletaDerecha.textura_pez = recursos.texturaPez;
        peces[i].ojoIzquierdo.textura_pez = 0;
        peces[i].ojoDerecho.textura_pez = 0;
    }

    pezJugador.pez.cuerpo.textura_pez = recursos.texturaPez;
    pezJugador.pez.cabeza.textura_pez = recursos.texturaPez;
    pezJugador.pez.cola.textura_pez = recursos.texturaPez;
    pezJugador.pez.aletaIzquierda.textura_pez = recursos.texturaPez;
    pezJugador.pez.aletaDerecha.textura_pez = recursos.texturaPez;
    pezJugador.pez.ojoIzquierdo.textura_pez = 0;
    pezJugador.pez.ojoDerecho.textura_pez = 0;
}