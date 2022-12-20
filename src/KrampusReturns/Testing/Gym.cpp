

#include <KrampusReturns/Testing/Gym.hpp>

#include <AllegroFlare/TerminalColors.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>


namespace KrampusReturns
{
namespace Testing
{


Gym::Gym(KrampusReturns::Gameplay::Screen platforming_2d)
   : ::testing::Test()
   , framework({})
   , platforming_2d(platforming_2d)
{
}


Gym::~Gym()
{
}


AllegroFlare::Frameworks::Full &Gym::get_framework_ref()
{
   return framework;
}


KrampusReturns::Gameplay::Screen &Gym::get_platforming_2d_ref()
{
   return platforming_2d;
}


void Gym::SetUp()
{
   ASSERT_EQ(false, al_is_system_installed());
   ASSERT_EQ(true, al_init());
   ASSERT_EQ(true, al_init_primitives_addon());
   ASSERT_EQ(true, al_init_font_addon());
   ASSERT_EQ(true, al_init_ttf_addon());
   ASSERT_EQ(true, al_init_image_addon());

   #if defined(_WIN32) || defined(_WIN64)
   #define TEST_BASE_FOLDER "/msys64/home/Mark/Repos/KrampusReturns/bin/programs/data/"
   #define TEST_FIXTURE_FONT_FOLDER "/msys64/home/Mark/Repos/allegro_flare/bin/data/fonts/"
   #define TEST_FIXTURE_BITMAP_FOLDER "/msys64/home/Mark/Repos/allegro_flare/bin/data/bitmaps/"
   #define TEST_FIXTURE_TEST_RUN_SNAPSHOTS_FOLDER "/msys64/home/Mark/Repos/allegro_flare/tmp/test_snapshots/"
   #else
   #define TEST_BASE_FOLDER "/Users/markoates/Repos/KrampusReturns/bin/programs/data/"
   #define TEST_FIXTURE_FONT_FOLDER "/Users/markoates/Repos/allegro_flare/bin/data/fonts/"
   #define TEST_FIXTURE_BITMAP_FOLDER "/Users/markoates/Repos/allegro_flare/bin/data/bitmaps/"
   #define TEST_FIXTURE_TEST_RUN_SNAPSHOTS_FOLDER "/Users/markoates/Repos/allegro_flare/tmp/test_snapshots/"
   #endif

   //AllegroFlare::Frameworks::Full framework;
   framework.disable_fullscreen();
   framework.disable_auto_created_config_warning();
   framework.initialize();

   framework.get_bitmap_bin_ref().set_full_path(TEST_FIXTURE_BITMAP_FOLDER);
   framework.get_font_bin_ref().set_full_path(TEST_FIXTURE_FONT_FOLDER);

   //KrampusReturns::Gameplay::Screen platforming_2d;
   platforming_2d.set_bitmap_bin(&framework.get_bitmap_bin_ref());
   platforming_2d.set_display(framework.get_primary_display());
   platforming_2d.set_event_emitter(&framework.get_event_emitter_ref());
   platforming_2d.set_map_dictionary({
      { "map_a", TEST_BASE_FOLDER "maps/krampus-returns-map01-0x.tmj" },
      //{ "map_b", TEST_BASE_FOLDER "maps/krampus-returns-map02-0x.tmj" },
   });
   platforming_2d.initialize_maps();
   platforming_2d.set_currently_active_map("map_a");
   //platforming_2d.initialize();

   framework.register_screen("platforming_2d", &platforming_2d);


   return;
}

void Gym::run_gym()
{
   platforming_2d.initialize();
   framework.activate_screen("platforming_2d");
   framework.run_loop();
   return;
}

void Gym::TearDown()
{
   if (test_name_indicates_it_wants_a_screenshot())
   {
      capture_screenshot(build_full_test_name_str() + ".png");
   }

   //font_bin.clear();
   //bitmap_bin.clear();
   //al_destroy_display(display);
   //al_shutdown_ttf_addon(); // this is required otherwise subsequent al_init_ttf_addon will not work
                             //this is a bug in Allegro
   //al_uninstall_system();
   return;
}

ALLEGRO_FONT* Gym::get_any_font()
{
   return framework.get_font_bin_ref().auto_get("consolas.ttf 32");
}

ALLEGRO_BITMAP* Gym::get_display_backbuffer()
{
   throw std::runtime_error("Testing/Gym::get_display_backbuffer() not implemented");
   //return al_get_backbuffer(framework.get_current_display()->get_al_display());
}

void Gym::sleep_for_frame()
{
   static int frame_length_in_milliseconds = 1000/60;
   std::this_thread::sleep_for(std::chrono::milliseconds(frame_length_in_milliseconds));
}

void Gym::sleep_for(float length_in_seconds)
{
   int length_in_milliseconds = (int)(length_in_seconds * 1000.0);
   std::this_thread::sleep_for(std::chrono::milliseconds(length_in_milliseconds));
}

std::string Gym::get_test_name()
{
   // TODO: use AllegroFlare::Testing::TestNameInference for this logic
   const testing::TestInfo* const test_info = testing::UnitTest::GetInstance()->current_test_info();
   return test_info->name();
}

std::string Gym::get_test_suite_name()
{
   // TODO: use AllegroFlare::Testing::TestNameInference for this logic
   const testing::TestInfo* const test_info = testing::UnitTest::GetInstance()->current_test_info();
   return test_info->test_suite_name();
}

std::string Gym::build_full_test_name_str()
{
   // TODO: use AllegroFlare::Testing::TestNameInference for this logic
   return get_test_suite_name() + " - " + get_test_name();
}

AllegroFlare::Placement2D Gym::build_centered_placement(float width, float height)
{
   if (!(al_get_target_bitmap()))
   {
      std::stringstream error_message;
      error_message << "[Gym::build_centered_placement]: error: guard \"al_get_target_bitmap()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Gym::build_centered_placement: error: guard \"al_get_target_bitmap()\" not met");
   }
   AllegroFlare::Placement2D place;
   //AllegroFlare::Placement2D place(al_get_display_width(display)/2, al_get_display_height(display)/2, width, height);
   return place;
}

void Gym::draw_rulers()
{
   if (!(al_get_target_bitmap()))
   {
      std::stringstream error_message;
      error_message << "[Gym::draw_rulers]: error: guard \"al_get_target_bitmap()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Gym::draw_rulers: error: guard \"al_get_target_bitmap()\" not met");
   }
   al_draw_line(1920/2, 0, 1920/2, 1080, al_color_name("gray"), 1.0); // rulers down the center
   al_draw_line(0, 1080/2, 1920, 1080/2, al_color_name("gray"), 1.0); // rulers across the middle
}

void Gym::draw_crosshair(float x, float y, ALLEGRO_COLOR color, float size)
{
   if (!(al_get_target_bitmap()))
   {
      std::stringstream error_message;
      error_message << "[Gym::draw_crosshair]: error: guard \"al_get_target_bitmap()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Gym::draw_crosshair: error: guard \"al_get_target_bitmap()\" not met");
   }
   float h_size = size * 0.5;
   // draw horizontal line
   al_draw_line(x-h_size, y, x+h_size, y, color, 1.0);
   // draw vertical line
   al_draw_line(x, y-h_size, x, y+h_size, color, 1.0);
}

void Gym::draw_horizontal_crosshair(float x, float y, ALLEGRO_COLOR color, float size_v, float size_h)
{
   if (!(al_get_target_bitmap()))
   {
      std::stringstream error_message;
      error_message << "[Gym::draw_horizontal_crosshair]: error: guard \"al_get_target_bitmap()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Gym::draw_horizontal_crosshair: error: guard \"al_get_target_bitmap()\" not met");
   }
   float h_size_h = size_h * 0.5;
   float h_size_v = size_v * 0.5;
   // draw horizontal line
   al_draw_line(x-h_size_h, y, x+h_size_h, y, color, 1.0);
   // draw vertical line
   al_draw_line(x, y-h_size_v, x, y+h_size_v, color, 1.0);
}

bool Gym::test_name_indicates_it_wants_a_screenshot()
{
   return (get_test_name().substr(0, 9) == "CAPTURE__");
}

void Gym::clear_display()
{
   // depreciated, please use clear();
   clear();
   return;
}

void Gym::clear()
{
   al_clear_depth_buffer(1);
   ALLEGRO_COLOR eigengrau = ALLEGRO_COLOR{0.086f, 0.086f, 0.114f, 1.0f};
   al_clear_to_color(eigengrau);
   return;
}

void Gym::capture_screenshot(std::string base_filename)
{
   throw std::runtime_error("Testing/Gym::capture_screenshot() not implemented");
   //// TODO: use AllegroFlare::Testing::TestNameInference for this logic
   //std::string full_file_save_location = TEST_FIXTURE_TEST_RUN_SNAPSHOTS_FOLDER + base_filename;

   //al_flip_display(); // this capture_screenshot technique assumes the pixels to capture are currently being
                       //shown on the display.  This al_flip_display is added here in order to flip the
                      // front-buffer *back* to the backbuffer so it can be used to capture the screenshot

   //bool screenshot_successful = al_save_bitmap(full_file_save_location.c_str(), al_get_backbuffer(display));
   //if (screenshot_successful)
   //{
      //std::cout << AllegroFlare::TerminalColors::CYAN
                //<< "[AllegroFlare::Testing::Gym::screenshot]: info: screenshot saved to "
                //<< "\"" << full_file_save_location << "\""
                //<< AllegroFlare::TerminalColors::DEFAULT
                //<< std::endl;
   //}
   //else
   //{
      //std::cout << AllegroFlare::TerminalColors::RED
                //<< "[AllegroFlare::Testing::Gym::screenshot]: error: screenshot "
                //<< "CAPTURE was not successful when trying to saving to \"" << full_file_save_location << "\""
                //<< AllegroFlare::TerminalColors::DEFAULT
                //<< std::endl;
   //}
}


} // namespace Testing
} // namespace KrampusReturns


