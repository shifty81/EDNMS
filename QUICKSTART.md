# EDNMS Quick Start Development Guide
## Getting Started with Development

---

## 0. Hello World — Build & Run in 60 Seconds

Before diving into the full development guide, verify your environment works:

```bash
# Clone and enter the repo
git clone https://github.com/shifty81/EDNMS.git
cd EDNMS

# Build everything
cmake -B build
cmake --build build

# Run the engine (creates a ship entity, prints to console)
./build/EDNMS

# Run the test suite (31 tests)
./build/EDNMSTests
```

**Expected engine output:**
```
[INFO]  EDNMS Engine starting...
[INFO]  Ship entity created (ID: 1)
[INFO]  Ship position: (0.000000, 0.000000, 0.000000)
[INFO]  Ship mass: 1000.000000 kg
[INFO]  EDNMS Engine initialized. Entities: 1
[INFO]  Ready for development. See QUICKSTART.md for next steps.
```

**Expected test output:**
```
EDNMS Test Suite
================
  PASS: ECS.CreateEntity
  PASS: ECS.DestroyEntity
  ... (31 tests)
31 passed, 0 failed, 31 total
```

**Troubleshooting:**

| Problem | Solution |
|---------|----------|
| `cmake` not found | Install CMake 3.20+ from https://cmake.org |
| C++17 errors | Update your compiler: GCC 10+, Clang 12+, or MSVC 2022 |
| Linker errors | Clean build: `rm -rf build && cmake -B build && cmake --build build` |
| Tests fail | Open a [bug report](https://github.com/shifty81/EDNMS/issues) with your OS/compiler info |

If you see the output above, you're ready to develop! Continue below for the full 12-week prototype plan.

---

## 1. Project Setup

### 1.1 Prerequisites

**Required Software**:
- C++17 compliant compiler (GCC 10+, Clang 12+, or MSVC 2022)
- CMake 3.20+ (build system)
- Git (version control)
- OpenGL 4.5+ or Vulkan SDK (rendering)

**Recommended Tools**:
- Visual Studio Code or CLion (C++ development)
- GPU profiling tools (RenderDoc, NVIDIA Nsight)
- Custom C++/OpenGL UI system (GUI, HUD, and debug overlays built from scratch)

### 1.2 Repository Structure

```
EDNMS/
├── Engine/                   # Custom engine core
│   ├── Core/                 # App lifecycle, time step, logging
│   ├── ECS/                  # Entity Component System
│   ├── Math/                 # Double-precision vectors, floating origin
│   ├── IO/                   # Binary serialization, chunked saves
│   ├── Jobs/                 # Task/job system
│   └── Platform/             # Platform abstraction
├── Simulation/               # Engine-agnostic simulation layer
│   ├── World/                # World hierarchy, chunk streaming
│   ├── Survival/             # O2, temperature, radiation, health
│   ├── Power/                # Power network graphs
│   ├── Logistics/            # Physical resource transport
│   ├── Construction/         # Staged building system
│   └── Ownership/            # Faction/system ownership
├── Game/                     # Game-specific logic
│   ├── Player/               # Input, camera, UI glue
│   ├── Ships/                # Ship entities and modules
│   ├── Bases/                # Base entities
│   └── Stations/             # Station entities
├── Renderer/                 # Minimal renderer (OpenGL/Vulkan)
├── Tools/                    # Development tools
│   ├── WorldEditor/          # In-engine world editing
│   └── AssetPipeline/        # OBJ import, LOD generation
├── Data/                     # Data-driven definitions
│   ├── Definitions/          # Entity/component definitions
│   ├── Blueprints/           # Construction blueprints
│   └── ShipModules/          # OBJ ship module packs
├── Tests/                    # Unit and integration tests
├── CruiserBattleshipModulePack.zip  # Large ship OBJ modules
├── ModularShipModulePack.zip        # Small ship OBJ modules
└── CMakeLists.txt            # Build system root
```

---

## 2. Core Systems Implementation Order

### Phase 1: Prototype (Months 1-3)
**Goal**: Prove core mechanics work

#### Week 1-2: Flight System
```cpp
// Priority: Get a ship flying in space
1. Set up ECS registry + core components
2. Create ship entity with Physics + Transform
3. Implement 6DOF thrust system
4. Camera system (basic follow cam)
5. Debug HUD (custom OpenGL UI)
```

#### Week 3-4: Single Planet
```cpp
// Priority: Land on one procedurally generated planet
1. Sphere mesh planet
2. Simple terrain generation
3. Collision detection
4. Seamless landing transition
5. On-foot movement
```

#### Week 5-6: Basic Building
```cpp
// Priority: Place one structure type
1. Building mode toggle
2. Foundation placement
3. Snap-to-grid system
4. Persist buildings to save
5. Delete placed structures
```

#### Week 7-8: Simple Survival
```cpp
// Priority: Resource gathering and basic needs
1. Multi-tool mining
2. Inventory system
3. Oxygen meter
4. Resource nodes on planet
5. Basic crafting (1-2 items)
```

#### Week 9-10: Ship Upgrades
```cpp
// Priority: One upgrade path
1. Ship loadout screen
2. Better thruster module
3. Cargo capacity upgrade
4. Module swapping system
5. Save loadout
```

#### Week 11-12: Polish & Demo
```cpp
// Priority: Playable vertical slice
1. Tutorial mission
2. Performance optimization
3. Bug fixes
4. Visual polish
5. Demo build
```

**Deliverable**: 15-minute gameplay demo showing flight, landing, building, and survival

---

### Phase 2: Alpha (Months 4-9)
**Goal**: Expand content and systems

#### Months 4-5: Multiple Star Systems
- Galaxy generation algorithm
- Star system procedural generation
- FTL travel system
- Navigation map
- Save exploration data

#### Months 6-7: Ship Variety & Combat
- 10 different ship models
- Weapon systems (3 types)
- Shield system
- Basic AI enemies
- Power distribution

#### Months 8-9: Expanded Building & Economy
- 30+ building pieces
- Power system for bases
- Basic trading posts
- NPC traders
- Simple economy simulation

**Deliverable**: Alpha build with 20+ hours of content

---

### Phase 3: Beta (Months 10-15)
**Goal**: Full feature set and multiplayer

#### Months 10-11: Station Building
- Orbital placement system
- Modular station pieces
- Station economy
- NPC crew system
- Defense platforms

#### Months 12-13: Multiplayer
- Client-server architecture
- Player synchronization
- Instancing system
- Shared bases
- PvP zones

#### Months 14-15: Polish & Optimization
- Performance tuning
- Bug fixing
- Balance adjustments
- Quality-of-life features
- Beta testing

**Deliverable**: Feature-complete game ready for external testing

---

## 3. Development Milestones

### 3.1 Critical Path Features

**Milestone 1: "First Flight"** (Week 2)
- [x] Ship flies with keyboard/mouse
- [x] Camera follows ship
- [x] Basic space environment
- [x] Can accelerate/decelerate

**Milestone 2: "First Landing"** (Week 4)
- [x] Generate single planet
- [x] Transition from space to surface
- [x] Walk on planet surface
- [x] Return to ship and take off

**Milestone 3: "First Build"** (Week 6)
- [x] Place foundation piece
- [x] Place wall piece
- [x] Place door
- [x] Enter built structure

**Milestone 4: "First Survival Loop"** (Week 8)
- [x] Mine resource
- [x] Add to inventory
- [x] Craft item
- [x] Consume item for benefit

**Milestone 5: "First Ship Upgrade"** (Week 10)
- [x] Earn credits
- [x] Purchase module
- [x] Install module
- [x] Notice improvement

**Milestone 6: "Vertical Slice Complete"** (Week 12)
- [x] All core systems working
- [x] 15 minutes of gameplay
- [x] No game-breaking bugs
- [x] Demo-ready build

---

## 4. Technical Implementation Tips

> **Note**: Code examples below use illustrative APIs. The actual implementation uses the custom ECS architecture described in [ENGINE_ARCHITECTURE.md](ENGINE_ARCHITECTURE.md). Unreal-style naming in examples is for readability; real code will use the engine's ECS components and systems.

### 4.1 Flight System Best Practices

```cpp
// Use relative forces for thrust
void AShip::ApplyThrust(FVector Direction, float Power)
{
    FVector WorldDirection = GetActorRotation().RotateVector(Direction);
    FVector Force = WorldDirection * ThrustPower * Power;
    ShipMesh->AddForce(Force);
}

// Implement flight assist for new players
void AShip::FlightAssist(float DeltaTime)
{
    if (!bFlightAssistEnabled) return;
    
    // Dampen rotation
    FVector AngularVel = ShipMesh->GetPhysicsAngularVelocityInDegrees();
    FVector DampenTorque = -AngularVel * RotationDamping;
    ShipMesh->AddTorqueInDegrees(DampenTorque);
    
    // Dampen lateral movement
    FVector Velocity = ShipMesh->GetPhysicsLinearVelocity();
    FVector LocalVel = GetActorRotation().UnrotateVector(Velocity);
    FVector LateralVel = FVector(LocalVel.X, LocalVel.Y, 0);
    FVector DampenForce = -LateralVel * LateralDamping;
    ApplyThrust(DampenForce, 1.0f);
}
```

### 4.2 Procedural Generation Best Practices

```cpp
// Always use seeds for consistency
class UPlanetGenerator : public UObject
{
private:
    FRandomStream RandomStream;
    
public:
    FPlanetData GeneratePlanet(int64 Seed)
    {
        // Initialize with seed
        RandomStream.Initialize(Seed);
        
        FPlanetData Planet;
        Planet.Radius = RandomStream.FRandRange(5000.0f, 15000.0f);
        Planet.Gravity = RandomStream.FRandRange(0.5f, 2.0f);
        Planet.BiomeType = GetRandomBiome();
        
        return Planet;
    }
    
    // Same seed always produces same planet
    EBiomeType GetRandomBiome()
    {
        float Value = RandomStream.FRand();
        if (Value < 0.2f) return EBiomeType::Lush;
        if (Value < 0.4f) return EBiomeType::Desert;
        if (Value < 0.6f) return EBiomeType::Frozen;
        if (Value < 0.8f) return EBiomeType::Toxic;
        return EBiomeType::Barren;
    }
};
```

### 4.3 Building System Best Practices

```cpp
// Use snap points for easy building
class ABuildingPiece : public AActor
{
public:
    UPROPERTY(EditAnywhere)
    TArray<USceneComponent*> SnapPoints;
    
    bool CanSnapTo(ABuildingPiece* Other, int32 SnapIndex)
    {
        if (SnapIndex >= SnapPoints.Num()) return false;
        
        FVector SnapLocation = SnapPoints[SnapIndex]->GetComponentLocation();
        
        // Check if another piece is close to this snap point
        for (USceneComponent* OtherSnap : Other->SnapPoints)
        {
            FVector OtherLocation = OtherSnap->GetComponentLocation();
            float Distance = FVector::Distance(SnapLocation, OtherLocation);
            
            if (Distance < SnapTolerance)
            {
                return true;
            }
        }
        
        return false;
    }
};
```

### 4.4 Performance Optimization Tips

```cpp
// LOD based on distance
void APlanetActor::UpdateLOD(FVector PlayerPosition)
{
    float Distance = FVector::Distance(GetActorLocation(), PlayerPosition);
    
    int32 LODLevel;
    if (Distance < 100.0f)
        LODLevel = 0;  // Highest detail
    else if (Distance < 500.0f)
        LODLevel = 1;
    else if (Distance < 2000.0f)
        LODLevel = 2;
    else if (Distance < 10000.0f)
        LODLevel = 3;
    else
        LODLevel = 4;  // Lowest detail
    
    PlanetMesh->SetLOD(LODLevel);
}

// Use object pooling for frequent spawns
class AProjectilePool : public AActor
{
private:
    TArray<AProjectile*> AvailableProjectiles;
    
public:
    AProjectile* GetProjectile()
    {
        if (AvailableProjectiles.Num() > 0)
        {
            AProjectile* Projectile = AvailableProjectiles.Pop();
            Projectile->SetActorHiddenInGame(false);
            Projectile->SetActorEnableCollision(true);
            return Projectile;
        }
        
        // Create new if pool empty
        return GetWorld()->SpawnActor<AProjectile>();
    }
    
    void ReturnProjectile(AProjectile* Projectile)
    {
        Projectile->SetActorHiddenInGame(true);
        Projectile->SetActorEnableCollision(false);
        AvailableProjectiles.Add(Projectile);
    }
};
```

---

## 5. Testing Strategy

### 5.1 Unit Tests

```cpp
// Example: Test planet generation consistency
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FPlanetGenerationTest,
    "EDNMS.Generation.Planet.Consistency",
    EAutomationTestFlags::ApplicationContextMask | 
    EAutomationTestFlags::ProductFilter
)

bool FPlanetGenerationTest::RunTest(const FString& Parameters)
{
    UPlanetGenerator* Generator = NewObject<UPlanetGenerator>();
    
    // Same seed should produce same planet
    FPlanetData Planet1 = Generator->GeneratePlanet(12345);
    FPlanetData Planet2 = Generator->GeneratePlanet(12345);
    
    TestEqual("Radius should match", Planet1.Radius, Planet2.Radius);
    TestEqual("Biome should match", Planet1.BiomeType, Planet2.BiomeType);
    
    return true;
}
```

### 5.2 Integration Tests

```cpp
// Example: Test full gameplay loop
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FGameplayLoopTest,
    "EDNMS.Gameplay.BasicLoop",
    EAutomationTestFlags::ApplicationContextMask | 
    EAutomationTestFlags::ProductFilter
)

bool FGameplayLoopTest::RunTest(const FString& Parameters)
{
    // Setup
    APlayerShip* Ship = SpawnTestShip();
    APlanet* Planet = SpawnTestPlanet();
    
    // Test: Fly ship
    Ship->ApplyThrust(FVector(0, 0, 1), 1.0f);
    AdvanceTime(1.0f);
    TestTrue("Ship should be moving", Ship->GetVelocity().Size() > 0);
    
    // Test: Land on planet
    Ship->SetActorLocation(Planet->GetSurfaceLocation());
    Ship->ExitShip();
    APlayerCharacter* Player = Ship->GetPilot();
    TestTrue("Player should be on foot", Player->IsOnFoot());
    
    // Test: Mine resource
    AResourceNode* Node = SpawnResourceNode();
    Player->MineResource(Node);
    TestEqual("Should have iron", Player->GetInventory()->GetItemCount("Iron"), 10);
    
    return true;
}
```

### 5.3 Performance Tests

```cpp
// Benchmark terrain generation
IMPLEMENT_SIMPLE_AUTOMATION_TEST(
    FTerrainPerformanceTest,
    "EDNMS.Performance.TerrainGeneration",
    EAutomationTestFlags::ApplicationContextMask | 
    EAutomationTestFlags::ProductFilter
)

bool FTerrainPerformanceTest::RunTest(const FString& Parameters)
{
    UTerrainGenerator* Generator = NewObject<UTerrainGenerator>();
    
    double StartTime = FPlatformTime::Seconds();
    
    // Generate 100 terrain chunks
    for (int32 i = 0; i < 100; i++)
    {
        FVector Position = FVector(i * 1000, 0, 0);
        Generator->GenerateChunk(Position, 2); // LOD 2
    }
    
    double EndTime = FPlatformTime::Seconds();
    double Duration = (EndTime - StartTime) * 1000.0; // milliseconds
    
    // Should take less than 1 second
    TestTrue("Terrain generation should be fast", Duration < 1000.0);
    
    return true;
}
```

---

## 6. Common Pitfalls & Solutions

### 6.1 Flight System Issues

**Problem**: Ship feels floaty or unresponsive
**Solution**: Increase mass, reduce dampening, add flight assist

**Problem**: Camera shakes during flight
**Solution**: Use spring arm with lag, smooth camera rotation

**Problem**: Ship spins out of control
**Solution**: Cap angular velocity, increase rotational dampening

### 6.2 Procedural Generation Issues

**Problem**: Planets look too similar
**Solution**: Increase variation in biome parameters, add unique features

**Problem**: Terrain has seams between chunks
**Solution**: Use overlapping generation, blend edges

**Problem**: Performance drops on planet surface
**Solution**: Aggressive LOD, limit draw distance, reduce polygon count

### 6.3 Building System Issues

**Problem**: Pieces don't align properly
**Solution**: Use snap points, increase snap tolerance, add visual feedback

**Problem**: Structures fall through terrain
**Solution**: Ensure proper collision, use foundation anchors

**Problem**: Complex bases cause lag
**Solution**: Limit piece count, use instanced static meshes, spatial partitioning

### 6.4 Multiplayer Issues

**Problem**: Desync between clients
**Solution**: Authoritative server, client prediction, lag compensation

**Problem**: High bandwidth usage
**Solution**: Delta compression, prioritize updates, limit sync frequency

**Problem**: Player position teleporting
**Solution**: Interpolation, extrapolation, smooth position updates

---

## 7. Debugging Tools

### 7.1 In-Game Console Commands

```cpp
// Teleport to planet
TeleportToPlanet <PlanetID>

// Spawn ship
SpawnShip <ShipModel>

// Give resources
GiveResource <ResourceName> <Amount>

// Toggle god mode
God

// Toggle noclip
Ghost

// Show debug info
ShowDebug Flight
ShowDebug Building
ShowDebug Survival
ShowDebug Network

// Performance stats
Stat FPS
Stat Unit
Stat Memory
```

### 7.2 Visual Debug Helpers

```cpp
// Draw debug shapes
void AShip::DrawDebugInfo()
{
    // Draw velocity vector
    FVector Start = GetActorLocation();
    FVector End = Start + GetVelocity() * 10.0f;
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 0.1f, 0, 5.0f);
    
    // Draw thrust direction
    FVector ThrustDir = GetActorForwardVector() * ThrustPower;
    DrawDebugLine(GetWorld(), Start, Start + ThrustDir, FColor::Red, false, 0.1f, 0, 5.0f);
    
    // Draw collision bounds
    DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(),
                 FColor::Yellow, false, 0.1f);
}
```

---

## 8. Resources & Documentation

### 8.1 Learning Resources

**Custom Engine & ECS**:
- See [ENGINE_ARCHITECTURE.md](ENGINE_ARCHITECTURE.md) for full architecture details
- "Game Engine Architecture" by Jason Gregory (comprehensive reference)
- EnTT ECS library documentation (reference ECS implementation)

**Procedural Generation**:
- "Procedural Generation in Game Design" book
- Sebastian Lague's Procedural Planet series (YouTube)
- GPU Gems chapters on terrain generation

**Game Design**:
- "The Art of Game Design" by Jesse Schell
- "Rules of Play" by Katie Salen and Eric Zimmerman
- GDC talks on Elite Dangerous and No Man's Sky

### 8.2 Useful Plugins

**Recommended Unreal Marketplace**:
- Advanced Locomotion System (character movement)
- Easy Multi-Save (save system)
- Voxel Plugin (terrain deformation)
- Cesium for Unreal (planet-scale rendering)

**Free Open Source**:
- FMOD or Wwise (audio)
- Steam SDK (multiplayer)
- Discord SDK (social features)

---

## 9. Team Structure (Recommended)

### 9.1 Core Team (Minimum Viable)

**Programmer (2-3)**:
- Lead: Architecture, systems design
- Gameplay: Core mechanics implementation
- Tools: Editor tools, automation

**Artist (2)**:
- 3D: Ships, structures, characters
- VFX: Particles, shaders, effects

**Designer (1-2)**:
- Systems: Balance, progression
- Content: Missions, planets, items

**Producer (1)**:
- Project management
- Scheduling
- Stakeholder communication

### 9.2 Extended Team (Full Production)

Add:
- Network Engineer (multiplayer)
- Sound Designer (audio)
- UI/UX Designer (interface)
- QA Tester (2-3)
- Community Manager (1)
- Marketing (1)

---

## 10. Next Steps

### 10.1 Week 1 Action Items

**Day 1**: Setup
- [ ] Install Unreal Engine 5
- [ ] Clone repository
- [ ] Create new UE5 project
- [ ] Setup version control

**Day 2-3**: First Ship
- [ ] Create ship blueprint
- [ ] Add physics component
- [ ] Implement thrust input
- [ ] Test in empty level

**Day 4-5**: First Flight
- [ ] Add camera system
- [ ] Create space environment
- [ ] Add HUD elements
- [ ] Polish movement feel

**Weekend**: Review
- [ ] Playtest first build
- [ ] Document issues
- [ ] Plan Week 2

### 10.2 Success Criteria

By end of Week 1, you should have:
- ✓ Ship that responds to WASD input
- ✓ Camera that follows ship smoothly
- ✓ Ability to rotate ship with mouse
- ✓ Basic HUD showing speed
- ✓ No critical bugs

If you achieve this, you're on track for the 12-week prototype!

---

## 11. Contact & Support

### 11.1 Getting Help

**GitHub**: Post issues with [Question] tag
**Discord**: Join EDNMS development server
**Email**: dev@ednms.game (for private inquiries)

### 11.2 Contributing

See CONTRIBUTING.md for:
- Code style guide
- Pull request process
- Testing requirements
- Documentation standards

---

## Conclusion

This quick start guide provides a practical roadmap for building EDNMS. The key is to:

1. **Start Small**: Get one system working perfectly
2. **Iterate Fast**: Weekly builds, constant testing
3. **Stay Focused**: Don't add features until core works
4. **Test Early**: Performance and bugs compound quickly
5. **Document Everything**: Future you will thank present you

Remember: Elite Dangerous took 10 years, No Man's Sky took 4 years. This is a marathon, not a sprint. Build the foundation right, and the rest will follow.

**First Milestone**: Working vertical slice in 12 weeks
**Next Review**: After Week 12 prototype demo

Good luck, Commander. o7
