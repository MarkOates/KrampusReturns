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
         static constexpr uint32_t STATE_STUNNED_FROM_TAKING_DAMAGE = 4;
         static constexpr char* TYPE = (char*)"Entities/Krampus";

      private:
         AllegroFlare::EventEmitter* event_emitter;
         uint32_t state;
         float state_changed_at;
         bool state_is_busy;
         bool attack_hit_activated;
         int health;
         int max_health;
         float stunned_from_damage_at;
         float invincible_from_taking_damage_at;
         bool invincible_from_taking_damage;
         bool initialized;
         bool set_state(uint32_t state=STATE_UNDEF, float time_now=al_get_time());

      protected:


      public:
         Krampus(AllegroFlare::EventEmitter* event_emitter=nullptr);
         virtual ~Krampus();

         void set_event_emitter(AllegroFlare::EventEmitter* event_emitter);
         int get_health() const;
         int get_max_health() const;
         void initialize();
         virtual void update() override;
         void emit_bump_camera_shake_event();
         void emit_smash_club_sound_effect();
         void emit_take_damage_sound_effect();
         void stand_still();
         void stand_still_x();
         void stand_still_y();
         void walk_right();
         void walk_up();
         void walk_down();
         void walk_left();
         void face_left();
         void face_right();
         void attack();
         void take_hit(int damage=1);
         float infer_state_age(float time_now=al_get_time());
         float infer_age_of(float event_at=0.0f, float time_now=al_get_time());
      };
   }
}



