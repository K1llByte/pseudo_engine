# Pseudo Engine
Pseudo Engine is a 2D game engine build from scratch for game development and engine programming.

This engine will provide basic features to be able to start making basic 2D games.

### Features

- [x] ECS based architecture
- [x] Sprite rendering
- [x] Animation system
- [x] Physics system
- [x] Input mapping system
- [ ] Engine semantic versoning
- [ ] Particle system

## Building

> Note: Untested on Linux environments

```sh
cmake -B build/ -S . -G "Visual Studio 17 2022"
cmake --build build/
```