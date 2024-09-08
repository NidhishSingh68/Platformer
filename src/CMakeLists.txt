cmake_minimum_required(VERSION 3.7)

set(CMAKE_BUILD_TYPE Debug)
project(platformer)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
# Find SDL2_image
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

add_executable(platformer main.cpp app.cpp component.cpp  tex_manager.cpp entityManager.cpp system.cpp map.cpp animationContainer.cpp)


target_link_libraries(platformer ${SDL2_LIBRARIES} -lSDL2_image)

