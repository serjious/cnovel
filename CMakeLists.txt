cmake_minimum_required(VERSION 3.5)

project(cnovel_engine LANGUAGES C)

include_directories(include)
set(CMAKE_C_FLAGS_DEBUG_INIT "-Wall -ansi -pedantic -g")
set(CMAKE_C_FLAGS_RELEASE_INIT  "-Wall -ansi -pedantic")
set(SOURCES src/main.c
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
)
set(HEADERS include/config.h
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
)
add_executable(cnovel ${SOURCES} ${HEADERS})
target_link_libraries(cnovel SDL2 SDL2_mixer SDL2_image)

