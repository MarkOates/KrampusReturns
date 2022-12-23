

#include <KrampusReturns/TMJObjectLoader.hpp>

#include <AllegroFlare/Errors.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <lib/nlohmann/json.hpp>
#include <sstream>
#include <stdexcept>


namespace KrampusReturns
{


TMJObjectLoader::TMJObjectLoader(std::string filename)
   : filename(filename)
   , loaded(false)
{
}


TMJObjectLoader::~TMJObjectLoader()
{
}


void TMJObjectLoader::load()
{
   if (!((!loaded)))
   {
      std::stringstream error_message;
      error_message << "[TMJObjectLoader::load]: error: guard \"(!loaded)\" not met.";
      std::cerr << "\033[1;31m" << error_message.str() << " An exception will be thrown to halt the program.\033[0m" << std::endl;
      throw std::runtime_error("TMJObjectLoader::load: error: guard \"(!loaded)\" not met");
   }
   if (!file_exists(filename))
   {
      std::stringstream error_message;
      error_message << "[KrampusReturns::TMJObjectLoader::load()]: error: The file "
                    << "\"" << filename << "\" does not exist.";
      throw std::runtime_error(error_message.str());
   }

   // load and validate the json data to variables
   std::ifstream i(filename);
   nlohmann::json j;
   try
   {
      i >> j;
   }
   catch (const std::exception& e)
   {
      i.close();
      std::stringstream error_message;
      error_message << "The file "
                    << "\"" << filename << "\" appears to have"
                    << " malformed JSON. The following error was thrown by nlohmann::json: \""
                    << e.what() << "\"";
      AllegroFlare::Errors::throw_error("KrampusReturns::TMJObjectLoader", error_message.str());
   }
   i.close();


   // TODO: validate there is one and only "objectgroup" layer

   // get first j["layers"] that is a ["type"] == "objectgroup"
   bool tilelayer_type_found = false;
   nlohmann::json tilelayer;
   for (auto &layer : j["layers"].items())
   {
      if (layer.value()["type"] == "objectgroup")
      {
         tilelayer = layer.value();
         tilelayer_type_found = true;
         break;
      }
   }
   if (!tilelayer_type_found)
   {
      // TODO: Swap this error message with AllegroFlare::Errors
      throw std::runtime_error("TMJObjectLoader: error: tilelayer type not found.");
   }


   return;
}

bool TMJObjectLoader::file_exists(std::string filename)
{
   return std::filesystem::exists(filename);
}


} // namespace KrampusReturns


