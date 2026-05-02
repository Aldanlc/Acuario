#include "luces.h"
#include "esfera.h"

#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "textura.h"

static void enviarVector3(GLuint shaderProgram, const std::string& nombreUniform, const glm::vec3& valor) {
    glUniform3f(glGetUniformLocation(shaderProgram, nombreUniform.c_str()), valor.x, valor.y, valor.z);
}

static void prepararColorFocoVisible(GLuint shaderProgram, const glm::vec3& color) {
    glUniform1i(glGetUniformLocation(shaderProgram, "usarTextura"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarTexturaSuelo"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarArbusto"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "usarColorUniform"), 1);
    glUniform1f(glGetUniformLocation(shaderProgram, "alphaUniform"), 1.0f);
    glUniform3f(glGetUniformLocation(shaderProgram, "colorUniform"), color.r, color.g, color.b);
}

static glm::mat4 crearMatrizFocoVisible(const FocoAcuario& foco) {
    glm::mat4 model = glm::mat4(1.0f);

    glm::vec3 direccion = glm::normalize(foco.direccion);

    float yaw = atan2(direccion.x, direccion.z);
    float longitudHorizontal = sqrt(direccion.x * direccion.x + direccion.z * direccion.z);
    float pitch = atan2(direccion.y, longitudHorizontal);

    model = glm::translate(model, foco.posicion);
    model = glm::rotate(model, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, -pitch, glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.14f));

    return model;
}

void configurarLucesAcuario(const Acuario& acuario, GLuint shaderProgram) {
    glUniform1i(glGetUniformLocation(shaderProgram, "numeroFocos"), NUM_FOCOS_ACUARIO);

    for (int i = 0; i < NUM_FOCOS_ACUARIO; i++) {
        std::string base = "focos[" + std::to_string(i) + "]";

        glUniform1i(glGetUniformLocation(shaderProgram, (base + ".encendido").c_str()), acuario.focos[i].encendido ? 1 : 0);
        glUniform1f(glGetUniformLocation(shaderProgram, (base + ".cutOff").c_str()), glm::cos(glm::radians(acuario.focos[i].cutOff)));
        glUniform1f(glGetUniformLocation(shaderProgram, (base + ".outerCutOff").c_str()), glm::cos(glm::radians(acuario.focos[i].outerCutOff)));

        enviarVector3(shaderProgram, base + ".posicion", acuario.focos[i].posicion);
        enviarVector3(shaderProgram, base + ".direccion", acuario.focos[i].direccion);
        enviarVector3(shaderProgram, base + ".color", acuario.focos[i].colorEncendido);
    }
}

void dibujarFocosAcuario(const Acuario& acuario, GLuint shaderProgram) {
    for (int i = 0; i < NUM_FOCOS_ACUARIO; i++) {
        glm::mat4 model = crearMatrizFocoVisible(acuario.focos[i]);
        glm::vec3 color = acuario.focos[i].encendido ? acuario.focos[i].colorEncendido : acuario.focos[i].colorApagado;

        prepararColorFocoVisible(shaderProgram, color);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        activarTextura(shaderProgram, 0);
        
        glBindVertexArray(VAO_esfera);
        glDrawArrays(GL_TRIANGLES, 0, verticesEsferaSize / (8 * sizeof(float)));
        glBindVertexArray(0);
    }
}