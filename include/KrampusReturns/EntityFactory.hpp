#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2D.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2DFactory.hpp>
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

      AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* create_krampus(std::string on_map="[unset-on_map]", float x=0.0f, float y=0.0f);
   };
}



