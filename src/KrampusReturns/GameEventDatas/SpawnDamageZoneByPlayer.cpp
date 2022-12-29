

#include <KrampusReturns/GameEventDatas/SpawnDamageZoneByPlayer.hpp>




namespace KrampusReturns
{
namespace GameEventDatas
{


SpawnDamageZoneByPlayer::SpawnDamageZoneByPlayer(float point_of_impact_x, float point_of_impact_y, float impact_width, float impact_height, int damage, int32_t direction_of_force)
   : AllegroFlare::GameEventDatas::Base(KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer::TYPE)
   , point_of_impact_x(point_of_impact_x)
   , point_of_impact_y(point_of_impact_y)
   , impact_width(impact_width)
   , impact_height(impact_height)
   , damage(damage)
   , direction_of_force(direction_of_force)
   , DUMMY_DEP(nullptr)
{
}


SpawnDamageZoneByPlayer::~SpawnDamageZoneByPlayer()
{
}


float SpawnDamageZoneByPlayer::get_point_of_impact_x() const
{
   return point_of_impact_x;
}


float SpawnDamageZoneByPlayer::get_point_of_impact_y() const
{
   return point_of_impact_y;
}


float SpawnDamageZoneByPlayer::get_impact_width() const
{
   return impact_width;
}


float SpawnDamageZoneByPlayer::get_impact_height() const
{
   return impact_height;
}


int SpawnDamageZoneByPlayer::get_damage() const
{
   return damage;
}


int32_t SpawnDamageZoneByPlayer::get_direction_of_force() const
{
   return direction_of_force;
}


void SpawnDamageZoneByPlayer::NOTE()
{
   // Interesting new pattern.  A `type` from an external class is used to initialize a value, but the type
   // is not picked up by the quintessence extrapolation. So, as a HACK, I'm adding a dummy property
   // DUMMY_DEP so it will list the extrenal class as a dependency (and include the header).
   return;
}


} // namespace GameEventDatas
} // namespace KrampusReturns


