#pragma once

#include <AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/Base.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>

namespace ChatGPT {

class Seeker : public AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base {
public:
    Seeker(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity, AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target, float speed);

    void update() override;
    void set_target(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target);
    AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* get_target();

private:
    AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity_;
    AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target_;
    float speed_;
};

} // namespace ChatGPT

