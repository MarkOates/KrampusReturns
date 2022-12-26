#include "ChatGPT/Seeker.hpp"
#include <cmath>

namespace ChatGPT {

Seeker::Seeker(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity, AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target, float speed)
    : entity_(entity)
    , target_(target)
    , speed_(speed)
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
    float length = std::sqrt(dx * dx + dy * dy);
    if (length > 0)
    {
        dx /= length;
        dy /= length;
        dx *= speed_;
        dy *= speed_;
    }

    // set the velocity of the entity based on the calculated displacement
    entity_->get_velocity_ref().position = {dx, dy};
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
