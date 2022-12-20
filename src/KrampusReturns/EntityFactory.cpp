

#include <KrampusReturns/EntityFactory.hpp>

#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{


EntityFactory::EntityFactory()
   : AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory()
{
}


EntityFactory::~EntityFactory()
{
}


KrampusReturns::Entities::Krampus* EntityFactory::create_krampus(std::string on_map, float x, float y)
{
   if (!(get_animation_book()))
   {
      std::stringstream error_message;
      error_message << "[EntityFactory::create_krampus]: error: guard \"get_animation_book()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("EntityFactory::create_krampus: error: guard \"get_animation_book()\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::Krampus *result = new KrampusReturns::Entities::Krampus();
   result->set_animation_book(get_animation_book());
   result->initialize();

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);

   //if (init_entities_drawing_debug) result->set_draw_debug(true);
   if (true) result->set_draw_debug(true);

   return result;
}


} // namespace KrampusReturns


