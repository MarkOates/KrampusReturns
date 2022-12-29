

#include <KrampusReturns/Entities/Blob.hpp>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Entities
{


Blob::Blob()
   : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
   , initialized(false)
   , preferred_direction(AllegroFlare::Vec2D(1, 0))
   , preferred_direction_started_at(0.0f)
   , preferred_direction_next_update_duration(0.0f)
   , health(2)
   , max_health(2)
{
}


Blob::~Blob()
{
}


int Blob::get_health() const
{
   return health;
}


int Blob::get_max_health() const
{
   return max_health;
}


void Blob::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Blob::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Blob::initialize: error: guard \"(!initialized)\" not met");
   }
   get_place_ref().size = { 15, 7 };
   get_bitmap_placement_ref().scale = { 0.6, 0.6 };
   set_bitmap_alignment_strategy("bottom_centered");
   set("type", "blob");
   set_animation("blob");
   set_preferred_direction();

   initialized = true;
   return;
}

void Blob::update()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Blob::update]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Blob::update: error: guard \"initialized\" not met");
   }
   FrameAnimated2D::update();

   float counter = al_get_time();
   float time_now = al_get_time();

   float age = time_now - preferred_direction_started_at;

   if (age > preferred_direction_next_update_duration)
   {
      set_preferred_direction();
      // TODO: update randomly to update preferred_direction_next_update_duration;
   }


   // behavior

   AllegroFlare::Vec2D wander_vector = {
      (float)sin(counter) * 0.2f,
      (float)cos(counter*0.78) * 0.3f,
   };
   wander_vector *= 1.3;

   //get_velocity_ref().position *= 0.01;

   get_velocity_ref().position = preferred_direction * 0.2 + wander_vector * 0.3;


   // appearance

   get_bitmap_placement_ref().rotation = (sin(counter*3) * sin(counter*2.3)) * 0.1;

   return;
}

void Blob::take_damage(int amount)
{
   if (health <= 0) return;
   health -= amount;
   if (health < 0) health = 0;
   return;
}

void Blob::set_preferred_direction()
{
   preferred_direction_started_at = al_get_time();
   float random_seed = preferred_direction_started_at;

   AllegroFlare::Vec2D wander_vector = {
      (float)sin(random_seed * 99979.2f * get_id()), // just some random numbers
      (float)cos(random_seed * 68896.71f * get_id()),
   };
   //wander_vector *= 1.3;
   preferred_direction = wander_vector.normalized();

   return;
}


} // namespace Entities
} // namespace KrampusReturns


