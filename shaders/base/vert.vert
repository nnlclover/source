#version 460 core
layout (location = 0) in vec3 position; // Устанавливаем позиция переменной с координатами в 0
//layout (location = 0) in vec2 texCoord;
//layout (location = 2) in vec3 normal;

out vec3 ourColor; // Передаем цвет во фрагментный шейдер
out vec2 ourCoord;

out vec3 FragPos;  
out vec3 Normal;

uniform mat4 transform;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    //gl_Position = projection * view * model * (transform * vec4(position, 1.0f)); // projection * view * model * vec4(position, 1.0f);
	gl_Position = transform * vec4(position, 1.0f); 
    FragPos = vec3(transform * vec4(position, 1.0f));
    //ourCoord = vec2(texCoord.x, 1.0f - texCoord.y);
    //Normal = mat3(transpose(inverse(transform))) * normal;
}