#include "game_world.hpp"

namespace engine3d{
    RocketWorld::RocketWorld(){}

    RocketWorld::RocketWorld(const std::string& Tag){
        m_CurrentWorldContext = WorldScope(Tag);
        m_MainCurrentScene = CreateRef<MainScene>("LevelScene");
        m_CurrentWorldContext.AddScene(m_MainCurrentScene);
    }
};