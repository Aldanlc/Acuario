#include "decoracion.h"
#include "textura.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Se usa el VAO del cubo para dibujar decoraciones planas con textura
extern GLuint VAO_CUBO;

static void dibujarCuboDecoracion(GLuint shaderProgram, const glm::mat4& model, GLuint textura) {
    // Mandamos al shader la matriz de transformación del objeto
    glUniformMatrix4fv(
        glGetUniformLocation(shaderProgram, "model"),
        1,
        GL_FALSE,
        glm::value_ptr(model)
    );

    // Configuración del shader para usar textura y color base blanco
    glUniform1i(glGetUniformLocation(shaderProgram, "usarTexturaSuelo"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarArbusto"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarColorUniform"), 1);
    glUniform1f(glGetUniformLocation(shaderProgram, "alphaUniform"), 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "colorUniform"), 1.0f, 1.0f, 1.0f);

    // Activamos la textura que se va a aplicar al cubo
    activarTextura(shaderProgram, textura);

    // Dibujamos el cubo usando su VAO
    glBindVertexArray(VAO_CUBO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void dibujarAlgas(const Acuario& acuario, GLuint shaderProgram, GLuint texturaAlga) {
    // Posiciones repartidas por el suelo del acuario
    glm::vec3 posiciones[] = {
        glm::vec3(-12.0f, 0.0f, -7.0f),
        glm::vec3(-8.0f,  0.0f,  5.0f),
        glm::vec3(-3.0f,  0.0f, -6.0f),
        glm::vec3(2.0f,   0.0f,  6.0f),
        glm::vec3(7.0f,   0.0f, -4.0f),
        glm::vec3(12.0f,  0.0f,  3.0f)
    };

    int numAlgas = 6;

    for (int i = 0; i < numAlgas; i++) {
        // Tamaño base de cada alga, con pequeñas variaciones
        float altura = 2.0f + 0.4f * (i % 3);
        float anchura = 2.0f + 0.2f * (i % 2);

        // Algunas algas se escalan más para que no sean todas iguales
        if (i == 1 || i == 4) {
            altura *= 1.8f;
            anchura *= 1.4f;
        }

        if (i == 5) {
            altura *= 2.2f;
            anchura *= 1.6f;
        }

        glm::vec3 posicion = acuario.centro + posiciones[i];

        // Ajustamos la altura para que la base del alga quede apoyada en el suelo
        posicion.y = acuario.limiteMin.y + altura * 0.5f;

        glm::mat4 model = glm::mat4(1.0f);

        // Trasladamos el cubo desde el origen hasta su posición dentro del acuario
        model = glm::translate(model, posicion);

        // Rotamos cada alga un poco sobre el eje Y para que no estén todas orientadas igual
        model = glm::rotate(model, glm::radians((float)(i * 25)), glm::vec3(0.0f, 1.0f, 0.0f));

        // Escalamos el cubo para convertirlo en un panel fino y alto
        model = glm::scale(model, glm::vec3(anchura, altura, 0.05f));

        dibujarCuboDecoracion(shaderProgram, model, texturaAlga);
    }

    // Desactivamos la textura al terminar
    activarTextura(shaderProgram, 0);
}

void dibujarCorales(const Acuario& acuario, GLuint shaderProgram, GLuint texturaCoral) {
    // Posiciones de los corales en distintas zonas del fondo
    glm::vec3 posiciones[] = {
        glm::vec3(-10.0f, 0.0f, -6.0f),
        glm::vec3(-5.0f,  0.0f,  7.0f),
        glm::vec3(4.0f,   0.0f, -7.0f),
        glm::vec3(10.0f,  0.0f,  5.0f)
    };

    int numCorales = 4;

    for (int i = 0; i < numCorales; i++) {
        // Tamaños distintos para que los corales no sean todos iguales
        float altura = 1.2f + 0.3f * (i % 3);
        float anchura = 1.2f + 0.2f * (i % 2);

        glm::vec3 posicion = acuario.centro + posiciones[i];

        // Colocamos el coral apoyado en el suelo
        posicion.y = acuario.limiteMin.y + altura * 0.5f;

        glm::mat4 model = glm::mat4(1.0f);

        // Movemos el coral a su posición final dentro del acuario
        model = glm::translate(model, posicion);

        // Rotamos cada coral sobre el eje Y para variar su orientación
        model = glm::rotate(model, glm::radians((float)(i * 35)), glm::vec3(0.0f, 1.0f, 0.0f));

        // Lo escalamos para que sea un panel vertical fino con la textura del coral
        model = glm::scale(model, glm::vec3(anchura, altura, 0.06f));

        dibujarCuboDecoracion(shaderProgram, model, texturaCoral);
    }

    // Desactivamos la textura al terminar
    activarTextura(shaderProgram, 0);
}