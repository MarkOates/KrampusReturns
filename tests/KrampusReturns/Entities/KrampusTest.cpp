
#include <gtest/gtest.h>

#include <KrampusReturns/Entities/Krampus.hpp>
#include <KrampusReturns/Testing/Gym.hpp>


class KrampusReturns_Entities_KrampusGym : public KrampusReturns::Testing::Gym {};


TEST_F(KrampusReturns_Entities_KrampusGym, can_be_created_without_blowing_up)
{
   //result->set_animation_book(get_animation_book());
   //result->initialize();

   //result->get_place_ref().position = { x, y };
   //result->set(ON_MAP_NAME, on_map);

      //if (init_entities_drawing_debug) result->set_draw_debug(true);
   //if (true) result->set_draw_debug(true);



   KrampusReturns::Entities::Krampus krampus;
   //krampus.set_animation_book(animation_book);

      //AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D *krampus =
         //entity_factory.create_krampus("map_a", 400/2, 240/2);
   get_platforming_2d_ref().add_entity_to_pool(&krampus);
  //- name: platforming_2d

   get_platforming_2d_ref().set_player_controlled_entity(&krampus);

   //run_gym();
   run_gym();
}


//TEST(KrampusReturns_Entities_KrampusTest, TYPE__has_the_expected_value)
//{
   // TODO: figure out this typing thing
   //KrampusReturns::Entities::Krampus krampus;
   //EXPECT_EQ("Entities/Krampus", krampus.get_type());
//}


//TEST(KrampusReturns_Entities_KrampusTest, type__has_the_expected_value_matching_TYPE)
//{
   // TODO: figure out this typing thing
   //KrampusReturns::Entities::Krampus krampus;
   //EXPECT_EQ(KrampusReturns::Entities::Krampus::TYPE, krampus.get_type());
//}


