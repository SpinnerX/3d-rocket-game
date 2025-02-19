#include <core/geometry/mesh.hpp>

#include "CharacterController.hpp"


CharacterController::CharacterController() {}

CharacterController::CharacterController(engine3d::SceneScope *p_current_scene)
{
    m_ObjectHandler = p_current_scene->CreateNewObject("Rocket");
    m_ObjectHandler->SetComponent<engine3d::Transform>({
        .Position = {0.f, 2.10f, -7.30f},
        .Rotation = {-1.57079632679f, 0.f, 1.57079632679f},
        .Scale = {.20f,.20f, .20f}
    });
    m_ObjectHandler->SetComponent<engine3d::MeshComponent>({"3d_models/wallacegrommit.obj"});
    this->velocity = {0.f, 0.f, -5.f}; 
}

CharacterController::~CharacterController() {}

void CharacterController::setLinearVelocity(engine3d::Transform moveVector)
{
    CharacterController::velocity = moveVector.Position;
}
glm::vec3 CharacterController::getLinearVelocity() 
{ 
    return this->velocity;
}
glm::vec3 CharacterController::getRotationalVelocity() 
{ 
    return this->rotation; 
}
glm::vec3 CharacterController::getLocation()
{
    return this->location;
}
void CharacterController::setAcceleration() {}

void CharacterController::OnUpdate() 
{

}

void CharacterController::OnPhysicsUpdate() 
{
    printf("My update works!!\n");
    
    m_ObjectHandler->SetComponent<engine3d::Transform>({
        .Position = m_ObjectHandler->GetComponent<engine3d::Transform>()->Position + velocity,
        .Rotation = m_ObjectHandler->GetComponent<engine3d::Transform>()->Rotation + rotation
    });
}


