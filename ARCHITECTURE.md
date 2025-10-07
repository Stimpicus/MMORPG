# Combat System Architecture

```
┌─────────────────────────────────────────────────────────────────┐
│                      Combat System Overview                      │
└─────────────────────────────────────────────────────────────────┘

┌──────────────────┐
│  CombatCharacter │  (Example Implementation)
└────────┬─────────┘
         │
         ├─────────────────────┐
         │                     │
         ▼                     ▼
┌────────────────────┐  ┌───────────────────┐
│ ResourceComponent  │  │  CombatComponent  │
└────────┬───────────┘  └────────┬──────────┘
         │                       │
         │                       │
         ▼                       ▼
┌─────────────────────────────────────────┐
│           CombatTypes.h                 │
│  ┌─────────────────────────────────┐   │
│  │ Enums:                          │   │
│  │  - EWeaponType                  │   │
│  │  - EAttackType                  │   │
│  │  - EResourceType                │   │
│  │                                 │   │
│  │ Structs:                        │   │
│  │  - FWeaponData                  │   │
│  │  - FAttackAbilityData           │   │
│  │  - FResourceCost                │   │
│  └─────────────────────────────────┘   │
└─────────────────────────────────────────┘
                    │
                    ▼
         ┌──────────────────┐
         │   WeaponItem     │
         └──────────────────┘
                    │
                    ├────────────┬──────────────┬─────────────┐
                    ▼            ▼              ▼             ▼
            ┌──────────┐  ┌──────────┐  ┌──────────┐  ┌──────────┐
            │IronSword │  │ Longbow  │  │FireStaff │  │Greatsword│
            └──────────┘  └──────────┘  └──────────┘  └──────────┘


Component Interaction Flow:
═══════════════════════════

Player Input → CombatCharacter
                    │
                    ├→ SetCurrentTarget(Enemy)
                    │
                    └→ PerformMeleeAttack()
                              │
                              ▼
                    ┌─────────────────────┐
                    │  CombatComponent    │
                    └──────────┬──────────┘
                               │
                    ┌──────────┴──────────┐
                    │                     │
                    ▼                     ▼
        ┌───────────────────┐   ┌─────────────────┐
        │ Check Cooldown    │   │ Check Range     │
        └───────────────────┘   └─────────────────┘
                    │                     │
                    └──────────┬──────────┘
                               ▼
                    ┌──────────────────────┐
                    │  ResourceComponent   │
                    │  Check Resources     │
                    └──────────┬───────────┘
                               │
                    ┌──────────┴──────────┐
                    │                     │
                    ▼                     ▼
        ┌───────────────────┐   ┌─────────────────┐
        │ Consume Resources │   │ Apply Cooldown  │
        └───────────────────┘   └─────────────────┘
                    │                     │
                    └──────────┬──────────┘
                               ▼
                    ┌──────────────────────┐
                    │  Calculate Damage    │
                    │  (Base + Weapon)     │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │  Apply to Target     │
                    └──────────┬───────────┘
                               │
                               ▼
                    ┌──────────────────────┐
                    │  Broadcast Events    │
                    │  - OnAttackExecuted  │
                    │  - OnDamageTaken     │
                    │  - OnResourceChanged │
                    └──────────────────────┘


Resource Management:
═══════════════════

┌────────────────────────────────────────┐
│         ResourceComponent              │
├────────────────────────────────────────┤
│                                        │
│  ┌──────────────────────────────────┐ │
│  │  Health:  [████████░░] 80/100   │ │
│  │  Mana:    [██████████] 100/100  │ │
│  │  Stamina: [████░░░░░░] 40/100   │ │
│  └──────────────────────────────────┘ │
│                                        │
│  Methods:                              │
│  ├─ GetCurrentResource()               │
│  ├─ ConsumeResource()                  │
│  ├─ RestoreResource()                  │
│  ├─ HasEnoughResource()                │
│  └─ ConsumeResources()                 │
│                                        │
│  Events:                               │
│  └─ OnResourceChanged                  │
│     (for UI updates)                   │
└────────────────────────────────────────┘


Attack Types:
════════════

┌─────────────────────────────────────────────────────────┐
│                    Melee Attack                         │
├─────────────────────────────────────────────────────────┤
│ Range: 150 units | Cooldown: 1s | Cost: 10 Stamina     │
│ Damage: 15 + Weapon Bonus                              │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                   Ranged Attack                         │
├─────────────────────────────────────────────────────────┤
│ Range: 500 units | Cooldown: 1.5s | Cost: 15 Stamina   │
│ Damage: 20 + Weapon Bonus                              │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│                  Magical Attack                         │
├─────────────────────────────────────────────────────────┤
│ Range: 400 units | Cooldown: 2s | Cost: 25 Mana        │
│ Damage: 30 + Weapon Bonus                              │
└─────────────────────────────────────────────────────────┘


Weapon Integration:
══════════════════

        ┌──────────────────────┐
        │    FWeaponData       │
        ├──────────────────────┤
        │ - WeaponName         │
        │ - WeaponType         │
        │ - BaseDamage         │
        │ - AttackRange        │
        │ - AttackSpeed        │
        └──────────┬───────────┘
                   │
         ┌─────────┴─────────┐
         │                   │
         ▼                   ▼
    ┌─────────┐        ┌─────────┐
    │ Melee   │        │ Ranged  │
    │ Range:  │        │ Range:  │
    │ 150     │        │ 500     │
    └─────────┘        └─────────┘
         │                   │
         ▼                   ▼
    ┌─────────┐        ┌─────────┐
    │ Magical │        │ Sets    │
    │ Range:  │        │ Ability │
    │ 400     │        │ Range   │
    └─────────┘        └─────────┘


Extensibility Points:
════════════════════

1. Custom Abilities:
   └─ Create FAttackAbilityData instances
      └─ Configure damage, range, cooldown, costs

2. Status Effects:
   └─ Extend CombatComponent
      └─ Add effect tracking
         └─ Modify damage calculations

3. Combo System:
   └─ Track attack sequence
      └─ Trigger special abilities
         └─ Reset on timeout

4. Character Stats:
   └─ Add stats component
      └─ Modify damage/costs
         └─ Scale with level

5. Network Replication:
   └─ Mark properties for replication
      └─ Implement RPCs
         └─ Add client prediction

6. Persistence:
   └─ Serialize data structures
      └─ Save to SaveGame/Database
         └─ Load on startup


Event System:
════════════

┌───────────────────────────────────────┐
│          Event Delegates              │
├───────────────────────────────────────┤
│                                       │
│  OnResourceChanged                    │
│  ├─ Update UI Bars                    │
│  ├─ Trigger Low Resource Warning      │
│  └─ Enable/Disable Abilities          │
│                                       │
│  OnAttackExecuted                     │
│  ├─ Play Attack Animation             │
│  ├─ Spawn Projectile/VFX              │
│  └─ Play Attack Sound                 │
│                                       │
│  OnDamageTaken                        │
│  ├─ Play Hit Reaction                 │
│  ├─ Spawn Damage Numbers              │
│  ├─ Play Hurt Sound                   │
│  └─ Check for Death                   │
│                                       │
└───────────────────────────────────────┘


Integration with Other Systems:
═══════════════════════════════

                ┌──────────────┐
                │Combat System │
                └──────┬───────┘
                       │
         ┌─────────────┼─────────────┐
         │             │             │
         ▼             ▼             ▼
    ┌────────┐   ┌─────────┐   ┌────────┐
    │Animation│  │  UI     │   │  Audio │
    │System  │   │ System  │   │ System │
    └────────┘   └─────────┘   └────────┘
         │             │             │
         │             │             │
         ▼             ▼             ▼
    ┌────────┐   ┌─────────┐   ┌────────┐
    │  VFX   │   │Inventory│   │Network │
    │System  │   │ System  │   │ System │
    └────────┘   └─────────┘   └────────┘
                       │
                       ▼
                 ┌──────────┐
                 │Persistence│
                 │  System  │
                 └──────────┘
```
