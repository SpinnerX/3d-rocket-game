cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    application.cpp
    game_world.cpp
    scenes/main_scene.cpp
    content/controller/rocket_controller.cpp

    PACKAGES
    imguidocking
    stb
    flecs
    tinyobjloader
    engine3d-nfd
    engine3d


    LINK_PACKAGES
    imguidocking::imguidocking
    stb::stb
    flecs::flecs_static
    tinyobjloader::tinyobjloader
    engine3d-nfd::engine3d-nfd


    engine3d::engine3d
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})