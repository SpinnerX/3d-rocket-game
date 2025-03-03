#include "game_world.hpp"

rocket_world::rocket_world(){}

rocket_world::rocket_world(const std::string& p_tag){
    m_current_world_context = atlas::world_scope(p_tag);
    m_main_scene = atlas::create_ref<main_scene>("LevelScene");
    m_current_world_context.add_scene(m_main_scene);
}
