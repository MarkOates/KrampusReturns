
#include <gtest/gtest.h>

#include <KrampusReturns/GameEventDatas/SpawnDamageZoneByPlayer.hpp>


TEST(KrampusReturns_GameEventDatas_SpawnDamageZoneByPlayerTest, can_be_created_without_blowing_up)
{
   KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer spawn_damage_zone_from_player;
}


TEST(KrampusReturns_GameEventDatas_SpawnDamageZoneByPlayerTest, TYPE__has_the_expected_value)
{
   KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer spawn_damage_zone_from_player;
   EXPECT_EQ("GameEventDatas/SpawnDamageZoneByPlayer", spawn_damage_zone_from_player.get_type());
}


TEST(KrampusReturns_GameEventDatas_SpawnDamageZoneByPlayerTest, type__has_the_expected_value_matching_TYPE)
{
   KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer spawn_damage_zone_from_player;
   EXPECT_EQ(KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer::TYPE, spawn_damage_zone_from_player.get_type());
}


