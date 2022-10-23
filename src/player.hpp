#pragma once

#include "pe/variant.hpp"
#include "pe/scene.hpp"

// struct Idle {};
// struct Walking {};
// struct Running {};
// struct Emote {};
// using PlayerState = pe::Variant<Idle,Walking,Running,Emote>;

// NOTE: Every actor needs to be Move constructible/assignable (so no references)

class Player: public pe::Actor {
    public:
    // ECS Related
    pe::Entity    entity;
    Transform*    transform;
    Rigidbody*    rigidbody;
    SpriteRender* sprite_render;
    SpriteAnimator* sprite_animator;

    // Player data
    // PlayerState state = Idle{};
    bool direction_right = false;
    bool is_running = false;
    bool idle = true;
    bool try_emote = false;
    
    Player(pe::Entity _entity)
        : entity(_entity)
        , transform{&entity.get<Transform>()}
        , sprite_render{&entity.get<SpriteRender>()}
        , sprite_animator{&entity.get<SpriteAnimator>()}
        , rigidbody{&entity.get<Rigidbody>()}
        {}

    void update(float dt) override {
        ////////////////////////////
        //        Movement        //
        ////////////////////////////
        is_running = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift);
        float SPEED = 100.f + 150.f * is_running;
        const float x_speed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) * SPEED - sf::Keyboard::isKeyPressed(sf::Keyboard::Left) * SPEED;
        const float y_speed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down)  * SPEED - sf::Keyboard::isKeyPressed(sf::Keyboard::Up)  * SPEED;
        rigidbody->velocity = glm::vec2{x_speed, y_speed};
        // transform->position.x += x_speed;
        // transform->position.y += y_speed;
        if(x_speed < 0) direction_right = false;
        if(x_speed > 0) direction_right = true;
        idle = !(x_speed || y_speed);
        try_emote = sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && idle;

        ////////////////////////////
        //        Animation       //
        ////////////////////////////
        sprite_animator->flip_horizontal(!direction_right);
        if(idle) {
            // 0 is Idle, 3 is Emote
            sprite_animator->set_active(3*try_emote);
        }
        else {
            // 1 is Walking, 2 is Running
            sprite_animator->set_active(1 + is_running);
        }
    }
};