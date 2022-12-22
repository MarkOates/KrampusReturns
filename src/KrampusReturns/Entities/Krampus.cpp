

#include <KrampusReturns/Entities/Krampus.hpp>

#include <AllegroFlare/Errors.hpp>
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
   get_bitmap_placement_ref().scale = { 0.8, 0.8 };
   set_bitmap_alignment_strategy("bottom_centered");
   set_state(STATE_STANDING);
   initialized = true;
   return;
}

bool Krampus::set_state(uint32_t state, float time_now)
{
   if (this->state == state) return false; // TODO: consider "override_if_same" option
   if (state_is_busy) return false;

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

   switch (state)
   {
      case STATE_STANDING:
        // nothing
      break;

      case STATE_ATTACKING:
         if (get_animation_finished())
         {
            state_is_busy = false;
            set_state(STATE_STANDING);
         }
         else if (!attack_hit_activated && (get_current_animation_frame_num() >= ANIMATION_FRAME_NUM_ON_HIT))
         {
            // TODO: add create emit damage zone
            emit_bump_camera_shake_event();
            emit_smash_club_sound_effect();
            attack_hit_activated = true;
         }
      break;

      case STATE_WALKING:
         float age = infer_state_age(time_now); 
         //float bounce_amount = sin(age * 3.0);

         float bounce_counter = sin(time_now*34)*0.5 + 0.5;
         get_bitmap_placement_ref().anchor = { 0, bounce_counter * 3.0f };
        
         // TODO: bouncing effect
      break;
   }

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
   if (state == STATE_STANDING || set_state(STATE_STANDING))
   {
      get_velocity_ref().position.x = 0.0;
      //get_velocity_ref().position.y = 0.0;
   }
   return;
}

void Krampus::stand_still_y()
{
   if (state == STATE_STANDING || set_state(STATE_STANDING))
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
   if (state != STATE_ATTACKING) set_state(STATE_ATTACKING);
   return;
}

float Krampus::infer_state_age(float time_now)
{
   return time_now - state_changed_at;
}


} // namespace Entities
} // namespace KrampusReturns


