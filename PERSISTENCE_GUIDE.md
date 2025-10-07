# Persistence Integration Guide

This guide explains how the combat system is designed for future persistence integration and how to implement save/load functionality.

## Serializable Structures

All core data structures are designed to be serializable using Unreal's property system:

### FWeaponData
```cpp
USTRUCT(BlueprintType)
struct FWeaponData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FString WeaponName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    EWeaponType WeaponType;
    
    // ... other properties
};
```

All properties marked with `UPROPERTY()` are automatically serializable.

## Save System Integration

### Option 1: Using SaveGame Objects

```cpp
UCLASS()
class UMyGameSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    // Character resources
    UPROPERTY()
    float SavedHealth;
    
    UPROPERTY()
    float SavedMana;
    
    UPROPERTY()
    float SavedStamina;
    
    // Equipped weapon
    UPROPERTY()
    FWeaponData EquippedWeapon;
    
    // Custom abilities (if modified)
    UPROPERTY()
    FAttackAbilityData MeleeAbility;
    
    UPROPERTY()
    FAttackAbilityData RangedAbility;
    
    UPROPERTY()
    FAttackAbilityData MagicalAbility;
};

// Saving
void SaveCharacterData(ACombatCharacter* Character)
{
    UMyGameSaveGame* SaveGameInstance = Cast<UMyGameSaveGame>(
        UGameplayStatics::CreateSaveGameObject(UMyGameSaveGame::StaticClass())
    );
    
    if (Character->ResourceComponent)
    {
        SaveGameInstance->SavedHealth = Character->ResourceComponent->GetCurrentResource(EResourceType::Health);
        SaveGameInstance->SavedMana = Character->ResourceComponent->GetCurrentResource(EResourceType::Mana);
        SaveGameInstance->SavedStamina = Character->ResourceComponent->GetCurrentResource(EResourceType::Stamina);
    }
    
    if (Character->CombatComponent)
    {
        SaveGameInstance->EquippedWeapon = Character->CombatComponent->GetEquippedWeapon();
    }
    
    UGameplayStatics::SaveGameToSlot(SaveGameInstance, "PlayerSave", 0);
}

// Loading
void LoadCharacterData(ACombatCharacter* Character)
{
    UMyGameSaveGame* LoadGameInstance = Cast<UMyGameSaveGame>(
        UGameplayStatics::LoadGameFromSlot("PlayerSave", 0)
    );
    
    if (LoadGameInstance)
    {
        if (Character->ResourceComponent)
        {
            Character->ResourceComponent->RestoreResource(EResourceType::Health, LoadGameInstance->SavedHealth);
            Character->ResourceComponent->RestoreResource(EResourceType::Mana, LoadGameInstance->SavedMana);
            Character->ResourceComponent->RestoreResource(EResourceType::Stamina, LoadGameInstance->SavedStamina);
        }
        
        if (Character->CombatComponent)
        {
            Character->CombatComponent->SetEquippedWeapon(LoadGameInstance->EquippedWeapon);
        }
    }
}
```

### Option 2: JSON Serialization

```cpp
#include "JsonObjectConverter.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

FString SerializeWeaponToJson(const FWeaponData& Weapon)
{
    FString JsonString;
    TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
    
    if (FJsonObjectConverter::UStructToJsonObject(FWeaponData::StaticStruct(), &Weapon, JsonObject, 0, 0))
    {
        TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
        FJsonSerializer::Serialize(JsonObject, JsonWriter);
    }
    
    return JsonString;
}

bool DeserializeWeaponFromJson(const FString& JsonString, FWeaponData& OutWeapon)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
    
    if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
    {
        return FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), FWeaponData::StaticStruct(), &OutWeapon, 0, 0);
    }
    
    return false;
}
```

### Option 3: Database Integration

For MMORPGs, you'll typically want server-side persistence:

```cpp
// Example structure for database storage
struct FCharacterPersistenceData
{
    int32 CharacterId;
    
    // Resources
    float CurrentHealth;
    float CurrentMana;
    float CurrentStamina;
    
    // Equipped weapon (could be weapon ID for database lookup)
    int32 EquippedWeaponId;
    
    // Custom ability modifications
    TArray<FAbilityModification> AbilityMods;
};

// Server-side save (pseudo-code)
void SaveToDatabase(ACombatCharacter* Character)
{
    FCharacterPersistenceData Data;
    Data.CharacterId = Character->GetUniqueID();
    
    // Populate data from components
    if (Character->ResourceComponent)
    {
        Data.CurrentHealth = Character->ResourceComponent->GetCurrentResource(EResourceType::Health);
        Data.CurrentMana = Character->ResourceComponent->GetCurrentResource(EResourceType::Mana);
        Data.CurrentStamina = Character->ResourceComponent->GetCurrentResource(EResourceType::Stamina);
    }
    
    if (Character->CombatComponent)
    {
        FWeaponData Weapon = Character->CombatComponent->GetEquippedWeapon();
        Data.EquippedWeaponId = LookupWeaponId(Weapon);
    }
    
    // Send to database
    DatabaseService->SaveCharacterData(Data);
}
```

## Network Replication

For multiplayer, you'll want to replicate the combat state:

```cpp
// In ResourceComponent.h
class MMORPG_API UResourceComponent : public UActorComponent
{
    GENERATED_BODY()

protected:
    // Mark properties for replication
    UPROPERTY(ReplicatedUsing=OnRep_CurrentHealth)
    float CurrentHealth;
    
    UPROPERTY(ReplicatedUsing=OnRep_CurrentMana)
    float CurrentMana;
    
    UPROPERTY(ReplicatedUsing=OnRep_CurrentStamina)
    float CurrentStamina;
    
    UFUNCTION()
    void OnRep_CurrentHealth();
    
    UFUNCTION()
    void OnRep_CurrentMana();
    
    UFUNCTION()
    void OnRep_CurrentStamina();
    
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

// In ResourceComponent.cpp
void UResourceComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    
    DOREPLIFETIME(UResourceComponent, CurrentHealth);
    DOREPLIFETIME(UResourceComponent, CurrentMana);
    DOREPLIFETIME(UResourceComponent, CurrentStamina);
}

void UResourceComponent::OnRep_CurrentHealth()
{
    OnResourceChanged.Broadcast(EResourceType::Health, CurrentHealth, MaxHealth);
}

void UResourceComponent::OnRep_CurrentMana()
{
    OnResourceChanged.Broadcast(EResourceType::Mana, CurrentMana, MaxMana);
}

void UResourceComponent::OnRep_CurrentStamina()
{
    OnResourceChanged.Broadcast(EResourceType::Stamina, CurrentStamina, MaxStamina);
}
```

```cpp
// In CombatComponent.h - Replicate attacks as RPCs
UFUNCTION(Server, Reliable, WithValidation)
void ServerExecuteAttack(const FAttackAbilityData& AbilityData, AActor* Target);

UFUNCTION(NetMulticast, Reliable)
void MulticastPlayAttackEffects(EAttackType AttackType, AActor* Target);

// In CombatComponent.cpp
bool UCombatComponent::ServerExecuteAttack_Validate(const FAttackAbilityData& AbilityData, AActor* Target)
{
    // Validate the attack is legal
    return true;
}

void UCombatComponent::ServerExecuteAttack_Implementation(const FAttackAbilityData& AbilityData, AActor* Target)
{
    // Execute attack on server
    if (ExecuteAttack(AbilityData, Target))
    {
        // Broadcast to all clients for visual effects
        MulticastPlayAttackEffects(AbilityData.AttackType, Target);
    }
}

void UCombatComponent::MulticastPlayAttackEffects_Implementation(EAttackType AttackType, AActor* Target)
{
    // Play animations, particles, sounds on all clients
}
```

## Data Table Integration

For managing large numbers of weapons/abilities:

```cpp
// Create a data table asset in the editor
// Content/Data/WeaponDataTable.uasset

// Row structure
USTRUCT(BlueprintType)
struct FWeaponTableRow : public FTableRowBase
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FWeaponData WeaponData;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* Mesh;
};

// Loading from data table
FWeaponData LoadWeaponFromDataTable(FName WeaponId)
{
    UDataTable* WeaponTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Data/WeaponDataTable"));
    
    if (WeaponTable)
    {
        FWeaponTableRow* Row = WeaponTable->FindRow<FWeaponTableRow>(WeaponId, TEXT(""));
        if (Row)
        {
            return Row->WeaponData;
        }
    }
    
    return FWeaponData(); // Return default if not found
}
```

## Best Practices

1. **Separate Data from Logic**: Keep weapon data separate from weapon actors
2. **Use Unique IDs**: Assign unique IDs to items for database storage
3. **Validate on Load**: Always validate loaded data to prevent cheating
4. **Cache Frequently Used Data**: Don't query database on every attack
5. **Use Replication Carefully**: Only replicate what's necessary for gameplay
6. **Compress Network Traffic**: Use quantization for floating-point values
7. **Implement Rollback**: For client prediction and lag compensation

## Migration Path

To add persistence to the existing system:

1. **Phase 1**: Add save/load for local single-player
   - Implement SaveGame objects
   - Save on level transition/game exit
   - Load on game start

2. **Phase 2**: Add network replication
   - Mark properties for replication
   - Implement RPCs for attacks
   - Add client prediction

3. **Phase 3**: Add server-side persistence
   - Design database schema
   - Implement save on logout/periodic saves
   - Load on login

4. **Phase 4**: Add anti-cheat
   - Server-authoritative combat
   - Validate all client actions
   - Rate limiting and sanity checks

The current system is designed to make all these phases straightforward without requiring major refactoring.
