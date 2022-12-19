
#include <gtest/gtest.h>


#define ASSERT_THROW_WITH_MESSAGE(code, raised_exception_type, raised_exception_message) \
   try { code; FAIL() << "Expected " # raised_exception_type; } \
   catch ( raised_exception_type const &err ) { EXPECT_EQ(err.what(), std::string( raised_exception_message )); } \
   catch (...) { FAIL() << "Expected " # raised_exception_type; }


#include <KrampusReturns/Shaders/Primary.hpp>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_color.h>
#include <AllegroFlare/Placement2D.hpp>


class KrampusReturns_Shaders_PrimaryTest : public ::testing::Test
{
protected:
   ALLEGRO_DISPLAY *display;

   KrampusReturns_Shaders_PrimaryTest()
      : display(nullptr)
   {
   }

   virtual void SetUp() override
   {
      ASSERT_EQ(false, al_is_system_installed());
      ASSERT_EQ(true, al_init());

      al_set_new_display_flags(ALLEGRO_OPENGL | ALLEGRO_PROGRAMMABLE_PIPELINE);
      ASSERT_EQ(ALLEGRO_OPENGL, al_get_new_display_flags() & ALLEGRO_OPENGL);
      ASSERT_EQ(ALLEGRO_PROGRAMMABLE_PIPELINE, al_get_new_display_flags() & ALLEGRO_PROGRAMMABLE_PIPELINE);
      ALLEGRO_DISPLAY *display = al_create_display(1920, 1080);
      ASSERT_NE(nullptr, display);
   }

   virtual void TearDown() override
   {
      if (al_get_current_display()) al_destroy_display(al_get_current_display());
      al_uninstall_system();
   }

   ALLEGRO_DISPLAY *get_display()
   {
      return display;
   }
};


TEST_F(KrampusReturns_Shaders_PrimaryTest, can_be_created_without_blowing_up)
{
   KrampusReturns::Shaders::Primary flat_color_shader;
   SUCCEED();
}


TEST_F(KrampusReturns_Shaders_PrimaryTest, activate__before_being_initialized_raises_an_exception)
{
   KrampusReturns::Shaders::Primary flat_color_shader;
   std::string expected_error_message = "[KrampusReturns::Shaders::Primary] Attempting to activate() shader before it has been initialized";
   ASSERT_THROW_WITH_MESSAGE(flat_color_shader.activate(), std::runtime_error, expected_error_message);
}


TEST_F(KrampusReturns_Shaders_PrimaryTest, initialize__works_without_blowing_up)
{
   KrampusReturns::Shaders::Primary flat_color_shader;
   flat_color_shader.initialize();
   SUCCEED();
}


TEST_F(KrampusReturns_Shaders_PrimaryTest, activate__works_without_blowing_up)
{
   KrampusReturns::Shaders::Primary flat_color_shader;

   flat_color_shader.initialize();
   flat_color_shader.activate();
}


TEST_F(KrampusReturns_Shaders_PrimaryTest, when_active__renders_the_image_with_the_flat_color)
{
   KrampusReturns::Shaders::Primary flat_color_shader;
   flat_color_shader.initialize();

   ALLEGRO_COLOR color = al_color_name("midnightblue");
   flat_color_shader.set_tint(color);
   //flat_color_shader.set_tint_intensity(0.5);
   flat_color_shader.activate();

   al_init_image_addon();

   // TODO: update this to be dynamic
   ALLEGRO_BITMAP *test_image = al_load_bitmap(
      "/Users/markoates/Repos/KrampusReturns/tests/fixtures/billboarding_tester_sprite.png"
      //"./tests/fixtures/billboarding_tester_sprite.png"
   );
   ASSERT_NE(nullptr, test_image);


   ALLEGRO_DISPLAY *current_display = al_get_current_display();
   ASSERT_NE(nullptr, current_display);

   al_clear_to_color(ALLEGRO_COLOR{0.2, 0.2, 0.25, 1.0});
   AllegroFlare::Placement2D place(
         al_get_display_width(current_display)/2,
         al_get_display_height(current_display)/2,
         al_get_bitmap_width(test_image),
         al_get_bitmap_height(test_image));
   place.scale = AllegroFlare::vec2d(4, 4);
   place.start_transform();
   al_draw_bitmap(test_image, 0, 0, 0);
   place.restore_transform();


   al_flip_display();
   al_rest(0.25);
   al_flip_display();


   ALLEGRO_COLOR expected_color = color;
   ALLEGRO_COLOR actual_color = al_get_pixel(al_get_backbuffer(current_display), al_get_display_width(current_display)/2, al_get_display_height(current_display)/2);

   //ASSERT_EQ(color.r, actual_color.r);
   //ASSERT_EQ(color.g, actual_color.g);
   //ASSERT_EQ(color.b, actual_color.b);
   //ASSERT_EQ(color.a, actual_color.a);

   std::string tmp_path = "/Users/markoates/Repos/KrampusReturns/tmp/";
   std::string output_image_full_filename = tmp_path + "when_activated__renders_bitmaps_with_a_solid_tinted_overlay.png";
   ASSERT_EQ(true, al_save_bitmap(output_image_full_filename.c_str(), al_get_backbuffer(current_display)));
}


//#include <KrampusReturns/Runner.hpp>

//TEST(KrampusReturns_RunnerTest, run__runs_the_program)
//{
   //KrampusReturns::Runner runner;
   //runner.run("test");
   //SUCCEED();
//}

