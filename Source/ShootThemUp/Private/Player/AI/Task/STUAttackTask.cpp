// Shoot Them Up , All Right Reserved


#include "Player/AI/Task/STUAttackTask.h"
#include "Components/STUWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"//�ڰ����ͷ�ļ�
#include "AIController.h"//AI������ͷ�ļ�

USTUAttackTask::USTUAttackTask()
{
	NodeName = "Attack Node";
}

EBTNodeResult::Type USTUAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	return EBTNodeResult::Succeeded;
}