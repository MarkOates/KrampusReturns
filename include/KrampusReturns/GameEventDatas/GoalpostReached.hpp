#pragma once


#include <AllegroFlare/GameEventDatas/Base.hpp>


namespace KrampusReturns
{
   namespace GameEventDatas
   {
      class GoalpostReached : public AllegroFlare::GameEventDatas::Base
      {
      public:
         static constexpr char* TYPE = (char*)"GameEventDatas/GoalpostReached";

      private:
         int goalpost_id;

      protected:


      public:
         GoalpostReached(int goalpost_id=-1);
         ~GoalpostReached();

         void set_goalpost_id(int goalpost_id);
         int get_goalpost_id() const;
      };
   }
}



