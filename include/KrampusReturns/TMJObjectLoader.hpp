#pragma once


#include <string>


namespace KrampusReturns
{
   class TMJObjectLoader
   {
   private:
      std::string filename;
      bool loaded;
      static bool file_exists(std::string filename="[unset-filename]");

   protected:


   public:
      TMJObjectLoader(std::string filename="filename-not-set.tmj");
      ~TMJObjectLoader();

      void load();
   };
}



