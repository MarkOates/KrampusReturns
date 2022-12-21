#pragma once


#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2DFactory.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <string>


namespace KrampusReturns
{
   class EntityFactory : public AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory
   {
   private:
      AllegroFlare::EventEmitter* event_emitter;

   protected:


   public:
      EntityFactory(AllegroFlare::EventEmitter* event_emitter=nullptr);
      ~EntityFactory();

      void set_event_emitter(AllegroFlare::EventEmitter* event_emitter);
      AllegroFlare::EventEmitter* get_event_emitter() const;
      KrampusReturns::Entities::Krampus* create_krampus(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f);
   };
}



