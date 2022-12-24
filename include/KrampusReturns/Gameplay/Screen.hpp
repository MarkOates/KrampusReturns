#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/CameraControlStrategies2D/Base.hpp>
#include <AllegroFlare/Display.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <AllegroFlare/FrameAnimation/Book.hpp>
#include <AllegroFlare/GameEvent.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2D.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/TileMaps/Basic2D.hpp>
#include <AllegroFlare/Screens/Base.hpp>
#include <AllegroFlare/Shader.hpp>
#include <AllegroFlare/TileMaps/PrimMesh.hpp>
#include <AllegroFlare/TileMaps/PrimMeshAtlas.hpp>
#include <AllegroFlare/TileMaps/TileMap.hpp>
#include <AllegroFlare/Vec2D.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <KrampusReturns/KrampusController.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <cstdint>
#include <map>
#include <string>
#include <vector>


namespace KrampusReturns
{
   namespace Gameplay
   {
      class Screen : public AllegroFlare::Screens::Base
      {
      public:
         static constexpr uint32_t STATE_UNDEF = 0;
         static constexpr uint32_t STATE_PRELOADING_LEVEL = 1;
         static constexpr uint32_t STATE_PLAYING_IN_LEVEL = 2;
         static constexpr uint32_t STATE_PLAYER_DIED = 3;
         static constexpr uint32_t STATE_FINISHED_LEVEL = 4;
         static constexpr uint32_t STATE_WAITING_KEYPRESS_TO_RETRY_LEVEL = 5;
         static constexpr uint32_t STATE_WAITING_KEYPRESS_TO_FINISH_LEVEL = 6;
         static constexpr uint32_t STATE_DOING_SHUTDOWN = 7;
         static constexpr uint32_t STATE_SHUTDOWN = 8;

      private:
         AllegroFlare::BitmapBin* bitmap_bin;
         AllegroFlare::FontBin* font_bin;
         AllegroFlare::Display* display;
         AllegroFlare::EventEmitter* event_emitter;
         AllegroFlare::FrameAnimation::Book animation_book;
         bool animation_book_initialized;
         int native_display_resolution_width;
         int native_display_resolution_height;
         bool initialized;
         bool destroyed;
         AllegroFlare::Prototypes::Platforming2D::Entities::TileMaps::Basic2D* currently_active_map;
         std::string currently_active_map_name;
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> entity_pool;
         std::map<std::string, std::string> map_dictionary;
         float gravity;
         bool gravity_reversed;
         AllegroFlare::Camera2D camera;
         AllegroFlare::Vec2D camera_baseline_zoom;
         KrampusReturns::KrampusController krampus_controller;
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* player_controlled_entity;
         AllegroFlare::Shader* shader;
         bool show_tile_mesh;
         bool show_collision_tile_mesh;
         AllegroFlare::CameraControlStrategies2D::Base* camera_control_strategy;
         ALLEGRO_BITMAP* backbuffer_sub_bitmap;
         bool showing_full_color_overlay;
         ALLEGRO_COLOR full_color_overlay_color;
         float full_color_overlay_opacity;
         bool showing_banner_text;
         ALLEGRO_COLOR banner_text_color;
         std::string banner_text;
         bool showing_banner_subtext;
         ALLEGRO_COLOR banner_subtext_color_a;
         ALLEGRO_COLOR banner_subtext_color_b;
         std::string banner_subtext;
         float banner_subtext_pulse_rate_per_sec;
         float banner_subtext_shown_at;
         uint32_t state;
         float state_changed_at;
         bool state_is_busy;
         void initialize_shader();
         void initialize_camera();
         void cleanup_entities_flagged_for_deletion();
         void flag_all_entities_for_deletion();
         void check_player_collisions_with_doors();
         void update_player_collisions_with_collectables();
         void update_player_collisions_with_goalposts();

      protected:


      public:
         Screen(AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::FontBin* font_bin=nullptr, AllegroFlare::Display* display=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr);
         virtual ~Screen();

         void set_entity_pool(std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> entity_pool);
         void set_camera_baseline_zoom(AllegroFlare::Vec2D camera_baseline_zoom);
         void set_show_tile_mesh(bool show_tile_mesh);
         void set_show_collision_tile_mesh(bool show_collision_tile_mesh);
         AllegroFlare::BitmapBin* get_bitmap_bin() const;
         AllegroFlare::FontBin* get_font_bin() const;
         std::map<std::string, std::string> get_map_dictionary() const;
         AllegroFlare::Vec2D get_camera_baseline_zoom() const;
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* get_player_controlled_entity() const;
         bool get_show_tile_mesh() const;
         bool get_show_collision_tile_mesh() const;
         AllegroFlare::FrameAnimation::Book &get_animation_book_ref();
         void set_state(uint32_t state=STATE_UNDEF, float time_now=al_get_time());
         void update_state(float time_now=al_get_time());
         ALLEGRO_COLOR get_sparkle_win_color();
         void play_win_music();
         void set_map_dictionary(std::map<std::string, std::string> map_dictionary={});
         void set_display(AllegroFlare::Display* display=nullptr);
         void set_event_emitter(AllegroFlare::EventEmitter* event_emitter=nullptr);
         void set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin=nullptr);
         void set_font_bin(AllegroFlare::FontBin* font_bin=nullptr);
         void set_currently_active_map(std::string name="[unset-current-map-name-to-use]");
         void set_player_controlled_entity(KrampusReturns::Entities::Krampus* player_controlled_entity=nullptr);
         AllegroFlare::Prototypes::Platforming2D::Entities::TileMaps::Basic2D* find_map_by_name(std::string name="[unset-map-name]");
         virtual void on_activate() override;
         virtual void on_deactivate() override;
         void destroy();
         void load_level_and_start(std::string level_name="[unset-level_name]");
         void load_objects_from_map_files();
         static AllegroFlare::Vec2D center_of(float x=0.0f, float y=0.0f, float w=0.0f, float h=0.0f);
         static void tmj_object_parse_callback_func(std::string object_class="[unset-object_class]", float x=0.0f, float y=0.0f, float w=0.0f, float h=0.0f, void* user_data=nullptr);
         void initialize_maps();
         void add_entity_to_pool(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity=nullptr);
         void reset_camera_control(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity_to_follow=nullptr);
         void initialize_backbuffer_sub_bitmap();
         void initialize_animation_book();
         void initialize();
         void start_level();
         void reverse_gravity();
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> select_enemies(std::string on_map_name="[unset-on_map_name]");
         void update_enemy_collisions_with_damage_zones();
         void update_player_collisions_with_damage_zones();
         void update_entities();
         void dump_entity_data();
         void draw_entities();
         void update();
         void draw();
         void set_full_color_overlay(ALLEGRO_COLOR base_color=ALLEGRO_COLOR{0.0, 0.3, 0.8, 1.0}, float opacity=0.2f);
         void show_full_color_overlay();
         void hide_full_color_overlay();
         void set_banner_text(std::string text="[unset-text]", ALLEGRO_COLOR base_color=ALLEGRO_COLOR{0.0, 0.3, 0.8, 1.0});
         void show_banner_text();
         void hide_banner_text();
         void set_banner_subtext(std::string text="[unset-text]", ALLEGRO_COLOR base_color_a=ALLEGRO_COLOR{0.0, 0.3, 0.8, 1.0}, ALLEGRO_COLOR base_color_b=ALLEGRO_COLOR{0.0, 0.3, 0.8, 1.0}, float pulse_rate_per_sec=8.0f);
         void show_banner_subtext();
         void hide_banner_subtext();
         void draw_hud();
         void toggle_show_collision_tile_mesh();
         void toggle_show_tile_mesh();
         virtual void primary_timer_func() override;
         void shake_camera(float intensity=1.0, float duration=2.0, float time_now=al_get_time());
         void spawn_flash_effect(std::string type_str="[unset-type_str]", float x=0.0f, float y=0.0f);
         virtual void game_event_func(AllegroFlare::GameEvent* ev=nullptr) override;
         virtual void key_char_func(ALLEGRO_EVENT* event=nullptr) override;
         virtual void key_up_func(ALLEGRO_EVENT* event=nullptr) override;
         virtual void key_down_func(ALLEGRO_EVENT* event=nullptr) override;
         virtual void user_event_func(ALLEGRO_EVENT* event=nullptr) override;
         void render_collision_tile_mesh();
         AllegroFlare::TileMaps::PrimMeshAtlas* get_tile_atlas();
         AllegroFlare::TileMaps::PrimMesh* get_tile_mesh();
         AllegroFlare::TileMaps::TileMap<int>* get_collision_tile_mesh();
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> get_current_map_entities();
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> get_current_map_entities_y_sorted();
         ALLEGRO_FONT* obtain_banner_text_font();
         ALLEGRO_FONT* obtain_banner_subtext_font();
      };
   }
}



