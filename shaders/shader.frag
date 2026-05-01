#version 330 core

struct Foco {
    int encendido;
    vec3 posicion;
    vec3 direccion;
    vec3 color;
    float cutOff;
    float outerCutOff;
};

#define MAX_FOCOS 8

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 colorUniform;
uniform float alphaUniform;
uniform vec3 viewPos;

uniform int numeroFocos;
uniform Foco focos[MAX_FOCOS];

vec3 calcularSpotlight(vec3 norm, vec3 fragPos, vec3 viewDirection, Foco foco, vec3 colorBaseRGB) {
    vec3 lightDirection = normalize(foco.posicion - fragPos);

    float theta = dot(lightDirection, normalize(-foco.direccion));
    float epsilon = foco.cutOff - foco.outerCutOff;
    float intensidadFoco = clamp((theta - foco.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(norm, lightDirection), 0.0);
    vec3 diffuse = diff * foco.color * colorBaseRGB;

    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32.0);
    vec3 specular = 0.25 * spec * foco.color;

    float distancia = length(foco.posicion - fragPos);
    float atenuacion = 1.0 / (1.0 + 0.018 * distancia + 0.0025 * distancia * distancia);

    return (diffuse * 1.10 + specular * 0.75) * intensidadFoco * atenuacion;
}

void main() {
    float alpha = alphaUniform > 0.0 ? alphaUniform : 1.0;
    vec3 colorBase = colorUniform;

    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

    vec3 resultado = colorBase * 0.30;

    for (int i = 0; i < numeroFocos && i < MAX_FOCOS; i++) {
        if (focos[i].encendido == 1) {
            resultado += calcularSpotlight(norm, FragPos, viewDirection, focos[i], colorBase);
        }
    }

    FragColor = vec4(resultado, alpha);
}