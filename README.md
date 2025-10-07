# MMORPG Proof-of-Concept - Unreal Engine 5.6

This is a foundational proof-of-concept for an MMORPG built in Unreal Engine 5.6 using C++.

## Project Structure

The project is organized into modular folders for better maintainability:

```
Source/MMORPG/
├── public/
│   ├── Characters/     - Character classes (player, NPCs)
│   ├── Items/          - Item and equipment system
│   ├── Factions/       - Faction and reputation system
│   ├── Missions/       - Quest and mission system
│   ├── UI/             - User interface components
│   ├── Skills/         - Skill and ability system
│   ├── Spells/         - Magic and spell system
│   └── Database/       - Database integration (SpacetimeDB)
└── private/
    └── (same structure as public/)
```

## Current Features

### Player Character (MMORPGCharacter)
- **Movement**: WASD keyboard input
- **Jump**: Space key
- **Crouch**: Left Ctrl key
- **Camera Rotation**: Mouse movement
- **Camera Alignment**: Right Mouse Button
- **Camera Zoom**: Mouse wheel
  - Smooth zoom in/out
  - Clamped distance (100-800 units)
  - Automatic transition to first-person when fully zoomed in

### Game Mode
- **MMORPGGameMode**: Base game mode that spawns the player character

## Technical Details

- **Engine Version**: Unreal Engine 5.6
- **Input System**: Enhanced Input System
- **Camera System**: Spring Arm + Camera Component with smooth interpolation
- **Movement**: Character Movement Component with crouch support

## Future Integration

This codebase is structured to support:
- SpacetimeDB integration for persistent multiplayer data
- Additional gameplay systems (combat, inventory, etc.)
- Networked multiplayer features
- Advanced AI for NPCs

## Building the Project

1. Ensure Unreal Engine 5.6 is installed
2. Right-click on `MMORPG.uproject` and select "Generate Visual Studio project files"
3. Open the generated solution file
4. Build the project in your IDE
5. Launch the editor from the IDE or by opening `MMORPG.uproject`

## Input Setup

The character uses the Enhanced Input system. You'll need to set up Input Actions and Input Mapping Context in the editor:

1. Create Input Actions for:
   - Move (Vector2D)
   - Look (Vector2D)
   - Jump (Digital)
   - Crouch (Digital)
   - Zoom (Axis1D)
   - AlignCamera (Digital)

2. Create an Input Mapping Context and assign it to the character's `DefaultMappingContext` property

3. Map the actions to keyboard/mouse inputs in the Input Mapping Context
