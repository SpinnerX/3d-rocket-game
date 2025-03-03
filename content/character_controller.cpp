#include <core/event/event.hpp>
#include <core/event/key_codes.hpp>
#include <core/geometry/mesh.hpp>
#include <core/scene/components.hpp>

#include "character_controller.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

#include <random>
#include "random.hpp"
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(-0.1f, 0.1f); // Random offset for sand effect

static glm::vec4 HSVtoRGB(const glm::vec3& hsv) {
    int H = (int)(hsv.x * 360.0f);
    double S = hsv.y;
    double V = hsv.z;

    double C = S * V;
    double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    double m = V - C;
    double Rs, Gs, Bs;

    if (H >= 0 && H < 60) {
        Rs = C;
        Gs = X;
        Bs = 0;
    }
    else if (H >= 60 && H < 120) {
        Rs = X;
        Gs = C;
        Bs = 0;
    }
    else if (H >= 120 && H < 180) {
        Rs = 0;
        Gs = C;
        Bs = X;
    }
    else if (H >= 180 && H < 240) {
        Rs = 0;
        Gs = X;
        Bs = C;
    }
    else if (H >= 240 && H < 300) {
        Rs = X;
        Gs = 0;
        Bs = C;
    }
    else {
        Rs = C;
        Gs = 0;
        Bs = X;
    }

    return { (Rs + m), (Gs + m), (Bs + m), 1.0f };
}

character_controller::character_controller() {}

character_controller::character_controller(atlas::scene_scope *p_current_scene)
{
    // moon : 240.f, 131.f, 58.f, 1.f
    m_player_object = p_current_scene->create_new_object("Rocket");
    m_player_object->set<atlas::Transform>({
        .Position = {0.f, 15.f, 0.f},
        .Rotation = {-1.57079632679f, 0.f, 1.57079632679f},
        .Scale = {.20f,.20f, .20f},
        .Color = {0.7f, 165.f, 0.f, 0.f}
    });
    m_player_object->set<atlas::MeshComponent>({"3d_models/wallacegrommit.obj"});
    this->velocity = {0.f, 0.f, -1.5f}; 
    this->acceleration = {0.f, 1.f, 0.f};
    this->rotationAcceler = {0.f, 1.f, 0.f};
    this->rotation = {0.f, 0.f, 0.f};
    
    m_particles.resize(1000);

    m_particles_origin = m_player_object->get<atlas::Transform>()->Position;

    for(size_t i = 0; i < m_particles.size(); i++){
        m_particles[i] = {};
        m_particles[i].RenderTarget = p_current_scene->create_new_object(fmt::format("Particle {}", i));
        m_particles[i].RenderTarget->set<atlas::MeshComponent>({"3d_models/tutorial/sphere.obj"});
    }
    
    
    atlas::sync_update::sync(this, &character_controller::on_update);
    atlas::sync_update::sync_physics(this, &character_controller::on_physics_update);
}

character_controller::~character_controller() {}

void character_controller::set_linear_velocity(atlas::Transform moveVector)
{
    character_controller::velocity = moveVector.Position;
}
glm::vec3 character_controller::get_linear_velocity() 
{ 
    return this->velocity;
}
glm::vec3 character_controller::get_rotation_velocity() 
{ 
    return this->rotation; 
}
glm::vec3 character_controller::get_location()
{
    return this->location;
}
void character_controller::set_acceleration(glm::vec3 acceler) 
{
    this->acceleration = acceler;
}

void character_controller::on_update() {}


void character_controller::on_physics_update() 
{
    float delta_time = atlas::sync_update::delta_time();
    if (atlas::event::is_key_pressed(KEY_UP))
    {
        this->velocity += this->acceleration*delta_time;
        this->rotation -= this->rotationAcceler*delta_time;
    }
    if (atlas::event::is_key_pressed(KEY_DOWN))
    {
        this->velocity -= this->acceleration*delta_time;
        this->rotation += this->rotationAcceler*delta_time;
    }

    emit_particles();
    
    if( !(-.5f < m_player_object->get<atlas::Transform>()->Rotation.y ) )
    {
        this->rotation = this->rotationAcceler*delta_time;
    }
    else if ( !(m_player_object->get<atlas::Transform>()->Rotation.y < .5f))
    {
        this->rotation = -this->rotationAcceler*delta_time;
    }
    //Rocket updating
    m_player_object->set<atlas::Transform>({
        .Position = m_player_object->get<atlas::Transform>()->Position + this->velocity*delta_time,
        .Rotation = m_player_object->get<atlas::Transform>()->Rotation + this->rotation*delta_time,
        .Scale = {.20f,.20f, .20f},
        .Color = {0.5f, .6f, 0.f, 0.f}
    });
}

void character_controller::emit_particles(){
    glm::vec4 ColorBegin = {1.f, 109/255.0f, 41/ 255.0f, 1.0f};
    glm::vec4 ColorEnd = {1.f, 212/255.0f, 123/255.0f, 1.0f};
    //! @note This is all of the particles logic.
    float delta_time = atlas::sync_update::delta_time();
    for(auto& obj : m_particles){
        auto transform = obj.RenderTarget->get<atlas::Transform>();
        // float life = particle.lifeRemain / particle.lifeTime;
        // glm::vec4 ColorBegin = {1.f, 109/255.0f, 41/ 255.0f, 1.0f};
        // glm::vec4 ColorEnd = {1.f, 212/255.0f, 123/255.0f, 1.0f};
        float life = obj.LifeRemaining / obj.LifeTime;
        obj.RenderTarget->set<atlas::Transform>({
            // .Position = {atlas::Transform->Position.x, atlas::Transform->Position.y - m_gravity, atlas::Transform->Position.z},
            // .Position = atlas::Transform->Position + m_velocity * delta_time,
            .Position = transform->Position - get_linear_velocity() * delta_time,
            .Rotation = {(Random::Float() * 2.0f * glm::pi<float>()), (Random::Float() * 2.0f * glm::pi<float>()), (Random::Float() * 2.0f * glm::pi<float>())},
            .Scale = transform->Scale,
            // .Color = {0.f, glm::lerp(ColorEnd, ColorBegin, life).y, glm::lerp(ColorEnd, ColorBegin, life).z, 1.f}
            .Color = {0.f, 1.f, 0.f, 0.f}
        });
    }

    //! @note These areeeere particles that are going to be activated (rendered)
    if(m_active_particle_count < m_particles.size()){
        atlas::Transform initial_transform;
        // rocket's current position is the particle's origin
        glm::vec3 rocket_origin_pos = m_player_object->get<atlas::Transform>()->Position;

        initial_transform.Position = rocket_origin_pos + glm::vec3(dis(gen), 0.0f, dis(gen));
        initial_transform.Scale = glm::vec3(0.01f); // Smaller scale for sand
        // initial_transform.Color = glm::vec4(0.8f, 0.7f, 0.6f, 1.0f);
        // initial_transform.Rotation += glm::vec3(dis(gen), 0.0f, dis(gen));
        initial_transform.Rotation = get_rotation_velocity();
        float life = m_particles[m_active_particle_count].LifeRemaining / m_particles[m_active_particle_count].LifeTime;
        initial_transform.Color = HSVtoRGB(glm::lerp(ColorEnd, ColorBegin, life));
        m_particles[m_active_particle_count].RenderTarget->set<atlas::Transform>(initial_transform);

        if(m_particles[m_active_particle_count].LifeRemaining <= 0.0f){
            m_particles[m_active_particle_count].IsAlive = false;
        }

        m_particles[m_active_particle_count].LifeTime -= delta_time;
        m_active_particle_count++;
    }
    else{
        // This just resets the counter so we can continously spawn particles (which u probably dont want to do and have some lifetime to control how long particles live lol)
        m_active_particle_count = 0;
    }

}

// void character_controller::updating()
// {
//     printf("My update works!!\n");
    
//     m_player_object->set<atlas::Transform>({
//         .Position = m_player_object->get<atlas::Transform>()->Position + velocity,
//         .Rotation = m_player_object->get<atlas::Transform>()->Rotation + rotation
//     });
// }

