# Quick Start Guide - Faction and Mission Systems

## Setting Up Factions (In Unreal Editor)

### Create Faction Data Assets

1. Open your Unreal project
2. In Content Browser, create a new folder: `Content/Data/Factions`
3. Create four Data Assets:

**Syndicate (Neutral):**
- Right-click → Miscellaneous → Data Asset
- Choose `FactionDataAsset`
- Name: `DA_Faction_Syndicate`
- Configure:
  - FactionName: `Syndicate`
  - FactionDescription: `"The neutral trading faction"`
  - bIsNeutralFaction: ✓ (checked)
  - FactionColor: RGB(128, 128, 128)

**Orange Faction:**
- Name: `DA_Faction_Orange`
- FactionName: `Orange`
- FactionDescription: `"Orange competitive faction"`
- bIsNeutralFaction: ☐ (unchecked)
- FactionColor: RGB(255, 128, 0)

**Green Faction:**
- Name: `DA_Faction_Green`
- FactionName: `Green`
- FactionDescription: `"Green competitive faction"`
- bIsNeutralFaction: ☐
- FactionColor: RGB(0, 255, 0)

**Blue Faction:**
- Name: `DA_Faction_Blue`
- FactionName: `Blue`
- FactionDescription: `"Blue competitive faction"`
- bIsNeutralFaction: ☐
- FactionColor: RGB(0, 128, 255)

## Setting Up Territories

1. In your level, add TerritoryActor instances
2. For each TerritoryActor:
   - Set TerritoryName (e.g., "North District")
   - Set TerritoryDescription
   - Assign OwningFaction (drag a faction data asset)
   - Set bAllowNeutralFaction (default: true)

## Creating Missions (Blueprint or C++)

### In Blueprint:

1. Get Game Instance
2. Get Subsystem → MissionManager
3. Call "Create Mission"
   - MissionClass: Choose mission type class
   - Territory: Reference to territory actor
   - Faction: Reference to faction data asset
4. Call "Start Mission" on returned mission

### In C++:

```cpp
UMissionManager* MissionManager = GetGameInstance()->GetSubsystem<UMissionManager>();
UFactionDataAsset* Faction = /* Your faction data asset */;
ATerritoryActor* Territory = /* Your territory actor */;

// Create a clearing mission
UMissionBase* Mission = MissionManager->CreateMission(
    UTerritoryClearingMission::StaticClass(),
    Territory,
    Faction
);

Mission->StartMission();
```

## Mission Progress Tracking

### Territory Clearing Mission:
```cpp
UTerritoryClearingMission* ClearMission = Cast<UTerritoryClearingMission>(Mission);
// When enemy is killed:
ClearMission->AddEnemyCleared();
```

### Resource Collection Mission:
```cpp
UResourceCollectionMission* CollectMission = Cast<UResourceCollectionMission>(Mission);
// When resource is gathered:
CollectMission->AddResourceCollected(1);
```

### Territory Expansion Mission:
```cpp
UTerritoryExpansionMission* ExpansionMission = Cast<UTerritoryExpansionMission>(Mission);
// When checkpoint reached:
ExpansionMission->ReachCheckpoint();
// When convoy takes damage:
ExpansionMission->DamageConvoy(25.0f);
```

## Checking Mission Completion

```cpp
if (Mission->GetMissionStatus() == EMissionStatus::Completed)
{
    int32 Reward = Mission->GetMissionReward(); // Returns 1000
    // Award to player
}
```

## Common Use Cases

### Check if Player Can Work in Territory:
```cpp
UFactionDataAsset* PlayerFaction = /* Get from player */;
ATerritoryActor* Territory = /* Get from world */;

if (Territory->CanPlayerBeEmployed(PlayerFaction))
{
    // Allow player to accept missions here
}
```

### Get All Active Missions:
```cpp
UMissionManager* MissionManager = GetGameInstance()->GetSubsystem<UMissionManager>();
TArray<UMissionBase*> ActiveMissions = MissionManager->GetActiveMissions();
```

### Change Territory Ownership:
```cpp
ATerritoryActor* Territory = /* Your territory */;
UFactionDataAsset* NewFaction = /* New owning faction */;
Territory->SetOwningFaction(NewFaction);
```

## Blueprint Example Workflow

1. **Game Start:**
   - Load faction data assets
   - Place/spawn territory actors in world
   - Get MissionManager subsystem reference

2. **Player Arrives at Territory:**
   - Get player's faction
   - Check CanPlayerBeEmployed
   - Show available missions if allowed

3. **Player Accepts Mission:**
   - Call CreateMission on MissionManager
   - Store mission reference
   - Call StartMission
   - Set up UI to track progress

4. **During Mission:**
   - Track mission-specific events (kills, collection, etc.)
   - Call appropriate progress methods
   - Update UI with mission status

5. **Mission Complete:**
   - Check GetMissionStatus() == Completed
   - Call AwardMissionReward
   - Update player credits
   - Remove mission from active list

## Tips

- Store faction data assets in a DataTable or GameInstance for easy access
- Use the MissionManager to centralize all mission logic
- Extend mission classes for custom behavior
- Use Blueprint-callable functions for UI integration
- Territory actors can be placed at design time or spawned at runtime
- All missions track their status automatically

## Debugging

Check mission state:
```cpp
UE_LOG(LogTemp, Warning, TEXT("Mission Status: %d"), (int32)Mission->GetMissionStatus());
UE_LOG(LogTemp, Warning, TEXT("Mission Type: %d"), (int32)Mission->GetMissionType());
UE_LOG(LogTemp, Warning, TEXT("Is Instanced: %s"), Mission->bIsInstanced ? TEXT("Yes") : TEXT("No"));
```

Check territory ownership:
```cpp
UFactionDataAsset* Owner = Territory->GetOwningFaction();
if (Owner)
{
    UE_LOG(LogTemp, Warning, TEXT("Territory owned by: %s"), *Owner->FactionName.ToString());
}
```
