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
#include "decoracion.h"

// Matriz de proyección global, se recalcula si cambia el tamaño de la ventana
static glm::mat4 projection;

GLFWwindow* inicializarVentana(unsigned int ancho, unsigned int alto, const char* titulo) {
    glfwInit();

    // Configuración de OpenGL 3.3 Core Profile
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

    // Activamos profundidad y transparencias
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    if (height == 0) {
        height = 1;
    }

    glViewport(0, 0, width, height);

    // Actualizamos la perspectiva según el nuevo tamaño de ventana
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
}

glm::mat4 obtenerProjection() {
    return projection;
}

void cargarRecursos(RecursosRender& recursos) {
    // Creamos las geometrías base y cargamos el shader principal
    crearCubo();
    crearEsfera();

    recursos.shaderProgram = setShaders("shaders/shader.vert", "shaders/shader.frag");
}

void renderizarFrame(const Acuario& acuario, const Pez peces[], int numeroPeces, const PezJugador& pezJugador, const EstadoEntrada& estado, const RecursosRender& recursos) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(recursos.shaderProgram);

    // Configuración de cámara y luces antes de dibujar la escena
    DatosCamara camara = calcularCamaraAcuario(acuario, estado, pezJugador);
    configurarCamaraShader(camara, recursos.shaderProgram, projection);
    configurarLucesAcuario(acuario, recursos.shaderProgram);

    // Objetos principales de la escena
    dibujarSueloAcuario(acuario, recursos.shaderProgram);
    dibujarPeces(peces, numeroPeces, recursos.shaderProgram);

    if (estado.modoCamara != 4) {
        dibujarPezJugador(pezJugador, recursos.shaderProgram);
    }

    // Decoración del acuario
    dibujarAlgas(acuario, recursos.shaderProgram, recursos.texturaAlga);
    dibujarCorales(acuario, recursos.shaderProgram, recursos.texturaCoral);

    dibujarFocosAcuario(acuario, recursos.shaderProgram);

    // Las paredes transparentes se dibujan al final
    glDepthMask(GL_FALSE);
    dibujarParedesAcuario(acuario, recursos.shaderProgram);
    glDepthMask(GL_TRUE);
}

void liberar(RecursosRender& recursos) {
    liberarEsfera();
    liberarCubo();

    // Liberamos texturas cargadas en GPU
    eliminarTextura(recursos.texturaSuelo);
    eliminarTextura(recursos.texturaAcuario);
    eliminarTextura(recursos.texturaPez);
    eliminarTextura(recursos.texturaAlga);
    eliminarTextura(recursos.texturaCoral);

    glDeleteProgram(recursos.shaderProgram);
    glfwTerminate();
}

void cargarTexturasEscena(RecursosRender& recursos, Acuario& acuario, Pez peces[], int numeroPeces, PezJugador& pezJugador) {
    // Carga de texturas desde disco
    recursos.texturaSuelo = cargarTextura("texturas/arena.png");
    recursos.texturaAcuario = cargarTextura("texturas/acuario.png");
    recursos.texturaPez = cargarTextura("texturas/pez.png");
    recursos.texturaAlga = cargarTextura("texturas/alga.png");
    recursos.texturaCoral = cargarTextura("texturas/coral.png");

    // Asignamos texturas al acuario
    acuario.suelo.textura_suelo = recursos.texturaSuelo;

    acuario.base.textura_acuario = recursos.texturaAcuario;
    acuario.techo.textura_acuario = recursos.texturaAcuario;
    acuario.paredIzquierda.textura_acuario = recursos.texturaAcuario;
    acuario.paredDerecha.textura_acuario = recursos.texturaAcuario;
    acuario.paredTrasera.textura_acuario = recursos.texturaAcuario;
    acuario.paredFrontal.textura_acuario = recursos.texturaAcuario;

    // Asignamos la textura común a las partes de cada pez
    for (int i = 0; i < numeroPeces; i++) {
        peces[i].cuerpo.textura_pez = recursos.texturaPez;
        peces[i].cabeza.textura_pez = recursos.texturaPez;
        peces[i].cola.textura_pez = recursos.texturaPez;
        peces[i].aletaIzquierda.textura_pez = recursos.texturaPez;
        peces[i].aletaDerecha.textura_pez = recursos.texturaPez;
        peces[i].ojoIzquierdo.textura_pez = 0;
        peces[i].ojoDerecho.textura_pez = 0;
    }

    // El pez jugador usa la misma textura que los peces normales
    pezJugador.pez.cuerpo.textura_pez = recursos.texturaPez;
    pezJugador.pez.cabeza.textura_pez = recursos.texturaPez;
    pezJugador.pez.cola.textura_pez = recursos.texturaPez;
    pezJugador.pez.aletaIzquierda.textura_pez = recursos.texturaPez;
    pezJugador.pez.aletaDerecha.textura_pez = recursos.texturaPez;
    pezJugador.pez.ojoIzquierdo.textura_pez = 0;
    pezJugador.pez.ojoDerecho.textura_pez = 0;
}