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
         float preferred_direction_next_update_duration;

      protected:


      public:
         Blob();
         virtual ~Blob();

         void initialize();
         virtual void update() override;
         void set_preferred_direction();
      };
   }
}



