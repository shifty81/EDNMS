# EDNMS — Elite Dangerous × No Man's Sky Hybrid

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](LICENSE)

**EDNMS** is an open-source space exploration game that fuses Elite Dangerous' precise 6DOF flight model with No Man's Sky's vibrant procedural universe — then adds deep on-foot base building, orbital station construction, physical logistics, and real survival mechanics. Everything in the game — planets, ships, creatures, textures — is procedurally generated at runtime from seeds: no pre-made assets, no loading screens between space and surface.

The project is in **early prototype** stage. The simulation-first custom engine (C++17, ECS architecture) compiles and passes tests today, and we're working toward a playable 15-minute vertical slice. Contributions of all kinds are welcome — see [CONTRIBUTING.md](CONTRIBUTING.md) to get started.

---

## Table of Contents

- [Quick Start](#-quick-start)
- [Project Structure](#-project-structure)
- [Key Features](#-key-features)
- [Technology Stack](#️-technology-stack)
- [Documentation](#-documentation)
- [Development Roadmap](#️-development-roadmap)
- [Contributing](#-contributing)
- [License](#-license)
- [Credits](#-credits)

---

## 🚀 Quick Start

### Prerequisites

| Tool | Version | Purpose |
|------|---------|---------|
| C++ compiler | GCC 10+, Clang 12+, or MSVC 2022 | C++17 support required |
| CMake | 3.20+ | Build system |
| Git | Any recent | Version control |

### Build & Run

```bash
# Clone the repository
git clone https://github.com/shifty81/EDNMS.git
cd EDNMS

# Configure and build
cmake -B build
cmake --build build

# Run the engine (creates a ship entity, prints to console)
./build/EDNMS

# Run the test suite (31 tests)
ctest --test-dir build --output-on-failure
```

### Expected Output

**Engine:**
```
[INFO]  EDNMS Engine starting...
[INFO]  Ship entity created (ID: 1)
[INFO]  Ship position: (0.000000, 0.000000, 0.000000)
[INFO]  Ship mass: 1000.000000 kg
[INFO]  EDNMS Engine initialized. Entities: 1
[INFO]  Ready for development. See QUICKSTART.md for next steps.
```

**Tests:**
```
EDNMS Test Suite
================
  PASS: ECS.CreateEntity
  PASS: ECS.DestroyEntity
  ...
31 passed, 0 failed, 31 total
```

### Troubleshooting

| Problem | Solution |
|---------|----------|
| `cmake` not found | Install CMake 3.20+ from https://cmake.org |
| C++17 errors | Update your compiler: GCC 10+, Clang 12+, or MSVC 2022 |
| Linker errors | Run a clean build: `rm -rf build && cmake -B build && cmake --build build` |

See [QUICKSTART.md](QUICKSTART.md) for the full 12-week development guide.

---

## 📁 Project Structure

```
EDNMS/
├── Engine/                   # Custom engine core
│   ├── Core/                 # App lifecycle, logging
│   ├── ECS/                  # Entity Component System (registry, components)
│   ├── Math/                 # Double-precision Vec3d, Quatd
│   ├── IO/                   # Binary serialization, chunk format
│   ├── Jobs/                 # Task/job system (planned)
│   └── Platform/             # Platform abstraction (planned)
├── Simulation/               # Engine-agnostic simulation layer
│   ├── World/                # Chunk streaming, world hierarchy
│   ├── Survival/             # O2, temperature, radiation (planned)
│   ├── Power/                # Power network graphs (planned)
│   ├── Logistics/            # Physical resource transport (planned)
│   ├── Construction/         # Staged building (planned)
│   └── Ownership/            # Faction/system ownership (planned)
├── Game/                     # Game-specific logic + entry point
├── Renderer/                 # Minimal renderer (planned)
├── Tests/                    # Unit and integration tests
├── Data/                     # Data-driven definitions (planned)
├── docs/                     # Extended documentation
│   ├── architecture/         # Engine & system architecture docs
│   └── features/             # Feature proposals & design docs
├── CMakeLists.txt            # Build system root
├── CONTRIBUTING.md           # How to contribute
├── CODE_OF_CONDUCT.md        # Community guidelines
├── QUICKSTART.md             # 12-week development guide
├── ROADMAP.md                # Milestone timeline
└── *.md                      # Design documents
```

**Key architectural rule:** Simulation does NOT depend on Engine. Game does NOT depend on Renderer. The simulation layer is engine-agnostic and can run headless.

---

## ✨ Key Features

### Space Flight & Combat
- 6DOF Newtonian physics with optional flight assist
- Power management (weapons / shields / engines)
- Modular ship hardpoints (fixed, gimballed, turreted)

### Procedural Universe
- Seed-based galaxy, planet, creature, and flora generation
- 15+ biomes with dynamic weather and day/night cycles
- Seamless space-to-surface transitions

### Building & Construction
- On-foot modular bases (30+ piece types)
- Orbital station construction (X4-style)
- Power networks, automation, terraforming
- Physical logistics — everything moves via ships/rovers/drones

### Survival
- Life support: oxygen, temperature, radiation, health
- Resource gathering, crafting, hunger/thirst/fatigue
- Environmental hazards and extreme weather

### Economy & Narrative
- Dynamic supply/demand markets
- Faction reputation and ownership systems
- AI-generated chronicle missions
- Post-collapse recovery narrative ("Ember Reach")

---

## 🛠️ Technology Stack

| Layer | Technology | Details |
|-------|-----------|---------|
| Language | C++17 | Data-driven, no scripting until systems stabilize |
| Architecture | Custom ECS | Entity Component System with POD components |
| Build | CMake 3.20+ | Static libraries per layer |
| Rendering | OpenGL/Vulkan | Minimal initially, upgraded over time |
| Math | Double-precision | Space-scale coordinates (Vec3d, Quatd) |
| Platform | PC (Win/Linux) | Console ports post-launch |

### Core Philosophy
1. Simulation first, visuals second
2. Everything is data-driven
3. No system depends on rendering
4. If it can't be saved/loaded, it doesn't exist
5. One solar system must run headless
6. Everything is procedurally generated — no pre-made assets

---

## 📚 Documentation

| Document | Description |
|----------|-------------|
| [QUICKSTART.md](QUICKSTART.md) | 12-week prototype guide with weekly milestones |
| [ENGINE_ARCHITECTURE.md](ENGINE_ARCHITECTURE.md) | ECS, chunks, serialization, procedural generation pipeline |
| [TECHNICAL_SPECS.md](TECHNICAL_SPECS.md) | Module specs, networking, performance targets |
| [GAME_DESIGN.md](GAME_DESIGN.md) | Complete feature set (73 features from 8 games) |
| [FEATURE_MATRIX.md](FEATURE_MATRIX.md) | Priority matrix and integration plan |
| [MODULAR_SHIPS.md](MODULAR_SHIPS.md) | Ship DNA, modular assembly, TITAN ships |
| [NARRATIVE_DESIGN.md](NARRATIVE_DESIGN.md) | Story arcs and Ember Reach walkthrough |
| [ROADMAP.md](ROADMAP.md) | 18-month timeline with team scaling |

---

## 🗺️ Development Roadmap

| Phase | Timeline | Goal | Status |
|-------|----------|------|--------|
| **Prototype** | Months 1–3 | Flight → Landing → Building → Survival (15-min demo) | 🟡 In Progress |
| **Alpha** | Months 4–9 | Multiple systems, combat, economy (20+ hrs content) | ⬜ Planned |
| **Beta** | Months 10–15 | Stations, multiplayer, full galaxy | ⬜ Planned |
| **Launch** | Month 16+ | Public release + post-launch support | ⬜ Planned |

See [ROADMAP.md](ROADMAP.md) for week-level breakdowns and milestones.

---

## 🤝 Contributing

We welcome contributions of all kinds — code, docs, bug reports, feature ideas, and playtesting feedback!

1. Read [CONTRIBUTING.md](CONTRIBUTING.md) for the full guide
2. Check [open issues](https://github.com/shifty81/EDNMS/issues) — look for `good-first-issue` labels
3. Fork → branch → code → test → PR

Please follow our [Code of Conduct](CODE_OF_CONDUCT.md).

---

## 📝 License

This project is licensed under the **GNU General Public License v3.0** — see [LICENSE](LICENSE) for details.

## 🌟 Credits

Inspired by **Elite Dangerous** (Frontier Developments), **No Man's Sky** (Hello Games), and elements from Starfield, Star Citizen, Subnautica, Satisfactory, EVE Online, and Minecraft.

## 📞 Contact

- **Issues**: [GitHub Issues](https://github.com/shifty81/EDNMS/issues)
- **Discussions**: [GitHub Discussions](https://github.com/shifty81/EDNMS/discussions)

---

*Built with passion for space exploration, simulation depth, and player creativity. o7*
