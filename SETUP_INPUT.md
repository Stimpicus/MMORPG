# Input Setup Guide for MMORPG Character

Since this project uses Unreal Engine's Enhanced Input System, you'll need to set up Input Actions and an Input Mapping Context in the Unreal Editor.

## Step 1: Create Input Actions

1. In the Content Browser, create a new folder called `Input`
2. Right-click in the `Input` folder and select `Input` > `Input Action`
3. Create the following Input Actions:

   - **IA_Move** (Value Type: Axis2D - Vector2D)
   - **IA_Look** (Value Type: Axis2D - Vector2D)
   - **IA_Jump** (Value Type: Digital - Boolean)
   - **IA_Crouch** (Value Type: Digital - Boolean)
   - **IA_Zoom** (Value Type: Axis1D - float)
   - **IA_AlignCamera** (Value Type: Digital - Boolean)

## Step 2: Create Input Mapping Context

1. In the `Input` folder, right-click and select `Input` > `Input Mapping Context`
2. Name it `IMC_Default`
3. Open the Input Mapping Context and add the following mappings:

### Movement Mappings

| Input Action | Key | Modifiers |
|--------------|-----|-----------|
| IA_Move | W | Swizzle Input Axis Values: YXZ, Negate Y |
| IA_Move | S | Swizzle Input Axis Values: YXZ, Negate Y, Negate X |
| IA_Move | A | Swizzle Input Axis Values: YXZ, Negate X |
| IA_Move | D | Swizzle Input Axis Values: YXZ |

Or simpler approach:
- W: IA_Move with value (0, 1)
- S: IA_Move with value (0, -1)
- A: IA_Move with value (-1, 0)
- D: IA_Move with value (1, 0)

### Camera Mappings

| Input Action | Input | Modifiers |
|--------------|-------|-----------|
| IA_Look | Mouse XY 2D-Axis | None |
| IA_Zoom | Mouse Wheel Axis | None |
| IA_AlignCamera | Right Mouse Button | None |

### Action Mappings

| Input Action | Key |
|--------------|-----|
| IA_Jump | Space Bar |
| IA_Crouch | Left Ctrl |

## Step 3: Assign to Character Blueprint

If you create a Blueprint based on MMORPGCharacter:

1. Open the Character Blueprint
2. In the Details panel, find the `Input` section
3. Set `Default Mapping Context` to `IMC_Default`
4. Assign each Input Action:
   - Move Action: IA_Move
   - Look Action: IA_Look
   - Jump Action: IA_Jump
   - Crouch Action: IA_Crouch
   - Zoom Action: IA_Zoom
   - Align Camera Action: IA_AlignCamera

## Step 4: Configure Game Mode

1. Create a Blueprint based on MMORPGGameMode (or use the C++ class directly)
2. In Project Settings > Maps & Modes, set your Game Mode as the default
3. Ensure the Default Pawn Class is set to your MMORPGCharacter (or blueprint derived from it)

## Step 5: Test

1. Create a new level or open an existing one
2. Place a Player Start actor in the level
3. Press Play to test the character controls

## Troubleshooting

- If input doesn't work, check that the Input Mapping Context is properly assigned
- Verify that the Enhanced Input plugin is enabled in Project Settings
- Make sure the Player Controller has the mapping context added in BeginPlay
- Check that Input Actions are properly configured with the correct value types
