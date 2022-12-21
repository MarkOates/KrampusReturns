

#include <KrampusReturns/Entities/Krampus.hpp>

#include <AllegroFlare/Errors.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Entities
{


Krampus::Krampus()
   : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
   , state(STATE_UNDEF)
   , state_changed_at(0.0f)
   , initialized(false)
{
}


Krampus::~Krampus()
{
}


void Krampus::update()
{
   AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D::update();

   switch (state)
   {
      case STATE_STANDING:
        // nothing
      break;

      case STATE_WALKING:
        // TODO: bouncing effect
      break;
   }

   return;
}

void Krampus::set_state(uint32_t state, float time_now)
{
   if (this->state == state) return; // TODO: consider "override_if_same" option

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

      default:
         AllegroFlare::Errors::throw_error("KrampusReturns::Entities::Krampus::set_state", "unhandled state");
      break;
   }

   return;
}

void Krampus::stand_still()
{
   if (state != STATE_STANDING) set_state(STATE_STANDING);
   get_velocity_ref().position.x = 0.0;
   get_velocity_ref().position.y = 0.0;
   return;
}

void Krampus::walk_right()
{
   face_right();
   if (state != STATE_WALKING) set_state(STATE_WALKING);
   get_velocity_ref().position.x = 1.5;
   return;
}

void Krampus::walk_up()
{
   if (state != STATE_WALKING) set_state(STATE_WALKING);
   get_velocity_ref().position.y = -1.5;
   return;
}

void Krampus::walk_down()
{
   if (state != STATE_WALKING) set_state(STATE_WALKING);
   get_velocity_ref().position.y = 1.5;
   return;
}

void Krampus::walk_left()
{
   face_left();
   if (state != STATE_WALKING) set_state(STATE_WALKING);
   get_velocity_ref().position.x = -1.5;
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
   set_animation("attack");
   return;
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

float Krampus::infer_state_age(float time_now)
{
   return time_now - state_changed_at;
}


} // namespace Entities
} // namespace KrampusReturns


