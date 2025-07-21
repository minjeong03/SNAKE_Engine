# SNAKE Engine

SNAKE Engine is a lightweight 2D game engine written in modern C++, designed for performance, scalability, and educational use. It features a clean architecture, efficient rendering pipeline, and automatic optimizations that make it suitable for rendering tens of thousands of objects per frame.

---

## Features

### Rendering
- Custom render layers with name-based registration (up to 16 layers supported)
- Automatic optimization pipeline
  - Batch submission with lambda-based draw calls

### State Management
- Flexible `GameState` system with overridable `Load`, `Init`, `LateInit`, `Update`, `LateUpdate`, `Draw`, `Free`, and `Unload` methods
- `StateManager` handles transitions and active state updates

### Memory Management
- Users do **not** need to manually allocate or deallocate memory for engine-managed objects
- `ObjectManager`, `RenderManager`, and `StateManager` take care of lifetime and cleanup of all core components
- Prevents memory leaks and dangling pointers by using smart ownership patterns internally
