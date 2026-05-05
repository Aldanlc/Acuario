#include "textura.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

GLuint cargarTextura(const char* ruta) {
    GLuint texturaID;
    glGenTextures(1, &texturaID);

    int ancho, alto, nrChannels;

    // Invertimos la imagen verticalmente
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(ruta, &ancho, &alto, &nrChannels, 0);

    if (data) {
        GLenum formato;

        // Elegimos el formato según número de canales (RGB / RGBA)
        if (nrChannels == 1)
            formato = GL_RED;
        else if (nrChannels == 3)
            formato = GL_RGB;
        else if (nrChannels == 4)
            formato = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texturaID);

        // Subimos la textura a la GPU
        glTexImage2D(GL_TEXTURE_2D, 0, formato, ancho, alto, 0, formato, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // Parámetros de repetición y filtrado
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {
        std::cout << "Error al cargar la textura: " << ruta << std::endl;
    }

    stbi_image_free(data);

    return texturaID;
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

void eliminarTextura(GLuint& textura) {
    if (textura != 0) {
        glDeleteTextures(1, &textura);
        textura = 0;
    }
}