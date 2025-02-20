#include <core/event/input_poll.hpp>
#include <core/event/key_codes.hpp>
#include <core/geometry/mesh.hpp>
#include <scene/components/components.hpp>

#include "CharacterController.hpp"

CharacterController::CharacterController() {}

CharacterController::CharacterController(engine3d::SceneScope *p_current_scene)
{
    // moon : 240.f, 131.f, 58.f, 1.f
    m_ObjectHandler = p_current_scene->CreateNewObject("Rocket");
    m_ObjectHandler->SetComponent<engine3d::Transform>({
        .Position = {0.f, 0.f, 0.f},
        .Rotation = {-1.57079632679f, 0.f, 1.57079632679f},
        .Scale = {.20f,.20f, .20f},
        .Color = {170.f, 30.f, 2.f, 0.f}
    });
    m_ObjectHandler->SetComponent<engine3d::MeshComponent>({"3d_models/wallacegrommit.obj"});
    this->velocity = {0.f, 0.f, -0.5f}; 
    this->acceleration = {0.f, 1.f, 0.f};
    this->rotationAcceler = {0.f, 1.f, 0.f};
    this->rotation = {0.f, 0.f, 0.f};
    

    
    engine3d::sync_update::sync(this, &CharacterController::OnUpdate);
    engine3d::sync_update::sync_physics(this, &CharacterController::OnPhysicsUpdate);
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
void CharacterController::setAcceleration(glm::vec3 acceler) 
{
    this->acceleration = acceler;
}

void CharacterController::OnUpdate() {}


void CharacterController::OnPhysicsUpdate() 
{
    float deltaTime = engine3d::sync_update::DeltaTime();
    if (engine3d::InputPoll::IsKeyPressed(ENGINE_KEY_UP))
    {
        this->velocity += this->acceleration*deltaTime;
        this->rotation -= this->rotationAcceler*deltaTime;
    }
    if (engine3d::InputPoll::IsKeyPressed(ENGINE_KEY_DOWN))
    {
        this->velocity -= this->acceleration*deltaTime;
        this->rotation += this->rotationAcceler*deltaTime;
    }
    
    if( !(-.5f < m_ObjectHandler->GetComponent<engine3d::Transform>()->Rotation.y ) )
    {
        this->rotation = this->rotationAcceler*deltaTime;
    }
    else if ( !(m_ObjectHandler->GetComponent<engine3d::Transform>()->Rotation.y < .5f))
    {
        this->rotation = -this->rotationAcceler*deltaTime;
    }

    

    
    m_ObjectHandler->SetComponent<engine3d::Transform>({
        .Position = m_ObjectHandler->GetComponent<engine3d::Transform>()->Position + this->velocity*deltaTime,
        .Rotation = m_ObjectHandler->GetComponent<engine3d::Transform>()->Rotation + this->rotation*deltaTime,
        .Scale = {.20f,.20f, .20f},
        .Color = {170.f, 30.f, 2.f, 0.f}
    });
}

// void CharacterController::updating()
// {
//     printf("My update works!!\n");
    
//     m_ObjectHandler->SetComponent<engine3d::Transform>({
//         .Position = m_ObjectHandler->GetComponent<engine3d::Transform>()->Position + velocity,
//         .Rotation = m_ObjectHandler->GetComponent<engine3d::Transform>()->Rotation + rotation
//     });
// }

