#version 460 core

// SHADER FOR FONTS

layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 texturePosition;

out vec2 text_pos;

void main()
{
    text_pos = texturePosition;
    gl_Position = vec4(vertex, 0.6f, 1.0f);
}

