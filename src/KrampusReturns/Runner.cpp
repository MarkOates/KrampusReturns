

#include <KrampusReturns/Runner.hpp>

#include <AllegroFlare/Color.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/Prototypes/FixedRoom2D/EventNames.hpp>
#include <AllegroFlare/Prototypes/FixedRoom2D/ScriptEventDatas/CollectEvidence.hpp>
#include <AllegroFlare/Prototypes/FixedRoom2D/ScriptEventDatas/CollectItem.hpp>
#include <AllegroFlare/StoryboardFactory.hpp>
#include <Wicked/Entities/Basic2DFactory.hpp>
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
   , platforming_2d_screen()
   , achievements_screen()
   , credits_screen(nullptr)
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
   audio_controller.set_and_load_sound_effect_elements({
     //{ "menu-click-01.wav", { "menu-click-01.wav", false } },
     //{ "door-locked-hall", { "door-locked-hall-03.ogg", false } },
     //{ "doorbell", { "doorbell-02.ogg", false } },
     //{ "intro_music", { "krampus-intro-1.ogg", false } },
     { "smash_club", { "smash-club-01.ogg", false } },
   });


   audio_controller.set_and_load_music_track_elements({
     //{ "intro_music", { "krampus-intro-1.ogg", false } },
     { "intro_music", { "krampus-intro-1.wav", false } },

     //{ "rain_on_roof", { "indoor-rain-looped-03.ogg", true } },
     //{ "heavy_outdoor_rain", { "heavy-rain-looped-01.ogg", true } },
     ////{ "file_example_OOG_1MG.ogg", { "file_example_OOG_1MG.ogg", true } },
     //{ "rain_while_in_basement", { "rain_while_in_basement.ogg", true } },
   });



   // setup the title screen
   title_screen.set_font_bin(&font_bin);
   title_screen.set_bitmap_bin(&bitmap_bin);
   title_screen.set_title_text("KRAMPUS RETURNS");
   title_screen.set_title_text_color(AllegroFlare::Color(AllegroFlare::Color::AliceBlue, 1.0).to_al());
   title_screen.set_title_font_size(-130);
   title_screen.set_font_name("ChronoTrigger.ttf");
   //title_screen.set_title_text_color(AllegroFlare::Color(0x000000, 1.0).to_al());
   //title_screen.set_background_bitmap_name("placeholder-title-background.jpg");
   title_screen.set_event_emitter(&event_emitter);
   title_screen.set_menu_font_size(-40);
   title_screen.set_menu_position_y(1080/32*19+10);
   title_screen.set_copyright_text("Copyright 2022 - Mark Oates - CLUBCATT Games - www.clubcatt.com\n");
   title_screen.set_copyright_text_color(AllegroFlare::Color(0x72aedd, 0.8).to_al());
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
         // music starts
         "December 2022",
         //"Christmas Eve.",
         //"[image showing aliens invading]",
         "Aliens have invaded Earth",
         "They are after Santa's magical powers",

         "And this night, Christmas Eve, is when his magical powers are the strongest",
         //"[image showing santa's power chaimber, glowing]",

         "Santa and his elves were unable to stop the aliens",
         //"[image showing santa's losing battle]",

         "With all options gone, Santa must no do the unthinkable",
         //"[image showing krampus with his back to the screen]",
         "Santa has no choice. He must face his biggest fear...",

         //"[image showing santa appraoching krampus]",
         "...and team up with his ultimate rival...",
         //"[image showing krampus turned around]",
         "Krampus.",
      });
   new_game_intro_storyboard_screen->set_event_emitter(&event_emitter);
   new_game_intro_storyboard_screen->set_game_event_name_to_emit_after_completing(
      "finish_new_game_intro_storyboard_screen"
      //"activate_nvestigation_room_screen"
   );
   framework->register_screen("new_game_intro_storyboard_screen", new_game_intro_storyboard_screen);



   setup_platforming_2d_screen();


   // setup the credits screen
   credits_screen = storyboard_factory.create_advancing_text_storyboard_screen({
         //"Thank you to Shawn Hargreaves for creating the first version of Allegro in the 90s.",
         "Thank you to all the members of the Allegro game programming community.",
            //"which had a huge impact in my development as a programmer.",
         "Thank you to Elias and SiegeLord for your continued dedication to Allegro.",
         "Thank you to amarillion for your work to support Allegro, including creating KrampuHack.",
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

void Runner::setup_platforming_2d_screen()
{
   //return;
   static std::string TEST_BASE_FOLDER = "/Users/markoates/Repos/KrampusReturns/bin/programs/data/";
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
         framework->activate_screen("title_screen");
      }},
      { EVENT_START_NEW_GAME, [this](){
         event_emitter->emit_play_music_track_event("intro_music");
         //framework->activate_screen("opening_logos_storyboard_screen");
         framework->activate_screen("new_game_intro_storyboard_screen");
      }},
      { "finish_new_game_intro_storyboard_screen", [this](){
         // TODO: replace this with an event EVENT_ACTIVATE_PLATFORMING_2D_SCREEN
         framework->activate_screen("platforming_2d_screen");
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
         framework->activate_screen("credits_screen");
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
         framework->activate_screen("title_screen");
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
      std::cout << "ERROR: event not found: \"" << event_name << "\"" << std::endl;
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


