
#include <gtest/gtest.h>

#include <KrampusReturns/Entities/Krampus.hpp>
#include <KrampusReturns/Testing/Gym.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>


class KrampusReturns_Entities_KrampusGym : public KrampusReturns::Testing::Gym
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


TEST_F(KrampusReturns_Entities_KrampusGym, can_be_created_without_blowing_up)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   AllegroFlare::AudioController &audio_controller = get_framework_ref().get_audio_controller_ref();
   audio_controller.set_and_load_sound_effect_elements({
     //{ "menu-click-01.wav", { "menu-click-01.wav", false } },
     //{ "door-locked-hall", { "door-locked-hall-03.ogg", false } },
     //{ "doorbell", { "doorbell-02.ogg", false } },
      //{ "intro_music", { "krampus-intro-1.ogg", false } },
      //{ "smash_club", { "smash-club-01.ogg", false } },
      { "smash_club", { "smash-club-01.ogg", false, "restart" } },
   });


   KrampusReturns::Entities::Krampus krampus;
   krampus.set_animation_book(&animation_book);
   krampus.set_event_emitter(event_emitter);
   krampus.initialize();
   //krampus.set_draw_debug(true);
   krampus.get_place_ref().position = { 400/2, 240/2 };
   krampus.set(ON_MAP_NAME, "gym");


   //KrampusReturns::Entities::Blob *blob = entity_factory.create_blob("map_a", 400/2+50, 240/2);
   //platforming_2d.add_entity_to_pool(blob);


   get_platforming_2d_ref().add_entity_to_pool(&krampus);
   get_platforming_2d_ref().set_player_controlled_entity(&krampus);

   run_gym();
}


