#include <ChatGPT/Teleporting.hpp>

#include <algorithm>
#include <random>

namespace ChatGPT
{
   Teleporting::Teleporting(
         AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity,
         std::pair<float, float> origin,
         std::pair<float, float> area_width_height
   )
      : entity(entity)
      , time_since_last_teleport(0)
      , random_engine(std::random_device()())
      , uniform_distribution(0, 5)
      , origin(origin)
      , area_width_height(area_width_height)
      , previous_random_location_index(0)
   {
      for (int i=0; i<6; i++)
      {
         float angle = i * (3.1415f * 2.0f / 6.0f);
         teleport_locations.emplace_back(std::make_pair(
            std::cos(angle) * area_width_height.first * 0.5,
            std::sin(angle) * area_width_height.second * 0.5
         ));
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
         int attempts = 0;
         while (random_location_index == previous_random_location_index)
         {
            random_location_index = uniform_distribution(random_engine);
            attempts++;
            if (attempts > 100) break;
         }
         previous_random_location_index = random_location_index;
         auto it = teleport_locations.begin();
         std::advance(it, random_location_index);
         entity->get_place_ref().position = { it->first + origin.first, it->second + origin.second };
      }
   }
}
