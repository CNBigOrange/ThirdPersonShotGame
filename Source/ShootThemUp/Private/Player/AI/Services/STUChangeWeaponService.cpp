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
	const auto Controller = OwnerComp.GetAIOwner();//ͨ����Ϊ���Դ��������AI������
	if (Controller)
	{
		const auto WeaponComponent = STUUtils::GetSTUPlayerComponent<USTUWeaponComponent>(Controller->GetPawn());//��ÿ����������Ӷ�Ӧ���Controller->GetPawn()
		if (WeaponComponent && FMath::FRand()< Probability && Probability > 0)//FMath::FRand()����0~1�е����������
		{
			WeaponComponent->NextWeapon();
		}
	}

Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}