#include "render.h"



namespace main_params {
	SDL_GLContext context = nullptr;
	SDL_Window* window = nullptr;
	int width{};
	int height{};
}





namespace render {

	//------------------------------------------------
	//  Initialize graphics (window)
	//------------------------------------------------
	int GraphicsInit(int width, int height, uint32_t flags)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL could not initialize! SDL Error: %s", SDL_GetError());
			return -1;
		}
		
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8); 
		
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1); // Включаем мультисемплинг
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Задаем число сэмплов (здесь 4, можно выбрать другое значение)


		main_params::width = width;
		main_params::height = height;
		main_params::window = SDL_CreateWindow("OpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);
		main_params::context = SDL_GL_CreateContext(main_params::window);
#ifdef _WIN32
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
			throw std::runtime_error("glewInit failed");
#endif

		
		SDL_GL_SetSwapInterval(1);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		//glEnable(GL_CULL_FACE);
		//glFrontFace(GL_BACK);//GL_CCW);
		//glCullFace(GL_CW);
		//glFrontFace(GL_FRONT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 8.0f); // Установить анисотропную фильтрацию на 4x


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		return 0;
	}
	//------------------------------------------------
	// Close public window context
	//------------------------------------------------
	void GraphicsClose()
	{
		SDL_GL_DeleteContext(main_params::context);
		SDL_DestroyWindow(main_params::window);
		SDL_Quit();
	}

	//------------------------------------------------
	//// Activate line visible 
	//------------------------------------------------
	void renderer::polyMode()
	{
		if (!isLinesDraw)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			isLinesDraw = true;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			isLinesDraw = false;
		}
	}
  
	//////////////////////////////
	//// Initialize renderer
	//////////////////////////////
	void renderer::init()
	{
		prog = lfd::Shader("name", "../shaders/base/ms.frag", "../shaders/base/ms.vert");
		skyProg = lfd::Shader("name", "../shaders/base/skybox.frag", "../shaders/base/skybox.vert");

		textures::StorageLoadTexture("notfound.png", notfound_texture);
		box.loadGeometry();
		box.LoadTexture("");

		
	}

	//////////////////////////////
	//// Paint objects on scene
	//////////////////////////////
	void renderer::draw()
	{
		GLint viewLoc, projLoc, modelLoc;
		
		renderColor = {0, 0, 0};
		glClearColor(renderColor.x, renderColor.y, renderColor.z, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		///// Mathing world matrix 		  
		mm::perspective(projection.mat, 45, (GLfloat)main_params::width / (GLfloat)main_params::height, 0.1f, 1000.0f);
		mm::LookAt(view.mat, cameraPos, cameraPos + cameraFront, cameraUp);


		///// Drawing Skybox
		if (paintSkybox)
		{
			viewLoc = glGetUniformLocation(skyProg, "view");
			projLoc = glGetUniformLocation(skyProg, "projection");
			modelLoc = glGetUniformLocation(prog, "model");

			mm::translate(box.model.mat, cameraPos.x, cameraPos.y, cameraPos.z, true);

			glUseProgram(skyProg);
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.mat);
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.mat);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, box.model.mat);
			glBindVertexArray(box.VAO);
			glBindTexture(GL_TEXTURE_CUBE_MAP, box.textureID);
			glDepthMask(GL_FALSE);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glDepthMask(GL_TRUE);
		}

		Mesh* mesh;

		glUseProgram(prog);
		modelLoc = glGetUniformLocation(prog, "model");
		viewLoc = glGetUniformLocation(prog, "view");
		projLoc = glGetUniformLocation(prog, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.mat);
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, projection.mat);
		
		glUniform1i(glGetUniformLocation(prog, "texture1"), 0);
		glActiveTexture(GL_TEXTURE0);

		/* Drawing all static meshes */
		for (auto& obj : objects_on_scene)
		{
			mesh = obj->mesh_ptr;
 
			mm::Matrix model;
			mm::translate(model.mat, obj->position.x, obj->position.y, obj->position.z);
			mm::rotate(model.mat, obj->rotation.x, obj->rotation.y, obj->rotation.z);

			
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.mat);
			
			if (mesh->VAO == 0)
			{
				fprintf(stderr, "Render error. Biten VAO\n");
				continue;
			}


			if (mesh->texture == 0)
			{
				glBindTexture(GL_TEXTURE_2D, notfound_texture);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, mesh->texture);
			}
			
			
			glBindVertexArray(mesh->VAO);
			glDrawElements(GL_TRIANGLES, mesh->index_size, GL_UNSIGNED_INT, 0);
		}
		glBindVertexArray(0);

		if (main_params::window == nullptr)
		{
			printf("[MEGA FATAL ERROR] window descriptor == nullptr");
		}
		
		SDL_GL_SwapWindow(main_params::window);

		
	}

	renderer::~renderer()
	{
		for(auto& obj : statics)
		{
			//delete obj;
		}
		for(auto& obj : objects_on_scene)
		{
			//delete obj;
		}
	}
}