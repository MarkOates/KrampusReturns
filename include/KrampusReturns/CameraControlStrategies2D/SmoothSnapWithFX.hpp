#pragma once


#include <AllegroFlare/CameraControlStrategies2D/Base.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2D.hpp>


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

      protected:


      public:
         SmoothSnapWithFX(float room_width=1.0f, float room_height=1.0f);
         virtual ~SmoothSnapWithFX();

         void set_entity_to_follow(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity_to_follow);
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* get_entity_to_follow() const;
         virtual void initialize() override;
         virtual void update() override;
      };
   }
}



