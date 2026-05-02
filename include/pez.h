#ifndef PEZ_H
#define PEZ_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "acuario.h"

struct PiezaPez {
    glm::vec3 posicionLocal;
    glm::vec3 escala;
    glm::vec3 color;
    GLuint textura_pez;
};

struct Pez {
    glm::vec3 posicion;
    glm::vec3 direccion;
    glm::vec3 color;
    float velocidad;
    float escala;
    float faseMovimiento;

    PiezaPez cuerpo;
    PiezaPez cabeza;
    PiezaPez cola;
    PiezaPez aletaIzquierda;
    PiezaPez aletaDerecha;
    PiezaPez ojoIzquierdo;
    PiezaPez ojoDerecho;
};

void inicializarPez(Pez& pez, const glm::vec3& posicion, const glm::vec3& direccion, const glm::vec3& color, float velocidad, float escala);
void inicializarPeces(Pez peces[], int numeroPeces, const Acuario& acuario);
void actualizarPez(Pez& pez, const Acuario& acuario, float deltaTime);
void actualizarPeces(Pez peces[], int numeroPeces, const Acuario& acuario, float deltaTime);
void dibujarPez(const Pez& pez, GLuint shaderProgram);
void dibujarPeces(const Pez peces[], int numeroPeces, GLuint shaderProgram);

#endif