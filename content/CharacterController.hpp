#pragma once
#include <core/scene/world.hpp>
#include <core/scene/scene.hpp>
#include <core/core.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <core/event/input_poll.hpp>
// #include "main_scene.hpp"

    class CharacterController{
        public:
            CharacterController();
            CharacterController(engine3d::SceneScope* p_current_scene);
            virtual ~CharacterController();
            void setAcceleration();
    
            operator engine3d::Ref<engine3d::SceneObject>(){ return m_ObjectHandler; }
        
        private:
            void OnUpdate();
            void OnPhysicsUpdate();
            glm::vec3 getLinearVelocity();
        private:
            std::string objectRef = "";
            engine3d::Ref<engine3d::SceneObject> m_ObjectHandler;
        };