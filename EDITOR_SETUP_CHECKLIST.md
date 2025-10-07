# Unreal Editor Setup Checklist

This checklist covers the steps needed to complete the setup of the MMORPG foundational structure in Unreal Engine 5.6 editor.

## Prerequisites
- [ ] Unreal Engine 5.6 installed
- [ ] Visual Studio or Rider with C++ support installed
- [ ] Project opened successfully in the editor

## Code Compilation
- [ ] Generate Visual Studio project files (right-click MMORPG.uproject → Generate Visual Studio project files)
- [ ] Open the solution in your IDE
- [ ] Build the project (should compile without errors)
- [ ] Launch editor from IDE or by opening MMORPG.uproject

## Input System Setup

### Create Input Assets (see SETUP_INPUT.md for detailed instructions)
- [ ] Create `Content/Input` folder
- [ ] Create Input Actions:
  - [ ] IA_Move (Axis2D)
  - [ ] IA_Look (Axis2D)
  - [ ] IA_Jump (Digital)
  - [ ] IA_Crouch (Digital)
  - [ ] IA_Zoom (Axis1D)
  - [ ] IA_AlignCamera (Digital)
- [ ] Create Input Mapping Context (IMC_Default)
- [ ] Configure mappings in IMC_Default:
  - [ ] W/A/S/D → IA_Move
  - [ ] Mouse XY → IA_Look
  - [ ] Mouse Wheel → IA_Zoom
  - [ ] Right Mouse Button → IA_AlignCamera
  - [ ] Space → IA_Jump
  - [ ] Left Ctrl → IA_Crouch

## Character Blueprint Setup
- [ ] Create Blueprint folder: `Content/Blueprints/Characters`
- [ ] Create Blueprint based on MMORPGCharacter: `BP_MMORPGCharacter`
- [ ] Open BP_MMORPGCharacter
- [ ] Assign Input Mapping Context to `Default Mapping Context`
- [ ] Assign Input Actions:
  - [ ] Move Action → IA_Move
  - [ ] Look Action → IA_Look
  - [ ] Jump Action → IA_Jump
  - [ ] Crouch Action → IA_Crouch
  - [ ] Zoom Action → IA_Zoom
  - [ ] Align Camera Action → IA_AlignCamera
- [ ] Configure Camera Settings (optional adjustments):
  - [ ] Min Camera Distance (default: 100.0)
  - [ ] Max Camera Distance (default: 800.0)
  - [ ] Zoom Speed (default: 50.0)
  - [ ] First Person Threshold (default: 120.0)
- [ ] Add a mesh to the character (use Third Person template mesh or create your own)
- [ ] Compile and save

## Game Mode Setup
- [ ] Create Blueprint based on MMORPGGameMode: `BP_MMORPGGameMode` (optional - C++ class works)
- [ ] Open Project Settings → Maps & Modes
- [ ] Set Default GameMode to MMORPGGameMode (or BP_MMORPGGameMode)
- [ ] Verify Default Pawn Class is MMORPGCharacter (or BP_MMORPGCharacter)

## Level Setup
- [ ] Create a new level or use existing Lvl_Hex_Base
- [ ] Add a Player Start actor to the level
- [ ] (Optional) Add a floor/platform for the character to stand on
- [ ] Save the level
- [ ] Set as the default map in Project Settings → Maps & Modes → Default Maps

## Testing
- [ ] Click Play in Editor (PIE)
- [ ] Test WASD movement - character should move relative to camera
- [ ] Test mouse look - camera should rotate
- [ ] Test Space jump - character should jump
- [ ] Test Left Ctrl crouch - character should crouch/uncrouch
- [ ] Test mouse wheel zoom - camera should zoom in/out smoothly
- [ ] Test zoom to first-person - camera should switch perspective when fully zoomed in
- [ ] Test Right Mouse Button - character should align to camera direction
- [ ] Verify camera collision - camera should not clip through walls

## Optional Enhancements
- [ ] Add a character mesh (Third Person template mannequin or custom)
- [ ] Configure camera socket on the mesh for better first-person view
- [ ] Add footstep sounds
- [ ] Add jump/land animations
- [ ] Add crouch animation
- [ ] Create a custom level for testing
- [ ] Add visual feedback for crouch state
- [ ] Configure camera lag settings for desired feel

## Troubleshooting

### Input Not Working
- Verify Enhanced Input plugin is enabled
- Check that Input Mapping Context is assigned in character
- Ensure Input Actions have correct value types
- Verify mappings in IMC_Default are configured correctly

### Compilation Errors
- Clean and rebuild the solution
- Delete `Intermediate`, `Binaries`, and `Saved` folders
- Regenerate project files
- Rebuild from IDE

### Character Not Spawning
- Check that Game Mode is set correctly
- Verify Player Start is in the level
- Ensure Default Pawn Class is set to MMORPGCharacter

### Camera Issues
- Verify Spring Arm and Camera components are created
- Check camera boom settings (TargetArmLength, etc.)
- Ensure camera has proper collision settings

## Next Steps

Once basic setup is complete and tested, you can proceed with:
- Implementing additional modules (Items, Skills, Spells, etc.)
- Adding combat system
- Integrating SpacetimeDB
- Creating UI elements
- Developing gameplay mechanics
