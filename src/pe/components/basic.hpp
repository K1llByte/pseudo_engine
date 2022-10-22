#pragma once

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>

#include "pe/actor.hpp"

struct Transform {
    glm::vec2 position;
    // Transform(Transform&&) = default;
};


struct SpriteRender {
    sf::Texture        texture;
    sf::RectangleShape rect;
    sf::IntRect        sub_sprite;

    SpriteRender(const sf::String& filename, float width, float height)
        : rect(sf::Vector2f(width, height))
    {
        if(!texture.loadFromFile(filename)) {
            std::exit(1);
        }
        texture.setRepeated(true);
        rect.setTexture(&texture);
        auto tex_size = texture.getSize();
        sub_sprite = sf::IntRect(0,0,tex_size.x,tex_size.y);
        rect.setTextureRect(sub_sprite);
    }
};

struct Camera {
    sf::View view;

    Camera(sf::FloatRect&& screen)
        : view(std::move(screen)) {}
};


struct RectRender {
    sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(40,40));
};


struct Script {
    pe::Actor* actor;
};


struct DebugComponent {
    size_t lines;
    size_t columns;
    // 0 is left, 1 is right
    int direction = 0;

    DebugComponent(size_t _lines, size_t _columns)
        : lines(_lines)
        , columns(_columns) {}
};