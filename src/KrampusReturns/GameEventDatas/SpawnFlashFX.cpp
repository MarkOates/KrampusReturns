

#include <KrampusReturns/GameEventDatas/SpawnFlashFX.hpp>




namespace KrampusReturns
{
namespace GameEventDatas
{


SpawnFlashFX::SpawnFlashFX(std::string animation_name, float x, float y)
   : AllegroFlare::GameEventDatas::Base(KrampusReturns::GameEventDatas::SpawnFlashFX::TYPE)
   , animation_name(animation_name)
   , x(x)
   , y(y)
{
}


SpawnFlashFX::~SpawnFlashFX()
{
}


void SpawnFlashFX::set_animation_name(std::string animation_name)
{
   this->animation_name = animation_name;
}


void SpawnFlashFX::set_x(float x)
{
   this->x = x;
}


void SpawnFlashFX::set_y(float y)
{
   this->y = y;
}


std::string SpawnFlashFX::get_animation_name() const
{
   return animation_name;
}


float SpawnFlashFX::get_x() const
{
   return x;
}


float SpawnFlashFX::get_y() const
{
   return y;
}




} // namespace GameEventDatas
} // namespace KrampusReturns


