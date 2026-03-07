# 🌍 Kaimicraft

A Minecraft-style voxel engine built with C++17 and OpenGL 3.3 Core Profile.

**✨ Highlights**
- 🗺️ Heightmap terrain from multi-octave 2D Perlin noise (6 octaves)
- 🔄 Dynamic chunk loading/unloading around the camera
- ✂️ Face-culling mesh generation (only visible faces are rendered)
- 🖼️ Texture atlas with per-face UVs (grass top/side, dirt, stone)
- 🎮 First-person camera with mouse look
- 🔍 Mouse wheel FOV zoom
- 🖥️ Fullscreen / borderless / windowed display modes (currently defaulting to fullscreen)

**🌍 Current World Rules**
- 🧱 Block types: `AIR`, `GRASS`, `DIRT`, `STONE`
- 📦 Chunk size: `16 x 16 x 16` (world data)
- 🌐 Generation radius: `genRadius = 64` (world-space units)
- 📷 Default camera: `FOV 70°`, `near 0.1`, `far 1000`

---

**🛠️ Build & Run**

Prerequisites
- ⚙️ CMake 3.26+
- 💻 C++17 compiler (GCC/Clang/MSVC)
- 🐍 Python 3 (for `run.py`)

Linux may need extra packages
```bash
sudo apt install libx11-dev libxi-dev libxrandr-dev libgl-dev
```

Clone with submodules
```bash
git clone --recurse-submodules https://github.com/yourname/kaimicraft.git
cd kaimicraft
```

One-shot build and run (cleans and rebuilds `build/`)
```bash
python run.py
```

Manual build
```bash
cmake -S . -B build
cmake --build build
./build/bin/kaimicraft
```

---

**🕹️ Controls**
- `W / A / S / D`: Move forward / left / back / right
- `Space`: Fly up ⬆️
- `Left Shift`: Fly down ⬇️
- Mouse move: Look around 👀
- Scroll wheel: Adjust FOV 🔭
- `Escape`: Quit 🚪

The cursor is hidden and captured on startup.

---

**📁 Project Structure**
```
kaimicraft/
├── assets/               # 🎨 Shaders and textures
├── src/                  # ⚙️ Engine source
├── thirdparty/           # 📦 GLFW, GLEW, GLM, stb (submodules)
├── CMakeLists.txt
├── run.py                # 🚀 Build + run helper
└── README.md
```

**🧩 Core Modules**
- `src/app.cpp`: App lifecycle and main loop
- `src/windowmanager.*`: Window and display modes
- `src/inputmanager.*`, `src/mouse.*`, `src/keyboard.*`: Input system
- `src/camera.*`: First-person camera and projection
- `src/chunkmanager.*`: Chunk generation, updates, culling
- `src/mesh.*`: Chunk meshing and face culling
- `src/terraingenerator.*`, `src/perlinnoise.*`: Noise-based terrain
- `src/texturemanager.*`, `src/tile.*`: Texture atlas and UVs
- `src/shader.*`: GLSL compile/link
- `src/debug.*`: Colored logging macros

---

**🖼️ Assets & Textures**
- 🗺️ Atlas: `assets/texture/atlas.png`
- 📐 Atlas size: `256 x 256`
- 🔲 Tile size: `16 px`, padding `1 px`
- 🧱 Tile indices: grass-top `0`, grass-side `1`, dirt `2`, stone `3`

---

**⚙️ Config (in code)**
- 🖥️ Display mode: `windowManager.init(title, FULLSCREEN)` in `src/app.cpp`
- 🌐 Generation radius: `genRadius` in `src/chunkmanager.h`
- 📷 Camera parameters: `src/camera.h`

---

**📝 Known Implementation Notes**
- ⚠️ Terrain height uses 2D noise; `getDensity()` (3D noise) is not wired into generation yet.
- 🚧 Current rendering is basic terrain + face culling; there is no block place/break logic yet.

---

**📄 License**
MIT License
