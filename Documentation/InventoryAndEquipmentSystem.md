# Equipment and Inventory System Documentation

## Overview
This document describes the equipment and inventory systems implemented for the MMORPG project. The systems are designed to be modular, extensible, and ready for integration with SpacetimeDB.

## Architecture

The system consists of four main components:

### 1. Base Item Class (`UItem`)
The foundation for all items in the game.

**Properties:**
- `ItemName` - Display name of the item
- `Description` - Item description
- `ItemID` - Unique identifier
- `Weight` - Item weight (affects inventory capacity)
- `Value` - Item value (for trading/selling)
- `MaxStackSize` - Maximum stack size for stackable items
- `CurrentStackSize` - Current stack size
- `bIsStackable` - Whether the item can be stacked

**Methods:**
- `Use()` - Virtual method for item usage (override in derived classes)
- `GetItemInfo()` - Returns formatted item information

### 2. Equipment Item Class (`UEquipmentItem`)
Extends `UItem` for equippable items.

**Equipment Slots:**
- Head
- Torso
- Arms
- Hands (worn)
- Left Hand (held)
- Right Hand (held)
- Legs
- Feet

**Properties:**
- `EquipmentSlot` - The slot this item occupies
- `AttributeModifiers` - Array of attribute modifiers (e.g., +10 Strength)
- `SkillModifiers` - Array of skill modifiers (e.g., +5% Smithing)
- `RequiredLevel` - Minimum level required to equip
- `ArmorRating` - Armor value for defensive items

**Methods:**
- `ApplyModifiers()` - Hook for applying attribute/skill modifications
- `RemoveModifiers()` - Hook for removing attribute/skill modifications

### 3. Equipment Manager Component (`UEquipmentManagerComponent`)
Actor component that manages equipped items.

**Features:**
- Tracks all equipped items by slot
- Handles equip/unequip operations
- Manages modifier application/removal
- Fires events when equipment changes

**Key Methods:**
- `EquipItem(UEquipmentItem*)` - Equips an item, returns previously equipped item
- `UnequipItem(EEquipmentSlot)` - Unequips an item from a slot
- `GetEquippedItem(EEquipmentSlot)` - Gets the item in a specific slot
- `IsSlotOccupied(EEquipmentSlot)` - Checks if a slot has an item
- `GetAllEquippedItems()` - Returns all equipped items
- `UnequipAll()` - Unequips all items
- `CanEquipItem(UEquipmentItem*)` - Checks if an item can be equipped

**Events:**
- `OnEquipmentChanged` - Fires when equipment changes (slot, item)

### 4. Inventory Component (`UInventoryComponent`)
Actor component that manages a character's inventory.

**Features:**
- Slot-based inventory system
- Weight-based capacity system
- Stackable item support
- Sorting functionality
- Persistent storage stubs for SpacetimeDB

**Key Properties:**
- `MaxSlots` - Maximum number of inventory slots (default: 30)
- `MaxWeight` - Maximum weight capacity (default: 100.0)
- `CurrentWeight` - Current total weight of items
- `InventorySlots` - Array of inventory slots

**Key Methods:**
- `AddItem(UItem*, Quantity)` - Adds items to inventory
- `RemoveItem(UItem*, Quantity)` - Removes items from inventory
- `RemoveItemFromSlot(SlotIndex, Quantity)` - Removes from specific slot
- `GetItemAtSlot(SlotIndex)` - Gets item at a slot
- `FindItem(UItem*)` - Finds an item's slot index
- `HasItem(UItem*, Quantity)` - Checks if inventory contains item
- `GetItemQuantity(UItem*)` - Gets total quantity of an item
- `SortInventory()` - Sorts inventory by item ID and quantity
- `GetEmptySlotCount()` - Returns number of empty slots
- `IsFull()` - Checks if inventory is full
- `GetCurrentWeight()` - Returns current weight
- `ClearInventory()` - Removes all items

**Persistence Methods (Stubs for SpacetimeDB):**
- `SaveInventoryState()` - Saves inventory to persistent storage
- `LoadInventoryState()` - Loads inventory from persistent storage
- `SerializeInventory()` - Converts inventory to JSON string
- `DeserializeInventory(FString)` - Loads inventory from JSON string

**Events:**
- `OnInventoryChanged` - Fires when inventory contents change

## Usage Examples

### Creating Items

```cpp
// Create a basic item
UItem* Potion = NewObject<UItem>();
Potion->ItemName = TEXT("Health Potion");
Potion->Description = TEXT("Restores 50 HP");
Potion->ItemID = 1001;
Potion->Weight = 0.5f;
Potion->Value = 25;
Potion->bIsStackable = true;
Potion->MaxStackSize = 20;

// Create an equipment item
UEquipmentItem* Helmet = NewObject<UEquipmentItem>();
Helmet->ItemName = TEXT("Iron Helmet");
Helmet->Description = TEXT("Basic iron helmet");
Helmet->ItemID = 2001;
Helmet->Weight = 2.0f;
Helmet->Value = 100;
Helmet->EquipmentSlot = EEquipmentSlot::Head;
Helmet->RequiredLevel = 5;
Helmet->ArmorRating = 10;

// Add attribute modifier
FAttributeModifier DefenseModifier;
DefenseModifier.AttributeName = TEXT("Defense");
DefenseModifier.ModifierValue = 5.0f;
DefenseModifier.bIsPercentage = false;
Helmet->AttributeModifiers.Add(DefenseModifier);
```

### Using the Inventory Component

```cpp
// Add component to an actor
UInventoryComponent* Inventory = Character->FindComponentByClass<UInventoryComponent>();

// Add items
Inventory->AddItem(Potion, 5);  // Add 5 potions
Inventory->AddItem(Helmet, 1);  // Add 1 helmet

// Check if item exists
if (Inventory->HasItem(Potion, 3))
{
    // Player has at least 3 potions
}

// Remove items
Inventory->RemoveItem(Potion, 1);  // Use 1 potion

// Sort inventory
Inventory->SortInventory();

// Get inventory info
int32 EmptySlots = Inventory->GetEmptySlotCount();
float CurrentWeight = Inventory->GetCurrentWeight();
```

### Using the Equipment Manager

```cpp
// Add component to an actor
UEquipmentManagerComponent* Equipment = Character->FindComponentByClass<UEquipmentManagerComponent>();

// Equip an item
UEquipmentItem* OldHelmet = Equipment->EquipItem(Helmet);
if (OldHelmet)
{
    // Previous helmet was unequipped, add it to inventory
    Inventory->AddItem(OldHelmet, 1);
}

// Check what's equipped
UEquipmentItem* CurrentHelmet = Equipment->GetEquippedItem(EEquipmentSlot::Head);

// Unequip an item
UEquipmentItem* UnequippedItem = Equipment->UnequipItem(EEquipmentSlot::Head);
if (UnequippedItem)
{
    Inventory->AddItem(UnequippedItem, 1);
}

// Listen to equipment changes
Equipment->OnEquipmentChanged.AddDynamic(this, &AMyCharacter::OnEquipmentChanged);
```

### Blueprint Integration

All classes and methods are marked as `BlueprintCallable` or `BlueprintReadWrite` where appropriate, allowing full Blueprint integration:

- Create custom item types in Blueprint by extending `UItem` or `UEquipmentItem`
- Add Equipment Manager and Inventory components to character Blueprints
- Use Blueprint nodes to interact with the inventory and equipment systems
- Bind to events like `OnEquipmentChanged` and `OnInventoryChanged`

## Modular Design

The system is designed with modularity in mind:

1. **Component-Based**: Both inventory and equipment are ActorComponents that can be added to any actor
2. **Extensible**: Base item and equipment classes can be extended for specific item types
3. **Event-Driven**: Events broadcast changes, allowing UI and other systems to react
4. **Database-Ready**: Persistence stubs are in place for SpacetimeDB integration
5. **Blueprint-Friendly**: All functionality is exposed to Blueprints

## Future Integration with SpacetimeDB

The system includes stubs for SpacetimeDB integration:

1. `SaveInventoryState()` - Will serialize and send inventory to SpacetimeDB
2. `LoadInventoryState()` - Will retrieve and deserialize inventory from SpacetimeDB
3. `SerializeInventory()` - JSON serialization helper
4. `DeserializeInventory()` - JSON deserialization helper

When integrating with SpacetimeDB:
- Implement actual database calls in the persistence methods
- Add network synchronization for multiplayer support
- Implement item database lookup by ItemID
- Add server-side validation for inventory operations

## Testing Recommendations

When testing the system:

1. Test inventory capacity (slot and weight limits)
2. Test stackable vs non-stackable items
3. Test equip/unequip with full inventory
4. Test equipment slot restrictions
5. Test modifier application/removal
6. Test inventory sorting
7. Test persistence serialization/deserialization
8. Test Blueprint integration
9. Test event broadcasting

## Notes

- Item instances are managed as UObjects and should be properly referenced
- Weight and slot limits can be adjusted per character/class if needed
- Attribute and skill modifier application requires a character/attribute system to be implemented
- The system is thread-safe for single-player but will need synchronization for multiplayer
