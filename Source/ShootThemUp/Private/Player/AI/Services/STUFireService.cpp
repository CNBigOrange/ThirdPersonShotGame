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

	const auto Controller = OwnerComp.GetAIOwner();//��ȡAI������

	if (Controller)
	{
		//const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);//��ȡ��������֪���
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());//��ȡ��������Ӧ���ӵ��������
		
		//if (PerceptionComponent->GetClosestEnemy())WeaponComponent->StopFire(); return;

		if (WeaponComponent)
		{
			WeaponComponent->StartFire();
			//UE_LOG(LogFireService, Display, TEXT("Fire"));
		}
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}