

#include <KrampusReturns/World.hpp>




namespace KrampusReturns
{


World::World(std::vector<KrampusReturns::Level> levels)
   : levels(levels)
{
}


World::~World()
{
}


void World::set_levels(std::vector<KrampusReturns::Level> levels)
{
   this->levels = levels;
}


std::vector<KrampusReturns::Level> World::get_levels() const
{
   return levels;
}


std::vector<KrampusReturns::Level> &World::get_levels_ref()
{
   return levels;
}




} // namespace KrampusReturns


