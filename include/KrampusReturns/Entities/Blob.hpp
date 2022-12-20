#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>
#include <AllegroFlare/Vec2D.hpp>


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
         AllegroFlare::Vec2D preferred_direction;
         float preferred_direction_started_at;

      protected:


      public:
         Blob();
         virtual ~Blob();

         virtual void update() override;
         void set_preferred_direction();
         void initialize();
      };
   }
}


