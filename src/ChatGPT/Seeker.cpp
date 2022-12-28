#include "ChatGPT/Seeker.hpp"
#include <cmath>

namespace ChatGPT {

Seeker::Seeker(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity, AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target, float speed)
    : entity_(entity)
    , target_(target)
    , speed_(speed)
    , is_tracking(false)
{
}

void Seeker::update()
{
    // if there is no target, do not move the entity
    if (!target_)
    {
        entity_->get_velocity_ref().position = {0, 0};
        return;
    }

    // calculate the displacement needed to reach the target
    float dx = target_->get_place().position.x - entity_->get_place().position.x;
    float dy = target_->get_place().position.y - entity_->get_place().position.y;

    // normalize the displacement vector and scale it by the speed
    float distance = std::sqrt(dx * dx + dy * dy);

    if (!is_tracking)
    {
       if (distance < 100)
       {
          is_tracking = true;
       }
    }

    if (is_tracking)
    {
       if (distance > 125)
       {
          is_tracking = false;
       }
    }

    if (is_tracking)
    {
       if (distance > 0)
       {
           dx /= distance;
           dy /= distance;
           dx *= speed_;
           dy *= speed_;
       }

       // set the velocity of the entity based on the calculated displacement
       entity_->get_velocity_ref().position = {dx, dy};
    }
    else
    {
        // not tracking, go to 0 velocity
        entity_->get_velocity_ref().position = {0, 0};
    }
}

void Seeker::set_target(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target)
{
    target_ = target;
}

AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* Seeker::get_target()
{
    return target_;
}

} // namespace ChatGPT
