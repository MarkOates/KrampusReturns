

#include <KrampusReturns/EntityFactory.hpp>

#include <KrampusReturns/Entities/Krampus.hpp>


namespace KrampusReturns
{


EntityFactory::EntityFactory()
{
}


EntityFactory::~EntityFactory()
{
}


AllegroFlare::Prototypes::Platforming2D::Entities::Basic2D* EntityFactory::create_krampus()
{
   KrampusReturns::Entities::Krampus *result = new KrampusReturns::Entities::Krampus();
   return result;
}


} // namespace KrampusReturns


