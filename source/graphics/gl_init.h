#ifndef __OPENGL_INIT_H_
#define __OPENGL_INIT_H_

#define GL_VERSION_4_3 1
#define GL_GLEXT_PROTOTYPES

#include <SDL2/SDL.h>

#ifdef __linux__
# include <GLES3/gl3.h>
# include <GL/gl.h>
# include <GL/glext.h>
#elif _WIN32
# define GLEW_STATIC
# include <GL/glew.h>
# include <GL/GL.h>
typedef char GLchar;
#endif

#include <SDL2/SDL_opengl.h>  
#include <stdio.h>

#define b_to_f(r) (float)r / 255



static GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        const char* error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM\nУстанавливается, если параметр перечисления не является допустимым."; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE\nУстанавливается, если параметр value не является допустимым."; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION\nУстанавливается, когда состояние команды не является допустимым для заданных параметров."; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW\nУстанавливается, когда операция отправки стека вызывает переполнение стека."; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW\nУстанавливает, когда операция выталкивания стека происходит, когда стек находится в самой низкой точке."; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY\nУстанавливается, когда операция выделения памяти не может выделить (достаточно) памяти."; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION\nПри чтении или записи задайте неполный буфер кадров."; break;
        }
        fprintf(stderr, "%s file %s line:%d\n", error, file, line);
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__) 

#endif //!__OPENGL_INIT_H_