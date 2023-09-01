#ifndef __RENDER_H
#define __RENDER_H

#include "graphics/gl_init.h"
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <vector>
#include "shader.h"
#include <math/matrix_math.h>
#include <algorithm>
#include "textures/textures.h"

#include <string>


namespace main_params {
	extern SDL_GLContext context;
	extern SDL_Window* window;
	extern int width;
	extern int height;
}








namespace render {

	int GraphicsInit(int width, int height, uint32_t flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);// | SDL_WINDOW_FULLSCREEN);

	void GraphicsClose();



	class Mesh {
	private:
	public:
		MVector3f position = MVector3f(0.0f, 0.0f, 0.0f);
		float* vertex = nullptr;
		float* uv = nullptr;
		unsigned int* index = nullptr;
		size_t vertex_size = 0, uv_size = 0, index_size = 0;;
		GLuint VBO = 0, uv_VBO = 0, VAO = 0, EBO = 0;
		GLuint texture = 1;
		

  		void multiply(float mno)
    	{
	        for(int i = 0; i < vertex_size; i++)
            	vertex[i] = vertex[i] * mno;
    	}

		void planar_mapping()
		{
			MVector3f planeNormal(0.0f, 1.0f, 0.0f);
		    MVector3f planePoint(0.0f, 0.0f, 0.0f);
			std::vector<float> uvs;
			uvs.reserve(vertex_size * 2);

			// Генерируем UV-координаты для всего меша
			for (size_t i = 0; i < vertex_size; i += 3)
			{
				float x = vertex[i];
				float z = vertex[i + 2];

				uvs.push_back(x); // Используем X-координату для U
				uvs.push_back(z); // Используем Z-координату для V
			}

			// Нормализуем UV-координаты до диапазона [0, 1]
			const float minU = *std::min_element(uvs.begin(), uvs.end());
			const float maxU = *std::max_element(uvs.begin(), uvs.end());

			for (size_t i = 0; i < uvs.size(); i += 2)
			{
				uvs[i] = (uvs[i] - minU) / (maxU - minU);
				uvs[i + 1] = (uvs[i + 1] - minU) / (maxU - minU);
			}

			uv_size = uvs.size() * sizeof(float);
			

			uv = (float*)malloc(uv_size);
			memcpy(uv, uvs.data(), uv_size);
			uv_size = uvs.size();
		}

		int finalGen()
		{
			if (!vertex || !index)
			{
				fprintf(stderr, "class mesh -> finalGen(): vertex or index is null. Please, copy vertex and index buffer\n");
				return -1;
			}
			/*
			GLfloat vertices[] = {
				// Positions          Texture Coords
				0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Top Right
				 0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Bottom Right
				-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Bottom Left
				-0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Top Left 
			};
			GLuint indices[] = {  // Note that we start from 0!
				0, 1, 3, // First Triangle
				1, 2, 3  // Second Triangle
			};
			index_size = 2;
			*/

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, vertex_size * sizeof(float), vertex, GL_STATIC_DRAW);

			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);


			if(!uv_size)
			{
				planar_mapping();
			}

			glGenBuffers(1, &uv_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, uv_VBO);
			glBufferData(GL_ARRAY_BUFFER, uv_size * sizeof(float), uv, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(1);

			//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			//glEnableVertexAttribArray(1);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(uint32_t), index, GL_STATIC_DRAW);			
		

			glBindVertexArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			

			return 0;
		}

		~Mesh()
		{
			if (vertex != nullptr)
				delete[] vertex;
			if (index != nullptr)
				delete[] index;
			if (uv != nullptr)
				delete[] uv;
		}
	};


	class renderer {
	private:
	public:
		struct object {
			Mesh* mesh_ptr = nullptr;
			MVector3f position;
			MVector3f rotation;
			Color col;
		};

		MVector3f renderColor = MVector3f(0.2f, 0.3f, 0.3f);
		std::vector<Mesh*> statics;
		std::vector<object*> objects_on_scene;
		bool isLinesDraw = false;
		bool paintSkybox = true;
		MVector3f cameraPos = MVector3f(0.0f, 0.0f, 3.0f);
		MVector3f cameraFront = MVector3f(0.0f, 0.0f, -1.0f);
		MVector3f cameraUp = MVector3f(0.0f, 1.0f, 0.0f);
		mm::Matrix view;
		mm::Matrix projection;



		GLuint prog, skyProg, notfound_texture;
		textures::skybox box;

		void polyMode();

		void init();

		void draw();

		~renderer();

	};
}

#endif
