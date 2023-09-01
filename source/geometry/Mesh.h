#ifndef __MESH_H_
#define __MESH_H_

#include <vector>
#include <iostream>
#include <math.h>
#include <vectors.h>


static MVector3f MVmin(const MVector3f& first, const MVector3f& second)
{
    return {
        std::min(first.x, second.x),
        std::min(first.y, second.y),
        std::min(first.z, second.z)
    };;
}

static MVector3f MVmax(const MVector3f& first, const MVector3f& second)
{
    return {
        std::max(first.x, second.x),
        std::max(first.y, second.y),
        std::max(first.z, second.z)
    };;
}



class Mesh {
public:
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<uint32_t> indices; // Индексы вершин для полигонов (только для треугольников)
    std::vector<bool> deletedPolygons; // Флаги для пометки удаленных полигонов
    MVector3f AABB_min;
    MVector3f AABB_max;

    void mathAABB()
    {        
        const size_t count = vertices.size() / 3;
        for(size_t i = 0; i < count; ++i)
        {
            MVector3f vec {vertices[i * 3], vertices[i * 3 + 1], vertices[i * 3 + 2]};
            AABB_min = MVmin(vec, AABB_min);
            AABB_max = MVmax(vec, AABB_max);
        }
    }

    Mesh(const std::vector<float>& verticesData, const std::vector<unsigned int>& indicesData)
        : vertices(verticesData), indices(indicesData), deletedPolygons(indicesData.size() / 3, false) {}

    Mesh()
    { }

  

    void AddVertex(double x, double y, double z)
    {
        vertices.reserve(3);
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    uint32_t addPolygon(uint32_t f, uint32_t s, uint32_t t)
    {
        indices.reserve(3);
        indices.push_back(f);
        indices.push_back(s);
        indices.push_back(t);
        return indices.size() - 1;
    }

    // Геттеры для получения данных о меше
    const std::vector<float>& getVertices() const {
        return vertices;
    }

    const std::vector<unsigned int>& getIndices() const {
        return indices;
    }

    

    // Метод для удаления полигона по индексу
    void removePolygon(unsigned int index) {
        if (index * 3 + 2 < indices.size()) {
            // Удаляем индексы вершин для данного полигона
            indices.erase(indices.begin() + index * 3, indices.begin() + index * 3 + 3);
            // Помечаем полигон как удаленный
            deletedPolygons[index] = true;
        }
    }

    // Вложенный класс для итерации по полигонам
    class PolygonIterator {
    private:
        const Mesh& mesh;
        unsigned int currentIndex;

    public:
        PolygonIterator(const Mesh& m, unsigned int index = 0) : mesh(m), currentIndex(index) {}

        bool operator!=(const PolygonIterator& other) const {
            return currentIndex != other.currentIndex;
        }

        void operator++() {
            currentIndex += 3; // Переход к следующему полигону (3 индекса на полигон)
        }

        unsigned int operator*() const {
            return currentIndex;
        }
    };

    // ... (прежнее содержимое класса)

    // Метод для получения итератора на начало полигонов
    PolygonIterator begin() const {
        return PolygonIterator(*this);
    }

    // Метод для получения итератора на конец полигонов
    PolygonIterator end() const {
        return PolygonIterator(*this, indices.size());
    }

    // Вложенный класс для итерации по вершинам
    class VertexIterator {
    private:
        const Mesh& mesh;
        unsigned int currentIndex;

    public:
        VertexIterator(const Mesh& m, unsigned int index = 0) : mesh(m), currentIndex(index) {}

        bool operator!=(const VertexIterator& other) const {
            return currentIndex != other.currentIndex;
        }

        void operator++() {
            currentIndex += 1; // Переход к следующей вершине
        }

        unsigned int operator*() const {
            return mesh.indices[currentIndex]; // Возвращаем индекс вершины в массиве вершин
        }
    };

    // Метод для получения итератора на начало вершин полигона
    VertexIterator vertexBegin(unsigned int polygonIndex) const {
        return VertexIterator(*this, polygonIndex * 3);
    }

    // Метод для получения итератора на конец вершин полигона
    VertexIterator vertexEnd(unsigned int polygonIndex) const {
        return VertexIterator(*this, polygonIndex * 3 + 3);
    }



};





#endif //!__MESH_H_
