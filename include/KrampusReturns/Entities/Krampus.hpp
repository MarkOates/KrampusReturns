#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2D.hpp>


namespace KrampusReturns
{
   namespace Entities
   {
      class Krampus : public AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D
      {
      public:
         static constexpr char* TYPE = (char*)"Entities/Krampus";

      private:

      protected:


      public:
         Krampus();
         ~Krampus();

      };
   }
}



