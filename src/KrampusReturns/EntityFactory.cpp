

#include <KrampusReturns/EntityFactory.hpp>

#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <KrampusReturns/Entities/Blob.hpp>
#include <KrampusReturns/Entities/Goalpost.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{


EntityFactory::EntityFactory(AllegroFlare::EventEmitter* event_emitter)
   : AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory()
   , event_emitter(event_emitter)
   , init_entities_drawing_debug(false)
{
}


EntityFactory::~EntityFactory()
{
}


void EntityFactory::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   this->event_emitter = event_emitter;
}


void EntityFactory::set_init_entities_drawing_debug(bool init_entities_drawing_debug)
{
   this->init_entities_drawing_debug = init_entities_drawing_debug;
}


AllegroFlare::EventEmitter* EntityFactory::get_event_emitter() const
{
   return event_emitter;
}


bool EntityFactory::get_init_entities_drawing_debug() const
{
   return init_entities_drawing_debug;
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

   // dependencies

   KrampusReturns::Entities::Krampus *result = new KrampusReturns::Entities::Krampus();
   result->set_animation_book(get_animation_book());
   result->set_event_emitter(event_emitter);
   result->initialize();

   // location

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);

   if (init_entities_drawing_debug) result->set_draw_debug(true);
   //if (true) result->set_draw_debug(true);

   return result;
}

KrampusReturns::Entities::Blob* EntityFactory::create_blob(std::string on_map, float x, float y)
{
   if (!(get_animation_book()))
   {
      std::stringstream error_message;
      error_message << "[EntityFactory::create_blob]: error: guard \"get_animation_book()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("EntityFactory::create_blob: error: guard \"get_animation_book()\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::Blob *result = new KrampusReturns::Entities::Blob();
   result->set_animation_book(get_animation_book());
   result->initialize();

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);

   if (init_entities_drawing_debug) result->set_draw_debug(true);
   //get_platforming_2d_ref().add_entity_to_pool(result);
   return result;
}

KrampusReturns::Entities::Goalpost* EntityFactory::create_goalpost(std::string on_map, int goalpost_id, float x, float y)
{
   if (!(get_animation_book()))
   {
      std::stringstream error_message;
      error_message << "[EntityFactory::create_goalpost]: error: guard \"get_animation_book()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("EntityFactory::create_goalpost: error: guard \"get_animation_book()\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::Goalpost *result = new KrampusReturns::Entities::Goalpost();
   result->set_animation_book(get_animation_book());
   result->set("goalpost_id", goalpost_id);
   result->initialize();

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);

   if (init_entities_drawing_debug) result->set_draw_debug(true);
   //get_platforming_2d_ref().add_entity_to_pool(result);
   return result;
}

KrampusReturns::Entities::FlashEffect* EntityFactory::create_flash_fx1(std::string on_map, float x, float y)
{
   if (!(get_animation_book()))
   {
      std::stringstream error_message;
      error_message << "[EntityFactory::create_flash_fx1]: error: guard \"get_animation_book()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("EntityFactory::create_flash_fx1: error: guard \"get_animation_book()\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::FlashEffect *result = new KrampusReturns::Entities::FlashEffect();
   result->set_animation_book(get_animation_book());
   result->initialize();

   // NOTE: for the time being FlashEffect sets the animation on initialization, so in this factory method
   // it's set here to build the object:
   result->set_animation("flash_fx1");

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);

   if (init_entities_drawing_debug) result->set_draw_debug(true);
   //get_platforming_2d_ref().add_entity_to_pool(result);
   return result;
}


} // namespace KrampusReturns


