#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2D.hpp>


namespace KrampusReturns
{
   class EntityFactory
   {
   private:

   protected:


   public:
      EntityFactory();
      ~EntityFactory();

      AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* create_krampus();
   };
}



