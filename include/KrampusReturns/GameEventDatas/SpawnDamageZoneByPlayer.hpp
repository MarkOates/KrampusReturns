#pragma once


#include <AllegroFlare/GameEventDatas/Base.hpp>
#include <KrampusReturns/Entities/DamageZone.hpp>
#include <cstdint>


namespace KrampusReturns
{
   namespace GameEventDatas
   {
      class SpawnDamageZoneByPlayer : public AllegroFlare::GameEventDatas::Base
      {
      public:
         static constexpr char* TYPE = (char*)"GameEventDatas/SpawnDamageZoneByPlayer";

      private:
         float point_of_impact_x;
         float point_of_impact_y;
         float impact_width;
         float impact_height;
         int damage;
         int32_t direction_of_force;
         KrampusReturns::Entities::DamageZone* DUMMY_DEP;

      protected:


      public:
         SpawnDamageZoneByPlayer(float point_of_impact_x=0.0f, float point_of_impact_y=0.0f, float impact_width=1.0f, float impact_height=1.0f, int damage=1, int32_t direction_of_force=KrampusReturns::Entities::DamageZone::DIRECTION_OF_FORCE_UNDEF);
         ~SpawnDamageZoneByPlayer();

         float get_point_of_impact_x() const;
         float get_point_of_impact_y() const;
         float get_impact_width() const;
         float get_impact_height() const;
         int get_damage() const;
         int32_t get_direction_of_force() const;
         void NOTE();
      };
   }
}



