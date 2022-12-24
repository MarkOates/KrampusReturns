

#include <KrampusReturns/Level.hpp>




namespace KrampusReturns
{


Level::Level(std::string identifier, std::string title, std::map<std::string, std::string> map_dictionary, std::string music_identifier)
   : identifier(identifier)
   , title(title)
   , map_dictionary(map_dictionary)
   , music_identifier(music_identifier)
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


