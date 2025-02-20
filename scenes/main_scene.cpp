#include "main_scene.hpp"
#include <core/ui/widgets.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <glm/fwd.hpp>
#include <scene/components/components.hpp>

// #include "random.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/compatibility.hpp>

// std::random_device rd;
// std::mt19937 gen(rd());
// std::uniform_real_distribution<> dis(-0.1f, 0.1f); // Random offset for sand effect

constexpr int ROTATION_DIRECTION_X = -1;
constexpr int ROTATION_DIRECTION_Y = -1;

namespace engine3d{
    MainScene::MainScene(){}

    MainScene::MainScene(const std::string& Tag) : SceneScope(Tag){
        ConsoleLogFatal("MainScene::MainScene Constructor Called to get world tag = {}", SystemRegistry::GetWorld().GetTag());
        cc = new CharacterController(this);
        m_Rocket = *cc;
        
        m_MainCamera =  CreateNewObject("Main Camera");
        m_MainCamera->SetComponent<Transform>({
            .Position = {0.f, 2.5f, 2.3f},
            .Rotation = {-90.f, 0.f, -30.f},
            .Scale = {2.80f, 1.f, 3.70f}
        });
        m_MainCamera->AddComponent<PerspectiveCamera>();

        previous_z_axis = m_MainCamera->GetComponent<Transform>()->Position.z;

        m_Sphere = CreateNewObject("backdrop");
        m_Sphere->SetComponent<Transform>({
            .Position = {0.f, 2.10f, -7.30f},
            .Scale = {200.20f,200.20f, 200.20f},
            .Color = {0.3f, 0.4f, 0.5f, 1.f}
        });
        m_Sphere->SetComponent<MeshComponent>({"3d_models/tutorial/colored_cube.obj"});

        glm::vec3 b_Pos = {0.f, 2.10f, -20.30f};
        int i = 0;
        for(Ref<SceneObject> b : m_box)
        {
            b = CreateNewObject("Object2");
            b->SetComponent<Transform>({
                .Position = {0.f, 2.10f, -27.30f},
                .Scale = {2.20f,2.20f, 2.20f}
            });
            b->SetComponent<MeshComponent>({"3d_models/cylinder.obj"});
        }

        sync_update::sync(this, &MainScene::OnUpdate);
        sync_update::sync_physics(this, &MainScene::OnPhysicsUpdate);
        sync_update::attach(this, &MainScene::OnUIUpdate);
        sync_update::submit(this, &MainScene::OnSceneRender);
    }
    
    void MainScene::getViewPortControl(PerspectiveCamera &perspective_camera, float deltaTime, bool click_check)
    {
        if (InputPoll::IsKeyPressed(ENGINE_KEY_W)){
            perspective_camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if (InputPoll::IsKeyPressed(ENGINE_KEY_S)){
            perspective_camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if (InputPoll::IsKeyPressed(ENGINE_KEY_A)){
            perspective_camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if (InputPoll::IsKeyPressed(ENGINE_KEY_D)){
            perspective_camera.ProcessKeyboard(RIGHT, deltaTime);
        }
        if(InputPoll::IsKeyPressed(ENGINE_KEY_Q)){
            perspective_camera.ProcessKeyboard(UP, deltaTime);
        }
        if(InputPoll::IsKeyPressed(ENGINE_KEY_E)){
            perspective_camera.ProcessKeyboard(DOWN, deltaTime);
        }
        

        if (InputPoll::IsKeyPressed(ENGINE_KEY_LEFT_CONTROL))
        {
            if(InputPoll::IsKeyPressed(ENGINE_KEY_F12))
                ApplicationInstance::GetWindow().Close();
        }

        //! @note Press right key and drag the mouse to rotate around
        if (InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_RIGHT)) {
            glm::vec2 cursor_pos = InputPoll::GetMousePosition();
            
            //! @note On right click make sure change starts as 0
            if(!on_click_check)
            {
                last_cursor_pos = cursor_pos;
                on_click_check = true;
            }

            //! @note offset is now delta_x and delta_y
            //! @note the difference between mouse old and new positions
            glm::vec2 offset = cursor_pos - last_cursor_pos;

            glm::vec2 velocity = offset * (deltaTime * 100);

            perspective_camera.ProcessMouseMovement(velocity.x * ROTATION_DIRECTION_X, 0.0f);

            perspective_camera.ProcessMouseMovement(0.0f,velocity.y * ROTATION_DIRECTION_Y);

            last_cursor_pos = cursor_pos;
        } else {
            on_click_check = false;
        }
    }

    void MainScene::OnUpdate()
    {
        Transform perspective_camera_transform = *m_MainCamera->GetComponent<Transform>();
        PerspectiveCamera perspective_camera = *m_MainCamera->GetComponent<PerspectiveCamera>();
        float deltaTime = sync_update::DeltaTime();
        
        perspective_camera.Pitch = perspective_camera_transform.Rotation.z;
        perspective_camera.Position = perspective_camera_transform.Position;

        if (editor)
        {
            ConsoleLogTrace("Editor Enabled!!!");
            getViewPortControl(perspective_camera, deltaTime, on_click_check );
            //printf("Camera Pos: %f, %f, %f\n", perspective_camera_transform.Position.x + camera.Position.x, perspective_camera_transform.Position.y+ camera.Position.y-1.5f, perspective_camera_transform.Position.z + camera.Position.z);
        }
        
        //! @note This should only be set
        if(m_game_mode){
            // m_MainCamera->SetComponent<Transform>({
            //     // .Position = {0.f, 2.5f, 2.3f},
            //     // .Position = {m_Rocket->GetComponent<engine3d::Transform>()->Position.x, m_Rocket->GetComponent<engine3d::Transform>()->Position.y, 2.3f},
            //     .Position = m_Rocket->GetComponent<engine3d::Transform>()->Position,
            //     .Rotation = {-90.f, 0.f, -30.f},
            //     .Scale = {2.80f, 1.f, 3.70f}
            // });
            // PerspectiveCamera camera = *m_MainCamera->GetComponent<PerspectiveCamera>();
            // camera.Position = m_Rocket->GetComponent<engine3d::Transform>()->Position;
            // camera.Position = {m_Rocket->GetComponent<engine3d::Transform>()->Position.x, m_Rocket->GetComponent<engine3d::Transform>()->Position.y, 2.3f};

            // m_MainCamera->SetComponent<engine3d::PerspectiveCamera>(camera);
            auto t = *m_Rocket->GetComponent<Transform>();
            auto x = t.Position.x;
            auto y = t.Position.y;
            auto z = t.Position.z;
            // perspective_camera.Position = {x, 2.5f, z - 2.3f};
            // perspective_camera.Position = {x, y, -(z - previous_z_axis)};
            perspective_camera.Position = t.Position;
            m_game_mode = false;
        }

        if(InputPoll::IsKeyPressed(ENGINE_KEY_F)){
            editor = true;
            m_game_mode = false;
        }

        if(InputPoll::IsKeyPressed(ENGINE_KEY_G)){
            editor = false;
            m_game_mode = true;
        }

        perspective_camera_transform.Position = perspective_camera.Position;
        perspective_camera_transform.Rotation = { perspective_camera.Yaw, perspective_camera.RotationAngle, perspective_camera.Pitch };
        perspective_camera.ProcessMouseMovement(0.f, 0.f);
        perspective_camera.UpdateProjView();

        m_MainCamera->SetComponent<engine3d::PerspectiveCamera>(perspective_camera);
        m_MainCamera->SetComponent<engine3d::Transform>(perspective_camera_transform);
    }


    void MainScene::OnUIUpdate(){
        //Getting our sphere transform to modify it
        auto sphere_data = *m_Sphere->GetComponent<Transform>();
        Transform rocket_data = *m_Rocket->GetComponent<Transform>();
        PerspectiveCamera camera = *m_MainCamera->GetComponent<PerspectiveCamera>();
        glm::vec3 color = {rocket_data.Color.x, rocket_data.Color.y, rocket_data.Color.z};
        ui::DrawPanelComponent<MeshComponent>("Rocket", [&](){
            ui::DrawVec3UI("pos 1", rocket_data.Position);
            ui::DrawVec3UI("rotate 1", rocket_data.Rotation);
            ui::DrawVec3UI("Color", color);
        });

        //! @note Basic Properties Panel
        if(ImGui::Begin("Properties Panel")) {
            //! @note THERE IS AN ERROR. Where if the imgui docking window is outside of the window
            //! @note Imgui will just have a window that appears until when you exit the application and the UI is not docked outside the window
            ui::DrawPanelComponent<MeshComponent>("Sphere", [&](){

                ui::DrawVec3UI("pos 1", sphere_data.Position);
                ui::DrawVec3UI("scale 1", sphere_data.Scale);
                ui::DrawVec3UI("rotate 1", sphere_data.Rotation);
                ui::LoadFileWithUI("Load Mesh 1", m_MeshFilepath);
                
                if(m_MeshFilepath != "") {
                    std::filesystem::path relative_path = std::filesystem::relative(m_MeshFilepath, "./");
                    ConsoleLogTrace("Filepath = {}", m_MeshFilepath);
                    ConsoleLogTrace("in branch 1 mesh_file = {}", relative_path.string());

                    m_Sphere->SetComponent<MeshComponent>({relative_path.string()});
                    m_MeshFilepath = "";
                }
            });

            ui::DrawPanelComponent<MeshComponent>("Camera", [&](){
                glm::vec3 camRot = {camera.Yaw, 0.f, camera.Pitch};
                ui::DrawVec3UI("pos 1", camera.Position);
                ui::DrawVec3UI("rotate 1", camRot);
            });
            ImGui::End();
        }
        m_Rocket->SetComponent<Transform>(rocket_data);
        m_Sphere->SetComponent<Transform>(sphere_data);
    }

    void MainScene::OnPhysicsUpdate()
    {
        if(!editor)
        {
            // auto t = *m_Rocket->GetComponent<Transform>();
            // auto x = t.Position.x;
            // auto y = t.Position.y;
            // auto z = t.Position.z;
            
            // m_MainCamera->SetComponent<Transform>({
            //     .Position = {x, 2.5f, z - 2.3f},
            //     .Rotation = {-90.f, 0.f, -20.f},
            //     .Scale = {2.80f, 1.f, 3.70f}
            // });

            float deltaTime = sync_update::DeltaTime();
            m_MainCamera->SetComponent<Transform>({
                .Position = m_MainCamera->GetComponent<Transform>()-> Position + cc->getLinearVelocity()*deltaTime,
                .Rotation = m_MainCamera->GetComponent<Transform>()-> Rotation + cc->getRotationalVelocity()*deltaTime
            });
        }

    }

    void MainScene::OnSceneRender(){
        Renderer::RenderWithCamera(m_Rocket, m_MainCamera);

        for(auto& particle : cc->get_particles()){
            if(particle.RenderTarget == nullptr) continue;
            // if(!particle.IsAlive) continue;
    
            // ConsoleLogTrace("Particle.IsAlive = {}", particle.IsAlive);
            engine3d::Renderer::RenderWithCamera(particle.RenderTarget, m_MainCamera);
        }
        //Renderer::RenderWithCamera(m_Sphere, m_MainCamera);
        // Renderer::RenderWithCamera(m_box, m_MainCamera);
    }
};