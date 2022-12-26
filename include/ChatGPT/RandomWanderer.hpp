#pragma once

#include <AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/Base.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>

namespace ChatGPT {

class RandomWanderer : public AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base
{
public:
    RandomWanderer(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity, float speed);

    virtual void update() override;

private:
    AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity_;
    float speed_;
    int move_time_;
    int move_direction_;
};

} // namespace ChatGPT

