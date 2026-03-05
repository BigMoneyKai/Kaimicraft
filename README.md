# 🌍 Kaimicraft

A Minecraft-inspired voxel engine written in C++17 with OpenGL 3.3 Core Profile.

![C++](https://img.shields.io/badge/C++-17-blue?logo=c%2B%2B)
![OpenGL](https://img.shields.io/badge/OpenGL-3.3-green)
![CMake](https://img.shields.io/badge/CMake-3.26+-red)
![License](https://img.shields.io/badge/license-MIT-lightgrey)

---

## ✨ Features

- **Procedural terrain generation** using multi-octave Perlin noise (2D and 3D)
- **Dynamic chunk system** — chunks load and unload around the player in real time
- **Face culling** — only exposed block faces are rendered for performance
- **Texture atlas** — grass, dirt, and stone blocks with per-face UV mapping
- **First-person camera** with mouse look and keyboard movement
- **Fly mode** — move freely through the world
- **Fullscreen / borderless / windowed** display modes

---

## 📸 Block Types

| Block  | Description                   |
|--------|-------------------------------|
| Grass  | Surface layer, unique top/side/bottom textures |
| Dirt   | Layer beneath grass           |
| Stone  | Deep underground layer        |
| Air    | Empty space (not rendered)    |

---

## 🏗️ Architecture

```
src/
├── main.cpp              # Entry point
├── app.cpp / .h          # Application lifecycle (init, run, shutdown)
├── windowmanager         # GLFW window + display mode management
├── inputmanager          # Unified keyboard & mouse input
├── keyboard / mouse      # Per-frame key/button state tracking
├── camera                # First-person camera (view/projection matrices)
├── renderer              # OpenGL draw calls, shader binding
├── mesh                  # Vertex/index buffer management, chunk mesh builder
├── chunkmanager          # Chunk loading, unloading, dynamic updates
├── terraingenerator      # Heightmap generation via Perlin noise
├── perlinnoise           # 2D and 3D Perlin noise implementation
├── texturemanager        # STB image loading, texture atlas binding
├── tile / block          # Tile UV lookup, block type enum
├── shader                # GLSL shader compilation and linking
├── timemanager           # (Stub) Frame timing
├── util                  # Tile UV calculation, input control logic
└── debug                 # Colored terminal logging macros
```

---

## 🔧 Dependencies

| Library | Purpose | Included via |
|---------|---------|--------------|
| [GLFW](https://www.glfw.org/) | Window and input | `thirdparty/glfw` |
| [GLEW](https://glew.sourceforge.net/) | OpenGL extension loading | `thirdparty/glew` |
| [GLM](https://github.com/g-truc/glm) | Math (vectors, matrices) | `thirdparty/glm` |
| [stb_image](https://github.com/nothings/stb) | PNG/JPG texture loading | `thirdparty/stb` |

All dependencies are bundled as Git submodules — no external installs required.

---

## 🚀 Building

### Prerequisites

- CMake 3.26+
- A C++17-capable compiler (GCC, Clang, MSVC)
- Python 3 (for the helper build script)

On Linux, you may also need:

```bash
sudo apt install libx11-dev libxi-dev libxrandr-dev libgl-dev
```

### Clone with submodules

```bash
git clone --recurse-submodules https://github.com/yourname/minecraft.git
cd minecraft
```

### Build and run (via Python helper)

```bash
python run.py
```

This will clean, configure, build, and launch the game automatically.

### Manual CMake build

```bash
cmake -S . -B build
cmake --build build
./build/bin/minecraft
```

---

## 🎮 Controls

| Key / Input         | Action              |
|---------------------|---------------------|
| `W / A / S / D`     | Move forward / left / backward / right |
| `Space`             | Fly up              |
| `Left Shift`        | Fly down            |
| `Mouse`             | Look around         |
| `Scroll Wheel`      | Adjust field of view |
| `Escape`            | Quit                |

---

## 🌐 World Generation

Terrain height is computed using **fractional Brownian motion (fBm)** — summing multiple octaves of Perlin noise:

```
octaves    = 6
lacunarity = 2.0   (frequency multiplier per octave)
gain       = 0.5   (amplitude multiplier per octave)
baseHeight = 64
heightScale = 32
```

The world is divided into **16×16×16 block chunks**. The engine generates all chunks within a configurable radius (`genRadius = 64`) around the player, and automatically unloads chunks that move out of range.

---

## 🗂️ Assets

```
assets/
├── shader/
│   ├── vertshader.glsl   # Vertex shader — MVP transform + UV passthrough
│   └── fragshader.glsl   # Fragment shader — texture sampling
└── texture/
    └── atlas.png         # 256×256 texture atlas (16px tiles, 1px padding)
```

The texture atlas is laid out in a row of tiles:

| Index | Tile        |
|-------|-------------|
| 0     | Grass top   |
| 1     | Grass side  |
| 2     | Dirt        |
| 3     | Stone       |

---

## 📁 Project Structure

```
minecraft/
├── assets/               # Shaders and textures
├── src/                  # All C++ source files
├── thirdparty/           # GLFW, GLEW, GLM, stb (submodules)
├── CMakeLists.txt
├── run.py                # Build + run helper script
└── README.md
```

---

## 🐛 Debug Logging

The project includes a set of colored terminal macros in `debug.h`:

```cpp
INFO("Texture loaded: %s", path.c_str());     // ✅ Green
WARNING("Low memory");                         // ⚠️  Purple
ERROR("Shader compile failed");                // ❌ Red
FATAL("Cannot create window");                 // 💥 Dark red + exit
```

---

## 📝 License

MIT License — see [LICENSE](LICENSE) for details.GPL-3.0 License — see [LICENSE](LICENSE) for details.
