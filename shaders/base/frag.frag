#version 460 core

in vec3 ourColor;
in vec2 ourCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform sampler2D ourTexture1;
uniform vec3 lightColor;

uniform vec3 viewPos;
uniform vec3 lightPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 



void main()
{
    // https://habr.com/ru/post/336166/
    // окружающий (ambient), рассеянный (diffuse) и бликовый (specular)
    //vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - FragPos);
    //float diff = max(dot(norm, lightDir), 0.0);
    //vec3 diffuse = diff * lightColor;

    //float specularStrength = 0.5f;
    //vec3 viewDir = normalize(viewPos - FragPos);
    //vec3 reflectDir = reflect(-lightDir, norm);

    //float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    //vec3 specular = specularStrength * spec * lightColor;



    // float ambientStrength = 0.1f;
    //float ambientStrength = 1.0f;
    //vec3 ambient = ambientStrength * lightColor;

    color = vec4(1.0, 1.0, 1.0, 1.0); //;vec4((ambient + diffuse + specular), 1.0f) * texture(ourTexture1, ourCoord);
}