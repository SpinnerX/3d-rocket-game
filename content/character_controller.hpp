#pragma once
#include <core/scene/world.hpp>
#include <core/scene/scene.hpp>
#include <core/core.hpp>
#include <core/update_handlers/sync_update.hpp>
#include <core/event/event.hpp>

struct particle{
    atlas::ref<atlas::scene_object> RenderTarget=nullptr;
    bool IsAlive = true;
    glm::vec3 Velocity;
    glm::vec3 VelocityVariation;

    float LifeTime=1.f;
    float LifeRemaining=0.f;


    operator atlas::ref<atlas::scene_object>(){ return RenderTarget; }

    operator atlas::ref<atlas::scene_object>() const { return RenderTarget; }
};

class character_controller {
public:
    character_controller();
    character_controller(atlas::scene_scope* p_current_scene);
    virtual ~character_controller();
    void set_acceleration(glm::vec3 acceler); 
    glm::vec3 get_linear_velocity();
    glm::vec3 get_rotation_velocity();
    glm::vec3 get_location();
    void on_update();
    void on_physics_update();
    void set_linear_velocity(atlas::Transform moveVector);

    operator atlas::ref<atlas::scene_object>(){ return m_player_object; }

    // operator std::vector<particle>() const { return m_particles; }
    std::vector<particle> get_particles() const { return m_particles; }

private:
    void emit_particles();

private:
    glm::vec3 velocity;
    glm::vec3 rotation;
    glm::vec3 rotationAcceler;
    glm::vec3 location;
    glm::vec3 acceleration;
    std::string objectref = "";
    atlas::ref<atlas::scene_object> m_player_object;

    // Particle stuff
    glm::vec3 m_particles_origin; // origin of the rocket itself
    std::vector<particle> m_particles;

    uint32_t m_active_particle_count = 0;
    glm::vec3 m_velocity;
    glm::vec3 m_velocity_variation;

    
    
private:
    
};