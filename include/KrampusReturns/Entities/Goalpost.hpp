#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>


namespace KrampusReturns
{
   namespace Entities
   {
      class Goalpost : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D
      {
      public:
         static constexpr char* TYPE = (char*)"Entities/Goalpost";

      private:
         bool initialized;

      protected:


      public:
         Goalpost();
         virtual ~Goalpost();

         void initialize();
         virtual void update() override;
         virtual void draw() override;
      };
   }
}



