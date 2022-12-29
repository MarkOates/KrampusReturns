#pragma once

#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/Base.hpp>

//* Enemy::get_movement_strategy() const { return movement_strategy_; }

//#include "ChatGPT/Base.hpp"

namespace ChatGPT {

class Enemy : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D {
public:
    enum class state_t {
        SPAWNING,
        ATTACKING,
        WANDERING,
        TAKING_DAMAGE,
        DYING,
        DEAD
    };

    Enemy(int max_health, int health, int attack);

    void take_damage(int damage);
    int get_max_health() const;
    int get_health() const;
    int get_attack() const;
    state_t get_state() const;
    AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base *get_movement_strategy() const;

    void set_state(state_t state);
    void set_movement_strategy(AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base* movement_strategy);

    virtual void update() override;

private:
    int max_health_;
    int health_;
    int attack_;
    state_t state_;
    AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base* movement_strategy_;
};

} // namespace ChatGPT
