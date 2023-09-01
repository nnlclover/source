#include "textures.h"

namespace textures {



    int StorageLoadTexture(const char *name, GLuint &texture)
    {
        SDL_Surface *image = IMG_Load((defaultPath + name).c_str());
        if (image == nullptr)
        {
            std::cout << "[mesharch::loadtexture]::IMG_Load: " << IMG_GetError() << std::endl;
            return -1;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // Все предстоящие GL_TEXTURE_2D операции теперь влияют на наш текстурный объект
        // Настройка параметров текстур
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Установите для переноса текстур значение GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Установка фильтрования текстур
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Выбор типа формата данных

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 11);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);

        if (image->format->format == 386930691)
            // using for jpg format = 386930691
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, image->w, image->h, 0, GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
        else if (image->format->format == 376840196)
            // using for png format = 376840196
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
        else
            std::cout << "[loadtexture]: unknown format:" << image->format->format << std::endl;
        glGenerateMipmap(GL_TEXTURE_2D);
        SDL_FreeSurface(image);
        glBindTexture(GL_TEXTURE_2D, 0);

        return 0;
    }

}