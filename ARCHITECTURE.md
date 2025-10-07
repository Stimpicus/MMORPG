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
