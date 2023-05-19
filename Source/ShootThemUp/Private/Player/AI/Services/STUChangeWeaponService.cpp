// Shoot Them Up , All Right Reserved


#include "Player/AI/Services/STUChangeWeaponService.h"
#include "Components/STUWeaponComponent.h"
#include "AIController.h"
#include "STUUtils.h"

USTUChangeWeaponService::USTUChangeWeaponService()
{
	NodeName = "Change Weapon";
}

void USTUChangeWeaponService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();//通过行为树自带函数获得AI控制器
	if (Controller)
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());//获得控制器的棋子对应组件Controller->GetPawn()
		if (WeaponComponent && FMath::FRand()< Probability && Probability > 0)//FMath::FRand()生成0~1中的随机浮点数
		{
			WeaponComponent->NextWeapon();
		}
	}

Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}