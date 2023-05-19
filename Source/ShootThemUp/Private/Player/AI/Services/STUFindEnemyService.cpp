// Shoot Them Up , All Right Reserved


#include "Player/AI/Services/STUFindEnemyService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "STUUtils.h"
#include "Components/STUAIPerceptionComponent.h"
#include "Components/STUHealthComponent.h"

USTUFindEnemyService::USTUFindEnemyService()
{
	NodeName = "Find Enemy";//�ڵ�����
}

void USTUFindEnemyService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	
	//UE_LOG(LogTemp,Display,TEXT("Find enemy"))
	const auto Blackboard = OwnerComp.GetBlackboardComponent();//��ȡ�ڰ����
	if (Blackboard)
	{
		const auto Controller = OwnerComp.GetAIOwner();//��ȡAI������
		const auto PerceptionComponent = STUUtils::GetSTUPlayerComponent<USTUAIPerceptionComponent>(Controller);//��ȡ��������֪���
		if (PerceptionComponent)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, PerceptionComponent->GetClosestEnemy());//���úڰ���ԿΪ��֪�����ȡ��������ˣ�����һ���ڰ���Կ���ƣ�����������Կֵ
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}