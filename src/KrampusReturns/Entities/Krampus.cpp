

#include <KrampusReturns/Entities/Krampus.hpp>

#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Entities
{


Krampus::Krampus()
   : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
   , initialized(false)
{
}


Krampus::~Krampus()
{
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
   get_place_ref().size = { 25, 10 };
   get_bitmap_placement_ref().scale = { 0.8, 0.8 };
   set_bitmap_alignment_strategy("bottom_centered");
   set_animation("krampus");
   initialized = true;
   return;
}


} // namespace Entities
} // namespace KrampusReturns


