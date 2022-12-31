

#include <KrampusReturns/Runner.hpp>

#include <AllegroFlare/Color.hpp>
#include <AllegroFlare/Elements/RollingCredits/SectionFactory.hpp>
#include <AllegroFlare/Errors.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/Prototypes/FixedRoom2D/EventNames.hpp>
#include <AllegroFlare/Prototypes/FixedRoom2D/ScriptEventDatas/CollectEvidence.hpp>
#include <AllegroFlare/Prototypes/FixedRoom2D/ScriptEventDatas/CollectItem.hpp>
#include <AllegroFlare/Screens/RollingCredits.hpp>
#include <AllegroFlare/StoryboardFactory.hpp>
#include <KrampusReturns/AssetFactory.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{


Runner::Runner(std::string mode, AllegroFlare::Frameworks::Full* framework, AllegroFlare::EventEmitter* event_emitter)
   : AllegroFlare::Screens::Base("Runner")
   , mode(mode)
   , framework(framework)
   , event_emitter(event_emitter)
   , opening_logos_storyboard_screen(nullptr)
   , title_screen()
   , pause_screen()
   , new_game_intro_storyboard_screen(nullptr)
   , game_won_outro_storyboard_screen(nullptr)
   , platforming_2d_screen()
   , platforming_2d_world()
   , finished_up_to_level(0)
   , achievements_screen()
   , credits_screen(nullptr)
   , rolling_credits_screen()
   , blurry_background_screen_capture(nullptr)
   , prior_screens_stack()
   , initialized(false)
{
}


Runner::~Runner()
{
}


void Runner::initialize()
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Runner::initialize]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::initialize: error: guard \"(!initialized)\" not met");
   }
   if (!(framework))
   {
      std::stringstream error_message;
      error_message << "[Runner::initialize]: error: guard \"framework\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::initialize: error: guard \"framework\" not met");
   }
   // setup the bin paths
   AllegroFlare::FontBin &font_bin = framework->get_font_bin_ref();
   AllegroFlare::BitmapBin &bitmap_bin = framework->get_bitmap_bin_ref();
   AllegroFlare::SampleBin &sample_bin = framework->get_sample_bin_ref();
   AllegroFlare::ModelBin &model_bin = framework->get_model_bin_ref();
   AllegroFlare::EventEmitter &event_emitter = framework->get_event_emitter_ref();
   AllegroFlare::AudioController &audio_controller = framework->get_audio_controller_ref();
   AllegroFlare::Achievements &achievements = framework->get_achievements_ref();


   // setup the achievements
   achievements.set_achievements({
      // TODO: add this unblobed achievement
      //{ "clear_all_blobs_level_1", {
         //new AllegroFlare::Achievement("Unblobbed", "Clear out all the blobs in Level 1" ), false, false }
      //},
      //{ "free_seat", { new AllegroFlare::Achievement("Free Seat at the Concert"), false, false } },
      //{ "start_the_game", { new AllegroFlare::Achievement("In Da House"), false, false } },
      //{ "do_thing", { new AllegroFlare::Achievement("Save the Zoo", "Will unhide when achieved"), false, false } },
      //{ "a_hidden_gem2", { new AllegroFlare::Achievement("Hidden Gem", "Will unhide when achieved"), false, true } },
      { "stay_through_the_credits", { new AllegroFlare::Achievement(
         "Stay Through the Credits",
         "Watch the credits"), false, false } },
   });


   // setup our helper factories
   AllegroFlare::StoryboardFactory storyboard_factory(&font_bin, &event_emitter);


   // create the opening logos storyboard screen
   opening_logos_storyboard_screen = storyboard_factory.create_images_storyboard_screen({
         bitmap_bin["allegro5-logo-gray-med.png"],
         bitmap_bin["clubcatt-med-01.png"],
         nullptr,
      });
   opening_logos_storyboard_screen->set_game_event_name_to_emit_after_completing("start_title_screen");
   framework->register_screen("opening_logos_storyboard_screen", opening_logos_storyboard_screen);


   // pre-load the audio controller (TODO)
   KrampusReturns::AssetFactory asset_factory;

   audio_controller.set_and_load_sound_effect_elements({
      asset_factory.obtain_production_sound_effect_elements()
   });

   audio_controller.set_and_load_music_track_elements({
      asset_factory.obtain_production_music_track_elements()
   });




   // setup the title screen
   title_screen.set_font_bin(&font_bin);
   title_screen.set_bitmap_bin(&bitmap_bin);
   title_screen.set_title_text("KRAMPUS RETURNS");
   title_screen.set_title_text_color(AllegroFlare::Color(AllegroFlare::Color::AliceBlue, 1.0).to_al());
   title_screen.set_title_font_size(-130);
   title_screen.set_title_font_name("ChronoType.otf");
   //title_screen.set_title_text_color(AllegroFlare::Color(0x000000, 1.0).to_al());
   //title_screen.set_background_bitmap_name("placeholder-title-background.jpg");
   title_screen.set_event_emitter(&event_emitter);
   title_screen.set_menu_font_size(-40);
   title_screen.set_menu_position_y(1080/32*19+10);
   title_screen.set_copyright_text("\nCopyright 2022     Mark Oates     CLUBCATT Games     www.clubcatt.com\n");
   title_screen.set_copyright_text_color(AllegroFlare::Color(0xefefff, 0.2).to_al());
   title_screen.set_menu_option_selection_activation_delay(2.0);
   //title_screen.set_copyright_font_name(");
   title_screen.set_copyright_font_size(-26);
   title_screen.set_menu_options({
      //{ "Investigate", "start_new_game" },
      { "Start New Game", EVENT_START_NEW_GAME },
                        //EVENT_ACTIVATE_PLATFORMING_2D_SCREEN }, //EVENT_ACTIVATE_NAME_YOUR_CHARACTER_SCREEN },
      //{ "Chronicle",      EVENT_OPEN_CHRONICLE_SCREEN },
      //{ "Driving",        EVENT_ACTIVATE_TILE_DRIVE_SCREEN },
      //{ "Platforming",    EVENT_ACTIVATE_PLATFORMING_2D_SCREEN },
      //{ "Investigate",    EVENT_ACTIVATE_INVESTIGATION_ROOM_SCREEN },
      { "Achievements",   EVENT_ACTIVATE_ACHIEVEMENTS_SCREEN },
      //{ "Storyboard",     EVENT_ACTIVATE_CUT_IN_STORYBOARD_SCREEN},
      { "Credits",        EVENT_ACTIVATE_CREDITS_SCREEN },
      { "Quit",           EVENT_EXIT_GAME },
   });
   framework->register_screen("title_screen", &title_screen);



   //// setup the achievements screen
   achievements_screen.set_font_bin(&font_bin);
   achievements_screen.set_event_emitter(&event_emitter);
   achievements_screen.set_achievements(&achievements);
   achievements_screen.set_game_event_name_to_emit_on_exit(EVENT_EXIT_ACHIEVEMENTS_SCREEN);
   achievements_screen.initialize();
   framework->register_screen("achievements_screen", &achievements_screen);




   // setup the pause screen
   pause_screen.set_font_bin(&font_bin);
   pause_screen.set_bitmap_bin(&bitmap_bin);
   pause_screen.set_event_emitter(&event_emitter);
   pause_screen.set_menu_options({
      { "Resume", "unpause_game" },
      { "Quit", "start_title_screen" },
   });
   framework->register_screen("pause_screen", &pause_screen);






   // setup the intro storyboards screen
   new_game_intro_storyboard_screen = storyboard_factory.create_advancing_text_storyboard_screen({
       "December 2022.",
       "Aliens have come to Earth and they want to take away Santa's presents!",
       "On Christmas Eve, after all of Santa's presents were wrapped, the aliens came.",
       "Santa and his helpers tried to stop the aliens...",
       "...but they couldn't.",
       "There was no one left to help, except for Krampus.",
       "Krampus is Santa's enemy, but he was Santa's only hope.",
       "Santa was scared, but he knew he had to be brave and ask Krampus for help.",
       "Krampus accepted the challenge to fight the aliens!",
       "At that moment, Krampus went out.",
       "He was the last hope...",
       "...to save Christmas.",
      });
   new_game_intro_storyboard_screen->set_event_emitter(&event_emitter);
   new_game_intro_storyboard_screen->set_game_event_name_to_emit_after_completing(
      "finish_new_game_intro_storyboard_screen"
      //"activate_nvestigation_room_screen"
   );
   framework->register_screen("new_game_intro_storyboard_screen", new_game_intro_storyboard_screen);




   // setup the outro storyboards screen
   game_won_outro_storyboard_screen = storyboard_factory.create_advancing_text_storyboard_screen({
       "Krampus faught against the aliens who saught to take away Santa's presents.",
       "Krampus was really brave and strong, and he won the fight!",
       "Krampus saved Christmas and made sure that the magic of the holiday was safe.",
       "Krampus and Santa didn't always get along, but they worked together to defeat the aliens "
          "and make sure that Christmas was special.",
       "In the end, they learned that the most important thing about Christmas is love and being friends "
          "with each other.",
       "That's what makes Christmas special.",
       "The End."
      });
   game_won_outro_storyboard_screen->set_event_emitter(&event_emitter);
   game_won_outro_storyboard_screen->set_game_event_name_to_emit_after_completing(
      "finish_game_won_outro_storyboard_screen"
      //"activate_nvestigation_room_screen"
   );
   framework->register_screen("game_won_outro_storyboard_screen", game_won_outro_storyboard_screen);




   setup_platforming_2d_screen();

   initialize_world();





   using namespace AllegroFlare::Elements::RollingCredits;


   AllegroFlare::Elements::RollingCredits::SectionFactory section_factory;
   std::string SPACER_TEXT = "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
   std::string SHORT_SPACER_TEXT = "\n\n\n\n\n\n";
   rolling_credits_screen.set_font_bin(&font_bin);
   rolling_credits_screen.set_event_emitter(&event_emitter);
   rolling_credits_screen.set_sections({
      //section_factory.create_header("Production"),
      //section_factory.create_column_with_labels({
         //{ "Producer", "Robyn Kendall" },
         //{ "Production Design", "Tayyibah Samuels" },
         //{ "Music", "Ryker Odling" },
         //{ "Sound Design", "Aiza Rivera" },
         //{ "Lead Programming", "Annaliese Bauer" },
         //{ "Motion Design", "Ellenor Cote" },
         //{ "Element Design", "Katy Swanson" },
         //{ "Marketing Manager", "Melina Kelly" },
         //{ "Set Design", "Abby Burton" },
      //}),
      //section_factory.create_header("Programming"),
      //section_factory.create_column_with_labels({
         //{ "Programmer", "Ducky Donaldson" },
         //{ "Programming", "Carla Cow" },
         //{ "Motion Programmer", "Colt Cat" },
         //{ "Scene Programmers", "John Calf" },
      //}),
      //section_factory.create_text("This film is a work of fiction. Any resemblance to actual persons, "
         //"living or dead, or events is purely coincidental."),

      //jingle-bells-calm.ogg

      section_factory.create_text(SHORT_SPACER_TEXT),
      section_factory.create_text(
         "A game by Mark Oates"
      ),

      section_factory.create_text(SPACER_TEXT),
      section_factory.create_text(
         "KrampusHack 2022\nhttps://tins.amarillion.org/krampu22"
      ),

      section_factory.create_text(SHORT_SPACER_TEXT),
      section_factory.create_text(SPACER_TEXT),
      section_factory.create_header("Programming"),
      section_factory.create_column_with_labels({
         { "Made with", "Allegro (5.2.9)" },
         {          "", "https://liballeg.org/" },
         //{          "", "by Allegro Developers" },
         { "Framework", "AllegroFlare (0.8.11-wip)" },
         {          "", "https://github.com/allegroflare/allegro_flare" },
         {          "", "by Mark Oates" },
      }),

      section_factory.create_text(SHORT_SPACER_TEXT),
      //section_factory.create_text(SPACER_TEXT),
      section_factory.create_header("Visual"),
      section_factory.create_column_with_labels({
         { "Character Design and Art", "Mark Oates" },
         { "RPG Dungeon Tileset", "Pita" },
         {                   "", "https://pita.itch.io" },
         { "Font Awesome Icons", "Dave Gandy" },
         {                   "", "https://fontawesome.com/" },
         {                   "", "SIL Open Font License, Version 1.1." },
         //{    "ChronoType Font", "" },
         //{                   "", "https://fontawesome.com/" },
         //{                   "", "SIL Open Font License, Version 1.1." },
         //{         "Inter Font", "Dave Gandy" },
         //{                   "", "https://fontawesome.com/" },
         //{                   "", "SIL Open Font License, Version 1.1." },

         //{ "Character Design and Art", "Mark Oates" },
      }),

      section_factory.create_text(SHORT_SPACER_TEXT),
      //section_factory.create_text(SPACER_TEXT),
      section_factory.create_header("Audio"),
      section_factory.create_column_with_labels({
         { "Sound Effect Design", "Mark Oates" },
         //{ "Fail Moan Sound Effect", "" },
         //{ "Tileset", "Pita" },
         //{        "", "(https://pita.itch.io)" },
      }),

      section_factory.create_text(SHORT_SPACER_TEXT),
      //section_factory.create_text(SPACER_TEXT),
      section_factory.create_header("Music"),
      //section_factory.create_column_with_labels({
         //{ "Music by", "Mark Oates" },
         //{ "Funeral March", "Frederic Chopin" },
         //{ "\"8 Bit Dungeon\"", "Kevin MacLoed" },
         //{ "Tileset", "Pita" },
         //{        "", "(https://pita.itch.io)" },
      //}),
      section_factory.create_text(
         "INTRO MUSIC\n"
         "by Mark Oates"// , RollingCredits::Sections::Text::ALLIGN_LEFT
      ),
      section_factory.create_text(
         "8 BIT DUNGEON\n"
         "by Kevin MacLeod (incompetech.com)\n"
         "Licensed under Creative Commons: By Attribution 3.0\n"
         "http://creativecommons.org/licenses/by/3.0/"
      ),
      section_factory.create_text(
         "FUNERAL MARCH\n"
         "by Frederic Chopin\n"
         "Public Domain\n"
         "Arranged by Mark Oates"// , RollingCredits::Sections::Text::ALLIGN_LEFT
      ),
      section_factory.create_text(
         "FINAL COUNT\n"
         "by Kevin MacLeod (incompetech.com)\n"
         "Licensed under Creative Commons: By Attribution 3.0\n"
         "http://creativecommons.org/licenses/by/3.0/\n"
         "Edited by Mark Oates"
      ),
      section_factory.create_text(
         "JINGLE BELLS (CALM)\n"
         "by Kevin MacLeod (incompetech.com)\n"
         "Licensed under Creative Commons: By Attribution 3.0\n"
         "http://creativecommons.org/licenses/by/3.0/"
      ),
      section_factory.create_text(
         "BOSS MUSIC\n"
         "by Mark Oates"//, "centered"
      ),



      //section_factory.create_text("\n\n\n\n\n\n"),


      section_factory.create_text(SHORT_SPACER_TEXT),
      section_factory.create_text(SPACER_TEXT),


      section_factory.create_text(
         "Thank you to all the members of the Allegro game programming community."
      ),
      section_factory.create_text(SPACER_TEXT),
      section_factory.create_text(
         "Thank you to Elias and SiegeLord for your continued dedication to Allegro."
      ),
      section_factory.create_text(SPACER_TEXT),
      section_factory.create_text(
         "Thank you to amarillion for your work to support the Allegro community, including creating KrampusHack."
      ),
      section_factory.create_text(SPACER_TEXT),
      section_factory.create_text(
         "Thank you to pmprog for your wishlist ideas for KrampusHack."
      ),
      section_factory.create_text(SPACER_TEXT),
      //section_factory.create_text(SPACER_TEXT),
      section_factory.create_text(
         "Special thanks to my girlfriend, Louise, for her patience."
      ),
      section_factory.create_text(SHORT_SPACER_TEXT),
      section_factory.create_text(SPACER_TEXT),
      section_factory.create_text(
         "And thank you for playing."
      ),


      section_factory.create_text(SHORT_SPACER_TEXT),
      section_factory.create_text(SPACER_TEXT),
      section_factory.create_text(
         "Copyright 2022 - CLUBCATT Games\nhttps://www.clubcatt.com"
      ),

      section_factory.create_text(SHORT_SPACER_TEXT),
      //section_factory.create_text(SPACER_TEXT),
   });
   rolling_credits_screen.initialize();
   rolling_credits_screen.set_game_event_name_to_emit_after_completing(EVENT_CREDITS_SCREEN_FINISHED);
   framework->register_screen("rolling_credits_screen", &rolling_credits_screen);
   //rolling_credits_screen.on_activate();





   // setup the credits screen
   credits_screen = storyboard_factory.create_advancing_text_storyboard_screen({
         //"Thank you to Shawn Hargreaves for creating the first version of Allegro in the 90s.",
         "Thank you to all the members of the Allegro game programming community.",
            //"which had a huge impact in my development as a programmer.",
         "Thank you to Elias and SiegeLord for your continued dedication to Allegro.",
         "Thank you to amarillion for your work to support the Allegro community, including creating KrampusHack.",
         //"Thank you to Matthew Leverton for creating allegro.cc",
         "Thank you to pmprog for your wishlist ideas.",
         "And thank you for playing.",
      });
   credits_screen->set_event_emitter(&event_emitter);
   credits_screen->set_game_event_name_to_emit_after_completing(EVENT_CREDITS_SCREEN_FINISHED);
   framework->register_screen("credits_screen", credits_screen);


   if (in_development_mode()) load_development_data();
   if (in_test_mode()) load_test_data();


   initialized = true;


   return;
}

void Runner::refresh_blurry_background_screen_capture()
{
   //if (blurry_background_screen_capture) al_destroy_bitmap(blurry_background_screen_capture);
   //blurry_background_screen_capture =
     //AllegroFlare::ScreenCapture::create_backbuffer_capture_as_scaled_bitmap(1920/20, 1080/20);
   return;
}

void Runner::load_development_data()
{
   return;
}

void Runner::load_test_data()
{
   load_development_data(); // for now, TODO: fix for a proper development setup
   //chronicle_screen.refresh();
   return;
}

void Runner::build_item_dictionary()
{
   // TODO:
   //item_dictionary.push_back("letter", ...);
   return;
}

void Runner::initialize_world()
{
   // TODO: CRITICAL: fix this TEST_BASE_FOLDER path
   // TODO: Load up actual data for the remaining levels of the game
   static std::string TEST_BASE_FOLDER = "/Users/markoates/Repos/KrampusReturns/bin/programs/data/";
   platforming_2d_world.set_levels({
      KrampusReturns::Level(
         "level_1",
         "The First Rescue",
         {
            { "map_a", TEST_BASE_FOLDER + "maps/krampus-returns-level-1-1-0x.tmj" }
         },
         "level_1_music"
      ),
      KrampusReturns::Level(
         "level_2",
         "The Second Rescue",
         {
            { "map_a", TEST_BASE_FOLDER + "maps/krampus-returns-level-2-1-0x.tmj" }
         },
         "level_1_music"
      ),
      KrampusReturns::Level(
         "level_1",
         "The Pits",
         {
            { "map_a", TEST_BASE_FOLDER + "maps/krampus-returns-level-3-1-0x.tmj" }
         },
         "level_1_music"
      ),
      KrampusReturns::Level(
         "level_1",
         "The Final",
         {
            { "map_a", TEST_BASE_FOLDER + "maps/krampus-returns-level-4-1-0x.tmj" }
         },
         "final_level_music"
      ),
   });
}

void Runner::setup_platforming_2d_screen()
{
   //return;
   static std::string TEST_BASE_FOLDER = "/Users/markoates/Repos/KrampusReturns/bin/programs/data/";
   platforming_2d_screen.set_font_bin(&framework->get_font_bin_ref());
   platforming_2d_screen.set_bitmap_bin(&framework->get_bitmap_bin_ref());
   platforming_2d_screen.set_display(framework->get_primary_display());
   platforming_2d_screen.set_event_emitter(&framework->get_event_emitter_ref());
   platforming_2d_screen.set_map_dictionary({
      // TODO: here
      { "map_a", TEST_BASE_FOLDER + "maps/krampus-returns-map01-0x.tmj" },
      { "map_b", TEST_BASE_FOLDER + "maps/krampus-returns-map02-0x.tmj" },
      //{ "gym", "/Users/markoates/Repos/allegro_flare/bin/data/maps/map1-0x.tmj" },
      //{ "map_b", "/Users/markoates/Repos/allegro_flare/bin/data/maps/map1b-0x.tmj" },
   });
   platforming_2d_screen.initialize_maps();


   platforming_2d_screen.set_currently_active_map("map_a");



   //platforming_2d_screen.set_currently_active_map("map_b");

   platforming_2d_screen.initialize();


   framework->register_screen("platforming_2d_screen", &platforming_2d_screen);

   return;
}

void Runner::unlock_achievement(std::string achievement_name)
{
   // TODO: make this an event_emitter->emit_unlock_achievement_event();
   event_emitter->emit_play_sound_effect_event("achievement_unlocked_sound");
   event_emitter->emit_event(
      ALLEGRO_FLARE_EVENT_UNLOCK_ACHIEVEMENT,
      intptr_t(new std::string(achievement_name))
   );
   return;
}

void Runner::push_screen()
{
   prior_screens_stack.push_back(framework->get_currently_active_screen_name());
   return;
}

std::string Runner::pop_screen()
{
   if (!prior_screen_is_in_stack()) return "";
   std::string value_to_return = prior_screens_stack.back();
   prior_screens_stack.pop_back();
   return value_to_return;
}

void Runner::reactivate_prior_screen(std::string fallback_if_stack_is_empty)
{
   std::string screen_to_activate = prior_screen_is_in_stack() ? pop_screen() : fallback_if_stack_is_empty;
   framework->activate_screen(screen_to_activate);
   return;
}

bool Runner::prior_screen_is_in_stack()
{
   return !prior_screens_stack.empty();
}

void Runner::game_event_func(AllegroFlare::GameEvent* ev)
{
   if (!(ev))
   {
      std::stringstream error_message;
      error_message << "[Runner::game_event_func]: error: guard \"ev\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::game_event_func: error: guard \"ev\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Runner::game_event_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Runner::game_event_func: error: guard \"event_emitter\" not met");
   }
   std::string event_name = ev->get_type();
   std::cout << "EVENT_EMITTED: \"" << event_name << "\"" << std::endl;

   std::map<std::string, std::function<void()>> event_map = {
      { "initialize", [this](){
         //if (in_production_mode())
         //{
            //event_emitter->emit_play_sound_effect_event("intro_music");
            event_emitter->emit_play_music_track_event("intro_music");
            framework->activate_screen("opening_logos_storyboard_screen");
         //}
         //else
         //{
            //event_emitter->emit_game_event(AllegroFlare::GameEvent(EVENT_ACTIVATE_INVESTIGATION_ROOM_SCREEN));
            //event_emitter->emit_game_event(AllegroFlare::GameEvent("start_title_screen"));
         //}
      }},
      { "start_title_screen", [this](){
         event_emitter->emit_play_music_track_event("intro_music");
         framework->activate_screen("title_screen");
      }},
      { EVENT_START_NEW_GAME, [this](){
         finished_up_to_level = 0;
         event_emitter->emit_play_music_track_event("intro_music");
         //framework->activate_screen("opening_logos_storyboard_screen");
         framework->activate_screen("new_game_intro_storyboard_screen");
      }},
      { "finish_new_game_intro_storyboard_screen", [this](){
         // TODO: replace this with an event EVENT_ACTIVATE_PLATFORMING_2D_SCREEN
         framework->activate_screen("platforming_2d_screen");
         // WARNING: TODO: fix this so that it will not crash when there are no levels
         if (platforming_2d_world.get_levels_ref().size() == 0)
         {
            AllegroFlare::Errors::throw_error("KrampusReturns::Runner::game_event_func",
                                             "Starting gameplay but there are zero levels. Aborting");
         }
         platforming_2d_screen.load_level_and_start(&platforming_2d_world.get_levels_ref()[0]);
      }},
      { "finish_level_successfully", [this](){
         // TODO: replace this with an event EVENT_ACTIVATE_PLATFORMING_2D_SCREEN
         finished_up_to_level++;
         int next_level_to_do = finished_up_to_level;

         framework->activate_screen("platforming_2d_screen");
         // WARNING: TODO: fix this so that it will not crash when there are no levels
         bool done_with_all_levels = (next_level_to_do >= platforming_2d_world.get_levels_ref().size());
         if (done_with_all_levels)
         {
            // TODO actiavate game credits you win screen
            event_emitter->emit_game_event(AllegroFlare::GameEvent("start_game_won_outro_storyboard_screen"));
         }
         else
         {
            platforming_2d_screen.load_level_and_start(&platforming_2d_world.get_levels_ref()[next_level_to_do]);
         }
      }},
      { "start_game_won_outro_storyboard_screen",  [this](){
         event_emitter->emit_play_music_track_event("level_1_music");
         framework->activate_screen("game_won_outro_storyboard_screen");
         //framework->register_screen("game_won_outro_storyboard_screen", game_won_outro_storyboard_screen);
      }},
      { "finish_game_won_outro_storyboard_screen",  [this](){
         event_emitter->emit_game_event(AllegroFlare::GameEvent(EVENT_ACTIVATE_CREDITS_SCREEN));
         //framework->activate_screen("game_won_outro_storyboard_screen");
         //framework->register_screen("game_won_outro_storyboard_screen", game_won_outro_storyboard_screen);
      }},
      //{ EVENT_ACTIVATE_TILE_DRIVE_SCREEN, [this](){
         //framework->activate_screen("tile_drive_screen");
      //}},
      { EVENT_ACTIVATE_PLATFORMING_2D_SCREEN, [this](){
         framework->activate_screen("platforming_2d_screen");
      }},
      //{ EVENT_ACTIVATE_NAME_YOUR_CHARACTER_SCREEN, [this](){
         //framework->activate_screen("character_name_input_screen");
      //}},
      //{ EVENT_ACTIVATE_PLATFORMING_2D_SCREEN, [this](){
         //framework->activate_screen("");
      //}},
      { EVENT_ACTIVATE_CREDITS_SCREEN, [this](){
         //framework->activate_screen("credits_screen");
         event_emitter->emit_play_music_track_event("end_music");
         framework->activate_screen("rolling_credits_screen");
      }},
      { EVENT_ACTIVATE_ACHIEVEMENTS_SCREEN, [this](){
         push_screen();
         framework->activate_screen("achievements_screen");
      }},
      { EVENT_EXIT_ACHIEVEMENTS_SCREEN, [this](){
         reactivate_prior_screen("title_screen");
      }},
      //{ EVENT_ACTIVATE_INVESTIGATION_ROOM_SCREEN, [this](){
         //push_screen();
         //framework->activate_screen("investigation_room_screen");
      //}},
      //{ EVENT_EXIT_INVESTIGATION_ROOM_SCREEN, [this](){
         //reactivate_prior_screen("tile_drive_screen");
      //}},
      //{ EVENT_ACTIVATE_CUT_IN_STORYBOARD_SCREEN, [this](){
         //push_screen();
         //framework->activate_screen("cut_in_storyboard_screen");
      //}},
      //{ EVENT_CUT_IN_STORYBOARD_SCREEN_FINISHED, [this](){
         //reactivate_prior_screen();
      //}},
      { EVENT_CREDITS_SCREEN_FINISHED, [this](){
         unlock_achievement("stay_through_the_credits");
         event_emitter->emit_game_event(AllegroFlare::GameEvent("start_title_screen"));
         //framework->activate_screen("title_screen");
      }},
      { EVENT_EXIT_GAME, [this](){
         event_emitter->emit_exit_game_event();
      }},
      //{ EVENT_OPEN_CHRONICLE_SCREEN, [this](){
         //refresh_blurry_background_screen_capture();
         //chronicle_screen.set_background_bitmap(blurry_background_screen_capture);
         //push_screen();
         //framework->activate_screen("chronicle_screen");
      //}},
      //{ EVENT_CLOSE_CHRONICLE_SCREEN, [this](){
         //reactivate_prior_screen("investigation_room_screen");
      //}},
      { EVENT_PAUSE_GAME, [this](){
         push_screen();
         framework->activate_screen("pause_screen");
      }},
      { EVENT_UNPAUSE_GAME, [this](){
         reactivate_prior_screen("investigation_room_screen");
      }},
      { AllegroFlare::Prototypes::FixedRoom2D::EventNames::SCRIPT_EVENT_NAME, [this, ev](){
         using namespace AllegroFlare::Prototypes::FixedRoom2D;

         AllegroFlare::GameEventDatas::Base* game_event_data = ev->get_data();

         if (!game_event_data)
         {
            std::stringstream error_message;
            error_message << "CatDetective::Runner::game_event_func error: "
                          << "When processing a SCRIPT_EVENT, game_event_data cannot be nullptr.";
            throw std::runtime_error(error_message.str());
         }

         //if (game_event_data->is_type(ScriptEventDatas::CollectEvidence::TYPE))
         //{
            //ScriptEventDatas::CollectEvidence* collect_evidence_event_data =
                //static_cast<ScriptEventDatas::CollectEvidence*>(game_event_data);

            //std::string evidence_name = collect_evidence_event_data->get_evidence_dictionary_name_to_collect();
            //int evidence_index = find_index_of_evidence_by_identifier_or_throw(evidence_name, "game_event_func");

            //add_evidence_by_id(evidence_index);
            //emit_event_to_spawn_you_got_new_evidence_dialog(evidence_index);
         //}
         //else if (game_event_data->is_type(ScriptEventDatas::CollectItem::TYPE))
         //{
            //ScriptEventDatas::CollectItem* collect_item_event_data =
                //static_cast<ScriptEventDatas::CollectItem*>(game_event_data);

            //// TODO: actually extract and pass in the item id (or name)
            //int item_id = 1;

            //// TODO: this line:
            //// add_item_by_id(evidence_id);
            //emit_event_to_spawn_you_got_an_item_dialog(item_id);
         //}
      }},
   };



   // locate and call the function to handle the event 

   if (event_map.count(event_name) == 0)
   {
      // event not found
      std::stringstream warning_message;
      warning_message << "Event not handled: \"" << event_name << "\".";
      AllegroFlare::Errors::warn_from("KrampusReturns::Runner::game_event_func", warning_message.str());
   }
   else
   {
      // call the event
      event_map[event_name]();
   }


   return;
}

void Runner::run(std::string mode)
{
   // TODO: add "is_valid_mode" (or similar) and validate the mode argument

   // setup the framework
   AllegroFlare::Frameworks::Full framework;
   //framework.disable_escape_key_will_shutdown();
   framework.initialize();

   AllegroFlare::FontBin &font_bin = framework.get_font_bin_ref();
   AllegroFlare::BitmapBin &bitmap_bin = framework.get_bitmap_bin_ref();
   AllegroFlare::SampleBin &sample_bin = framework.get_sample_bin_ref();
   AllegroFlare::ModelBin &model_bin = framework.get_model_bin_ref();

   // TODO: develop a strategy where this branching can have dependable production
   // data when switching to "production"
   if (is_test_mode(mode) || is_development_mode(mode))
   {
      font_bin.set_full_path("/Users/markoates/Repos/KrampusReturns/bin/programs/data/fonts");
      bitmap_bin.set_full_path("/Users/markoates/Repos/KrampusReturns/bin/programs/data/bitmaps");
      sample_bin.set_full_path("/Users/markoates/Repos/KrampusReturns/bin/programs/data/samples");
      model_bin.set_full_path("/Users/markoates/Repos/KrampusReturns/bin/programs/data/models");
   }

   Runner runner(mode, &framework, &framework.get_event_emitter_ref());
   runner.initialize();
   framework.register_screen("runner", &runner);

   framework.run_loop();

   return;
}

bool Runner::is_test_mode(std::string mode)
{
   return mode == MODE_TEST;
}

bool Runner::is_development_mode(std::string mode)
{
   return mode == MODE_DEVELOPMENT;
}

bool Runner::is_production_mode(std::string mode)
{
   return mode == MODE_PRODUCTION;
}

bool Runner::in_development_mode()
{
   return mode == MODE_DEVELOPMENT;
}

bool Runner::in_production_mode()
{
   return mode == MODE_PRODUCTION;
}

bool Runner::in_test_mode()
{
   return mode == MODE_TEST;
}


} // namespace KrampusReturns


