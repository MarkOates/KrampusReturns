
#include <gtest/gtest.h>

#include <KrampusReturns/Gameplay/Screen.hpp>

#include <AllegroFlare/Frameworks/Full.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2DFactory.hpp>
#include <KrampusReturns/EntityFactory.hpp>


// TODO: improve this:
#if defined(_WIN32) || defined(_WIN64)
#define TEST_BASE_FOLDER "/msys64/home/Mark/Repos/KrampusReturns/bin/programs/data/"
#else
#define TEST_BASE_FOLDER "/Users/markoates/Repos/KrampusReturns/bin/programs/data/"
#endif



TEST(KrampusReturns_Gameplay_ScreenTest, can_be_created_without_blowing_up)
{
   KrampusReturns::Gameplay::Screen platforming2d;
}


TEST(KrampusReturns_Gameplay_ScreenTest,
   INTERACTIVE__in_an_AllegroFlare_Frameworks_Full_context__will_run_as_expected)
   //DISABLED__INTERACTIVE__in_an_AllegroFlare_Frameworks_Full_context__will_run_as_expected)
{
   AllegroFlare::Frameworks::Full framework;
   framework.disable_fullscreen();
   framework.initialize();

   framework.get_bitmap_bin_ref().set_full_path(TEST_BASE_FOLDER "bitmaps/");
   framework.get_font_bin_ref().set_full_path(TEST_BASE_FOLDER "fonts/");
   framework.get_sample_bin_ref().set_full_path(TEST_BASE_FOLDER "samples/");


      framework.get_audio_controller_ref().set_and_load_sound_effect_elements({
         { "smash_club", { "smash-club-01.ogg", false, "restart" } },
         { "krampus_hit", { "krampus_hit.ogg", false, "restart" } },
      });

      framework.get_audio_controller_ref().set_and_load_music_track_elements({
        { "intro_music", { "krampus-intro-1.wav", false } },
        { "win_music", { "win-music-01.ogg", false, "restart" } },
      });



   // create an animation book (to create an frame_animated type from the factory)
   // TODO: introduce this concept to the test
   AllegroFlare::FrameAnimation::Book animation_book(
      "/Users/markoates/Repos/KrampusReturns/bin/programs/data/bitmaps/krampus-returns-sprites-0x.png",
      "/Users/markoates/Repos/KrampusReturns/bin/programs/data/bitmaps/krampus-returns-sprites-0x.json",
      2
   );
   animation_book.initialize();






   KrampusReturns::Gameplay::Screen platforming_2d;
   platforming_2d.set_font_bin(&framework.get_font_bin_ref());
   platforming_2d.set_bitmap_bin(&framework.get_bitmap_bin_ref());
   //platforming_2d.set_animation_book(&animation_book);
   platforming_2d.set_display(framework.get_primary_display());
   platforming_2d.set_event_emitter(&framework.get_event_emitter_ref());
   platforming_2d.set_map_dictionary({
      { "map_a", TEST_BASE_FOLDER "maps/krampus-returns-map01-0x.tmj" },
      { "map_b", TEST_BASE_FOLDER "maps/krampus-returns-map02-0x.tmj" },
   });
   //platforming_2d.set_currently_active_map("map_a");
   platforming_2d.initialize_maps();
   //platforming_2d.initialize();

   platforming_2d.set_currently_active_map("map_a");
   //platforming_2d.initialize();

   // create some entities





   KrampusReturns::EntityFactory entity_factory;
   entity_factory.set_event_emitter(&framework.get_event_emitter_ref());
   entity_factory.set_bitmap_bin(&framework.get_bitmap_bin_ref());
   //entity_factory.set_animation_book(&platforming_2d.get_animation_book_ref());
   entity_factory.set_animation_book(&animation_book);


      //AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory factory(
         //&framework.get_bitmap_bin_ref(),
         //&animation_book
      //);
      //factory.set_init_entities_drawing_debug(true);

      //AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* created_entity =
         //factory.create_for_aabb2d("map_a", 16-1, 32-1);
      //created_entity->get_place_ref().position.x = 400/2;
      //created_entity->get_place_ref().position.y = 240/2;
      //platforming_2d.add_entity_to_pool(created_entity);


      //AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* created_animated_entity =
         //factory.create_frame_animated("map_a", 400/2, 240/2, 12, 8, "blob", "bottom_centered_edge");
      //created_animated_entity->get_place_ref().scale = { 1.5f, 1.5f };
      //platforming_2d.add_entity_to_pool(created_animated_entity);



      KrampusReturns::Entities::Krampus *krampus = entity_factory.create_krampus("map_a", 400/2 - 50, 240/2);
      platforming_2d.add_entity_to_pool(krampus);

      platforming_2d.set_player_controlled_entity(krampus);
      //platforming_2d.set_player_controlled_entity(created_entity);


      KrampusReturns::Entities::Blob *blob = entity_factory.create_blob("map_a", 400/2+50, 240/2);
      platforming_2d.add_entity_to_pool(blob);

      KrampusReturns::Entities::Goalpost *goalpost = entity_factory.create_goalpost(
         "map_a", 1, 400/2+50 + 100, 240/2
      );
      platforming_2d.add_entity_to_pool(goalpost);


      for (unsigned i=0; i<6; i++)
      {
         //AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* enemy = factory.create_flapping_enemy(
            //"map_a",
            //400/8*6,
            //240/3*1,
            //16.0-1.0f,
            //16.0-1.0f,
            //80.0f, // target elevation
            //4.0f, // flap strength
            //0.1f // recovery rate
         //);
         //platforming_2d.add_entity_to_pool(enemy);
      }

      AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* collectable2 =
         entity_factory.create_collectable("map_a", 5*16, 20*16);
      platforming_2d.add_entity_to_pool(collectable2);

      platforming_2d.add_entity_to_pool(entity_factory.create_collectable("map_a", 45*16, 2*16));

      // create the door
      platforming_2d.add_entity_to_pool(
         entity_factory.create_door("map_b", 4.5*16, 7*16, "map_a", 46*16, 26*16)
      );
      platforming_2d.add_entity_to_pool(
         entity_factory.create_door("map_a", 46*16, 26*16, "map_b", 4.5*16, 7*16)
      );


   platforming_2d.initialize();
   //platforming_2d.set_currently_active_map("map_a");
   platforming_2d.start_level();


   framework.register_screen("platforming_2d", &platforming_2d);
   framework.activate_screen("platforming_2d");

   framework.run_loop();
}

