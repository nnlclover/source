#version 430 core
uniform samplerCube skybox;
in vec3 TexCoords;
out vec4 FragColor;

void main()
{
    FragColor = texture(skybox, TexCoords);
} 