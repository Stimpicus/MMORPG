# Combat System Examples

This directory contains example implementations showing how to use the combat and abilities system.

## Example Classes

### CombatCharacter

A complete character class that demonstrates integration of the combat system:

**Features:**
- Integrates both `ResourceComponent` and `CombatComponent`
- Provides methods for executing different attack types
- Handles weapon equipping
- Event handlers for damage, attacks, and resource changes
- Target selection system

**Usage in Blueprint:**
1. Create a Blueprint based on `CombatCharacter`
2. Configure default resources in the Details panel
3. Bind input actions to `PerformMeleeAttack`, `PerformRangedAttack`, `PerformMagicalAttack`
4. Use `SetCurrentTarget` to select an enemy before attacking

**Code Example:**
```cpp
// In your player controller or game mode
ACombatCharacter* Player = Cast<ACombatCharacter>(GetPawn());
if (Player)
{
    // Set up a weapon
    FWeaponData Sword;
    Sword.WeaponName = "Iron Sword";
    Sword.WeaponType = EWeaponType::Melee;
    Sword.BaseDamage = 15.0f;
    Player->EquipWeapon(Sword);
    
    // Set target and attack
    Player->SetCurrentTarget(EnemyActor);
    Player->PerformMeleeAttack();
}
```

### Example Weapons

Pre-configured weapon classes that can be spawned or used as templates:

#### IronSword
- **Type:** Melee
- **Damage:** 15
- **Range:** 150 units
- **Speed:** 1.2

#### Longbow
- **Type:** Ranged
- **Damage:** 18
- **Range:** 600 units
- **Speed:** 0.8

#### FireStaff
- **Type:** Magical
- **Damage:** 25
- **Range:** 450 units
- **Speed:** 0.7

#### Greatsword
- **Type:** Melee
- **Damage:** 35
- **Range:** 180 units
- **Speed:** 0.6 (slower but more powerful)

**Spawning Weapons:**
```cpp
// Spawn a weapon in the world
AFireStaff* Staff = GetWorld()->SpawnActor<AFireStaff>(
    AFireStaff::StaticClass(), 
    SpawnLocation, 
    FRotator::ZeroRotator
);

// Get its weapon data
FWeaponData StaffData = Staff->GetWeaponData();

// Equip it to a character
CombatCharacter->EquipWeapon(StaffData);
```

## Creating Custom Weapons

### Method 1: C++ Class

```cpp
UCLASS(Blueprintable)
class MMORPG_API AMyCustomWeapon : public AWeaponItem
{
    GENERATED_BODY()

public:
    AMyCustomWeapon()
    {
        WeaponData.WeaponName = "My Custom Weapon";
        WeaponData.WeaponType = EWeaponType::Magical;
        WeaponData.BaseDamage = 40.0f;
        WeaponData.AttackRange = 300.0f;
        WeaponData.AttackSpeed = 1.0f;
    }
};
```

### Method 2: Blueprint

1. Create a Blueprint based on `WeaponItem` or any example weapon class
2. In the Details panel, configure the Weapon Data properties
3. Add a static mesh to the `WeaponMesh` component
4. Save and use in your game

### Method 3: Data-Driven

```cpp
// Create weapon data at runtime
FWeaponData CreateCustomWeapon()
{
    FWeaponData Weapon;
    Weapon.WeaponName = "Runtime Weapon";
    Weapon.WeaponType = EWeaponType::Ranged;
    Weapon.BaseDamage = 22.0f;
    Weapon.AttackRange = 500.0f;
    Weapon.AttackSpeed = 1.1f;
    return Weapon;
}
```

## Creating Custom Abilities

You can create custom abilities by modifying the ability data:

```cpp
// Create a powerful magical attack
FAttackAbilityData Fireball;
Fireball.AbilityName = "Fireball";
Fireball.AttackType = EAttackType::MagicalAttack;
Fireball.Damage = 50.0f;
Fireball.Range = 500.0f;
Fireball.Cooldown = 5.0f;

// Add multiple resource costs
FResourceCost ManaCost;
ManaCost.ResourceType = EResourceType::Mana;
ManaCost.Amount = 40.0f;
Fireball.ResourceCosts.Add(ManaCost);

FResourceCost StaminaCost;
StaminaCost.ResourceType = EResourceType::Stamina;
StaminaCost.Amount = 10.0f;
Fireball.ResourceCosts.Add(StaminaCost);

// Set on combat component
CombatComponent->MagicalAbility = Fireball;
```

## Event Handling

The combat system provides several events you can bind to:

### Resource Change Events
```cpp
void AMyCharacter::BeginPlay()
{
    Super::BeginPlay();
    
    if (ResourceComponent)
    {
        ResourceComponent->OnResourceChanged.AddDynamic(
            this, 
            &AMyCharacter::HandleResourceChange
        );
    }
}

void AMyCharacter::HandleResourceChange(
    EResourceType ResourceType, 
    float CurrentValue, 
    float MaxValue)
{
    // Update UI, trigger effects, etc.
    if (ResourceType == EResourceType::Health && CurrentValue < MaxValue * 0.2f)
    {
        // Play low health warning
    }
}
```

### Attack Executed Events
```cpp
void AMyCharacter::HandleAttackExecuted(
    EAttackType AttackType, 
    AActor* Target, 
    float Damage)
{
    // Play attack animation, spawn projectiles, etc.
    if (AttackType == EAttackType::RangedAttack)
    {
        // Spawn arrow projectile
    }
}
```

### Damage Taken Events
```cpp
void AMyCharacter::HandleDamageTaken(AActor* Instigator, float Damage)
{
    // Play hit reaction, spawn damage numbers, etc.
    
    // Check for death
    if (ResourceComponent->GetCurrentResource(EResourceType::Health) <= 0.0f)
    {
        Die();
    }
}
```

## Blueprint Integration

All components and functions are exposed to Blueprints:

1. **Add Components in Blueprint:**
   - Open your Character Blueprint
   - Add Component → Resource Component
   - Add Component → Combat Component

2. **Call Functions from Blueprint:**
   - All `UFUNCTION(BlueprintCallable)` functions are available
   - Use `Execute Melee Attack`, `Execute Ranged Attack`, etc.

3. **Listen to Events:**
   - Select a component
   - In the Event Graph, add events like `On Resource Changed`
   - Implement your custom logic

## Testing the System

Simple test scenario:

```cpp
// Create two characters
ACombatCharacter* Player = SpawnCharacter();
ACombatCharacter* Enemy = SpawnCharacter();

// Equip weapons
FWeaponData Sword;
Sword.WeaponName = "Test Sword";
Sword.WeaponType = EWeaponType::Melee;
Sword.BaseDamage = 20.0f;
Player->EquipWeapon(Sword);

// Position them in range
Player->SetActorLocation(FVector(0, 0, 0));
Enemy->SetActorLocation(FVector(100, 0, 0)); // 100 units away

// Execute attack
Player->SetCurrentTarget(Enemy);
Player->PerformMeleeAttack();

// Check results
float EnemyHealth = Enemy->ResourceComponent->GetCurrentResource(EResourceType::Health);
// Should be reduced by ~35 (20 base + 15 weapon damage)
```

## Next Steps

1. **Animation System:** Bind attack events to animation montages
2. **Projectile System:** Create projectile actors for ranged/magical attacks
3. **Visual Effects:** Add particle systems to attack execution
4. **Sound Effects:** Add audio cues for attacks and damage
5. **UI System:** Create health/mana/stamina bars
6. **Networking:** Add replication for multiplayer
7. **Persistence:** Save/load character stats and equipped weapons
8. **AI Integration:** Create AI controllers that use the combat system
