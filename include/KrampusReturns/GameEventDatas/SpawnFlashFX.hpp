#pragma once


#include <AllegroFlare/GameEventDatas/Base.hpp>
#include <string>


namespace KrampusReturns
{
   namespace GameEventDatas
   {
      class SpawnFlashFX : public AllegroFlare::GameEventDatas::Base
      {
      public:
         static constexpr char* TYPE = (char*)"GameEventDatas/SpawnFlashFX";

      private:
         std::string animation_name;
         float x;
         float y;

      protected:


      public:
         SpawnFlashFX(std::string animation_name="[unset-animation_name]", float x=0.0f, float y=0.0f);
         ~SpawnFlashFX();

         void set_animation_name(std::string animation_name);
         void set_x(float x);
         void set_y(float y);
         std::string get_animation_name() const;
         float get_x() const;
         float get_y() const;
      };
   }
}



