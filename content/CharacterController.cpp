#include "CharacterController.hpp"
#include <core/geometry/mesh.hpp>

CharacterController::CharacterController() {}

CharacterController::CharacterController(engine3d::SceneScope *p_current_scene)
{
    m_ObjectHandler = p_current_scene->CreateNewObject("Rocket");
    m_ObjectHandler->SetComponent<engine3d::Transform>({
        .Position = {7.f, 2.10f, -7.30f},
        .Scale = {.20f,.20f, .20f}
    });

    m_ObjectHandler->SetComponent<engine3d::MeshComponent>({"C:/DEV/ENGINE3D/3d-rocket-game/3d_models/wallacegrommit.obj"});
}

CharacterController::~CharacterController() {}

void setLinearVelocity(engine3d::Transform moveVector)
{
    
}
glm::vec3 CharacterController::getLinearVelocity() 
{ 
    glm::vec3 speed = m_ObjectHandler->GetComponent<engine3d::Transform>()->Position;
    float deltaTime = engine3d::sync_update::DeltaTime();
    
    return speed;
}
void CharacterController::setAcceleration() {}

void CharacterController::OnUpdate() {}

void CharacterController::OnPhysicsUpdate() {}


