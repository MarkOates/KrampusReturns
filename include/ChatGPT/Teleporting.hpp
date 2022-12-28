#ifndef CHATGPT_TELEPORTING_HPP
#define CHATGPT_TELEPORTING_HPP

#include <AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/Base.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>

#include <random>
#include <utility>
#include <vector>


namespace ChatGPT
{
   class Teleporting : public AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base
   {
   private:
      std::vector<std::pair<float, float>> teleport_locations;
      std::pair<float, float> origin;
      std::default_random_engine random_engine;
      std::uniform_int_distribution<int> uniform_distribution;
      AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity;
      float time_since_last_teleport;

   public:
      Teleporting(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity, std::pair<float, float> origin);
      virtual ~Teleporting();

      virtual void update() override;
   };
}

#endif
