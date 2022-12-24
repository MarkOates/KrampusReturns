#pragma once


#include <map>
#include <string>


namespace KrampusReturns
{
   class Level
   {
   private:
      std::string identifier;
      std::string title;
      std::map<std::string, std::string> map_dictionary;
      std::string music_identifier;

   protected:


   public:
      Level(std::string identifier="[unset-identifier]", std::string title="[unset-name]", std::map<std::string, std::string> map_dictionary={}, std::string music_identifier="[unset-music_identifier]");
      ~Level();

      void set_identifier(std::string identifier);
      void set_title(std::string title);
      std::string get_identifier() const;
      std::string get_title() const;
      std::map<std::string, std::string> get_map_dictionary() const;
      std::string get_music_identifier() const;
   };
}



