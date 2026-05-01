#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform bool usarColorUniform;
uniform bool usarTextura;
uniform bool usarTexturaSuelo;
uniform bool usarArbusto;

uniform vec3 colorUniform;
uniform float alphaUniform;

uniform sampler2D textura1;

void main() {
    float alpha = alphaUniform > 0.0 ? alphaUniform : 1.0;
    vec4 colorBase = vec4(0.8, 0.8, 0.8, alpha);

    if (usarColorUniform) {
        colorBase = vec4(colorUniform, alpha);
    }

    if (usarTextura || usarTexturaSuelo || usarArbusto) {
        colorBase = texture(textura1, TexCoord);
    }

    FragColor = colorBase;
}