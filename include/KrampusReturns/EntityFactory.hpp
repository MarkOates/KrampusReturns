#pragma once


#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2DFactory.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>
#include <ChatGPT/Enemy.hpp>
#include <KrampusReturns/Entities/Blob.hpp>
#include <KrampusReturns/Entities/DamageZone.hpp>
#include <KrampusReturns/Entities/FlashEffect.hpp>
#include <KrampusReturns/Entities/Goalpost.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <KrampusReturns/Entities/SpawnPoint.hpp>
#include <cstdint>
#include <string>


namespace KrampusReturns
{
   class EntityFactory : public AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory
   {
   private:
      AllegroFlare::EventEmitter* event_emitter;
      bool init_entities_drawing_debug;
      AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* _create_collectable(std::string map_name="[map-name-not-set]", float x=0.0f, float y=0.0f, std::string type_name="[unset-type_name]", std::string animation="generic_powerup") const;

   protected:


   public:
      EntityFactory(AllegroFlare::EventEmitter* event_emitter=nullptr);
      ~EntityFactory();

      void set_event_emitter(AllegroFlare::EventEmitter* event_emitter);
      void set_init_entities_drawing_debug(bool init_entities_drawing_debug);
      AllegroFlare::EventEmitter* get_event_emitter() const;
      bool get_init_entities_drawing_debug() const;
      KrampusReturns::Entities::Krampus* create_krampus(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f) const;
      KrampusReturns::Entities::Goalpost* create_goalpost(std::string on_map="[unset-on_map]", int goalpost_id=-1, float x=0.0f, float y=0.0f) const;
      KrampusReturns::Entities::SpawnPoint* create_spawn_point(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f) const;
      KrampusReturns::Entities::FlashEffect* create_flash_fx1(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f) const;
      KrampusReturns::Entities::Blob* create_blob(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f) const;
      ChatGPT::Enemy* create_skeleton_enemy(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f, std::string animation="[unset-animation]") const;
      ChatGPT::Enemy* create_flaming_skull_enemy(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f, std::string animation="[unset-animation]", AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target=nullptr) const;
      ChatGPT::Enemy* create_flaming_skull_red_enemy(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f, std::string animation="[unset-animation]", AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target=nullptr) const;
      ChatGPT::Enemy* create_skull_head_boss_enemy(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f, float area_width=0.0f, float area_height=0.0f, std::string animation="[unset-animation]", AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* target=nullptr) const;
      KrampusReturns::Entities::DamageZone* create_damage_zone_by_player(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f, float w=20.0f, float h=8.0f, int damage=1, uint32_t direction_of_force=KrampusReturns::Entities::DamageZone::DIRECTION_OF_FORCE_UNDEF);
      AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* create_attack_up_item(std::string map_name="[map-name-not-set]", float x=0.0f, float y=0.0f) const;
      AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* create_full_health_item(std::string map_name="[map-name-not-set]", float x=0.0f, float y=0.0f) const;
   };
}



