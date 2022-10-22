#include "pe/app.hpp"

#include <thread>
#include <chrono>

#include <fmt/core.h>

float time_now() {
    static auto begin = std::chrono::high_resolution_clock::now();
    auto end = std::chrono::high_resolution_clock::now();
    // Subtract and convert from nanoseconds to seconds
    auto res = (end-begin).count() * 1.0e-9;
    begin = end;
    return res;
}

// float render_time_now() {
//     static auto start = std::chrono::high_resolution_clock::now();
//     auto finish = std::chrono::high_resolution_clock::now();
//     return (finish-start).count();
// }

namespace pe {

    void App::run() {
        // Render loop
        while((is_running = window.isOpen())) {
            // Get render loop delta time
            auto delta_time = time_now();
            window.setTitle(fmt::format("Bit Mayhem | {} fps", int(1/delta_time)));

            // 1. Process events //
            sf::Event event;
            while(window.pollEvent(event)) {
                // Close window: exit
                if(event.type == sf::Event::Closed)
                    window.close();
            }

            // 2. Update app //
            this->update(delta_time);

            // 3. Render app //
            this->render(delta_time);
            // Display framebuffer
            window.display();
        }
    }
}