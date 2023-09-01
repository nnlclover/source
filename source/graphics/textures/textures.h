#ifndef __SKYBOX_H_
#define __SKYBOX_H_

#include <math/matrix_math.h>
#include <graphics/gl_init.h>
#include <array>
#include <cstring>
#include <SDL2/SDL_image.h>

namespace textures
{
    extern std::string defaultPath;

    class skybox
    {
    private:
    public:
        mm::Matrix model;

        unsigned int textureID;

        GLuint VAO, VBO;
        void loadGeometry();

        void LoadTexture(const char *path);
    };



	int StorageLoadTexture(const char* name, GLuint& texture);

    namespace bmp {
        int saveBMP(const char *filename, const unsigned int &w, const unsigned int &h, void *map);
    }

}

#endif //!__SKYBOX_H_