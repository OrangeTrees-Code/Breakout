cmake_minimum_required(VERSION 3.15)
project(Breakout)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED on)

find_package(OpenGL REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

add_executable(game
    src/Main.cpp
    src/glad.c
    src/Game.cpp
    src/Shader.cpp
    src/Texture2D.cpp
    src/ResourceManager.cpp
    src/Window.cpp
    src/Camera.cpp
    src/SpriteRenderer.cpp
    src/GameObject.cpp
    src/GameLevel.cpp
    src/Global.cpp
    src/Paddle.cpp
    src/DynamicGOBase.cpp
    src/Ball.cpp
    src/IManipulable.ipp
)

target_include_directories(game PRIVATE
    ${GLFW_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
    src
)

target_link_libraries(game
    ${GLFW_LIBRARIES}
    OpenGL::GL
)

if(UNIX)
    target_link_libraries(game dl pthread)
endif()

target_compile_definitions(game PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
