# Quick Reference Guide

## File Locations

### C++ Source Files
- **Character Header**: `Source/MMORPG/public/Characters/MMORPGCharacter.h`
- **Character Implementation**: `Source/MMORPG/private/Characters/MMORPGCharacter.cpp`
- **Game Mode Header**: `Source/MMORPG/public/MMORPGGameMode.h`
- **Game Mode Implementation**: `Source/MMORPG/private/MMORPGGameMode.cpp`

### Documentation
- **Main README**: `README.md` - Project overview
- **Architecture**: `ARCHITECTURE.md` - Technical details
- **Input Setup**: `SETUP_INPUT.md` - Enhanced Input configuration
- **Editor Checklist**: `EDITOR_SETUP_CHECKLIST.md` - Complete setup steps
- **Implementation Summary**: `IMPLEMENTATION_SUMMARY.md` - What was built

## Key Classes

### AMMORPGCharacter
Base player character with:
- WASD movement
- Jump (Space), Crouch (Left Ctrl)
- Mouse camera rotation
- RMB alignment
- Mouse wheel zoom with first-person transition

### AMMORPGGameMode
Game mode that spawns MMORPGCharacter as default pawn.

## Module Structure

All modules have public/private folders under `Source/MMORPG/`:
- **Characters** - Player/NPC classes
- **Items** - Item/inventory systems
- **Factions** - Faction/reputation
- **Missions** - Quests/missions
- **UI** - User interface
- **Skills** - Character abilities
- **Spells** - Magic system
- **Database** - SpacetimeDB integration

## Controls (After Input Setup)

| Action | Control |
|--------|---------|
| Move Forward | W |
| Move Backward | S |
| Move Left | A |
| Move Right | D |
| Jump | Space |
| Crouch | Left Ctrl |
| Look | Mouse Movement |
| Zoom | Mouse Wheel |
| Align Camera | Right Mouse Button |

## Camera Settings (Configurable)

| Property | Default Value |
|----------|---------------|
| Min Camera Distance | 100.0 units |
| Max Camera Distance | 800.0 units |
| Zoom Speed | 50.0 units/tick |
| First Person Threshold | 120.0 units |

## Build Dependencies

Required modules in `MMORPG.Build.cs`:
- Core
- CoreUObject
- Engine
- InputCore
- EnhancedInput
- ProceduralMeshComponent

## Next Steps

1. Open project in UE5.6
2. Generate Visual Studio project files
3. Build the solution
4. Follow `EDITOR_SETUP_CHECKLIST.md`
5. Set up Enhanced Input assets
6. Test in Play In Editor

## Common Commands

```bash
# View project structure
tree Source/MMORPG -L 3

# Find all C++ files
find Source/MMORPG -name "*.h" -o -name "*.cpp"

# Check git status
git status

# View recent changes
git log --oneline -5
```

## Troubleshooting Quick Tips

**Build fails**: Delete Intermediate, Binaries, Saved folders, regenerate project files

**Input doesn't work**: Verify Enhanced Input plugin enabled and mapping context assigned

**Character doesn't spawn**: Check Game Mode settings in Project Settings → Maps & Modes

**Camera issues**: Verify Spring Arm and Camera components exist and are properly configured

## Resources

- Unreal Engine Documentation: https://docs.unrealengine.com/
- Enhanced Input System: https://docs.unrealengine.com/5.0/en-US/enhanced-input-in-unreal-engine/
- Character Movement: https://docs.unrealengine.com/5.0/en-US/character-movement-component/

## Project Stats

- **C++ Classes**: 2 (MMORPGCharacter, MMORPGGameMode)
- **Header Files**: 2
- **Implementation Files**: 2
- **Total Lines of Code**: ~366
- **Documentation Files**: 5
- **Module Folders**: 8
- **Total Commits**: 3

## Contact & Support

For issues or questions, refer to:
- Project repository issues
- Documentation files in project root
- Unreal Engine community forums
