// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STUFindEnemyService.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUFindEnemyService : public UBTService
{
	GENERATED_BODY()
	
public:
	USTUFindEnemyService();//���캯��

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector EnemyActorKey;//�ڰ�ѡ����������

		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
