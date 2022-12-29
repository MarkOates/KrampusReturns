

#include <KrampusReturns/GameEventDatas/SpawnDamageZoneByPlayer.hpp>




namespace KrampusReturns
{
namespace GameEventDatas
{


SpawnDamageZoneByPlayer::SpawnDamageZoneByPlayer(std::string property)
   : AllegroFlare::GameEventDatas::Base(KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer::TYPE)
   , property(property)
{
}


SpawnDamageZoneByPlayer::~SpawnDamageZoneByPlayer()
{
}


std::string SpawnDamageZoneByPlayer::get_property() const
{
   return property;
}




} // namespace GameEventDatas
} // namespace KrampusReturns


