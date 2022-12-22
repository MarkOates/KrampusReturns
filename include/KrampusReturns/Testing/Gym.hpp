#pragma once


#include <AllegroFlare/FrameAnimation/Book.hpp>
#include <AllegroFlare/Frameworks/Full.hpp>
#include <AllegroFlare/Placement2D.hpp>
#include <KrampusReturns/EntityFactory.hpp>
#include <KrampusReturns/Gameplay/Screen.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <gtest/gtest.h>
#include <string>


namespace KrampusReturns
{
   namespace Testing
   {
      class Gym : public ::testing::Test
      {
      private:
         AllegroFlare::Frameworks::Full framework;
         KrampusReturns::EntityFactory entity_factory;
         KrampusReturns::Gameplay::Screen platforming_2d;
         AllegroFlare::FrameAnimation::Book animation_book;

      protected:


      public:
         Gym();
         virtual ~Gym();

         AllegroFlare::Frameworks::Full &get_framework_ref();
         KrampusReturns::EntityFactory &get_entity_factory_ref();
         KrampusReturns::Gameplay::Screen &get_platforming_2d_ref();
         virtual void SetUp() override;
         void run_gym();
         virtual void TearDown() override;
         ALLEGRO_FONT* get_any_font();
         ALLEGRO_BITMAP* get_display_backbuffer();
         void sleep_for_frame();
         void sleep_for(float length_in_seconds=0.0f);
         std::string get_test_name();
         std::string get_test_suite_name();
         std::string build_full_test_name_str();
         AllegroFlare::Placement2D build_centered_placement(float width=0.0f, float height=0.0f);
         void draw_rulers();
         void draw_crosshair(float x=0.0f, float y=0.0f, ALLEGRO_COLOR color=ALLEGRO_COLOR{1, 0, 0, 1}, float size=200.0f);
         void draw_horizontal_crosshair(float x=0.0f, float y=0.0f, ALLEGRO_COLOR color=ALLEGRO_COLOR{1, 1, 0, 1}, float size_v=30.0f, float size_h=500.0f);
         bool test_name_indicates_it_wants_a_screenshot();
         void clear_display();
         void clear();
         void capture_screenshot(std::string base_filename="Gym-screenshot.png");
      };
   }
}



