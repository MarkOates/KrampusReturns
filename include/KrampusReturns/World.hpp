#pragma once


#include <KrampusReturns/Level.hpp>
#include <vector>


namespace KrampusReturns
{
   class World
   {
   private:
      std::vector<KrampusReturns::Level> levels;

   protected:


   public:
      World(std::vector<KrampusReturns::Level> levels={});
      ~World();

      void set_levels(std::vector<KrampusReturns::Level> levels);
      std::vector<KrampusReturns::Level> get_levels() const;
      std::vector<KrampusReturns::Level> &get_levels_ref();
   };
}



