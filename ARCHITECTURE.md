# Resource Gathering System Architecture

## System Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                    Resource Gathering System                     │
└─────────────────────────────────────────────────────────────────┘

┌──────────────────────┐         ┌──────────────────────┐
│   ResourceTypes.h    │         │  ExamplePlayer       │
│  ─────────────────   │         │  Character           │
│  - EResourceType     │         │  ──────────────────  │
│  - EGatheringSkill   │◄────────│  + Inventory         │
│  - FResourceItem     │         │  + SkillProgression  │
└──────────────────────┘         └──────────┬───────────┘
                                            │
                                            │ uses
                                            │
                        ┌───────────────────▼────────────────────┐
                        │  ResourceGatheringSubsystem            │
                        │  ────────────────────────────────────  │
                        │  + PerformGathering()                  │
                        │  + CanGatherFrom()                     │
                        └───┬───────────────┬──────────────┬────┘
                            │               │              │
              ┌─────────────▼──┐    ┌──────▼─────┐   ┌───▼────────────┐
              │ ResourceNode   │    │ Inventory  │   │ Skill          │
              │ Actor          │    │ Component  │   │ Progression    │
              │ ─────────────  │    │ ──────────│   │ Component      │
              │ - Type         │    │ - Resources│   │ - SkillData    │
              │ - Quantity     │    │            │   │ - Experience   │
              │ - Regenerates  │    │ + Add()    │   │ + AddExp()     │
              │ + Gather()     │    │ + Remove() │   │ + GetLevel()   │
              └────────────────┘    └────────────┘   └────────────────┘
```

## Component Relationships

### 1. Core Data Layer (ResourceTypes.h)
- Defines all enumerations and structures
- No dependencies
- Used by all other components

### 2. Actor Layer (ResourceNodeActor)
- Represents physical resource nodes in the world
- Can be placed in levels
- Manages resource availability and regeneration
- Independent of player/inventory

### 3. Component Layer
**InventoryComponent**
- Attached to any actor (typically player)
- Stores gathered resources
- Manages resource quantities

**SkillProgressionComponent**
- Attached to any actor (typically player)
- Tracks skill levels and experience
- Handles automatic leveling

### 4. Coordination Layer (ResourceGatheringSubsystem)
- Static utility class
- Coordinates between ResourceNode, Inventory, and Skills
- Provides centralized gathering logic
- Returns detailed results

### 5. Integration Layer (ExamplePlayerCharacter)
- Example implementation
- Shows component integration
- Demonstrates usage patterns

## Data Flow: Gathering Operation

```
Player Interaction
      │
      ▼
┌─────────────────────────────────────────────────┐
│ ExamplePlayerCharacter::GatherFromNode()        │
└──────────────────┬──────────────────────────────┘
                   │
                   ▼
┌─────────────────────────────────────────────────┐
│ ResourceGatheringSubsystem::PerformGathering()  │
└──────┬────────────────┬──────────────┬──────────┘
       │                │              │
       ▼                ▼              ▼
  ┌─────────┐    ┌──────────┐   ┌─────────────┐
  │  Node   │    │Inventory │   │   Skills    │
  │ Gather()│    │  Add()   │   │ AddExp()    │
  └────┬────┘    └────┬─────┘   └──────┬──────┘
       │              │                 │
       └──────────────┴─────────────────┘
                      │
                      ▼
              ┌──────────────┐
              │FGathering    │
              │Result        │
              └──────────────┘
                      │
                      ▼
              Update UI/Effects
```

## Persistence Architecture

All components are designed for easy persistence:

```
┌────────────────────────────────────────┐
│        Save Game Data                   │
├────────────────────────────────────────┤
│  Inventory:                             │
│  - TMap<EResourceType, int32>          │
│                                         │
│  Skills:                                │
│  - TMap<EGatheringSkillType,           │
│          FGatheringSkillData>          │
│                                         │
│  Resource Nodes (optional):             │
│  - Array of node states                │
└────────────────────────────────────────┘
         │                    ▲
         │ Save               │ Load
         ▼                    │
┌────────────────────────────────────────┐
│    Persistent Storage                   │
│    (File/Database/Cloud)                │
└────────────────────────────────────────┘
```

## Extension Points

### Adding New Resource Types
1. Edit `EResourceType` in ResourceTypes.h
2. No other code changes needed
3. Existing systems work automatically

### Adding New Skills
1. Edit `EGatheringSkillType` in ResourceTypes.h
2. Set `AssociatedSkill` on resource nodes
3. Existing progression works automatically

### Custom Resource Nodes
```cpp
AResourceNodeActor (Base)
    │
    ├─ ARareResourceNode
    ├─ ATimedResourceNode
    └─ APlayerOwnedNode
```

### Network Replication (Future)
```
Add to components:
- UPROPERTY(Replicated)
- GetLifetimeReplicatedProps()
- ServerRPC functions for gathering
```

## Thread Safety

- All components designed for game thread
- No async operations in base implementation
- Safe for Blueprint usage
- Regeneration uses UE timer system

## Performance Characteristics

- **Memory**: O(n) for n resource types in inventory
- **Gathering**: O(1) constant time operation
- **Skill Update**: O(1) constant time per skill
- **Regeneration**: Amortized O(1) with timers

## Testing Strategy

### Unit Testing Points
1. Inventory add/remove operations
2. Skill experience calculations
3. Level-up detection
4. Resource node depletion/regeneration

### Integration Testing
1. Full gathering flow
2. Multiple resource types
3. Skill progression across levels
4. Edge cases (empty nodes, full inventory)

## Best Practices

1. **Component Reuse**: Attach components to any actor, not just players
2. **Centralized Logic**: Use ResourceGatheringSubsystem for consistency
3. **Blueprint Exposure**: All systems fully Blueprint-accessible
4. **Data-Driven**: Configure behavior through properties, not code
5. **Modularity**: Each component works independently
6. **Extensibility**: Inherit and override for custom behavior

## Future Enhancements

Potential additions:
- Tool requirements for gathering
- Gathering speed/duration
- Quality/rarity variations
- Multi-resource nodes
- Skill-based gathering bonuses
- Resource transformation/crafting
- Seasonal availability
- Network synchronization
