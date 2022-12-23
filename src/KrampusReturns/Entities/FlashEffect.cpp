

#include <KrampusReturns/Entities/FlashEffect.hpp>

#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Entities
{


FlashEffect::FlashEffect()
   : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
   , initialized(false)
{
}


FlashEffect::~FlashEffect()
{
}


void FlashEffect::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[FlashEffect::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("FlashEffect::initialize: error: guard \"(!initialized)\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D;

   get_place_ref().size = { 0, 0 };
   get_bitmap_placement_ref().scale = { 0.65, 0.65 };
   set_bitmap_alignment_strategy("centered");
   set_animation("flash_fx1"); // TODO: consider changing this, or setting it to a different default

   initialized = true;
   return;
}

void FlashEffect::update()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[FlashEffect::update]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("FlashEffect::update: error: guard \"initialized\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D;
   FrameAnimated2D::update();

   if (get_animation_finished()) set(EntityFlagNames::PLEASE_DELETE);
   return;
}


} // namespace Entities
} // namespace KrampusReturns


