cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    application.cpp
    game_world.cpp
    scenes/main_scene.cpp

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