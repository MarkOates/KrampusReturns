#pragma once


#include <AllegroFlare/Shader.hpp>
#include <string>


namespace KrampusReturns
{
   namespace Shaders
   {
      class AllegroDefault : public AllegroFlare::Shader
      {
      private:
         bool initialized;
         static std::string obtain_vertex_source();
         static std::string obtain_fragment_source();

      protected:


      public:
         AllegroDefault();
         ~AllegroDefault();

         void initialize();
         void activate();
      };
   }
}



