#pragma once

#include <entt/entt.hpp>

#include "pe/actor.hpp"
#include "pe/components/basic.hpp"

namespace pe {
    class Entity {
        public:
        entt::handle handle;

        public:
        Entity()
            : handle() {}
        Entity(entt::registry& reg, entt::entity ent)
            : handle(reg, ent) {}

        template<typename Component, typename... Args>
        Component& add(Args&&... args) {
            // Component reference
            auto& ref = handle.emplace<Component>(std::forward<Args>(args)...);
            if constexpr (std::is_base_of_v<pe::Actor,Component>) {
                handle.emplace<Script>(&ref);
            }
            return ref;
        }

        template<typename ...Components>
        decltype(auto) get() {
            return handle.get<Components...>();
        }

        // TODO: remove component

        bool is_valid() const { return handle.valid(); }
    };


    struct Scene: public entt::registry {
        Entity main_camera;
        
        Entity create_entity() {
            entt::entity entity = this->create();
            return Entity(*this, entity);
        }

        void set_main_camera(const Entity& cam) {
            main_camera = cam;
        }

        Entity& get_main_camera() {
            return main_camera;
        }
    };
}