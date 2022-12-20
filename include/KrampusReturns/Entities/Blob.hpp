#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>


namespace KrampusReturns
{
   namespace Entities
   {
      class Blob : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D
      {
      public:
         static constexpr char* TYPE = (char*)"Entities/Blob";

      private:
         bool initialized;

      protected:


      public:
         Blob();
         ~Blob();

         void walk_left();
         void walk_right();
         void initialize();
      };
   }
}



