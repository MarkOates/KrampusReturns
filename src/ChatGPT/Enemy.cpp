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
    state_ = state;
}

void Enemy::set_movement_strategy(AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base* strategy) { movement_strategy_ = strategy; }

void Enemy::update() {
    if (movement_strategy_) movement_strategy_->update();
    FrameAnimated2D::update();
}

} // namespace ChatGPT




