
#include <gtest/gtest.h>

#include <KrampusReturns/CameraControlStrategies2D/SmoothSnapWithFX.hpp>


TEST(KrampusReturns_CameraControlStrategies2D_SmoothSnapWithFXTest, can_be_created_without_blowing_up)
{
   KrampusReturns::CameraControlStrategies2D::SmoothSnapWithFX smooth_snap_with_effects;
}


TEST(KrampusReturns_CameraControlStrategies2D_SmoothSnapWithFXTest, TYPE__has_the_expected_value)
{
   KrampusReturns::CameraControlStrategies2D::SmoothSnapWithFX smooth_snap_with_effects;
   std::string expected_type = "KrampusReturns/CameraControlStrategies2D/SmoothSnapWithFX";
   EXPECT_EQ(expected_type, smooth_snap_with_effects.get_type());
}


TEST(KrampusReturns_CameraControlStrategies2D_SmoothSnapWithFXTest, type__has_the_expected_value_matching_TYPE)
{
   KrampusReturns::CameraControlStrategies2D::SmoothSnapWithFX smooth_snap_with_effects;
   EXPECT_EQ(
      KrampusReturns::CameraControlStrategies2D::SmoothSnapWithFX::TYPE,
      smooth_snap_with_effects.get_type()
   );
}

