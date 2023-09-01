#include "gl_init.h"

struct bufman {
    GLuint VAO = 0, VBO = 0, EBO = 0;
    bool gened = false;

    void gen()
    {
        del();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        if(!gened)
            gened = true;
    }

    void del()
    {
        if(!gened)
            return;
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteVertexArrays(1, &VAO);
    }

    ~bufman()
    {
        del();
    }
};




struct texman {
    bool gened = false;
    GLuint texture = 0;

    void gen()
    {
        del();

        glGenTextures(1, &texture);
        // std::cout << "gened texture:" << texture << std::endl;
        if(!gened)
            gened = true;
    }

    void del()
    {
        if(!gened)
            return;
        glDeleteTextures(1, &texture);
    }

    ~texman()
    {
        del();
    }
};
