#pragma once
#include <core/scene/world.hpp>
#include <core/scene/scene.hpp>
#include <core/core.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <core/event/input_poll.hpp>

// #include "main_scene.hpp"
    class rocket_controller{
    public:
        rocket_controller();
        rocket_controller(const std::string& Tag);
        virtual ~rocket_controller(){}


    private:
        void OnUpdate();

        void OnPhysicsUpdate();

        void OnSceneRender();

    private:
        std::string objectRef = "";
        Ref<SceneObject> m_Rocket=nullptr;
    };