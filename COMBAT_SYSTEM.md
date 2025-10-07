# Combat and Abilities System

This document describes the combat and abilities system implementation for the MMORPG project.

## Overview

The combat system consists of several components that work together to provide a flexible and extensible combat framework:

1. **Combat Types** - Enums and structs defining weapon types, attack types, and resource types
2. **Resource Component** - Manages character resources (HP, Mana, Stamina)
3. **Combat Component** - Handles attack execution and combat logic
4. **Weapon Item** - Base class for weapon items in the game

## Components

### CombatTypes.h

Defines the core types used throughout the combat system:

- **EWeaponType**: Melee, Ranged, Magical
- **EAttackType**: MeleeAttack, RangedAttack, MagicalAttack
- **EResourceType**: Health, Mana, Stamina
- **FWeaponData**: Structure containing weapon properties
- **FResourceCost**: Structure defining resource costs for abilities
- **FAttackAbilityData**: Structure containing attack ability properties

### ResourceComponent

Manages character resources with the following features:

- Get/Set current and max values for HP, Mana, and Stamina
- Consume resources with validation
- Restore resources with max value clamping
- Support for multiple resource costs
- Events for resource changes (for UI updates)

**Usage Example:**
```cpp
// Get the resource component
UResourceComponent* Resources = Character->FindComponentByClass<UResourceComponent>();

// Check if enough mana
if (Resources->HasEnoughResource(EResourceType::Mana, 25.0f))
{
    // Consume mana
    Resources->ConsumeResource(EResourceType::Mana, 25.0f);
}

// Restore health
Resources->RestoreResource(EResourceType::Health, 50.0f);
```

### CombatComponent

Handles combat abilities and attacks with these features:

- Execute melee, ranged, and magical attacks
- Automatic resource consumption
- Cooldown management
- Range checking
- Damage calculation (base damage + weapon bonus)
- Events for attack execution and damage taken

**Usage Example:**
```cpp
// Get the combat component
UCombatComponent* Combat = Character->FindComponentByClass<UCombatComponent>();

// Equip a weapon
FWeaponData Sword;
Sword.WeaponName = "Iron Sword";
Sword.WeaponType = EWeaponType::Melee;
Sword.BaseDamage = 20.0f;
Sword.AttackRange = 150.0f;
Combat->SetEquippedWeapon(Sword);

// Execute a melee attack
if (Combat->ExecuteMeleeAttack(TargetActor))
{
    // Attack was successful
}

// Check cooldown
float Cooldown = Combat->GetRemainingCooldown(EAttackType::MeleeAttack);
```

### WeaponItem

Base class for weapon actors in the game:

- Contains weapon data (name, type, damage, range, speed)
- Optional visual mesh component
- Can be extended in Blueprints for specific weapon types

**Usage Example:**
```cpp
// Spawn a weapon
AWeaponItem* Weapon = GetWorld()->SpawnActor<AWeaponItem>(WeaponClass, SpawnLocation, FRotator::ZeroRotator);

// Configure weapon
FWeaponData Data;
Data.WeaponName = "Fire Staff";
Data.WeaponType = EWeaponType::Magical;
Data.BaseDamage = 35.0f;
Data.AttackRange = 500.0f;
Weapon->SetWeaponData(Data);
```

## Attack Types

### Melee Attack
- **Default Damage**: 15
- **Range**: 150 units
- **Cooldown**: 1 second
- **Resource Cost**: 10 Stamina

### Ranged Attack
- **Default Damage**: 20
- **Range**: 500 units
- **Cooldown**: 1.5 seconds
- **Resource Cost**: 15 Stamina

### Magical Attack
- **Default Damage**: 30
- **Range**: 400 units
- **Cooldown**: 2 seconds
- **Resource Cost**: 25 Mana

## Integration with Characters

To use the combat system on a character:

1. Add the `UResourceComponent` to your character class
2. Add the `UCombatComponent` to your character class
3. Configure default values in the editor or via code

```cpp
// In your character class constructor
ResourceComponent = CreateDefaultSubobject<UResourceComponent>(TEXT("ResourceComponent"));
CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
```

## Extensibility

The system is designed for easy extension:

- **New Attack Types**: Add to `EAttackType` enum and implement in `CombatComponent`
- **New Resource Types**: Add to `EResourceType` enum and update `ResourceComponent`
- **Custom Abilities**: Create new ability data structures extending `FAttackAbilityData`
- **Status Effects**: Can be added to `CombatComponent` to modify damage/costs
- **Persistence**: All data structures use `UPROPERTY` for serialization support

## Future Integration Points

The system is prepared for:

- **Inventory System**: Weapon items can be stored and equipped from inventory
- **Character Stats**: Damage calculations can factor in character attributes
- **Network Replication**: Components can be marked for replication
- **Database Persistence**: All structs are serializable for save/load
- **Animation System**: Attack events can trigger animation montages
- **Particle Effects**: Attack execution events can spawn VFX
- **Sound System**: Events can trigger combat sound effects
