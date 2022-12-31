

#include <KrampusReturns/Gameplay/Screen.hpp>

#include <AllegroFlare/CameraControlStrategies2D/HorizontalRail.hpp>
#include <AllegroFlare/CameraControlStrategies2D/SmoothSnap.hpp>
#include <AllegroFlare/CameraControlStrategies2D/SmoothSnapWithZoomEffect.hpp>
#include <AllegroFlare/CameraControlStrategies2D/Snap.hpp>
#include <AllegroFlare/Color.hpp>
#include <AllegroFlare/Elements/HealthBars/Classic.hpp>
#include <AllegroFlare/Elements/HealthBars/Hearts.hpp>
#include <AllegroFlare/Errors.hpp>
#include <AllegroFlare/EventNames.hpp>
#include <AllegroFlare/Physics/AABB2D.hpp>
#include <AllegroFlare/Physics/TileMapCollisionStepper.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2DFactory.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Doors/Basic2D.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/EntityCollectionHelper.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <ChatGPT/Seeker.hpp>
#include <KrampusReturns/CameraControlStrategies2D/SmoothSnapWithFX.hpp>
#include <KrampusReturns/Entities/Blob.hpp>
#include <KrampusReturns/Entities/DamageZone.hpp>
#include <KrampusReturns/EntityFactory.hpp>
#include <KrampusReturns/GameEventDatas/GoalpostReached.hpp>
#include <KrampusReturns/GameEventDatas/SpawnDamageZoneByPlayer.hpp>
#include <KrampusReturns/GameEventDatas/SpawnFlashFX.hpp>
#include <KrampusReturns/Shaders/AllegroDefault.hpp>
#include <KrampusReturns/Shaders/Primary.hpp>
#include <KrampusReturns/TMJObjectLoader.hpp>
#include <algorithm>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_font.h>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{
namespace Gameplay
{


Screen::Screen(AllegroFlare::BitmapBin* bitmap_bin, AllegroFlare::FontBin* font_bin, AllegroFlare::Display* display, AllegroFlare::EventEmitter* event_emitter)
   : AllegroFlare::Screens::Base("Prototypes::Platforming2D::Screen")
   , bitmap_bin(bitmap_bin)
   , font_bin(font_bin)
   , display(display)
   , event_emitter(event_emitter)
   , animation_book()
   , animation_book_initialized(false)
   , native_display_resolution_width(1920)
   , native_display_resolution_height(1080)
   , initialized(false)
   , destroyed(false)
   , currently_active_map(nullptr)
   , currently_active_map_name("[currently-active-map-name-unset]")
   , entity_pool({})
   , map_dictionary({})
   , gravity(0.0f)
   , gravity_reversed(false)
   , camera()
   , camera_baseline_zoom({4.8f, 4.5f})
   , krampus_controller({})
   , player_controlled_entity(nullptr)
   , shader(nullptr)
   , show_tile_mesh(true)
   , show_collision_tile_mesh(false)
   , camera_control_strategy(nullptr)
   , backbuffer_sub_bitmap(nullptr)
   , showing_full_color_overlay(false)
   , full_color_overlay_color(ALLEGRO_COLOR{1.0, 0.0, 0.0, 1.0})
   , full_color_overlay_opacity(0.2)
   , showing_banner_text(false)
   , banner_text_color(ALLEGRO_COLOR{1.0, 0.0, 0.0, 1.0})
   , banner_text("[unnset-banner_text]")
   , showing_banner_subtext(false)
   , banner_subtext_color_a(ALLEGRO_COLOR{1.0, 0.0, 0.0, 1.0})
   , banner_subtext_color_b(ALLEGRO_COLOR{1.0, 0.0, 0.0, 1.0})
   , banner_subtext("[unnset-banner_subtext]")
   , banner_subtext_pulse_rate_per_sec(8.0)
   , banner_subtext_shown_at(0.0)
   , state(0)
   , state_changed_at(0.0f)
   , state_is_busy(false)
   , current_level_data()
   , main_background_music_identifier("[unnset-main_background_music_identifier]")
   , little_shadow_bitmap(nullptr)
   , DUMMY_DEP(nullptr)
{
}


Screen::~Screen()
{
}


void Screen::set_entity_pool(std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> entity_pool)
{
   this->entity_pool = entity_pool;
}


void Screen::set_camera_baseline_zoom(AllegroFlare::Vec2D camera_baseline_zoom)
{
   this->camera_baseline_zoom = camera_baseline_zoom;
}


void Screen::set_show_tile_mesh(bool show_tile_mesh)
{
   this->show_tile_mesh = show_tile_mesh;
}


void Screen::set_show_collision_tile_mesh(bool show_collision_tile_mesh)
{
   this->show_collision_tile_mesh = show_collision_tile_mesh;
}


AllegroFlare::BitmapBin* Screen::get_bitmap_bin() const
{
   return bitmap_bin;
}


AllegroFlare::FontBin* Screen::get_font_bin() const
{
   return font_bin;
}


std::map<std::string, std::string> Screen::get_map_dictionary() const
{
   return map_dictionary;
}


AllegroFlare::Vec2D Screen::get_camera_baseline_zoom() const
{
   return camera_baseline_zoom;
}


AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* Screen::get_player_controlled_entity() const
{
   return player_controlled_entity;
}


bool Screen::get_show_tile_mesh() const
{
   return show_tile_mesh;
}


bool Screen::get_show_collision_tile_mesh() const
{
   return show_collision_tile_mesh;
}


AllegroFlare::FrameAnimation::Book &Screen::get_animation_book_ref()
{
   return animation_book;
}


void Screen::NOTE()
{
   // Interesting new pattern.  A  from an external class is used to initialize a value, but the type
   // is not picked up by the quintessence extrapolation. So, as a HACK, I'm adding a dummy property
   // DUMMY_DEP so it will list the extrenal class as a dependency (and include the header).
   return;
}

void Screen::set_state(uint32_t state, float time_now)
{
   if (!((state != STATE_UNDEF)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_state]: error: guard \"(state != STATE_UNDEF)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_state: error: guard \"(state != STATE_UNDEF)\" not met");
   }
   if (state_is_busy) return;
   if (this->state == state) throw std::runtime_error("Gameplay::Screen: error: duplicate state");
   this->state = state;
   state_changed_at = time_now;

   switch(state)
   {
      case STATE_SHUTDOWN:
      case STATE_DOING_SHUTDOWN:
      case STATE_PRELOADING_LEVEL:
         // shutdown handled at call
      break;

      case STATE_PLAYING_IN_LEVEL:
         play_level_music();
         //event_emitter->emit_play_music_track_event(main_background_music_identifier);
      break;

      case STATE_PLAYER_DIED:
         set_full_color_overlay(al_color_name("firebrick"), 0.2);
         event_emitter->emit_stop_all_music_tracks_event();
         event_emitter->emit_play_sound_effect_event("fail_moan_sound");
         show_full_color_overlay();
         shake_camera(4, 1.0, time_now);
      break;

      case STATE_FINISHED_LEVEL:
         set_full_color_overlay(al_color_name("white"), 0.1);
         show_full_color_overlay();
         play_win_music();
      break;

      case STATE_WAITING_KEYPRESS_TO_FINISH_LEVEL:
      break;

      case STATE_WAITING_KEYPRESS_TO_RETRY_LEVEL:
      break;

      default:
         AllegroFlare::Errors::throw_error("KrampusReturns::Gameplay::Screen::set_state", "unhandled state");
      break;
   }
   return;
}

void Screen::update_state(float time_now)
{
   if (!((state != STATE_UNDEF)))
   {
      std::stringstream error_message;
      error_message << "[Screen::update_state]: error: guard \"(state != STATE_UNDEF)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::update_state: error: guard \"(state != STATE_UNDEF)\" not met");
   }
   float state_age = time_now - state_changed_at;

   switch(state)
   {
      case STATE_SHUTDOWN:
      case STATE_DOING_SHUTDOWN:
      case STATE_PRELOADING_LEVEL:
         // nothing
      break;

      case STATE_PLAYING_IN_LEVEL:
         if (player_controlled_entity) krampus_controller.update();
         update_entities();
      break;

      case STATE_WAITING_KEYPRESS_TO_RETRY_LEVEL:
      case STATE_PLAYER_DIED:
         {
            update_entities();
            if (state_age > 2.0 && !showing_banner_text)
            {
               // TODO: add fail music
               event_emitter->emit_play_music_track_event("dead_krampus_music");
               set_banner_text("YOU LOSE", al_color_name("firebrick"));
               show_banner_text();
            }

            float subtext_starts_at_age = 6.25f;
            if (!showing_banner_subtext && state_age > subtext_starts_at_age)
            {
               set_banner_subtext(
                  "Press any key to continue",
                  al_color_name("darkorange"),
                  al_color_name("firebrick"),
                  3.0
               );
               show_banner_subtext();
               set_state(STATE_WAITING_KEYPRESS_TO_RETRY_LEVEL);
            }
         }
      break;

      case STATE_WAITING_KEYPRESS_TO_FINISH_LEVEL:
      case STATE_FINISHED_LEVEL:
         if (state_age > 1.62 && !showing_banner_text)
         {
            show_banner_text();
         }
         if (showing_banner_text)
         {
            float fade_out_to_white_text_counter = 0.0;
            float fade_out_starts_at_age = 3.0;
            float fade_out_to_white_duration = 3.0;
            ALLEGRO_COLOR sparkle_win_color;
            ALLEGRO_COLOR win_color_final = al_color_name("aquamarine");
            ALLEGRO_COLOR final_level_clear_color;

            if (state_age > fade_out_starts_at_age) // TODO: fix this bug where it repeats the fading from
                                                    // strobe when in STATE_WAITING_KEYPRESS_TO_FINISH
                                                    // after STATE_FINISHED_LEVEL
            {
               fade_out_to_white_text_counter =
                  std::min(1.0f, std::max(0.0f, (state_age - fade_out_starts_at_age) / fade_out_to_white_duration));
            }

            sparkle_win_color = get_sparkle_win_color();
            final_level_clear_color = AllegroFlare::color::mix(
               sparkle_win_color,
               win_color_final,
               fade_out_to_white_text_counter
            );
            set_banner_text("LEVEL CLEAR", final_level_clear_color);

            float subtext_starts_at_age = 6.25f;
            if (!showing_banner_subtext && state_age > subtext_starts_at_age)
            {
               set_banner_subtext(
                  "Press any key to continue",
                  al_color_name("turquoise"),
                  al_color_name("white"),
                  8.0
               );
               show_banner_subtext();
               set_state(STATE_WAITING_KEYPRESS_TO_FINISH_LEVEL);
            }
         }
      break;

      default:
         AllegroFlare::Errors::throw_error("KrampusReturns::Gameplay::Screen::update_state", "unhandled state");
      break;
   }
   return;
}

ALLEGRO_COLOR Screen::get_sparkle_win_color()
{
   static int strobe = 0;
   strobe++;
   if (strobe > 6) { strobe = 0; }

   ALLEGRO_COLOR result;

   static const char *COLOR_RED =   "ea3377";
   static const char *COLOR_YELLOW ="fef653";
   static const char *COLOR_BLUE =  "5eccfa";
   static const char *COLOR_WHITE = "ffffff";
   static const char *COLOR_GREEN = "96fc4d";
   static const char *COLOR_BLACK = "000000";
   static const char *LIGHT_OFF =   "404846";

   if (strobe == 0) result = al_color_name("orange");
   if (strobe == 1) result = al_color_html(COLOR_WHITE);
   if (strobe == 2) result = al_color_html(COLOR_YELLOW);
   if (strobe == 3) result = al_color_html(COLOR_WHITE);
   if (strobe == 4) result = al_color_html(COLOR_BLUE);
   if (strobe == 5) result = al_color_html(COLOR_WHITE);
   if (strobe == 6) result = al_color_html(COLOR_GREEN);

   return result;
}

void Screen::play_win_music()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::play_win_music]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::play_win_music: error: guard \"event_emitter\" not met");
   }
   event_emitter->emit_play_music_track_event("win_music");
}

void Screen::play_level_music()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::play_level_music]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::play_level_music: error: guard \"event_emitter\" not met");
   }
   event_emitter->emit_play_music_track_event(main_background_music_identifier);
}

void Screen::play_boss_music()
{
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::play_boss_music]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::play_boss_music: error: guard \"event_emitter\" not met");
   }
   event_emitter->emit_play_music_track_event("boss_music");
}

void Screen::set_map_dictionary(std::map<std::string, std::string> map_dictionary)
{
   // WARNING: this was previously set with "guards: [ (!initialized) ]", Unsure of its removal
   this->map_dictionary = map_dictionary;
   // TODO: allow this to be set after initialization
   return;
}

void Screen::set_display(AllegroFlare::Display* display)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_display]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_display: error: guard \"(!initialized)\" not met");
   }
   this->display = display;
   return;
}

void Screen::set_event_emitter(AllegroFlare::EventEmitter* event_emitter)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_event_emitter]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_event_emitter: error: guard \"(!initialized)\" not met");
   }
   this->event_emitter = event_emitter;
   return;
}

void Screen::set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_bitmap_bin]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_bitmap_bin: error: guard \"(!initialized)\" not met");
   }
   this->bitmap_bin = bitmap_bin;
   return;
}

void Screen::set_font_bin(AllegroFlare::FontBin* font_bin)
{
   if (!((!initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::set_font_bin]: error: guard \"(!initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::set_font_bin: error: guard \"(!initialized)\" not met");
   }
   this->font_bin = font_bin;
   return;
}

void Screen::set_currently_active_map(std::string name)
{
   currently_active_map = find_map_by_name(name);
   if (!currently_active_map) throw std::runtime_error("Bruh, no map");
   currently_active_map_name = name;
   return;
}

void Screen::set_player_controlled_entity(KrampusReturns::Entities::Krampus* player_controlled_entity)
{
   // TODO: Consider having a warning displayed on the screen if there is no actively controlled character.

   // NOTE: for now, this "player_controlled_entity" is a Krampus type. This will likely change eventually.
   this->player_controlled_entity = player_controlled_entity;

   if (player_controlled_entity)
   {
      krampus_controller.set_krampus(player_controlled_entity);
      krampus_controller.reset();
      reset_camera_control(player_controlled_entity);
   }
   else
   {
      AllegroFlare::Errors::throw_error("KrampusReturns::Gameplay::Screen::set_player_controlled_entity",
         "Currently there is no support for setting this value to nullptr.");
   }
   return;
}

AllegroFlare::Prototypes::Platforming2D::Entities::TileMaps::Basic2D* Screen::find_map_by_name(std::string name)
{
   AllegroFlare::Prototypes::Platforming2D::EntityCollectionHelper collection_helper(&entity_pool);
   AllegroFlare::Prototypes::Platforming2D::Entities::TileMaps::Basic2D* found_map =
      collection_helper.find_map_by_name(name);
   if (!found_map)
   {
      std::stringstream error_message;
      error_message << "map named \"" << name << "\" does not exist.";
      throw std::runtime_error(error_message.str());

   }

   return found_map;
}

void Screen::on_activate()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::on_activate]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::on_activate: error: guard \"initialized\" not met");
   }
   //load_level_and_start();
   // nothing here
   return;
}

void Screen::on_deactivate()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::on_deactivate]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::on_deactivate: error: guard \"initialized\" not met");
   }
   // nothing here
   return;
}

void Screen::destroy_all()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::destroy_all]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::destroy_all: error: guard \"initialized\" not met");
   }
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[Screen::destroy_all]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::destroy_all: error: guard \"(!destroyed)\" not met");
   }
   std::cout << "Destroying everything in Gameplay::Screen... " << std::endl;
   set_state(STATE_DOING_SHUTDOWN);

   // flag all entities for deletion
   flag_all_entities_for_deletion();

   // delete all the entities
   cleanup_entities_flagged_for_deletion();

   if (!entity_pool.empty())
   {
      AllegroFlare::Errors::throw_error(
         "KrampusReturns::Gameplay::Screen::load_level_and_start",
         "While cleaning/deleting all existing entities, some entities unexpectedly remained in the pool."
      );
   }

   // clear/reset the cached elements
   currently_active_map = nullptr;
   currently_active_map_name = "[unset-currently_active_map";
   player_controlled_entity = nullptr;

   map_dictionary.clear();

   krampus_controller.set_krampus(nullptr);
   krampus_controller.reset();

   delete shader;
   delete camera_control_strategy;
   al_destroy_bitmap(backbuffer_sub_bitmap);

   // hide all the banners
   hide_banner_text();
   hide_banner_subtext();
   hide_full_color_overlay();


   if (little_shadow_bitmap)
   {
      delete little_shadow_bitmap;
      little_shadow_bitmap = nullptr;
   }


   initialized = false;
   destroyed = true;

   set_state(STATE_SHUTDOWN);

   return;
}

void Screen::load_level_and_start(KrampusReturns::Level* level)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::load_level_and_start]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::load_level_and_start: error: guard \"initialized\" not met");
   }
   if (!((!destroyed)))
   {
      std::stringstream error_message;
      error_message << "[Screen::load_level_and_start]: error: guard \"(!destroyed)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::load_level_and_start: error: guard \"(!destroyed)\" not met");
   }
   set_state(STATE_PRELOADING_LEVEL);


   ///////////
   // cleanup contents from any previously loaded level
   ///////////

   // NOTE: This line is an artifiact due to initialization not being simple in this current design.
   // HACK:
   // TODO: fix initialization so that animation book is included with Gameplay::Screen::initialize().  As a
   // precursor, allow Gameplay::Screen::initialize before setting maps, currently_active_level, etc.
   if (!animation_book_initialized) initialize_animation_book();

   // cleanup
   flag_all_entities_for_deletion();
   cleanup_entities_flagged_for_deletion();

   if (!entity_pool.empty())
   {
      AllegroFlare::Errors::throw_error(
         "KrampusReturns::Gameplay::Screen::load_level_and_start",
         "While cleaning/deleting all existing entities, some entities unexpectedly remained in the pool."
      );
   }



   // destroy the little shadow bitmap if it exists

   if (little_shadow_bitmap)
   {
      delete little_shadow_bitmap;
      little_shadow_bitmap = nullptr;
   }

   // load up the little shadow bitmap
   //{
   //}

   //if (little_shadow_bitmap)

   //little_shadow_bitmap = animation_book.get_still_frame("fly");

   //set_player_controlled_entity(nullptr);


   // reset the music to defaults
    //type: std::string
    //init_with: '"[unnset-banner_subtext]"'
   main_background_music_identifier = "level_1_music";



   // hide all the banners
   hide_full_color_overlay();
   hide_banner_text();
   hide_banner_subtext();



   ///////////
   // grab the map dictionary from this level (if present)
   ///////////

   // TODO: CRITICAL: fix this folder
   #if defined(_WIN32) || defined(_WIN64)
   #define TEST_BASE_FOLDER "/msys64/home/Mark/Repos/KrampusReturns/bin/programs/data/"
   #else
   #define TEST_BASE_FOLDER "/Users/markoates/Repos/KrampusReturns/bin/programs/data/"
   #endif


   if (!level)
   {
      set_map_dictionary({
         //{ "map_a", TEST_BASE_FOLDER "maps/krampus-returns-map01-0x.tmj" },
         //{ "map_b", TEST_BASE_FOLDER "maps/krampus-returns-map02-0x.tmj" },
         { "map_a", TEST_BASE_FOLDER "maps/krampus-returns-level-1-1-0x.tmj" },
      });
   }
   else
   {
      // load the map dictionary
      set_map_dictionary(level->get_map_dictionary());

      // set the main background music
      main_background_music_identifier = level->get_music_identifier();

      // set our own copy of the "current_level_data" from the passed in level
      current_level_data = *level;
   }



   initialize_maps();
   load_objects_from_map_files();



   ///////////
   // locate the spawn point for this level
   ///////////

   // TODO: find first spawn point and use
   // for each map in map dictionary...
   //      select_spawn_points_on_map_name ...
   //      use the first one

   int num_spawn_points = count_num_spawn_points_in_all_maps();
   AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D *spawn_point = find_first_spawn_point_on_any_map();
   if (num_spawn_points == 0)
   {
      AllegroFlare::Errors::warn_from("Krampus::Gameplay::Screen::load_level_and_start",
                                      "Expecting a spawn points to be present in this level, but there is not one."
                                      );
   }
   if (num_spawn_points > 1)
   {
      AllegroFlare::Errors::warn_from("Krampus::Gameplay::Screen::load_level_and_start",
                                      "Expecting only one spawn point to be present in this level, but there are "
                                      "multiple ones. Proceeding to use the first one found."
                                      );
   }




   ///////////
   // create the player character
   ///////////

   KrampusReturns::EntityFactory entity_factory;
   entity_factory.set_event_emitter(event_emitter);
   entity_factory.set_bitmap_bin(bitmap_bin);
   entity_factory.set_animation_book(&animation_book);

   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   std::string MAP_NAME_TO_START_ON = spawn_point->get(ON_MAP_NAME);

   KrampusReturns::Entities::Krampus *krampus = entity_factory.create_krampus(
      MAP_NAME_TO_START_ON, 400/2 - 50, 240/2);
   add_entity_to_pool(krampus);

   //
   move_krampus_to_first_spawn_point_or_default(krampus, MAP_NAME_TO_START_ON);


   set_player_controlled_entity(krampus);



   ///////////
   // set the start map
   ///////////

   set_currently_active_map(MAP_NAME_TO_START_ON);


   ///////////
   // start it up!
   ///////////

   start_level();
   return;
}

void Screen::start_level()
{
   set_state(STATE_PLAYING_IN_LEVEL);
   return;
}

void Screen::move_krampus_to_first_spawn_point_or_default(KrampusReturns::Entities::Krampus* krampus, std::string map_name)
{
   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> spawn_points =
      select_spawn_points_on_map_name(map_name);

   static AllegroFlare::Vec2D DEFAULT_SPAWN_LOCATION = AllegroFlare::Vec2D(400/2 + 50, 240/2);
   AllegroFlare::Vec2D spawn_location;

   if (spawn_points.empty())
   {
      AllegroFlare::Errors::warn_from("Krampus::Gameplay::Screen::move_krampus_to_first_spawn_point_or_default",
                                      "Expecting spawn points to be present on this map, but there are none. Will "
                                      "proceed using a DEFAULT_SPAWN_LOCATION."
                                      );
      spawn_location = DEFAULT_SPAWN_LOCATION;
   }
   if (spawn_points.size() > 1)
   {
      AllegroFlare::Errors::warn_from("Krampus::Gameplay::Screen::move_krampus_to_first_spawn_point_or_default",
                                      "Note there are multiple spawn points on this map. Will proceed by using "
                                      "the first that was loaded when the map was loaded from the file."
                                      );
   }

   spawn_location = spawn_points[0]->get_place_ref().position;

   krampus->get_place_ref().position = spawn_location;
   //krampus->set(ON_MAP_NAME, map_name);
   //krampus->get_place_ref().position = { x, y };
   return;
}

void Screen::load_objects_from_map_files()
{
   for (auto &map_dictionary_listing : map_dictionary)
   {
      std::string map_name = map_dictionary_listing.first;
      std::string map_filename = map_dictionary_listing.second;

      KrampusReturns::TMJObjectLoader loader(map_filename);
      loader.set_object_parsed_callback(tmj_object_parse_callback_func);

      //std::string *passing_map_name = new std::string(map_name);
      std::pair<KrampusReturns::Gameplay::Screen*, std::string> *user_data =
         new std::pair<KrampusReturns::Gameplay::Screen*, std::string>{this, map_name};
      
      loader.set_object_parsed_callback_user_data(user_data);
      loader.load();

      //delete passing_map_name;
      delete user_data;
   }
}

std::pair<int, int> Screen::calc_room_coords(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity)
{
   if (!entity) return std::pair<int, int>(0, 0);

   std::pair<float, float> room_width = calc_room_width();

   return std::pair<int, int>(
      (int)(entity->get_place_ref().position.x / room_width.first),
      (int)(entity->get_place_ref().position.y / room_width.second)
   );
}

std::pair<float, float> Screen::calc_room_width()
{
   if (currently_active_map && currently_active_map->get_tile_mesh())
   {
      float room_width = currently_active_map->get_tile_mesh()->get_tile_width() * 25;
      float room_height = currently_active_map->get_tile_mesh()->get_tile_height() * 15;
      return {room_width, room_height};
   }
   return {0.0, 0.0};
}

AllegroFlare::Vec2D Screen::center_of(float x, float y, float w, float h)
{
   return AllegroFlare::Vec2D(x + w*0.5, y + h*0.5);
}

void Screen::tmj_object_parse_callback_func(std::string object_class, float x, float y, float w, float h, void* user_data)
{
   if (!(user_data))
   {
      std::stringstream error_message;
      error_message << "[Screen::tmj_object_parse_callback_func]: error: guard \"user_data\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::tmj_object_parse_callback_func: error: guard \"user_data\" not met");
   }
   std::pair<KrampusReturns::Gameplay::Screen*, std::string> *as_custom_user_data=
      static_cast<std::pair<KrampusReturns::Gameplay::Screen*, std::string>*>(user_data);

   std::string map_name = as_custom_user_data->second;
   KrampusReturns::Gameplay::Screen* gameplay_screen = as_custom_user_data->first;

   KrampusReturns::EntityFactory entity_factory;
   //entity_factory.set_init_entities_drawing_debug(true); // <--- TESTING DEBUG:
   entity_factory.set_event_emitter(gameplay_screen->event_emitter);
   entity_factory.set_bitmap_bin(gameplay_screen->bitmap_bin);
   entity_factory.set_animation_book(&gameplay_screen->animation_book);

   std::map<std::string, std::function<void()>> object_map = {
      { "goal", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          // TODO: extract goalpost number (if present)
          int goalpost_id = 1;

          auto entity = entity_factory.create_goalpost("map_a", goalpost_id, center.x, center.y);
          gameplay_screen->add_entity_to_pool(entity);
      }},
      //{ "boss", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          // TODO: here, replace this blob with a proper boss
          //AllegroFlare::Vec2D center = center_of(x, y, w, h);
          //auto entity = entity_factory.create_blob("map_a", center.x, center.y);
          //gameplay_screen->add_entity_to_pool(entity);
      //}},
      { "blob", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          auto entity = entity_factory.create_blob("map_a", center.x, center.y);
          gameplay_screen->add_entity_to_pool(entity);
      }},
      { "Enemy", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          // NOTE: could consider loading this "custom class" of "Enemy" as defined in Tiled
          //auto entity = entity_factory.create_skull_head_boss_enemy(
            //"map_a", center.x, center.y, w, h, "teleport_boss");
          //entity->set("is_boss");
          //gameplay_screen->add_entity_to_pool(entity);
      }},
      { "skull_head_boss", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          auto entity = entity_factory.create_skull_head_boss_enemy(
            "map_a", center.x, center.y, w, h, "teleport_boss");
          entity->set("is_boss");
          gameplay_screen->add_entity_to_pool(entity);
      }},
      { "skeleton", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          auto entity = entity_factory.create_skeleton_enemy("map_a", center.x, center.y, "skeleton");
          gameplay_screen->add_entity_to_pool(entity);
      }},
      { "attack_up_item", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          auto entity = entity_factory.create_attack_up_item("map_a", center.x, center.y);
          gameplay_screen->add_entity_to_pool(entity);
      }},
      { "flaming_skull", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          //gameplay_screen->player_controlled_entity;
          auto entity = entity_factory.create_flaming_skull_enemy("map_a", center.x, center.y, "fly", nullptr);
          gameplay_screen->add_entity_to_pool(entity);
      }},
      { "flaming_skull_red", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          //gameplay_screen->player_controlled_entity;
          auto entity = entity_factory.create_flaming_skull_red_enemy("map_a", center.x, center.y, "fly_red", nullptr);
          gameplay_screen->add_entity_to_pool(entity);
      }},
      { "door", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          // TODO: here
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          auto entity = entity_factory.create_door("map_a", center.x, center.y);
          gameplay_screen->add_entity_to_pool(entity);
      }},
      { "spawn_point", [x, y, w, h, map_name, entity_factory, gameplay_screen](){
          AllegroFlare::Vec2D center = center_of(x, y, w, h);
          auto entity = entity_factory.create_spawn_point("map_a", center.x, center.y);
          gameplay_screen->add_entity_to_pool(entity);
      }},
   };

   // locate and call the function to handle the object
   if (object_map.count(object_class) == 0)
   {
      std::stringstream message;
      message << "Object not handled for object of class \"" << object_class << "\"";
      AllegroFlare::Errors::warn_from(
         "KrampusReturns::Gameplay::Screen::tmj_object_parse_callback_func",
         message.str()
      );
   }
   else
   {
      // call the object handling function
      object_map[object_class]();
   }

   return;
}

void Screen::initialize_maps()
{
   AllegroFlare::Prototypes::Platforming2D::Entities::Basic2DFactory factory(bitmap_bin);
   AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D *created_map_ = nullptr;

   // TODO: clean this up
   for (auto &map_dictionary_entry : map_dictionary)
   {
      std::string map_name = std::get<0>(map_dictionary_entry);
      std::string map_filename = std::get<1>(map_dictionary_entry);

      created_map_ = factory.create_tile_map(map_filename, map_name);
      AllegroFlare::Prototypes::Platforming2D::Entities::TileMaps::Basic2D* created_map =
         static_cast<AllegroFlare::Prototypes::Platforming2D::Entities::TileMaps::Basic2D*>(created_map_);

      if (!created_map)
      {
         std::cout << "ERROR: Could not create map \"" << map_filename << "\"" << std::endl;
      }
      else
      {
         std::cout << "NOTE: TMJ Tile map file \"" << map_filename << "\" loaded successfully." << std::endl;
      }

      if (!created_map->get_tile_mesh())
      {
         std::cout << "ERROR: could not create tile mesh on \"" << map_filename << "\"" << std::endl;
      }
      else
      {
         std::cout << "NOTE: TMJ Tile loaded tile mesh \"" << map_filename << "\" loaded successfully." << std::endl;
      }

      if (!created_map->get_tile_mesh()->get_initialized())
      {
         std::cout << "ERROR: prim mesh not initialized on \"" << map_filename << "\"" << std::endl;
      }
      else
      {
         std::cout << "NOTE: prim mesh initialized on \"" << map_filename << "\" loaded successfully." << std::endl;
      }

      if (!created_map->get_collision_tile_mesh())
      {
         std::cout << "ERROR: collision tile map not loaded on \"" << map_filename << "\"" << std::endl;
      }
      else
      {
         std::cout << "NOTE: collision tile map loaded on \"" << map_filename << "\" loaded successfully." << std::endl;
      }


      entity_pool.push_back(created_map);
   }


   //set_currently_active_map("map_a");

   return;
}

void Screen::add_entity_to_pool(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity)
{
   entity_pool.push_back(entity);
   return;
}

void Screen::reset_camera_control(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity_to_follow)
{
   float assumed_tile_width = 16.0f;
   float assumed_tile_height = 16.0f;
   float room_width = assumed_tile_width * 25; // tile_mesh->get_real_width();
   float room_height = assumed_tile_height * 15; //tile_mesh->get_real_height();

   if (camera_control_strategy) delete camera_control_strategy;

   KrampusReturns::CameraControlStrategies2D::SmoothSnapWithFX *camera_control =
      new KrampusReturns::CameraControlStrategies2D::SmoothSnapWithFX(room_width, room_height);
   //Wicked::CameraControlStrategies2D::HorizontalRail *camera_control =
      //new Wicked::CameraControlStrategies2D::HorizontalRail; //(room_width, room_height);
   camera_control->set_camera(&camera);
   camera_control->set_entity_to_follow(entity_to_follow);
   camera_control->initialize();

   camera_control_strategy = camera_control;
   return;
}

void Screen::initialize_backbuffer_sub_bitmap()
{
   ALLEGRO_BITMAP *backbuffer = al_get_backbuffer(al_get_current_display());
   backbuffer_sub_bitmap = al_create_sub_bitmap(
      backbuffer,
      0,
      0,
      al_get_bitmap_width(backbuffer),
      al_get_bitmap_height(backbuffer)
   );

   if (!backbuffer_sub_bitmap)
   {
      std::stringstream error_message;
      error_message << "AllegroFlare::Prototypes::Platforming2D::Screen::initialize() error: "
                    << "could not create backbuffer_sub_bitmap";
      throw std::runtime_error(error_message.str());
   }
   return;
}

void Screen::initialize_animation_book()
{
   if (!((!animation_book_initialized)))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize_animation_book]: error: guard \"(!animation_book_initialized)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize_animation_book: error: guard \"(!animation_book_initialized)\" not met");
   }
   // TODO: CRITICAL: fix this path
   animation_book.set_png_source_filename(
      "/Users/markoates/Repos/KrampusReturns/bin/programs/data/bitmaps/krampus-returns-sprites-0x.png"
   );
   animation_book.set_json_source_filename(
      "/Users/markoates/Repos/KrampusReturns/bin/programs/data/bitmaps/krampus-returns-sprites-0x.json"
   );
   animation_book.set_sprite_sheet_scale(2);
   animation_book.initialize();
   animation_book_initialized = true;
   return;
}

void Screen::initialize()
{
   if (!(bitmap_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"bitmap_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"bitmap_bin\" not met");
   }
   if (!(al_get_current_display()))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize]: error: guard \"al_get_current_display()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize: error: guard \"al_get_current_display()\" not met");
   }
   //initialize_camera_control();
   initialize_backbuffer_sub_bitmap();
   //if (!animation_book_initialized) initialize_animation_book();
   initialize_camera();
   initialize_shader();
   initialized = true;
   destroyed = false;
   return;
}

void Screen::initialize_shader()
{
   if (!((!shader)))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize_shader]: error: guard \"(!shader)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize_shader: error: guard \"(!shader)\" not met");
   }
   // NOTE: Using a custom shader will requires some work because the *tile map* has a different vertex format
   //KrampusReturns::Shaders::Primary *primary_shader = new KrampusReturns::Shaders::Primary();
   //primary_shader->initialize();
   //shader = primary_shader;

   //KrampusReturns::Shaders::AllegroDefault *allegro_default_shader = new KrampusReturns::Shaders::AllegroDefault();
   //allegro_default_shader->initialize();
   //shader = allegro_default_shader;

   //shader = primary_shader;
   return;
}

void Screen::initialize_camera()
{
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize_camera]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize_camera: error: guard \"currently_active_map\" not met");
   }
   if (!(currently_active_map->get_tile_atlas()))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize_camera]: error: guard \"currently_active_map->get_tile_atlas()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize_camera: error: guard \"currently_active_map->get_tile_atlas()\" not met");
   }
   if (!(currently_active_map->get_tile_mesh()))
   {
      std::stringstream error_message;
      error_message << "[Screen::initialize_camera]: error: guard \"currently_active_map->get_tile_mesh()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::initialize_camera: error: guard \"currently_active_map->get_tile_mesh()\" not met");
   }
   //camera.size = { 1920.0f, 1080.0f };
   camera.size = AllegroFlare::vec2d(1920.0f, 1080.0f);
   camera.align = AllegroFlare::vec2d(0.5, 0.5);

   //float width = tile_mesh->get_real_width();
   //float height = tile_mesh->get_real_height();

   float room_width = currently_active_map->get_tile_mesh()->get_tile_width() * 25; // tile_mesh->get_real_width();
   float room_height = currently_active_map->get_tile_mesh()->get_tile_height() * 15; //tile_mesh->get_real_height();

   // note that Shovel Knight has 4.5x4.5 sized pixels (actually 4.8 x 4.5)
      // this means:
      //     - a virtual resolution of 400x240
      //     = a native display resolution of 1920x1080 (HD)
      //     - 25 tiles x 15 tiles to fill the virtual resolution
      //     - 16x16 pixel tiles
      // see https://www.yachtclubgames.com/blog/breaking-the-nes

   camera.set_zoom(camera_baseline_zoom);
   //AllegroFlare::vec2d(1.0 / 4.8, 1.0 / 4.5);
   camera.position = {room_width/2, room_height/2};

   return;
}

void Screen::reverse_gravity()
{
   gravity_reversed = !gravity_reversed;
}

void Screen::update_collisions_with_damaging_zones()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> damages_player_entities =
      select_damages_player_entities_on_map(currently_active_map_name);

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> damages_enemies_entities =
      select_damages_enemies_entities_on_map(currently_active_map_name);

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*>
      takes_damage_from_player_damage_zone_entities =
         select_takes_damage_from_player_damage_zone_entities_on_map(currently_active_map_name);



   // NOTE: for now, one player controlled character evaluated:
   KrampusReturns::Entities::Krampus* player_krampus =
      static_cast<KrampusReturns::Entities::Krampus*>(player_controlled_entity);


   // TODO: Implement the body logic of this function for entities damaging enemies

   if (!player_krampus || player_krampus->get_invincible_from_taking_damage())
   {
      // Nothing. Don't test damage on krampus here.
   }
   else
   {
      for (auto &damages_player_entity : damages_player_entities)
      {
         if (player_krampus->get_place_ref().collide(damages_player_entity->get_place_ref()))
         {
            // a collision happened here! woo!
            player_krampus->take_hit(1); // TODO: Fix the ammount of damage delt. It could vary depending on
                                         // the type, or the damage zone.
            break;
         }
      }
   }

   // TODO: Implement the body logic of this function for entities damaging player

   for (auto &entity_that_takes_damage_from_player_damage_zone : takes_damage_from_player_damage_zone_entities)
   {
      for (auto &entity_that_damages_enemies : damages_enemies_entities)
      {
         //std::cout << "=0-0-0-0-0-0- entityt that damages enemies" << std::endl;
         if (entity_that_damages_enemies->get_place_ref().collide(
            entity_that_takes_damage_from_player_damage_zone->get_place_ref()))
         {
            // TODO: account for stunned enemies
            // TODO: account for strength of damage
            // TODO: consider if force of impact should be a thing




            int damage_to_be_delt = 1;

            // HERE: Account for damage caused in damage zone
            auto as_damage_zone = 
               dynamic_cast<KrampusReturns::Entities::DamageZone*>(entity_that_damages_enemies);
            if (as_damage_zone)
            {
               damage_to_be_delt = as_damage_zone->get_damage();
            }
            


            // Known types (at the moment):
            KrampusReturns::Entities::Blob* as_blob =
               dynamic_cast<KrampusReturns::Entities::Blob*>(entity_that_takes_damage_from_player_damage_zone);
            if (as_blob)
            {
               // HERE:
               as_blob->take_damage(damage_to_be_delt);
               if (as_blob->get_health() <= 0) as_blob->set(PLEASE_DELETE);

               emit_spawn_flash_fx_event(as_blob->get_place_ref().position.x, as_blob->get_place_ref().position.y);
               //std::cout << "---- BLOB HIT" << std::endl;
            }
            else if (
               ChatGPT::Enemy* as_chat_gpt_enemy =
                  dynamic_cast<ChatGPT::Enemy*>(entity_that_takes_damage_from_player_damage_zone);
               as_chat_gpt_enemy != nullptr
            )
            {
               as_chat_gpt_enemy->take_damage(damage_to_be_delt);
               if (as_chat_gpt_enemy->get_health() <= 0) as_chat_gpt_enemy->set(PLEASE_DELETE);

               emit_spawn_flash_fx_event(
                  as_chat_gpt_enemy->get_place_ref().position.x,
                  as_chat_gpt_enemy->get_place_ref().position.y
               );
               //std::cout << "-------- ChatGPT Enemy Hit!" << std::endl;
            }


            //type: ChatGPT::Enemy*

            //entity_that_takes_damage_from_player_damage_zone->take_damage(1);
         }
      }
   }


   return;
}

bool Screen::player_controled_entity_in_same_room_as_boss()
{
   AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* boss = find_boss();
   static bool boss_fight_triggered = false;
   if (player_controlled_entity && boss)
   {
      std::pair<int, int> player_room_coords = calc_room_coords(player_controlled_entity);
      std::pair<int, int> boss_room_coords = calc_room_coords(boss);
      bool in_same_room_coords = (player_room_coords == boss_room_coords);
      return in_same_room_coords;
   }
   return false;
}

void Screen::update_entities()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::update_entities]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::update_entities: error: guard \"initialized\" not met");
   }
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::update_entities]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::update_entities: error: guard \"currently_active_map\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   // apply gravity
   for (auto &entity : get_current_map_entities())
   {
      if (entity->exists(NOT_AFFECTED_BY_GRAVITY)) continue;
      AllegroFlare::Placement2D &velocity = entity->get_velocity_ref();
      velocity.position.y += (gravity_reversed ? -gravity : gravity);
   }


   // update targets on "seeker" types
   // TODO: add a better selection mechanism for seeker targets
   if (player_controlled_entity)
   {
      for (auto &entity : select_seekers_on_map_name(currently_active_map_name))
      {
         ChatGPT::Seeker* seeker_movement_strategy = static_cast<ChatGPT::Seeker*>(entity->get_movement_strategy());
         if (seeker_movement_strategy->get_target() == nullptr)
         {
            AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D *thing =
               static_cast<AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D*>(
                  player_controlled_entity
               );
            ChatGPT::Seeker* seeker_movement_strategy = static_cast<ChatGPT::Seeker*>(entity->get_movement_strategy());
            
            seeker_movement_strategy->set_target(thing);
         }

         // HACK:
         // flip the orientation of the bitmap direction
         if (entity->get_velocity_ref().position.x < 0) entity->set_bitmap_flip_h(false);
         else entity->set_bitmap_flip_h(false);
      }
   }



   // update the entities (typically includes movement strategies)
   for (auto &entity : get_current_map_entities())
   {
      entity->update();
   }


   // step each entity
   for (auto &entity : get_current_map_entities())
   {
      AllegroFlare::Placement2D &place = entity->get_place_ref();
      AllegroFlare::Placement2D &velocity = entity->get_velocity_ref();

      // handle case where entity does not interact with world tile mesh
      if (entity->exists(DOES_NOT_COLLIDE_WITH_WORLD))
      {
         place.position.x += velocity.position.x;
         place.position.y += velocity.position.y;
         continue;
      }

      // create a "simulated aabb2d" of the entity and run it through the collision stepper
      AllegroFlare::Physics::AABB2D aabb2d(
         place.position.x - place.size.x * place.align.x,
         place.position.y - place.size.y * place.align.y,
         place.size.x,
         place.size.y,
         velocity.position.x,
         velocity.position.y
      );
      float tile_width = 16.0f;
      float tile_height = 16.0f;
      AllegroFlare::Physics::TileMapCollisionStepper collision_stepper(
         currently_active_map->get_collision_tile_mesh(),
         &aabb2d,
         tile_width,
         tile_height
      );
      collision_stepper.step();

      // supplant our entity's position and velocity values with the "simulated aabb2d"'s values
      place.position.x = aabb2d.get_x() + place.size.x * place.align.x;
      place.position.y = aabb2d.get_y() + place.size.y * place.align.y;
      velocity.position.x = aabb2d.get_velocity_x();
      velocity.position.y = aabb2d.get_velocity_y();

      // add positioning flags
      if (collision_stepper.adjacent_to_bottom_edge(tile_width, tile_height)) entity->set(ADJACENT_TO_FLOOR);
      else entity->remove(ADJACENT_TO_FLOOR);

      if (collision_stepper.adjacent_to_top_edge(tile_width, tile_height)) entity->set(ADJACENT_TO_CEILING);
      else entity->remove(ADJACENT_TO_CEILING);

      if (collision_stepper.adjacent_to_left_edge(tile_width, tile_height)) entity->set(ADJACENT_TO_LEFT_WALL);
      else entity->remove(ADJACENT_TO_LEFT_WALL);

      if (collision_stepper.adjacent_to_right_edge(tile_width, tile_height)) entity->set(ADJACENT_TO_RIGHT_WALL);
      else entity->remove(ADJACENT_TO_RIGHT_WALL);
   }

   // update the collectables
   // TODO: allow this function to run without being coupled with a "player_controlled_entity"
   if (player_controlled_entity) update_player_collisions_with_collectables();

   // update the player colliding on the goalposts
   // TODO: allow this function to run without being coupled with a "player_controlled_entity"
   if (player_controlled_entity) update_player_collisions_with_goalposts();


   // HACK: update is player fighting boss music
   AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* boss = find_boss();
   static bool boss_fight_triggered = false;
   static bool boss_beaten = false; // TODO: <-- make this a member variable, and reset it when loading a level

   if (boss_fight_triggered && !boss && !boss_beaten)
   {
      // omg boss is beaten yay!
      boss_beaten = true;
      event_emitter->emit_stop_all_music_tracks_event();
      //stop_all_music();
      event_emitter->emit_play_sound_effect_event("boss_beaten_chime");
   }
   if (player_controlled_entity && boss)
   {
      std::pair<int, int> player_room_coords = calc_room_coords(player_controlled_entity);
      std::pair<int, int> boss_room_coords = calc_room_coords(boss);
      bool in_same_room_coords = (player_room_coords == boss_room_coords);

      if (!boss_fight_triggered)
      {
         if (in_same_room_coords)
         {
            play_boss_music();
            boss_fight_triggered = true;
         }
      }
      else if (boss_fight_triggered)
      {
         if (!in_same_room_coords)
         {
            play_level_music();
            boss_fight_triggered = false;
         }
      }
   }


   update_collisions_with_damaging_zones();


   //update_player_collisions_with_damage_zones();


   // update the player colliding on the doors
   //check_player_collisions_with_doors(); // this is now done by pressing 'UP' when over a door

   // mark all the ephemeral entities for deltion
   mark_all_ephemeral_entities_for_deletion();

   // delete entities flagged to be deleted
   cleanup_entities_flagged_for_deletion();

   // update camera
   if (camera_control_strategy) camera_control_strategy->update();

   return;
}

void Screen::mark_all_ephemeral_entities_for_deletion()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   for (int i=0; i<entity_pool.size(); i++)
   {
      if (entity_pool[i]->exists(EPHEMERAL)) entity_pool[i]->set(PLEASE_DELETE);
   }
   return;
}

void Screen::cleanup_entities_flagged_for_deletion()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   for (int i=0; i<entity_pool.size(); i++)
   {
      if (entity_pool[i]->exists(PLEASE_DELETE))
      {
         std::cout << "NOTICE: deleting entity." << std::endl;
         delete entity_pool[i];
         entity_pool.erase(entity_pool.begin() + i);
         i--;
      }
   }
   return;
}

void Screen::flag_all_entities_for_deletion()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   for (int i=0; i<entity_pool.size(); i++)
   {
      entity_pool[i]->set(PLEASE_DELETE);
   }
   return;
}

void Screen::check_player_collisions_with_doors()
{
   if (!(player_controlled_entity))
   {
      std::stringstream error_message;
      error_message << "[Screen::check_player_collisions_with_doors]: error: guard \"player_controlled_entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::check_player_collisions_with_doors: error: guard \"player_controlled_entity\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::check_player_collisions_with_doors]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::check_player_collisions_with_doors: error: guard \"event_emitter\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> _entities = get_current_map_entities();
   AllegroFlare::Prototypes::Platforming2D::EntityCollectionHelper collection_helper(&_entities);

   // NOTE: collisions here occcur with the origin of the player character (bottom center of the sprite)
   float player_x = player_controlled_entity->get_place_ref().position.x;
   float player_y = player_controlled_entity->get_place_ref().position.y;

   for (auto &entity : collection_helper.select_doors())
   {
      if (entity->get_place_ref().collide(player_x, player_y, 4, 4, 4, 4))
      {
         AllegroFlare::Prototypes::Platforming2D::Entities::Doors::Basic2D *door =
            static_cast<AllegroFlare::Prototypes::Platforming2D::Entities::Doors::Basic2D*>(entity);

         std::string game_event_name_to_emit = door->get_game_event_name_to_emit();
         bool this_door_emits_game_event = !game_event_name_to_emit.empty();
         if (this_door_emits_game_event)
         {
            event_emitter->emit_game_event(AllegroFlare::GameEvent(game_event_name_to_emit));
         }
         else // will door travel to another map or place on the current map
         {
            std::string map_target_name = door->get_target_map_name();
            float target_spawn_x = door->get_target_spawn_x();
            float target_spawn_y = door->get_target_spawn_y();

            // find the target map
            AllegroFlare::Prototypes::Platforming2D::Entities::TileMaps::Basic2D* target_map =
               find_map_by_name(map_target_name);

            // reposition player in map
            player_controlled_entity->set(ON_MAP_NAME, map_target_name);
            player_controlled_entity->get_place_ref().position.x = target_spawn_x;
            player_controlled_entity->get_place_ref().position.y = target_spawn_y;

            // set current map
            set_currently_active_map(map_target_name);
         }
         
         return;
      }
   }
   return;
}

void Screen::update_player_collisions_with_collectables()
{
   if (!(player_controlled_entity))
   {
      std::stringstream error_message;
      error_message << "[Screen::update_player_collisions_with_collectables]: error: guard \"player_controlled_entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::update_player_collisions_with_collectables: error: guard \"player_controlled_entity\" not met");
   }
   // TODO: allow this function to run without being coupled with a "player_controlled_entity"
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   //std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> _entities = get_current_map_entities();
   //AllegroFlare::Prototypes::Platforming2D::EntityCollectionHelper collection_helper(&_entities);
   KrampusReturns::Entities::Krampus* player_krampus =
      dynamic_cast<KrampusReturns::Entities::Krampus*>(player_controlled_entity);
   if (!player_krampus) return;

   // NOTE: collisions here occcur with the origin of the player character (bottom center of the sprite)
   float player_x = player_controlled_entity->get_place_ref().position.x;
   float player_y = player_controlled_entity->get_place_ref().position.y;

   for (auto &entity : select_collectable_by_player_on_map(currently_active_map_name))
   {
      if (entity->get_place_ref().collide(player_x, player_y, 4, 4, 4, 4))
      {
         std::string collectable_type = "unknown";
         if (entity->exists("collectable_type")) collectable_type = entity->get("collectable_type");

         bool collected = false;
         // HERE
         // TODO: add different items for powerups
         if (collectable_type == "attack_up")
         {
            player_krampus->increment_attack_strength();
            // TODO: play power-up sound effect
            collected = true;
         }
         //if (collectable_type == "boobaz")
         //{
            //collected = true;
         //}

         // TODO: make collectable sound effect
         if (collected)
         {
            entity->set(PLEASE_DELETE);
            event_emitter->emit_play_sound_effect_event("pickup_powerup");
         }
         // NOTE: typically will do something here as a result of picking up the item
      }
   }
   return;
}

void Screen::dump_entity_data()
{
   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> _entities = get_current_map_entities();
   AllegroFlare::Prototypes::Platforming2D::EntityCollectionHelper collection_helper(&_entities);

   std::cout << "entities:" << std::endl;
   for (auto &entity : _entities)
   {
      std::map<std::string, std::string> attributes_copy = entity->Attributes::get_copy();
      std::cout << "  - [entity]" << std::endl;
      for (auto &attribute : attributes_copy)
      {
         std::cout << "    " << attribute.first << ": " << attribute.second << std::endl;
      }
   }
   return;
}

void Screen::update_player_collisions_with_goalposts()
{
   if (!(player_controlled_entity))
   {
      std::stringstream error_message;
      error_message << "[Screen::update_player_collisions_with_goalposts]: error: guard \"player_controlled_entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::update_player_collisions_with_goalposts: error: guard \"player_controlled_entity\" not met");
   }
   // TODO: allow this function to run without being coupled with a "player_controlled_entity"
   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> _entities = get_current_map_entities();
   AllegroFlare::Prototypes::Platforming2D::EntityCollectionHelper collection_helper(&_entities);

   KrampusReturns::Entities::Krampus* player_krampus =
      static_cast<KrampusReturns::Entities::Krampus*>(player_controlled_entity);

   for (auto &entity : collection_helper.select_goalposts())
   {
      if (player_krampus->get_place_ref().collide(entity->get_place_ref()))
      {
         int goalpost_id = entity->exists("goalpost_id") ? entity->get_as_int("goalpost_id") : -1;
         event_emitter->emit_game_event(
            AllegroFlare::GameEvent(
               "goalpost_reached",
               new KrampusReturns::GameEventDatas::GoalpostReached(goalpost_id)
            )
         );
      }
   }
   return;
}

void Screen::draw_entities()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   // HACK: ok this next line is embarrasing. It's needed because apparently "draw_entities" is 
   // potentially being called before init?  And the animation book is not initialized. Oh well.
   //if (!animation_book_initialized) initialize_animation_book();

   //ALLEGRO_BITMAP *little_shadow_bitmap = nullptr;
   //little_shadow_bitmap = animation_book.get_still_frame("fly");
   //ALLEGRO_BITMAP *little_shadow_bitmap = nullptr;
   //if (little_shadow_bitmap)
   {
      //int little_shadow_bitmap_width = al_get_bitmap_width(little_shadow_bitmap);
      //int little_shadow_bitmap_height = al_get_bitmap_height(little_shadow_bitmap);
      for (auto &entity : get_current_map_entities_y_sorted())
      {
         if (entity->exists("noshadow")) continue;
         if (entity->exists(INVISIBLE)) continue;

         {
            float x = entity->get_place_ref().position.x; // - little_shadow_bitmap_width / 2;
            float y = entity->get_place_ref().position.y; // - little_shadow_bitmap_height / 2;
            float w = entity->get_place_ref().size.x; // - little_shadow_bitmap_height / 2;
            //int hw = 

            al_draw_filled_ellipse(x, y, 4 + w/5, 2, ALLEGRO_COLOR{0, 0, 0, 0.3});

            //al_draw_scaled_bitmap(
               //little_shadow_bitmap,
               //0,
               //0,
               //little_shadow_bitmap_width,
               //little_shadow_bitmap_height,
               //x,
               //y,
               //little_shadow_bitmap_width,
               //little_shadow_bitmap_height,
               //0
            //);
         }
         //entity->get_place_ref().start_transform();
         //if (al_draw_scaled_bitmap(
         //entity->get_place_ref().restore_transform();
      }
   }

   for (auto &entity : get_current_map_entities_y_sorted())
   {
      if (!entity->exists(INVISIBLE))
      {
         entity->draw();
      }
   }
   return;
}

void Screen::update()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::update]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::update: error: guard \"initialized\" not met");
   }
   float time_now = al_get_time();
   update_state(time_now);
   return;
}

void Screen::draw()
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::draw]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::draw: error: guard \"initialized\" not met");
   }
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::draw]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::draw: error: guard \"currently_active_map\" not met");
   }
   if (!(get_tile_mesh()))
   {
      std::stringstream error_message;
      error_message << "[Screen::draw]: error: guard \"get_tile_mesh()\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::draw: error: guard \"get_tile_mesh()\" not met");
   }
   al_clear_to_color(al_color_html("291d29")); // TODO: this double-clears the background color since
                                               // framework does it already

   camera.setup_dimentional_projection(backbuffer_sub_bitmap);

   ALLEGRO_STATE previous_target_bitmap;
   al_store_state(&previous_target_bitmap, ALLEGRO_STATE_TARGET_BITMAP);
   al_set_target_bitmap(backbuffer_sub_bitmap);
   camera.start_reverse_transform();

   al_set_render_state(ALLEGRO_DEPTH_FUNCTION, ALLEGRO_RENDER_LESS_EQUAL); // less or equal allows 
                                                                           // subsequent renders at the same
                                                                           // z-level to overwrite. This 
                                                                           // mimics the rendering of the
                                                                           // typical drawing functions when using
                                                                           // 2d. The reason this exists is so that
                                                                           // the players are drawn on top of the
                                                                           // tile mesh.
   if (shader) shader->activate();
   if (show_tile_mesh) get_tile_mesh()->render();
   draw_entities();
   if (show_collision_tile_mesh) render_collision_tile_mesh();
   if (shader) shader->deactivate();

   camera.restore_transform();
   al_restore_state(&previous_target_bitmap);

   // draw the hud

   draw_hud();


   return;
}

void Screen::set_full_color_overlay(ALLEGRO_COLOR base_color, float opacity)
{
   full_color_overlay_color = base_color; //ALLEGRO_COLOR{1.0, 0.0, 0.0, 1.0};
   full_color_overlay_opacity = 0.2;
   return;
}

void Screen::show_full_color_overlay()
{
   showing_full_color_overlay = true;
   return;
}

void Screen::hide_full_color_overlay()
{
   showing_full_color_overlay = false;
   return;
}

void Screen::set_banner_text(std::string text, ALLEGRO_COLOR base_color)
{
   banner_text_color = base_color;
   banner_text = text;
   return;
}

void Screen::show_banner_text()
{
   showing_banner_text = true;
   return;
}

void Screen::hide_banner_text()
{
   showing_banner_text = false;
   return;
}

void Screen::set_banner_subtext(std::string text, ALLEGRO_COLOR base_color_a, ALLEGRO_COLOR base_color_b, float pulse_rate_per_sec)
{
   banner_subtext = text;
   banner_subtext_color_a = base_color_a;
   banner_subtext_color_b = base_color_b;
   banner_subtext_pulse_rate_per_sec = pulse_rate_per_sec;
   return;
}

void Screen::show_banner_subtext()
{
   showing_banner_subtext = true;
   banner_subtext_shown_at = al_get_time();
   return;
}

void Screen::hide_banner_subtext()
{
   showing_banner_subtext = false;
   return;
}

void Screen::draw_hud()
{
   if (!(font_bin))
   {
      std::stringstream error_message;
      error_message << "[Screen::draw_hud]: error: guard \"font_bin\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::draw_hud: error: guard \"font_bin\" not met");
   }
   static float showing_boss_meter_counter = 0.0f;

   if (!player_controlled_entity) return;


   // draw full color overlay

   if (showing_full_color_overlay)
   {
      ALLEGRO_COLOR calced_full_color_overlay_color = ALLEGRO_COLOR{
         full_color_overlay_color.r * full_color_overlay_opacity,
         full_color_overlay_color.g * full_color_overlay_opacity,
         full_color_overlay_color.b * full_color_overlay_opacity,
         full_color_overlay_color.a * full_color_overlay_opacity,
      };

      al_draw_filled_rectangle(0, 0, 1920, 1080, calced_full_color_overlay_color);
   }



   if (state == STATE_PLAYING_IN_LEVEL)
   {
      // draw hud elements (hearts, etc.)

      // TODO: NOTE: here player_controlled_entity is assumed to be krampus
      // WARNING: TESTING:
      KrampusReturns::Entities::Krampus* player_krampus =
         static_cast<KrampusReturns::Entities::Krampus*>(player_controlled_entity);


      // draw player hearts
      AllegroFlare::Placement2D heart_placement;
      heart_placement.position = {80, 40};
      heart_placement.start_transform();
         AllegroFlare::Elements::HealthBars::Hearts hearts(
            font_bin,
            player_krampus->get_max_health(),
            player_krampus->get_health()
         );
         hearts.set_heart_size(48);
         hearts.set_heart_spacing(48+6);
         hearts.render();
      heart_placement.restore_transform();


      float width_of_health_bar = 360; // <-- NOTE: may want to improve this

      // draw player strength bar
      //AllegroFlare::Placement2D strength_bar_placement;
      int player_max_strength = player_krampus->get_max_attack_strength();
      int player_strength = player_krampus->get_attack_strength();
      AllegroFlare::Elements::HealthBars::Classic player_strength_bar(player_max_strength, player_strength);
      player_strength_bar.set_bar_width(34);
      player_strength_bar.set_bar_spacing(44);
      player_strength_bar.set_bar_height(22);
      player_strength_bar.set_fill_color(al_color_name("dodgerblue"));
      player_strength_bar.set_empty_box_outline_color(al_color_name("midnightblue"));
      player_strength_bar.set_bar_stroke_thickness(4.0f);
      player_strength_bar.fit_placement_width_and_height();
      player_strength_bar.get_placement_ref().position = { 80 + width_of_health_bar, 60 };
      player_strength_bar.render();



      //AllegroFlare::Placement2D strength_bar_placement;
      //heart_placement.position = {80 + 100, 40};
      //heart_placement.start_transform();
         //AllegroFlare::Elements::HealthBars::Classic strength_bar(
            ////font_bin,
            //player_krampus->get_max_health(),
            //player_krampus->get_health()
         //);
         //hearts.set_heart_size(48);
         //hearts.set_heart_spacing(48+6);
         //hearts.render();
      //heart_placement.restore_transform();



      bool player_fighting_boss = player_controled_entity_in_same_room_as_boss();
      if (player_fighting_boss)
      {
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* boss = find_boss();
         ChatGPT::Enemy *enemy = dynamic_cast<ChatGPT::Enemy*>(boss);
         if (!enemy)
         {
            AllegroFlare::Errors::throw_error("KrampusReturns::Gameplay::Screen::draw_hud", "Unexpectedly, oss is a "
                                              "nullptr, but somehow we got inside logic that expects it not to be.");
         }

         int boss_max_health = enemy->get_max_health();
         int boss_health = enemy->get_health();
         AllegroFlare::Elements::HealthBars::Classic boss_health_bar(boss_max_health, boss_health);
         boss_health_bar.set_bar_width(32);
         boss_health_bar.set_bar_spacing(40);
         boss_health_bar.set_bar_height(18);
         boss_health_bar.fit_placement_width_and_height();
         boss_health_bar.get_placement_ref().position = { 1920 * 0.5, (float)1080 - 72 };


         ALLEGRO_FONT *boss_font = font_bin->auto_get("Merriweather-Bold.ttf -46");
         std::string boss_name = "Skull Head";
         al_draw_text(
            boss_font,
            ALLEGRO_COLOR{1, 1, 1, 1},
            1920 * 0.5,
            boss_health_bar.get_placement_ref().position.y - al_get_font_line_height(boss_font) * 1.6,
            ALLEGRO_ALIGN_CENTER,
            boss_name.c_str()
         );

         //AllegroFlare::Placement2D boss_health_meter_placement;

         //boss_health_meter_placement.position = { 1920 * 0.5, (float)1080 - 40 };
         //boss_health_meter_placement.align = { 0.5, 0.5 };
         // TODO: boss_health_meter_placement.size = { ?, ? };

         //boss_health_meter_placement.start_transform();
         boss_health_bar.render();
         //boss_health_meter_placement.restore_transform();
      }
   }



   // draw some state overlay

   if (showing_banner_text)
   {
      ALLEGRO_FONT *font = obtain_banner_text_font();
      //std::string banner_text = "YOU LOSE";
      al_draw_text(font, banner_text_color, 1920/2, 1080/2-100, ALLEGRO_ALIGN_CENTER, banner_text.c_str());

      // draw the frame around the screen indicating the banner
      float inset = 80;
      float thickness = 20;
      al_draw_rectangle(inset, inset, 1920-inset, 1080-inset, banner_text_color, thickness);
   }

   if (showing_banner_subtext)
   {
      float time_now = al_get_time();
      float banner_age = time_now - banner_subtext_shown_at;
      float pulse = sin(banner_age*banner_subtext_pulse_rate_per_sec) * 0.5 + 0.5;
      ALLEGRO_COLOR final_color = AllegroFlare::color::mix(
         banner_subtext_color_a,
         banner_subtext_color_b,
         pulse
      );
      ALLEGRO_FONT *font = obtain_banner_subtext_font();
      al_draw_text(font, final_color, 1920/2, 1080/2+100, ALLEGRO_ALIGN_CENTER, banner_subtext.c_str());
   }



   return;
}

void Screen::toggle_show_collision_tile_mesh()
{
   show_collision_tile_mesh = !show_collision_tile_mesh;
   return;
}

void Screen::toggle_show_tile_mesh()
{
   show_tile_mesh = !show_tile_mesh;
   return;
}

void Screen::primary_timer_func()
{
   update();
   draw();
   return;
}

void Screen::shake_camera(float intensity, float duration, float time_now)
{
   using namespace KrampusReturns::CameraControlStrategies2D;
   if (camera_control_strategy->is_type(SmoothSnapWithFX::TYPE))
   {
      SmoothSnapWithFX *as_smooth_snap_with_fx = static_cast<SmoothSnapWithFX*>(camera_control_strategy);
      as_smooth_snap_with_fx->start_impact_shake(intensity, duration, time_now);
   }
   else
   {
      // TODO: THROW on unshakable camera type
   }

   return;
}

void Screen::spawn_flash_effect(std::string type_str, float x, float y)
{
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::spawn_flash_effect]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::spawn_flash_effect: error: guard \"currently_active_map\" not met");
   }
   KrampusReturns::EntityFactory entity_factory;
      entity_factory.set_animation_book(&animation_book);

   KrampusReturns::Entities::FlashEffect *flash_effect =
      entity_factory.create_flash_fx1(currently_active_map_name, x, y);


   add_entity_to_pool(flash_effect);

   return;
}

void Screen::emit_spawn_flash_fx_event(float x, float y, std::string type_str)
{
   event_emitter->emit_game_event(
      AllegroFlare::GameEvent(
         "spawn_flash_effect", 
         new KrampusReturns::GameEventDatas::SpawnFlashFX(type_str, x, y)
      )
   );
   return;
}

void Screen::create_damage_zone_by_player(std::string on_map, float point_of_impact_x, float point_of_impact_y, float impact_width, float impact_height, int damage, uint32_t direction_of_force)
{
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::create_damage_zone_by_player]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::create_damage_zone_by_player: error: guard \"currently_active_map\" not met");
   }
   KrampusReturns::EntityFactory entity_factory;
      entity_factory.set_animation_book(&animation_book);

   // DEBUG:

   KrampusReturns::Entities::DamageZone *damage_zone =
      entity_factory.create_damage_zone_by_player(
         on_map,
         point_of_impact_x,
         point_of_impact_y,
         impact_width,
         impact_height,
         damage,
         direction_of_force
      );

   add_entity_to_pool(damage_zone);

   return;
}

void Screen::game_event_func(AllegroFlare::GameEvent* ev)
{
   if (!(ev))
   {
      std::stringstream error_message;
      error_message << "[Screen::game_event_func]: error: guard \"ev\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::game_event_func: error: guard \"ev\" not met");
   }
   if (!(event_emitter))
   {
      std::stringstream error_message;
      error_message << "[Screen::game_event_func]: error: guard \"event_emitter\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::game_event_func: error: guard \"event_emitter\" not met");
   }
   std::string event_name = ev->get_type();
   float time_now = al_get_time();
   //std::cout << "--- Gameplay/Screen --- EVENT RECEIVED: \"" << event_name << "\"" << std::endl;

   std::map<std::string, std::function<void()>> event_map = {
      { "retry_level", [this, time_now](){
          load_level_and_start(&current_level_data);
          //shake_camera(3, 0.5, time_now);
      }},
      { "camera_shake_bump", [this, time_now](){
          shake_camera(3, 0.5, time_now);
      }},
      { "player_died", [this, time_now](){
          set_state(STATE_PLAYER_DIED, time_now);
      }},
      { "goalpost_reached", [this, time_now](){
          set_state(STATE_FINISHED_LEVEL, time_now);
      }},
      { "spawn_damage_zone_by_player", [this, ev, time_now](){
          if (!ev->get_data())
          {
             throw std::runtime_error("Gameplay::Screen::game_event_func on spawn_damage_zone_by_player: ERROR: "
                                      "no ev->data");
          }
          else
          {
             if (!ev->get_data()->is_type(KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer::TYPE))
             {
                throw std::runtime_error("Gameplay::Screen::game_event_func on spawn_damage_zone_by_player: "
                                         "ERROR: game even data not of expected type");
             }
             KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer *as_damage_zone_by_player =
                static_cast<KrampusReturns::GameEventDatas::SpawnDamageZoneByPlayer*>(ev->get_data());
            create_damage_zone_by_player(
               currently_active_map_name, // NOTE: this could eventually be a bug. The map where the 
                                          // impact occurs should be transmitted from the time of impact.
                                          // Delayed impacts occurred between door changes could spawn in 
                                          // the wrong room, for example... I think...
               as_damage_zone_by_player->get_point_of_impact_x(),
               as_damage_zone_by_player->get_point_of_impact_y(),
               as_damage_zone_by_player->get_impact_width(),
               as_damage_zone_by_player->get_impact_height(),
               as_damage_zone_by_player->get_damage(),
               as_damage_zone_by_player->get_direction_of_force()
            );
          }
      }},
      { "spawn_flash_effect", [this, ev, time_now](){
          if (!ev->get_data())
          {
             throw std::runtime_error("Gameplay::Screen::game_event_func on spawn_flash_effect: ERROR: no ev->data");
          }
          else
          {
             if (!ev->get_data()->is_type(KrampusReturns::GameEventDatas::SpawnFlashFX::TYPE))
             {
                throw std::runtime_error("Gameplay::Screen::game_event_func on spawn_flash_effect: "
                                         "ERROR: not of expected type");
             }
             KrampusReturns::GameEventDatas::SpawnFlashFX *as_spawn_flash_fx =
                static_cast<KrampusReturns::GameEventDatas::SpawnFlashFX*>(ev->get_data());
             spawn_flash_effect(
               as_spawn_flash_fx->get_animation_name(),
               as_spawn_flash_fx->get_x(),
               as_spawn_flash_fx->get_y()
             );
          }
      }},
   };

   // locate and call the function to handle the event 
   if (event_map.count(event_name) == 0)
   {
      // event not found
      //std::cout << "ERROR: event not found: \"" << event_name << "\"" << std::endl;
   }
   else
   {
      // call the event
      event_map[event_name]();
   }

   return;
}

void Screen::key_char_func(ALLEGRO_EVENT* event)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_char_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_char_func: error: guard \"initialized\" not met");
   }
   if (!(event))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_char_func]: error: guard \"event\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_char_func: error: guard \"event\" not met");
   }
   // TODO: Consider if this should be removed in favor of explicit key_*_func functions.  In some cases
   // this function should remain in place so that the keyboard could be used for debugging control.
   switch (event->keyboard.keycode)
   {
      case ALLEGRO_KEY_W:
      //set_state(STATE_FINISHED_LEVEL);
      //toggle_show_collision_tile_mesh();
      break;

      case ALLEGRO_KEY_1:
      {
         // TODO: CRITICAL: remove this hard-coded base folder
         //static std::string _TEST_BASE_FOLDER = "/Users/markoates/Repos/KrampusReturns/bin/programs/data/";
         //KrampusReturns::Level level(
            //"level_1",
            //"The First Rescue",
            //{
               //{ "map_a", _TEST_BASE_FOLDER + "maps/krampus-returns-level-1-1-0x.tmj" }
            //},
            //"level_1_music"
         //);

         // TESTING DEBUG:
         //load_level_and_start(&level);

         break;
      }

      case ALLEGRO_KEY_EQUALS: // aka PLUS
      {
         //KrampusReturns::Entities::Krampus* player_krampus =
           //static_cast<KrampusReturns::Entities::Krampus*>(player_controlled_entity);

         // TESTING DEBUG:
         //if (player_krampus) player_krampus->increment_attack_strength();
         //break;
      }

      case ALLEGRO_KEY_MINUS: // aka MINUS
      {
         //KrampusReturns::Entities::Krampus* player_krampus =
           //static_cast<KrampusReturns::Entities::Krampus*>(player_controlled_entity);

         // TESTING DEBUG:
         //if (player_krampus) player_krampus->decrement_attack_strength();
         //break;
      }

      case ALLEGRO_KEY_2:
      {
         // TODO: CRITICAL: remove this hard-coded base folder
         //static std::string _TEST_BASE_FOLDER = "/Users/markoates/Repos/KrampusReturns/bin/programs/data/";
         //KrampusReturns::Level level(
            //"level_2",
            //"The Second Rescue",
            //{
               //{ "map_a", _TEST_BASE_FOLDER + "maps/krampus-returns-level-2-1-0x.tmj" }
            //},
            //"level_1_music"
         //);

         // TESTING DEBUG
         //load_level_and_start(&level);

         break;
      }

      case ALLEGRO_KEY_S:
         // TESTING DEBUG
         //event_emitter->emit_game_event(
            //AllegroFlare::GameEvent(
               //"spawn_flash_effect", 
               //new KrampusReturns::GameEventDatas::SpawnFlashFX("flash_fx1", 200, 200)
            //)
         //);
         //toggle_show_collision_tile_mesh();
      break;

      default:
      break;
   }
   return;
}

void Screen::key_up_func(ALLEGRO_EVENT* event)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_up_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_up_func: error: guard \"initialized\" not met");
   }
   if (!(event))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_up_func]: error: guard \"event\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_up_func: error: guard \"event\" not met");
   }
   // TODO: move this to a virtual input func rather than an explicit "key_up_func"
   krampus_controller.key_up_func(event->keyboard.keycode, false);
   return;
}

void Screen::key_down_func(ALLEGRO_EVENT* event)
{
   if (!(initialized))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_down_func]: error: guard \"initialized\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_down_func: error: guard \"initialized\" not met");
   }
   if (!(event))
   {
      std::stringstream error_message;
      error_message << "[Screen::key_down_func]: error: guard \"event\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::key_down_func: error: guard \"event\" not met");
   }
   // TODO: implement here
   if (state == STATE_WAITING_KEYPRESS_TO_RETRY_LEVEL)
   {
      event_emitter->emit_game_event(AllegroFlare::GameEvent("retry_level"));
      return;
   }
   else if (state == STATE_WAITING_KEYPRESS_TO_FINISH_LEVEL)
   {
      event_emitter->emit_game_event(AllegroFlare::GameEvent("finish_level_successfully"));
      return;
   }


   // TODO: move this to a virtual input func rather than an explicit "key_down_func"
   krampus_controller.key_down_func(event->keyboard.keycode, false);
   return;
}

void Screen::user_event_func(ALLEGRO_EVENT* event)
{
   //switch(event->type)
   //{
      //case ALLEGRO_FLARE_EVENT_VIRTUAL_CONTROL_BUTTON_UP:
        //virtual_control_button_up_func(event);
      //break;

      //case ALLEGRO_FLARE_EVENT_VIRTUAL_CONTROL_BUTTON_DOWN:
        //virtual_control_button_down_func(event);
      //break;

      //case ALLEGRO_FLARE_EVENT_VIRTUAL_CONTROL_AXIS_CHANGE:
        //virtual_control_axis_change_func(event);
      //break;
   //}

   return;
}

void Screen::render_collision_tile_mesh()
{
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::render_collision_tile_mesh]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::render_collision_tile_mesh: error: guard \"currently_active_map\" not met");
   }
   AllegroFlare::TileMaps::TileMap<int> *tile_map = currently_active_map->get_collision_tile_mesh();
   float tile_width=16.0f;
   float tile_height=16.0f;

   if (!al_is_primitives_addon_initialized()) throw std::runtime_error("render_tile_map: primitives must be init");

   for (int y=0; y<tile_map->get_num_rows(); y++)
      for (int x=0; x<tile_map->get_num_columns(); x++)
      {
         int tile_type = tile_map->get_tile(x, y);
         switch(tile_type)
         {
            case 0:
              al_draw_rectangle(x * tile_width, y * tile_height, (x+1) * tile_width, (y+1) * tile_height, 
                 ALLEGRO_COLOR{0.2, 0.2, 0.21, 0.21}, 1.0);
            break;

            case 1:
              al_draw_filled_rectangle(x * tile_width, y * tile_height, (x+1) * tile_width, (y+1) * tile_height, 
                 ALLEGRO_COLOR{0.65, 0.62, 0.6, 1.0});
            break;

            default:
              al_draw_filled_rectangle(x * tile_width, y * tile_height, (x+1) * tile_width, (y+1) * tile_height, 
                 ALLEGRO_COLOR{0.8, 0.32, 0.4, 1.0});
            break;
         }
      }
   return;
}

AllegroFlare::TileMaps::PrimMeshAtlas* Screen::get_tile_atlas()
{
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::get_tile_atlas]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::get_tile_atlas: error: guard \"currently_active_map\" not met");
   }
   return currently_active_map->get_tile_atlas();
}

AllegroFlare::TileMaps::PrimMesh* Screen::get_tile_mesh()
{
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::get_tile_mesh]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::get_tile_mesh: error: guard \"currently_active_map\" not met");
   }
   return currently_active_map->get_tile_mesh();
}

AllegroFlare::TileMaps::TileMap<int>* Screen::get_collision_tile_mesh()
{
   if (!(currently_active_map))
   {
      std::stringstream error_message;
      error_message << "[Screen::get_collision_tile_mesh]: error: guard \"currently_active_map\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("Screen::get_collision_tile_mesh: error: guard \"currently_active_map\" not met");
   }
   return currently_active_map->get_collision_tile_mesh();
}

std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> Screen::get_current_map_entities()
{
   AllegroFlare::Prototypes::Platforming2D::EntityCollectionHelper collection_helper(&entity_pool);
   std::string on_map_name = currently_active_map_name;
   return collection_helper.select_on_map(on_map_name);
}

std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> Screen::get_current_map_entities_y_sorted()
{
   AllegroFlare::Prototypes::Platforming2D::EntityCollectionHelper collection_helper(&entity_pool);
   std::string on_map_name = currently_active_map_name;
   return collection_helper.select_on_map_y_sorted(on_map_name);
}

std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> Screen::select_spawn_points_on_map_name(std::string map_name)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   std::string on_map_name = map_name;

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> result;
   for (auto &entity : entity_pool)
   {
      if (!entity->exists(TYPE, "spawn_point")) continue;
      if (!entity->exists(ON_MAP_NAME, on_map_name)) continue;
      result.push_back(entity);
   }
   return result;
}

std::vector<ChatGPT::Enemy*> Screen::select_seekers_on_map_name(std::string map_name)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   std::string on_map_name = map_name;

   std::vector<ChatGPT::Enemy*> result;
   for (auto &entity : entity_pool)
   {
      if (!entity->exists("seeker")) continue;
      if (!entity->exists(ON_MAP_NAME, on_map_name)) continue;
      result.push_back(static_cast<ChatGPT::Enemy*>(entity));
   }
   return result;
}

std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> Screen::select_damages_player_entities_on_map(std::string on_map_name)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> result;
   for (auto &entity : entity_pool)
   {
      if (!entity->exists("damages_player")) continue;
      if (!entity->exists(ON_MAP_NAME, on_map_name)) continue;
      result.push_back(entity);
   }
   return result;
}

std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> Screen::select_takes_damage_from_player_damage_zone_entities_on_map(std::string on_map_name)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> result;
   for (auto &entity : entity_pool)
   {
      if (!entity->exists("takes_damage_from_player_damage_zones")) continue;
      if (!entity->exists(ON_MAP_NAME, on_map_name)) continue;
      result.push_back(entity);
   }
   return result;
}

std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> Screen::select_damages_enemies_entities_on_map(std::string on_map_name)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> result;
   for (auto &entity : entity_pool)
   {
      if (!entity->exists("damages_enemies")) continue;
      if (!entity->exists(ON_MAP_NAME, on_map_name)) continue;
      result.push_back(entity);
   }
   return result;
}

std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> Screen::select_collectable_by_player_on_map(std::string on_map_name)
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> result;
   for (auto &entity : entity_pool)
   {
      if (!entity->exists(COLLECTABLE_BY_PLAYER)) continue;
      if (!entity->exists(ON_MAP_NAME, on_map_name)) continue;
      result.push_back(entity);
   }
   return result;
}

bool Screen::are_any_blobs_present()
{
   //using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   //std::string on_map_name = map_name;

   //std::vector<ChatGPT::Enemy*> result;
   for (auto &entity : entity_pool)
   {
      if (entity->exists("type", "blob")) return true;
   }
   return false;
}

AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* Screen::find_boss()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   //std::string on_map_name = map_name;

   //std::vector<ChatGPT::Enemy*> result;
   for (auto &entity : entity_pool)
   {
      if (entity->exists("is_boss")) return entity;
   }
   return nullptr;
}

int Screen::count_num_spawn_points_in_all_maps()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   int result = 0;
   for (auto &entity : entity_pool)
   {
      if (entity->exists(TYPE, "spawn_point")) result++;
   }
   return result;
}

AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* Screen::find_first_spawn_point_on_any_map()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;
   for (auto &entity : entity_pool)
   {
      if (entity->exists(TYPE, "spawn_point")) return entity;
   }
   return nullptr;
}

ALLEGRO_FONT* Screen::obtain_banner_text_font()
{
   return font_bin->auto_get("ChronoType.otf -200");
}

ALLEGRO_FONT* Screen::obtain_banner_subtext_font()
{
   return font_bin->auto_get("ChronoType.otf -68");
}


} // namespace Gameplay
} // namespace KrampusReturns


