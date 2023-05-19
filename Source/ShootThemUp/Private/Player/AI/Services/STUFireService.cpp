// Shoot Them Up , All Right Reserved


#include "Player/AI/Services/STUFireService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"
#include "Components/STUWeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFireService,All,All)

USTUFireService::USTUFireService()
{
	NodeName = "Fire";
}

void USTUFireService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

	const auto Controller = OwnerComp.GetAIOwner();//获取AI控制器

	if (Controller)
	{
		//const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);//获取控制器感知组件
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());//获取控制器对应棋子的武器组件
		
		//if (PerceptionComponent->GetClosestEnemy())WeaponComponent->StopFire(); return;

		if (WeaponComponent)
		{
			WeaponComponent->StartFire();
			//UE_LOG(LogFireService, Display, TEXT("Fire"));
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}