#pragma once
#include "scenes/main_scene.hpp"
#include <atlas/core/core.hpp>
#include <atlas/core/scene/world.hpp>

class rocket_world{
public:
    rocket_world();
    rocket_world(const std::string& p_tag);

private:
    atlas::world_scope m_current_world_context;
    atlas::ref<main_scene> m_main_scene = nullptr;
};
