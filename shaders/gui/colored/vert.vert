#version 460 core

// SHADER FOR COLORED RECTANGLES

layout (location = 0) in vec2 vertex;

void main()
{
    gl_Position = vec4(vertex, 0.6f, 1.0f);
}

