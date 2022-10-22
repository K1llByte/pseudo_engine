#pragma once

// NOTE: Components must have at least a ctor
#include "pe/components/basic.hpp"
#include "pe/components/physics.hpp"
#include "pe/components/animation.hpp"


struct MovementController {
    float speed = 100.f;
    // InputConfiguration
    sf::Keyboard::Key up = sf::Keyboard::Up;
    sf::Keyboard::Key down = sf::Keyboard::Down;
    sf::Keyboard::Key left = sf::Keyboard::Left;
    sf::Keyboard::Key right = sf::Keyboard::Right;
    sf::Keyboard::Key sprint = sf::Keyboard::LShift;
};