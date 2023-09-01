#include "shader.h"
#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>

namespace lfd
{
    /*
    struct shaderPair {
        
        int load()
        {

        }
    };*/
    int buildInCompiler(const char** frag, const char** vert, int frag_len, int vert_len)
    {
        GLuint vertex, fragment, Program;
        GLint success;
        GLchar infoLog[512];

        // Вершинный шейдер
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, vert_len, vert, NULL);
        glCompileShader(vertex);
        // Если есть ошибки - вывести их
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << vert << std::endl << infoLog << std::endl;
        };
        // Аналогично для фрагментного шейдера
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, frag_len, frag, NULL);
        glCompileShader(fragment);
        // Если есть ошибки - вывести их
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << frag << std::endl << infoLog << std::endl;
        };

        Program = glCreateProgram();
        glAttachShader(Program, vertex);
        glAttachShader(Program, fragment);
        glLinkProgram(Program);
        //Если есть ошибки - вывести их
        glGetProgramiv(Program, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED vert:" << vert << "\t frag:" << frag << std::endl << infoLog << std::endl;
            std::exit(-1);
        }

        // Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
        glDeleteShader(vertex);
        glDeleteShader(fragment);

        return Program;
    }

    int Shader(const char* name, const char* frag_path, const char* vert_path)
    {
        std::ifstream fr;
        size_t len = 0;
        char* fShaderCode = nullptr;
        char* vShaderCode = nullptr;
        GLuint Program;
        int vert_len, frag_len;
    
        fr.open(frag_path);
        if(fr.is_open())
        {
            fr.seekg(0, std::ios::end);
            vert_len = fr.tellg();
            fr.seekg(0, std::ios::beg);
            fShaderCode = new char[vert_len + 1];
            memset(fShaderCode, 0, vert_len + 1);
            fr.read(fShaderCode, vert_len);
        }
        else 
        {
            std::cerr << "SHADER FILE NOT OPEN! PATH:" << frag_path << std::endl;
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR SHADER NOT FOUND", (std::string("shader path:") + std::string(frag_path)).c_str(), 0);
            std::exit(-1);
        }
        fr.close();
    
        fr.open(vert_path);
        if(fr.is_open())
        {
            fr.seekg(0, std::ios::end);
            frag_len = fr.tellg();
            fr.seekg(0, std::ios::beg);
            vShaderCode = new char[frag_len + 1];
            memset(vShaderCode, 0, frag_len + 1);
            fr.read(vShaderCode, frag_len);
        }
        else 
        {
            std::cerr << "SHADER FILE NOT OPEN! PATH:" << vert_path << std::endl;
            SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "ERROR SHADER NOT FOUND", (std::string("shader path:") + std::string(vert_path)).c_str(), 0);
            std::exit(-1);
        }
        fr.close();
        
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];
        
        // Вершинный шейдер
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        // Если есть ошибки - вывести их
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(vertex, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED: " << vert_path << std::endl << infoLog << std::endl;
        };
        // Аналогично для фрагментного шейдера
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        // Если есть ошибки - вывести их
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(fragment, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: " << frag_path << std::endl << infoLog << std::endl;
        };
    
        Program = glCreateProgram();
        glAttachShader(Program, vertex);
        glAttachShader(Program, fragment);
        glLinkProgram(Program);
        //Если есть ошибки - вывести их
        glGetProgramiv(Program, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(Program, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED vert:" << vert_path << "\t frag:" << frag_path << std::endl << infoLog << std::endl;
            std::exit(-1);
        }
    
        // Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    
        delete [] fShaderCode;
        delete [] vShaderCode;
        
        // shader_register.insert({ name, Program });
        return Program;
    }
}
