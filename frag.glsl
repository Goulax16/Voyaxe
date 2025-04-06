#version 330 core

out vec4 FragColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

// Nuevos uniforms para controlar el tipo de luz
uniform int lightType = 1; // 0 = point, 1 = directional, 2 = spot

// Parámetros del spot light
uniform float outerCone = 0.90f;
uniform float innerCone = 0.95f;
uniform vec3 lightDirection = vec3(0.0f, -1.0f, 0.0f); // Para directional y spot

vec4 calculatePointLight() {
    vec3 lightVec = lightPos - crntPos;
    float dist = length(lightVec);
    float a = 3.0;
    float b = 0.7;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    // Ambient
    float ambient = 0.20f;

    // Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightVec);
    float diffuse = max(dot(normal, lightDir), 0.0f);

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    float specular = spec * specularStrength;

    // Combine results
    vec4 diffuseTex = texture(diffuse0, texCoord);
    float specularTex = texture(specular0, texCoord).r;
    
    return (diffuseTex * (diffuse * inten + ambient) + specularTex * specular * inten) * lightColor;
}

vec4 calculateDirectionalLight() {
    // Ambient
    float ambient = 0.20f;

    // Diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(-lightDirection);
    float diffuse = max(dot(normal, lightDir), 0.0f);

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    float specular = spec * specularStrength;

    // Combine results
    vec4 diffuseTex = texture(diffuse0, texCoord);
    float specularTex = texture(specular0, texCoord).r;
    
    return (diffuseTex * (diffuse + ambient) + specularTex * specular) * lightColor;
}

vec4 calculateSpotLight() {
    vec3 lightVec = lightPos - crntPos;
    vec3 lightDir = normalize(lightVec);
    
    // Intensity based on angle
    float angle = dot(lightDirection, -lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    // Ambient
    float ambient = 0.20f;

    // Diffuse
    vec3 normal = normalize(Normal);
    float diffuse = max(dot(normal, lightDir), 0.0f);

    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    float specular = spec * specularStrength;

    // Combine results
    vec4 diffuseTex = texture(diffuse0, texCoord);
    float specularTex = texture(specular0, texCoord).r;
    
    return (diffuseTex * (diffuse * inten + ambient) + specularTex * specular * inten) * lightColor;
}

void main() {
    // Selección del tipo de luz
    switch(lightType) {
        case 0:
            FragColor = calculatePointLight();
            break;
        case 1:
            FragColor = calculateDirectionalLight();
            break;
        case 2:
            FragColor = calculateSpotLight();
            break;
        default:
            FragColor = vec4(color, 1.0f);
    }

    // Gamma correction
    float gamma = 2.2;
    FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
}