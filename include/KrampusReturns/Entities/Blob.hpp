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
         int health;
         int max_health;

      protected:


      public:
         Blob();
         virtual ~Blob();

         int get_health() const;
         int get_max_health() const;
         void initialize();
         virtual void update() override;
         void take_damage(int amount=1);
         void set_preferred_direction();
      };
   }
}



