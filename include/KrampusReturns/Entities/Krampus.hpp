#pragma once


#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>
#include <cstdint>


namespace KrampusReturns
{
   namespace Entities
   {
      class Krampus : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D
      {
      public:
         static constexpr uint32_t STATE_UNDEF = 0;
         static constexpr uint32_t STATE_STANDING = 1;
         static constexpr uint32_t STATE_ATTACKING = 2;
         static constexpr uint32_t STATE_WALKING = 3;
         static constexpr char* TYPE = (char*)"Entities/Krampus";

      private:
         AllegroFlare::EventEmitter* event_emitter;
         uint32_t state;
         float state_changed_at;
         bool state_is_busy;
         bool initialized;
         bool set_state(uint32_t state=STATE_UNDEF, float time_now=al_get_time());

      protected:


      public:
         Krampus(AllegroFlare::EventEmitter* event_emitter=nullptr);
         virtual ~Krampus();

         void set_event_emitter(AllegroFlare::EventEmitter* event_emitter);
         virtual void update() override;
         void emit_camera_shake_event();
         void stand_still();
         void walk_right();
         void walk_up();
         void walk_down();
         void walk_left();
         void face_left();
         void face_right();
         void attack();
         void initialize();
         float infer_state_age(float time_now=al_get_time());
      };
   }
}



