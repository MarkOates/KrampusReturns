
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


#include <KrampusReturns/Entities/Krampus.hpp>
#include <KrampusReturns/Testing/Gym.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>


class KrampusReturns_CameraControlStrategies2D_SmoothSnapWithFXGym : public KrampusReturns::Testing::Gym
{
public:
   AllegroFlare::FrameAnimation::Book animation_book;
   AllegroFlare::EventEmitter *event_emitter;

   virtual void SetUp() override
   {
      KrampusReturns::Testing::Gym::SetUp();
      animation_book.set_json_source_filename(
         "/Users/markoates/Repos/KrampusReturns/bin/programs/data/bitmaps/krampus-returns-sprites-0x.json"
      );
      animation_book.set_png_source_filename(
         "/Users/markoates/Repos/KrampusReturns/bin/programs/data/bitmaps/krampus-returns-sprites-0x.png"
      );
      animation_book.set_sprite_sheet_scale(2);
      animation_book.initialize();

      event_emitter = &get_framework_ref().get_event_emitter_ref();
   }
};


TEST_F(KrampusReturns_CameraControlStrategies2D_SmoothSnapWithFXGym, can_be_created_without_blowing_up)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::Krampus krampus;
   krampus.set_animation_book(&animation_book);
   krampus.set_event_emitter(event_emitter);
   krampus.set_draw_debug(true);
   krampus.initialize();
   krampus.get_place_ref().position = { 400/2, 240/2 };
   krampus.set(ON_MAP_NAME, "gym");

   get_platforming_2d_ref().add_entity_to_pool(&krampus);
   get_platforming_2d_ref().set_player_controlled_entity(&krampus);

   run_gym();
}


