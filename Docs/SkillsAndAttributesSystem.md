# Skills and Attributes System

This document explains the skills and attributes system implemented for MMORPG player characters.

## Overview

The system consists of three main components that work together to provide a modular, extensible character progression system:

1. **PlayerAttributesComponent** - Manages persistent values like XP, HP, Mana, Stamina, and Credits
2. **PlayerSkillsComponent** - Handles skill progression for 7 different skill types
3. **EquipmentManagerComponent** - Manages equipment-based modifiers to attributes and skills

## Components

### PlayerAttributesComponent

Manages core character attributes:

- **Persistent Values:**
  - XP (Experience Points)
  - HP (Health Points)
  - Mana
  - Stamina
  - Credits (in-game currency)

- **Features:**
  - Base max values (BaseMaxHP, BaseMaxMana, BaseMaxStamina)
  - Equipment modifier support
  - Automatic clamping of current values to max values
  - Network replication ready
  - Blueprint accessible

**Example Usage in Blueprint:**
```cpp
// Get attributes component
UPlayerAttributesComponent* Attributes = Character->GetAttributesComponent();

// Modify values
Attributes->AddXP(100.0f);
Attributes->ModifyHP(-20.0f); // Take damage
Attributes->ModifyMana(-10.0f); // Use mana
Attributes->AddCredits(50);
```

### PlayerSkillsComponent

Manages skill progression for 7 skill types:

1. **Toughness** - Affects survivability
2. **Mana Efficiency** - Reduces mana costs
3. **Stamina Efficiency** - Reduces stamina costs
4. **Melee Combat** - Improves melee damage and accuracy
5. **Ranged Combat** - Improves ranged damage and accuracy
6. **Magical Ability** - Improves spell effectiveness
7. **Resource Gathering** - Improves gathering speed and yield

- **Features:**
  - XP-based progression with configurable curve
  - Equipment modifier support
  - Effective skill value calculation (level + equipment bonus)
  - Network replication ready
  - Blueprint accessible

**Example Usage in Blueprint:**
```cpp
// Get skills component
UPlayerSkillsComponent* Skills = Character->GetSkillsComponent();

// Add skill XP
Skills->AddSkillXP(ESkillType::MeleeCombat, 50.0f);

// Get skill level
int32 Level = Skills->GetSkillLevel(ESkillType::MeleeCombat);

// Get effective skill value (with equipment bonuses)
float EffectiveValue = Skills->GetEffectiveSkillValue(ESkillType::MeleeCombat);
```

### EquipmentManagerComponent

Coordinates equipment modifiers between items and player components:

- **Features:**
  - Apply/remove equipment modifiers by unique ID
  - Automatic recalculation of total modifiers
  - Support for both attribute and skill modifiers
  - Prevents duplicate modifiers from same equipment

**Example Usage in Blueprint:**
```cpp
// Get equipment manager
UEquipmentManagerComponent* EquipManager = Character->GetEquipmentManagerComponent();

// Create a modifier for a sword
FEquipmentModifier SwordModifier(FName("Sword_1"));
SwordModifier.SkillModifiers.Add(ESkillType::MeleeCombat, 5.0f);
SwordModifier.AttributeModifiers.Add(EAttributeType::MaxHP, 10.0f);

// Apply the modifier
EquipManager->ApplyEquipmentModifier(SwordModifier);

// Remove when unequipping
EquipManager->RemoveEquipmentModifier(FName("Sword_1"));
```

## Integration

### Using with ACharacter

The `AMMORPGCharacter` class demonstrates how to integrate these components:

```cpp
#include "MMORPGCharacter.h"

// Character automatically creates all three components
AMMORPGCharacter* Character = GetWorld()->SpawnActor<AMMORPGCharacter>();

// Access components
UPlayerAttributesComponent* Attributes = Character->GetAttributesComponent();
UPlayerSkillsComponent* Skills = Character->GetSkillsComponent();
UEquipmentManagerComponent* Equipment = Character->GetEquipmentManagerComponent();
```

### Custom Character Classes

You can create your own character classes and add these components:

```cpp
UCLASS()
class AMyCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AMyCharacter()
    {
        AttributesComponent = CreateDefaultSubobject<UPlayerAttributesComponent>(TEXT("Attributes"));
        SkillsComponent = CreateDefaultSubobject<UPlayerSkillsComponent>(TEXT("Skills"));
        EquipmentManager = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("Equipment"));
    }

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UPlayerAttributesComponent* AttributesComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UPlayerSkillsComponent* SkillsComponent;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UEquipmentManagerComponent* EquipmentManager;
};
```

## Skill Progression

Skills use an XP-based progression system with a configurable exponential curve:

- **BaseXPForLevel2**: XP required to reach level 2 (default: 100)
- **XPMultiplierPerLevel**: Multiplier applied per level (default: 1.5)

Formula for level N:
```
XP_Required = BaseXPForLevel2 * (XPMultiplierPerLevel ^ (N - 1))
```

Example progression with defaults:
- Level 1 → 2: 100 XP
- Level 2 → 3: 150 XP
- Level 3 → 4: 225 XP
- Level 4 → 5: 337.5 XP

## Networking

All components are designed with networking in mind:

- Components are marked as replicated
- Key properties are replicated automatically
- Changes on server will propagate to clients
- All components call `SetIsReplicatedComponent(true)` in constructor

## Persistence

While not implemented in this initial version, the system is designed for easy persistence:

1. All important data is in serializable UPROPERTY fields
2. Components can be easily saved/loaded using Unreal's save game system
3. Consider implementing:
   - `SaveGame` class with component data
   - Serialize to JSON for database storage
   - Custom serialization for specific backend needs

## Extension Points

The system is designed to be modular and extensible:

1. **Custom Attributes**: Add new attribute types to `EAttributeType` enum
2. **Custom Skills**: Add new skill types to `ESkillType` enum
3. **Custom Progression**: Override `CalculateLevelFromXP()` for custom curves
4. **Events**: Add delegates for level-up, attribute changes, etc.
5. **Buffs/Debuffs**: Extend modifier system to include temporary effects

## Best Practices

1. **Use EquipmentManagerComponent**: Don't manually apply modifiers to attributes/skills
2. **Unique Modifier IDs**: Use unique FName for each equipment piece
3. **Clamp Values**: Components automatically clamp values, but validate inputs
4. **Network Authority**: Only modify values on the server in multiplayer
5. **Blueprint Integration**: Expose custom logic through BlueprintCallable functions

## Example Scenarios

### Taking Damage
```cpp
void AMyCharacter::TakeDamage(float Damage)
{
    if (HasAuthority()) // Server only
    {
        AttributesComponent->ModifyHP(-Damage);
        
        if (AttributesComponent->GetHP() <= 0.0f)
        {
            OnDeath();
        }
    }
}
```

### Gaining XP and Leveling Up
```cpp
void AMyCharacter::GainCombatXP(float XP)
{
    if (HasAuthority())
    {
        int32 OldLevel = SkillsComponent->GetSkillLevel(ESkillType::MeleeCombat);
        SkillsComponent->AddSkillXP(ESkillType::MeleeCombat, XP);
        int32 NewLevel = SkillsComponent->GetSkillLevel(ESkillType::MeleeCombat);
        
        if (NewLevel > OldLevel)
        {
            OnSkillLevelUp(ESkillType::MeleeCombat, NewLevel);
        }
    }
}
```

### Equipping Items
```cpp
void AMyCharacter::EquipWeapon(AWeaponActor* Weapon)
{
    if (HasAuthority())
    {
        FEquipmentModifier WeaponMod(Weapon->GetUniqueID());
        WeaponMod.SkillModifiers.Add(ESkillType::MeleeCombat, Weapon->GetMeleeBonus());
        WeaponMod.AttributeModifiers.Add(EAttributeType::MaxHP, Weapon->GetHealthBonus());
        
        EquipmentManager->ApplyEquipmentModifier(WeaponMod);
    }
}
```

## Future Enhancements

Potential additions to the system:

1. Attribute regeneration (HP/Mana/Stamina over time)
2. Buff/Debuff system with duration tracking
3. Achievement tracking based on skill levels
4. Skill requirements for equipment
5. Talent/perk trees based on skill levels
6. Save/load integration with database
7. Statistics tracking (damage dealt, healing done, etc.)
8. Event delegates for UI updates
