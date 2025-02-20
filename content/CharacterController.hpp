#pragma once
#include <core/scene/world.hpp>
#include <core/scene/scene.hpp>
#include <core/core.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <core/event/input_poll.hpp>

struct particle{
    engine3d::Ref<engine3d::SceneObject> RenderTarget=nullptr;
    bool IsAlive = true;
    glm::vec3 Velocity;
    glm::vec3 VelocityVariation;

    float LifeTime=1.f;
    float LifeRemaining=0.f;


    operator engine3d::Ref<engine3d::SceneObject>(){ return RenderTarget; }

    operator engine3d::Ref<engine3d::SceneObject>() const { return RenderTarget; }
};

class CharacterController {
public:
    CharacterController();
    CharacterController(engine3d::SceneScope* p_current_scene);
    virtual ~CharacterController();
    void setAcceleration(glm::vec3 acceler); 
    glm::vec3 getLinearVelocity();
    glm::vec3 getRotationalVelocity();
    glm::vec3 getLocation();
    void OnUpdate();
    void OnPhysicsUpdate();
    void setLinearVelocity(engine3d::Transform moveVector);

    operator engine3d::Ref<engine3d::SceneObject>(){ return m_ObjectHandler; }

    // operator std::vector<particle>() const { return m_particles; }
    std::vector<particle> get_particles() const { return m_particles; }

private:
    void EmitParticles();

private:
    glm::vec3 velocity;
    glm::vec3 rotation;
    glm::vec3 rotationAcceler;
    glm::vec3 location;
    glm::vec3 acceleration;
    std::string objectRef = "";
    engine3d::Ref<engine3d::SceneObject> m_ObjectHandler;

    // Particle stuff
    glm::vec3 m_particles_origin; // origin of the rocket itself
    std::vector<particle> m_particles;

    uint32_t m_active_particle_count = 0;
    glm::vec3 m_velocity;
    glm::vec3 m_velocity_variation;

    
    
private:
    
};