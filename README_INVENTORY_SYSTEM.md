# MMORPG - Equipment and Inventory System

## Quick Start

This project now includes a complete equipment and inventory system for player characters.

### Components Added

1. **Item System** (`UItem`)
   - Base class for all items in the game
   - Supports stackable and non-stackable items
   - Properties: Name, Description, Weight, Value, etc.

2. **Equipment System** (`UEquipmentItem`, `UEquipmentManagerComponent`)
   - Equipment items with attribute/skill modifiers
   - 8 equipment slots: Head, Torso, Arms, Hands, Left Hand, Right Hand, Legs, Feet
   - Equip/unequip functionality with modifier hooks
   - Events for equipment changes

3. **Inventory System** (`UInventoryComponent`)
   - Slot-based inventory management
   - Weight-based capacity system
   - Add/remove/sort operations
   - Persistent storage interface (ready for SpacetimeDB)

### File Structure

```
Source/MMORPG/
├── public/
│   ├── Item.h
│   ├── EquipmentItem.h
│   ├── EquipmentManagerComponent.h
│   └── InventoryComponent.h
├── private/
│   ├── Item.cpp
│   ├── EquipmentItem.cpp
│   ├── EquipmentManagerComponent.cpp
│   └── InventoryComponent.cpp
Documentation/
└── InventoryAndEquipmentSystem.md
```

### Usage

#### In C++:
```cpp
// Add components to your character
UInventoryComponent* Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
UEquipmentManagerComponent* Equipment = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("Equipment"));

// Create and add items
UItem* HealthPotion = NewObject<UItem>();
HealthPotion->ItemName = TEXT("Health Potion");
Inventory->AddItem(HealthPotion, 5);

// Equip items
UEquipmentItem* Helmet = NewObject<UEquipmentItem>();
Helmet->EquipmentSlot = EEquipmentSlot::Head;
Equipment->EquipItem(Helmet);
```

#### In Blueprints:
1. Add `InventoryComponent` and `EquipmentManagerComponent` to your character Blueprint
2. Create item Blueprints extending `Item` or `EquipmentItem`
3. Use Blueprint nodes to add/remove items and equip/unequip equipment

### Key Features

✅ Modular component-based architecture  
✅ Full Blueprint support  
✅ Stackable item support  
✅ Weight and slot-based inventory limits  
✅ Equipment slot system with 8 slots  
✅ Attribute and skill modifier hooks  
✅ Event-driven updates  
✅ Persistent storage stubs (ready for SpacetimeDB)  

### Documentation

For detailed documentation, usage examples, and integration guidelines, see:
- [Inventory and Equipment System Documentation](Documentation/InventoryAndEquipmentSystem.md)

### Next Steps

The system is ready for integration with:
- Character attribute systems
- UI/HUD displays
- SpacetimeDB for persistent storage
- Multiplayer synchronization

All modifier hooks are in place and will automatically work once a character/attribute system is implemented.
