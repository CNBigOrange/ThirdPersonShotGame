// Shoot Them Up , All Right Reserved


#include "Player/AI/Decorators/STUAmmoAmountDecorator.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUWeaponComponent.h"

USTUAmmoAmountDecorator::USTUAmmoAmountDecorator()
{
	NodeName = "AmmoAmount";
}

bool USTUAmmoAmountDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const auto Controller = OwnerComp.GetAIOwner();
	if (!Controller)return false;

	const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());
	if (!WeaponComponent)return false;

	return WeaponComponent->GetCurrentWeaponForAI()->GetAmmoData().Clips <= ClipsAmount;
}