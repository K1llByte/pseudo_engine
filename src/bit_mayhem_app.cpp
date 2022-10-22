#include <fmt/core.h>
#include <entt/entt.hpp>

#include "pe/app.hpp"
#include "pe/systems.hpp"
#include "player.hpp"
#include "spawner.hpp"

struct BitMayhem: public pe::App {
    pe::Scene scene;
    // Systems
    pe::SystemGroup<
        pe::ScriptSystem,
        pe::AnimationSystem,
        pe::RenderSystem,
        pe::MovementSystem,
        pe::PhysicsSystem
    > systems;

    BitMayhem() 
        : App(sf::VideoMode(800,800), "Bit Mayhem")
        , systems(scene, window)
    {
        // auto camera = create_camera(scene, 0,0,300,300);
        // scene.set_main_camera(&camera);
        auto camera = scene.create_entity();
        camera.add<Camera>(sf::FloatRect(0,0,400,400));
        // camera.add<CubeSpawner>(camera);
        scene.set_main_camera(camera);
        
        // Player
        // auto player = scene.create_entity();
        // player.add<Transform>(glm::vec2{300, 300});
        // player.add<SpriteRender>("assets/textures/monkey_tileset_16.png",40.f,40.f);
        // // player.add<MovementController>();
        // player.add<SpriteAnimator>(64,48,4,3)
        //     // 0: Idle
        //     .add_animation({{0, 0.1f}, {1, 0.1f}})
        //     // 1: Walking
        //     .add_animation({{4,0.1f}, {5,0.1f}, {6,0.1f}, {7,0.1f}})
        //     // 2: Running
        //     .add_animation({{8,0.1f}, {9,0.1f}, {10,0.1f}, {11,0.1f}})
        //     // 3: Emote
        //     .add_animation({{2, 0.1f}})
        //     .set_active(0);
        // player.add<Player>(player);
        // player.add<BoxCollider>(glm::vec2(27,36));
        // // player.add<Rigidbody>(100.f).set_gravity(true);//.add_force(glm::vec2{0,100});

        // auto block_spawner = scene.create_entity();
        // block_spawner.add<Transform>(glm::vec2{200, 200});


        auto bouncer = scene.create_entity();
        bouncer.add<Transform>(glm::vec2{200, 200});
        bouncer.add<RectRender>(sf::RectangleShape(sf::Vector2f(30,30)));
        auto& body1 = bouncer.add<Rigidbody>(1.f).set_gravity(true);
        body1.restitution = 1.f;
        // body.velocity = {0.f, 10.f};
        bouncer.add<BoxCollider>(glm::vec2(30,30));

        auto plane = scene.create_entity();
        plane.add<Transform>(glm::vec2{200, 300});
        plane.add<RectRender>(sf::RectangleShape(sf::Vector2f(100,40)));
        auto& body2 = plane.add<Rigidbody>(1.f).set_gravity(false);
        plane.add<BoxCollider>(glm::vec2(100,40));
    }
    
    void update(float dt) override {
        systems.update(dt);
    }
};

PE_APP_MAIN(BitMayhem)