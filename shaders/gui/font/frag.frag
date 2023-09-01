#version 460 core

// SHADER FOR FONSTS

in vec2 text_pos;

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    color = texture(ourTexture, text_pos);
}
