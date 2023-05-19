// Shoot Them Up , All Right Reserved

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STUNextLocationTask.generated.h"

UCLASS()
class SHOOTTHEMUP_API USTUNextLocationTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	USTUNextLocationTask();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Radius = 1000.0f;//AIÑ°Â·°ë¾¶

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		FBlackboardKeySelector AimLocationKey;//ºÚ°åÃØÔ¿Ñ¡ÔñÆ÷ Ãé×¼Î»ÖÃ

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool SelfCenter = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI",meta = (EditCondition = "!SelfCenter"))
		FBlackboardKeySelector CenterActorKey;//ºÚ°åÃØÔ¿Ñ¡ÔñÆ÷ ÖÐÐÄ½ÇÉ«
};
