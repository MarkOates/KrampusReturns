#include "ChatGPT/Enemy.hpp"

namespace ChatGPT {

Enemy::Enemy(int health, int attack)
    : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
    , health_(health)
    , attack_(attack)
    , state_(state_t::SPAWNING)
    , movement_strategy_(nullptr)
{
}

void Enemy::take_damage(int damage) {
    health_ = std::max(0, health_ - damage);
    state_ = state_t::TAKING_DAMAGE;
}

int Enemy::get_health() const { return health_; }
int Enemy::get_attack() const { return attack_; }
Enemy::state_t Enemy::get_state() const { return state_; }
AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base* Enemy::get_movement_strategy() const { return movement_strategy_; }

void Enemy::set_state(state_t state) {
    switch (state) {
        case state_t::SPAWNING:
            // TODO: add code to handle the SPAWNING state
            break;
        case state_t::ATTACKING:
            // TODO: add code to handle the ATTACKING state
            break;
        case state_t::WANDERING:
            // TODO: add code to handle the WANDERING state
            break;
        case state_t::TAKING_DAMAGE:
            // TODO: add code to handle the TAKING_DAMAGE state
            break;
        case state_t::DYING:
            // TODO: add code to handle the DYING state
            break;
        case state_t::DEAD:
            // TODO: add code to handle the DEAD state
            break;
    }
    state_ = state;
}

void Enemy::set_movement_strategy(AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base* strategy) { movement_strategy_ = strategy; }

void Enemy::update() {
    if (movement_strategy_) movement_strategy_->update();
    FrameAnimated2D::update();
    switch (state_) {
        case state_t::SPAWNING:
            // TODO: add custom code related to the SPAWNING state
            break;
        case state_t::ATTACKING:
            // TODO: add custom code related to the ATTACKING state
            break;
        case state_t::WANDERING:
            // TODO: add custom code related to the WANDERING state
            break;
        case state_t::TAKING_DAMAGE:
            // TODO: add custom code related to the TAKING_DAMAGE state
            break;
        case state_t::DYING:
            // TODO: add custom code related to the DYING state
            break;
        case state_t::DEAD:
            // TODO: add custom code related to the DEAD state
            break;
    }
}

} // namespace ChatGPT
