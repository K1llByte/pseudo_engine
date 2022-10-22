#pragma once

#include "pe/scene.hpp"
#include "pe/components.hpp"

#include <glm/glm.hpp>

pe::Entity spawn_bullet(pe::Scene& scene, glm::vec2 direction, float speed) {
    auto player = scene.create_entity();
        player.add_component<Transform>(glm::vec2{300, 300});
        player.add_component<RectRender>();
        player.add_component<Rigidbody>(100.f);
    return player;
}