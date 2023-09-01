#include "bitmap.h"


#define __DE

void bitmap_s::circle(MVector2i pos, int radius, Color col, int pred)
{
    int x, y, i;
    for(; radius != pred; --radius)
    {
        for(i = 0; i < 360; ++i)
        {
            x = int((cosf(i) * radius) / 2);
            y = int((sinf(i) * radius) / 2);
            x += pos.x;
            y += pos.y;
            this->set_pixel(x, y, col);
        }
    }
}


bitmap_s::bitmap_s(int x, int y, uint8_t _levels)
{
    if (x < 0 || y < 0)
    {
        std::cerr << "[bitmap_s: invalid size constructor x or y]" << std::endl;
    }
    levels = _levels;
    width  = x;
    height = y;
    size   = x * y * levels;

    if(x == 0 || y == 0)
    {
        fprintf(stderr, "[bitmap_s] x == 0 || y == 0 return true\n");
        return;
    }

    buffer = (uint8_t*)calloc((size_t)size + 1, sizeof(uint8_t));
    if(buffer == nullptr)
    {
        fprintf(stderr, "[bitmap_s] buffer == nullptr\n");
        return;
    }
}



void bitmap_s::range(int x_s, int y_s, int x_e, int y_e, Color col)
{
    if (x_s < 0 || y_s < 0 || x_e < 0 || y_e < 0)
    {
        std::cerr << "[bitmap_s] low diap!" << std::endl;
        std::cerr << "[bitmap_s] x_s:" << x_s << "\ty_s:" << y_s << "\tx_e:" << x_e << "\ty_e:" << y_e << std::endl;
    }

    if(((width * y_s + x_s) * levels) > size || ((width * y_e + x_e) * levels) > size)
    {
    #ifdef __DE
        std::cerr << "[bitmap_s] range color width * y + x > size" << std::endl;
        std::cerr << "[bitmap_s] start: " << width * y_s + x_s << " > " << size << std::endl;
        std::cerr << "[bitmap_s] end  : " << (((long)width * y_e + x_e) * levels) << " > " << size << std::endl;
        std::cerr << "[bitmap_s] width:" << width << " height:" << height << " levels:" << levels << std::endl;
    #endif
        return;
    }

    int disa;
    for(; y_s < y_e; ++y_s)
    {
        for(disa = x_s; disa < x_e; ++disa)
        {
            //std::cout << "disa X:" << disa << "\tY:" << y_s << std::endl;
            //std::cout << (width * y_s + disa) * levels << std::endl;
            if(levels > 0)
                buffer[(width * y_s + disa) * levels] = col.r;
            if(levels > 1)
                buffer[(width * y_s + disa) * levels + 1] = col.g;
            if(levels > 2)
                buffer[(width * y_s + disa) * levels + 2] = col.b;
            if(levels > 3)
                buffer[(width * y_s + disa) * levels + 3] = col.a;
        }
    }
}

void bitmap_s::set_fill(Color col)
{
    const size_t siz = width * height;
    for(size_t i = 0; i < siz; ++i)
    {
        if(levels > 0)
            buffer[i * levels] = col.r;
        if(levels > 1)
            buffer[i * levels + 1] = col.g;
        if(levels > 2)
            buffer[i * levels + 2] = col.b;
        if(levels > 3)
            buffer[i * levels + 3] = col.a;
    }
}

void bitmap_s::set_pixel(int x, int y, Color col)
{
    if(((width * y + x) * levels) > size)
    {
    #ifdef __DE
        std::cerr << "[bitmap_s] set_pixel color width * y + x > size" << std::endl;
        std::cerr << "[bitmap_s] " << width << " * " << y << " + " << x << " * " << (int)levels << std::endl;
        std::cerr << "[bitmap_s] " << (int)((width * y + x) * levels) << " > " << size << std::endl;
        std::cerr << "[bitmap_s] width:" << width << " height:" << height << std::endl;
    #endif
        return;
    }
    if(levels > 0)
        buffer[(width * y + x) * levels    ] = col.r;
    if(levels > 1)
        buffer[(width * y + x) * levels + 1] = col.g;
    if(levels > 2)
        buffer[(width * y + x) * levels + 2] = col.b;
    if(levels > 3)
        buffer[(width * y + x) * levels + 3] = col.a;
}

void bitmap_s::set_pixel(int x, int y, uint8_t col)
{
    if(((width * y + x) * levels) > size)
    {
    #ifdef __DE
        std::cerr << "[bitmap_s] set_pixel width * y + x * levels > size" << std::endl;
        std::cerr << "[bitmap_s] " << width << " * " << y << " + " << x << " * " << (int)levels << std::endl;
        std::cerr << "[bitmap_s] " << (int)((width * y + x) * levels) << " > " << size << std::endl;
        std::cerr << "[bitmap_s] width:" << width << " height:" << height << std::endl;
        std::cerr << std::endl << std::endl;
    #endif
        return;
    }
    buffer[(width * y + x) * levels] = col;
}

Color bitmap_s::get_pixel(int x, int y)
{
    if(((width * y + x) * levels) > size)
    {
    #ifdef __DE
        std::cerr << "[bitmap_s] get_pixel width * y + x * levels > size" << std::endl;
        std::cerr << "[bitmap_s] " << width << " * " << y << " + " << x << " * " << (int)levels << std::endl;
        std::cerr << "[bitmap_s] " << (int)((width * y + x) * levels) << " > " << size << std::endl;
        std::cerr << "[bitmap_s] width:" << width << " height:" << height << std::endl;
        std::cerr << std::endl << std::endl;
    #endif
        return { 0.0f, 0.0f, 0.0f, 0.0f };
    }
    Color col;
    if(levels > 0)
        col.r = buffer[(width * y + x) * levels    ];
    if(levels > 1)
        col.g = buffer[(width * y + x) * levels + 1];
    if(levels > 2)
        col.b = buffer[(width * y + x) * levels + 2];
    if(levels > 3)
        col.a = buffer[(width * y + x) * levels + 3];

    return col;
}

bitmap_s::~bitmap_s()
{
    if(buffer != nullptr)
        free(buffer);
}


void bitmap_to_texture(bitmap_s* bitmap, GLuint& texture)
{
    if(bitmap->levels != 4)
    {
        std::cerr << "[bitmap_to_texture:bitmap.h:206]: bitmap invalid level count" << std::endl;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bitmap->width, bitmap->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);// GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);// GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);
}

int saveBMP(bitmap_s& surf, const char* filename)
{
    FILE *f;
    int filesize = 54 + 3*surf.width*surf.height;  //w is your image width, h is image height, both int
    const int w = surf.width, h = surf.height;

    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
    unsigned char bmppad[3] = {0,0,0};

    bmpfileheader[ 2] = (unsigned char)(filesize    );
    bmpfileheader[ 3] = (unsigned char)(filesize>> 8);
    bmpfileheader[ 4] = (unsigned char)(filesize>>16);
    bmpfileheader[ 5] = (unsigned char)(filesize>>24);

    bmpinfoheader[ 4] = (unsigned char)(       w    );
    bmpinfoheader[ 5] = (unsigned char)(       w>> 8);
    bmpinfoheader[ 6] = (unsigned char)(       w>>16);
    bmpinfoheader[ 7] = (unsigned char)(       w>>24);
    bmpinfoheader[ 8] = (unsigned char)(       h    );
    bmpinfoheader[ 9] = (unsigned char)(       h>> 8);
    bmpinfoheader[10] = (unsigned char)(       h>>16);
    bmpinfoheader[11] = (unsigned char)(       h>>24);

    f = fopen(filename,"wb");
    fwrite(bmpfileheader,1,14,f);
    fwrite(bmpinfoheader,1,40,f);
    for(int i=0; i<h; i++)
    {
        fwrite(surf.buffer+(w*(h-i-1)*3),3,w,f);
        fwrite(bmppad,1,(4-(w*3)%4)%4,f);
    }


    fclose(f);
    return 0;
}