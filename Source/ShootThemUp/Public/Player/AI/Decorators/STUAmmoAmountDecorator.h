// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STUAmmoAmountDecorator.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUAmmoAmountDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	USTUAmmoAmountDecorator();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		int32 ClipsAmount = 2;

	//计算原始条件值
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;
};
