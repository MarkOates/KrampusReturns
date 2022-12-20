#pragma once


#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>


namespace KrampusReturns
{
   namespace Entities
   {
      class Blob : public AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D
      {
      public:
         static constexpr char* TYPE = (char*)"Entities/Blob";

      private:
         bool initialized;

      protected:


      public:
         Blob();
         virtual ~Blob();

         virtual void update() override;
         void initialize();
      };
   }
}



