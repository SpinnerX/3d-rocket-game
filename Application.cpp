#include "game_world.hpp"
#include <atlas/core/application.hpp>

class rocket_game_application : public atlas::application{
public:
    rocket_game_application(const atlas::application_settings& p_settings) : atlas::application(p_settings){
        m_MainWorld = atlas::create_ref<rocket_world>("Rocket World");
    }

private:
    atlas::ref<rocket_world> m_MainWorld = nullptr;
};

namespace atlas{
    ref<application> initialize_application(){
        atlas::application_settings p_settings = {
            .Name = "Rocket Game",
            .Width = 1200,
            .Height = 800
        };
        return create_ref<rocket_game_application>(p_settings);
    }
};