#include "ChatGPT/RandomWanderer.hpp"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <cstdlib>
#include <ctime>

namespace ChatGPT {

RandomWanderer::RandomWanderer(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity, float speed)
    : entity_(entity)
    , speed_(speed)
{
    std::srand(std::time(nullptr));
    move_time_ = std::rand() % 60 + 60;
    move_direction_ = std::rand() % 4;
}

void RandomWanderer::update()
{
    move_time_--;
    if (move_time_ <= 0)
    {
        move_time_ = std::rand() % 60 + 60;
        move_direction_ = std::rand() % 4;
    }

    float x = 0.0;
    float y = 0.0;
    switch (move_direction_)
    {
        case 0:
            x = speed_;
            break;
        case 1:
            x = -speed_;
            break;
        case 2:
            y = speed_;
            break;
        case 3:
            y = -speed_;
            break;
    }

    entity_->get_velocity_ref().position = {x, y};
}

} // namespace ChatGPT

