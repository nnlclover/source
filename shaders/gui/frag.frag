#version 460 core

// SHADER FOR SIMPLE GUI

in vec2 tc;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    color = texture(ourTexture, tc);
}
