// Using SDL and standard IO

#define _DEBUG

#include <stdio.h>
#include <iostream>
#include "graphics/render.h"
#include <SDL2/SDL.h>
#include <graphics/bitmap.h>
#include <fstream>
#include <graphics/bitmap.h>
#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include <geometry/Mesh.h>
#include <stdio.h>
#include <geometry/mesh_objects.h>
#include <ie/objload.h>
//#include <graphics/ui/ui_render.h>






class Projectile {
public:
    Projectile(float initialX, float initialY, float initialZ, float initialVelocity, float launchAngle) :
        x(initialX), y(initialY), z(initialZ), velocity(initialVelocity), angle(launchAngle) {}

    void simulateTimeStep(float timeStep) {
        // Calculate horizontal and vertical components of velocity
        float velocityX = velocity * std::cos(angle);
        float velocityY = velocity * std::sin(angle);
        
        // Update positions using kinematic equations
        x += velocityX * timeStep;
        y += velocityY * timeStep;
        z += verticalVelocity * timeStep;

        // Update vertical velocity due to gravity
        verticalVelocity -= gravity * timeStep;

        // Print current position
        //std::cout << "Time: " << currentTime << " seconds | Position: (" << x << ", " << y << ", " << z << ")\n";

        // Increase current time
        currentTime += timeStep;
    }

//private:
    float x;                  // Current x position
    float y;                  // Current y position
    float z;                  // Current z position
    float velocity;           // Launch velocity
    float angle;              // Launch angle (radians)
    float verticalVelocity = 0.0f; // Initial vertical velocity
    float gravity = 9.81f;    // Acceleration due to gravity (m/s^2)
    float currentTime = 0.0f; // Current simulation time (seconds)
};




int main(int argc, char *args[])
{
	// render::GraphicsInit(800, 650);
	render::GraphicsInit(1080, 720);
	render::renderer renderer;
	renderer.init();


	float height = 3.0f;   // Высота конуса
	float radius = 2.0f;   // Радиус основания конуса
	int num_segments = 20; // Количество сегментов основания



	Projectile projectile(0.0f, 0.0f, 0.0f, 50.0f, M_PI / 4.0f); // 45 degrees
    // Simulate projectile motion for a certain time period
    float totalTime = 10.0f; // seconds
    float timeStep = 0.1f;   // seconds


	Mesh TubeMesh = generateSphere(20, 100, 100);//generateTube(2.0f, 1.9f, 10, 50);

	render::Mesh *mtube = new render::Mesh;
	mtube->vertex = TubeMesh.vertices.data();
	mtube->index = TubeMesh.indices.data();
	mtube->vertex_size = TubeMesh.vertices.size();
	mtube->index_size = TubeMesh.indices.size();
	mtube->uv = TubeMesh.uvs.data();
	mtube->uv_size = TubeMesh.uvs.size();
	mtube->finalGen();
	renderer.statics.push_back(mtube);
	textures::StorageLoadTexture("../textures/corrosion-metal.png.jpeg", mtube->texture);

	render::renderer::object *asd_plane = new render::renderer::object;
	asd_plane->mesh_ptr = mtube;
	asd_plane->position = MVector3f(-50, -5, -50);
	renderer.objects_on_scene.push_back(asd_plane);


	asd_plane = new render::renderer::object;
	asd_plane->mesh_ptr = mtube;
	asd_plane->position = MVector3f(-50, -5, -50);
	renderer.objects_on_scene.push_back(asd_plane);
	




	loadObj obj;
	obj.load("/mnt/hard/home/nlclover/3d/obj.obj");
	

	exportObj(obj.meshes, "/mnt/hard/home/nlclover/3d/model.obj");


	render::Mesh *moxi = new render::Mesh;
	moxi->vertex = obj.meshes[0]->vertices.data();
	moxi->index = obj.meshes[0]->indices.data();
	moxi->vertex_size = obj.meshes[0]->vertices.size();
	moxi->index_size = obj.meshes[0]->indices.size();
	moxi->finalGen();
	renderer.statics.push_back(moxi);
	textures::StorageLoadTexture("../textures/corrosion-metal.png.jpeg", moxi->texture);

	moxi->multiply(100);

	render::renderer::object* aassd_plane = new render::renderer::object;
	aassd_plane->mesh_ptr = moxi;
	aassd_plane->position = MVector3f(-50, -5, -50);
	renderer.objects_on_scene.push_back(aassd_plane);

	
	


	int half_w = main_params::width / 2;
	int half_h = main_params::height / 2;

	GLfloat yaw = -90.0f;
	GLfloat pitch = 0.0f;
	MVector2i last(half_w, half_h);

	SDL_WarpMouseInWindow(main_params::window, last.x, last.y);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	int oldtime = 0, newtime = 0;
	float fps = 0;
	//SDL_TimerID timerID = SDL_AddTimer(3 * 1000, callback, &fps);

	const double desiredFPS = 64.0;

	GLfloat sensitivity = 0.3;
	GLfloat velocity = 0.038f;
	SDL_Event e;
	bool pause = false;
	bool quit = false, print_mode = false;
	double deltaTime;
	int trigger_button = false;


	static float time = 0;

	

	while (quit == false)
	{
		const Uint8 *inits = SDL_GetKeyboardState(NULL);
		bool has_be = false;
		for (int i = 0; i < 50; ++i)
		{
			if (inits[i] != 0)
			{
				if (print_mode)
				{
					std::cout << "pressed: " << i << std::endl;
				}
				if (pause)
					break;
				switch (i)
				{
				case 26: // w
					renderer.cameraPos += renderer.cameraFront * velocity;
					break;
				case 4: // a
					renderer.cameraPos -= mm::normalize(mm::cross(renderer.cameraFront, renderer.cameraUp)) * velocity;
					break;
				case 22: // s
					renderer.cameraPos -= renderer.cameraFront * velocity;
					break;
				case 7: // d
					renderer.cameraPos += mm::normalize(mm::cross(renderer.cameraFront, renderer.cameraUp)) * velocity;
					break;
				case 12: // i
					if(trigger_button != 12)
					{
					trigger_button = 12;
					std::cout << "polyMode" << std::endl;
					renderer.polyMode();
					}
					has_be = true;
					break;
				}
				
			}
		}
		if(!has_be)
		{
			trigger_button = 0;
		}
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_q:
					std::cout << "x: " << renderer.cameraPos.x << ", y: " << renderer.cameraPos.y << ", z: " << renderer.cameraPos.z << std::endl;
					break;
				case SDLK_NUMLOCKCLEAR:
					pause = !pause;
					if (pause)
					{
						SDL_SetRelativeMouseMode(SDL_FALSE);
						std::cout << "game state: Pause" << std::endl;
					}
					else
					{
						SDL_SetRelativeMouseMode(SDL_TRUE);
						last.x = half_w;
						last.y = half_h;
						SDL_WarpMouseInWindow(main_params::window, half_w, half_h);
						std::cout << "game state: Play" << std::endl;
					}
					break;
				case SDLK_ESCAPE:
					quit = true;
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
				if (pause)
					continue;
				yaw += e.motion.xrel * sensitivity;
				pitch += e.motion.yrel * -sensitivity;
				if (pitch > 89.0f)
					pitch = 89.0f;
				if (pitch < -89.0f)
					pitch = -89.0f;
				const float rad_yaw = mm::radians(yaw);
				const float rad_pitch = mm::radians(pitch);

				const MVector3f front = {
					cos(rad_yaw) * cos(rad_pitch),
					sin(rad_pitch),
					sin(rad_yaw) * cos(rad_pitch)};
				renderer.cameraFront = mm::normalize(front);
				SDL_WarpMouseInWindow(main_params::window, half_w, half_h);
			}
			break;
			case SDL_MOUSEWHEEL:
    		{
        		if(e.wheel.y > 0) // scroll up
        		{
             		velocity += 0.05f;
        		}
        		else if(e.wheel.y < 0) // scroll down
        		{
					if(velocity - 0.05 > 0)
        		    	velocity -= 0.05f;
        		}

        		if(e.wheel.x > 0) // scroll right
        		{
        		     // ...
        		}
        		else if(e.wheel.x < 0) // scroll left
        		{
        		     // ...
        		}
			}
			break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					int x, y;
					SDL_GetWindowSize(main_params::window, &x, &y);
					glViewport(0, 0, x, y);
				}
				break;
			}
		}
		if (pause)
			continue;

		

		renderer.draw();

		// frame analitics
		oldtime = newtime;
		newtime = SDL_GetTicks();

		deltaTime = ((double)newtime - oldtime);
		fps = 1000.0 / deltaTime;

		double start_time = SDL_GetTicks();
		double target_frame_time = 1000.0 / desiredFPS;

		

		while (SDL_GetTicks() - start_time < target_frame_time)
		{
			// Ждем до следующего кадра
			SDL_Delay(static_cast<std::uint32_t>(target_frame_time - (SDL_GetTicks() - start_time)));
		}

		glGetError();
	}
	render::GraphicsClose();

	return 0;
}
