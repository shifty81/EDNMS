# EDNMS - Elite Dangerous + No Man's Sky Hybrid

> *A next-generation space exploration game combining realistic flight simulation with infinite procedural worlds, survival mechanics, and deep building systems.*

## 🚀 Overview

EDNMS merges the best elements of Elite Dangerous and No Man's Sky into a comprehensive space exploration experience:

- **Elite Dangerous Flight Model**: Precise 6DOF Newtonian physics and tactical combat
- **No Man's Sky Exploration**: Vibrant procedural planets with diverse biomes and creatures
- **Base Building**: Comprehensive construction on planets and space stations
- **Survival Mechanics**: Resource management, environmental hazards, and crafting
- **Multiplayer**: Seamless co-op and optional PvP

## 📚 Documentation

Comprehensive design documentation is available:

- **[GAME_DESIGN.md](GAME_DESIGN.md)** - Complete game design document with all features
- **[TECHNICAL_SPECS.md](TECHNICAL_SPECS.md)** - Technical implementation specifications
- **[ENGINE_ARCHITECTURE.md](ENGINE_ARCHITECTURE.md)** - Custom engine architecture (ECS, chunks, serialization, procedural generation)
- **[NARRATIVE_DESIGN.md](NARRATIVE_DESIGN.md)** - Story premise, narrative arcs, and Ember Reach walkthrough
- **[MODULAR_SHIPS.md](MODULAR_SHIPS.md)** - Modular ship system (generation, combat, DNA serialization, TITAN ships)
- **[FEATURE_MATRIX.md](FEATURE_MATRIX.md)** - Feature comparison and integration plan
- **[QUICKSTART.md](QUICKSTART.md)** - Development quick start guide
- **[ROADMAP.md](ROADMAP.md)** - Detailed development roadmap

## ✨ Key Features

### Space Flight & Combat
- 6 degrees of freedom realistic physics
- Power management system (weapons/shields/engines)
- 20+ ships from small fighters to large freighters
- Fixed, gimballed, and turreted weapons
- Heat management and stealth mechanics

### Planetary Exploration
- Infinite procedurally generated planets
- 15+ unique biomes (lush, toxic, frozen, scorched, exotic)
- Procedural flora and fauna to discover
- Dynamic weather and day/night cycles
- Seamless space-to-surface transitions

### Building Systems
- **On-Foot Building**: Modular base construction with 30+ pieces
- **Hangars**: Planetary and orbital hangars for physical ship storage and repair
- **Space Stations**: Orbital platform construction and management (X4-style)
- **Automation**: Factory-style production chains
- **Power Systems**: Solar, battery, and generator networks with graph-based simulation
- **Terraforming**: Multi-stage planetary transformation system
- **Teleportation**: Fast travel between bases

### Survival Mechanics
- Life support management (oxygen, temperature, radiation)
- Hunger, thirst, and fatigue systems
- Environmental hazards and extreme weather
- Resource gathering and crafting
- Multi-tool system (mining, scanning, combat)

### Economy & Progression
- Dynamic market economy with supply/demand
- Multiple income sources (trading, mining, missions)
- Ship upgrades and customization
- Technology research and blueprints
- Faction reputation system
- Physical logistics — no magic inventories, everything moves via ships/rovers/drones

### Narrative & Story
- The galaxy is recovering from a civilization collapse — not thriving
- Environmental storytelling through broken infrastructure and simulation state
- Player role: Systems Pioneer / Recovery Engineer
- Three major narrative arcs: The Terraforming Paradox, The Silent Infrastructure, Ownership vs Stewardship
- Story System 001: "Ember Reach" — first playable narrative sandbox

## 🎮 Game Modes

- **Solo**: Complete offline experience
- **Private Group**: Play with friends
- **Open Play**: Full multiplayer with PvP zones
- **Creative Mode**: Unlimited resources for building

## 🚢 Ship Assets

Modular OBJ ship models are included for use in the engine's asset pipeline:

| Asset Pack | Contents | Description |
|-----------|----------|-------------|
| `CruiserBattleshipModulePack.zip` | `core_m.obj`, `spine_m.obj`, `engine_block_m.obj`, `turret_m.obj`, `hangar_m.obj` | Large capital ship modules (cruiser/battleship class) with hardpoints for engines, weapons, spine connectors, and hangar bays |
| `ModularShipModulePack.zip` | `core_s.obj`, `engine_s.obj`, `weapon_s.obj`, `wing_s.obj` | Small modular ship modules with hardpoints for engines, weapons, and wing attachments |

Models use a hardpoint naming convention (`hp_` prefix) for attachment points, enabling modular ship assembly in the construction system. These serve as reference geometry and test placeholders — the procedural generation pipeline will generate equivalent module geometry at runtime. See [TECHNICAL_SPECS.md](TECHNICAL_SPECS.md) for the asset pipeline specification.

## 🛠️ Technology Stack

- **Engine**: Custom simulation-first engine (see [ENGINE_ARCHITECTURE.md](ENGINE_ARCHITECTURE.md))
- **Language**: C++17 (data-driven, ECS architecture)
- **Rendering**: OpenGL/Vulkan (minimal initially, upgraded over time)
- **Networking**: Client-server with P2P wings
- **Platform**: PC (Windows/Linux) at launch, consoles post-launch

### Core Philosophy
1. Simulation first, visuals second
2. Everything is data-driven
3. No system depends on rendering
4. If it can't be saved/loaded, it doesn't exist
5. One solar system must run headless
6. Everything is procedurally generated or AI-created — no pre-made assets

## 🗺️ Development Roadmap

### Phase 1: Prototype (Months 1-3)
- Basic flight mechanics
- Single planet generation
- Simple base building
- Core survival systems

### Phase 2: Alpha (Months 4-9)
- Multiple star systems
- Ship variety and combat
- Expanded building
- Economy system

### Phase 3: Beta (Months 10-15)
- Station building
- Multiplayer implementation
- Full galaxy generation
- Polish and optimization

### Phase 4: Launch (Month 16+)
- Public release
- Day-one content
- Community support
- Post-launch updates

See [ROADMAP.md](ROADMAP.md) for detailed milestones.

## 🎯 Unique Features

What sets EDNMS apart from other space games:

1. **Hybrid Flight System**: Choose between simulation and arcade modes
2. **Living Ecosystem**: Planets evolve based on player actions
3. **Mega-Structures**: Build Dyson spheres, orbital rings, space elevators
4. **Chronicle System**: AI-generated personal story missions
5. **Guild Forge**: Community-created ships and stations

## 🤝 Contributing

We welcome contributions! Please see:
- Code style guidelines
- Pull request process
- Testing requirements

## 📝 License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## 🌟 Credits

Inspired by:
- **Elite Dangerous** by Frontier Developments
- **No Man's Sky** by Hello Games
- Plus elements from Starfield, Star Citizen, Subnautica, and more

## 📞 Contact

- **Issues**: [GitHub Issues](https://github.com/shifty81/EDNMS/issues)
- **Discussions**: [GitHub Discussions](https://github.com/shifty81/EDNMS/discussions)

---

*Built with passion for space exploration, simulation depth, and player creativity.*
