#pragma once
#include "scenes/main_scene.hpp"
#include <engine3d/core/core.hpp>
#include <engine3d/core/scene/world.hpp>

namespace engine3d{
    class RocketWorld{
    public:
        RocketWorld();
        RocketWorld(const std::string& Tag);

    private:
        WorldScope m_CurrentWorldContext;
        Ref<MainScene> m_MainCurrentScene = nullptr;
    };
};