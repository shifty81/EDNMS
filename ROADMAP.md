# EDNMS Development Roadmap
## Detailed Timeline & Milestones

---

## Executive Summary

This roadmap outlines the development journey from prototype to launch and beyond. The project is structured in phases, each with clear deliverables and success metrics.

**Total Development Time**: 18-24 months to launch
**Team Size**: 8-12 core team members
**Budget Estimate**: $2-4M (indie scale)

---

## Phase 1: Prototype (Months 1-3)

### Goal
Prove core mechanics work together and create a playable vertical slice demonstrating the game's potential.

### Month 1: Foundation

#### Week 1-2: Engine Core & Flight System
**Deliverables**:
- [ ] ECS registry + core components (Transform, Physics, Survival, Power)
- [ ] Ship entity with 6DOF physics system
- [ ] Keyboard/mouse input handling
- [ ] Camera system with smooth following
- [ ] Basic debug HUD via ImGui (speed, heading)
- [ ] Flight assist toggle

**Success Metrics**:
- Ship feels responsive (< 50ms input lag)
- Flight controls are intuitive for new players
- Camera never clips through geometry

**Team**: 2 programmers

#### Week 3-4: Planet Generation
**Deliverables**:
- [ ] Procedural sphere planet
- [ ] Simple terrain generation (1 biome)
- [ ] Collision detection on surface
- [ ] LOD system for terrain
- [ ] Atmosphere shader

**Success Metrics**:
- Planet generates in < 5 seconds
- Terrain looks varied and interesting
- Performance stays above 30 FPS

**Team**: 1 programmer, 1 artist

### Month 2: Core Gameplay

#### Week 5-6: Landing & On-Foot
**Deliverables**:
- [ ] Seamless space-to-surface transition
- [ ] Third-person character controller
- [ ] Enter/exit ship mechanic
- [ ] Basic animations (walk, run, jump)
- [ ] Gravity transition system

**Success Metrics**:
- Landing feels natural (no jarring transitions)
- Character movement is smooth
- No physics glitches

**Team**: 2 programmers, 1 animator

#### Week 7-8: Building System & Hangars
**Deliverables**:
- [ ] Building mode toggle
- [ ] 5 structure types (foundation, wall, roof, door, window)
- [ ] Placement validation via construction staging system
- [ ] Snap-to-grid system
- [ ] Delete/move placed pieces
- [ ] Basic planetary hangar (Light Pad) with ship docking

**Success Metrics**:
- Can build simple shelter in < 5 minutes
- Pieces snap together properly
- No floating structures

**Team**: 1 programmer, 1 artist

### Month 3: Survival & Polish

#### Week 9-10: Survival Mechanics & Persistence
**Deliverables**:
- [ ] Multi-tool with mining beam
- [ ] Resource nodes on planet
- [ ] Inventory system (20 slots)
- [ ] Oxygen meter with depletion
- [ ] Simple crafting (5 recipes)
- [ ] Chunk binary save/load system (world persistence)

**Success Metrics**:
- Resource gathering feels rewarding
- Oxygen creates mild tension without being tedious
- Crafting UI is clear and intuitive

**Team**: 1 programmer, 1 UI designer

#### Week 11-12: Vertical Slice Polish
**Deliverables**:
- [ ] Tutorial mission (15 minutes)
- [ ] Sound effects for all actions
- [ ] Music (ambient space, planet)
- [ ] Visual polish pass
- [ ] Bug fixing
- [ ] Demo build for pitching

**Success Metrics**:
- Can play through demo without bugs
- Visuals are impressive enough for marketing
- Clear game loop: fly → land → gather → build → craft

**Team**: Full team

### Phase 1 Deliverable
**"Proof of Concept" Demo**:
- 15-minute vertical slice on custom engine
- Shows ECS, chunk streaming, and core gameplay working together
- Headless simulation can run independently of renderer
- Used for investor pitches/crowdfunding
- Public alpha announcement trailer

---

## Phase 2: Alpha (Months 4-9)

### Goal
Expand content variety and implement core systems at scale.

### Month 4-5: Galaxy & Multiple Systems

**Deliverables**:
- [ ] Galaxy generator (1 million+ systems)
- [ ] Star system generator (multiple planets per system)
- [ ] 5 biome types (lush, desert, frozen, toxic, barren)
- [ ] FTL travel system (hyperdrive)
- [ ] Galaxy map UI
- [ ] Star map navigation

**Success Metrics**:
- Can travel between 10+ systems
- Each planet feels unique
- Galaxy map is readable and useful
- Load times < 5 seconds per system

**Team**: 2 programmers, 1 UI designer

### Month 6: Ship Variety

**Deliverables**:
- [ ] 10 ship models (3 small, 4 medium, 3 large)
- [ ] Distinct ship handling per class
- [ ] Ship purchase system
- [ ] Ship loadout customization
- [ ] Ship storage/garage

**Success Metrics**:
- Each ship feels different to fly
- Clear progression path (small → large)
- All ships balanced for their role

**Team**: 1 programmer, 2 3D artists

### Month 7: Combat System

**Deliverables**:
- [ ] 3 weapon types (laser, cannon, missile)
- [ ] Shield system with regeneration
- [ ] Power distribution (SYS/ENG/WEP)
- [ ] Basic AI enemy ships
- [ ] Combat missions

**Success Metrics**:
- Combat is engaging and skill-based
- Power management creates tactical depth
- AI provides appropriate challenge

**Team**: 2 programmers

### Month 8: Economy & Trading

**Deliverables**:
- [ ] Market system with dynamic prices
- [ ] 20+ commodities to trade
- [ ] Trading posts on planets
- [ ] Space stations with markets
- [ ] Trade routes calculator
- [ ] Cargo hold management

**Success Metrics**:
- Can earn credits through trading
- Prices respond to player actions
- Trading is profitable but balanced

**Team**: 1 programmer, 1 designer

### Month 9: Expanded Building

**Deliverables**:
- [ ] 30+ building pieces
- [ ] Power system (generators, batteries, wiring)
- [ ] Storage containers
- [ ] Refiners for material processing
- [ ] Landing pads
- [ ] Teleporter network

**Success Metrics**:
- Can build complex bases
- Power system is understandable
- Automation feels rewarding

**Team**: 1 programmer, 1 artist

### Phase 2 Deliverable
**"Alpha Build"**:
- 20+ hours of content
- Full gameplay loop with progression
- Closed alpha testing (100 players)
- Stress test systems
- Gather feedback for balance

---

## Phase 3: Beta (Months 10-15)

### Goal
Implement all remaining features, add multiplayer, and polish to release quality.

### Month 10-11: Station Building

**Deliverables**:
- [ ] Orbital placement system
- [ ] 10 station module types
- [ ] Station power/life support
- [ ] NPC crew system
- [ ] Station economy (buy/sell from station)
- [ ] Defense turrets

**Success Metrics**:
- Can build functional station in < 1 hour
- Station generates passive income
- Stations attract NPC traffic

**Team**: 2 programmers, 1 artist

### Month 12: Multiplayer Foundation

**Deliverables**:
- [ ] Client-server architecture
- [ ] Player synchronization
- [ ] Instancing system (32 players)
- [ ] Wing system (4 players co-op)
- [ ] Chat system
- [ ] Friend list

**Success Metrics**:
- Stable with 32 concurrent players
- Latency < 100ms acceptable
- No major desync issues

**Team**: 2 network programmers, 1 backend

### Month 13: Multiplayer Features

**Deliverables**:
- [ ] Shared base permissions
- [ ] PvP zones
- [ ] Player trading
- [ ] Squadron system (guilds)
- [ ] Voice chat integration
- [ ] Griefing prevention

**Success Metrics**:
- Multiplayer enhances experience
- Can play cooperatively without issues
- PvP is balanced and optional

**Team**: 2 programmers, 1 designer

### Month 14: Content & Polish

**Deliverables**:
- [ ] 15+ biome types
- [ ] 100+ craftable items
- [ ] Procedural creature generation
- [ ] Quest system (50+ missions)
- [ ] Faction system
- [ ] Reputation mechanics

**Success Metrics**:
- Content feels varied and deep
- Always something new to discover
- Clear long-term goals

**Team**: Full team

### Month 15: Optimization & Bug Fixing

**Deliverables**:
- [ ] Performance optimization pass
- [ ] Fix all critical bugs
- [ ] Fix all major bugs
- [ ] Balance pass on economy
- [ ] Balance pass on combat
- [ ] Accessibility features

**Success Metrics**:
- 60 FPS on recommended specs
- < 10 known bugs at launch
- Positive beta tester feedback

**Team**: Full team + QA

### Phase 3 Deliverable
**"Beta Build"**:
- Feature complete
- Open beta (1000+ players)
- Marketing campaign begins
- Pre-orders open
- Final polish based on feedback

---

## Phase 4: Launch (Month 16-18)

### Month 16: Pre-Launch

**Deliverables**:
- [ ] Day-one patch ready
- [ ] Server infrastructure tested
- [ ] Marketing materials finalized
- [ ] Press review builds sent
- [ ] Steam page optimized
- [ ] Community management ready

**Success Metrics**:
- Zero critical bugs
- Servers can handle launch load
- Positive preview coverage

**Team**: Full team + marketing

### Month 17: Launch Week

**Deliverables**:
- [ ] Game launches on Steam
- [ ] Day-one patch deployed
- [ ] Community support active
- [ ] Monitor server stability
- [ ] Hotfix critical issues
- [ ] Launch trailer live

**Success Metrics**:
- Successful launch (no major issues)
- Positive review scores (>70%)
- Strong sales in first week

**Team**: Full team + support

### Month 18: Post-Launch Support

**Deliverables**:
- [ ] Week 1 hotfix patch
- [ ] Week 2 balance patch
- [ ] Month 1 content update
- [ ] Community event
- [ ] Roadmap for Year 2
- [ ] Bug fixes ongoing

**Success Metrics**:
- Player retention > 50% at month 1
- Positive sentiment in community
- Growing player base

**Team**: Full team

---

## Post-Launch Roadmap (Year 2)

### Quarter 1 (Months 19-21)
**Focus**: Stability & Quality of Life

**Features**:
- [ ] Major bug fixes
- [ ] Performance improvements
- [ ] UI/UX improvements
- [ ] Balance adjustments
- [ ] Quality of life features
- [ ] First major content drop

### Quarter 2 (Months 22-24)
**Focus**: Depth Systems

**Features**:
- [ ] Factory automation (Satisfactory-style)
- [ ] Advanced manufacturing
- [ ] Ground vehicles
- [ ] Underwater exploration
- [ ] Submersible vehicles

### Quarter 3 (Months 25-27)
**Focus**: Creative & Community

**Features**:
- [ ] Ship designer tool
- [ ] Creative mode
- [ ] Blueprint sharing
- [ ] Community marketplace
- [ ] Modding API (beta)

### Quarter 4 (Months 28-30)
**Focus**: Endgame Content

**Features**:
- [ ] Mega-structures (Dyson spheres)
- [ ] Territory control
- [ ] Large-scale PvP events
- [ ] Procedural story missions
- [ ] Boss encounters

---

## Major DLC Plans

### DLC 1: "The Depths" (Month 24)
**Price**: $19.99
**Content**:
- Full underwater exploration system
- 5 new ocean planet types
- Submersible vehicles and bases
- Underwater creatures and plants
- Deep sea mining

### DLC 2: "Architects" (Month 30)
**Price**: $24.99
**Content**:
- Ship designer tool
- Station designer
- Mega-structure construction
- Advanced automation
- Factory building

### DLC 3: "First Contact" (Month 36)
**Price**: $19.99
**Content**:
- Alien civilization encounters
- Diplomacy system
- New alien ships to pilot
- Alien technology to research
- Story campaign

---

## Platform Expansion

### Console Ports (Month 24-30)

**PlayStation 5**:
- Adapt controls for DualSense
- Optimize for PS5 hardware
- Trophy system
- PS+ integration

**Xbox Series X/S**:
- Adapt controls for Xbox controller
- Optimize for Xbox hardware
- Achievement system
- Game Pass consideration

**Technical Challenges**:
- Memory optimization for consoles
- Control scheme adaptation
- Cross-platform play
- Certification process

---

## Success Metrics by Phase

### Prototype (Month 3)
- ✓ Playable 15-minute demo
- ✓ Core systems functional
- ✓ Positive playtester feedback
- ✓ Investor interest / crowdfunding success

### Alpha (Month 9)
- ✓ 20+ hours of content
- ✓ 100 alpha testers
- ✓ Average play session > 2 hours
- ✓ 80% would recommend to friend

### Beta (Month 15)
- ✓ Feature complete
- ✓ 1000+ beta testers
- ✓ 60+ FPS on recommended specs
- ✓ < 10 critical bugs
- ✓ Positive review sentiment

### Launch (Month 18)
- ✓ Smooth launch (no game-breaking issues)
- ✓ 10,000+ sales in first month
- ✓ 70+ Metacritic score
- ✓ 50% player retention at month 1
- ✓ Positive community sentiment

### Year 1 Complete (Month 30)
- ✓ 100,000+ total sales
- ✓ Active community (1000+ daily players)
- ✓ 2+ major content updates
- ✓ Profitable enough for continued development
- ✓ Established for long-term support

---

## Risk Assessment & Mitigation

### Technical Risks

**Risk**: Performance issues with procedural generation
**Mitigation**: 
- Aggressive optimization from day one
- LOD systems implemented early
- Regular performance testing
- Scalable quality settings

**Risk**: Multiplayer synchronization problems
**Mitigation**:
- Use proven networking middleware
- Extensive multiplayer testing
- Graceful degradation for poor connections
- P2P for small groups, servers for hubs

**Risk**: Save corruption or data loss
**Mitigation**:
- Cloud save backups
- Local save backups
- Save validation systems
- Recovery tools for corrupt saves

### Design Risks

**Risk**: Game becomes too grindy
**Mitigation**:
- Extensive playtesting
- Multiple progression paths
- Balance between challenge and fun
- Respect player time

**Risk**: Players find game overwhelming
**Mitigation**:
- Gradual feature introduction
- Comprehensive tutorial
- Contextual help system
- Easy and hard modes

**Risk**: Content becomes repetitive
**Mitigation**:
- High variety in procedural generation
- Handcrafted unique events
- Regular content updates
- Community-created content

### Business Risks

**Risk**: Development costs exceed budget
**Mitigation**:
- Strict scope management
- Milestone-based development
- Early access funding
- Scalable feature set

**Risk**: Competition releases similar game
**Mitigation**:
- Focus on unique features
- Build strong community early
- Rapid iteration on feedback
- Strong brand identity

**Risk**: Poor launch reception
**Mitigation**:
- Extensive beta testing
- Polish over features
- Strong community management
- Day-one support ready

---

## Resource Requirements

### Team Size by Phase

**Prototype (Months 1-3)**: 4-5 people
- 2 Programmers
- 1 Artist
- 1 Designer
- 1 Producer

**Alpha (Months 4-9)**: 8-10 people
- 3 Programmers
- 2 Artists
- 2 Designers
- 1 Sound Designer
- 1 Producer
- 1 QA

**Beta (Months 10-15)**: 12-15 people
- 4 Programmers (including network engineer)
- 3 Artists
- 2 Designers
- 1 Sound Designer
- 1 Producer
- 2-3 QA
- 1 Community Manager

**Launch+ (Month 16+)**: 15-20 people
- 5 Programmers
- 3 Artists
- 2 Designers
- 1 Sound Designer
- 1 Producer
- 3 QA
- 1 Community Manager
- 2 Marketing
- 1 Support

### Budget Breakdown (Rough Estimate)

**Phase 1 (3 months)**: $150,000
- Salaries: $100,000
- Software/Tools: $20,000
- Contingency: $30,000

**Phase 2 (6 months)**: $400,000
- Salaries: $300,000
- Software/Tools: $50,000
- Contractors: $30,000
- Contingency: $20,000

**Phase 3 (6 months)**: $600,000
- Salaries: $450,000
- Infrastructure: $50,000
- Marketing: $50,000
- Contingency: $50,000

**Phase 4 (3 months)**: $300,000
- Salaries: $200,000
- Marketing: $50,000
- Infrastructure: $30,000
- Contingency: $20,000

**Total Development**: ~$1.5M
**Marketing & Launch**: ~$500K
**Contingency**: ~$500K
**Total Budget**: ~$2.5M

---

## Key Milestones Calendar

| Month | Milestone | Deliverable |
|-------|-----------|-------------|
| 1 | First Flight | Ship flies in space |
| 2 | First Landing | Can land on planet |
| 3 | Vertical Slice | 15-min demo complete |
| 6 | Alpha Content | 10 ships, 5 biomes |
| 9 | Alpha Build | 20+ hours gameplay |
| 12 | Multiplayer | 32-player instances |
| 15 | Beta Build | Feature complete |
| 16 | Code Freeze | Final polish only |
| 17 | Gold Master | Launch build |
| 18 | Launch | Public release |
| 24 | First DLC | Major content expansion |
| 30 | Console Ports | PS5/Xbox release |

---

## Community Engagement Timeline

### Prototype Phase
- Closed development (stealth mode)
- Friends & family testing only

### Alpha Phase
- Announce project publicly
- Start Discord community
- Monthly dev blogs
- Alpha signups

### Beta Phase
- Open beta applications
- Weekly dev streams
- Community events
- Reddit AMA

### Launch
- Full marketing campaign
- Launch event
- Content creator program
- Daily community engagement

### Post-Launch
- Weekly community updates
- Monthly dev roadmap
- Community spotlight
- Player-created content features

---

## Conclusion

This roadmap provides a realistic path from concept to successful launch. The key principles are:

1. **Iterate Early**: Get core systems working in month 1
2. **Test Constantly**: Alpha testers from month 4
3. **Scale Gradually**: Grow team as features expand
4. **Polish Over Features**: Better to have fewer polished features than many broken ones
5. **Engage Community**: Build audience before launch

**Critical Success Factors**:
- Hitting Month 3 vertical slice milestone
- Maintaining 60 FPS throughout development
- Building active community pre-launch
- Smooth multiplayer at beta
- Bug-free launch

**Flexibility**:
This roadmap is a guide, not a rigid plan. Be prepared to:
- Adjust timeline based on funding
- Cut features if behind schedule
- Add features if ahead of schedule
- Respond to player feedback

The goal is a successful launch that establishes EDNMS as a premier space exploration game, with a clear path for years of post-launch support and growth.

**Next Review**: After Month 3 prototype demo
**Decision Point**: Continue to alpha or adjust scope

---

*Last Updated: 2026-02-12*
*Version: 1.0*
