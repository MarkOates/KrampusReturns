#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2DFactory.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <string>


namespace KrampusReturns
{
   class EntityFactory : public AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory
   {
   private:

   protected:


   public:
      EntityFactory();
      ~EntityFactory();

      KrampusReturns::Entities::Krampus* create_krampus(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f);
   };
}



