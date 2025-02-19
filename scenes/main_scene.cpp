#include "main_scene.hpp"
#include <core/ui/widgets.hpp>

constexpr int ROTATION_DIRECTION_X = 1;
constexpr int ROTATION_DIRECTION_Y = 1;

namespace engine3d{
    MainScene::MainScene(){}

    MainScene::MainScene(const std::string& Tag) : SceneScope(Tag){

        ConsoleLogFatal("MainScene::MainScene Constructor Called to get world tag = {}", SystemRegistry::GetWorld().GetTag());
        m_MainCamera =  CreateNewObject("Main Camera");
        m_MainCamera->SetComponent<Transform>({
            .Position = {7.f, 2.10f, -7.4f},
            .Scale = {2.80f, 1.f, 3.70f}
        });
        m_MainCamera->AddComponent<PerspectiveCamera>();

        m_Rocket = CharacterController(this);

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
    
    PerspectiveCamera MainScene::getViewPortControl(PerspectiveCamera& perspective_camera, float deltaTime, bool click_check)
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

        return perspective_camera;
    }

    void MainScene::OnUpdate()
    {
        Transform perspective_camera_transform = *m_MainCamera->GetComponent<Transform>();
        PerspectiveCamera perspective_camera = *m_MainCamera->GetComponent<PerspectiveCamera>();
        float deltaTime = sync_update::DeltaTime();
        
        PerspectiveCamera camera = getViewPortControl(perspective_camera, deltaTime, on_click_check );
        camera.UpdateProjView();

        m_MainCamera->SetComponent<engine3d::PerspectiveCamera>(camera);
        m_MainCamera->SetComponent<engine3d::Transform>(perspective_camera_transform);
    }

    void MainScene::OnUIUpdate(){
        //Getting our sphere transform to modify it
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
        Renderer::RenderWithCamera(m_Rocket, m_MainCamera);

    }
};