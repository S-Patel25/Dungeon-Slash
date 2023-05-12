#pragma once


UENUM(BlueprintType) //uenum exposes enum to refleciton system, blueprinttype allows enums to be used in blueprints
enum class ECharacterState : uint8 //enum is a special data type that holds integer constants (but can be represented as var names too), typically used to determine states. for UE, E letter is needed before to classify as enum, and we make it scoped (use class keyword) to avoid ambiguity. also we put a prefix before enum state name for more clarity
{
	ECS_Unequipped UMETA(DisplayeName = "Unequipped"),
	ECS_EquippedOneHandWeapon UMETA(DisplayName = "Equipped One-Handed Weapon"), //using UMETA macro to display names in blueprint in a better way (looks better then having ECS_Equipped in bp)
	ECS_EquippedTwoHandWeapon UMETA(DisplayName = "Equipped Two-Handed Weapon"),
};

UENUM(BlueprintType)
enum class EActionState : uint8 //this enum is to keep track of the combat state of the player (is player attacking, doing something else, etc)
{
	EAS_Unoccupied UMETA(DispayName = "Unoccupied"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingOrUnequippingWeapon UMETA(DisplayName = "Equip or Unequip")
};