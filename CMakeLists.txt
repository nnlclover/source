cmake_minimum_required(VERSION 3.2)

# for compile you need to install this:
# sudo apt-get install libsdl2-dev libsdl2-mixer-dev libsdl2-net-dev libsdl2-ttf-dev libsdl2-image-dev libsdl2-gfx-dev

set(NAME Mak)
#set(DIR_SOURCE "source")

get_filename_component(DIR_SOURCE "./source" ABSOLUTE)

if(WIN32)
    set(SDL_WIN_DIR "./libs/common/SDL2")
    set(FREETYPE_WIN_DIR "./libs/common/Freetype")
    set(OPENGL_WIN_DIR "./libs/common/OpenGL")
endif()

set(NLOHMANN_JSON "./libs/common/json/include")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)




file(GLOB_RECURSE SRC_FILES 
        "${DIR_SOURCE}/main.cpp"

        "${DIR_SOURCE}/graphics/gl_init.cpp"
        "${DIR_SOURCE}/graphics/bitmap.cpp"
        "${DIR_SOURCE}/graphics/gl_init.cpp"
        "${DIR_SOURCE}/graphics/render.cpp"
        "${DIR_SOURCE}/graphics/shader.cpp"
        
        "${DIR_SOURCE}/graphics/textures/skybox.cpp"
        "${DIR_SOURCE}/graphics/textures/texture.cpp"	      
        
        "${DIR_SOURCE}/geometry/mesh_objects.cpp"	      
        "${DIR_SOURCE}/ie/objload.cpp"

	"${DIR_SOURCE}/graphics/textures/bmp.cpp"
        
	      )

file(GLOB_RECURSE HEADER_FILES
        "${DIR_SOURCE}/vectors.h"	

        "${DIR_SOURCE}/graphics/gl_glob.h"	
        "${DIR_SOURCE}/graphics/bitmap.h"
        "${DIR_SOURCE}/graphics/gl_glob.h"
        "${DIR_SOURCE}/graphics/gl_help.h"
        "${DIR_SOURCE}/graphics/gl_init.h"
        "${DIR_SOURCE}/graphics/matrix_math.h"
        "${DIR_SOURCE}/graphics/render.h"
        "${DIR_SOURCE}/graphics/shader.h"
        
        "${DIR_SOURCE}/geometry/mesh_objects.h"
        "${DIR_SOURCE}/geometry/Mesh.h"	      

        "${DIR_SOURCE}/graphics/textures/texture.h"
        
        "${DIR_SOURCE}/ie/objload.h"
    
	      )

project(${NAME})

set (CMAKE_CXX_STANDARD 17)
add_executable(${NAME} ${SRC_FILES} ${HEADER_FILES})



include_directories(${DIR_SOURCE})
include_directories("libs/common/glm")





#### Used on Windows
if(WIN32)
    
    ########### fsdk static libraries
    #set(FSDK "D:\\repos_archive\\fsdk")
    #set(FSDK_LIBRARIES "${FSDK}\\build\\Debug\\prox.lib")
    #set(FSDK_INCLUDE_DIRS "${FSDK}")


    set(SDL2_LIBRARIES    "${SDL_WIN_DIR}/lib/x64")
    set(SDL2_INCLUDE_DIRS "${SDL_WIN_DIR}/include")

########### freetype static libraries windows 
    file(GLOB_RECURSE FREETYPE_LIBRARIES    "${FREETYPE_WIN_DIR}/release static/vs2015-2022/win64/freetype.lib")
    set(FREETYPE_INCLUDE_DIRS "${FREETYPE_WIN_DIR}/include")

########### OpenGL static libraries windows x64
    file(GLOB_RECURSE OPENGL_LIBRARIES "${OPENGL_WIN_DIR}/lib/Release/x64/glew32s.lib"
                                       "${OPENGL_WIN_DIR}/lib/Release/x64/OpenGL32.lib"
                                        )
    set(OPENGL_INCLUDE_DIRS "${OPENGL_WIN_DIR}/include")



    include_directories(${SDL2_INCLUDE_DIRS})
    include_directories(${FREETYPE_INCLUDE_DIRS})
    include_directories(${OPENGL_INCLUDE_DIRS})

    file(GLOB_RECURSE LIBS 
              "${SDL2_LIBRARIES}/SDL2main.lib"
              "${SDL2_LIBRARIES}/SDL2.lib"
              "${SDL2_LIBRARIES}/SDL2main.lib"
              "${SDL2_LIBRARIES}/SDL2test.lib"
              "${SDL2_LIBRARIES}/SDL2_image.lib"
              "${SDL2_LIBRARIES}/SDL2_mixer.lib"
        )

else() # for linux

    #set(FSDK_LIBRARIES "/lib/libprox/libprox.a")
    
    # Debug symbols
    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -g")

    #include_directories("/include/prox")
    
    find_package(GLEW REQUIRED)
    find_package(Freetype REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(SDL2 REQUIRED)

    set(SDL2IMAGE_LIBRARIES -lSDL2_image -lSDL2_mixer)

    set(LIBS "${SDL2_LIBRARIES}")

endif()

include_directories(${NLOHMANN_JSON})

message("${SRC_FILES}")
message("${FREETYPE_LIBRARIES}")
message("${LIBS}")
message("${OPENGL_LIBRARIES}")

#####################set_property(GLOBAL PROPERTY USE_FOLDERS ON)


set(FSDK_DIR "/home/nlclover/fsdk/new")
include_directories(${FSDK_DIR})
link_directories(${FSDK_DIR})


target_link_libraries(${NAME} 
                "${FREETYPE_LIBRARIES}" 
                "${OPENGL_LIBRARIES}"
                "${LIBS}"
                "${SDL2IMAGE_LIBRARIES}"
                "${FSDK_LIBRARIES}"
                "${ASSIMP_LIBRARIES}"
                )
                              
target_include_directories(${NAME} PRIVATE 
                ${FREETYPE_INCLUDE_DIRS}
                ${OPENGL_INCLUDE_DIRS}
                ${SDL2_INCLUDE_DIRS}  
                )

set_property(TARGET ${NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ../bin)
