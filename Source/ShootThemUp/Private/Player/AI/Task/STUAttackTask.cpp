// Shoot Them Up , All Right Reserved


#include "Player/AI/Task/STUAttackTask.h"
#include "Components/STUWeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"//黑板组件头文件
#include "AIController.h"//AI控制器头文件

USTUAttackTask::USTUAttackTask()
{
	NodeName = "Attack Node";
}

EBTNodeResult::Type USTUAttackTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	return EBTNodeResult::Succeeded;
}