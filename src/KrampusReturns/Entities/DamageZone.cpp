

#include <KrampusReturns/Entities/DamageZone.hpp>




namespace KrampusReturns
{
namespace Entities
{


DamageZone::DamageZone(int damage, uint32_t direction_of_force)
   : AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D()
   , damage(damage)
   , direction_of_force(direction_of_force)
{
}


DamageZone::~DamageZone()
{
}


void DamageZone::set_damage(int damage)
{
   this->damage = damage;
}


void DamageZone::set_direction_of_force(uint32_t direction_of_force)
{
   this->direction_of_force = direction_of_force;
}


int DamageZone::get_damage() const
{
   return damage;
}


uint32_t DamageZone::get_direction_of_force() const
{
   return direction_of_force;
}


void DamageZone::TODO()
{
   // HACK: this class derives from FrameAnimated2D, which is silly. It should derive from a Base of some kind.
   // Unfortunately, the current KrampusReturns game system relies more on FrameAnimated2D as a kind of base class
   // so will continue to rely on that for now until it can be fixed later. Sorry for the confusion.
   // Also, the TYPE property is completely unused in this context.
   return;
}


} // namespace Entities
} // namespace KrampusReturns


