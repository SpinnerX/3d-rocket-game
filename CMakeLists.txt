cmake_minimum_required(VERSION 3.27)
project(Starter CXX)


build_application(
    SOURCES
    application.cpp
    game_world.cpp
    scenes/main_scene.cpp
    content/character_controller.cpp
    random.cpp

    PACKAGES
    atlas

    LINK_PACKAGES
    atlas::atlas
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})