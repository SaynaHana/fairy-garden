cmake_minimum_required(VERSION 3.18)

# Name of project
set(PROJ_NAME Project)
project(${PROJ_NAME})

# Specify project files: header files and source files
set(HDRS
    file_utils.h
    game.h
    game_object.h
    player_game_object.h
    explosion_game_object.h
    collectible_game_object.h
    collectibles/fairy_dust_collectible.h
    collectibles/rainbow_fairy_dust_collectible.h
    collectibles/dark_fairy_dust_collectible.h
    enemies/enemy_game_object.h
    enemies/dying_earth_enemy.h
    enemies/dying_earth_enemy_link.h
    enemies/spawner.h
    projectile_game_object.h
    projectiles/magic_missile_projectile.h
    projectiles/water_wave_projectile.h
    projectiles/player_projectile.h
    shader.h
    geometry.h
    sprite.h
    timer.h
    patrol_data.h
    data/move_data.h
    attack_data.h
    Constants.h
    ui/text_game_object.h
    weapons/weapon.h
    data/weapon_data.h
    weapons/interval_weapon.h
    weapons/magic_missile_weapon.h
    weapons/water_wave_weapon.h
    weapons/default_player_weapon.h
    weapons/shotgun_player_weapon.h
)
 
set(SRCS
    file_utils.cpp
    game.cpp
    main.cpp
    game_object.cpp
    player_game_object.cpp
    explosion_game_object.cpp
    collectible_game_object.cpp
    collectibles/fairy_dust_collectible.cpp
    collectibles/rainbow_fairy_dust_collectible.cpp
    collectibles/dark_fairy_dust_collectible.cpp
    enemies/enemy_game_object.cpp
    enemies/dying_earth_enemy.cpp
    enemies/dying_earth_enemy_link.cpp
    enemies/spawner.cpp
    projectiles/magic_missile_projectile.cpp
    projectiles/water_wave_projectile.cpp
    projectiles/player_projectile.cpp
    projectile_game_object.cpp
    shader.cpp
    sprite.cpp
    timer.cpp
    patrol_data.cpp
    data/move_data.cpp
    attack_data.cpp
    sprite_vertex_shader.glsl
    sprite_fragment_shader.glsl
    ui/text_game_object.cpp
    weapons/weapon.cpp
    weapons/interval_weapon.cpp
    weapons/magic_missile_weapon.cpp
    weapons/water_wave_weapon.cpp
    weapons/default_player_weapon.cpp
    weapons/shotgun_player_weapon.cpp
)

# Add path name to configuration file
configure_file(path_config.h.in path_config.h)

# Add executable based on the source files
add_executable(${PROJ_NAME} ${HDRS} ${SRCS})

# Directories to include for header files, so that the compiler can find
# path_config.h
target_include_directories(${PROJ_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Require OpenGL library
set(OpenGL_GL_PREFERENCE LEGACY)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJ_NAME} ${OPENGL_gl_LIBRARY})

# Other libraries needed
set(LIBRARY_PATH $ENV{COMP2501_LIBRARY_PATH} CACHE PATH "Folder with GLEW, GLFW, GLM, and SOIL libraries")
include_directories(${LIBRARY_PATH}/include)
if(NOT WIN32)
    find_library(GLEW_LIBRARY GLEW)
    find_library(GLFW_LIBRARY glfw)
    find_library(SOIL_LIBRARY SOIL)
elseif(WIN32)
    find_library(GLEW_LIBRARY glew32s HINTS ${LIBRARY_PATH}/lib)
    find_library(GLFW_LIBRARY glfw3 HINTS ${LIBRARY_PATH}/lib)
    find_library(SOIL_LIBRARY SOIL HINTS ${LIBRARY_PATH}/lib)
endif(NOT WIN32)
target_link_libraries(${PROJ_NAME} ${GLEW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJ_NAME} ${SOIL_LIBRARY})

# The rules here are specific to Windows Systems
if(WIN32)
    # Avoid ZERO_CHECK target in Visual Studio
    set(CMAKE_SUPPRESS_REGENERATION TRUE)
 
    # This will use the proper libraries in debug mode in Visual Studio
    set_target_properties(${PROJ_NAME} PROPERTIES DEBUG_POSTFIX _d)

    # Set the default project in VS
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJ_NAME})
endif(WIN32)
