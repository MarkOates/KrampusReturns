

#include <KrampusReturns/Entities/Krampus.hpp>

#include <AllegroFlare/Errors.hpp>
#include <KrampusReturns/Entities/DamageZone.hpp>
#include <KrampusReturns/GameEventDatas/SpawnDamageZoneByPlayer.hpp>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Entities
{


Krampus::Krampus(AllegroFlare::EventEmitter* event_emitter)
   : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
   , event_emitter(event_emitter)
   , state(STATE_UNDEF)
   , state_changed_at(0.0f)
   , state_is_busy(false)
   , attack_hit_activated(false)
   , health(5)
   , max_health(5)
   , max_attack_strength(3)
   , attack_strength(1)
   , stunned_from_damage_at(0.0)
   , invincible_from_taking_damage_at(0.0)
   , invincible_from_taking_damage(false)
   , initialized(false)
{
}


Krampus::~Krampus()
{
}


void Krampus::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   this->event_emitter = event_emitter;
}


int Krampus::get_health() const
{
   return health;
}


int Krampus::get_max_health() const
{
   return max_health;
}


int Krampus::get_max_attack_strength() const
{
   return max_attack_strength;
}


int Krampus::get_attack_strength() const
{
   return attack_strength;
}


bool Krampus::get_invincible_from_taking_damage() const
{
   return invincible_from_taking_damage;
}


void Krampus::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Krampus::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Krampus::initialize: error: guard \"(!initialized)\" not met");
   }
   get_place_ref().size = { 32, 6 };
   get_bitmap_placement_ref().scale = { 0.7, 0.7 };
   set_bitmap_alignment_strategy("bottom_centered");
   set_state(STATE_STANDING);
   initialized = true;
   return;
}

bool Krampus::set_state(uint32_t state, float time_now)
{
   if (this->state == state) return false; // TODO: consider "override_if_same" option
   if (state_is_busy) return false;

   uint32_t exiting_state = this->state;
   switch (exiting_state)
   {
      case STATE_WALKING:
         get_bitmap_placement_ref().anchor = { 0, 0 }; // clear the bounce counter
      break;
   }

   this->state = state;
   state_changed_at = time_now;

   switch (state)
   {
      case STATE_STANDING:
         set_animation("krampus");
         set_animation_playback_rate(1.0);
      break;

      case STATE_WALKING:
         set_animation("krampus");
         set_animation_playback_rate(1.7);
      break;

      case STATE_DYING:
         state_is_busy = true;
         set_animation("dying");
         invincible_from_taking_damage = true;
         get_velocity_ref().position.x = 0.0;
         get_velocity_ref().position.y = 0.0;
         set_animation_playback_rate(1.0);
         emit_player_died_event();
      break;

      case STATE_STUNNED_FROM_TAKING_DAMAGE:
         set_animation("take_damage");
         set_animation_playback_rate(1.0);
         get_velocity_ref().position.x = 0.0;
         get_velocity_ref().position.y = 0.0;
         state_is_busy = true;
         invincible_from_taking_damage = true;
         invincible_from_taking_damage_at = time_now;
         emit_bump_camera_shake_event();
         emit_take_damage_sound_effect();
      break;

      case STATE_ATTACKING:
         set_animation("attack");
         set_animation_playback_rate(1.0);
         get_velocity_ref().position.x = 0.0;
         get_velocity_ref().position.y = 0.0;
         state_is_busy = true;
         attack_hit_activated = false;
      break;

      default:
         AllegroFlare::Errors::throw_error("KrampusReturns::Entities::Krampus::set_state", "unhandled state");
      break;
   }

   return true;
}

bool Krampus::increment_attack_strength()
{
   int previous_attack_strength = attack_strength;
   attack_strength++;
   if (attack_strength >= max_attack_strength) attack_strength = max_attack_strength;
   return (attack_strength != previous_attack_strength);
}

bool Krampus::decrement_attack_strength()
{
   int previous_attack_strength = attack_strength;
   attack_strength--;
   if (attack_strength < 1) attack_strength = 1;
   return (attack_strength != previous_attack_strength);
}

bool Krampus::increment_max_health()
{
   int previous_max_health = max_health;
   max_health++;
   if (max_health >= 10) max_health = 10;
   return (max_health != previous_max_health);
}

bool Krampus::restore_all_health()
{
   health = max_health;
   return true;
}

void Krampus::update()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus::update]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Krampus::update: error: guard \"event_emitter\" not met");
   }
   static int ANIMATION_FRAME_NUM_ON_HIT = 3;
   float time_now = al_get_time();
   AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D::update();
   if (infer_age_of(invincible_from_taking_damage_at, time_now) > 2.5) // TODO: replace 2.5 with "AGE*"
   {
      invincible_from_taking_damage = false;
   }

   switch (state)
   {
      case STATE_STANDING:
        // nothing
      break;

      case STATE_WALKING:
         {
            float bounce_counter = sin(time_now*34)*0.5 + 0.5;
            get_bitmap_placement_ref().anchor = { 0, -(bounce_counter * 3.0f) };
         }
      break;

      case STATE_DYING:
         {
            //float bounce_counter = sin(time_now*34)*0.5 + 0.5;
            //get_bitmap_placement_ref().anchor = { 0, bounce_counter * 3.0f };
         }
      break;

      case STATE_STUNNED_FROM_TAKING_DAMAGE:
         {
            //if (get_animation_finished())
            //{
               //set_animation("krampus");
            //}
            float age = infer_state_age(time_now);
            if (age > 0.5)
            {
               state_is_busy = false;
               set_state(STATE_STANDING);
            }
         }
      break;

      case STATE_ATTACKING:
         if (get_animation_finished())
         {
            state_is_busy = false;
            set_state(STATE_STANDING);
         }
         else if (!attack_hit_activated && (get_current_animation_frame_num() >= ANIMATION_FRAME_NUM_ON_HIT))
         {
            do_impact_hit();
            // TODO: add create emit damage zone
            //emit_bump_camera_shake_event();
            //emit_smash_club_sound_effect();
            attack_hit_activated = true;
         }
      break;
   }

   return;
}

bool Krampus::would_be_lethal_damage(int damage)
{
   return (health - damage <= 0);
}

void Krampus::do_impact_hit()
{
   // NOTE: Data would vary by weapon, for for now, just a basic hit with relatively reasonable range and damage
   float point_of_impact_x = get_place_ref().position.x;
   float point_of_impact_y = get_place_ref().position.y;
   float impact_width = 32;
   float impact_height = 16;
   int damage = attack_strength;
   int32_t direction_of_force = KrampusReturns::Entities::DamageZone::DIRECTION_OF_FORCE_UNDEF;

   float x_distance_from_krampus_center = impact_width * 0.5;
   if (is_facing_right())
   {
      point_of_impact_x += x_distance_from_krampus_center;
      direction_of_force = KrampusReturns::Entities::DamageZone::DIRECTION_OF_FORCE_RIGHT;
   }
   else
   {
      point_of_impact_x -= x_distance_from_krampus_center;
      direction_of_force = KrampusReturns::Entities::DamageZone::DIRECTION_OF_FORCE_LEFT;
   }

   event_emitter->emit_game_event(
      AllegroFlare::GameEvent("spawn_damage_zone_by_player",
         new KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer(
            point_of_impact_x,
            point_of_impact_y,
            impact_width,
            impact_height,
            damage,
            direction_of_force
         )
      )
   );
   emit_bump_camera_shake_event();
   emit_smash_club_sound_effect();
   return;
}

void Krampus::emit_bump_camera_shake_event()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus::emit_bump_camera_shake_event]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Krampus::emit_bump_camera_shake_event: error: guard \"event_emitter\" not met");
   }
   event_emitter->emit_game_event(AllegroFlare::GameEvent("camera_shake_bump"));
}

void Krampus::emit_smash_club_sound_effect()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus::emit_smash_club_sound_effect]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Krampus::emit_smash_club_sound_effect: error: guard \"event_emitter\" not met");
   }
   event_emitter->emit_play_sound_effect_event("smash_club");
}

void Krampus::emit_take_damage_sound_effect()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus::emit_take_damage_sound_effect]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Krampus::emit_take_damage_sound_effect: error: guard \"event_emitter\" not met");
   }
   event_emitter->emit_play_sound_effect_event("krampus_hit");
}

void Krampus::emit_player_died_event()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Krampus::emit_player_died_event]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Krampus::emit_player_died_event: error: guard \"event_emitter\" not met");
   }
   event_emitter->emit_game_event(AllegroFlare::GameEvent("player_died"));
}

void Krampus::stand_still()
{
   if (state == STATE_STANDING || set_state(STATE_STANDING))
   {
      get_velocity_ref().position.x = 0.0;
      get_velocity_ref().position.y = 0.0;
   }
   return;
}

void Krampus::stand_still_x()
{
   //if (state == STATE_STANDING || set_state(STATE_STANDING))
   {
      get_velocity_ref().position.x = 0.0;
      //get_velocity_ref().position.y = 0.0;
   }
   return;
}

void Krampus::stand_still_y()
{
   //if (state == STATE_STANDING || set_state(STATE_STANDING))
   {
      //get_velocity_ref().position.x = 0.0;
      get_velocity_ref().position.y = 0.0;
   }
   return;
}

void Krampus::walk_right()
{
   face_right();
   if (state == STATE_WALKING || set_state(STATE_WALKING))
   {
      get_velocity_ref().position.x = 1.5;
   }
   return;
}

void Krampus::walk_up()
{
   if (state == STATE_WALKING || set_state(STATE_WALKING))
   {
      get_velocity_ref().position.y = -1.5;
   }
   return;
}

void Krampus::walk_down()
{
   if (state == STATE_WALKING || set_state(STATE_WALKING))
   {
      get_velocity_ref().position.y = 1.5;
   }
   return;
}

void Krampus::walk_left()
{
   face_left();
   if (state == STATE_WALKING || set_state(STATE_WALKING))
   {
      get_velocity_ref().position.x = -1.5;
   }
   return;
}

bool Krampus::is_facing_left()
{
   return get_bitmap_flip_h();
}

bool Krampus::is_facing_right()
{
   return !get_bitmap_flip_h();
}

void Krampus::face_left()
{
   set_bitmap_flip_h(true);
   return;
}

void Krampus::face_right()
{
   set_bitmap_flip_h(false);
   return;
}

void Krampus::attack()
{
   // NOTE: the conditional check may not be necessary. The state machine in theory should prevent
   // the player from interrupting the STATE_ATTACKING if it is currently busy (state_is_busy == true).
   if (state != STATE_ATTACKING) set_state(STATE_ATTACKING);
   return;
}

void Krampus::take_hit(int damage)
{
   if (invincible_from_taking_damage) return;
   //if (state == STATE_STUNNED_FROM_TAKING_DAMAGE) return; // TODO: replace this with more recovery time

   // HACK: this line will override the state_is_busy if the state_is_busy is in the context of STATE_ATTACKING
   if (state == STATE_ATTACKING) state_is_busy = false;

   if (would_be_lethal_damage(damage))
   {
      bool state_change_was_successful = set_state(STATE_DYING);
      if (state_change_was_successful) health = 0;
   }
   else
   {
      bool state_change_was_successful = set_state(STATE_STUNNED_FROM_TAKING_DAMAGE);
      if (state_change_was_successful) health -= damage;
   }
   return;
}

float Krampus::infer_state_age(float time_now)
{
   return time_now - state_changed_at;
}

float Krampus::infer_age_of(float event_at, float time_now)
{
   return time_now - event_at;
}


} // namespace Entities
} // namespace KrampusReturns


