#ifndef __OBJLOAD_H_
#define __OBJLOAD_H_

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <geometry/Mesh.h>

class loadObj
{
    public:
    std::vector<Mesh*> meshes;

    int load(const char* file)
    {
        std::ifstream fp(file);
        std::string line;

        if(!fp.is_open())
        {
            std::cerr << "loadObj->load()->Failed open file: " << file<< std::endl;
            return -1;
        }

        float reg_a, reg_b, reg_c;
        std::uint32_t reg_d, reg_e, reg_f;
        std::string type;
        Mesh* ctxMesh = nullptr;
        long last = 0;

        //ctxMesh = new Mesh;
        while(getline(fp, line))
        {
            std::stringstream iss(line);
            iss >> type;

            if(type == "v")
            {
                if(ctxMesh == nullptr)
                    continue;
                iss >> reg_a >> reg_b >> reg_c;
                //std::cout << reg_a << " " << reg_b << " " << reg_c << std::endl;
                //reg_a *= 10;
                //reg_b *= 10;
                //reg_c *= 10;
                //ctxMesh->vertices.push_back(reg_a);
                //ctxMesh->vertices.push_back(reg_b);
                //ctxMesh->vertices.push_back(reg_c);
                ctxMesh->AddVertex(reg_a, reg_b, reg_c);
            }
            else if(type == "f")
            {
                if(ctxMesh == nullptr)
                    continue;
                iss >> reg_d >> reg_e >> reg_f;

                reg_d -= 1;
                reg_e -= 1;
                reg_f -= 1;

                //ctxMesh->indices.push_back(reg_d);
                //ctxMesh->indices.push_back(reg_e);
                //ctxMesh->indices.push_back(reg_f);
                ctxMesh->addPolygon(reg_d, reg_e, reg_f);
            }
            else if(type == "o")
            {
                std::string name;
                iss >> name;
                //std::cout << name << std::endl;
                ctxMesh = new Mesh;
                meshes.push_back(ctxMesh);
            }
        }

        fp.close();
        return 0;
    }
};



int exportObj(const std::vector<Mesh*>& meshes, const char* filename)
{
    std::ofstream file(filename);
    size_t i;
    for(const auto& mesh : meshes)
    {
        file << "o " << rand() << std::endl;
        for(i = 0; i < mesh->vertices.size() / 3; ++i)
        {
            file << "v " << mesh->vertices[i * 3] << " " << mesh->vertices[i * 3 + 1] << " " << mesh->vertices[i * 3 + 2] << std::endl;
        }
        for(i = 0; i < mesh->indices.size() / 3; ++i)
        {
            file << "f " << mesh->indices[i * 3] + 1 << " " << mesh->indices[i * 3 + 1] + 1 << " " << mesh->indices[i * 3 + 2] + 1 << std::endl;
        }
    }


    file.close();
    return 0;
}



#endif //!__OBJLOAD_H_