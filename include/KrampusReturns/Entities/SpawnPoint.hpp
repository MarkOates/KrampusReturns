#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>


namespace KrampusReturns
{
   namespace Entities
   {
      class SpawnPoint : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D
      {
      public:
         static constexpr char* TYPE = (char*)"Entities/SpawnPoint";

      private:
         bool initialized;

      protected:


      public:
         SpawnPoint();
         virtual ~SpawnPoint();

         void initialize();
         virtual void update() override;
         virtual void draw() override;
      };
   }
}



