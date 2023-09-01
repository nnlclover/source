#version 430 core
out vec4 FragColor;

uniform sampler2D texture1;

in vec2 ourCoord;

void main()
{
    FragColor = texture(texture1, ourCoord);
} 