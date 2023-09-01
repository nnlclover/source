#include "Mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <math/matrix_math.h>
#include "mesh_objects.h"



Mesh generateConeMesh(float height, float radius, int num_segments)
{
	Mesh meshData;
	// Генерируем вершины для конуса
	// Генерируем вершины для боковой поверхности конуса
	for (unsigned int i = 0; i < num_segments; ++i)
	{
		float angle = static_cast<float>(i) / static_cast<float>(num_segments) * 2.0f * mm::Pi;
		float x = radius * cos(angle);
		float z = radius * sin(angle);

		meshData.vertices.push_back(x);
		meshData.vertices.push_back(0.0f); // y-координата на основании
		meshData.vertices.push_back(z);
	}

	// Добавляем центральную вершину сверху
	meshData.vertices.push_back(0.0f);
	meshData.vertices.push_back(height); // y-координата сверху
	meshData.vertices.push_back(0.0f);

	// Генерируем индексы для боковой поверхности конуса
	for (unsigned int i = 0; i < num_segments; ++i)
	{
		meshData.indices.push_back(i);
		meshData.indices.push_back((i + 1) % num_segments);
		meshData.indices.push_back(num_segments); // Индекс центральной вершины
	}

	// Генерируем вершины для дна конуса
	for (unsigned int i = 0; i < num_segments; ++i)
	{
		float angle = static_cast<float>(i) / static_cast<float>(num_segments) * 2.0f * mm::Pi;
		float x = radius * cos(angle);
		float z = radius * sin(angle);

		meshData.vertices.push_back(x);
		meshData.vertices.push_back(0.0f); // y-координата на дне
		meshData.vertices.push_back(z);
	}

	// Добавляем центральную вершину дна
	meshData.vertices.push_back(0.0f);
	meshData.vertices.push_back(0.0f); // y-координата дна
	meshData.vertices.push_back(0.0f);

	// Генерируем индексы для дна конуса
	unsigned int dno_index = num_segments * 2; // Индекс центральной вершины на дне

	for (unsigned int i = 0; i < num_segments; ++i)
	{
		meshData.indices.push_back((i + 1) % num_segments + num_segments);
		meshData.indices.push_back(i + num_segments);
		meshData.indices.push_back(dno_index);
	}

	return meshData;
}

Mesh generateSphere(float radius, unsigned int numRings, unsigned int numSegments)
{
	Mesh meshData;

	float ringAngleStep = mm::Pi / static_cast<float>(numRings);
	float segmentAngleStep = 2.0f * mm::Pi / static_cast<float>(numSegments);

	// Генерируем вершины сферы
	for (unsigned int ring = 0; ring <= numRings; ++ring)
	{
		float phi = ring * ringAngleStep;
		for (unsigned int segment = 0; segment <= numSegments; ++segment)
		{
			float theta = segment * segmentAngleStep;

			float x = radius * sin(phi) * cos(theta);
			float y = radius * cos(phi);
			float z = radius * sin(phi) * sin(theta);

			meshData.vertices.push_back(x);
			meshData.vertices.push_back(y);
			meshData.vertices.push_back(z);

			float u = static_cast<float>(segment) / static_cast<float>(numSegments);
			float v = static_cast<float>(ring) / static_cast<float>(numRings);

			meshData.uvs.push_back(u);
			meshData.uvs.push_back(v);
		}
	}

	// Генерируем индексы для сферы
	for (unsigned int ring = 0; ring < numRings; ++ring)
	{
		for (unsigned int segment = 0; segment < numSegments; ++segment)
		{
			unsigned int currentIdx = ring * (numSegments + 1) + segment;
			unsigned int nextIdx = currentIdx + numSegments + 1;

			meshData.indices.push_back(currentIdx);
			meshData.indices.push_back(nextIdx);
			meshData.indices.push_back(currentIdx + 1);

			meshData.indices.push_back(nextIdx);
			meshData.indices.push_back(nextIdx + 1);
			meshData.indices.push_back(currentIdx + 1);
		}
	}

	return meshData;
}

Mesh generateTorus(float outerRadius, float innerRadius, unsigned int numSegments, unsigned int numRings)
{
	Mesh meshData;

	float sectorStep = 2.0f * M_PI / numSegments;
	float ringStep = 2.0f * M_PI / numRings;

	for (unsigned int i = 0; i <= numRings; ++i)
	{
		float phi = i * ringStep;
		float cosPhi = cos(phi);
		float sinPhi = sin(phi);

		for (unsigned int j = 0; j <= numSegments; ++j)
		{
			float theta = j * sectorStep;
			float cosTheta = cos(theta);
			float sinTheta = sin(theta);

			float x = (outerRadius + innerRadius * cosTheta) * cosPhi;
			float y = (outerRadius + innerRadius * cosTheta) * sinPhi;
			float z = innerRadius * sinTheta;

			meshData.vertices.push_back(x);
			meshData.vertices.push_back(y);
			meshData.vertices.push_back(z);

			float u = static_cast<float>(j) / static_cast<float>(numSegments);
			float v = static_cast<float>(i) / static_cast<float>(numRings);

			meshData.uvs.push_back(u);
			meshData.uvs.push_back(v);
		}
	}

	for (unsigned int i = 0; i < numRings; ++i)
	{
		for (unsigned int j = 0; j < numSegments; ++j)
		{
			unsigned int currentRow = i * (numSegments + 1);
			unsigned int nextRow = (i + 1) * (numSegments + 1);

			meshData.indices.push_back(currentRow + j);
			meshData.indices.push_back(nextRow + j);
			meshData.indices.push_back(currentRow + j + 1);

			meshData.indices.push_back(currentRow + j + 1);
			meshData.indices.push_back(nextRow + j);
			meshData.indices.push_back(nextRow + j + 1);
		}
	}

	return meshData;
}

Mesh generateTorusPart(float outerRadius, float innerRadius, float length, unsigned int numSegments, unsigned int numRings) 
{
    Mesh meshData;

    float sectorStep = 2.0f * M_PI / numSegments;
    float ringStep = length / numRings;

    for (unsigned int i = 0; i <= numRings; ++i) {
        float phi = i * ringStep;
        float cosPhi = cos(phi);
        float sinPhi = sin(phi);

        for (unsigned int j = 0; j <= numSegments; ++j) {
            float theta = j * sectorStep;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            float x = (outerRadius + innerRadius * cosTheta) * cosPhi;
            float y = (outerRadius + innerRadius * cosTheta) * sinPhi;
            float z = innerRadius * sinTheta;

            meshData.vertices.push_back(x);
            meshData.vertices.push_back(y);
            meshData.vertices.push_back(z);

            float u = static_cast<float>(j) / static_cast<float>(numSegments);
            float v = static_cast<float>(i) / static_cast<float>(numRings);

            meshData.uvs.push_back(u);
            meshData.uvs.push_back(v);
        }
    }

    for (unsigned int i = 0; i < numRings; ++i) {
        for (unsigned int j = 0; j < numSegments; ++j) {
            unsigned int currentRow = i * (numSegments + 1);
            unsigned int nextRow = (i + 1) * (numSegments + 1);

            meshData.indices.push_back(currentRow + j);
            meshData.indices.push_back(nextRow + j);
            meshData.indices.push_back(currentRow + j + 1);

            meshData.indices.push_back(currentRow + j + 1);
            meshData.indices.push_back(nextRow + j);
            meshData.indices.push_back(nextRow + j + 1);
        }
    }

    return meshData;
}

Mesh generateTube(float outerRadius, float innerRadius, float length, unsigned int numSegments) 
{
	Mesh meshData;

    float sectorStep = 2.0f * M_PI / numSegments;

    for (unsigned int i = 0; i <= numSegments; ++i) {
        float theta = i * sectorStep;
        float cosTheta = cos(theta);
        float sinTheta = sin(theta);

        // Внешний радиус
        float x1 = outerRadius * cosTheta;
        float y1 = outerRadius * sinTheta;

        // Внутренний радиус
        float x2 = innerRadius * cosTheta;
        float y2 = innerRadius * sinTheta;

        // Вершины для внешнего радиуса
        meshData.vertices.push_back(x1);
        meshData.vertices.push_back(y1);
        meshData.vertices.push_back(length / 2.0f);

        meshData.vertices.push_back(x1);
        meshData.vertices.push_back(y1);
        meshData.vertices.push_back(-length / 2.0f);

        // Вершины для внутреннего радиуса
        meshData.vertices.push_back(x2);
        meshData.vertices.push_back(y2);
        meshData.vertices.push_back(length / 2.0f);

        meshData.vertices.push_back(x2);
        meshData.vertices.push_back(y2);
        meshData.vertices.push_back(-length / 2.0f);

        float u = static_cast<float>(i) / static_cast<float>(numSegments);
    	meshData.uvs.push_back(u);
    	meshData.uvs.push_back(1.0f);

    	meshData.uvs.push_back(u);
    	meshData.uvs.push_back(0.0f);

    	// Внутренний радиус
    	meshData.uvs.push_back(u);
    	meshData.uvs.push_back(1.0f);

    	meshData.uvs.push_back(u);
    	meshData.uvs.push_back(0.0f);
    }




    for (unsigned int i = 0; i < numSegments; ++i) {
        unsigned int currentRow = i * 4;
        unsigned int nextRow = (i + 1) * 4;

        // Верхние полигоны
        meshData.indices.push_back(currentRow);
        meshData.indices.push_back(nextRow);
        meshData.indices.push_back(nextRow + 1);

        meshData.indices.push_back(currentRow);
        meshData.indices.push_back(nextRow + 1);
        meshData.indices.push_back(currentRow + 1);

        // Нижние полигоны
        meshData.indices.push_back(currentRow + 2);
        meshData.indices.push_back(nextRow + 2);
        meshData.indices.push_back(nextRow + 3);

        meshData.indices.push_back(currentRow + 2);
        meshData.indices.push_back(nextRow + 3);
        meshData.indices.push_back(currentRow + 3);


		// Боковые полигоны
        meshData.indices.push_back(currentRow);
        meshData.indices.push_back(nextRow);
        meshData.indices.push_back(currentRow + 2);

        meshData.indices.push_back(currentRow + 2);
        meshData.indices.push_back(nextRow);
        meshData.indices.push_back(nextRow + 2);

        meshData.indices.push_back(currentRow + 1);
        meshData.indices.push_back(nextRow + 1);
        meshData.indices.push_back(currentRow + 3);

        meshData.indices.push_back(currentRow + 3);
        meshData.indices.push_back(nextRow + 1);
        meshData.indices.push_back(nextRow + 3);

    }



    return meshData;
}

Mesh generatePlane(float width, float height, unsigned int numSegments) 
{
    Mesh meshData;

    float dx = width / static_cast<float>(numSegments);
    float dy = height / static_cast<float>(numSegments);

    // Генерируем вершины для плоскости и текстурные координаты
    for (unsigned int y = 0; y <= numSegments; ++y) {
        float yPos = static_cast<float>(y) * dy;
        float v = static_cast<float>(y) / static_cast<float>(numSegments) * numSegments;

        for (unsigned int x = 0; x <= numSegments; ++x) {
            float xPos = static_cast<float>(x) * dx;
            float u = static_cast<float>(x) / static_cast<float>(numSegments) * numSegments;

            // Добавляем вершину и текстурные координаты
            meshData.vertices.push_back(xPos - width / 2.0f);  // x
            meshData.vertices.push_back(0.0f);                 // y (плоскость находится на y=0)
            meshData.vertices.push_back(yPos - height / 2.0f); // z

            meshData.uvs.push_back(u);
            meshData.uvs.push_back(v);
        }
    }

    // Генерируем индексы для плоскости
    for (unsigned int y = 0; y < numSegments; ++y) {
        for (unsigned int x = 0; x < numSegments; ++x) {
            unsigned int topLeft = y * (numSegments + 1) + x;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = topLeft + (numSegments + 1);
            unsigned int bottomRight = bottomLeft + 1;

            // Верхний треугольник
            meshData.indices.push_back(topLeft);
            meshData.indices.push_back(topRight);
            meshData.indices.push_back(bottomLeft);

            // Нижний треугольник
            meshData.indices.push_back(topRight);
            meshData.indices.push_back(bottomRight);
            meshData.indices.push_back(bottomLeft);
        }
    }

    return meshData;
}


