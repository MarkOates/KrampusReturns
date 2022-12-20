
#include <gtest/gtest.h>

#include <KrampusReturns/Entities/Blob.hpp>
#include <KrampusReturns/Testing/Gym.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>


class KrampusReturns_Entities_BlobGym : public KrampusReturns::Testing::Gym
{
public:
   AllegroFlare::FrameAnimation::Book animation_book;

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
   }
};


TEST_F(KrampusReturns_Entities_BlobGym, can_be_created_without_blowing_up)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   KrampusReturns::Entities::Blob blob;
   blob.set_draw_debug(true);
   blob.set_animation_book(&animation_book);
   blob.initialize();
   blob.get_place_ref().position = { 400/2, 240/2 };
   blob.set(ON_MAP_NAME, "gym");

   get_platforming_2d_ref().add_entity_to_pool(&blob);
   //get_platforming_2d_ref().set_player_controlled_entity(&blob);

   run_gym();
}


