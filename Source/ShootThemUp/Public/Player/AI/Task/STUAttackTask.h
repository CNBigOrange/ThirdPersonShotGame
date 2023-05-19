// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STUAttackTask.generated.h"


UCLASS()
class SHOOTTHEMUP_API USTUAttackTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	USTUAttackTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector AttackActorKey;//ºÚ°åÃØÔ¿Ñ¡ÔñÆ÷
};
