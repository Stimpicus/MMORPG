# Resource Gathering System

## Overview
This resource gathering system provides a modular framework for collecting resources in the MMORPG game. The system is designed to be extensible and ready for future persistence integration.

## Components

### 1. ResourceTypes.h
Defines core data types for the resource gathering system:
- **EResourceType**: Enumeration of available resource types (Wood, Stone, Iron, Herb, Crystal)
- **FResourceItem**: Structure representing a resource with type and quantity
- **EGatheringSkillType**: Enumeration of gathering skill types (Mining, Herbalism, Woodcutting)

### 2. ResourceNodeActor
An actor that represents a resource node in the game world.

**Key Features:**
- Configurable resource type and quantity
- Resource regeneration over time (optional)
- Associated gathering skill for experience gain
- Experience points awarded per gather

**Usage in Blueprint/C++:**
```cpp
// Spawn a resource node
AResourceNodeActor* WoodNode = GetWorld()->SpawnActor<AResourceNodeActor>();
WoodNode->ResourceType = EResourceType::Wood;
WoodNode->CurrentQuantity = 100;
WoodNode->MaxQuantity = 100;
WoodNode->bRegenerates = true;
WoodNode->RegenerationInterval = 60.0f; // Regenerate every 60 seconds
WoodNode->AssociatedSkill = EGatheringSkillType::Woodcutting;
```

### 3. InventoryComponent
A component that manages resource storage for any actor (typically a player character).

**Key Features:**
- Store multiple resource types with quantities
- Optional max stack size per resource type
- Add and remove resources
- Query resource quantities

**Usage in Blueprint/C++:**
```cpp
// Add to player character
UInventoryComponent* Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

// Add resources
Inventory->AddResource(EResourceType::Wood, 10);

// Check resources
int32 WoodCount = Inventory->GetResourceQuantity(EResourceType::Wood);
bool HasEnoughWood = Inventory->HasResource(EResourceType::Wood, 5);
```

### 4. SkillProgressionComponent
A component that tracks and manages skill progression for gathering activities.

**Key Features:**
- Track experience and levels for each gathering skill
- Configurable experience requirements and scaling
- Automatic level-up when sufficient experience is gained
- Experience calculation with exponential scaling

**Usage in Blueprint/C++:**
```cpp
// Add to player character
USkillProgressionComponent* Skills = CreateDefaultSubobject<USkillProgressionComponent>(TEXT("Skills"));

// Add experience
bool LeveledUp = Skills->AddSkillExperience(EGatheringSkillType::Mining, 50);

// Check skill level
int32 MiningLevel = Skills->GetSkillLevel(EGatheringSkillType::Mining);
```

### 5. ResourceGatheringSubsystem
A utility class that coordinates gathering operations between resource nodes, inventory, and skill progression.

**Key Features:**
- Centralized gathering logic
- Handles the complete gathering workflow
- Returns detailed gathering results including experience and level-ups

**Usage in Blueprint/C++:**
```cpp
// Perform gathering
FGatheringResult Result = UResourceGatheringSubsystem::PerformGathering(
    ResourceNode,     // The node to gather from
    Inventory,        // Player's inventory
    SkillProgression, // Player's skill progression (optional)
    5                 // Amount to gather
);

if (Result.bSuccess)
{
    // Result.AmountGathered - how many resources were collected
    // Result.ExperienceGained - how much XP was earned
    // Result.bLeveledUp - whether a level-up occurred
}
```

## Integration Example

Here's how to integrate the system into a player character:

```cpp
// In your player character class header
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
UInventoryComponent* Inventory;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
USkillProgressionComponent* SkillProgression;

// In your player character constructor
AMyPlayerCharacter::AMyPlayerCharacter()
{
    Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
    SkillProgression = CreateDefaultSubobject<USkillProgressionComponent>(TEXT("SkillProgression"));
}

// When player interacts with a resource node
void AMyPlayerCharacter::GatherFromNode(AResourceNodeActor* Node)
{
    FGatheringResult Result = UResourceGatheringSubsystem::PerformGathering(
        Node,
        Inventory,
        SkillProgression,
        1 // Gather 1 resource at a time
    );
    
    if (Result.bSuccess)
    {
        // Update UI, play effects, etc.
        if (Result.bLeveledUp)
        {
            // Show level-up notification
        }
    }
}
```

## Persistence Integration

The system is designed with persistence in mind:

1. **UProperty Specifiers**: All data members use UPROPERTY with appropriate specifiers for serialization
2. **Structured Data**: Resources and skills are stored in maps and structures that can be easily serialized
3. **Separation of Concerns**: Logic is separated from data, making it easy to save/load state

### Future Persistence Implementation

To add persistence:

1. Implement save game functionality:
```cpp
// Save inventory
for (const auto& Pair : Inventory->ResourceInventory)
{
    SaveGame->SavedResources.Add(Pair.Key, Pair.Value);
}

// Save skill progression
for (const auto& Pair : SkillProgression->SkillData)
{
    SaveGame->SavedSkills.Add(Pair.Key, Pair.Value);
}
```

2. Implement load game functionality:
```cpp
// Load inventory
for (const auto& Pair : SaveGame->SavedResources)
{
    Inventory->AddResource(Pair.Key, Pair.Value);
}

// Load skill progression
for (const auto& Pair : SaveGame->SavedSkills)
{
    SkillProgression->SkillData.Add(Pair.Key, Pair.Value);
}
```

## Extension Points

The system is designed to be easily extended:

1. **New Resource Types**: Add entries to EResourceType enumeration
2. **New Skills**: Add entries to EGatheringSkillType enumeration
3. **Custom Gathering Logic**: Override or extend ResourceGatheringSubsystem methods
4. **Networking**: Components are ready for replication with appropriate UPROPERTY flags
5. **Resource Node Variants**: Subclass ResourceNodeActor for specialized behavior

## Blueprint Support

All classes and functions are Blueprint-accessible with appropriate specifiers:
- UCLASS() with BlueprintType
- UENUM() with BlueprintType
- USTRUCT() with BlueprintType
- UFUNCTION() with BlueprintCallable where appropriate
- UPROPERTY() with BlueprintReadWrite/BlueprintReadOnly where appropriate

This allows designers to:
- Create resource nodes in the editor
- Configure gathering parameters
- Create Blueprint-based gathering interactions
- Display inventory and skills in UI
