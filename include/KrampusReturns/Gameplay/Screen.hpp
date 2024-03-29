#pragma once


#include <AllegroFlare/BitmapBin.hpp>
#include <AllegroFlare/Camera2D.hpp>
#include <AllegroFlare/CameraControlStrategies2D/Base.hpp>
#include <AllegroFlare/EventEmitter.hpp>
#include <AllegroFlare/FontBin.hpp>
#include <AllegroFlare/FrameAnimation/Book.hpp>
#include <AllegroFlare/GameEvent.hpp>
#include <AllegroFlare/Profiler.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/Basic2D.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/TileMaps/Basic2D.hpp>
#include <AllegroFlare/RenderSurfaces/Base.hpp>
#include <AllegroFlare/Screens/Base.hpp>
#include <AllegroFlare/Shaders/Base.hpp>
#include <AllegroFlare/TileMaps/PrimMesh.hpp>
#include <AllegroFlare/TileMaps/PrimMeshAtlas.hpp>
#include <AllegroFlare/TileMaps/TileMap.hpp>
#include <AllegroFlare/Vec2D.hpp>
#include <ChatGPT/Enemy.hpp>
#include <KrampusReturns/Entities/DamageZone.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>
#include <KrampusReturns/KrampusController.hpp>
#include <KrampusReturns/Level.hpp>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <cstdint>
#include <map>
#include <string>
#include <utility>
#include <vector>


namespace KrampusReturns
{
   namespace Gameplay
   {
      class Screen : public AllegroFlare::Screens::Base
      {
      public:
         static constexpr char* MODE_TEST = "test";
         static constexpr char* MODE_PRODUCTION = "production";
         static constexpr char* MODE_DEVELOPMENT = "development";
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
         AllegroFlare::EventEmitter* event_emitter;
         AllegroFlare::RenderSurfaces::Base* primary_render_surface;
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
         AllegroFlare::Shaders::Base* shader;
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
         bool boss_beaten;
         KrampusReturns::Level current_level_data;
         std::string main_background_music_identifier;
         ALLEGRO_BITMAP* little_shadow_bitmap;
         std::string mode;
         AllegroFlare::Profiler* profiler;
         KrampusReturns::Entities::DamageZone* DUMMY_DEP;
         void move_krampus_to_first_spawn_point_or_default(KrampusReturns::Entities::Krampus* krampus=nullptr, std::string map_name="[unset-map_name]");
         void initialize_shader();
         void initialize_camera();
         void mark_all_ephemeral_entities_for_deletion();
         void cleanup_entities_flagged_for_deletion();
         void flag_all_entities_for_deletion();
         void check_player_collisions_with_doors();
         void update_player_collisions_with_collectables();
         void update_player_collisions_with_goalposts();

      protected:


      public:
         Screen(AllegroFlare::BitmapBin* bitmap_bin=nullptr, AllegroFlare::FontBin* font_bin=nullptr, AllegroFlare::EventEmitter* event_emitter=nullptr, std::string mode=MODE_PRODUCTION);
         virtual ~Screen();

         void set_entity_pool(std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> entity_pool);
         void set_camera_baseline_zoom(AllegroFlare::Vec2D camera_baseline_zoom);
         void set_show_tile_mesh(bool show_tile_mesh);
         void set_show_collision_tile_mesh(bool show_collision_tile_mesh);
         void set_mode(std::string mode);
         void set_profiler(AllegroFlare::Profiler* profiler);
         AllegroFlare::BitmapBin* get_bitmap_bin() const;
         AllegroFlare::FontBin* get_font_bin() const;
         AllegroFlare::RenderSurfaces::Base* get_primary_render_surface() const;
         std::map<std::string, std::string> get_map_dictionary() const;
         AllegroFlare::Vec2D get_camera_baseline_zoom() const;
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* get_player_controlled_entity() const;
         bool get_show_tile_mesh() const;
         bool get_show_collision_tile_mesh() const;
         std::string get_mode() const;
         AllegroFlare::Profiler* get_profiler() const;
         AllegroFlare::FrameAnimation::Book &get_animation_book_ref();
         void NOTE();
         void set_state(uint32_t state=STATE_UNDEF, float time_now=al_get_time());
         void update_state(float time_now=al_get_time());
         ALLEGRO_COLOR get_sparkle_win_color();
         void play_win_music();
         void play_level_music();
         void play_boss_music();
         void set_map_dictionary(std::map<std::string, std::string> map_dictionary={});
         void set_event_emitter(AllegroFlare::EventEmitter* event_emitter=nullptr);
         void set_bitmap_bin(AllegroFlare::BitmapBin* bitmap_bin=nullptr);
         void set_font_bin(AllegroFlare::FontBin* font_bin=nullptr);
         void set_primary_render_surface(AllegroFlare::RenderSurfaces::Base* primary_render_surface=nullptr);
         void set_currently_active_map(std::string name="[unset-current-map-name-to-use]");
         void set_player_controlled_entity(KrampusReturns::Entities::Krampus* player_controlled_entity=nullptr);
         AllegroFlare::Prototypes::Platforming2D::Entities::TileMaps::Basic2D* find_map_by_name(std::string name="[unset-map-name]");
         virtual void on_activate() override;
         virtual void on_deactivate() override;
         void destroy_all();
         void load_level_and_start(KrampusReturns::Level* level=nullptr);
         void start_level();
         void load_objects_from_map_files();
         std::pair<int, int> calc_room_coords(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity=nullptr);
         std::pair<float, float> calc_room_width();
         static AllegroFlare::Vec2D center_of(float x=0.0f, float y=0.0f, float w=0.0f, float h=0.0f);
         static void tmj_object_parse_callback_func(std::string object_class="[unset-object_class]", float x=0.0f, float y=0.0f, float w=0.0f, float h=0.0f, void* user_data=nullptr);
         void initialize_maps();
         void add_entity_to_pool(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity=nullptr);
         void reset_camera_control(AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* entity_to_follow=nullptr);
         void initialize_animation_book();
         void initialize();
         void reverse_gravity();
         void update_collisions_with_damaging_zones();
         bool player_controled_entity_in_same_room_as_boss();
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
         void emit_spawn_flash_fx_event(float x=0.0f, float y=0.0f, std::string type_str="flash_fx1");
         void create_damage_zone_by_player(std::string on_map="[unset-on_map]", float point_of_impact_x=0.0f, float point_of_impact_y=0.0f, float impact_width=20.0f, float impact_height=8.0f, int damage=1, uint32_t direction_of_force=KrampusReturns::Entities::DamageZone::DIRECTION_OF_FORCE_UNDEF);
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
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> select_spawn_points_on_map_name(std::string map_name="[unset-map_name]");
         std::vector<ChatGPT::Enemy*> select_seekers_on_map_name(std::string map_name="[unset-map_name]");
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> select_damages_player_entities_on_map(std::string on_map_name="[unset-on_map_name]");
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> select_takes_damage_from_player_damage_zone_entities_on_map(std::string on_map_name="[unset-on_map_name]");
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> select_damages_enemies_entities_on_map(std::string on_map_name="[unset-map_name]");
         std::vector<AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D*> select_collectable_by_player_on_map(std::string on_map_name="[unset-map_name]");
         bool are_any_blobs_present();
         bool are_any_flaming_skull_reds_present();
         bool are_any_skeletons_present();
         bool are_any_bosses_present();
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* find_boss();
         int count_num_spawn_points_in_all_maps();
         AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* find_first_spawn_point_on_any_map();
         ALLEGRO_FONT* obtain_banner_text_font();
         ALLEGRO_FONT* obtain_banner_subtext_font();
         ALLEGRO_FONT* obtain_profiler_font();
         static bool is_production_mode(std::string mode="[unset-mode]");
         static bool is_development_mode(std::string mode="[unset-mode]");
         static bool is_test_mode(std::string mode="[unset-mode]");
         bool in_development_mode();
         bool in_production_mode();
         bool in_test_mode();
      };
   }
}



