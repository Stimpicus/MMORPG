# Faction and Mission Systems

This document describes the faction and mission systems implemented for the MMORPG game.

## Overview

The faction and mission systems provide:
- Faction management with territory ownership
- Player employment rules based on faction affiliation
- Three distinct mission types with instanced and open-world variations
- Modular design ready for persistence and networking integration

## Faction System

### FactionDataAsset
A UDataAsset-based class that defines factions in the game world.

**Properties:**
- `FactionName`: Unique identifier for the faction
- `FactionDescription`: Text description of the faction
- `bIsNeutralFaction`: Whether this is the neutral faction (Syndicate)
- `FactionColor`: Visual color representing the faction
- `RewardMultiplier`: Multiplier for mission rewards (future feature)

### Creating Faction Data Assets

To create the four required faction data assets in the Unreal Editor:

1. **Syndicate (Neutral Faction)**
   - Right-click in Content Browser → Miscellaneous → Data Asset
   - Choose `FactionDataAsset` as the class
   - Name: `DA_Faction_Syndicate`
   - Set properties:
     - FactionName: "Syndicate"
     - FactionDescription: "The neutral trading faction that can work with all territories"
     - bIsNeutralFaction: `true`
     - FactionColor: Grey (0.5, 0.5, 0.5, 1.0)
     - RewardMultiplier: 1.0

2. **Orange Faction**
   - Create new Data Asset
   - Name: `DA_Faction_Orange`
   - Set properties:
     - FactionName: "Orange"
     - FactionDescription: "Competitive faction controlling orange territories"
     - bIsNeutralFaction: `false`
     - FactionColor: Orange (1.0, 0.5, 0.0, 1.0)
     - RewardMultiplier: 1.0

3. **Green Faction**
   - Create new Data Asset
   - Name: `DA_Faction_Green`
   - Set properties:
     - FactionName: "Green"
     - FactionDescription: "Competitive faction controlling green territories"
     - bIsNeutralFaction: `false`
     - FactionColor: Green (0.0, 1.0, 0.0, 1.0)
     - RewardMultiplier: 1.0

4. **Blue Faction**
   - Create new Data Asset
   - Name: `DA_Faction_Blue`
   - Set properties:
     - FactionName: "Blue"
     - FactionDescription: "Competitive faction controlling blue territories"
     - bIsNeutralFaction: `false`
     - FactionColor: Blue (0.0, 0.5, 1.0, 1.0)
     - RewardMultiplier: 1.0

## Territory System

### TerritoryActor
An Actor that represents a territory in the game world with ownership and employment rules.

**Key Features:**
- Track owning faction
- Enforce employment rules (players can only work in their faction's territories)
- Syndicate (neutral) players can work in any territory

**Usage:**
```cpp
// Check if a player can be employed in a territory
bool bCanWork = Territory->CanPlayerBeEmployed(PlayerFaction);

// Change territory ownership
Territory->SetOwningFaction(NewFaction);
```

## Mission System

### Mission Types

#### 1. Territory Clearing Mission (Instanced PvE)
**Class:** `UTerritoryClearingMission`
- Instanced mission
- Players clear enemies from a territory instance
- Tracks enemies cleared vs required
- Auto-completes when all enemies are eliminated

**Usage:**
```cpp
UTerritoryClearingMission* Mission = Cast<UTerritoryClearingMission>(
    MissionManager->CreateMission(UTerritoryClearingMission::StaticClass(), Territory, Faction)
);
Mission->StartMission();
Mission->AddEnemyCleared(); // Call when enemy is defeated
```

#### 2. Resource Collection Mission (Open World)
**Class:** `UResourceCollectionMission`
- Open world mission
- Players collect resources in non-instanced areas
- Tracks resources collected vs required
- Auto-completes when quota is met

**Usage:**
```cpp
UResourceCollectionMission* Mission = Cast<UResourceCollectionMission>(
    MissionManager->CreateMission(UResourceCollectionMission::StaticClass(), Territory, Faction)
);
Mission->StartMission();
Mission->AddResourceCollected(1); // Call when resource is gathered
```

#### 3. Territory Expansion Mission (Instanced Combat Escort)
**Class:** `UTerritoryExpansionMission`
- Instanced mission
- Players escort a convoy through enemy territory
- Tracks convoy health and checkpoints reached
- Fails if convoy health reaches zero
- Completes when all checkpoints are reached

**Usage:**
```cpp
UTerritoryExpansionMission* Mission = Cast<UTerritoryExpansionMission>(
    MissionManager->CreateMission(UTerritoryExpansionMission::StaticClass(), Territory, Faction)
);
Mission->StartMission();
Mission->ReachCheckpoint(); // Call when checkpoint is reached
Mission->DamageConvoy(25.0f); // Call when convoy takes damage
```

### Mission Manager

**Class:** `UMissionManager`
A GameInstanceSubsystem that manages all missions.

**Key Functions:**
- `CreateMission()`: Create new mission instances
- `GetActiveMissions()`: Get all currently active missions
- `GetAvailableMissions()`: Get all available missions
- `AwardMissionReward()`: Award credits to players (stub implementation)

**Usage:**
```cpp
UMissionManager* MissionManager = GetGameInstance()->GetSubsystem<UMissionManager>();
UMissionBase* NewMission = MissionManager->CreateMission(MissionClass, Territory, Faction);
```

### Mission Rewards

All missions currently award a fixed **1,000 credits** upon completion. This is implemented as a stub in `UMissionBase::GetMissionReward()` and can be extended in the future to support:
- Variable rewards based on difficulty
- Faction reward multipliers
- Bonus rewards for performance
- Item/equipment rewards

## Architecture Notes

### Modularity
The system is designed with modularity in mind:

1. **Data-Driven**: Factions are defined as Data Assets, making it easy to add new factions
2. **Extensible**: Mission types inherit from `UMissionBase`, allowing new mission types to be added easily
3. **Persistence-Ready**: All mission state is tracked in properties that can be serialized
4. **Network-Ready**: Uses UObject-based classes that support replication when needed

### Future Extensions

The current implementation provides hooks for:
- **Persistence**: All mission and faction data uses UPROPERTY, ready for save/load systems
- **Networking**: Classes use Unreal's reflection system, ready for replication
- **Dynamic Rewards**: `RewardMultiplier` in FactionDataAsset and extensible `GetMissionReward()`
- **Complex Employment Rules**: Employment logic is centralized in `TerritoryActor::CanPlayerBeEmployed()`
- **Mission Prerequisites**: Base class structure supports adding prerequisite checking
- **Mission Chains**: Manager pattern allows tracking mission dependencies

## Integration

To integrate these systems into your game:

1. Create the four faction data assets as described above
2. Add `TerritoryActor` instances to your level and assign owning factions
3. Get the `UMissionManager` subsystem from your GameInstance
4. Create missions using the MissionManager
5. Hook up mission progress events (enemy kills, resource collection, etc.) to the appropriate mission methods
6. Award rewards through the MissionManager when missions complete

## Example Workflow

```cpp
// Setup
UMissionManager* MissionManager = GetGameInstance()->GetSubsystem<UMissionManager>();
UFactionDataAsset* OrangeFaction = LoadObject<UFactionDataAsset>(nullptr, TEXT("/Game/Data/DA_Faction_Orange"));
ATerritoryActor* Territory = /* get territory from world */;

// Create and start a mission
UTerritoryClearingMission* Mission = Cast<UTerritoryClearingMission>(
    MissionManager->CreateMission(UTerritoryClearingMission::StaticClass(), Territory, OrangeFaction)
);
Mission->StartMission();

// During gameplay
Mission->AddEnemyCleared(); // Player killed an enemy

// Mission completion
if (Mission->GetMissionStatus() == EMissionStatus::Completed)
{
    int32 Reward = Mission->GetMissionReward(); // Returns 1000
    MissionManager->AwardMissionReward(Mission, PlayerCredits);
}
```
