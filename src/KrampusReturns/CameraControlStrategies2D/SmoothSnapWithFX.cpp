

#include <KrampusReturns/CameraControlStrategies2D/SmoothSnapWithFX.hpp>

#include <AllegroFlare/Useful.hpp>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace CameraControlStrategies2D
{


SmoothSnapWithFX::SmoothSnapWithFX(float room_width, float room_height)
   : AllegroFlare::CameraControlStrategies2D::Base(KrampusReturns::CameraControlStrategies2D::SmoothSnapWithFX::TYPE)
   , room_width(room_width)
   , room_height(room_height)
   , entity_to_follow(nullptr)
   , tracking_target_position_x(0.0)
   , tracking_target_position_y(0.0)
   , impact_shake_started_at(-9999.0)
   , random({})
{
}


SmoothSnapWithFX::~SmoothSnapWithFX()
{
}


void SmoothSnapWithFX::set_entity_to_follow(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity_to_follow)
{
   this->entity_to_follow = entity_to_follow;
}


AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* SmoothSnapWithFX::get_entity_to_follow() const
{
   return entity_to_follow;
}


void SmoothSnapWithFX::initialize()
{
   if (!((!get_initialized())))
   {
      std::stringstream error_message;
      error_message << "[SmoothSnapWithFX::initialize]: error: guard \"(!get_initialized())\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SmoothSnapWithFX::initialize: error: guard \"(!get_initialized())\" not met");
   }
   if (!(get_camera()))
   {
      std::stringstream error_message;
      error_message << "[SmoothSnapWithFX::initialize]: error: guard \"get_camera()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SmoothSnapWithFX::initialize: error: guard \"get_camera()\" not met");
   }
   if (!((room_width > 0)))
   {
      std::stringstream error_message;
      error_message << "[SmoothSnapWithFX::initialize]: error: guard \"(room_width > 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SmoothSnapWithFX::initialize: error: guard \"(room_width > 0)\" not met");
   }
   if (!((room_height > 0)))
   {
      std::stringstream error_message;
      error_message << "[SmoothSnapWithFX::initialize]: error: guard \"(room_height > 0)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SmoothSnapWithFX::initialize: error: guard \"(room_height > 0)\" not met");
   }
   // TODO: only allow setting "room_width" and "room_height" to positive values

   get_camera_ref()->scale = AllegroFlare::vec2d(1.0 / 4.8, 1.0 / 4.5);
   get_camera_ref()->position = {room_width/2, room_height/2};

   random.set_seed(1337);

   Base::initialize();
   return;
}

void SmoothSnapWithFX::start_impact_shake(float time_now)
{
   impact_shake_started_at = time_now;
   return;
}

void SmoothSnapWithFX::update()
{
   if (!(get_initialized()))
   {
      std::stringstream error_message;
      error_message << "[SmoothSnapWithFX::update]: error: guard \"get_initialized()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SmoothSnapWithFX::update: error: guard \"get_initialized()\" not met");
   }
   if (!(get_camera()))
   {
      std::stringstream error_message;
      error_message << "[SmoothSnapWithFX::update]: error: guard \"get_camera()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SmoothSnapWithFX::update: error: guard \"get_camera()\" not met");
   }
   float time_now = al_get_time();
   float impact_shake_age = calc_impact_shake_age(time_now);


   // Track entity, snapping to frame coordinates

   // If the camera loses track of the "entity_to_follow" (the tracking target dies for example), this logic will
   // retain the last coordinate that had been tracked, and use it until a new tracking target is introduced.

   if (entity_to_follow)
   {
      tracking_target_position_x = entity_to_follow->get_place_ref().position.x;
      tracking_target_position_y = entity_to_follow->get_place_ref().position.y;
   }

   int target_room_x = (tracking_target_position_x / room_width);
   int target_room_y = (tracking_target_position_y / room_height);
   AllegroFlare::vec2d target_position = {room_width/2, room_height/2};
   target_position += AllegroFlare::vec2d(target_room_x * room_width, target_room_y * room_height);

   AllegroFlare::vec2d current_camera_position = get_camera_ref()->position;
   AllegroFlare::vec2d delta = target_position - current_camera_position;
   AllegroFlare::vec2d largest_possible_fabs_delta = AllegroFlare::Vec2D(room_width*0.5, room_height*0.5);

   get_camera_ref()->position += (delta * 0.1);


   // Add impact shake effect

   AllegroFlare::Vec2D max_impact_shake_offset = { random.get_random_float(-5, 5), random.get_random_float(-5, 5) };
   float effect_multiplier = 1.0 - normalized_impact_shake_age(1.0, time_now);
   AllegroFlare::Vec2D impact_shake_offset = max_impact_shake_offset * effect_multiplier;

   get_camera_ref()->position += impact_shake_offset;



   // Add zoom effect based on distance to target:

   float distance = AllegroFlare::distance(get_camera_ref()->position, target_position);
   AllegroFlare::vec2d abs_delta = AllegroFlare::Vec2D(distance * 0.01, distance * 0.01);
   float xtra_zoom = (abs_delta.x + abs_delta.y);
   get_camera_ref()->scale = AllegroFlare::vec2d(1.0 / (4.8 + xtra_zoom), 1.0 / (4.5 + xtra_zoom));

   return;
}

float SmoothSnapWithFX::calc_impact_shake_age(float time_now)
{
   return time_now - impact_shake_started_at;
}

float SmoothSnapWithFX::normalized_impact_shake_age(float duration, float time_now)
{
   if (!((duration >= 0.0001f)))
   {
      std::stringstream error_message;
      error_message << "[SmoothSnapWithFX::normalized_impact_shake_age]: error: guard \"(duration >= 0.0001f)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SmoothSnapWithFX::normalized_impact_shake_age: error: guard \"(duration >= 0.0001f)\" not met");
   }
   return std::max(0.0f, std::min(duration, (time_now - impact_shake_started_at))) / duration;
}


} // namespace CameraControlStrategies2D
} // namespace KrampusReturns


