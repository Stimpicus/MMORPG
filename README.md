# MMORPG Project

An Unreal Engine 5 MMORPG game project with a comprehensive skills and attributes system.

## Features

### Skills and Attributes System
A modular, network-ready system for player character progression. See [Documentation](Docs/SkillsAndAttributesSystem.md) for detailed information.

**Components:**
- **PlayerAttributesComponent** - Manages XP, HP, Mana, Stamina, and Credits
- **PlayerSkillsComponent** - Handles 7 skill types with XP-based progression
- **EquipmentManagerComponent** - Coordinates equipment modifiers

**Skill Types:**
1. Toughness
2. Mana Efficiency
3. Stamina Efficiency
4. Melee Combat
5. Ranged Combat
6. Magical Ability
7. Resource Gathering

### Key Features:
- ✅ Persistent attribute values (XP, HP, Mana, Stamina, Credits)
- ✅ XP-based skill progression with configurable curves
- ✅ Equipment-based modifiers for attributes and skills
- ✅ Network replication support
- ✅ Blueprint-accessible API
- ✅ Modular design for easy extension

## Project Structure

```
MMORPG/
├── Source/MMORPG/
│   ├── public/
│   │   ├── PlayerAttributesComponent.h
│   │   ├── PlayerSkillsComponent.h
│   │   ├── EquipmentManagerComponent.h
│   │   ├── EquipmentModifier.h
│   │   ├── MMORPGCharacter.h
│   │   └── ...
│   └── private/
│       ├── PlayerAttributesComponent.cpp
│       ├── PlayerSkillsComponent.cpp
│       ├── EquipmentManagerComponent.cpp
│       ├── MMORPGCharacter.cpp
│       └── ...
├── Docs/
│   └── SkillsAndAttributesSystem.md
└── Content/
    └── ... (Unreal assets)
```

## Getting Started

### Using the Character Components

```cpp
#include "MMORPGCharacter.h"

// Spawn a character with all components
AMMORPGCharacter* Character = GetWorld()->SpawnActor<AMMORPGCharacter>();

// Access components
UPlayerAttributesComponent* Attributes = Character->GetAttributesComponent();
UPlayerSkillsComponent* Skills = Character->GetSkillsComponent();
UEquipmentManagerComponent* Equipment = Character->GetEquipmentManagerComponent();

// Modify attributes
Attributes->AddXP(100.0f);
Attributes->ModifyHP(-10.0f);

// Gain skill XP
Skills->AddSkillXP(ESkillType::MeleeCombat, 50.0f);

// Apply equipment modifier
FEquipmentModifier Modifier(FName("Sword"));
Modifier.SkillModifiers.Add(ESkillType::MeleeCombat, 5.0f);
Equipment->ApplyEquipmentModifier(Modifier);
```

## Documentation

- [Skills and Attributes System](Docs/SkillsAndAttributesSystem.md) - Complete documentation on the progression system

## Building the Project

This is an Unreal Engine 5.6 project. To build:

1. Ensure you have Unreal Engine 5.6 installed
2. Right-click on `MMORPG.uproject` and select "Generate Visual Studio project files"
3. Open the generated solution file
4. Build the project in your IDE

## Requirements

- Unreal Engine 5.6
- Visual Studio 2022 (Windows) or Xcode (macOS)
- C++17 compatible compiler

## License

Copyright Epic Games, Inc. All Rights Reserved.
