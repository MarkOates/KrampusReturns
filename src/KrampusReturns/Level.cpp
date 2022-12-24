

#include <KrampusReturns/Level.hpp>




namespace KrampusReturns
{


Level::Level()
   : identifier("[unset-identifier]")
   , title("[unset-name]")
   , map_dictionary({})
   , music_identifier("[unset-music_identifier]")
{
}


Level::~Level()
{
}


void Level::set_identifier(std::string identifier)
{
   this->identifier = identifier;
}


void Level::set_title(std::string title)
{
   this->title = title;
}


std::string Level::get_identifier() const
{
   return identifier;
}


std::string Level::get_title() const
{
   return title;
}


std::map<std::string, std::string> Level::get_map_dictionary() const
{
   return map_dictionary;
}


std::string Level::get_music_identifier() const
{
   return music_identifier;
}




} // namespace KrampusReturns


