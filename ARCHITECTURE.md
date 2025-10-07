# Resource Gathering System Architecture

## System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                    Resource Gathering System                     │
└─────────────────────────────────────────────────────────────────┘

┌──────────────────────┐         ┌──────────────────────┐
│   ResourceTypes.h    │         │  ExamplePlayer       │
│  ─────────────────   │         │  Character           │
│  - EResourceType     │         │  ──────────────────  │
│  - EGatheringSkill   │◄────────│  + Inventory         │
│  - FResourceItem     │         │  + SkillProgression  │
└──────────────────────┘         └──────────┬───────────┘
                                            │
                                            │ uses
                                            │
                        ┌───────────────────▼────────────────────┐
                        │  ResourceGatheringSubsystem            │
                        │  ────────────────────────────────────  │
                        │  + PerformGathering()                  │
                        │  + CanGatherFrom()                     │
                        └───┬───────────────┬──────────────┬────┘
                            │               │              │
              ┌─────────────▼──┐    ┌──────▼─────┐   ┌───▼────────────┐
              │ ResourceNode   │    │ Inventory  │   │ Skill          │
              │ Actor          │    │ Component  │   │ Progression    │
              │ ─────────────  │    │ ──────────│   │ Component      │
              │ - Type         │    │ - Resources│   │ - SkillData    │
              │ - Quantity     │    │            │   │ - Experience   │
              │ - Regenerates  │    │ + Add()    │   │ + AddExp()     │
              │ + Gather()     │    │ + Remove() │   │ + GetLevel()   │
              └────────────────┘    └────────────┘   └────────────────┘
```

## Component Relationships

### 1. Core Data Layer (ResourceTypes.h)
- Defines all enumerations and structures
- No dependencies
- Used by all other components

### 2. Actor Layer (ResourceNodeActor)
- Represents physical resource nodes in the world
- Can be placed in levels
- Manages resource availability and regeneration
- Independent of player/inventory

### 3. Component Layer
**InventoryComponent**
- Attached to any actor (typically player)
- Stores gathered resources
- Manages resource quantities

**SkillProgressionComponent**
- Attached to any actor (typically player)
- Tracks skill levels and experience
- Handles automatic leveling

### 4. Coordination Layer (ResourceGatheringSubsystem)
- Static utility class
- Coordinates between ResourceNode, Inventory, and Skills
- Provides centralized gathering logic
- Returns detailed results

### 5. Integration Layer (ExamplePlayerCharacter)
- Example implementation
- Shows component integration
- Demonstrates usage patterns

## Data Flow: Gathering Operation

```
Player Interaction
      │
      ▼
┌─────────────────────────────────────────────────┐
│ ExamplePlayerCharacter::GatherFromNode()        │
└──────────────────┬──────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────┐
│ ResourceGatheringSubsystem::PerformGathering()  │
└──────┬────────────────┬──────────────┬──────────┘
       │                │              │
       ▼                ▼              ▼
  ┌─────────┐    ┌──────────┐   ┌─────────────┐
  │  Node   │    │Inventory │   │   Skills    │
  │ Gather()│    │  Add()   │   │ AddExp()    │
  └────┬────┘    └────┬─────┘   └──────┬──────┘
       │              │                 │
       └──────────────┴─────────────────┘
                      │
                      ▼
              ┌──────────────┐
              │FGathering    │
              │Result        │
              └──────────────┘
                      │
                      ▼
              Update UI/Effects
```

## Persistence Architecture

All components are designed for easy persistence:

```
┌────────────────────────────────────────┐
│        Save Game Data                   │
├────────────────────────────────────────┤
│  Inventory:                             │
│  - TMap<EResourceType, int32>          │
│                                         │
│  Skills:                                │
│  - TMap<EGatheringSkillType,           │
│          FGatheringSkillData>          │
│                                         │
│  Resource Nodes (optional):             │
│  - Array of node states                │
└────────────────────────────────────────┘
         │                    ▲
         │ Save               │ Load
         ▼                    │
┌────────────────────────────────────────┐
│    Persistent Storage                   │
│    (File/Database/Cloud)                │
└────────────────────────────────────────┘
```

## Extension Points

### Adding New Resource Types
1. Edit `EResourceType` in ResourceTypes.h
2. No other code changes needed
3. Existing systems work automatically

### Adding New Skills
1. Edit `EGatheringSkillType` in ResourceTypes.h
2. Set `AssociatedSkill` on resource nodes
3. Existing progression works automatically

### Custom Resource Nodes
```cpp
AResourceNodeActor (Base)
    │
    ├─ ARareResourceNode
    ├─ ATimedResourceNode
    └─ APlayerOwnedNode
```

### Network Replication (Future)
```
Add to components:
- UPROPERTY(Replicated)
- GetLifetimeReplicatedProps()
- ServerRPC functions for gathering
```

## Thread Safety

- All components designed for game thread
- No async operations in base implementation
- Safe for Blueprint usage
- Regeneration uses UE timer system

## Performance Characteristics

- **Memory**: O(n) for n resource types in inventory
- **Gathering**: O(1) constant time operation
- **Skill Update**: O(1) constant time per skill
- **Regeneration**: Amortized O(1) with timers

## Testing Strategy

### Unit Testing Points
1. Inventory add/remove operations
2. Skill experience calculations
3. Level-up detection
4. Resource node depletion/regeneration

### Integration Testing
1. Full gathering flow
2. Multiple resource types
3. Skill progression across levels
4. Edge cases (empty nodes, full inventory)

## Best Practices

1. **Component Reuse**: Attach components to any actor, not just players
2. **Centralized Logic**: Use ResourceGatheringSubsystem for consistency
3. **Blueprint Exposure**: All systems fully Blueprint-accessible
4. **Data-Driven**: Configure behavior through properties, not code
5. **Modularity**: Each component works independently
6. **Extensibility**: Inherit and override for custom behavior

## Future Enhancements

Potential additions:
- Tool requirements for gathering
- Gathering speed/duration
- Quality/rarity variations
- Multi-resource nodes
- Skill-based gathering bonuses
- Resource transformation/crafting
- Seasonal availability
- Network synchronization
# Combat System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                      Combat System Overview                      │
└─────────────────────────────────────────────────────────────────┘

┌──────────────────┐
│  CombatCharacter │  (Example Implementation)
└────────┬─────────┘
         │
         ├─────────────────────┐
         │                     │
         ▼                     ▼
┌────────────────────┐  ┌───────────────────┐
│ ResourceComponent  │  │  CombatComponent  │
└────────┬───────────┘  └────────┬──────────┘
         │                       │
         │                       │
         ▼                       ▼
┌─────────────────────────────────────────┐
│           CombatTypes.h                 │
│  ┌─────────────────────────────────┐   │
│  │ Enums:                          │   │
│  │  - EWeaponType                  │   │
│  │  - EAttackType                  │   │
│  │  - EResourceType                │   │
│  │                                 │   │
│  │ Structs:                        │   │
│  │  - FWeaponData                  │   │
│  │  - FAttackAbilityData           │   │
│  │  - FResourceCost                │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
                    │
                    ▼
         ┌──────────────────┐
         │   WeaponItem     │
         └──────────────────┘
                    │
                    ├────────────┬──────────────┬─────────────┐
                    ▼            ▼              ▼             ▼
            ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐
            │IronSword │  │ Longbow  │  │FireStaff │  │Greatsword│
            └──────────┘  └──────────┘  └──────────┘  └──────────┘


Component Interaction Flow:
═══════════════════════════

Player Input → CombatCharacter
                    │
                    ├→ SetCurrentTarget(Enemy)
                    │
                    └→ PerformMeleeAttack()
                              │
                              ▼
                    ┌─────────────────────┐
                    │  CombatComponent    │
                    └──────────┬──────────┘
                               │
                    ┌──────────┴──────────┐
                    │                     │
                    ▼                     ▼
        ┌───────────────────┐   ┌─────────────────┐
        │ Check Cooldown    │   │ Check Range     │
        └───────────────────┘   └─────────────────┘
                    │                     │
                    └──────────┬──────────┘
                               ▼
                    ┌──────────────────────┐
                    │  ResourceComponent   │
                    │  Check Resources     │
                    └──────────┬───────────┘
                               │
                    ┌──────────┴──────────┐
                    │                     │
                    ▼                     ▼
        ┌───────────────────┐   ┌─────────────────┐
        │ Consume Resources │   │ Apply Cooldown  │
        └───────────────────┘   └─────────────────┘
                    │                     │
                    └──────────┬──────────┘
                               ▼
                    ┌──────────────────────┐
                    │  Calculate Damage    │
                    │  (Base + Weapon)     │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │  Apply to Target     │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │  Broadcast Events    │
                    │  - OnAttackExecuted  │
                    │  - OnDamageTaken     │
                    │  - OnResourceChanged │
                    └──────────────────────┘


Resource Management:
═══════════════════

┌────────────────────────────────────────┐
│         ResourceComponent              │
├────────────────────────────────────────┤
│                                        │
│  ┌──────────────────────────────────┐ │
│  │  Health:  [████████░░] 80/100   │ │
│  │  Mana:    [██████████] 100/100  │ │
│  │  Stamina: [████░░░░░░] 40/100   │ │
│  └──────────────────────────────────┘ │
│                                        │
│  Methods:                              │
│  ├─ GetCurrentResource()               │
│  ├─ ConsumeResource()                  │
│  ├─ RestoreResource()                  │
│  ├─ HasEnoughResource()                │
│  └─ ConsumeResources()                 │
│                                        │
│  Events:                               │
│  └─ OnResourceChanged                  │
│     (for UI updates)                   │
└────────────────────────────────────────┘


Attack Types:
════════════

┌─────────────────────────────────────────────────────────┐
│                    Melee Attack                         │
├─────────────────────────────────────────────────────────┤
│ Range: 150 units | Cooldown: 1s | Cost: 10 Stamina     │
│ Damage: 15 + Weapon Bonus                              │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                   Ranged Attack                         │
├─────────────────────────────────────────────────────────┤
│ Range: 500 units | Cooldown: 1.5s | Cost: 15 Stamina   │
│ Damage: 20 + Weapon Bonus                              │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                  Magical Attack                         │
├─────────────────────────────────────────────────────────┤
│ Range: 400 units | Cooldown: 2s | Cost: 25 Mana        │
│ Damage: 30 + Weapon Bonus                              │
└─────────────────────────────────────────────────────────┘


Weapon Integration:
══════════════════

        ┌──────────────────────┐
        │    FWeaponData       │
        ├──────────────────────┤
        │ - WeaponName         │
        │ - WeaponType         │
        │ - BaseDamage         │
        │ - AttackRange        │
        │ - AttackSpeed        │
        └──────────┬───────────┘
                   │
         ┌─────────┴─────────┐
         │                   │
         ▼                   ▼
    ┌─────────┐        ┌─────────┐
    │ Melee   │        │ Ranged  │
    │ Range:  │        │ Range:  │
    │ 150     │        │ 500     │
    └─────────┘        └─────────┘
         │                   │
         ▼                   ▼
    ┌─────────┐        ┌─────────┐
    │ Magical │        │ Sets    │
    │ Range:  │        │ Ability │
    │ 400     │        │ Range   │
    └─────────┘        └─────────┘


Extensibility Points:
════════════════════

1. Custom Abilities:
   └─ Create FAttackAbilityData instances
      └─ Configure damage, range, cooldown, costs

2. Status Effects:
   └─ Extend CombatComponent
      └─ Add effect tracking
         └─ Modify damage calculations

3. Combo System:
   └─ Track attack sequence
      └─ Trigger special abilities
         └─ Reset on timeout

4. Character Stats:
   └─ Add stats component
      └─ Modify damage/costs
         └─ Scale with level

5. Network Replication:
   └─ Mark properties for replication
      └─ Implement RPCs
         └─ Add client prediction

6. Persistence:
   └─ Serialize data structures
      └─ Save to SaveGame/Database
         └─ Load on startup


Event System:
════════════

┌───────────────────────────────────────┐
│          Event Delegates              │
├───────────────────────────────────────┤
│                                       │
│  OnResourceChanged                    │
│  ├─ Update UI Bars                    │
│  ├─ Trigger Low Resource Warning      │
│  └─ Enable/Disable Abilities          │
│                                       │
│  OnAttackExecuted                     │
│  ├─ Play Attack Animation             │
│  ├─ Spawn Projectile/VFX              │
│  └─ Play Attack Sound                 │
│                                       │
│  OnDamageTaken                        │
│  ├─ Play Hit Reaction                 │
│  ├─ Spawn Damage Numbers              │
│  ├─ Play Hurt Sound                   │
│  └─ Check for Death                   │
│                                       │
└───────────────────────────────────────┘


Integration with Other Systems:
═══════════════════════════════

                ┌──────────────┐
                │Combat System │
                └──────┬───────┘
                       │
         ┌─────────────┼─────────────┐
         │             │             │
         ▼             ▼             ▼
    ┌────────┐   ┌─────────┐   ┌────────┐
    │Animation│  │  UI     │   │  Audio │
    │System  │   │ System  │   │ System │
    └────────┘   └─────────┘   └────────┘
         │             │             │
         │             │             │
         ▼             ▼             ▼
    ┌────────┐   ┌─────────┐   ┌────────┐
    │  VFX   │   │Inventory│   │Network │
    │System  │   │ System  │   │ System │
    └────────┘   └─────────┘   └────────┘
                       │
                       ▼
                 ┌──────────┐
                 │Persistence│
                 │  System  │
                 └──────────┘
```
# MMORPG Technical Architecture Documentation

## Overview

This document describes the technical architecture of the MMORPG proof-of-concept built in Unreal Engine 5.6 using C++.

## Module Structure

The codebase is organized into distinct modules to promote separation of concerns and maintainability:

### Characters Module
- **Location**: `Source/MMORPG/public/Characters/` and `Source/MMORPG/private/Characters/`
- **Purpose**: Player and NPC character classes
- **Key Classes**:
  - `AMMORPGCharacter`: Base player character with movement and camera controls

### Items Module
- **Location**: `Source/MMORPG/public/Items/` and `Source/MMORPG/private/Items/`
- **Purpose**: Item, inventory, and equipment systems
- **Status**: Planned for future development

### Factions Module
- **Location**: `Source/MMORPG/public/Factions/` and `Source/MMORPG/private/Factions/`
- **Purpose**: Faction allegiance and reputation systems
- **Status**: Planned for future development

### Missions Module
- **Location**: `Source/MMORPG/public/Missions/` and `Source/MMORPG/private/Missions/`
- **Purpose**: Quest and mission tracking
- **Status**: Planned for future development

### UI Module
- **Location**: `Source/MMORPG/public/UI/` and `Source/MMORPG/private/UI/`
- **Purpose**: User interface components and HUD elements
- **Status**: Planned for future development

### Skills Module
- **Location**: `Source/MMORPG/public/Skills/` and `Source/MMORPG/private/Skills/`
- **Purpose**: Character skills and progression systems
- **Status**: Planned for future development

### Spells Module
- **Location**: `Source/MMORPG/public/Spells/` and `Source/MMORPG/private/Spells/`
- **Purpose**: Magic and spell casting systems
- **Status**: Planned for future development

### Database Module
- **Location**: `Source/MMORPG/public/Database/` and `Source/MMORPG/private/Database/`
- **Purpose**: SpacetimeDB integration for persistent data
- **Status**: Planned for future development

## Character System

### MMORPGCharacter Class

The `AMMORPGCharacter` class extends `ACharacter` and implements the base player character functionality.

#### Components

1. **Spring Arm Component** (`USpringArmComponent`)
   - Provides smooth camera following with collision detection
   - Supports camera lag for natural movement
   - Configurable arm length for zoom functionality

2. **Camera Component** (`UCameraComponent`)
   - Attached to the spring arm
   - Provides third-person perspective
   - Switches to first-person when fully zoomed in

3. **Character Movement Component** (inherited from `ACharacter`)
   - Handles WASD movement
   - Supports jumping and crouching
   - Configured for smooth character rotation

#### Input System

Uses Unreal Engine's Enhanced Input System:

- **Input Actions**: Define abstract actions (Move, Look, Jump, etc.)
- **Input Mapping Context**: Maps physical inputs to actions
- **Input Component**: Processes and dispatches input events

#### Movement Features

1. **WASD Movement**
   - Forward/Backward: W/S keys
   - Strafe Left/Right: A/D keys
   - Movement relative to camera direction
   - Smooth character rotation towards movement direction

2. **Jump**
   - Triggered by Space key
   - Uses standard Character Movement Component jump mechanics

3. **Crouch**
   - Toggle crouch with Left Ctrl
   - Reduces character height to 60 units (half height)
   - Smooth transition animation support

#### Camera Features

1. **Mouse Look**
   - Rotate camera with mouse movement
   - Pitch and yaw control
   - Controller rotation drives camera boom

2. **Right Mouse Button Alignment**
   - Aligns character rotation to camera direction
   - Useful for combat and precise movement

3. **Mouse Wheel Zoom**
   - Smooth interpolation between zoom levels
   - Clamped distance: 100-800 units
   - Configurable zoom speed
   - **First-Person Transition**: Automatically switches to first-person view when zoom distance < 120 units

#### Camera Perspective Switching

The character implements dynamic camera perspective switching:

```cpp
void AMMORPGCharacter::UpdateCameraPerspective()
{
    if (CameraBoom->TargetArmLength <= FirstPersonThreshold)
    {
        // First-person: offset camera to eye level
        FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f));
    }
    else
    {
        // Third-person: reset to default position
        FollowCamera->SetRelativeLocation(FVector::ZeroVector);
    }
}
```

## Game Mode

### MMORPGGameMode Class

The `AMMORPGGameMode` class extends `AGameModeBase` and serves as the base game mode.

#### Responsibilities

1. **Player Spawning**: Sets `AMMORPGCharacter` as the default pawn class
2. **Game Rules**: Foundation for future game logic implementation
3. **State Management**: Will handle game state transitions in future development

## Build Configuration

### Module Dependencies

Defined in `MMORPG.Build.cs`:

```csharp
PublicDependencyModuleNames.AddRange(new string[] { 
    "Core", 
    "CoreUObject", 
    "Engine", 
    "InputCore", 
    "EnhancedInput",
    "ProceduralMeshComponent" 
});
```

### Required Plugins

- **Enhanced Input**: Modern input system for Unreal Engine 5
- **Modeling Tools Editor Mode**: For editor functionality

## Future Development

### SpacetimeDB Integration

The Database module will integrate SpacetimeDB for:
- **Persistent Player Data**: Character stats, inventory, progress
- **Multiplayer Synchronization**: Real-time data updates
- **Server Authority**: Authoritative server-side data management

### Planned Systems

1. **Combat System**: Melee and ranged combat
2. **Inventory System**: Item management and equipment
3. **Quest System**: Mission tracking and rewards
4. **Skill Trees**: Character progression and abilities
5. **Spell Casting**: Magic system with cooldowns and effects
6. **Faction System**: Reputation and allegiance management
7. **UI Framework**: HUD, menus, and interactive elements

## Code Style Guidelines

1. **Naming Conventions**:
   - Classes: PascalCase with appropriate prefix (A for Actors, U for UObjects)
   - Member variables: PascalCase with type prefix (b for bool, f for float, etc.)
   - Functions: PascalCase
   - Local variables: PascalCase

2. **File Organization**:
   - Header files (.h) in `public/` directories
   - Implementation files (.cpp) in `private/` directories
   - Organized by module in subdirectories

3. **Documentation**:
   - Copyright notice at top of each file
   - Comments for complex logic
   - UPROPERTY macros with categories for blueprint exposure

## Performance Considerations

1. **Tick Function**: Character uses tick for smooth camera interpolation
2. **Input Buffering**: Enhanced Input System provides built-in input buffering
3. **Camera Lag**: Smooth camera movement without jitter
4. **Collision Detection**: Spring arm performs collision tests for camera positioning

## Testing

Since we don't have direct Unreal Engine access in this development environment:

1. **Manual Testing Required**: Test in Unreal Editor after setup
2. **Input Testing**: Verify all input actions work correctly
3. **Camera Testing**: Test zoom, rotation, and first-person transition
4. **Movement Testing**: Verify WASD, jump, and crouch functionality

## Version Information

- **Unreal Engine**: 5.6
- **Build Settings**: V5
- **Include Order**: Unreal5_6
