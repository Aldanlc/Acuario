#include "textura.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint cargarTextura(const char* nombre_textura) {

    GLuint textura;
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int anchura, altura, nrChannels;
    unsigned char* datos = stbi_load(nombre_textura, &anchura, &altura, &nrChannels, 0);

    if (datos == nullptr) {
        std::cerr << "Error al cargar la textura: " << nombre_textura << std::endl;
        glBindTexture(GL_TEXTURE_2D, 0);
        return 0;
    }

    GLenum format;

    switch (nrChannels) {
        case 1:
            format = GL_RED;
            break;

        case 3:
            format = GL_RGB;
            break;

        case 4:
            format = GL_RGBA;
            break;

        default:
            std::cerr << "Formato de textura no soportado: " << nombre_textura
                      << " (" << nrChannels << " canales)" << std::endl;
            stbi_image_free(datos);
            glBindTexture(GL_TEXTURE_2D, 0);
            return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, anchura, altura, 0, format, GL_UNSIGNED_BYTE, datos);

    stbi_image_free(datos);

    glBindTexture(GL_TEXTURE_2D, 0);

    return textura;
}

void eliminarTextura(GLuint& textura) {
    if (textura != 0) {
        glDeleteTextures(1, &textura);
        textura = 0;
    }
}

void activarTextura(GLuint shaderProgram, GLuint textura) {
    if (textura == 0) {
        glUniform1i(glGetUniformLocation(shaderProgram, "usarTextura"), GL_FALSE);
        glBindTexture(GL_TEXTURE_2D, 0);
        return;
    }

    glUniform1i(glGetUniformLocation(shaderProgram, "usarTextura"), GL_TRUE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textura);
    glUniform1i(glGetUniformLocation(shaderProgram, "textura1"), 0);
}