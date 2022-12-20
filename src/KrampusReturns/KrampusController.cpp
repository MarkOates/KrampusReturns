

#include <KrampusReturns/KrampusController.hpp>

#include <AllegroFlare/Prototypes/Platforming2D/EntityFlagNames.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{


KrampusController::KrampusController(KrampusReturns::Entities::Krampus* player_controlled_entity)
   : player_controls()
   , player_controlled_entity(player_controlled_entity)
{
}


KrampusController::~KrampusController()
{
}


void KrampusController::set_player_controlled_entity(KrampusReturns::Entities::Krampus* player_controlled_entity)
{
   this->player_controlled_entity = player_controlled_entity;
}


KrampusReturns::Entities::Krampus* KrampusController::get_player_controlled_entity() const
{
   return player_controlled_entity;
}


void KrampusController::reset()
{
   player_controls.clear();
}

void KrampusController::key_up_func(int al_key_num, bool is_repeat)
{
   if (!(player_controlled_entity))
   {
      std::stringstream error_message;
      error_message << "[KrampusController::key_up_func]: error: guard \"player_controlled_entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("KrampusController::key_up_func: error: guard \"player_controlled_entity\" not met");
   }
   switch (al_key_num)
   {
      case ALLEGRO_KEY_LEFT:
         player_controls.set_left_button_pressed(false);
      break;

      case ALLEGRO_KEY_RIGHT:
         player_controls.set_right_button_pressed(false);
      break;

      case ALLEGRO_KEY_UP:
         player_controls.set_up_button_pressed(false);
      break;

      case ALLEGRO_KEY_DOWN:
         player_controls.set_down_button_pressed(false);
      break;
   }
   return;
}

void KrampusController::key_down_func(int al_key_num, bool is_repeat)
{
   if (!(player_controlled_entity))
   {
      std::stringstream error_message;
      error_message << "[KrampusController::key_down_func]: error: guard \"player_controlled_entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("KrampusController::key_down_func: error: guard \"player_controlled_entity\" not met");
   }
   switch (al_key_num)
   {
      case ALLEGRO_KEY_LEFT:
         player_controls.set_left_button_pressed(true);
      break;

      case ALLEGRO_KEY_RIGHT:
         player_controls.set_right_button_pressed(true);
      break;

      case ALLEGRO_KEY_UP:
         player_controls.set_up_button_pressed(true);
      break;

      case ALLEGRO_KEY_DOWN:
         player_controls.set_down_button_pressed(true);
      break;

      case ALLEGRO_KEY_SPACE:
         jump();
      break;
   }
   return;
}

void KrampusController::update()
{
   if (!(player_controlled_entity))
   {
      std::stringstream error_message;
      error_message << "[KrampusController::update]: error: guard \"player_controlled_entity\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("KrampusController::update: error: guard \"player_controlled_entity\" not met");
   }
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   // if this block is active, the player cannot control themselves while in the air, only when on the ground:
   //if (player_controlled_entity->exists(ADJACENT_TO_FLOOR))
   //{
      //player_controlled_entity->get_velocity_ref().position.x = 0.0;
   //}

   if (player_controls.get_right_bumper_pressed())
   {
      // player character is in a defensive position and not moving (or, they're aiming and not moving)
      player_controlled_entity->get_velocity_ref().position.x = 0.0;
      player_controlled_entity->get_velocity_ref().position.y = 0.0;
   }
   else
   {
      // if this block is active, the player cannot control themselves while in the air, only when on the ground:
      // NOTE: previously, being ajacent to the floor would stop the player from moving unless
      // they have a movment control button pressed
      //if (player_controlled_entity->exists(ADJACENT_TO_FLOOR))
      {
         player_controlled_entity->get_velocity_ref().position.x = 0.0;
         player_controlled_entity->get_velocity_ref().position.y = 0.0;
      }

         if (player_controls.get_right_button_pressed())
         {
            player_controlled_entity->get_velocity_ref().position.x = 1.5; //2.0;
         }
         if (player_controls.get_left_button_pressed())
         {
            player_controlled_entity->get_velocity_ref().position.x = -1.5; //-2.0;
         }
         if (player_controls.get_up_button_pressed())
         {
            player_controlled_entity->get_velocity_ref().position.y = -1.5; //2.0;
         }
         if (player_controls.get_down_button_pressed())
         {
            player_controlled_entity->get_velocity_ref().position.y = 1.5; //-2.0;
         }
   }
   return;
}

void KrampusController::jump()
{
   using namespace AllegroFlare::Prototypes::Platforming2D::EntityFlagNames;

   if (!player_controlled_entity) return;
   if (player_controlled_entity->exists(ADJACENT_TO_FLOOR))
   {
      player_controlled_entity->get_velocity_ref().position.y -= 4.25;
   }
   else if (player_controlled_entity->exists(ADJACENT_TO_LEFT_WALL))
   {
      player_controlled_entity->get_velocity_ref().position.y = -3.5;
      player_controlled_entity->get_velocity_ref().position.x = 3.0;
   }
   else if (player_controlled_entity->exists(ADJACENT_TO_RIGHT_WALL))
   {
      player_controlled_entity->get_velocity_ref().position.y = -3.5;
      player_controlled_entity->get_velocity_ref().position.x = -3.0;
   }
   return;
}


} // namespace KrampusReturns


