#pragma once


#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2DFactory.hpp>
#include <KrampusReturns/Entities/Blob.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <string>


namespace KrampusReturns
{
   class EntityFactory : public AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory
   {
   private:
      AllegroFlare::EventEmitter* event_emitter;
      bool init_entities_drawing_debug;

   protected:


   public:
      EntityFactory(AllegroFlare::EventEmitter* event_emitter=nullptr);
      ~EntityFactory();

      void set_event_emitter(AllegroFlare::EventEmitter* event_emitter);
      void set_init_entities_drawing_debug(bool init_entities_drawing_debug);
      AllegroFlare::EventEmitter* get_event_emitter() const;
      bool get_init_entities_drawing_debug() const;
      KrampusReturns::Entities::Krampus* create_krampus(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f);
      KrampusReturns::Entities::Blob* create_blob(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f);
   };
}



