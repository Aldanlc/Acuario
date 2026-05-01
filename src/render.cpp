#include "render.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "lecturaShader_0_9.h"
#include "cubo.h"
#include "esfera.h"
#include "camaras.h"

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

void renderizarFrame(const Acuario& acuario, const EstadoEntrada& estado, const RecursosRender& recursos) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(recursos.shaderProgram);

    DatosCamara camara = calcularCamaraAcuario(acuario, estado.modoCamara);
    configurarCamaraShader(camara, recursos.shaderProgram, projection);

    dibujarAcuario(acuario, recursos.shaderProgram);
}

void liberar(const RecursosRender& recursos) {
    liberarEsfera();
    liberarCubo();

    glDeleteProgram(recursos.shaderProgram);
    glfwTerminate();
}