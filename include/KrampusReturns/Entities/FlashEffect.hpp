#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>


namespace KrampusReturns
{
   namespace Entities
   {
      class FlashEffect : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D
      {
      public:
         static constexpr char* TYPE = (char*)"Entities/FlashEffect";

      private:
         bool initialized;

      protected:


      public:
         FlashEffect();
         virtual ~FlashEffect();

         void initialize();
         virtual void update() override;
      };
   }
}



