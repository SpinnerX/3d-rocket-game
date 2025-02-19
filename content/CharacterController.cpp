#include <core/geometry/mesh.hpp>

#include "CharacterController.hpp"


CharacterController::CharacterController() {}

CharacterController::CharacterController(engine3d::SceneScope *p_current_scene)
{
    
    m_ObjectHandler = p_current_scene->CreateNewObject("Rocket");
    m_ObjectHandler->SetComponent<engine3d::Transform>({
        .Position = {7.f, 2.10f, -7.30f},
        .Rotation = {0.f, 0.f, 90.f},
        .Scale = {.20f,.20f, .20f}
    });
    m_ObjectHandler->SetComponent<engine3d::MeshComponent>({"C:/DEV/ENGINE3D/3d-rocket-game/3d_models/wallacegrommit.obj"});
}

CharacterController::~CharacterController() {}

void CharacterController::setLinearVelocity(engine3d::Transform moveVector)
{
    CharacterController::velocity = moveVector.Position;
}
glm::vec3 CharacterController::getLinearVelocity() 
{ 
    return CharacterController::velocity;
}
glm::vec3 CharacterController::getRotationalVelocity() 
{ 
    return CharacterController::rotation; 
}
void CharacterController::setAcceleration() {}

void CharacterController::OnUpdate() {}

void CharacterController::OnPhysicsUpdate() {}


