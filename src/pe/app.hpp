#pragma once

#include <SFML/Graphics.hpp>

#include <fmt/core.h>

#define PE_APP_MAIN(app_class) int main() { app_class ## ().run(); return 0; }

namespace pe {
    // The extended App must:
    // - Create a window
    // - Create game state
    // - Initialize entities
    // - Setup systems entities
    struct App {
        App() = default;
        App(sf::VideoMode win_video_mode, const sf::String& win_title)
            : window(win_video_mode, win_title) {}
        void run();
        virtual void update(float dt) {};
        virtual void render(float dt) {};

        protected:
        sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800, 600), "PseudoEngine App");
        bool is_running = true;
    };
}