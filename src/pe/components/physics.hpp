#pragma once

#include "pe/components/basic.hpp"

struct Rigidbody {
    float     mass;
    glm::vec2 velocity;
    glm::vec2 force;
    // Elasticity
    float     restitution = 0.f;
    bool      gravity = true;

    Rigidbody& set_gravity(bool torf) { 
        gravity = torf;
        return *this;
    }

    Rigidbody& add_force(glm::vec2 _force) {
        force += mass * _force;
        return *this;
    }

    Rigidbody& apply_impulse(glm::vec2 _force) {
        force += mass * _force;
        return *this;
    }
};


struct BoxCollider {
    glm::vec2 size;

    // AA Box collision
    bool test_collision(const Transform& self_t, const BoxCollider& collidee, const Transform& collidee_t) {
        const float l1 = self_t.position.x - size.x/2;
        const float r1 = self_t.position.x + size.x/2;
        const float t1 = self_t.position.y - size.y/2;
        const float b1 = self_t.position.y + size.y/2;

        const float l2 = collidee_t.position.x - collidee.size.x/2;
        const float r2 = collidee_t.position.x + collidee.size.x/2;
        const float t2 = collidee_t.position.y - collidee.size.y/2;
        const float b2 = collidee_t.position.y + collidee.size.y/2;

        return !(b1 < t2 || t1 > b2 || r1 < l2 || l1 > r2);
    }
    // on_colision callback

    
    static void resolve_collision(Rigidbody& a, Rigidbody& b, const glm::vec2& collision_normal) {
        // Minimum restitution (simple approach)
        auto e = std::min(a.restitution, b.restitution);

        // Relative velocity
        auto vrel = a.velocity - b.velocity;

        // Compute impulse magnitude
        float impulse_magnitude = -(1 + e) * glm::dot(vrel, collision_normal) / ((1/a.mass) + (1/b.mass));

        // Apply collision impulse
        auto collision_impulse = impulse_magnitude * collision_normal;
        
        a.apply_impulse(collision_impulse);
        // b.apply_impulse(-collision_impulse);
    }
    
};

// struct CircleCollider {
//     float radius;
// };