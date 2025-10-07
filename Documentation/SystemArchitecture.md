# System Architecture Diagram

## Class Hierarchy

```
UObject
│
├── UItem
│   ├── Base item class with properties:
│   │   - ItemName, Description, ItemID
│   │   - Weight, Value
│   │   - Stack information
│   │
│   └── UEquipmentItem
│       ├── Inherits all UItem properties
│       └── Adds:
│           - EquipmentSlot (enum)
│           - AttributeModifiers (array)
│           - SkillModifiers (array)
│           - RequiredLevel, ArmorRating
│           - ApplyModifiers() / RemoveModifiers()
│
UActorComponent
│
├── UInventoryComponent
│   ├── Manages item collection
│   ├── Properties:
│   │   - InventorySlots (array of FInventorySlot)
│   │   - MaxSlots, MaxWeight, CurrentWeight
│   └── Methods:
│       - AddItem() / RemoveItem()
│       - SortInventory()
│       - Persistence methods (stubs)
│
└── UEquipmentManagerComponent
    ├── Manages equipped items by slot
    ├── Properties:
    │   - EquippedItems (map: EEquipmentSlot -> UEquipmentItem)
    └── Methods:
        - EquipItem() / UnequipItem()
        - GetEquippedItem()
        - Event: OnEquipmentChanged
```

## Equipment Slot Enumeration

```
EEquipmentSlot (enum)
├── None
├── Head
├── Torso
├── Arms
├── Hands (worn)
├── LeftHand (held)
├── RightHand (held)
├── Legs
└── Feet
```

## Data Structures

```
FInventorySlot (struct)
├── UItem* Item
└── int32 Quantity

FAttributeModifier (struct)
├── FString AttributeName
├── float ModifierValue
└── bool bIsPercentage
```

## Component Integration Flow

```
Player Character (AActor)
│
├── UInventoryComponent
│   ├── Stores items not currently equipped
│   ├── Manages add/remove/sort
│   └── Fires OnInventoryChanged events
│
└── UEquipmentManagerComponent
    ├── Tracks equipped items by slot
    ├── Applies/removes modifiers
    └── Fires OnEquipmentChanged events

When equipping an item:
1. Item is removed from inventory
2. If slot occupied, old item is unequipped
3. New item is equipped to slot
4. Modifiers are applied
5. Event is broadcast

When unequipping an item:
1. Item modifiers are removed
2. Item is removed from slot
3. Item can be added back to inventory
4. Event is broadcast
```

## Persistence Flow (SpacetimeDB Integration)

```
Game State
    │
    ├─► SaveInventoryState()
    │       ├─► SerializeInventory()
    │       │       └─► Creates JSON representation
    │       │
    │       └─► [Future: Send to SpacetimeDB]
    │
    └─► LoadInventoryState()
            ├─► [Future: Retrieve from SpacetimeDB]
            │
            └─► DeserializeInventory(JSON)
                    └─► Reconstructs inventory from JSON
```

## Event Broadcasting

```
Inventory Changes:
    AddItem() ──┐
    RemoveItem()─┼──► OnInventoryChanged ──► UI Updates
    SortInventory()┘                      └► Other Systems

Equipment Changes:
    EquipItem() ──┐
    UnequipItem()─┼──► OnEquipmentChanged ──► UI Updates
                  │                         ├► Modifier System
                  │                         └► Visual Updates
    ApplyModifiers()
    RemoveModifiers()
```

## Modifier System Hook Points

```
UEquipmentItem::ApplyModifiers()
    │
    ├─► Future: Character->AddAttributeModifier(modifier)
    ├─► Future: Character->AddSkillModifier(modifier)
    └─► Can be extended for:
        - Stat bonuses
        - Skill improvements
        - Special effects
        - Visual changes

UEquipmentItem::RemoveModifiers()
    │
    ├─► Future: Character->RemoveAttributeModifier(modifier)
    ├─► Future: Character->RemoveSkillModifier(modifier)
    └─► Reverses all modifier effects
```

## Usage Pattern

```
Typical Item Flow:
    1. Item Created (NewObject<UEquipmentItem>())
    2. Item Added to Inventory (AddItem())
    3. Player Equips Item (EquipItem())
       - Item removed from inventory
       - Modifiers applied
       - Event broadcast
    4. Player Unequips Item (UnequipItem())
       - Modifiers removed
       - Item returned
    5. Item Added Back to Inventory (AddItem())
```

## Extension Points

The system is designed for easy extension:

1. **Custom Item Types**: Extend UItem or UEquipmentItem
2. **Custom Modifiers**: Add new modifier types to FAttributeModifier
3. **Custom Slots**: Extend EEquipmentSlot enum
4. **Custom Logic**: Override virtual methods (Use(), ApplyModifiers(), etc.)
5. **Database Integration**: Implement persistence stubs
6. **UI Integration**: Listen to OnInventoryChanged and OnEquipmentChanged events
