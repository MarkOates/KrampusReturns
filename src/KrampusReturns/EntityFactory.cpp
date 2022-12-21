

#include <KrampusReturns/EntityFactory.hpp>

#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{


EntityFactory::EntityFactory(AllegroFlare::EventEmitter* event_emitter)
   : AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory()
   , event_emitter(event_emitter)
{
}


EntityFactory::~EntityFactory()
{
}


void EntityFactory::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   this->event_emitter = event_emitter;
}


AllegroFlare::EventEmitter* EntityFactory::get_event_emitter() const
{
   return event_emitter;
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
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[EntityFactory::create_krampus]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("EntityFactory::create_krampus: error: guard \"event_emitter\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::Krampus *result = new KrampusReturns::Entities::Krampus();
   result->set_animation_book(get_animation_book());
   result->set_event_emitter(event_emitter);
   result->initialize();

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);

   //if (init_entities_drawing_debug) result->set_draw_debug(true);
   if (true) result->set_draw_debug(true);

   return result;
}


} // namespace KrampusReturns


