#ifndef __SHADER_H_
#define __SHADER_H_

#include <fstream>
#include <stdio.h>
#include "gl_init.h"
#include <map>
#include <iostream>

namespace lfd 
{
    int buildInCompiler(const char** frag, const char** vert, int frag_len, int vert_len);
    int Shader(const char* name, const char* frag_path, const char* vert_path);

    class ShaderControl
    {
        public:
        GLuint s_vertex, s_fragment, program;
        std::string name;


        enum shader_types{
            vertex = 1,
            fragment = 2,
        };

        GLint letShaderErrorState(const shader_types shader_num)
        {
            GLuint shader;
            const char* name;
            switch(shader_num)
            {
                case shader_types::vertex:
                    shader = s_vertex;
                    name = "VERTEX";
                break;

                case shader_types::fragment:
                    shader = s_fragment;
                    name = "FRAGMENT";
                break;

                default:
                    return 0;
            }
            
            GLint success;
            char infoLog[512];
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                printf("ERROR::SHADER::%s::COMPILATION_FAILED:\n %s\n", name, infoLog);
            };
            return success;
        }

        //-------------------------------------------------------------------------------
        // Load custom shader type from buffer
        //-------------------------------------------------------------------------------
        int loadCustom(const shader_types shader_type, const char** buf, const int length)
        {
            GLuint *edit_shader;
            GLenum type;
            switch(shader_type)
            {
                case shader_types::vertex:
                    edit_shader = &s_vertex;
                    type = GL_VERTEX_SHADER;
                break;
                
                case shader_types::fragment:
                    edit_shader = &s_fragment;
                    type = GL_FRAGMENT_SHADER;
                break;
                
                default:
                    return -2;
            }

            if(*edit_shader != 0)
                glDeleteShader(*edit_shader);

            *edit_shader = glCreateShader(type);
            glShaderSource(*edit_shader, length, buf, NULL);
            glCompileShader(*edit_shader);
            if(!letShaderErrorState(shader_type))
                return -1;

            return 0;
        }
        //-------------------------------------------------------------------------------
        // Link all shaders to one program
        //-------------------------------------------------------------------------------
        int link()
        {
            program = glCreateProgram();
            glAttachShader(program, vertex);
            glAttachShader(program, fragment);
            glLinkProgram(program);
            //Если есть ошибки - вывести их
            char infoLog[512];
            GLint success;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(program, 512, NULL, infoLog);
                std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED" << std::endl << infoLog << std::endl;
                return -1;
            }
            return 0;
        }
        //-------------------------------------------------------------------------------
        // Return compiled shader in binary data
        //-------------------------------------------------------------------------------
        size_t exportBinary(GLubyte** data)
        {
            GLint length;
            glGetProgramiv(program, GL_PROGRAM_BINARY_LENGTH, &length);
            if (length > 0)
            {
                *data = new GLubyte[length];
                GLenum format;
                glGetProgramBinary(program, length, nullptr, &format, data);
            }
            return 0;
        }
        //-------------------------------------------------------------------------------
        // Save compiled shader to binary file
        //-------------------------------------------------------------------------------
        int saveBinary(const char* file_path)
        {
            GLubyte* data;
            const GLint length = exportBinary(&data);
            if(!data)
                return -1;
            ::std::ofstream file(file_path, ::std::ios::binary);
            if(!file.is_open())
                return -2;
            file.write(reinterpret_cast<const char*>(data), length);
            delete[] data;
            return 0;
        }
        //-------------------------------------------------------------------------------
        // Load binary compiled shader from custom buffer
        //-------------------------------------------------------------------------------
        int loadCompiled(const GLubyte* source, const int length)
        {
            GLuint binaryProgram = glCreateProgram();
            glProgramBinary(binaryProgram, GL_PROGRAM_BINARY_FORMAT_MESA, source, length);

            // Детали компиляции шейдера на основе бинарных данных

            glDeleteProgram(binaryProgram);

            return true;
        }
        //-------------------------------------------------------------------------------
        // Load Compiled shader program from binary file
        //-------------------------------------------------------------------------------
        int loadCompiledFile(const char* file_path)
        {
            std::ifstream file(file_path, std::ios::binary);
            if (!file)
            {
                return false;
            }

            file.seekg(0, std::ios::end);
            GLint length = file.tellg();
            file.seekg(0, std::ios::beg);

            GLubyte* binaryData = new GLubyte[length];
            file.read(reinterpret_cast<char*>(binaryData), length);
            file.close();

            delete[] binaryData;

        }
    };
}

#endif // !__SHADER_H_
