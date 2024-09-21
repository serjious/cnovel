cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME cnovel)
project(${PROJECT_NAME} LANGUAGES C)

set(CMAKE_C_STANDARD 90) 
set(CMAKE_C_STANDARD_REQUIRED OFF)

include_directories(include)

set(CMAKE_C_FLAGS_DEBUG_INIT "-Wall -ansi -pedantic -g")
set(CMAKE_C_FLAGS_RELEASE_INIT  "-Wall -ansi -pedantic")

set(SOURCES 	src/main.c
		src/config.c
		src/error.c
		src/parse.c
		src/strutils.c
		src/utils.c
		src/window.c
		src/sound.c
		src/menu.c
		src/engine.c
		src/input.c
		src/save.c
		src/log.c
)

set(HEADERS 	include/config.h
		include/error.h
		include/parse.h
		include/strutils.h
		include/utils.h
		include/def.h
		include/window.h
		include/sound.h
		include/menu.h
		include/engine.h
		include/input.h
		include/save.h
		include/log.h
)

add_executable(cnovel ${SOURCES} ${HEADERS})

if(WIN32)
	target_link_libraries(${PROJECT_NAME} mingw32 SDL2main
							 SDL2 SDL2_mixer SDL2_image)
endif(WIN32)

if(UNIX)
	target_link_libraries(${PROJECT_NAME} SDL2main SDL2 SDL2_mixer
											 SDL2_image)
endif(UNIX)


