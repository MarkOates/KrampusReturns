

#include <KrampusReturns/Entities/SpawnPoint.hpp>

#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Entities
{


SpawnPoint::SpawnPoint()
   : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
   , initialized(false)
{
}


SpawnPoint::~SpawnPoint()
{
}


void SpawnPoint::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[SpawnPoint::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SpawnPoint::initialize: error: guard \"(!initialized)\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D;

   get_place_ref().size = { 16, 16 };
   get_bitmap_placement_ref().scale = { 0.6, 0.6 };
   set_bitmap_alignment_strategy("centered");
   //set_draw_debug(true);
   set(EntityFlagNames::TYPE, "spawn_point"); // TODO: make EntityFlagNames::SPAWN_POINT
   //set(EntityFlagNames::TYPE, EntityFlagNames::SPAWN_POINT);
   set_animation("spawn_point"); // TODO: change this type


   initialized = true;
   return;
}

void SpawnPoint::update()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[SpawnPoint::update]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SpawnPoint::update: error: guard \"initialized\" not met");
   }
   FrameAnimated2D::update();
   get_bitmap_placement_ref().rotation += 0.1;
   return;
}

void SpawnPoint::draw()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[SpawnPoint::draw]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("SpawnPoint::draw: error: guard \"initialized\" not met");
   }
   FrameAnimated2D::draw();
   return;
}


} // namespace Entities
} // namespace KrampusReturns


