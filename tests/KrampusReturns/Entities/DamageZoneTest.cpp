
#include <gtest/gtest.h>

#include <KrampusReturns/Entities/DamageZone.hpp>


TEST(KrampusReturns_Entities_DamageZoneTest, can_be_created_without_blowing_up)
{
   KrampusReturns::Entities::DamageZone damage_zone;
}


TEST(KrampusReturns_Entities_DamageZoneTest, TYPE__has_the_expected_value)
{
   // TODO: fix this typing system
   KrampusReturns::Entities::DamageZone damage_zone;
   //EXPECT_EQ("Entities/DamageZone", damage_zone.get_type());
}


TEST(KrampusReturns_Entities_DamageZoneTest, type__has_the_expected_value_matching_TYPE)
{
   // TODO: fix this typing system
   KrampusReturns::Entities::DamageZone damage_zone;
   //EXPECT_EQ(KrampusReturns::Entities::DamageZone::TYPE, damage_zone.get_type());
}


