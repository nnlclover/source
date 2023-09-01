#ifndef __BITMAP_H_
#define __BITMAP_H_

#include "vectors.h"
#include <stdint.h>
#include <memory>
#include <graphics/gl_init.h>
#include <iostream>
#include <math.h>
#include <cstring>
#include <cstring>
#include <cstring>

//#define __DE

class bitmap_s {
        public:
        uint8_t* buffer = nullptr;
        size_t size   = 0;
        int width  = 0;
        int height = 0;
        short levels = 2; // max count - 4

        void operator=(const bitmap_s& object)
        {
            if ((bitmap_s*)&object == nullptr)
            {
                std::cerr << "copiable object is null" << std::endl;
                return;
            }
            width = object.width;
            size = object.size;
            height = object.height;
            levels = object.levels;

            if (object.buffer != nullptr && object.size != 0)
            {
                buffer = (uint8_t*)calloc(object.size + 1, 1);
#ifdef _WIN32
                memcpy_s(this->buffer, object.size, object.buffer, object.size);
#elif __linux__
                memcpy(this->buffer, object.buffer, object.size);
#endif
            }
        }

        bitmap_s(int x, int y, uint8_t _levels);

        void circle(MVector2i pos, int radius, Color col, int pred = 0);

        void range(int x_s, int y_s, int x_e, int y_e, Color col);

        void set_fill(Color col);

        void set_pixel(int x, int y, Color col);

        void set_pixel(int x, int y, uint8_t col);

        Color get_pixel(int x, int y);

        ~bitmap_s();
    };




void bitmap_to_texture(bitmap_s* bitmap, GLuint& texture);

int saveBMP(bitmap_s& surf, const char* filename);

static void GLtextureToBitmap(bitmap_s& surf, int texture)
{
    glGetTexImage(texture,
 	0,
 	GL_RGB,
 	GL_UNSIGNED_BYTE,
 	surf.buffer);
}


#endif
