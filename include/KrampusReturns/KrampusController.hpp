#pragma once


#include <AllegroFlare/VirtualController.hpp>
#include <KrampusReturns/Entities/Krampus.hpp>


namespace KrampusReturns
{
   class KrampusController
   {
   private:
      AllegroFlare::VirtualController player_controls;
      KrampusReturns::Entities::Krampus* krampus;
      void jump();

   protected:


   public:
      KrampusController(KrampusReturns::Entities::Krampus* krampus=nullptr);
      virtual ~KrampusController();

      void set_krampus(KrampusReturns::Entities::Krampus* krampus);
      KrampusReturns::Entities::Krampus* get_krampus() const;
      void reset();
      virtual void key_up_func(int al_key_num=0, bool is_repeat=false);
      virtual void key_down_func(int al_key_num=0, bool is_repeat=false);
      void update();
   };
}



