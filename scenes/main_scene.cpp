#include "main_scene.hpp"
#include <core/event/input_poll.hpp>
#include <core/system_framework/system_registry.hpp>
#include <scene/components/components.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <renderer/renderer.hpp>
#include <core/ui/widgets.hpp>

namespace engine3d{
    MainScene::MainScene(){
    }

    MainScene::MainScene(const std::string& Tag) : SceneScope(Tag){

        ConsoleLogFatal("MainScene::MainScene Constructor Called to get world tag = {}", SystemRegistry::GetWorld().GetTag());
        m_MainCamera =  CreateNewObject("Main Camera");
        m_MainCamera->SetComponent<Transform>({
            .Position = {0.f, 1.40f, -7.4f},
            .Scale = {2.80f, 1.f, 3.70f}
        });
        m_MainCamera->AddComponent<PerspectiveCamera>();

        m_Sphere = CreateNewObject("Object 1");
        m_Sphere->SetComponent<Transform>({
            .Position = {0.f, 2.10f, -7.30f},
            .Scale = {.20f,.20f, .20f}
        });

        m_Sphere->SetComponent<MeshComponent>({"Ball OBJ.obj"});


        sync_update::sync(this, &MainScene::OnUpdate);
        sync_update::attach(this, &MainScene::OnUIUpdate);
        sync_update::submit(this, &MainScene::OnSceneRender);
    }

    void MainScene::OnUpdate(){
        auto perspective_camera_transform = *m_MainCamera->GetComponent<Transform>();
        auto perspective_camera = *m_MainCamera->GetComponent<PerspectiveCamera>();
        auto deltaTime = sync_update::DeltaTime();

        if (InputPoll::IsKeyPressed(ENGINE_KEY_ESCAPE)){
            ApplicationInstance::GetWindow().Close();
        }

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

        //! @note Press shift key to move using the mouse to rotate around
        if(InputPoll::IsKeyPressed(ENGINE_KEY_LEFT_SHIFT)){
            if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_RIGHT)){
                glm::vec2 cursor_pos = InputPoll::GetMousePosition();

                float x_offset = cursor_pos.x;
                float velocity = x_offset * deltaTime;
                perspective_camera.ProcessMouseMovement(-velocity, 0.f);
            }

            if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_LEFT)){
                glm::vec2 cursor_pos = InputPoll::GetMousePosition();

                float x_offset = cursor_pos.x;
                float velocity = x_offset * deltaTime;
                perspective_camera.ProcessMouseMovement(velocity, 0.f);
            }

            if(InputPoll::IsMousePressed(ENGINE_MOUSE_BUTTON_MIDDLE)){
                glm::vec2 cursor_pos = InputPoll::GetMousePosition();

                float velocity = cursor_pos.y * deltaTime;
                perspective_camera.ProcessMouseMovement(0.f, velocity);
            }

            if(InputPoll::IsKeyPressed(ENGINE_KEY_SPACE)){
                glm::vec2 cursor_pos = InputPoll::GetMousePosition();
                float velocity = cursor_pos.y * deltaTime;
                perspective_camera.ProcessMouseMovement(0.f, -velocity);
            }
        }

        perspective_camera.UpdateProjView();


        m_MainCamera->SetComponent<engine3d::PerspectiveCamera>(perspective_camera);
        m_MainCamera->SetComponent<engine3d::Transform>(perspective_camera_transform);
    }

    void MainScene::OnUIUpdate(){
        // Getting our sphere transform to modify it
        auto sphere_data = *m_Sphere->GetComponent<Transform>();

        //! @note Basic Properties Panel
        if(ImGui::Begin("Properties Panel")){
            //! @note THERE IS AN ERROR. Where if the imgui docking window is outside of the window
            //! @note Imgui will just have a window that appears until when you exit the application and the UI is not docked outside the window
            ui::DrawPanelComponent<MeshComponent>("Sphere", [&](){

                ui::DrawVec3UI("pos 1", sphere_data.Position);
                ui::DrawVec3UI("scale 1", sphere_data.Scale);
                ui::DrawVec3UI("rotate 1", sphere_data.Rotation);
                ui::LoadFileWithUI("Load Mesh 1", m_MeshFilepath);
                
                if(m_MeshFilepath != ""){
                    std::filesystem::path relative_path = std::filesystem::relative(m_MeshFilepath, "./");
                    ConsoleLogTrace("Filepath = {}", m_MeshFilepath);
                    ConsoleLogTrace("in branch 1 mesh_file = {}", relative_path.string());

                    m_Sphere->SetComponent<MeshComponent>({relative_path.string()});
                    m_MeshFilepath = "";
                }
            });
            ImGui::End();
        }

        m_Sphere->SetComponent<Transform>(sphere_data);
    }

    void MainScene::OnSceneRender(){
        Renderer::RenderWithCamera(m_Sphere, m_MainCamera);
    }
};