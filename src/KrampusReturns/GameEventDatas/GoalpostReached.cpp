

#include <KrampusReturns/GameEventDatas/GoalpostReached.hpp>




namespace KrampusReturns
{
namespace GameEventDatas
{


GoalpostReached::GoalpostReached(int goalpost_id)
   : AllegroFlare::GameEventDatas::Base(KrampusReturns::GameEventDatas::GoalpostReached::TYPE)
   , goalpost_id(goalpost_id)
{
}


GoalpostReached::~GoalpostReached()
{
}


void GoalpostReached::set_goalpost_id(int goalpost_id)
{
   this->goalpost_id = goalpost_id;
}


int GoalpostReached::get_goalpost_id() const
{
   return goalpost_id;
}




} // namespace GameEventDatas
} // namespace KrampusReturns


