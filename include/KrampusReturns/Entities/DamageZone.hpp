#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>
#include <cstdint>


namespace KrampusReturns
{
   namespace Entities
   {
      class DamageZone : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D
      {
      public:
         static constexpr char* TYPE = (char*)"Entities/DamageZone";
         static constexpr uint32_t DIRECTION_OF_FORCE_UNDEF = 0;
         static constexpr uint32_t DIRECTION_OF_FORCE_UP = 1;
         static constexpr uint32_t DIRECTION_OF_FORCE_DOWN = 2;
         static constexpr uint32_t DIRECTION_OF_FORCE_LEFT = 3;
         static constexpr uint32_t DIRECTION_OF_FORCE_RIGHT = 4;
         static constexpr uint32_t DIRECTION_OF_FORCE_OUT = 5;

      private:
         int damage;
         uint32_t direction_of_force;

      protected:


      public:
         DamageZone(int damage=0, uint32_t direction_of_force=DIRECTION_OF_FORCE_UNDEF);
         ~DamageZone();

         void set_damage(int damage);
         void set_direction_of_force(uint32_t direction_of_force);
         int get_damage() const;
         uint32_t get_direction_of_force() const;
         void TODO();
      };
   }
}



