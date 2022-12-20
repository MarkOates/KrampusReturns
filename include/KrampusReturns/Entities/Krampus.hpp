#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>


namespace KrampusReturns
{
   namespace Entities
   {
      class Krampus : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D
      {
      public:
         static constexpr char* TYPE = (char*)"Entities/Krampus";

      private:
         bool initialized;

      protected:


      public:
         Krampus();
         ~Krampus();

         void face_left();
         void face_right();
         void initialize();
      };
   }
}



