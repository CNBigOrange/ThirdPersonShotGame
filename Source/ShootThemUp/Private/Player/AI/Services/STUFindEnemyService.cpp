// Shoot Them Up , All Right Reserved


#include "Player/AI/Services/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"
#include "Components/STUHealthComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = "Find Enemy";//节点名字
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	//UE_LOG(LogTemp,Display,TEXT("Find enemy"))
	const auto Blackboard = OwnerComp.GetBlackboardComponent();//获取黑板组件
	if (Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();//获取AI控制器
		const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);//获取控制器感知组件
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());//设置黑板秘钥为感知组件获取的最近敌人，参数一：黑板秘钥名称，参数二：秘钥值
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}