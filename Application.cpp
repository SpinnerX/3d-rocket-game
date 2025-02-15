#include "game_world.hpp"
#include <engine3d/core/application_instance.hpp>
#include <string>

namespace engine3d{
    class RocketGameApplication : public engine3d::ApplicationInstance{
    public:
        RocketGameApplication(const std::string& Tag) : engine3d::ApplicationInstance(Tag){
            m_MainWorld = CreateRef<RocketWorld>("Rocket World");
        }

    private:
        Ref<RocketWorld> m_MainWorld = nullptr;
    };

    Ref<ApplicationInstance> Initialize(){
        return CreateRef<RocketGameApplication>("Rocket Game");
    }
};