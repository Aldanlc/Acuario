#include "render.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "lecturaShader_0_9.h"
#include "cubo.h"
#include "esfera.h"

// Matriz de proyección global del módulo de render.
// Se calcula al crear/redimensionar la ventana y luego se usa en cada frame.
// Representa la transformación de view space a clip space.
static glm::mat4 projection;

// Esta función se llama al principio del main.
// Su objetivo es inicializar GLFW, crear la ventana, crear el contexto OpenGL,
// registrar el callback de redimensionado e inicializar GLAD.
GLFWwindow* inicializarVentana(unsigned int ancho, unsigned int alto, const char* titulo) {
    // Inicializa la librería GLFW, que se encarga de crear la ventana,
    // gestionar el contexto OpenGL y leer eventos de teclado/ventana.
    glfwInit();

    // Se pide usar OpenGL 3.3.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Se pide el perfil Core Profile, es decir, OpenGL moderno sin funciones antiguas.
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Crea la ventana con el tamaño y título indicados desde el main.
    GLFWwindow* window = glfwCreateWindow(ancho, alto, titulo, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Error creando ventana" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Hace que el contexto OpenGL de esta ventana sea el contexto actual.
    // A partir de aquí, las llamadas de OpenGL actúan sobre esta ventana.
    glfwMakeContextCurrent(window);

    // Registra la función que se ejecutará cuando cambie el tamaño del framebuffer.
    // Esa función actualiza el viewport y recalcula la matriz projection.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Activa sincronización vertical.
    // Esto limita el refresco al de la pantalla y evita tearing.
    glfwSwapInterval(1);

    // Inicializa GLAD, que carga los punteros a las funciones modernas de OpenGL.
    // Sin esto, no podríamos usar correctamente funciones como glGenVertexArrays,
    // glUseProgram, glUniform, etc.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Error inicializando GLAD" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    // Devuelve la ventana al main para que pueda usarla en el bucle principal.
    return window;
}

// Esta función se llama en el main justo después de crear la ventana.
// Su objetivo es configurar estados globales de OpenGL que afectarán al renderizado.
void openGlInit() {
    // Valor con el que se limpia el depth buffer.
    // 1.0 representa la máxima profundidad, es decir, lo más lejano.
    glClearDepth(1.0f);

    // Color de fondo usado cuando se limpia la pantalla.
    // En este caso negro.
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Activa el test de profundidad.
    // Sirve para que los fragmentos más cercanos tapen a los que están detrás.
    // Sin esto, los objetos podrían dibujarse mal según el orden de dibujo.
    glEnable(GL_DEPTH_TEST);

    // Activa blending para poder usar transparencias.
    // Es necesario, por ejemplo, para texturas PNG con canal alfa como los arbustos.
    glEnable(GL_BLEND);

    // Define cómo se mezcla un fragmento transparente con lo que ya había detrás.
    // GL_SRC_ALPHA usa el alfa del fragmento actual.
    // GL_ONE_MINUS_SRC_ALPHA usa el complemento para el color de fondo.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Esta función se registra como callback en inicializarVentana.
// También la llama el main una vez al principio para inicializar projection
// con el tamaño inicial de la ventana.
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Define el área de la ventana donde OpenGL va a dibujar.
    // Si la ventana cambia de tamaño, el viewport debe actualizarse.
    glViewport(0, 0, width, height);

    // Recalcula la matriz de proyección en perspectiva.
    // Esta matriz transforma de view space a clip space.
    projection = glm::perspective(
        glm::radians(45.0f),              // Campo de visión vertical.
        (float)width / (float)height,     // Relación de aspecto.
        0.1f,                             // Plano cercano.
        100.0f                            // Plano lejano.
    );
}

// Permite que otros módulos obtengan la matriz projection si la necesitan.
// En este proyecto se usa sobre todo desde renderizarFrame al enviarla al shader.
glm::mat4 obtenerProjection() {
    return projection;
}

// Esta función se llama una vez en el main antes del bucle.
// Su objetivo es cargar todo lo que se va a necesitar para renderizar:
// geometrías, shaders y texturas.
void cargarRecursos(RecursosRender& recursos) {
    // Crea y configura los VAO/VBO del cubo.
    // El cubo se reutiliza para piezas de la grúa, suelo, arbustos, etc.
    crearCubo();

    // Crea y configura los VAO/VBO de la esfera.
    // La esfera se usa para ruedas, articulación, foco visible y fondo.
    crearEsfera();

    // Compila y enlaza los shaders de vértices y fragmentos.
    // El identificador resultante se guarda para usarlo al dibujar.
    recursos.shaderProgram = setShaders("shaders/shader.vert", "shaders/shader.frag");
}

//LA FUNCION PODEMOS REUTILIZARLA

// Esta función se llama en cada iteración del bucle principal del main.
// Su objetivo es dibujar un frame completo de la escena con el estado actual.
void renderizarFrame(/*const Grua& grua, */const EstadoEntrada& estado, const RecursosRender& recursos) {
    // Limpia el color buffer y el depth buffer.
    // Es decir, borra la imagen anterior y la información de profundidad anterior.
    // Cada frame se dibuja desde cero.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activa el programa de shaders que se usará para dibujar la escena.
    glUseProgram(recursos.shaderProgram);

    /*// Calcula la cámara actual en función de la posición/dirección de la grúa
    // y del modo de cámara seleccionado desde el input.
    DatosCamara camara = calcularCamara(grua, estado.modoCamara);

    // Envía al shader la matriz view, la matriz projection y la posición de cámara.
    // view/projection se usan en el vertex shader.
    // viewPos se usa en el fragment shader para la componente especular de la luz.
    configurarCamaraShader(camara, recursos.shaderProgram, projection);

    // Configura las luces de la escena en el shader.
    // Usa el estado actual de la grúa para colocar los focos:
    // uno frontal y otro en el brazo.
    // También tiene en cuenta si cada luz está encendida o apagada.
    configurarLucesGrua(grua, recursos.shaderProgram, estado.luzEncendidaFoco, estado.luzEncendidaBrazo);

    // Dibuja todos los elementos de la escena:
    // fondo, suelo, grúa, arbustos, focos visibles, etc.
    // Se le pasan las texturas cargadas y la posición de cámara para colocar el fondo.
    dibujarEscena(
        grua,
        recursos.shaderProgram,
        recursos.texturaSuelo,
        recursos.texturaArbusto,
        recursos.texturaGrua,
        recursos.texturaFondo,
        camara.camPos
    );*/
}

// Esta función se llama al salir del bucle principal del main.
// Su objetivo es liberar recursos antes de cerrar el programa.
void liberar(const RecursosRender& recursos){
    // Libera los recursos asociados a la esfera.
    liberarEsfera();

    // Libera los recursos asociados al cubo.
    liberarCubo();

    // Borra el programa de shaders de OpenGL.
    glDeleteProgram(recursos.shaderProgram);

    // Finaliza GLFW y cierra correctamente el contexto/ventana.
    glfwTerminate();
}
