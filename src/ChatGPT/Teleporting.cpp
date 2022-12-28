#include "ChatGPT/Teleporting.hpp"
#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>
#include <cstdlib>
#include <ctime>

namespace ChatGPT
{
    // Constructor
    Teleporting::Teleporting(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity)
        : entity_(entity)
    {
        // Initialize teleport locations
        teleport_locations_ = {
            std::pair<int, int>{50, 50},
            std::pair<int, int>{25, 75},
            std::pair<int, int>{75, 75},
            std::pair<int, int>{50, 25},
            std::pair<int, int>{25, 25},
            std::pair<int, int>{75, 25}
        };

        // Seed the random number generator
        srand(time(NULL));

        // Initialize timer
        timer_ = 0;
        interval_ = 5.0f; // Teleport every 5 seconds
    }

    // Teleport the enemy to a random location from the predetermined list of teleport locations
    void Teleporting::teleport()
    {
        int index = rand() % NUM_TELEPORT_LOCATIONS;
        entity_->get_place_ref().position.x = teleport_locations_[index].first;
        entity_->get_place_ref().position.y = teleport_locations_[index].second;
    }

    // Update the enemy's position based on its speed
    void Teleporting::update()
    {
        // Update timer
        timer_ += 1.0f / 60.0f;

        // Check if it's time to teleport
        if (timer_ >= interval_)
        {
            // Reset timer
            timer_ = 0;

            // Teleport enemy
            teleport();
        }
    }

    // Accessor methods
    AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* Teleporting::get_entity() const { return entity_; }

    // Mutator methods
    void Teleporting::set_entity(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity) { entity_ = entity; }
}
