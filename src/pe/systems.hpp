#pragma once

#include <type_traits>
#include <tuple>

#include "pe/components.hpp"
#include "pe/scene.hpp"

// TODO: Remove the arguments of the System ctor and make a singleton monostate to save GameData with everything there
// Or any better alternative to that

namespace pe {
    class System {
        protected:
        pe::Scene& scene;
        sf::RenderWindow& window;

        public:   
        System(pe::Scene& _s, sf::RenderWindow& _win)
            : scene(_s), window(_win) {}
        virtual void update(float dt) = 0;
    };


    template<typename... Systems>
        // requires (std::is_base_of_v<pe::System,Systems> && ...)
    struct SystemGroup {
        std::tuple<Systems...> systems;

        template<typename... Args>
        SystemGroup(pe::Scene& scene, sf::RenderWindow& win)
            : systems{Systems(scene,win)...} {}

        void update(float dt) {
            std::apply([&dt](Systems& ...ta) {
                (ta.update(dt) , ...);
            }, systems);
        }
    };

    /////////////// Systems implementations ///////////////

    struct ScriptSystem: public System {
        ScriptSystem(pe::Scene& _scene, sf::RenderWindow& _win)
            : System(_scene,_win) {}

        void update(float dt) override {
            static auto view = scene.view<Script>();
            view.each([&](auto& s) {
                // This is the only system that uses vtable
                s.actor->update(dt);
            });
        }
    };


    struct RenderSystem: public System {
        RenderSystem(pe::Scene& _scene, sf::RenderWindow& _win)
            : System(_scene,_win) {}

        void update(float dt) override {
            window.clear(sf::Color(57, 60, 74, 255));

            // Get main camera
            auto& camera = scene.get_main_camera();
            if(camera.is_valid()) {
                window.setView(camera.get<Camera>().view);
            }
            else {
                window.setView(window.getDefaultView());
            }

            // Render all Rect shapes
            static auto rect_view = scene.view<Transform, RectRender>();
            rect_view.each([&](auto& t, auto& r) {
                auto [w,h] = r.rect.getSize();
                r.rect.setPosition(t.position.x - w/2, t.position.y - h/2);
                window.draw(r.rect);
            });

            // Animate sprites
            // static auto anim_view = scene.view<DebugComponent, SpriteRender>();
            // static float time_counter = 0.f;
            
            // time_counter += dt;
            // anim_view.each([&](auto& d, auto& s) {
            //     int anim_idx = int(time_counter*5) % 4;
            //     if(d.direction == 1) {
            //         s.sub_sprite = sf::IntRect(16*anim_idx,16,16,16);
            //     }
            //     else {
            //         s.sub_sprite = sf::IntRect(16*(anim_idx+1),16,-16,16);
            //     }
            // });
            // Render all Sprites
            static auto sprite_view = scene.view<Transform, SpriteRender>();
            sprite_view.each([&](auto& t, auto& s) {
                // Draw texture to rectangle shape
                auto [w,h] = s.rect.getSize();
                s.rect.setPosition(t.position.x - w/2, t.position.y - h/2);
                s.rect.setTextureRect(s.sub_sprite);
                window.draw(s.rect);
            });
        }
    };


    struct MovementSystem: public System {
        MovementSystem(pe::Scene& _scene, sf::RenderWindow& _win)
            : System(_scene,_win) {}

        void update(float dt) override {
            static auto view = scene.view<Transform, MovementController>();
            view.each([&](auto& t, auto& m) {
                float SPEED = 100.f + 150.f * sf::Keyboard::isKeyPressed(m.sprint);
                const float x_speed = dt * (sf::Keyboard::isKeyPressed(m.right) * SPEED - sf::Keyboard::isKeyPressed(m.left) * SPEED);
                const float y_speed = dt * (sf::Keyboard::isKeyPressed(m.down)  * SPEED - sf::Keyboard::isKeyPressed(m.up)  * SPEED);
                t.position.x += x_speed;
                t.position.y += y_speed;
            });
        }
    };


    struct PhysicsSystem: public System {
        PhysicsSystem(pe::Scene& _scene, sf::RenderWindow& _win)
            : System(_scene,_win) {}

        void update(float dt) override {

            // Dynamics
            static auto dynamics_view = scene.view<Transform, Rigidbody>();
            dynamics_view.each([&](auto& t, auto& r) {
                if(r.gravity)
                    r.force += r.mass * glm::vec2(0.f,300.f);
                r.velocity += r.force / r.mass * dt;
                t.position += r.velocity * dt;
                r.force = glm::vec2(0.f);
            });

            // Collision detection
            static auto colliders_view = scene.view<Transform, BoxCollider>();
            colliders_view.each([&](auto entity1, auto& t1, auto& c1) {
                // Box to Box collition detection
                colliders_view.each([&](auto entity2, auto& t2, auto& c2) {
                    if(entity1 == entity2) return;
                    if(c1.test_collision(t1,c2,t2)) {
                        // Try calling on_collision callback
                        // TODO: On collision callback

                        // Collision resolution
                        // If any of the entities doesn't have a Rigidbody then we won't do this step 
                        auto body1_ptr = scene.try_get<Rigidbody>(entity1);
                        auto body2_ptr = scene.try_get<Rigidbody>(entity2);
                        if(body1_ptr && body2_ptr) {
                            // TODO: Compute collision normal
                            glm::vec2 collision_normal = glm::normalize(t1.position - t2.position);
                            // Restore collision positioning
                            // Collision intersection size
                            glm::vec2 cis;
                            // FIXME: Branchless programming here pls
                            const auto v1 = body1_ptr->velocity;
                            const auto v2 = body2_ptr->velocity;
                            float factor1x = (v1.x + v2.x) ? v1.x / (v1.x + v2.x) : 0.f;
                            float factor1y = (v1.y + v2.y) ? v1.y / (v1.y + v2.y) : 0.f;
                            float factor2x = (v1.x + v2.x) ? v2.x / (v1.x + v2.x) : 0.f;
                            float factor2y = (v1.y + v2.y) ? v2.y / (v1.y + v2.y) : 0.f;
                            if(t1.position.x < t2.position.x) {
                                cis.x = glm::abs((t1.position.x + c1.size.x / 2) - (t2.position.x - c2.size.x / 2));
                                t1.position.x -= factor1x * cis.x;
                                t2.position.x += factor2x * cis.x;
                            }
                            else { // if t1.x >= t2.x
                                cis.x = glm::abs((t1.position.x - c1.size.x / 2) - (t2.position.x + c2.size.x / 2));
                                t1.position.x += factor1x * cis.x;
                                t2.position.x -= factor2x * cis.x;
                            }
                            if(t1.position.y < t2.position.y) {
                                cis.y = glm::abs((t1.position.y + c1.size.y / 2) - (t2.position.y - c2.size.y / 2));
                                t1.position.y -= factor1y * cis.y;
                                t2.position.y += factor2y * cis.y;
                            }
                            else { // if t1.y >= t2.y
                                cis.y = glm::abs((t1.position.y - c1.size.y / 2) - (t2.position.y + c2.size.y / 2));
                                t1.position.y += factor1y * cis.y;
                                t2.position.y -= factor2y * cis.y;
                            }
                            
                            // TODO: Call restitution impulse solver
                            // BoxCollider::resolve_collision(*body1_ptr, *body2_ptr, collision_normal);

                            // DEBUG:
                            // body1_ptr->velocity = glm::vec2{0.f};
                            // body2_ptr->velocity = glm::vec2{0.f};
                            // body1_ptr->force = glm::vec2{0.f};
                            // body2_ptr->force = glm::vec2{0.f};
                            // body1_ptr->gravity = false;
                            // body2_ptr->gravity = false;
                        }                        

                        // Draw collision bounding boxes
                        sf::RectangleShape bb1(sf::Vector2f(c1.size.x,c1.size.y));
                        bb1.setFillColor(sf::Color::Transparent);
                        bb1.setOutlineThickness(1.5f);
                        bb1.setOutlineColor(sf::Color(87, 166, 83));
                        bb1.setPosition(t1.position.x - c1.size.x/2, t1.position.y - c1.size.y/2);
                        window.draw(bb1);
                        sf::RectangleShape bb2(sf::Vector2f(c2.size.x,c2.size.y));
                        bb2.setFillColor(sf::Color::Transparent);
                        bb2.setOutlineThickness(1.5f);
                        bb2.setOutlineColor(sf::Color(87, 166, 83));
                        bb2.setPosition(t2.position.x - c2.size.x/2, t2.position.y - c2.size.y/2);
                        window.draw(bb2);
                    }
                });
            });
        }
    };

    
    struct AnimationSystem: public System {
        AnimationSystem(pe::Scene& _scene, sf::RenderWindow& _win)
            : System(_scene,_win) {}

        void update(float dt) override {
            static auto view = scene.view<SpriteRender, SpriteAnimator>();
            view.each([&](auto& r, auto& a) {
                a.update(dt);
                auto _slice = a.get_active_sprite();
                r.sub_sprite = sf::IntRect(_slice.x, _slice.y, _slice.width, _slice.height);
            });
        }
    };
}