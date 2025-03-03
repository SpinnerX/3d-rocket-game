#include "main_scene.hpp"
#include <core/ui/widgets.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <glm/fwd.hpp>
#include <core/scene/components.hpp>
#include <format> 
#include <cstdlib>

// #include "random.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/compatibility.hpp>

// std::random_device rd;
// std::mt19937 gen(rd());
// std::uniform_real_distribution<> dis(-0.1f, 0.1f); // Random offset for sand effect

constexpr int ROTATION_DIRECTION_X = -1;
constexpr int ROTATION_DIRECTION_Y = -1;


static atlas::Transform g_platform_transform;

main_scene::main_scene(){}

main_scene::main_scene(const std::string& Tag) : scene_scope(Tag){
    console_log_fatal("main_scene::main_scene Constructor Called to get world tag = {}", atlas::system_registry::get_world().get_tag());
    cc = new character_controller(this);
    m_Rocket = *cc;
    
    m_MainCamera =  create_new_object("Main Camera");
    TargetCamera target_camera = {
        .Position = {0.f, 2.5f, 2.3f},
        .Rotation = {-90.f, 0.f, -30.f}
    };
    m_MainCamera->set<atlas::Transform>({
        .Position = {0.f, 2.5f, 2.3f},
        .Rotation = {-90.f, 0.f, -30.f},
        .Scale = {2.80f, 1.f, 3.70f}
    });

    m_MainCamera->set<TargetCamera>(target_camera);
    m_MainCamera->add<atlas::PerspectiveCamera>();

    m_Sphere = create_new_object("backdrop");
    m_Sphere->set<atlas::Transform>({
        .Position = {0.f, 2.10f, -7.30f},
        .Scale = {200.20f,200.20f, 200.20f},
        .Color = {0.3f, 0.4f, 0.5f, 0.f}
    });
    m_Sphere->set<atlas::MeshComponent>({"3d_models/tutorial/colored_cube.obj"});

    m_moon = create_new_object("moon");
    m_moon->set<atlas::Transform>({
        .Position = {0.f, 0.10f, -200.30f},
        .Scale = {20.20f,20.20f, 20.20f},
        .Color = {240.f, 131.f, 58.f, 1.f}
    });
    m_moon->set<atlas::MeshComponent>({"3d_models/tutorial/Ball OBJ.obj"});

    int height = 0;
    std::string objNam = "";
    

    for(int i = 0; i < 6; i++)
    {
        printf("We are looping %d times!\n", i);
        atlas::ref<atlas::scene_object> b = nullptr;
        atlas::ref<atlas::scene_object> t = nullptr;
        objNam = std::format("Object{}", i); 
        b = create_new_object(objNam);
        objNam = std::format("TObject{}", i);
        t = create_new_object(objNam);

        srand(i*10);
        height = rand() % 31;
        b->set<atlas::Transform>({
            .Position = {0.f, -130.10f, static_cast<float>(i*(-10)-50)},
            .Scale = {2.20f, static_cast<float>(height)+130.f, 2.20f}
        });
        
        //anything greater than 200 is the gap size
        t->set<atlas::Transform>({
            .Position = {0.f, static_cast<float>(height)+205.f, static_cast<float>(i*(-10)-50)},
            .Scale = {2.20f, 200.f, 2.20f}
        });
        b->set<atlas::MeshComponent>({"3d_models/cylinder.obj"});
        t->set<atlas::MeshComponent>({"3d_models/cylinder.obj"});
        m_box.push_back(b);
        m_box.push_back(t);
    }

    atlas::sync_update::sync(this, &main_scene::on_update);
    atlas::sync_update::sync_physics(this, &main_scene::on_physics_update);
    atlas::sync_update::attach(this, &main_scene::OnUIUpdate);
    atlas::sync_update::submit(this, &main_scene::OnSceneRender);
}

void main_scene::getViewPortControl(atlas::PerspectiveCamera &perspective_camera, float delta_time, bool click_check)
{
    if (atlas::event::is_key_pressed(KEY_W)){
        perspective_camera.ProcessKeyboard(atlas::FORWARD, delta_time);
    }
    if (atlas::event::is_key_pressed(KEY_S)){
        perspective_camera.ProcessKeyboard(atlas::BACKWARD, delta_time);
    }
    if (atlas::event::is_key_pressed(KEY_A)){
        perspective_camera.ProcessKeyboard(atlas::LEFT, delta_time);
    }
    if (atlas::event::is_key_pressed(KEY_D)){
        perspective_camera.ProcessKeyboard(atlas::RIGHT, delta_time);
    }
    if(atlas::event::is_key_pressed(KEY_Q)){
        perspective_camera.ProcessKeyboard(atlas::UP, delta_time);
    }
    if(atlas::event::is_key_pressed(KEY_E)){
        perspective_camera.ProcessKeyboard(atlas::DOWN, delta_time);
    }
    

    if (atlas::event::is_key_pressed(KEY_LEFT_CONTROL))
    {
        if(atlas::event::is_key_pressed(KEY_F12)){
            atlas::application::get_window().close();
        }
    }

    //! @note Press right key and drag the mouse to rotate around
    if (atlas::event::is_mouse_pressed(MOUSE_BUTTON_RIGHT)) {
        glm::vec2 cursor_pos = atlas::event::cursor_position();
        
        //! @note On right click make sure change starts as 0
        if(!on_click_check)
        {
            last_cursor_pos = cursor_pos;
            on_click_check = true;
        }

        //! @note offset is now delta_x and delta_y
        //! @note the difference between mouse old and new positions
        glm::vec2 offset = cursor_pos - last_cursor_pos;

        glm::vec2 velocity = offset * (delta_time * 100);

        perspective_camera.ProcessMouseMovement(velocity.x * ROTATION_DIRECTION_X, 0.0f);

        perspective_camera.ProcessMouseMovement(0.0f,velocity.y * ROTATION_DIRECTION_Y);

        last_cursor_pos = cursor_pos;
    } else {
        on_click_check = false;
    }
}

void main_scene::on_update()
{
    atlas::Transform perspective_camera_transform = *m_MainCamera->get<atlas::Transform>();
    atlas::PerspectiveCamera perspective_camera = *m_MainCamera->get<atlas::PerspectiveCamera>();
    float delta_time = atlas::sync_update::delta_time();
    
    perspective_camera.Pitch = perspective_camera_transform.Rotation.z;
    perspective_camera.Position = perspective_camera_transform.Position;

    if (editor)
    {
        // console_log_trace("Editor Enabled!!!");
        getViewPortControl(perspective_camera, delta_time, on_click_check );
        //printf("Camera Pos: %f, %f, %f\n", perspective_camera_atlas::Transform.Position.x + camera.Position.x, perspective_camera_atlas::Transform.Position.y+ camera.Position.y-1.5f, perspective_camera_atlas::Transform.Position.z + camera.Position.z);
    }
    
    //! @note This should only be set
    if(m_game_mode){
        // m_MainCamera->set<atlas::Transform>({
        //     // .Position = {0.f, 2.5f, 2.3f},
        //     // .Position = {m_Rocket->get<atlas::Transform>()->Position.x, m_Rocket->get<atlas::Transform>()->Position.y, 2.3f},
        //     .Position = m_Rocket->get<atlas::Transform>()->Position,
        //     .Rotation = {-90.f, 0.f, -30.f},
        //     .Scale = {2.80f, 1.f, 3.70f}
        // });
        // atlas::PerspectiveCamera camera = *m_MainCamera->get<atlas::PerspectiveCamera>();
        // camera.Position = m_Rocket->get<atlas::Transform>()->Position;
        // camera.Position = {m_Rocket->get<atlas::Transform>()->Position.x, m_Rocket->get<atlas::Transform>()->Position.y, 2.3f};

        // m_MainCamera->set<atlas::atlas::PerspectiveCamera>(camera);
        auto t = *m_Rocket->get<atlas::Transform>();
        float x = t.Position.x;
        float y = t.Position.y;
        float z = t.Position.z;
        t.Position = {x, y + 2.5f, z + 2.3f};
        t.Rotation = {-90.f, 0.f, -30.f};

        // perspective_camera.Position = {x, 2.5f, z - 2.3f};
        // perspective_camera.Position = {x, y, -(z - previous_z_axis)};
        perspective_camera.Position = t.Position;
        perspective_camera.Pitch = t.Rotation.z;
        perspective_camera.Yaw = t.Rotation.x;
        m_game_mode = false;
    }

    if(atlas::event::is_key_pressed(KEY_F)){
        editor = true;
        m_game_mode = false;
    }

    if(atlas::event::is_key_pressed(KEY_G)){
        editor = false;
        m_game_mode = true;
    }

    perspective_camera_transform.Position = perspective_camera.Position;
    perspective_camera_transform.Rotation = { perspective_camera.Yaw, perspective_camera.RotationAngle, perspective_camera.Pitch };
    perspective_camera.ProcessMouseMovement(0.f, 0.f);
    perspective_camera.UpdateProjView();

    m_MainCamera->set<atlas::PerspectiveCamera>(perspective_camera);
    m_MainCamera->set<atlas::Transform>(perspective_camera_transform);
}


void main_scene::OnUIUpdate(){
    //Getting our sphere atlas::Transform to modify it
    auto sphere_data = *m_Sphere->get<atlas::Transform>();
    atlas::Transform rocket_data = *m_Rocket->get<atlas::Transform>();
    atlas::PerspectiveCamera camera = *m_MainCamera->get<atlas::PerspectiveCamera>();
    glm::vec3 color = {rocket_data.Color.x, rocket_data.Color.y, rocket_data.Color.z};
    atlas::ui::draw_panel_component<atlas::MeshComponent>("Rocket", [&](){
        atlas::ui::draw_vec3("pos 1", rocket_data.Position);
        atlas::ui::draw_vec3("rotate 1", rocket_data.Rotation);
        atlas::ui::draw_vec3("Color", color);
    });

    //! @note Basic Properties Panel
    if(ImGui::Begin("Properties Panel")) {
        //! @note THERE IS AN ERROR. Where if the imgui docking window is outside of the window
        //! @note Imgui will just have a window that appears until when you exit the application and the UI is not docked outside the window
        atlas::ui::draw_panel_component<atlas::MeshComponent>("Sphere", [&](){

            atlas::ui::draw_vec3("pos 1", sphere_data.Position);
            atlas::ui::draw_vec3("scale 1", sphere_data.Scale);
            atlas::ui::draw_vec3("rotate 1", sphere_data.Rotation);
            atlas::ui::button_open_file_dialog("Load Mesh 1", m_MeshFilepath);
            
            if(m_MeshFilepath != "") {
                std::filesystem::path relative_path = std::filesystem::relative(m_MeshFilepath, "./");
                console_log_trace("Filepath = {}", m_MeshFilepath);
                console_log_trace("in branch 1 mesh_file = {}", relative_path.string());

                m_Sphere->set<atlas::MeshComponent>({relative_path.string()});
                m_MeshFilepath = "";
            }
        });

        atlas::ui::draw_panel_component<atlas::MeshComponent>("Camera", [&](){
            glm::vec3 camRot = {camera.Yaw, 0.f, camera.Pitch};
            atlas::ui::draw_vec3("pos 1", camera.Position);
            atlas::ui::draw_vec3("rotate 1", camRot);
        });

        // g_platform_transform
        atlas::ui::draw_panel_component<atlas::MeshComponent>("Platform", [&](){
            // glm::vec3 camRot = {camera.Yaw, 0.f, camera.Pitch};
            // atlas::ui::draw_vec3("pos 1", camera.Position);
            atlas::ui::draw_vec3("position", g_platform_transform.Position);
            atlas::ui::draw_vec3("rotation", g_platform_transform.Rotation);
            atlas::ui::draw_vec3("scale", g_platform_transform.Scale);
        });

        ImGui::End();
    }

    
    m_Rocket->set<atlas::Transform>(rocket_data);
    m_Sphere->set<atlas::Transform>(sphere_data);
}

void main_scene::on_physics_update()
{
    if(!editor)
    {
        // auto t = *m_Rocket->get<atlas::Transform>();
        // auto x = t.Position.x;
        // auto y = t.Position.y;
        // auto z = t.Position.z;
        
        // m_MainCamera->set<atlas::Transform>({
        //     .Position = {x, 2.5f, z - 2.3f},
        //     .Rotation = {-90.f, 0.f, -20.f},
        //     .Scale = {2.80f, 1.f, 3.70f}
        // });

        float delta_time = atlas::sync_update::delta_time();
        
        m_MainCamera->set<atlas::Transform>({
            .Position = m_MainCamera->get<atlas::Transform>()-> Position + cc->get_linear_velocity()*delta_time,
            .Rotation = m_MainCamera->get<atlas::Transform>()-> Rotation + cc->get_rotation_velocity()*delta_time
        });
    }

}

void main_scene::OnSceneRender(){
    atlas::renderer::render_with_camera(m_Rocket, m_MainCamera);
    atlas::renderer::render_with_camera(m_moon, m_MainCamera);
    for(auto& pip : m_box)
    {
        // printf("We are loopingon render!\n");
        atlas::renderer::render_with_camera(pip, m_MainCamera);
    }

    for(auto& particle : cc->get_particles()){
        if(particle.RenderTarget == nullptr) continue;
        // if(!particle.IsAlive) continue;

        // console_log_trace("Particle.IsAlive = {}", particle.IsAlive);
        atlas::renderer::render_with_camera(particle.RenderTarget, m_MainCamera);
    }

    // for(ref<scene_object>& b : m_box){
    //     atlas::renderer::render_with_camera(b, m_MainCamera);
    // }
    //atlas::renderer::render_with_camera(m_Sphere, m_MainCamera);
}
