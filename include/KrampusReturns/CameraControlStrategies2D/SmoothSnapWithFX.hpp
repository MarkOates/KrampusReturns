#pragma once


#include <AllegroFlare/CameraControlStrategies2D/Base.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2D.hpp>
#include <AllegroFlare/Random.hpp>


namespace KrampusReturns
{
   namespace CameraControlStrategies2D
   {
      class SmoothSnapWithFX : public AllegroFlare::CameraControlStrategies2D::Base
      {
      public:
         static constexpr char* TYPE = (char*)"KrampusReturns/CameraControlStrategies2D/SmoothSnapWithFX";

      private:
         float room_width;
         float room_height;
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity_to_follow;
         float tracking_target_position_x;
         float tracking_target_position_y;
         float impact_shake_started_at;
         AllegroFlare::Random random;

      protected:


      public:
         SmoothSnapWithFX(float room_width=1.0f, float room_height=1.0f);
         virtual ~SmoothSnapWithFX();

         void set_entity_to_follow(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity_to_follow);
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* get_entity_to_follow() const;
         virtual void initialize() override;
         void start_impact_shake(float time_now=al_get_time());
         virtual void update() override;
         float calc_impact_shake_age(float time_now=al_get_time());
         float normalized_impact_shake_age(float duration=1.0f, float time_now=al_get_time());
      };
   }
}



