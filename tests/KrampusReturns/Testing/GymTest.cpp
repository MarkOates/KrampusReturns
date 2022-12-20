
#include <gtest/gtest.h>

#include <KrampusReturns/Testing/Gym.hpp>


class TestClassFor_KrampusReturns_Testing_Gym
   : public KrampusReturns::Testing::Gym{};


TEST_F(TestClassFor_KrampusReturns_Testing_Gym, get_test_name__will_return_the_name_of_the_test)
{
   std::string expected_test_name = "get_test_name__will_return_the_name_of_the_test";
   EXPECT_EQ(expected_test_name, get_test_name());
}


TEST_F(TestClassFor_KrampusReturns_Testing_Gym, run_gym__will_run_the_framework_loop_with_the_expected_content)
{
   //run_gym();
}


//TEST_F(TestClassFor_KrampusReturns_Testing_Gym,
   //get_test_suite_name__will_return_the_name_of_the_test)
//{
   //std::string expected_test_name = "TestClassFor_KrampusReturns_Testing_Gym";
   //EXPECT_EQ(expected_test_name, get_test_suite_name());
//}


//TEST_F(TestClassFor_KrampusReturns_Testing_Gym,
   //DISABLED__will_set_the_window_title_to_the_test_suite_name_and_test_name)
//{
   //// NOTE: cannot currently test this, Allegro5 does not currently have a al_get_window_title() feature
//}


//TEST_F(TestClassFor_KrampusReturns_Testing_Gym,
   //DISABLED__with_a_test_name_prefixed_with_CAPTURE__will_save_a_screenshot_of_the_backbuffer_during_teardown)
//{
   //// NOTE: cannot currently test this, assertion would need to happen after teardown
//}


//TEST_F(TestClassFor_KrampusReturns_Testing_Gym,
   //DISABLED__clear_display__will_clear_the_current_target_rendering_surface_to_an_eigengrau_color)
//{
   //// TODO
//}


//TEST_F(TestClassFor_KrampusReturns_Testing_Gym,
   //DISABLED__get_display_backbuffer__will_return_the_backbuffer_of_the_test_window)
//{
   //// TODO
//}

