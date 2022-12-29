#pragma once


#include <AllegroFlare/GameEventDatas/Base.hpp>
#include <string>


namespace KrampusReturns
{
   namespace GameEventDatas
   {
      class SpawnDamageZoneByPlayer : public AllegroFlare::GameEventDatas::Base
      {
      public:
         static constexpr char* TYPE = (char*)"GameEventDatas/SpawnDamageZoneByPlayer";

      private:
         std::string property;

      protected:


      public:
         SpawnDamageZoneByPlayer(std::string property="[unset-property]");
         ~SpawnDamageZoneByPlayer();

         std::string get_property() const;
      };
   }
}



