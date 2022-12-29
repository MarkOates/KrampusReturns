

#include <KrampusReturns/EntityFactory.hpp>

#include <AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/ReflectOffWalls.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <ChatGPT/RandomWanderer.hpp>
#include <ChatGPT/Seeker.hpp>
#include <ChatGPT/Teleporting.hpp>
#include <KrampusReturns/Entities/Blob.hpp>
#include <KrampusReturns/Entities/Goalpost.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <KrampusReturns/Entities/SpawnPoint.hpp>
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


KrampusReturns::Entities::Krampus* EntityFactory::create_krampus(std::string on_map, float x, float y) const
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

KrampusReturns::Entities::Goalpost* EntityFactory::create_goalpost(std::string on_map, int goalpost_id, float x, float y) const
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

KrampusReturns::Entities::SpawnPoint* EntityFactory::create_spawn_point(std::string on_map, float x, float y) const
{
   if (!(get_animation_book()))
   {
      std::stringstream error_message;
      error_message << "[EntityFactory::create_spawn_point]: error: guard \"get_animation_book()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("EntityFactory::create_spawn_point: error: guard \"get_animation_book()\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::SpawnPoint *result = new KrampusReturns::Entities::SpawnPoint();
   result->set_animation_book(get_animation_book());
   //result->set("goalpost_id", goalpost_id);
   result->initialize();

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);

   if (init_entities_drawing_debug) result->set_draw_debug(true);
   //get_platforming_2d_ref().add_entity_to_pool(result);
   return result;
}

KrampusReturns::Entities::FlashEffect* EntityFactory::create_flash_fx1(std::string on_map, float x, float y) const
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

KrampusReturns::Entities::Blob* EntityFactory::create_blob(std::string on_map, float x, float y) const
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

   result->set("damages_player");
   result->set("takes_damage_from_player_damage_zones");

   if (init_entities_drawing_debug) result->set_draw_debug(true);
   //get_platforming_2d_ref().add_entity_to_pool(result);
   return result;
}

ChatGPT::Enemy* EntityFactory::create_skeleton_enemy(std::string on_map, float x, float y, std::string animation) const
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   //AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base
   //AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::ReflectOffWalls
   //headers: [ AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/ReflectOffWalls.hpp ]

   int max_health = 1;
   int health = max_health;
   int attack = 1;
   ChatGPT::Enemy* result = new ChatGPT::Enemy(max_health, health, attack);
   result->set_animation_book(get_animation_book());
   result->set_animation(animation);
   result->get_place_ref().size = { 12, 4 };
   //result->get_place_ref().scale = { 0.8, 0.8 };
   result->set_bitmap_alignment_strategy("bottom_centered");

   // NOTE: this class now needs a proper destruct process
   result->set_movement_strategy(
      //new AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::ReflectOffWalls(result)
      new ChatGPT::RandomWanderer(result, 0.5)
      //new ChatGPT::Seeker(result, 0.01)
   );

   result->set("damages_player");
   result->set("takes_damage_from_player_damage_zones");

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);
   return result;
}

ChatGPT::Enemy* EntityFactory::create_skull_head_enemy(std::string on_map, float x, float y, std::string animation, AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target) const
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   //AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base
   //AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::ReflectOffWalls
   //headers: [ AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/ReflectOffWalls.hpp ]

   int max_health = 1;
   int health = max_health;
   int attack = 1;
   ChatGPT::Enemy* result = new ChatGPT::Enemy(max_health, health, attack);
   result->set_animation_book(get_animation_book());
   result->get_place_ref().size = { 8, 4 };
   //result->get_place_ref().scale = { 0.8, 0.8 };
   result->set_animation(animation);
   result->set_bitmap_alignment_strategy("bottom_centered");

   // NOTE: this class now needs a proper destruct property
   result->set_movement_strategy(
      //new AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::ReflectOffWalls(result)
      //new ChatGPT::RandomWanderer(result, 1)
      new ChatGPT::Seeker(result, target, 1.0)
   );

   result->set("damages_player");
   result->set("takes_damage_from_player_damage_zones");

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);
   result->set("seeker");
   return result;
}

ChatGPT::Enemy* EntityFactory::create_teleporting_boss_enemy(std::string on_map, float x, float y, float area_width, float area_height, std::string animation, AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target) const
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   //AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base
   //AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::ReflectOffWalls
   //headers: [ AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/ReflectOffWalls.hpp ]

   int max_health = 20;
   int health = max_health;
   int attack = 1;
   ChatGPT::Enemy* result = new ChatGPT::Enemy(max_health, health, attack);
   result->set_animation_book(get_animation_book());
   result->set_animation(animation);
   result->set_bitmap_alignment_strategy("bottom_centered");

   // NOTE: this class now needs a proper destruct property
   result->set_movement_strategy(
      //new AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::ReflectOffWalls(result)
      //new ChatGPT::RandomWanderer(result, 1)
      new ChatGPT::Teleporting(result, { x, y }, { area_width, area_height })
   );

   result->set("damages_player");
   result->set("takes_damage_from_player_damage_zones");

   result->get_place_ref().position = { x, y };
   result->set(ON_MAP_NAME, on_map);
   result->set("seeker");
   return result;
}

KrampusReturns::Entities::DamageZone* EntityFactory::create_damage_zone_by_player(std::string on_map, float x, float y, float w, float h, int damage, uint32_t direction_of_force)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::DamageZone* result = new KrampusReturns::Entities::DamageZone(damage);
   result->set("damages_enemies");
   result->set("is_DamageZone_class");
   result->set(EPHEMERAL);

   // HACK: these two lines to set the animation book, even though it's not needed.
   result->set_animation_book(get_animation_book());
   result->set_animation("shadow_small");
   result->set_bitmap_alignment_strategy("bottom_centered");

   // TESTING DEBUG
   result->set_draw_debug(true);

   result->get_place_ref().position = { x, y };
   result->get_place_ref().size = { w, h };
   result->get_place_ref().align = { 0.5, 0.5 }; // conceptually, the "position" is the central point of impact.
   result->set(ON_MAP_NAME, on_map);
   return result;
}


} // namespace KrampusReturns


