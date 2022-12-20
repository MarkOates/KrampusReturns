

#include <KrampusReturns/Entities/Blob.hpp>

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
{
}


Blob::~Blob()
{
}


void Blob::walk_left()
{
   set_bitmap_flip_h(true);
   return;
}

void Blob::walk_right()
{
   set_bitmap_flip_h(false);
   return;
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
   get_place_ref().size = { 32, 6 };
   get_bitmap_placement_ref().scale = { 0.6, 0.6 };
   set_bitmap_alignment_strategy("bottom_centered");
   set_animation("blob");
   initialized = true;
   return;
}


} // namespace Entities
} // namespace KrampusReturns


