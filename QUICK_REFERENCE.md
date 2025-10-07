# Combat System Quick Reference

## Quick Setup

### Add to Character (C++)
```cpp
// In Character.h
UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
UResourceComponent* ResourceComponent;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
UCombatComponent* CombatComponent;

// In Character.cpp constructor
ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ResourceComponent"));
CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
```

### Add to Character (Blueprint)
1. Open Character Blueprint
2. Add Component → Resource Component
3. Add Component → Combat Component
4. Configure in Details panel

## Common Tasks

### Equip a Weapon
```cpp
FWeaponData Weapon;
Weapon.WeaponName = "Iron Sword";
Weapon.WeaponType = EWeaponType::Melee;
Weapon.BaseDamage = 20.0f;
Weapon.AttackRange = 150.0f;
CombatComponent->SetEquippedWeapon(Weapon);
```

### Execute Attack
```cpp
// Simple
CombatComponent->ExecuteMeleeAttack(TargetActor);
CombatComponent->ExecuteRangedAttack(TargetActor);
CombatComponent->ExecuteMagicalAttack(TargetActor);

// Custom
FAttackAbilityData CustomAbility;
CustomAbility.Damage = 50.0f;
CustomAbility.Range = 300.0f;
// ... configure
CombatComponent->ExecuteAttack(CustomAbility, TargetActor);
```

### Check Resources
```cpp
// Check single resource
bool HasMana = ResourceComponent->HasEnoughResource(EResourceType::Mana, 25.0f);

// Check multiple resources
TArray<FResourceCost> Costs;
FResourceCost ManaCost;
ManaCost.ResourceType = EResourceType::Mana;
ManaCost.Amount = 25.0f;
Costs.Add(ManaCost);
bool HasAll = ResourceComponent->HasEnoughResources(Costs);

// Get current value
float CurrentHP = ResourceComponent->GetCurrentResource(EResourceType::Health);
```

### Manage Resources
```cpp
// Consume
ResourceComponent->ConsumeResource(EResourceType::Stamina, 10.0f);

// Restore
ResourceComponent->RestoreResource(EResourceType::Health, 50.0f);
```

### Check Cooldowns
```cpp
// Is on cooldown?
bool OnCooldown = CombatComponent->IsAbilityOnCooldown(EAttackType::MeleeAttack);

// Get remaining time
float Remaining = CombatComponent->GetRemainingCooldown(EAttackType::MagicalAttack);
```

### Bind to Events
```cpp
// In BeginPlay
CombatComponent->OnAttackExecuted.AddDynamic(this, &AMyClass::HandleAttack);
CombatComponent->OnDamageTaken.AddDynamic(this, &AMyClass::HandleDamage);
ResourceComponent->OnResourceChanged.AddDynamic(this, &AMyClass::HandleResourceChange);

// Event handlers
void AMyClass::HandleAttack(EAttackType Type, AActor* Target, float Damage) { }
void AMyClass::HandleDamage(AActor* Instigator, float Damage) { }
void AMyClass::HandleResourceChange(EResourceType Type, float Current, float Max) { }
```

## Default Values

### Resources
- Health: 100 / 100
- Mana: 100 / 100
- Stamina: 100 / 100

### Melee Attack
- Damage: 15
- Range: 150
- Cooldown: 1.0s
- Cost: 10 Stamina

### Ranged Attack
- Damage: 20
- Range: 500
- Cooldown: 1.5s
- Cost: 15 Stamina

### Magical Attack
- Damage: 30
- Range: 400
- Cooldown: 2.0s
- Cost: 25 Mana

## Example Weapons

### IronSword
```cpp
AWeaponItem* Sword = GetWorld()->SpawnActor<AIronSword>();
// Melee, Damage: 15, Range: 150
```

### Longbow
```cpp
AWeaponItem* Bow = GetWorld()->SpawnActor<ALongbow>();
// Ranged, Damage: 18, Range: 600
```

### FireStaff
```cpp
AWeaponItem* Staff = GetWorld()->SpawnActor<AFireStaff>();
// Magical, Damage: 25, Range: 450
```

### Greatsword
```cpp
AWeaponItem* Greatsword = GetWorld()->SpawnActor<AGreatsword>();
// Melee, Damage: 35, Range: 180
```

## Custom Ability Example

```cpp
FAttackAbilityData Fireball;
Fireball.AbilityName = "Fireball";
Fireball.AttackType = EAttackType::MagicalAttack;
Fireball.Damage = 50.0f;
Fireball.Range = 500.0f;
Fireball.Cooldown = 5.0f;

// Add mana cost
FResourceCost ManaCost;
ManaCost.ResourceType = EResourceType::Mana;
ManaCost.Amount = 40.0f;
Fireball.ResourceCosts.Add(ManaCost);

// Set as magical ability
CombatComponent->MagicalAbility = Fireball;
```

## Blueprint Functions

All UFUNCTION(BlueprintCallable) are available in Blueprints:

**CombatComponent:**
- Execute Melee Attack
- Execute Ranged Attack
- Execute Magical Attack
- Set Equipped Weapon
- Get Equipped Weapon
- Is Ability On Cooldown
- Get Remaining Cooldown

**ResourceComponent:**
- Get Current Resource
- Get Max Resource
- Consume Resource
- Restore Resource
- Has Enough Resource
- Has Enough Resources
- Consume Resources

## Debugging

### Log Current State
```cpp
UE_LOG(LogTemp, Warning, TEXT("HP: %.2f / %.2f"), 
    ResourceComponent->GetCurrentResource(EResourceType::Health),
    ResourceComponent->GetMaxResource(EResourceType::Health));

UE_LOG(LogTemp, Warning, TEXT("Weapon: %s (Damage: %.2f)"), 
    *CombatComponent->GetEquippedWeapon().WeaponName,
    CombatComponent->GetEquippedWeapon().BaseDamage);
```

### Enable Component Logging
Events automatically log to LogTemp category:
- Attack execution
- Damage taken
- Resource changes

## Performance Tips

1. **Disable Tick When Not Needed**
   ```cpp
   CombatComponent->SetComponentTickEnabled(false); // If no cooldowns active
   ```

2. **Cache Component References**
   ```cpp
   // In BeginPlay, not every frame
   ResourceComponent = GetOwner()->FindComponentByClass<UResourceComponent>();
   ```

3. **Use Events Instead of Polling**
   ```cpp
   // Good: Bind to OnResourceChanged
   // Bad: Check resources every tick
   ```

## Common Pitfalls

❌ Don't call ExecuteAttack every frame
✅ Call once per button press

❌ Don't modify component properties directly
✅ Use provided setter methods

❌ Don't forget to check return values
✅ ExecuteAttack returns false if failed

❌ Don't assume target is valid
✅ Always null-check actors

## File Locations

**Headers:** `Source/MMORPG/public/`
- CombatTypes.h
- ResourceComponent.h
- CombatComponent.h
- WeaponItem.h
- CombatCharacter.h
- ExampleWeapons.h

**Implementation:** `Source/MMORPG/private/`
- ResourceComponent.cpp
- CombatComponent.cpp
- WeaponItem.cpp
- CombatCharacter.cpp
- ExampleWeapons.cpp

**Documentation:**
- COMBAT_SYSTEM.md - Full API reference
- EXAMPLES.md - Usage examples
- PERSISTENCE_GUIDE.md - Save/load integration
- ARCHITECTURE.md - System diagrams
- IMPLEMENTATION_SUMMARY.md - Overview

## Getting Help

1. Check EXAMPLES.md for usage patterns
2. Review CombatCharacter for integration example
3. See PERSISTENCE_GUIDE.md for save/load
4. Check ARCHITECTURE.md for system overview
5. Review inline comments in header files
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
