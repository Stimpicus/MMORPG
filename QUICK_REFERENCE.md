# Resource Gathering System - Quick Reference

## Core Components

### ResourceTypes.h
**Enumerations:**
- `EResourceType`: None, Wood, Stone, Iron, Herb, Crystal
- `EGatheringSkillType`: None, Mining, Herbalism, Woodcutting

**Structures:**
- `FResourceItem`: Contains ResourceType and Quantity

---

### ResourceNodeActor
**Purpose:** Placeable actor representing a harvestable resource in the world

**Key Properties:**
- `ResourceType`: Type of resource this node provides
- `CurrentQuantity`: Current amount available
- `MaxQuantity`: Maximum capacity
- `bRegenerates`: Whether the node regenerates
- `RegenerationInterval`: Time between regeneration ticks
- `AssociatedSkill`: Skill type awarded XP
- `ExperiencePerGather`: XP awarded per gather

**Key Methods:**
- `GatherResource(int32 Amount)`: Gather resources, returns actual amount gathered
- `HasResourcesAvailable()`: Check if node has resources

---

### InventoryComponent
**Purpose:** Component for storing gathered resources on any actor

**Key Properties:**
- `ResourceInventory`: TMap of resources and quantities
- `MaxStackSize`: Optional stack limit per resource type

**Key Methods:**
- `AddResource(EResourceType, int32)`: Add resources to inventory
- `RemoveResource(EResourceType, int32)`: Remove resources from inventory
- `GetResourceQuantity(EResourceType)`: Query quantity of a resource
- `HasResource(EResourceType, int32)`: Check if minimum quantity exists
- `ClearInventory()`: Clear all resources

---

### SkillProgressionComponent
**Purpose:** Component for tracking gathering skill levels and experience

**Key Properties:**
- `SkillData`: TMap of skill types to skill data
- `BaseExperienceForLevel`: XP required for first level
- `ExperienceMultiplierPerLevel`: Scaling factor per level

**Key Methods:**
- `AddSkillExperience(EGatheringSkillType, int32)`: Add XP, returns true if leveled up
- `GetSkillLevel(EGatheringSkillType)`: Get current level
- `GetSkillExperience(EGatheringSkillType)`: Get current XP
- `GetExperienceToNextLevel(EGatheringSkillType)`: Get XP needed for next level
- `InitializeSkill(EGatheringSkillType)`: Initialize a skill

---

### ResourceGatheringSubsystem
**Purpose:** Static utility class coordinating gathering operations

**Key Methods:**
- `PerformGathering(ResourceNode, Inventory, SkillProgression, Amount)`: Performs complete gathering operation, returns FGatheringResult
- `CanGatherFrom(ResourceNode, Inventory)`: Check if gathering is possible

**Result Structure (FGatheringResult):**
- `bSuccess`: Whether gathering succeeded
- `AmountGathered`: How many resources were collected
- `ExperienceGained`: How much XP was earned
- `bLeveledUp`: Whether a skill level-up occurred

---

## Integration Pattern

### 1. Add Components to Player Character
```cpp
UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
UInventoryComponent* Inventory;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
USkillProgressionComponent* SkillProgression;

// In constructor
Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
SkillProgression = CreateDefaultSubobject<USkillProgressionComponent>(TEXT("SkillProgression"));
```

### 2. Place Resource Nodes in World
```cpp
AResourceNodeActor* Node = GetWorld()->SpawnActor<AResourceNodeActor>();
Node->ResourceType = EResourceType::Wood;
Node->CurrentQuantity = 100;
Node->AssociatedSkill = EGatheringSkillType::Woodcutting;
```

### 3. Implement Gathering Interaction
```cpp
void GatherFromNode(AResourceNodeActor* Node)
{
    FGatheringResult Result = UResourceGatheringSubsystem::PerformGathering(
        Node, Inventory, SkillProgression, 1
    );
    
    if (Result.bSuccess)
    {
        // Show UI feedback
        if (Result.bLeveledUp)
        {
            // Show level-up notification
        }
    }
}
```

---

## Blueprint Usage

All classes are fully Blueprint-accessible:

1. **Create Resource Nodes:** Drag ResourceNodeActor into level, configure in details panel
2. **Add Components:** Add InventoryComponent and SkillProgressionComponent to your character Blueprint
3. **Create Gathering Logic:** Use "Perform Gathering" Blueprint node
4. **Query Data:** Use Blueprint-accessible getters for UI

---

## Persistence Ready

All data uses UPROPERTY with appropriate specifiers:
- Inventory: Save/load `ResourceInventory` TMap
- Skills: Save/load `SkillData` TMap
- Nodes: Save/load position, ResourceType, CurrentQuantity

Example save pattern:
```cpp
// Save
SaveGame->Resources = Inventory->GetAllResources();
SaveGame->Skills = SkillProgression->SkillData;

// Load
for (auto& Pair : SaveGame->Resources)
    Inventory->AddResource(Pair.Key, Pair.Value);
SkillProgression->SkillData = SaveGame->Skills;
```

---

## Extensibility

### Add New Resource Types
Edit `EResourceType` in ResourceTypes.h:
```cpp
enum class EResourceType : uint8
{
    // ... existing types
    Gold UMETA(DisplayName = "Gold"),
    Gem UMETA(DisplayName = "Gem")
};
```

### Add New Skills
Edit `EGatheringSkillType` in ResourceTypes.h:
```cpp
enum class EGatheringSkillType : uint8
{
    // ... existing skills
    Fishing UMETA(DisplayName = "Fishing")
};
```

### Create Custom Node Types
```cpp
UCLASS()
class ARareResourceNode : public AResourceNodeActor
{
    // Custom behavior
};
```

---

## Example Character

See `ExamplePlayerCharacter` for a complete integration example showing:
- Component setup in constructor
- Skill initialization in BeginPlay
- Gathering interaction method
- Blueprint-accessible query methods
