

#include <KrampusReturns/Entities/Blob.hpp>

#include <cmath>
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


void Blob::update()
{
   FrameAnimated2D::update();

   float counter = al_get_time();


   // behavior

   AllegroFlare::Vec2D jump_vector = {
      (float)sin(counter) * 0.2f,
      (float)cos(counter*0.78) * 0.3f,
   };
   jump_vector *= 1.3;

   //get_velocity_ref().position *= 0.01;

   get_velocity_ref().position = jump_vector;


   // appearance

   get_bitmap_placement_ref().rotation = (sin(counter*3) * sin(counter*2.3)) * 0.1;

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
   get_place_ref().size = { 15, 7 };
   get_bitmap_placement_ref().scale = { 0.6, 0.6 };
   set_bitmap_alignment_strategy("bottom_centered");
   set_animation("blob");
   initialized = true;
   return;
}


} // namespace Entities
} // namespace KrampusReturns


