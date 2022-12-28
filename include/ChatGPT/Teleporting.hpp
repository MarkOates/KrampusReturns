#ifndef CHATGPT_TELEPORTING_HPP
#define CHATGPT_TELEPORTING_HPP

#include <utility>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/MovementStrategies2D/Base.hpp>
#include <AllegroFlare/Prototypes/Platforming2D/Entities/FrameAnimated2D.hpp>

namespace ChatGPT
{
    // Forward declaration
    //class AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D;

    // Class definition
    class Teleporting : public AllegroFlare::Prototypes::Platforming2D::Entities::MovementStrategies2D::Base
    {
    private:
        // Private constants
        static const int NUM_TELEPORT_LOCATIONS = 6;

        // Private member variables
        AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity_;
        std::vector<std::pair<int, int>> teleport_locations_;
        float timer_;
        float interval_;

        // Private member functions
        void teleport();

    public:
        // Constructor
        Teleporting(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity);

        // Update the enemy's position based on its speed
        virtual void update();

        // Accessor methods
        AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* get_entity() const;

        // Mutator methods
        void set_entity(AllegroFlare::Prototypes::Platforming2D::Entities::FrameAnimated2D* entity);
    };
}

#endif
