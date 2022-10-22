#pragma once

#include "pe/scene.hpp"
#include "pe/components.hpp"

// NOTE: Every actor needs to be Move constructible/assignable (so no references)

class CubeSpawner: public pe::Actor {
    public:
    // ECS Related
    pe::Entity entity;
    
    CubeSpawner(pe::Entity _entity)
        : entity(_entity) {}

    void update(float dt) override {
        static bool clicked = false;
        if(!clicked && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            clicked = true;
            auto registry = entity.handle.registry();
            auto bouncer = pe::Entity(*registry, registry->create());
            // Components of the new entity
            auto [px, py] = sf::Mouse::getPosition();
            bouncer.add<Transform>(glm::vec2{px,py});
            bouncer.add<RectRender>(sf::RectangleShape(sf::Vector2f(30,30)));
            bouncer.add<Rigidbody>(1.f).set_gravity(true);
            bouncer.add<BoxCollider>(glm::vec2(30,30));

        }
        else {
            clicked = false;
        }
    }
};