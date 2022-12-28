#include <ChatGPT/Teleporting.hpp>

#include <algorithm>
#include <random>

namespace ChatGPT
{
   Teleporting::Teleporting(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity, std::pair<float, float> origin)
      : entity(entity)
      , time_since_last_teleport(0)
      , random_engine(std::random_device()())
      , uniform_distribution(0, 5)
      , origin(origin)
   {
      for (int i=0; i<6; i++)
      {
         float angle = i * (3.1415f * 2.0f / 6.0f);
         teleport_locations.emplace_back(std::make_pair(std::cos(angle) * 100, std::sin(angle) * 100));
      }
   }

   Teleporting::~Teleporting()
   {}

   void Teleporting::update()
   {
      time_since_last_teleport += 1.0f / 60.0f;
      if (time_since_last_teleport >= 5)
      {
         time_since_last_teleport = 0;
         int random_location_index = uniform_distribution(random_engine);
         auto it = teleport_locations.begin();
         std::advance(it, random_location_index);
         entity->get_place_ref().position = { it->first + origin.first, it->second + origin.second };
      }
   }
}
