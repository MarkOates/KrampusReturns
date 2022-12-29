

#include <KrampusReturns/AssetFactory.hpp>

#include <AllegroFlare/AudioRepositoryElement.hpp>


namespace KrampusReturns
{


AssetFactory::AssetFactory()
{
}


AssetFactory::~AssetFactory()
{
}


std::map<std::string, AllegroFlare::AudioRepositoryElement> AssetFactory::obtain_production_sound_effect_elements()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> result = {
      { "smash_club", { "smash-club-01.ogg", false, "restart", 0.9 } },
      { "krampus_hit", { "krampus_hit.ogg", false, "restart" } },
      { "menu_move", { "menu-move-01.ogg", false, "restart", 0.5 } },
      { "menu_select", { "menu-select-big-01.ogg", false, "restart", 0.95 } },
      { "fail_moan_sound", { "fail_moan.ogg", false, "restart", 0.4 } },
   };
   return result;
}

std::map<std::string, AllegroFlare::AudioRepositoryElement> AssetFactory::obtain_production_music_track_elements()
{
   std::map<std::string, AllegroFlare::AudioRepositoryElement> result = {
     { "intro_music", { "krampus-intro-1.wav", false } },
     { "level_1_music", { "8bit-dungeon-level-kevin-macleod-80kbps.ogg", true, "restart", 0.5 } },
     { "boss_music", { "krampus-boss-01.ogg", true, "restart", 0.78 } },
     { "dead_krampus_music", { "dead-krampus-01.ogg", false, "restart", 0.78 } },
     { "win_music", { "win-music-01.ogg", false, "restart" } },
   };
   return result;
}


} // namespace KrampusReturns


