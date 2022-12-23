

#include <KrampusReturns/Entities/Goalpost.hpp>

#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Entities
{


Goalpost::Goalpost()
   : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
   , initialized(false)
{
}


Goalpost::~Goalpost()
{
}


void Goalpost::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Goalpost::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Goalpost::initialize: error: guard \"(!initialized)\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D;

   get_place_ref().size = { 100, 100 };
   get_bitmap_placement_ref().scale = { 0.6, 0.6 };
   set_bitmap_alignment_strategy("bottom_centered");
   set_draw_debug(true);
   set(EntityFlagNames::TYPE, EntityFlagNames::GOALPOST);
   set_animation("blob"); // TODO: change this type

   initialized = true;
   return;
}

void Goalpost::update()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Goalpost::update]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Goalpost::update: error: guard \"initialized\" not met");
   }
   FrameAnimated2D::update();
   return;
}

void Goalpost::draw()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Goalpost::draw]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Goalpost::draw: error: guard \"initialized\" not met");
   }
   FrameAnimated2D::draw();
   return;
}


} // namespace Entities
} // namespace KrampusReturns


