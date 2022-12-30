#pragma once


#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/Frameworks/Full.hpp>
#include <AllegroFlare/GameEvent.hpp>
#include <AllegroFlare/Screens/Achievements.hpp>
#include <AllegroFlare/Screens/Base.hpp>
#include <AllegroFlare/Screens/PauseScreen.hpp>
#include <AllegroFlare/Screens/RollingCredits.hpp>
#include <AllegroFlare/Screens/Storyboard.hpp>
#include <AllegroFlare/Screens/TitleScreen.hpp>
#include <KrampusReturns/Gameplay/Screen.hpp>
#include <KrampusReturns/World.hpp>
#include <allegro5/allegro.h>
#include <string>
#include <vector>


namespace KrampusReturns
{
   class Runner : public AllegroFlare::Screens::Base
   {
   public:
      static constexpr char* MODE_TEST = "test";
      static constexpr char* MODE_PRODUCTION = "production";
      static constexpr char* MODE_DEVELOPMENT = "development";
      static constexpr char* EVENT_START_NEW_GAME = "event_start_new_game";
      static constexpr char* EVENT_ACTIVATE_PLATFORMING_2D_SCREEN = "activate_platforming_2d_screen";
      static constexpr char* EVENT_ACTIVATE_ACHIEVEMENTS_SCREEN = "activate_achievements_screen";
      static constexpr char* EVENT_EXIT_ACHIEVEMENTS_SCREEN = "exit_achievements_screen";
      static constexpr char* EVENT_ACTIVATE_CREDITS_SCREEN = "activate_credits_screen";
      static constexpr char* EVENT_CREDITS_SCREEN_FINISHED = "credits_screen_finished";
      static constexpr char* EVENT_EXIT_GAME = "exit_game";
      static constexpr char* EVENT_PAUSE_GAME = "pause_game";
      static constexpr char* EVENT_UNPAUSE_GAME = "unpause_game";
      static constexpr char* THIS_CLASS = (char*)"KrampusReturns::Runner";

   private:
      std::string mode;
      AllegroFlare::Frameworks::Full* framework;
      AllegroFlare::EventEmitter* event_emitter;
      AllegroFlare::Screens::Storyboard* opening_logos_storyboard_screen;
      AllegroFlare::Screens::TitleScreen title_screen;
      AllegroFlare::Screens::PauseScreen pause_screen;
      AllegroFlare::Screens::Storyboard* new_game_intro_storyboard_screen;
      AllegroFlare::Screens::Storyboard* game_won_outro_storyboard_screen;
      KrampusReturns::Gameplay::Screen platforming_2d_screen;
      KrampusReturns::World platforming_2d_world;
      int finished_up_to_level;
      AllegroFlare::Screens::Achievements achievements_screen;
      AllegroFlare::Screens::Storyboard* credits_screen;
      AllegroFlare::Screens::RollingCredits rolling_credits_screen;
      ALLEGRO_BITMAP* blurry_background_screen_capture;
      std::vector<std::string> prior_screens_stack;
      bool initialized;

   protected:


   public:
      Runner(std::string mode=MODE_PRODUCTION, AllegroFlare::Frameworks::Full* framework=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr);
      virtual ~Runner();

      void initialize();
      void refresh_blurry_background_screen_capture();
      void load_development_data();
      void load_test_data();
      void build_item_dictionary();
      void initialize_world();
      void setup_platforming_2d_screen();
      void unlock_achievement(std::string achievement_name="[unset-achievement_name]");
      void push_screen();
      std::string pop_screen();
      void reactivate_prior_screen(std::string fallback_if_stack_is_empty="tile_drive_screen");
      bool prior_screen_is_in_stack();
      virtual void game_event_func(AllegroFlare::GameEvent* ev=nullptr) override;
      static void run(std::string mode="production");
      static bool is_test_mode(std::string mode="[unset-mode]");
      static bool is_development_mode(std::string mode="[unset-mode]");
      static bool is_production_mode(std::string mode="[unset-mode]");
      bool in_development_mode();
      bool in_production_mode();
      bool in_test_mode();
   };
}



