#pragma once


#include <AllegroFlare/AudioRepositoryElement.hpp>
#include <map>
#include <string>


namespace KrampusReturns
{
   class AssetFactory
   {
   private:

   protected:


   public:
      AssetFactory();
      ~AssetFactory();

      static std::map<std::string, AllegroFlare::AudioRepositoryElement> obtain_production_sound_effect_elements();
      static std::map<std::string, AllegroFlare::AudioRepositoryElement> obtain_production_music_track_elements();
   };
}



