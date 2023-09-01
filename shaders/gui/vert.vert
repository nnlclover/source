#version 460 core

// SHADER FOR SIMPLE GUI

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 coordsTexture;

out vec2 tc;

void main()
{
    gl_Position = vec4(vertex, 0.5f, 1.0f);
    tc = coordsTexture;
}