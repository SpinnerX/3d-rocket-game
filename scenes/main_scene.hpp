#pragma once
#include <core/scene/scene.hpp>
#include <core/event/input_poll.hpp>
#include <core/system_framework/system_registry.hpp>
#include <scene/components/components.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <renderer/renderer.hpp>
#include "content/CharacterController.hpp"

// #include <core/ui/widgets.hpp>


namespace engine3d{
    class MainScene : public SceneScope{
    public:
        MainScene();
        MainScene(const std::string& Tag);

        virtual ~MainScene(){}

    private:
        void OnUpdate();
        void OnUIUpdate();
        void OnPhysicsUpdate();
        void OnSceneRender();
        bool on_click_check = false;
        glm::vec2 last_cursor_pos;

        void getViewPortControl
        (
            PerspectiveCamera& perspective_camera, 
            float deltaTime, bool click_check
        );
    private:
        // bool m_IsMeshFilepathSelected=false;
        std::string m_MeshFilepath="";
        Ref<SceneObject> m_MainCamera=nullptr;
        Ref<SceneObject> m_Sphere=nullptr;
        Ref<SceneObject> m_Rocket=nullptr;
        Ref<SceneObject> m_box=nullptr;
        CharacterController cc = nullptr;
    };
};