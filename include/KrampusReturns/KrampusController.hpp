#pragma once


#include <AllegroFlare/VirtualControls.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>


namespace KrampusReturns
{
   class KrampusController
   {
   private:
      AllegroFlare::VirtualControls player_controls;
      KrampusReturns::Entities::Krampus* player_controlled_entity;

   protected:


   public:
      KrampusController(KrampusReturns::Entities::Krampus* player_controlled_entity=nullptr);
      virtual ~KrampusController();

      void set_player_controlled_entity(KrampusReturns::Entities::Krampus* player_controlled_entity);
      KrampusReturns::Entities::Krampus* get_player_controlled_entity() const;
      void reset();
      void update();
      void unset_player_controlled_entity_vertical_velocity();
      void unset_player_controlled_entity_horizontal_velocity();
      void set_player_controlled_entity_jump();
      virtual void key_up_func(int al_key_num=0, bool is_repeat=false);
      virtual void key_down_func(int al_key_num=0, bool is_repeat=false);
   };
}



