// Shoot Them Up , All Right Reserved


#include "Pickups/STUAmmoPickup.h"
#include "Components/STUHealthComponent.h"//����������ڼ���ɫ�Ƿ���
#include "Components/STUWeaponComponent.h"
#include "STUUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogAmmoPickup, All, All);

bool ASTUAmmoPickup::GivePickupTo(APawn* PlayerPawn)
{
	const auto HealthComponent = STUUtils::GetSTUPlayerComponent<USTUHealthComponent>(PlayerPawn);
	if (!HealthComponent || HealthComponent->IsDead())return false;
	//����ɫ�Ƿ���

	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(PlayerPawn);
	if (!WeaponComponent) return false;

	return WeaponComponent->TryToAddAmmo(WeaponType,ClipsAmount);
}