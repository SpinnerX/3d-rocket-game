#pragma once
#include <core/scene/scene.hpp>
#include <core/event/event.hpp>
#include <core/system_framework/system_registry.hpp>
#include <core/scene/components.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <renderer/renderer.hpp>
#include "content/character_controller.hpp"

struct TargetCamera{
    glm::vec3 Position;
    glm::vec3 Rotation;
    float Pitch = 0.f;
    float Yaw = 0.f;
    float RotationAngle = 0.f;
};

class main_scene : public atlas::scene_scope{
public:
    main_scene();
    main_scene(const std::string& Tag);

    virtual ~main_scene(){}

private:
    void on_update();
    void OnUIUpdate();
    void on_physics_update();
    void OnSceneRender();
    bool on_click_check = false;
    glm::vec2 last_cursor_pos;
    bool editor = false;
    void getViewPortControl
    (
        atlas::PerspectiveCamera& perspective_camera, 
        float delta_time, bool click_check
    );
private:
    // bool m_IsMeshFilepathSelected=false;
    std::string m_MeshFilepath="";
    atlas::ref<atlas::scene_object> m_MainCamera=nullptr;
    atlas::ref<atlas::scene_object> m_Sphere=nullptr;
    atlas::ref<atlas::scene_object> m_Rocket=nullptr;
    std::vector<atlas::ref<atlas::scene_object>> m_box;
    atlas::ref<atlas::scene_object> m_moon=nullptr;

    character_controller* cc = nullptr;
    // atlas::atlas::PerspectiveCamera m_editor_camera;
    // atlas::atlas::PerspectiveCamera m_target_camera;
    bool m_game_mode = true;
};
